#include "MoveAgent.h"

Move_Branch::Move_Branch(float prior_value) : prior(prior_value), visit_count(0), total_value(0.0), incomplete_visit(0) {}

Move_Branch::Move_Branch(Move_Branch&& other) noexcept
    : prior(other.prior),
    visit_count(other.visit_count.load()),
    incomplete_visit(other.visit_count.load()),
    total_value(other.total_value) {
}

// 移动赋值操作符的实现
Move_Branch& Move_Branch::operator=(Move_Branch&& other) noexcept {
    if (this != &other) {
        prior = other.prior;
        visit_count.store(other.visit_count.load());
        incomplete_visit.store(other.incomplete_visit.load());
        total_value = other.total_value;
    }
    return *this;
}

Move_TreeNode::Move_TreeNode(std::shared_ptr<GameState> state_value, float value_value, const std::map<int, float>& priors, std::shared_ptr<Move_TreeNode> parent_value, int last_move_value)
    : state(state_value), value(value_value), parent(parent_value), last_move(last_move_value), total_visit_count(1) {
    Board& board = state->board;
    MoveEncoder ed(std::pair(14, 14), 3);
    std::vector<std::vector<int>> legal_moves;
    /*legal_moves = walk_strategy2(*state);*/
    if (state->next_player == Player::BLACK) {
        legal_moves = board.cut_legal_moves(state->next_player);
    }
    else
    {
        legal_moves = board.legal_moves(state->next_player, state->state);
    }
    /*legal_moves = board.cut_legal_moves(state->next_player);*/
    /*legal_moves = board.legal_moves(state->next_player, state->state);*/
    /*state->board.displayGrid();*/
    /*std::vector<std::vector<int>> legal_moves = walk_strategy1(*state);*/
    /*std::cout << "asd"<<legal_moves.size() << std::endl;*/
    //if (legal_moves.empty()) {
    //    legal_moves = board.legal_moves(state->next_player, state->state);
    //}
    for (const auto& move : legal_moves) {

        int encoded_move = ed.encode_move(move);
        auto it = priors.find(encoded_move);
        if (it != priors.end()) {
            branches[encoded_move] = Move_Branch(it->second);
        }
    }
    //for (auto& move_prior : priors) {
    //    int move = move_prior.first; 
    //    if (state->board.is_valid_move_layout(move)) {
    //        branches[move] = Move_Branch(move_prior.second);
    //    }
    //}
}

std::vector<int> Move_TreeNode::moves() {
    std::vector<int> moves;
    moves.reserve(10400);
    for (const auto& branch : branches) {
        moves.emplace_back(branch.first);
    }
    return moves;
}

void Move_TreeNode::add_child(int move, std::shared_ptr<Move_TreeNode> child_node) {
    children[move] = child_node;
}

bool Move_TreeNode::has_child(int move) {
    return children.find(move) != children.end();
}

std::shared_ptr<Move_TreeNode> Move_TreeNode::get_child(int move) {
    return children.at(move);
}

void Move_TreeNode::complete_visit(int move, float value) {
    {
        std::lock_guard<std::mutex> lock1(data_lock);
        /*total_visit_count++;*/
        //total_incomplete_visit--;
        //branches[move].incomplete_visit--;
        branches[move].visit_count++;
        branches[move].total_value += value;
    }

}

void Move_TreeNode::incomplete_visit(int move) {
    {
        std::lock_guard<std::mutex> lock1(data_lock);
        branches[move].incomplete_visit++;
        /*total_incomplete_visit++;*/
        total_visit_count++;

    }
}

float Move_TreeNode::expected_value(int move) {
    Move_Branch& branch = branches[move];
    if (branch.visit_count == 0) {
        return 0.0;
    }
    return branch.total_value / static_cast<float>(branch.visit_count);
}

float Move_TreeNode::prior(int move) {
    return branches[move].prior;
}

int Move_TreeNode::visit_count(int move) {
    return branches.count(move) > 0 ? branches[move].incomplete_visit.load() : 0;
}

//int Move_TreeNode::incomplete_visit(Point move) {
//    return branches.count(move) > 0 ? branches[move].visit_count.load() : 0;
//}

Move_Agent::Move_Agent(Net* net, const std::string& model_path, int rounds_per_move, float c_value)
    :net(net),
    /*net1(net),*/
    encoder(std::make_shared<MoveEncoder>(std::make_pair(14, 14), 3)),
    encoder1(std::make_shared<MoveEncoder>(std::make_pair(14, 14), 5)),
    num_rounds(rounds_per_move),
    c(c_value){
    module = torch::jit::load(model_path);
    module.eval();
    module.to(at::kCUDA);
}

//Move_Agent::Move_Agent(int rounds_per_move, float c_value)
//    : encoder(std::make_shared<MoveEncoder>(std::make_pair(14, 14))), num_rounds(rounds_per_move), c(c_value) {}


std::tuple<std::vector<int>, std::vector<int>> Move_Agent::select_move(std::shared_ptr<GameState> game_state) {

   
    std::shared_ptr<Move_TreeNode> root = nullptr;
    root = p_create_node(game_state);
    /*game_state->board.displayGrid();*/
    std::vector<std::future<void>> futures;
    for (int i = 0; i < num_rounds; ++i) {
        auto future =
            thread_pool.commit(std::bind(&Move_Agent::simulate_round, this, root));
        futures.emplace_back(std::move(future));
    }
    for (unsigned int i = 0; i < futures.size(); i++)
        futures[i].wait();
    torch::Tensor root_state_tensor = encoder->encode(*game_state);
    // 获取访问次数
    std::vector<int> visit_counts;
    visit_counts.reserve(10400);
    for (int idx = 0; idx < encoder->num_points(); ++idx) {
        visit_counts.emplace_back(root->visit_count(idx));
    }
    //int sum = std::accumulate(visit_counts.begin(), visit_counts.end(), 0);

    //// 输出和
    //std::cout << "The sum of visit_counts is: " << sum << std::endl;
    std::vector<int> moves = root->moves();

    std::vector<double> probs(visit_counts.size());
    double sum = 0.0;
    /*double temperature = 1.0;
    if (game_state->step > 30) {
        temperature = 0.1;
    }*/
    double temperature = 0.1;
    // Apply temperature and calculate sum of exp values
    for (size_t i = 0; i < visit_counts.size(); ++i) {
        probs[i] = std::pow(static_cast<double>(visit_counts[i]), 1.0 / temperature);
        sum += probs[i];
    }

    // Normalize to get probability distribution
    for (size_t i = 0; i < probs.size(); ++i) {
        probs[i] /= sum;
    }

    std::default_random_engine generator(std::random_device{}());
    std::discrete_distribution<int> distribution(probs.begin(), probs.end());

    int random_index = distribution(generator);
    std::vector<int> move1 = { random_index };
    std::vector<int> move2 = { root->eat_moves[random_index] };
    return  {move1,move2};
}
std::mutex output_mutex2;
void Move_Agent::simulate_round(std::shared_ptr<Move_TreeNode> root) {
    /*std::cout << "开始" << std::endl;*/
    auto node = root;
    int next_move = select_branch(node);
    node->incomplete_visit(next_move);
    while (node->has_child(next_move)) {
        node->incomplete_visit(next_move);
        auto node1 = node->get_child(next_move);
        if (node1->moves().empty()) {
            break;
        }
        node = node1;
        next_move = select_branch(node);
    }
    int move = 0;
    float value = 0;
    
    if (node->has_child(next_move)) {
        /*std::vector<int> next_move1 = { next_move };*/
        move = next_move;
        value = node->value;
    }
    else
    {
        GameState news = *node->state;
        Player pl = news.next_player;
        //std::vector<int> next_move1;
        //std::lock_guard<std::mutex> guard(output_mutex2); 
        //{
        //    pl = news.next_player;
        //    news = *node->state;
        //}
        Board new_board1 = news.board;
        std::vector<int> next_move1 = encoder->decode_move_index(next_move);
        std::vector<std::vector<int>> legal_moves = new_board1.singal_moves(pl, next_move1[0]);
        for (const auto& move5 : legal_moves) {
            if (!move5.empty() && move5.back() == next_move1[1]) {
                // 如果移动的起始和结束位置与 eat_moves 匹配，添加到结果中
                next_move1 = move5;
                break;
            }
        }
        
        /*Board new_board1 = node->state->board;*/
        int s_nums = 0;
        new_board1.go_take_stone(pl, next_move1);
        s_nums = new_board1.squire(pl, next_move1.back());


        //if (s_nums > 0) {
        //    std::cout << s_nums << std::endl;
        //    node->state->board.displayGrid();
        //    /*new_board1.displayGrid();*/
        //    news.board.displayGrid();
        //    news.board.Print_move(next_move1, {}, pl);
        //}

        std::vector<int> next_eat1 = {};
        if (s_nums > 0) {
            auto state_tensor = encoder1->eat_encode(news, next_move1);
            std::vector<int> eatmoves;
            if (pl == Player::BLACK) {
                /*node->state->board.Print_move(next_move1, {}, Player::WHITE);*/
                eatmoves = eat_strategy3(new_board1, s_nums, pl);
                std::map<int, float> move_priors;
                for (int eat_move : eatmoves) {
                    torch::Tensor move_tensor = torch::tensor({ eat_move }, torch::kLong).to(torch::kCUDA);
                    std::vector<torch::jit::IValue> inputs;
                    //std::cout << move_tensor << std::endl;
                    //std::cout << move_tensor.sizes() << std::endl;
                    inputs.push_back(state_tensor.to(torch::kCUDA));
                    inputs.push_back(move_tensor);
                    auto output = module.forward(inputs);
                    auto value_output = output.toTensor().to(at::kCPU);
                    move_priors[eat_move] = value_output.item<float>();
                }
                auto max_it = std::max_element(move_priors.begin(), move_priors.end(),
                    [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
                        return a.second < b.second;
                    });
                node->eat_moves[next_move] = max_it->first;
                next_eat1 = encoder->decode_eat_index(max_it->first);
                std::tuple<std::vector<int>, std::vector<int>> moves = std::make_tuple(next_move1, next_eat1);
                auto new_state = std::make_shared<GameState>(news.apply_move_mcts(moves));

                std::shared_ptr<Move_TreeNode> child_node = nullptr;
                child_node = p_create_node(new_state);
                move = next_move;
                float value = -child_node->value;
            }
            else {
                ExpertAgent agent;
                Board nboard = news.board;
                for (int d = 0; d < s_nums; d++) {
                    auto et = agent.eat_strategy(nboard, pl);
                    eatmoves.emplace_back(et);
                    nboard.grid[et] = 0;
                }
                auto nn = encoder1->encode_eat(eatmoves);
                node->eat_moves[next_move] = nn;
                next_eat1 = eatmoves;
                std::tuple<std::vector<int>, std::vector<int>> moves = std::make_tuple(next_move1, next_eat1);
                auto new_state = std::make_shared<GameState>(news.apply_move_mcts(moves));
                std::shared_ptr<Move_TreeNode> child_node = nullptr;
                child_node = p_create_node(new_state);
                move = next_move;
                float value = -child_node->value;
            }
        }
        else
        {
            std::tuple<std::vector<int>, std::vector<int>> moves = std::make_tuple(next_move1, next_eat1);
            auto new_state = std::make_shared<GameState>(node->state->apply_move_mcts(moves));
            node->eat_moves[next_move] = 19306;
            std::shared_ptr<Move_TreeNode> child_node = nullptr;
            child_node = p_create_node(new_state);
            move = next_move;
            float value = -child_node->value;
        }
    }
    
    while (node != nullptr) {
        node->complete_visit(move, value);
        move = node->last_move;
        node = node->parent;
        value = -value;
    }
}


int Move_Agent::select_branch(std::shared_ptr<Move_TreeNode> node) {
    float max_score = -std::numeric_limits<float>::infinity();

    {
        if (node->moves().size() == 0) {
            std::cout << "越界" << std::endl;

        }
        int best_move = node->moves()[0];
        std::lock_guard<std::mutex> lock(node->data_lock);
        int total_n = node->total_visit_count;
        for (int move : node->moves()) {
            float q = node->expected_value(move);
            float p = node->prior(move);
            int n = node->visit_count(move);
            float score = q + c * p * std::sqrt(total_n) / (n + 1);
            //std::lock_guard<std::mutex> lock2(cout_lockt);
            //{
            //    std::cout << "Move: (" << move<< "), "
            //        << "Q-value: " << q << ", "
            //        << "Prior: " << p << ", "
            //        << "Visit count: " << n << ", "
            //        << "Total visit count: " << total_n << ", "
            //        << "UCT score: " << score << std::endl;
            //}
            /*std::cout << move <<":" << score << std::endl;*/

            if (score > max_score) {
                max_score = score;
                best_move = move;
            }
        }
        return best_move;

        /*int best_move = node->moves()[0];*/
        //std::lock_guard<std::mutex> lock(node->data_lock);
        //int total_n = node->total_ ;
        //for (int move : node->moves()) {
        //    float q = node->expected_value(move);
        //    float p = node->prior(move);
        //    int n = node->visit_count(move);
        //    float score = q + c * p * std::sqrt(total_n) / (n + 1);
        //    //std::lock_guard<std::mutex> lock2(cout_lockt);
        //    //{
        //    //    std::cout << "Move: (" << move.row << "," << move.col << "), "
        //    //        << "Q-value: " << q << ", "
        //    //        << "Prior: " << p << ", "
        //    //        << "Visit count: " << n << ", "
        //    //        << "Total visit count: " << total_n << ", "
        //    //        << "UCT score: " << score << std::endl;
        //    //}


        //    if (score > max_score) {
        //        max_score = score;
        //        best_move = move;
        //    }
        //}
        //return best_move;
    }
}

std::shared_ptr<Move_TreeNode> Move_Agent::create_node_dirichlet(std::shared_ptr<GameState> game_state, int move, std::shared_ptr<Move_TreeNode> parent) {
    auto state_tensor = encoder->encode(*game_state);

    std::map<int, float> move_priors;
    auto future = net->predict(state_tensor);
    auto outputs = future.get();
    auto policy_output = std::move(outputs.first);
    auto value_output = outputs.second;
    for (size_t idx = 0; idx < policy_output.size(); ++idx) {
        move_priors[idx] = policy_output[idx];
    }
    //std::cout << "Original Policy Probabilities:" << std::endl;
    //int count = 0;
    //for (const auto& kv : move_priors) {
    //    std::cout << "Move: " << kv.first << " " << std::fixed << std::setprecision(4) << kv.second << " ";
    //    if (++count % 14 == 0) {
    //        std::cout << std::endl;
    //    }
    //}
    std::cout << std::endl;
    float alpha = 0.03;
    float epsilon = 0.25;

    std::vector<float> noise(move_priors.size());
    std::default_random_engine generator(std::random_device{}());

    // 生成伽马分布噪声并计算总和
    std::gamma_distribution<float> distribution(alpha, 1.0);
    float sum = 0.0;
    for (size_t i = 0; i < noise.size(); ++i) {
        noise[i] = distribution(generator);
        sum += noise[i];
    }

    // 归一化噪声以模拟狄利克雷分布
    for (auto& n : noise) {
        n /= sum;
    }

    // 更新概率
    size_t index = 0;
    for (auto& kv : move_priors) {
        float dirichlet_noise = noise[index++];
        kv.second = kv.second * (1 - epsilon) + dirichlet_noise * epsilon;
    }
    // 输出添加噪声后的概率
  /*  std::cout << "Noisy Policy Probabilities:" << std::endl;
    count = 0;
    for (const auto& kv : move_priors) {
        std::cout << "Move: " << kv.first << " " << std::fixed << std::setprecision(4) << kv.second << " ";
        if (++count % 14 == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;*/
    //std::cout << "[";
    //for (size_t idx = 0; idx < move_priors.size(); ++idx) {
    //    std::cout << move_priors[idx];
    //    if (idx < move_priors.size() - 1) {
    //        std::cout << ", ";
    //    }
    //}
    //std::cout << "]" << std::endl;
    //auto max_it = std::max_element(move_priors.begin(), move_priors.end(),
    //    [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
    //        return a.second < b.second;
    //    });

    //if (max_it != move_priors.end()) {
    //    std::cout << "Move " << max_it->first << " has the highest probability: " << max_it->second << std::endl;
    //}

    auto new_node = std::make_shared<Move_TreeNode>(game_state, value_output, move_priors, parent, move);

    if (parent) {
        parent->add_child(move, new_node);
    }
    return new_node;
}




std::shared_ptr<Move_TreeNode> Move_Agent::create_node_huadong(std::shared_ptr<GameState> game_state, int move, std::shared_ptr<Move_TreeNode> parent) {
    auto state_tensor = encoder->encode(*game_state);

    std::map<int, float> move_priors;
    auto future = net->predict(state_tensor);

    auto outputs = future.get();
    auto policy_output = std::move(outputs.first);
    auto value_output = outputs.second;
    int count = 0;

    for (size_t idx = 0; idx < policy_output.size(); ++idx) {
        move_priors[idx] = policy_output[idx];
        /*std::cout << move_priors[idx] << std::endl;*/
    }

    double blockSum = 0.0;
    int centerRow = 7;
    int centerCol = 7;
    int blockSize = 0;
    int n = 14;
    if (game_state->step <= 3) {
        blockSize = 2;
    }
    else if (game_state->step > 3 && game_state->step <= 10) {
        blockSize = 4;
    }
    else if (game_state->step > 10 && game_state->step <= 24) {
        blockSize = 6;
    }
    else
    {
        blockSize = 8;
    }

    // 计算方块内的概率总和
    for (int i = std::max(0, centerRow - blockSize / 2); i < std::min(n - 1, centerRow + blockSize / 2); ++i) {
        for (int j = std::max(0, centerCol - blockSize / 2); j < std::min(n - 1, centerCol + blockSize / 2); ++j) {
            blockSum += move_priors[i * n + j];
            /*std::cout << move_priors[i * n + j] << std::endl;*/
        }
    }

    double otherSum = 1.0 - blockSum;
    double scaleFactor = 3;  // 你可以根据需求调整这个缩放因子

    // 放大方块内的概率
    for (int i = std::max(0, centerRow - blockSize / 2); i < std::min(n - 1, centerRow + blockSize / 2); ++i) {
        for (int j = std::max(0, centerCol - blockSize / 2); j < std::min(n - 1, centerCol + blockSize / 2); ++j) {
            move_priors[i * n + j] *= scaleFactor;
            /*std::cout << move_priors[i * n + j] << std::endl;*/
        }
    }

    // 调整后的方块总概率
    double scaledBlockSum = blockSum * scaleFactor;

    // 调整其他区域的概率，以保持总和为1
    for (int i = 0; i < move_priors.size(); ++i) {
        if (!(i / n >= centerRow - blockSize / 2 && i / n < centerRow + blockSize / 2 && i % n >= centerCol - blockSize / 2 && i % n < centerCol + blockSize / 2)) {
            move_priors[i] *= otherSum / (1.0 - blockSum + scaledBlockSum - blockSum);
        }
    }

    /*std::cout << "[";
    for (size_t idx = 0; idx < move_priors.size(); ++idx) {
        std::cout << move_priors[idx];
        if (idx < move_priors.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;*/
    auto new_node = std::make_shared<Move_TreeNode>(game_state, value_output, move_priors, parent, move);

    if (parent) {
        std::lock_guard<std::mutex> lock1(parent->data_lock);
        parent->add_child(move, new_node);
    }

    return new_node;
}



std::shared_ptr<Move_TreeNode> Move_Agent::p_create_node(std::shared_ptr<GameState> game_state, int move, std::shared_ptr<Move_TreeNode> parent) {
    auto state_tensor = encoder->encode(*game_state);

    std::map<int, float> move_priors;
    auto future = net->predict(state_tensor);
    auto outputs = future.get();
    auto policy_output = std::move(outputs.first);
    auto value_output = outputs.second;
    //if (game_state->next_player == Player::BLACK) {
    //    std::cout << value_output << std::endl;
    //}
    /*std::cout << value_output << std::endl;*/
    int count = 0;

    for (size_t idx = 0; idx < policy_output.size(); ++idx) {
        //if (policy_output[idx] < 0) {
        //    std::cout << "wwdwddwd" << std::endl;
        //}
        move_priors[idx] = policy_output[idx];
    }
    /*  std::cout << "[";
      for (size_t idx = 0; idx < move_priors.size(); ++idx) {
          std::cout << move_priors[idx];
          if (idx < move_priors.size() - 1) {
              std::cout << ", ";
          }
      }
      std::cout << "]" << std::endl;*/
    auto new_node = std::make_shared<Move_TreeNode>(game_state, value_output, move_priors, parent, move);

    if (parent) {
        std::lock_guard<std::mutex> lock1(parent->data_lock);
        parent->add_child(move, new_node);
    }
    return new_node;
    //auto state_tensor = encoder->encode(*game_state);
    //py::object result;
    //{
    //    py::gil_scoped_acquire acquire;
    //    result = model.attr("predict")(state_tensor);
    //}
    ////py::gil_scoped_acquire acquire;
    ////py::object result = model.attr("predict")(state_tensor);
    //
    //if (py::isinstance<py::list>(result)) {
    //    py::list result_list = result.cast<py::list>();

    //    // 处理第一个NumPy数组, 转换为 std::vector<float>
    //    py::array_t<float> priors_array = result_list[0].cast<py::array_t<float>>();
    //    auto priors_unchecked = priors_array.unchecked<2>(); // 假设数组是二维的
    //    std::vector<float> priors(priors_unchecked.shape(1)); // 获取列的数量
    //    for (ssize_t i = 0; i < priors_unchecked.shape(1); ++i) {
    //        priors[i] = priors_unchecked(0, i); // 第一维只有一个元素, 所以直接取0
    //    }

    //    // 处理第二个NumPy数组, 转换为 float
    //    py::array_t<float> values_array = result_list[1].cast<py::array_t<float>>();
    //    auto values_unchecked = values_array.unchecked<2>(); // 假设数组是二维的
    //    float values = values_unchecked(0, 0); // 直接取(0,0)位置的值

    //    
    //    std::map<Point, float> move_priors;
    //    for (size_t idx = 0; idx < priors.size(); ++idx) {
    //        move_priors[encoder->decode_point_index(idx)] = priors[idx];
    //    }

    //    auto new_node = std::make_shared<Move_TreeNode>(game_state, values, move_priors, parent, move);

    //    if (parent) {
    //        std::lock_guard<std::mutex> lock1(parent->data_lock); 
    //        parent->add_child(move, new_node);
    //    }
    //    return new_node;
    //}
    //float fixed_value = 0.5; // 假设的固定值估计
    //std::vector<float> fixed_priors(196, 1.0 / 196); // 假设每个位置的先验概率相同

    //std::map<Point, float> move_priors;
    //for (size_t idx = 0; idx < fixed_priors.size(); ++idx) {
    //    move_priors[encoder->decode_point_index(idx)] = fixed_priors[idx];
    //}

    //auto new_node = std::make_shared<Move_TreeNode>(game_state, fixed_value, move_priors, parent, move);

    //if (parent) {
    //    std::lock_guard<std::mutex> lock1(parent->data_lock);
    //    parent->add_child(move, new_node);
    //}
    //return new_node;

}



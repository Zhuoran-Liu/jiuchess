//#include "ParallelTreeNode.h"
//#include <algorithm>
//#include <cmath>
//
//P_Branch::P_Branch(float prior_value) : prior(prior_value), visit_count(0), total_value(0.0), incomplete_visit(0) {}
//
//P_Branch::P_Branch(P_Branch&& other) noexcept
//    : prior(other.prior),
//    visit_count(other.visit_count.load()),
//    incomplete_visit(other.visit_count.load()),
//    total_value(other.total_value) {
//}
//
//// 移动赋值操作符的实现
//P_Branch& P_Branch::operator=(P_Branch&& other) noexcept {
//    if (this != &other) {
//        prior = other.prior;
//        visit_count.store(other.visit_count.load());
//        incomplete_visit.store(other.incomplete_visit.load());
//        total_value = other.total_value;
//    }
//    return *this;
//}
//
//P_ZeroTreeNode::P_ZeroTreeNode(std::shared_ptr<GameState> state_value, float value_value, const std::map<int, float>& priors, std::shared_ptr<P_ZeroTreeNode> parent_value, int last_move_value)
//    : state(state_value), value(value_value), parent(parent_value), last_move(last_move_value), total_visit_count(1) {
//    for (auto& move_prior : priors) {
//        int move = move_prior.first;
//        if (state->board.is_valid_move_layout(move)) {
//            branches[move] = P_Branch(move_prior.second);
//        }
//    }
//}
//
//std::vector<int> P_ZeroTreeNode::moves() {
//    std::vector<int> moves;
//    moves.reserve(200);
//    for (const auto& branch : branches) {
//        moves.emplace_back(branch.first);
//    }
//    return moves;
//}
//
//void P_ZeroTreeNode::add_child(Point move, std::shared_ptr<P_ZeroTreeNode> child_node) {
//    children[move] = child_node;
//}
//
//bool P_ZeroTreeNode::has_child(Point move) {
//    return children.find(move) != children.end();
//}
//
//std::shared_ptr<P_ZeroTreeNode> P_ZeroTreeNode::get_child(Point move) {
//    return children.at(move);
//}
//
//void P_ZeroTreeNode::complete_visit(Point move, float value) {
//    {
//        std::lock_guard<std::mutex> lock1(data_lock);
//        /*total_visit_count++;*/
//        //total_incomplete_visit--;
//        //branches[move].incomplete_visit--;
//        branches[move].visit_count++;
//        branches[move].total_value += value;
//    }
//
//}
//
//void P_ZeroTreeNode::incomplete_visit(Point move) {
//    {
//        std::lock_guard<std::mutex> lock1(data_lock);
//        branches[move].incomplete_visit++;
//        /*total_incomplete_visit++;*/
//        total_visit_count++;
//
//    }
//}
//
//float P_ZeroTreeNode::expected_value(Point move) {
//    P_Branch& branch = branches[move];
//    if (branch.visit_count == 0) {
//        return 0.0;
//    }
//    return branch.total_value / static_cast<float>(branch.visit_count);
//}
//
//float P_ZeroTreeNode::prior(Point move) {
//    return branches[move].prior;
//}
//
//int P_ZeroTreeNode::visit_count(Point move) {
//    return branches.count(move) > 0 ? branches[move].incomplete_visit.load() : 0;
//}
//
////int P_ZeroTreeNode::incomplete_visit(Point move) {
////    return branches.count(move) > 0 ? branches[move].visit_count.load() : 0;
////}
//
//P_ZeroAgent::P_ZeroAgent(const std::string& model_path, int rounds_per_move, float c_value)
//    : encoder(std::make_shared<LayoutEncoder>(std::make_pair(14, 14))),
//    num_rounds(rounds_per_move),
//    c(c_value) {
//    try {
//        // 加载PyTorch模型
//        model = torch::jit::load(model_path);
//        std::cout << "sda" << std::endl;
//    }
//    catch (const c10::Error& e) {
//        std::cerr << "Error loading the model: " << e.what() << std::endl;
//        std::exit(EXIT_FAILURE);
//    }
//    // ...可能还有其他初始化代码
//}
//
////P_ZeroAgent::P_ZeroAgent(int rounds_per_move, float c_value)
////    : encoder(std::make_shared<LayoutEncoder>(std::make_pair(14, 14))), num_rounds(rounds_per_move), c(c_value) {}
//
//
//std::vector<Point> P_ZeroAgent::select_move(std::shared_ptr<GameState> game_state) {
//    //std::ofstream logFile("logfile.txt");
//
//    //// 将std::cout重定向到日志文件
//    //std::streambuf* originalCoutBuffer = std::cout.rdbuf(logFile.rdbuf()); 
//    auto root = create_node(game_state);
//    /*game_state->board.displayGrid();*/
//    std::vector<std::future<void>> futures;
//    for (int i = 0; i < num_rounds; ++i) {
//        auto future =
//            thread_pool.commit(std::bind(&P_ZeroAgent::simulate_round, this, root));
//        futures.emplace_back(std::move(future));
//    }
//    for (unsigned int i = 0; i < futures.size(); i++)
//        futures[i].wait();
//
//    std::vector<Point> moves = root->moves();
//    std::vector<Point> move = { *std::max_element(moves.begin(), moves.end(),
//        [&root](const Point& a, const Point& b) {
//            return root->visit_count(a) < root->visit_count(b);
//        }) };
//    return move;
//}
//
//void P_ZeroAgent::simulate_round(std::shared_ptr<P_ZeroTreeNode> root) {
//    auto node = root;
//    Point next_move = select_branch(node);
//    node->incomplete_visit(next_move);
//    while (node->has_child(next_move)) {
//        node = node->get_child(next_move);
//        next_move = select_branch(node);
//        node->incomplete_visit(next_move);
//    }
//
//    std::vector<Point> next_move1 = { next_move };
//    auto new_state = std::make_shared<GameState>(node->state->go_apply_move(next_move1));
//    //{
//    //    std::lock_guard<std::mutex> lock2(cout_lock);
//    //    std::cout << node->state->next_player << " point " << next_move.row << ", " << next_move.col << std::endl;
//    //    new_state->board.displayGrid();
//    //}
//    //std::cout << node->state->next_player << " point " << next_move.row << ", " << next_move.col << std::endl;
//    //new_state->board.displayGrid();
//    /*std::cout << "45454" << std::endl;*/
//    
//    auto child_node = create_node(new_state, next_move, node);
//    
//    Point move = next_move;
//    float value = -child_node->value;
//
//    while (node != nullptr) {
//        node->complete_visit(move, value);
//        move = node->last_move;
//        node = node->parent;
//        value = -value;
//    }
//}
//
//
//std::shared_ptr<P_ZeroTreeNode> P_ZeroAgent::create_node(std::shared_ptr<GameState> game_state, Point move, std::shared_ptr<P_ZeroTreeNode> parent) {
//    auto state_tensor = encoder->encode(*game_state);
//    
//    torch::jit::IValue output;
//    std::vector<float> priors;
//    float values;
//    std::map<Point, float> move_priors;
//    {
//        std::lock_guard<std::mutex> lock4(model_lock);
//        torch::Device device(torch::kCUDA, 0);
//        //state_tensor = state_tensor.to(torch::kCUDA);
//        //model.to(torch::kCUDA);
//        model.to(device);
//        state_tensor = state_tensor.to(device);
//        //for (const auto& param : model.parameters()) {
//        //    std::cout << "Parameter device: " << param.device() << std::endl;
//        //    break; // 只显示第一个参数的设备信息
//        //}
//        //std::cout << "Tensor device: " << state_tensor.device() << std::endl;
//        output = model.forward({ state_tensor });
//        
//    }
// 
//    auto outputs = output.toTuple()->elements();
//    auto policy_output = outputs[0];
//    auto value_output = outputs[1];
//    if (policy_output.isTensor()) {
//        
//        auto policy_output_tensor = policy_output.toTensor();
//        // 如果需要在GPU上处理（如softmax）
//        
//        // 检查Tensor的维度和数据类型
//        if (policy_output_tensor.dim() != 1) {
//            policy_output_tensor = policy_output_tensor.view(-1); // 转换为一维张量
//        }
//        
//        auto policy_output_softmax = torch::softmax(policy_output_tensor, 0);
//
//        
//        auto policy_output_softmax_cpu = policy_output_softmax.to(at::kCPU);
//
//        auto policy_output_accessor = policy_output_softmax_cpu.accessor<float, 1>();
//
//        // 在CPU上构建move_priors映射表
//        
//        for (size_t idx = 0; idx < policy_output_accessor.size(0); ++idx) {
//            move_priors[encoder->decode_point_index(idx)] = policy_output_accessor[idx];
//        }
//       
//    }
//
//    // 将value_output张量从CUDA转移到CPU，并提取单个值
//    if (value_output.isTensor()) {
//        // 复制到CPU并访问数据
//        auto value_output_tensor = value_output.toTensor();
//        // 在GPU上使用 item 方法提取单个元素，无需转换到CPU上
//        values = value_output_tensor.item<float>();
//        
//    }
//    
//    auto new_node = std::make_shared<P_ZeroTreeNode>(game_state, values, move_priors, parent, move);
//
//    if (parent) {
//        std::lock_guard<std::mutex> lock1(parent->data_lock);
//        parent->add_child(move, new_node);
//    }
//    return new_node;
//    //auto state_tensor = encoder->encode(*game_state);
//    //py::object result;
//    //{
//    //    py::gil_scoped_acquire acquire;
//    //    result = model.attr("predict")(state_tensor);
//    //}
//    ////py::gil_scoped_acquire acquire;
//    ////py::object result = model.attr("predict")(state_tensor);
//    //
//    //if (py::isinstance<py::list>(result)) {
//    //    py::list result_list = result.cast<py::list>();
//
//    //    // 处理第一个NumPy数组, 转换为 std::vector<float>
//    //    py::array_t<float> priors_array = result_list[0].cast<py::array_t<float>>();
//    //    auto priors_unchecked = priors_array.unchecked<2>(); // 假设数组是二维的
//    //    std::vector<float> priors(priors_unchecked.shape(1)); // 获取列的数量
//    //    for (ssize_t i = 0; i < priors_unchecked.shape(1); ++i) {
//    //        priors[i] = priors_unchecked(0, i); // 第一维只有一个元素, 所以直接取0
//    //    }
//
//    //    // 处理第二个NumPy数组, 转换为 float
//    //    py::array_t<float> values_array = result_list[1].cast<py::array_t<float>>();
//    //    auto values_unchecked = values_array.unchecked<2>(); // 假设数组是二维的
//    //    float values = values_unchecked(0, 0); // 直接取(0,0)位置的值
//
//    //    
//    //    std::map<Point, float> move_priors;
//    //    for (size_t idx = 0; idx < priors.size(); ++idx) {
//    //        move_priors[encoder->decode_point_index(idx)] = priors[idx];
//    //    }
//
//    //    auto new_node = std::make_shared<P_ZeroTreeNode>(game_state, values, move_priors, parent, move);
//
//    //    if (parent) {
//    //        std::lock_guard<std::mutex> lock1(parent->data_lock); 
//    //        parent->add_child(move, new_node);
//    //    }
//    //    return new_node;
//    //}
//    //float fixed_value = 0.5; // 假设的固定值估计
//    //std::vector<float> fixed_priors(196, 1.0 / 196); // 假设每个位置的先验概率相同
//
//    //std::map<Point, float> move_priors;
//    //for (size_t idx = 0; idx < fixed_priors.size(); ++idx) {
//    //    move_priors[encoder->decode_point_index(idx)] = fixed_priors[idx];
//    //}
//
//    //auto new_node = std::make_shared<P_ZeroTreeNode>(game_state, fixed_value, move_priors, parent, move);
//
//    //if (parent) {
//    //    std::lock_guard<std::mutex> lock1(parent->data_lock);
//    //    parent->add_child(move, new_node);
//    //}
//    //return new_node;
//
//}
//
//Point P_ZeroAgent::select_branch(std::shared_ptr<P_ZeroTreeNode> node) {
//    float max_score = -std::numeric_limits<float>::infinity();
//    Point best_move = node->moves()[0];
//    {
//        std::lock_guard<std::mutex> lock(node->data_lock);
//        int total_n = node->total_visit_count;
//        for (Point move : node->moves()) {
//            float q = node->expected_value(move);
//            float p = node->prior(move);
//            int n = node->visit_count(move);
//            float score = q + c * p * std::sqrt(total_n) / (n + 1);
//            //std::lock_guard<std::mutex> lock2(cout_lockt);
//            //{
//            //    std::cout << "Move: (" << move.row << "," << move.col << "), "
//            //        << "Q-value: " << q << ", "
//            //        << "Prior: " << p << ", "
//            //        << "Visit count: " << n << ", "
//            //        << "Total visit count: " << total_n << ", "
//            //        << "UCT score: " << score << std::endl;
//            //}
//
//
//            if (score > max_score) {
//                max_score = score;
//                best_move = move;
//            }
//        }
//    }
//
//
//    return best_move;
//}

#include "ParallelTreeNode.h"

P_Branch::P_Branch(float prior_value) : prior(prior_value), visit_count(0), total_value(0.0), incomplete_visit(0) {}

P_Branch::P_Branch(P_Branch&& other) noexcept
    : prior(other.prior),
    visit_count(other.visit_count.load()),
    incomplete_visit(other.visit_count.load()),
    total_value(other.total_value) {
}

// 移动赋值操作符的实现
P_Branch& P_Branch::operator=(P_Branch&& other) noexcept {
    if (this != &other) {
        prior = other.prior;
        visit_count.store(other.visit_count.load());
        incomplete_visit.store(other.incomplete_visit.load());
        total_value = other.total_value;
    }
    return *this;
}

P_ZeroTreeNode::P_ZeroTreeNode(std::shared_ptr<GameState> state_value, float value_value, const std::map<int, float>& priors, std::shared_ptr<P_ZeroTreeNode> parent_value, int last_move_value)
    : state(state_value), value(value_value), parent(parent_value), last_move(last_move_value), total_visit_count(1) {
    for (auto& move_prior : priors) {
        int move = move_prior.first;
        if (state->board.is_valid_move_layout(move)) {
            branches[move] = P_Branch(move_prior.second);
        }
    }
}

std::vector<int> P_ZeroTreeNode::moves() {
    std::vector<int> moves;
    moves.reserve(200);
    for (const auto& branch : branches) {
        moves.emplace_back(branch.first);
    }
    return moves;
}

void P_ZeroTreeNode::add_child(int move, std::shared_ptr<P_ZeroTreeNode> child_node) {
    children[move] = child_node;
}

bool P_ZeroTreeNode::has_child(int move) {
    return children.find(move) != children.end();
}

std::shared_ptr<P_ZeroTreeNode> P_ZeroTreeNode::get_child(int move) {
    return children.at(move);
}

void P_ZeroTreeNode::complete_visit(int move, float value) {
    {
        std::lock_guard<std::mutex> lock1(data_lock);
        /*total_visit_count++;*/
        //total_incomplete_visit--;
        //branches[move].incomplete_visit--;
        branches[move].visit_count++;
        branches[move].total_value += value;
    }

}

void P_ZeroTreeNode::incomplete_visit(int move) {
    {
        std::lock_guard<std::mutex> lock1(data_lock);
        branches[move].incomplete_visit++;
        /*total_incomplete_visit++;*/
        total_visit_count++;

    }
}

float P_ZeroTreeNode::expected_value(int move) {
    P_Branch& branch = branches[move];
    if (branch.visit_count == 0) {
        return 0.0;
    }
    return branch.total_value / static_cast<float>(branch.visit_count);
}

float P_ZeroTreeNode::prior(int move) {
    return branches[move].prior;
}

int P_ZeroTreeNode::visit_count(int move) {
    return branches.count(move) > 0 ? branches[move].incomplete_visit.load() : 0;
}

//int P_ZeroTreeNode::incomplete_visit(Point move) {
//    return branches.count(move) > 0 ? branches[move].visit_count.load() : 0;
//}

P_ZeroAgent::P_ZeroAgent(Net* net, int rounds_per_move, float c_value)
    :net(net),
    encoder(std::make_shared<LayoutEncoder>(std::make_pair(14, 14))),
    num_rounds(rounds_per_move),
    c(c_value){}

//P_ZeroAgent::P_ZeroAgent(int rounds_per_move, float c_value)
//    : encoder(std::make_shared<LayoutEncoder>(std::make_pair(14, 14))), num_rounds(rounds_per_move), c(c_value) {}


std::vector<int> P_ZeroAgent::select_move(std::shared_ptr<GameState> game_state) {

    //std::ofstream logFile("logfile.txt");

    //// 将std::cout重定向到日志文件
    //std::streambuf* originalCoutBuffer = std::cout.rdbuf(logFile.rdbuf()); 
    /*auto root = create_node_dirichlet(game_state);*/
    std::shared_ptr<P_ZeroTreeNode> root = nullptr;
    //if (game_state->step <= 30) {
    //    /*root = create_node_huadong(game_state);*/
    //    root = create_node_dirichlet(game_state);
    //    /*std::cout << "huadong" << std::endl;*/
    //}
    //else
    //{
    //    root = p_create_node(game_state);
    //    /*std::cout << "putong" << std::endl;*/
    //}
    /*auto root = create_node_gaussian(game_state);*/
    root = p_create_node(game_state);
    /*game_state->board.displayGrid();*/
    std::vector<std::future<void>> futures;
    for (int i = 0; i < num_rounds; ++i) {
        auto future =
            thread_pool.commit(std::bind(&P_ZeroAgent::simulate_round, this, root));
        futures.emplace_back(std::move(future));
    }
    for (unsigned int i = 0; i < futures.size(); i++)
        futures[i].wait();
    torch::Tensor root_state_tensor = encoder->encode(*game_state);
    /*std::cout << root_state_tensor << std::endl;*/
    // 获取访问次数
    std::vector<int> visit_counts;
    visit_counts.reserve(200);
    for (int idx = 0; idx < encoder->num_points(); ++idx) {
        visit_counts.emplace_back(root->visit_count(idx));
    }
    //std::vector<int> visit_counts_scaled;
    //visit_counts_scaled.reserve(200);
    //double temperature = 0.1;
    //// Scale the visit counts with the temperature
    //for (int count : visit_counts) {
    //    int scaled = exp(static_cast<double>(count) / temperature);
    //    visit_counts_scaled.emplace_back(scaled);
    //}

    // 记录决定
    //std::cout << "记录前" << zerocollector._current_episode_states.size() << std::endl;
    
    //std::cout << "记录后" << zerocollector._current_episode_states.size() << std::endl;

    //std::vector<int> moves = root->moves();
    //std::vector<int> move = { *std::max_element(moves.begin(), moves.end(),
    //    [&root](const int& a, const int& b) {
    //        return root->visit_count(a) < root->visit_count(b);
    //    }) };
    //return move;
    std::vector<int> moves = root->moves();

    std::vector<double> probs(visit_counts.size());
    double sum = 0.0;
    //double temperature = 1.0;
    //if (game_state->step > 30) {
    //    temperature = 0.1;
    //}
    double temperature = 0.05;
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

    //// 找到最大的访问次数
    //int max_visit_count = 0;
    //for (int move : moves) {
    //    int visit_count = root->visit_count(move);
    //    std::cout << visit_count<<" ";
    //    if (visit_count > max_visit_count) {
    //        max_visit_count = visit_count;
    //    }
    //}
    //
    //// 收集所有具有最大访问次数的移动
    //std::vector<int> candidates;
    //for (int move : moves) {
    //    if (root->visit_count(move) == max_visit_count) {
    //        candidates.push_back(move);
    //    }
    //}

    //// 随机选择一个
    //std::random_device rd;
    //std::mt19937 gen(rd());
    //std::uniform_int_distribution<> dis(0, candidates.size() - 1);
    //int random_index = dis(gen);
    //int random_move = candidates[random_index];

    return { distribution(generator) };
}

void P_ZeroAgent::simulate_round(std::shared_ptr<P_ZeroTreeNode> root) {
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
    /*std::cout <<"选择" << next_move << std::endl;*/
    //while (node->has_child(next_move)) {
    //    node = node->get_child(next_move);
    //    next_move = select_branch(node);
    //    node->incomplete_visit(next_move);
    //}
    int move = 0;
    float value = 0;
    if (node->has_child(next_move)) {

        std::vector<int> next_move1 = { next_move };
        move = next_move;
        float value = node->value;
    }
    else
    {

        std::vector<int> next_move1 = { next_move };
        auto new_state = std::make_shared<GameState>(node->state->go_apply_move_mcts(next_move1));

        std::shared_ptr<P_ZeroTreeNode> child_node = nullptr;
        //if (new_state->step <= 43) {
        //    /*child_node = create_node_huadong(new_state);*/
        //    child_node = create_node_huadong(new_state);
        //    /*std::cout << "1" << std::endl;*/
        //}
        //else
        //{
        //    child_node = p_create_node(new_state);
        //    /*std::cout << "2" << std::endl;*/
        //}
        child_node = p_create_node(new_state);
        move = next_move;
        float value = -child_node->value;
    }


    while (node != nullptr) {
        node->complete_visit(move, value);
        move = node->last_move;
        node = node->parent;
        value = -value;
    }
}


std::shared_ptr<P_ZeroTreeNode> P_ZeroAgent::create_node(std::shared_ptr<GameState> game_state, int move, std::shared_ptr<P_ZeroTreeNode> parent) {
    auto state_tensor = encoder->encode(*game_state);

    torch::jit::IValue output;
    std::vector<float> priors;
    float values;
    std::map<int, float> move_priors;

    {
        std::lock_guard<std::mutex> lock4(model_lock);
        //torch::Device device(torch::kCUDA, 0);
        ////state_tensor = state_tensor.to(torch::kCUDA);
        ////model.to(torch::kCUDA);
        //model.to(device);
        torch::Device device(torch::kCUDA, 0);
        state_tensor = state_tensor.to(device);
        //for (const auto& param : model.parameters()) {
        //    std::cout << "Parameter device: " << param.device() << std::endl;
        //    break; // 只显示第一个参数的设备信息
        //}
        //std::cout << "Tensor device: " << state_tensor.device() << std::endl;
        output = model.forward({ state_tensor });

    }

    auto outputs = output.toTuple()->elements();
    auto policy_output = outputs[0];
    auto value_output = outputs[1];
    if (policy_output.isTensor()) {
        auto policy_output_tensor = policy_output.toTensor();
        // 如果需要在GPU上处理（如softmax）

        // 检查Tensor的维度和数据类型
        if (policy_output_tensor.dim() != 1) {
            policy_output_tensor = policy_output_tensor.view(-1); // 转换为一维张量
        }

        auto policy_output_softmax = torch::softmax(policy_output_tensor, 0);


        auto policy_output_softmax_cpu = policy_output_softmax.to(at::kCPU);

        auto policy_output_accessor = policy_output_softmax_cpu.accessor<float, 1>();

        // 在CPU上构建move_priors映射表

        for (size_t idx = 0; idx < policy_output_accessor.size(0); ++idx) {
            move_priors[idx] = policy_output_accessor[idx];
        }
        //std::cout << "[";
        //for (size_t idx = 0; idx < move_priors.size(); ++idx) {
        //    std::cout << move_priors[idx];
        //    if (idx < move_priors.size() - 1) {
        //        std::cout << ", ";
        //    }
        //}
        //std::cout << "]" << std::endl;

    }

    // 将value_output张量从CUDA转移到CPU，并提取单个值
    if (value_output.isTensor()) {
        // 复制到CPU并访问数据
        auto value_output_tensor = value_output.toTensor();
        // 在GPU上使用 item 方法提取单个元素，无需转换到CPU上
        values = value_output_tensor.item<float>();

    }

    auto new_node = std::make_shared<P_ZeroTreeNode>(game_state, values, move_priors, parent, move);

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

    //    auto new_node = std::make_shared<P_ZeroTreeNode>(game_state, values, move_priors, parent, move);

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

    //auto new_node = std::make_shared<P_ZeroTreeNode>(game_state, fixed_value, move_priors, parent, move);

    //if (parent) {
    //    std::lock_guard<std::mutex> lock1(parent->data_lock);
    //    parent->add_child(move, new_node);
    //}
    //return new_node;

}

int P_ZeroAgent::select_branch(std::shared_ptr<P_ZeroTreeNode> node) {
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
        //int total_n = node->total_visit_count;
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

std::shared_ptr<P_ZeroTreeNode> P_ZeroAgent::create_node_dirichlet(std::shared_ptr<GameState> game_state, int move, std::shared_ptr<P_ZeroTreeNode> parent) {
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

    auto new_node = std::make_shared<P_ZeroTreeNode>(game_state, value_output, move_priors, parent, move);

    if (parent) {
        parent->add_child(move, new_node);
    }
    return new_node;
}

std::shared_ptr<P_ZeroTreeNode> P_ZeroAgent::create_node_gaussian(std::shared_ptr<GameState> game_state, int move, std::shared_ptr<P_ZeroTreeNode> parent) {
    auto state_tensor = encoder->encode(*game_state);

    torch::jit::IValue output;
    std::vector<float> priors;
    float values;
    std::map<int, float> move_priors;
    torch::Device device(torch::kCUDA, 0);
    state_tensor = state_tensor.to(device);

    output = model.forward({ state_tensor });

    auto outputs = output.toTuple()->elements();
    auto policy_output = outputs[0];
    auto value_output = outputs[1];
    if (policy_output.isTensor()) {

        auto policy_output_tensor = policy_output.toTensor();
        if (policy_output_tensor.dim() != 1) {
            policy_output_tensor = policy_output_tensor.view(-1);
        }

        auto policy_output_softmax = torch::softmax(policy_output_tensor, 0);


        auto policy_output_softmax_cpu = policy_output_softmax.to(at::kCPU);
        const int size = 14;
        const auto sizeSquared = size * size;
        int sigma = 3;

        std::vector<float> prob_matrix(196);
        for (int idx = 0; idx < 196; ++idx) {
            prob_matrix[idx] = policy_output_softmax_cpu.data_ptr<float>()[idx];
        }
        // 将概率矩阵转换为 14*14 的二维数组
        std::vector<std::vector<float>> board_prob(size, std::vector<float>(size));
        for (int row = 0; row < size; ++row) {
            for (int col = 0; col < size; ++col) {
                board_prob[row][col] = prob_matrix[row * size + col];
            }
        }

        // 创建 14*14 的网格坐标矩阵
        std::vector<std::vector<float>> x(size, std::vector<float>(size));
        std::vector<std::vector<float>> y(size, std::vector<float>(size));
        for (int row = 0; row < size; ++row) {
            for (int col = 0; col < size; ++col) {
                x[row][col] = row;
                y[row][col] = col;
            }
        }

        // 计算每个位置到中心点的欧式距离
        std::vector<std::vector<float>> distance(size, std::vector<float>(size));
        for (int row = 0; row < size; ++row) {
            for (int col = 0; col < size; ++col) {
                distance[row][col] = std::sqrt(std::pow(x[row][col] - 6.5, 2) + std::pow(y[row][col] - 6.5, 2));
            }
        }

        // 计算高斯权重矩阵
        std::vector<std::vector<float>> gaussian_weights(size, std::vector<float>(size));
        for (int row = 0; row < size; ++row) {
            for (int col = 0; col < size; ++col) {
                gaussian_weights[row][col] = (1 / (sigma * std::sqrt(2 * M_PI))) * std::exp(-0.5 * std::pow(distance[row][col] / sigma, 2));
            }
        }

        // 对原始概率矩阵进行高斯加权
        std::vector<std::vector<float>> weighted_prob(size, std::vector<float>(size));
        for (int row = 0; row < size; ++row) {
            for (int col = 0; col < size; ++col) {
                weighted_prob[row][col] = board_prob[row][col] * gaussian_weights[row][col];
            }
        }

        // 计算归一化因子
        double normalizer = 0;
        for (int row = 0; row < size; ++row) {
            for (int col = 0; col < size; ++col) {
                normalizer += weighted_prob[row][col];
            }
        }

        // 归一化得到最终的高斯分布概率矩阵
        std::vector<float> gaussian_prob(196);
        for (int row = 0; row < size; ++row) {
            for (int col = 0; col < size; ++col) {
                move_priors[row * size + col] = weighted_prob[row][col] / normalizer;
            }
        }

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
    }

    if (value_output.isTensor()) {
        // 复制到CPU并访问数据
        auto value_output_tensor = value_output.toTensor();
        // 在GPU上使用 item 方法提取单个元素，无需转换到CPU上
        values = value_output_tensor.item<float>();

    }

    auto new_node = std::make_shared<P_ZeroTreeNode>(game_state, values, move_priors, parent, move);

    if (parent) {
        parent->add_child(move, new_node);
    }
    return new_node;
}


std::shared_ptr<P_ZeroTreeNode> P_ZeroAgent::create_node_huadong(std::shared_ptr<GameState> game_state, int move, std::shared_ptr<P_ZeroTreeNode> parent) {
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
    auto new_node = std::make_shared<P_ZeroTreeNode>(game_state, value_output, move_priors, parent, move);

    if (parent) {
        std::lock_guard<std::mutex> lock1(parent->data_lock);
        parent->add_child(move, new_node);
    }

    return new_node;
}




std::shared_ptr<P_ZeroTreeNode> P_ZeroAgent::p_create_node(std::shared_ptr<GameState> game_state, int move, std::shared_ptr<P_ZeroTreeNode> parent) {
    auto state_tensor = encoder->encode(*game_state);

    std::map<int, float> move_priors;
    auto future = net->predict(state_tensor);
    auto outputs = future.get();
    auto policy_output = std::move(outputs.first);
    auto value_output = outputs.second;
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
    auto new_node = std::make_shared<P_ZeroTreeNode>(game_state, value_output, move_priors, parent, move);

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

    //    auto new_node = std::make_shared<P_ZeroTreeNode>(game_state, values, move_priors, parent, move);

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

    //auto new_node = std::make_shared<P_ZeroTreeNode>(game_state, fixed_value, move_priors, parent, move);

    //if (parent) {
    //    std::lock_guard<std::mutex> lock1(parent->data_lock);
    //    parent->add_child(move, new_node);
    //}
    //return new_node;

}



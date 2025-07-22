#include"Expert.h"


const float MIN_SCORE = -9999999;
const float MAX_SCORE = 9999999;
const int layout_score[] = { 7, 35, 800, 8000, 15, 400, 15000 };
int layout_strategy(const GameState& game_state) {
    std::vector<std::pair<int, int>> stones_score;
    stones_score.reserve(196);
    for (int candidate_index = 0; candidate_index < 196; candidate_index++) {
        if (game_state.board.grid[candidate_index] == 0) {
            int stone_score = compute_layout_strategy(game_state, candidate_index);
            stones_score.emplace_back(candidate_index, stone_score);
        }
    }

    std::sort(stones_score.begin(), stones_score.end(),
        [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            return a.second > b.second;
        });

    std::mt19937 rng(static_cast<unsigned int>(time(nullptr))); // 随机数生成器
    std::uniform_int_distribution<int> dist; // 随机数分布

    if (stones_score.size() > 2) {
        if (stones_score[0].second - stones_score[2].second < 50) {
            dist = std::uniform_int_distribution<int>(0, 2);
            int pos = dist(rng);
            return stones_score[pos].first;
        }
        else if (stones_score[0].second - stones_score[1].second < 100) {
            dist = std::uniform_int_distribution<int>(0, 1);
            int pos = dist(rng);
            return stones_score[pos].first;
        }
        else {
            return stones_score[0].first;
        }
    }
    else {
        return stones_score[0].first;
    }
}

int compute_layout_strategy(const GameState& game_state, const int& point) {
    int score_sum = 0;
    for (const std::vector<int>& neighbor : neighbors4(point)) {
        int player1_stones = 0;
        int player2_stones = 0;
        int score = 0;
        int s = 0;

        for (const int& nei : neighbor) {
            if (game_state.board.grid[nei] == game_state.next_player) {
                player1_stones += 1;
                s = 1;
            }
            if (game_state.board.grid[nei] == other(game_state.next_player)) {
                player2_stones += 1;
                s = 1;
            }
        }

        if (player1_stones == 0 && player2_stones == 0 && s == 1) {
            score = layout_score[0];
        }
        else if (player1_stones > 0 && player2_stones > 0) {
            score = 0;
        }
        else if (player1_stones > 0) {
            score = layout_score[player1_stones];
        }
        else if (player2_stones > 0) {
            score = layout_score[player2_stones + 3];
        }
        score_sum += score;
    }
    return score_sum;
}


float eval_fn(GameState& game_state) {
    int player_stones = game_state.board.sum_stone(game_state.next_player);
    int opponent_stones = game_state.board.sum_stone(other(game_state.next_player));
    int stone_diff = player_stones - opponent_stones;

    float player_routes_score = game_state.board.special_moves1(game_state.next_player);
    float opponent_routes_score = game_state.board.special_moves1(other(game_state.next_player));
    float routes_diff = player_routes_score - opponent_routes_score * 1.5;

    return stone_diff * 0.6f + routes_diff * 0.4f;
}

// Alpha-beta 剪枝函数
float alpha_beta_result(GameState& game_state, int max_depth, float best_black, float best_white) {

    if (max_depth == 0) {
        return eval_fn(game_state);
    }
    float best_so_far = MIN_SCORE;
    std::vector<std::vector<int>> all_legal_moves = game_state.board.legal_moves(game_state.next_player, game_state.state);
    for (const auto& candidate_move : all_legal_moves) {
        auto eat_move = eat_strategy(game_state, candidate_move);
        std::tuple<std::vector<int>, std::vector<int>> move = std::make_tuple(candidate_move, eat_move);
        GameState next_state = game_state.apply_move(move);
        float opponent_best_result = alpha_beta_result(next_state, max_depth - 1, best_black, best_white);
        float our_result = -opponent_best_result;
        if (our_result > best_so_far) {
            best_so_far = our_result;
        }
        if (game_state.next_player == WHITE) {
            if (best_so_far > best_white) {
                best_white = best_so_far;
            }
            float outcome_for_black = -best_so_far;
            if (outcome_for_black < best_black) {
                return best_so_far;
            }
        }
        else if (game_state.next_player == BLACK) {
            if (best_so_far > best_black) {
                best_black = best_so_far;
            }
            float outcome_for_white = -best_so_far;
            if (outcome_for_white < best_white) {
                return best_so_far;
            }
        }
    }
    return best_so_far;
}


std::mutex mutex;
void search_best_moves(GameState game_state, const std::vector<std::vector<int>>& moves, float& global_best_score, std::vector<std::vector<int>>& global_best_moves) {
    float local_best_score = std::numeric_limits<int>::min();
    std::vector<std::vector<int>> local_best_moves;

    for (const auto& possible_move : moves) {
        auto eat_move = eat_strategy(game_state, possible_move);
        std::tuple<std::vector<int>, std::vector<int>> move = std::make_tuple(possible_move, eat_move);
        GameState next_state = game_state.apply_move(move);
        float best_black = MIN_SCORE;
        float best_white = MIN_SCORE;
        float opponent_best_outcome = alpha_beta_result(next_state, 0, best_black, best_white);
        float our_best_outcome = -opponent_best_outcome;

        //std::lock_guard<std::mutex> guard(mutex); // 保护全局变量
        if (our_best_outcome > local_best_score) {
            local_best_moves.clear();
            local_best_moves.push_back(possible_move);
            local_best_score = our_best_outcome;
        }
        else if (our_best_outcome == local_best_score) {
            local_best_moves.push_back(possible_move);
        }
    }

    std::lock_guard<std::mutex> guard(mutex); // 再次保护全局变量
    if (local_best_score > global_best_score) {
        global_best_moves = local_best_moves;
        global_best_score = local_best_score;
    }
    else if (local_best_score == global_best_score) {
        global_best_moves.insert(global_best_moves.end(), local_best_moves.begin(), local_best_moves.end());
    }
}

std::vector<int> walk_strategy(GameState& game_state) {

    std::vector<std::vector<int>> legal_moves = game_state.board.legal_moves(game_state.next_player, game_state.state);
    if (legal_moves.empty()) {
        return {};
    }
    std::vector<std::vector<int>> global_best_moves;
    float global_best_score = std::numeric_limits<int>::min();
    // Divide the work among threads
    int num_threads = std::thread::hardware_concurrency(); // Number of concurrent threads supported
    /*std::cout << num_threads << std::endl;*/
    std::vector<std::thread> threads;
    size_t moves_per_thread = legal_moves.size() / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        size_t start = i * moves_per_thread;
        size_t end = (i == num_threads - 1) ? legal_moves.size() : (i + 1) * moves_per_thread;
        std::vector<std::vector<int>> thread_moves(legal_moves.begin() + start, legal_moves.begin() + end);
        threads.emplace_back(search_best_moves, game_state, thread_moves, std::ref(global_best_score), std::ref(global_best_moves)); // 不用 std::ref
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }

    // Randomly choose one of the best moves
    std::srand(static_cast<unsigned int>(std::time(0)));
    /*for (auto moves : global_best_moves) {
        game_state.board.Print_move(moves, game_state.next_player);
    }*/
    /*std::cout << global_best_moves.size() << std::endl;*/
    return global_best_moves[std::rand() % global_best_moves.size()];
}

std::mutex mtx;
float eval_fn1(GameState& game_state, int stones, float diff) {
    int player_stones = game_state.board.sum_stone(game_state.next_player);
    int stone_diff = stones - player_stones;

    float player_routes_score = game_state.board.special_moves1(game_state.next_player);
    float opponent_routes_score = game_state.board.special_moves3(other(game_state.next_player));
    float routes_diff = opponent_routes_score - player_routes_score * 1.5;
    return stone_diff * 0.2f + routes_diff * 0.8f;
}



void search_best_moves1(GameState game_state, const std::vector<std::vector<int>>& moves, int stones, float diff, std::vector<std::vector<int>>& global_best_moves, int& found_filtered) {
    std::vector<std::vector<int>> filtered_moves;
    std::vector<std::vector<int>> backup_moves;
    std::vector<std::vector<int>> backup_moves1;
    std::vector<std::vector<int>> backup_moves2;

    for (const auto& possible_move : moves) {
        Board next_board = game_state.board;
        next_board.go_take_stone(game_state.next_player, possible_move);
        int nums = next_board.squire(game_state.next_player, possible_move.back());
        if (nums > 0) {
            filtered_moves.emplace_back(possible_move);
            continue;
        }
        std::vector<int> eat_move = {};
        std::tuple<std::vector<int>, std::vector<int>> move = std::make_tuple(possible_move, eat_move);
        GameState next_state = game_state.apply_move(move);

        float score = eval_fn1(next_state, stones, diff);
        /*std::cout << score << std::endl;*/
        if (score >= (diff * 0.8 + 2)) {

            filtered_moves.emplace_back(possible_move);
        }
        else if (score > (diff * 0.8)) {
            backup_moves.emplace_back(possible_move);
        }
        else if (score == (diff * 0.8)) {
            backup_moves1.emplace_back(possible_move);
        }
        else
        {
            backup_moves2.emplace_back(possible_move);
        }
    }

    if (!filtered_moves.empty()) {
        global_best_moves = std::move(filtered_moves);
        found_filtered = 1;
    }
    else if (!backup_moves.empty()) {
        global_best_moves = std::move(backup_moves);
        found_filtered = 2;
    }
    else if (!backup_moves1.empty()) {
        global_best_moves = std::move(backup_moves1);
        found_filtered = 3;
    }
    else {
        global_best_moves = std::move(backup_moves2);
        found_filtered = 4;
    }
}


std::vector<std::vector<int>> walk_strategy1(GameState& game_state) {
    // Get all legal moves
    std::vector<std::vector<int>> legal_moves = game_state.board.legal_moves(game_state.next_player, game_state.state);
    float player_score = game_state.board.special_moves1(game_state.next_player);
    float opponent_score = game_state.board.special_moves1(other(game_state.next_player));
    float routes_diff = (player_score - opponent_score * 1.5) * 0.8;
    if (legal_moves.empty()) {
        return {};
    }
    int stones = game_state.board.sum_stone(other(game_state.next_player));
    std::vector<std::vector<int>> global_best_moves;
    std::vector<std::vector<int>> global_best_moves1;
    int found_filtered1 = 0;
    int found_filtered2 = 0;
    int num_threads = 2;
    /*std::cout << num_threads << std::endl;*/
    std::vector<std::thread> threads;
    size_t moves_per_thread = legal_moves.size() / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        size_t start = i * moves_per_thread;
        size_t end = (i == num_threads - 1) ? legal_moves.size() : (i + 1) * moves_per_thread;
        std::vector<std::vector<int>> thread_moves(legal_moves.begin() + start, legal_moves.begin() + end);
        if (i == 0) {
            threads.emplace_back(search_best_moves1, game_state, thread_moves, stones, routes_diff, std::ref(global_best_moves), std::ref(found_filtered1));
        }
        else
        {
            threads.emplace_back(search_best_moves1, game_state, thread_moves, stones, routes_diff, std::ref(global_best_moves), std::ref(found_filtered2));
        }
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }
    if (found_filtered1 == found_filtered2) {
        global_best_moves.insert(global_best_moves.end(), global_best_moves1.begin(), global_best_moves1.end());
    }
    else if (found_filtered1 > found_filtered2) {
        global_best_moves = std::move(global_best_moves1); // result2 contains filtered moves, so we use it
    }


    ///*    for (auto moves : global_best_moves) {
    //        mtx.lock();
    //        game_state.board.Print_move(moves, {},game_state.next_player);
    //        mtx.unlock();
    //    */}
        /*std::cout << global_best_moves.size() << std::endl;*/
    return global_best_moves;
}

std::mutex mtx1;
float eval_fn2(GameState& game_state, int stones, float diff) {
    int player_stones = game_state.board.sum_stone(game_state.next_player);
    int stone_diff = stones - player_stones;

    float player_routes_score = game_state.board.special_moves1(game_state.next_player);
    float opponent_routes_score = game_state.board.special_moves3(other(game_state.next_player));
    float routes_diff = opponent_routes_score - player_routes_score * 1.5;
    return stone_diff * 0.2f + routes_diff * 0.8f;
}



void search_best_moves2(GameState game_state, const std::vector<std::vector<int>>& moves, std::vector<std::vector<int>>& global_best_moves, int& found_filtered) {
    std::vector<std::vector<int>> filtered_moves;
    std::vector<std::vector<int>> filtered_moves1;
    std::vector<std::vector<int>> backup_moves;

    for (const auto& possible_move : moves) {
        Board next_board = game_state.board;
        next_board.go_take_stone(game_state.next_player, possible_move);
        int nums = next_board.squire(game_state.next_player, possible_move.back());
        int nums1 = next_board.squire(game_state.next_player, possible_move[0]);
        if (nums > 0) {
            if (nums1 > 0) {
                filtered_moves.emplace_back(possible_move);
                continue;
            }
            else
            {
                filtered_moves1.emplace_back(possible_move);
                continue;
            }
        }
        backup_moves.emplace_back(possible_move);
    }

    if (!filtered_moves.empty()) {
        global_best_moves = std::move(filtered_moves);
        found_filtered = 1;
    }
    else if (!filtered_moves1.empty()) {
        global_best_moves = std::move(filtered_moves1);
        found_filtered = 2;
    }
    else
    {
        global_best_moves = std::move(backup_moves);
        found_filtered = 3;
    }

}


std::vector<std::vector<int>> walk_strategy2(GameState& game_state) {
    // Get all legal moves
    std::vector<std::vector<int>> legal_moves = game_state.board.legal_moves(game_state.next_player, game_state.state);
    //float player_score = game_state.board.special_moves1(game_state.next_player);
    //float opponent_score = game_state.board.special_moves1(other(game_state.next_player));
    //float routes_diff = (player_score - opponent_score * 1.5) * 0.8;
    if (legal_moves.empty()) {
        return {};
    }
    std::vector<std::vector<int>> global_best_moves;
    int found_filtered1 = 0;
    search_best_moves2(game_state, legal_moves, global_best_moves, found_filtered1);
    return global_best_moves;
}

std::vector<int> eat_strategy(GameState& game_state, const std::vector<int>& move) {
    Board next_board = game_state.board;
    next_board.go_take_stone(game_state.next_player, move);
    int nums = next_board.squire(game_state.next_player, move.back());
    std::vector<std::pair<float, int>> candidate_scores;
    std::vector<int> eat_moves;
    Player other_player = other(game_state.next_player);

    if (nums == 1) {
        for (int candidate_index = 0; candidate_index < 196; candidate_index++) {
            if (next_board.grid[candidate_index] == other_player) {
                next_board.grid[candidate_index] = 0;
                float opponent_score = next_board.special_moves1(other_player);
                float our_score = next_board.special_moves1(game_state.next_player);
                next_board.grid[candidate_index] = other_player;
                candidate_scores.emplace_back(opponent_score * 1.5 - our_score, candidate_index);
            }
        }
        std::sort(candidate_scores.begin(), candidate_scores.end());
        eat_moves.emplace_back(candidate_scores.front().second);
    }
    else if (nums == 2) {
        for (int i = 0; i < 196; i++) {
            if (next_board.grid[i] != other_player) continue;
            for (int j = 0; j < i; j++) {
                if (next_board.grid[j] != other_player) continue;
                next_board.grid[i] = 0;
                next_board.grid[j] = 0;
                float opponent_score = next_board.special_moves1(other_player);
                float our_score = next_board.special_moves1(game_state.next_player);
                next_board.grid[i] = other_player;
                next_board.grid[j] = other_player;
                int candidate_index = (i * 196) + j;
                candidate_scores.emplace_back(opponent_score * 1.5 - our_score, candidate_index);
            }
        }

        std::sort(candidate_scores.begin(), candidate_scores.end());
        int min_index = candidate_scores.front().second;
        eat_moves.push_back(min_index / 196);  // i 的值
        eat_moves.push_back(min_index % 196);  // j 的值
    }
    else
    {
        return {};
    }
    return eat_moves;
}

std::vector<std::pair<float, int>> eat_strategy1(GameState& game_state, const std::vector<int>& move, int nums1) {
    Board next_board = game_state.board;
    next_board.go_take_stone(game_state.next_player, move);
    int nums = next_board.squire(game_state.next_player, move.back());
    std::vector<std::pair<float, int>> candidate_scores;
    std::vector<int> eat_moves;
    Player other_player = other(game_state.next_player);

    if (nums == 1) {
        for (int candidate_index = 0; candidate_index < 196; candidate_index++) {
            if (next_board.grid[candidate_index] == other_player) {
                next_board.grid[candidate_index] = 0;
                float opponent_score = next_board.special_moves1(other_player);
                float our_score = next_board.special_moves1(game_state.next_player);
                next_board.grid[candidate_index] = other_player;
                candidate_scores.emplace_back(opponent_score * 1.5 - our_score, candidate_index);
            }
        }
        std::sort(candidate_scores.begin(), candidate_scores.end());

    }
    else if (nums == 2) {
        MoveEncoder encode({ 14,14 }, 5);
        for (int i = 0; i < 196; i++) {
            if (next_board.grid[i] != other_player) continue;
            for (int j = 0; j < i; j++) {
                if (next_board.grid[j] != other_player) continue;
                next_board.grid[i] = 0;
                next_board.grid[j] = 0;
                float opponent_score = next_board.special_moves1(other_player);
                float our_score = next_board.special_moves1(game_state.next_player);
                next_board.grid[i] = other_player;
                next_board.grid[j] = other_player;

                int candidate_index = encode.encode_eat({ i,j });
                candidate_scores.emplace_back(opponent_score * 1.5 - our_score, candidate_index);
            }
        }

        std::sort(candidate_scores.begin(), candidate_scores.end());
    }
    std::vector<std::pair<float, int>> top_candidates;

    // Resize the new vector to hold at most `nums` elements
    top_candidates.resize(std::min(nums1, static_cast<int>(candidate_scores.size())));

    // Copy the top `nums` elements from candidate_scores to top_candidates
    std::copy_n(candidate_scores.begin(), top_candidates.size(), top_candidates.begin());
    return top_candidates;
}


std::vector<int> eat_strategy2(GameState& game_state, const std::vector<int>& move, int nums1) {
    Board next_board = game_state.board;
    Player other_player = other(game_state.next_player);
    float opponent_scores = next_board.special_moves1(other_player);
    float our_scores = next_board.special_moves1(game_state.next_player);
    float scores = 1.5 * opponent_scores - our_scores;
    next_board.go_take_stone(game_state.next_player, move);
    int nums = next_board.squire(game_state.next_player, move.back());

    std::vector<int> eat_moves;
    std::vector<int> eat_moves1;
    if (nums == 1) {
        for (int candidate_index = 0; candidate_index < 196; candidate_index++) {
            if (next_board.grid[candidate_index] == other_player) {
                next_board.grid[candidate_index] = 0;
                float opponent_score = next_board.special_moves1(other_player);
                float our_score = next_board.special_moves1(game_state.next_player);
                next_board.grid[candidate_index] = other_player;
                float sc = opponent_score * 1.5 - our_score;
                if (scores > sc) {
                    eat_moves.emplace_back(candidate_index);
                }
                else
                {
                    eat_moves1.emplace_back(candidate_index);
                }
            }
        }

    }
    else if (nums == 2) {
        MoveEncoder encode({ 14,14 }, 5);
        for (int i = 0; i < 196; i++) {
            if (next_board.grid[i] != other_player) continue;
            for (int j = 0; j < i; j++) {
                if (next_board.grid[j] != other_player) continue;
                next_board.grid[i] = 0;
                next_board.grid[j] = 0;
                float opponent_score = next_board.special_moves1(other_player);
                float our_score = next_board.special_moves1(game_state.next_player);
                next_board.grid[i] = other_player;
                next_board.grid[j] = other_player;
                int candidate_index = encode.encode_eat({ i,j });
                float sc = opponent_score * 1.5 - our_score;
                if (scores > sc) {
                    eat_moves.emplace_back(candidate_index);
                }
                else
                {
                    eat_moves1.emplace_back(candidate_index);
                }
            }
        }

    }

    if (!eat_moves.empty()) {
        return eat_moves;
    }
    else
    {
        return eat_moves1;
    }

}

std::vector<int> legal_eat(GameState& game_state, const std::vector<int>& move) {
    Board next_board = game_state.board;
    Player other_player = other(game_state.next_player);
    next_board.go_take_stone(game_state.next_player, move);
    int nums = next_board.squire(game_state.next_player, move.back());
    std::vector<int> eat_moves;
    if (nums == 1) {
        for (int candidate_index = 0; candidate_index < 196; candidate_index++) {
            if (next_board.grid[candidate_index] == other_player) {
                eat_moves.emplace_back(candidate_index);
            }
        }
    }
    else if (nums == 2) {
        MoveEncoder encode({ 14,14 }, 5);
        for (int i = 0; i < 196; i++) {
            if (next_board.grid[i] != other_player) continue;
            for (int j = 0; j < i; j++) {
                if (next_board.grid[j] != other_player) continue;
                int candidate_index = encode.encode_eat({ i,j });
                eat_moves.emplace_back(candidate_index);
            }
        }

    }
    return eat_moves;

}

std::vector<int> eat_strategy3(Board& next_board, int nums2, Player player) {
    //game_state.board.displayGrid();
    //game_state.board.Print_move(move, {}, Player::WHITE);
    Player other_player = other(player);
    float opponent_scores = next_board.special_moves1(other_player);
    float our_scores = next_board.special_moves1(player);
    float scores = 1.5 * opponent_scores - our_scores;
    std::vector<std::pair<float, int>> candidate_scores;
    std::vector<int> eat_moves;
    std::vector<int> eat_moves1;
    std::vector<int> eat_moves2;
    /*std::cout << "Our Scores: " << nums2 << std::endl;*/
    if (nums2 == 1) {
        for (int candidate_index = 0; candidate_index < 196; candidate_index++) {
            if (next_board.grid[candidate_index] == other_player) {
                next_board.grid[candidate_index] = 0;
                float opponent_score = next_board.special_moves1(other_player);
                float our_score = next_board.special_moves1(player);
                next_board.grid[candidate_index] = other_player;
                float sc = opponent_score * 1.5 - our_score;
                // 输出分数
                //std::cout << "Opponent Scores: " << opponent_score << std::endl;
                //std::cout << "Our Scores: " << our_score << std::endl;
                //std::cout << "Scores: " << sc<< std::endl;
                //if (scores > sc + 2.0) {
                //    eat_moves.emplace_back(candidate_index);
                //}
                if (scores > sc) {
                    eat_moves1.emplace_back(candidate_index);
                }
                else
                {
                    eat_moves2.emplace_back(candidate_index);
                }
            }
        }
        
        //if (!eat_moves.empty()) {
        //    return eat_moves;
        //}
        if (!eat_moves1.empty()) {
            return eat_moves1;
        }
        else
        {
            return eat_moves2;
        }

    }
    else if (nums2 == 2) {
        /*std::cout << "asdd" << std::endl;*/
        MoveEncoder encode({ 14,14 }, 5);
        for (int i = 0; i < 196; i++) {
            if (next_board.grid[i] != other_player) continue;
            for (int j = 0; j < i; j++) {
                if (next_board.grid[j] != other_player) continue;
                next_board.grid[i] = 0;
                next_board.grid[j] = 0;
                float opponent_score = next_board.special_moves1(other_player);
                float our_score = next_board.special_moves1(player);
                next_board.grid[i] = other_player;
                next_board.grid[j] = other_player;
                int candidate_index = encode.encode_eat({ i,j });
                candidate_scores.emplace_back(opponent_score * 1.5 - our_score, candidate_index);
            }
        }

        std::sort(candidate_scores.begin(), candidate_scores.end());
        // 提取前20个候选项或全部候选项（取决于 candidate_scores 的大小）
        int top_n = std::min(static_cast<int>(candidate_scores.size()), 20);
        for (int k = 0; k < top_n; ++k) {
            int min_index = candidate_scores[k].second;
            eat_moves.emplace_back(min_index);
        }
        
        return eat_moves;
    }
    //else {
    //    std::cout << "bbbbb" << std::endl;
    //}

}

std::vector<int> fly_strategy(GameState& game_state) {
    float pl_routes_score = game_state.board.fly_special_moves(game_state.next_player);
    float op_routes_score = game_state.board.fly_special_moves(other(game_state.next_player));
    /*std::cout << "asd " << pl_routes_score << std::endl;*/
    std::vector<std::vector<int>> legal_moves;
    if (pl_routes_score >= 10.5) {
        legal_moves = game_state.board.fly_legal_special(game_state.next_player);
    }
    else
    {
        legal_moves = game_state.board.fly_legal_moves(game_state.next_player);
    }
    
    if (legal_moves.empty()) {
        return {};
    }
    //for (const auto& move : legal_moves) {
    //    for (int value : move) {
    //        std::cout << value << " ";
    //    }
    //    std::cout << std::endl;
    //}
    std::vector<std::vector<int>> global_best_moves;
    float global_best_score = std::numeric_limits<int>::min();
    // Divide the work among threads
    int num_threads = std::thread::hardware_concurrency(); // Number of concurrent threads supported
    /*std::cout << num_threads << std::endl;*/
    std::vector<std::thread> threads;
    size_t moves_per_thread = legal_moves.size() / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        size_t start = i * moves_per_thread;
        size_t end = (i == num_threads - 1) ? legal_moves.size() : (i + 1) * moves_per_thread;
        std::vector<std::vector<int>> thread_moves(legal_moves.begin() + start, legal_moves.begin() + end);
        threads.emplace_back(fly_search_best_moves, game_state, thread_moves, std::ref(global_best_score), std::ref(global_best_moves)); // 不用 std::ref
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }

    // Randomly choose one of the best moves
    std::srand(static_cast<unsigned int>(std::time(0)));
    /*for (auto moves : global_best_moves) {
        game_state.board.Print_move(moves, game_state.next_player);
    }*/
    /*std::cout << global_best_moves.size() << std::endl;*/
    return global_best_moves[std::rand() % global_best_moves.size()];
}

std::mutex fly_mutex;
void fly_search_best_moves(GameState game_state, const std::vector<std::vector<int>>& moves, float& global_best_score, std::vector<std::vector<int>>& global_best_moves) {
    float local_best_score = std::numeric_limits<int>::min();
    std::vector<std::vector<int>> local_best_moves;

    for (const auto& possible_move : moves) {
        auto eat_move = fly_eat_strategy(game_state, possible_move);
        std::tuple<std::vector<int>, std::vector<int>> move = std::make_tuple(possible_move, eat_move);
        GameState next_state = game_state.fly_apply_move(move);
        float best_black = MIN_SCORE;
        float best_white = MIN_SCORE;
        float opponent_best_outcome = fly_alpha_beta_result(next_state, 1, best_black, best_white);
        float our_best_outcome = -opponent_best_outcome;

        //std::lock_guard<std::mutex> guard(mutex); // 保护全局变量
        if (our_best_outcome > local_best_score) {
            local_best_moves.clear();
            local_best_moves.push_back(possible_move);
            local_best_score = our_best_outcome;
        }
        else if (our_best_outcome == local_best_score) {
            local_best_moves.push_back(possible_move);
        }
    }

    std::lock_guard<std::mutex> guard(fly_mutex); // 再次保护全局变量
    if (local_best_score > global_best_score) {
        global_best_moves = local_best_moves;
        global_best_score = local_best_score;
    }
    else if (local_best_score == global_best_score) {
        global_best_moves.insert(global_best_moves.end(), local_best_moves.begin(), local_best_moves.end());
    }
}

float fly_alpha_beta_result(GameState& game_state, int max_depth, float best_black, float best_white) {

    if (max_depth == 0) {
        return fly_eval_fn(game_state);
    }
    float best_so_far = MIN_SCORE;
    std::vector<std::vector<int>> all_legal_moves = game_state.board.fly_legal_moves(game_state.next_player);
    for (const auto& candidate_move : all_legal_moves) {
        auto eat_move = fly_eat_strategy(game_state, candidate_move);
        std::tuple<std::vector<int>, std::vector<int>> move = std::make_tuple(candidate_move, eat_move);
        GameState next_state = game_state.fly_apply_move(move);
        float opponent_best_result = fly_alpha_beta_result(next_state, max_depth - 1, best_black, best_white);
        float our_result = -opponent_best_result;
        if (our_result > best_so_far) {
            best_so_far = our_result;
        }
        if (game_state.next_player == WHITE) {
            if (best_so_far > best_white) {
                best_white = best_so_far;
            }
            float outcome_for_black = -best_so_far;
            if (outcome_for_black < best_black) {
                return best_so_far;
            }
        }
        else if (game_state.next_player == BLACK) {
            if (best_so_far > best_black) {
                best_black = best_so_far;
            }
            float outcome_for_white = -best_so_far;
            if (outcome_for_white < best_white) {
                return best_so_far;
            }
        }
    }
    return best_so_far;
}



std::vector<std::vector<int>> combinations(const std::vector<int>& elements, size_t k) {
    std::vector<std::vector<int>> result;
    std::vector<bool> selected(elements.size(), false);
    std::fill(selected.end() - k, selected.end(), true);

    do {
        std::vector<int> combination;
        for (size_t i = 0; i < elements.size(); ++i) {
            if (selected[i]) {
                combination.push_back(elements[i]);
            }
        }
        result.push_back(combination);
    } while (std::next_permutation(selected.begin(), selected.end()));

    return result;
}

std::vector<int> fly_eat_strategy(GameState& game_state, const std::vector<int>& move) {
    Board next_board = game_state.board;
    next_board.go_take_stone(game_state.next_player, move);
    int nums = next_board.squire(game_state.next_player, move.back());
    std::vector<std::pair<float, int>> candidate_scores;
    std::vector<int> eat_moves;
    std::vector<int> empty_stones;
    Player other_player = other(game_state.next_player);
    for (int candidate_index = 0; candidate_index < 196; candidate_index++) {
        if (next_board.grid[candidate_index] == other_player) {
            empty_stones.emplace_back(candidate_index);
        }
    }
    if (nums == 1) {
        for (int candidate_index = 0; candidate_index < 196; candidate_index++) {
            if (next_board.grid[candidate_index] == other_player) {
                next_board.grid[candidate_index] = 0;
                float opponent_score = next_board.fly_special_moves(other_player);
                float our_score = next_board.fly_special_moves(game_state.next_player);
                next_board.grid[candidate_index] = other_player;
                candidate_scores.emplace_back(opponent_score * 1.5 - our_score, candidate_index);
            }
        }
        std::sort(candidate_scores.begin(), candidate_scores.end());
        eat_moves.emplace_back(candidate_scores.front().second);
    }
    else if (nums == 2) {
        for (int i = 0; i < 196; i++) {
            if (next_board.grid[i] != other_player) continue;
            for (int j = 0; j < i; j++) {
                if (next_board.grid[j] != other_player) continue;
                next_board.grid[i] = 0;
                next_board.grid[j] = 0;
                float opponent_score = next_board.fly_special_moves(other_player);
                float our_score = next_board.fly_special_moves(game_state.next_player);
                next_board.grid[i] = other_player;
                next_board.grid[j] = other_player;
                int candidate_index = (i * 196) + j;
                candidate_scores.emplace_back(opponent_score * 1.5 - our_score, candidate_index);
            }
        }

        std::sort(candidate_scores.begin(), candidate_scores.end());
        int min_index = candidate_scores.front().second;
        eat_moves.push_back(min_index / 196);  // i 的值
        eat_moves.push_back(min_index % 196);  // j 的值
    }
    else if (nums == 3) {
        auto comb = combinations(empty_stones, 3);
        std::vector<ScoredCombination> scoredCombinations;
        for (auto i : comb) {
            for (auto j : i) {
                next_board.grid[j] = 0;
            }
            float opponent_score = next_board.fly_special_moves(other_player);
            float our_score = next_board.fly_special_moves(game_state.next_player);
            float score = 1.5 * opponent_score - our_score;
            scoredCombinations.push_back({ i, score });
            for (auto h : i) {
                next_board.grid[h] = other_player;
            }
        }
        std::sort(scoredCombinations.begin(), scoredCombinations.end(), [](const ScoredCombination& a, const ScoredCombination& b) {
            return a.score_difference < b.score_difference;
            });

        // 返回分数差最小的组合
        return scoredCombinations.front().combination;
    }
    else if (nums == 4) {
        auto comb = combinations(empty_stones, 4);
        std::vector<ScoredCombination> scoredCombinations;
        for (auto i : comb) {
            for (auto j : i) {
                next_board.grid[j] = 0;
            }
            float opponent_score = next_board.fly_special_moves(other_player);
            float our_score = next_board.fly_special_moves(game_state.next_player);
            float score = 1.5 * opponent_score - our_score;
            scoredCombinations.push_back({ i, score });
            for (auto h : i) {
                next_board.grid[h] = other_player;
            }
        }
        std::sort(scoredCombinations.begin(), scoredCombinations.end(), [](const ScoredCombination& a, const ScoredCombination& b) {
            return a.score_difference < b.score_difference;
            });

        // 返回分数差最小的组合
        return scoredCombinations.front().combination;
    }
    else
    {
        return {};
    }
    return eat_moves;
}

float fly_eval_fn(GameState& game_state) {
    int player_stones = game_state.board.sum_stone(game_state.next_player);
    int opponent_stones = game_state.board.sum_stone(other(game_state.next_player));
    int stone_diff = player_stones - opponent_stones;

    float player_routes_score = game_state.board.fly_special_moves(game_state.next_player);
    float opponent_routes_score = game_state.board.fly_special_moves(other(game_state.next_player));
    float routes_diff = player_routes_score - opponent_routes_score * 1.5;

    return stone_diff * 0.6f + routes_diff * 0.4f;
}
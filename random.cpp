#include "random.h"
//布局专家，走子rl
std::tuple<std::vector<int>, std::vector<int>> RLBot1::select_move(GameState& game_state) {

    if (game_state.state == 1) {
        if (game_state.board.grid[90] == 0 || game_state.board.grid[105] == 0) {

            std::vector<int> legal_move;
            legal_move.reserve(1);

            bool isBothZero = (game_state.board.grid[90] == 0 && game_state.board.grid[105] == 0);
            if (isBothZero) {
                // 如果都为0，随机选择一个
                srand(static_cast<unsigned int>(time(0))); // 设置随机种子
                int choice = rand() % 2; // 生成0或1
                if (choice == 0) {
                    legal_move.emplace_back(90);
                }
                else {
                    legal_move.emplace_back(105);
                }
            }
            else {
                // 只有一个为0，选择那个为0的
                if (game_state.board.grid[90] == 0) {
                    legal_move.emplace_back(90);
                    /*std::cout << "90 "<< std::endl;*/
                }
                else {
                    legal_move.emplace_back(105);
                    /*std::cout << "105 " << std::endl;*/
                }
            }
            std::vector<int> eat_move;
            std::tuple<std::vector<int>, std::vector<int>> new_moves = std::make_tuple(legal_move, eat_move);
            return new_moves;
        }
        else
        {
            int point = layout_strategy(game_state);
            std::vector<int> eat_move = {};
            std::vector<int> layout_moves = { point };
            std::tuple<std::vector<int>, std::vector<int>> new_moves = std::make_tuple(layout_moves, eat_move);
            return new_moves;
        }
    }
    else if (game_state.state == 2)
    {
        std::shared_ptr<GameState> shared_game_state = std::make_shared<GameState>(game_state);
        auto [move, eat] = agent.select_move(shared_game_state);
        
        std::vector<int> moves1 = encoder->decode_move_index(move[0]);
        std::vector<std::vector<int>> legal_moves = game_state.board.singal_moves(game_state.next_player, moves1[0]);
        for (const auto& move : legal_moves) {
            if (!move.empty() && move.back() == moves1[1]) {
                // 如果移动的起始和结束位置与 eat_moves 匹配，添加到结果中
                moves1 = move;
                break;
            }
        }

        //shared_game_state = nullptr;
        //shared_game_state.reset();
        std::vector<int> eat_moves1 = encoder->decode_eat_index(eat[0]);

        /* std::cout << "expert ";
         game_state.board.Print_move({}, eat_moves1, game_state.next_player);*/
        return { moves1,eat_moves1 };


    }
}

//布局rl，走子rl
std::tuple<std::vector<int>, std::vector<int>> RLBot::select_move(GameState& game_state) {

    if (game_state.state == 1) {
        std::vector<int> eat = {};
        if (game_state.board.grid[90] == 0 || game_state.board.grid[105] == 0) {

            std::vector<int> legal_move;
            legal_move.reserve(1);

            bool isBothZero = (game_state.board.grid[90] == 0 && game_state.board.grid[105] == 0);
            if (isBothZero) {
                // 如果都为0，随机选择一个
                srand(static_cast<unsigned int>(time(0))); // 设置随机种子
                int choice = rand() % 2; // 生成0或1
                if (choice == 0) {
                    legal_move.emplace_back(90);
                }
                else {
                    legal_move.emplace_back(105);
                }
            }
            else {
                // 只有一个为0，选择那个为0的
                if (game_state.board.grid[90] == 0) {
                    legal_move.emplace_back(90);
                    /*std::cout << "90 "<< std::endl;*/
                }
                else {
                    legal_move.emplace_back(105);
                    /*std::cout << "105 " << std::endl;*/
                }
            }
            
            return {legal_move,eat};
        }
        else
        {
            std::shared_ptr<GameState> shared_game_state = std::make_shared<GameState>(game_state);
            auto moves = layout_agent.select_move(shared_game_state);
            //shared_game_state = nullptr;
            //shared_game_state.reset();
            return { moves,eat };
        }

    }
    else if (game_state.state == 2)
    {
        std::shared_ptr<GameState> shared_game_state = std::make_shared<GameState>(game_state);
        auto [move, eat] = agent.select_move(shared_game_state);

        std::vector<int> moves1 = encoder->decode_move_index(move[0]);
        std::vector<std::vector<int>> legal_moves = game_state.board.singal_moves(game_state.next_player, moves1[0]);
        for (const auto& move : legal_moves) {
            if (!move.empty() && move.back() == moves1[1]) {
                // 如果移动的起始和结束位置与 eat_moves 匹配，添加到结果中
                moves1 = move;
                break;
            }
        }

        //shared_game_state = nullptr;
        //shared_game_state.reset();
        std::vector<int> eat_moves1 = encoder->decode_eat_index(eat[0]);

        /* std::cout << "expert ";
         game_state.board.Print_move({}, eat_moves1, game_state.next_player);*/
        return { moves1,eat_moves1 };


    }
}


//shiyao专家
std::tuple<std::vector<int>, std::vector<int>> ExpertBot::select_move(GameState& game_state) {

    if (game_state.state == 1) {
        if (game_state.board.grid[90] == 0 || game_state.board.grid[105] == 0) {

            std::vector<int> legal_move;
            legal_move.reserve(1);

            bool isBothZero = (game_state.board.grid[90] == 0 && game_state.board.grid[105] == 0);
            if (isBothZero) {
                // 如果都为0，随机选择一个
                srand(static_cast<unsigned int>(time(0))); // 设置随机种子
                int choice = rand() % 2; // 生成0或1
                if (choice == 0) {
                    legal_move.emplace_back(90);
                }
                else {
                    legal_move.emplace_back(105);
                }
            }
            else {
                // 只有一个为0，选择那个为0的
                if (game_state.board.grid[90] == 0) {
                    legal_move.emplace_back(90);
                    /*std::cout << "90 "<< std::endl;*/
                }
                else {
                    legal_move.emplace_back(105);
                    /*std::cout << "105 " << std::endl;*/
                }
            }
            std::vector<int> eat_move;
            std::tuple<std::vector<int>, std::vector<int>> new_moves = std::make_tuple(legal_move, eat_move);
            return new_moves;
        }
        else
        {
            int point = agent.expert_layout(game_state);
            std::vector<int> eat_move = {};
            std::vector<int> layout_moves = { point };
            std::tuple<std::vector<int>, std::vector<int>> new_moves = std::make_tuple(layout_moves, eat_move);
            return new_moves;
        }
    }
    else if (game_state.state == 2)
    {
        auto moves = agent.walk_strategy(game_state);
        return moves;

    }
}

//my专家
std::tuple<std::vector<int>, std::vector<int>> ExpertBotm::select_move(GameState& game_state) {

    if (game_state.state == 1) {
        if (game_state.board.grid[90] == 0 || game_state.board.grid[105] == 0) {

            std::vector<int> legal_move;
            legal_move.reserve(1);

            bool isBothZero = (game_state.board.grid[90] == 0 && game_state.board.grid[105] == 0);
            if (isBothZero) {
                // 如果都为0，随机选择一个
                srand(static_cast<unsigned int>(time(0))); // 设置随机种子
                int choice = rand() % 2; // 生成0或1
                if (choice == 0) {
                    legal_move.emplace_back(90);
                }
                else {
                    legal_move.emplace_back(105);
                }
            }
            else {
                // 只有一个为0，选择那个为0的
                if (game_state.board.grid[90] == 0) {
                    legal_move.emplace_back(90);
                    /*std::cout << "90 "<< std::endl;*/
                }
                else {
                    legal_move.emplace_back(105);
                    /*std::cout << "105 " << std::endl;*/
                }
            }
            std::vector<int> eat_move;
            std::tuple<std::vector<int>, std::vector<int>> new_moves = std::make_tuple(legal_move, eat_move);
            return new_moves;
        }
        else
        {
            int point = layout_strategy(game_state);
            std::vector<int> eat_move;
            std::vector<int> layout_moves = { point };
            std::tuple<std::vector<int>, std::vector<int>> new_moves = std::make_tuple(layout_moves, eat_move);
            return new_moves;
        }
    }
    else if (game_state.state == 2)
    {
        std::vector<int> moves = walk_strategy(game_state);
        std::vector<int> eat_moves = eat_strategy(game_state, moves);
        return { moves,eat_moves };
        

    }
}

std::tuple<std::vector<int>, std::vector<int>> bisaiBot::select_move(GameState& game_state) {

    if (game_state.state == 1) {
        if (game_state.board.grid[90] == 0 || game_state.board.grid[105] == 0) {

            std::vector<int> legal_move;
            legal_move.reserve(1);

            bool isBothZero = (game_state.board.grid[90] == 0 && game_state.board.grid[105] == 0);
            if (isBothZero) {
                // 如果都为0，随机选择一个
                srand(static_cast<unsigned int>(time(0))); // 设置随机种子
                int choice = rand() % 2; // 生成0或1
                if (choice == 0) {
                    legal_move.emplace_back(90);
                }
                else {
                    legal_move.emplace_back(105);
                }
            }
            else {
                // 只有一个为0，选择那个为0的
                if (game_state.board.grid[90] == 0) {
                    legal_move.emplace_back(90);
                    /*std::cout << "90 "<< std::endl;*/
                }
                else {
                    legal_move.emplace_back(105);
                    /*std::cout << "105 " << std::endl;*/
                }
            }
            std::vector<int> eat_move;
            std::tuple<std::vector<int>, std::vector<int>> new_moves = std::make_tuple(legal_move, eat_move);
            return new_moves;
        }
        else
        {
            int point = layout_strategy(game_state);
            std::vector<int> eat_move;
            std::vector<int> layout_moves = { point };
            std::tuple<std::vector<int>, std::vector<int>> new_moves = std::make_tuple(layout_moves, eat_move);
            return new_moves;
        }
    }
    else if (game_state.state == 2)
    {
        std::vector<int> moves = walk_strategy(game_state);
        std::vector<int> eat_moves = eat_strategy(game_state, moves);
        return { moves,eat_moves };
    }
    else
    {
        std::vector<int> moves = fly_strategy(game_state);
        std::vector<int> eat_moves = fly_eat_strategy(game_state, moves);
        return { moves,eat_moves };

    }
}
//#include<vector>
#include"goboard.h"


Board::Board(int num_rows, int num_cols) : num_rows(num_rows), num_cols(num_cols) {
    grid = std::vector<int>(num_rows * num_cols, 0);
}


std::string Board::PointToString(char row, int col) {
    std::stringstream ss;
    ss << "Point(" << row << ", " << col << ")";
    return ss.str();
}

void Board::Print_move(std::vector<int> moves, std::vector<int> eat_moves, Player player) {
    std::vector<std::string> point_strings;
    std::vector<std::string> eat_point_strings;
    for (auto& move : moves) {
        int row = move / 14 + 1;
        int col = move % 14 + 1;
        char letter = 'A' + row - 1;
        point_strings.push_back(PointToString(letter, col));
    }
    if (!eat_moves.empty()) {
        for (auto& eat_move : eat_moves) {
            int row = eat_move / 14 + 1;
            int col = eat_move % 14 + 1;
            char letter = 'A' + row - 1;
            eat_point_strings.push_back(PointToString(letter, col));
        }
    }
    std::cout << player << "[";
    for (size_t i = 0; i < point_strings.size(); ++i) {
        std::cout << point_strings[i];
        if (i != point_strings.size() - 1) {
            std::cout << ",";
        }
    }
    if (!eat_point_strings.empty()) {
        std::cout << ",eat:";
        for (size_t i = 0; i < eat_point_strings.size(); ++i) {
            std::cout << eat_point_strings[i];
            if (i != eat_point_strings.size() - 1) {
                std::cout << ",";
            }
        }
    }
    std::cout << "]" << std::endl;
}

void Board::displayGrid()
{
    //std::mutex pri_mutex;
    //std::lock_guard<std::mutex> lock(pri_mutex);
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            if (grid[i * num_cols + j] == 1) {
                std::cout << 'x' << "  ";
            }
            else if (grid[i * num_cols + j] == 2)
            {
                std::cout << 'o' << "  ";
            }
            else
            {
                std::cout << '.' << "  ";
            }

        }
        std::cout << static_cast<char>('A' + i) << std::endl;
    }

    // 打印行标识数字
    for (int i = 0; i < num_cols; i++) {
        if (i < 9) {
            std::cout << i + 1 << "  ";
        }
        else
        {
            std::cout << i + 1 << " ";
        }

    }
    std::cout << std::endl;
}

int& Board::at(int row, int col) {
    return grid[row * num_cols + col];
}

const int& Board::at(int row, int col) const {
    return grid[row * num_cols + col];
}



//std::vector<std::vector<Point>> Board::legal_moves(Player next_player, int game_state) {
//    /*std::vector<Point> legal_moves;*/
//    if (game_state == 1) {
//        if (grid[90] == 0 || grid[105] == 0) {
//            std::vector<std::vector<Point>> legal_moves;
//            std::vector<Point> legal_move;
//            legal_move.reserve(1);
//            if (grid[90] == 0) {
//                legal_move.emplace_back(Point(91 / num_cols + 1, 91 % num_cols));
//                /*std::cout << "90 "<< std::endl;*/
//            }
//            else {
//                legal_move.emplace_back(Point(105 / num_cols + 1, 105 % num_cols + 1));
//                /*std::cout << "104 " << std::endl;*/
//            }
//            legal_moves.emplace_back(legal_move);
//            return legal_moves;
//        }
//        else
//        {
//            std::vector<std::vector<Point>> legal_moves;
//            std::vector<Point> legal_move;
//            legal_move.reserve(num_rows * num_cols + 1);
//            for (int r = 1; r < num_rows + 1; r++) {
//                for (int c = 1; c < num_cols + 1; c++) {
//                    int candidate_index = (r - 1) * num_cols + (c - 1);
//                    if (grid[candidate_index] == 0) {
//                        legal_move.emplace_back(Point(r, c));
//                    }
//                    /*std::cout << candidate_index << std::endl;*/
//                }
//            }
//            legal_moves.emplace_back(legal_move);
//            return legal_moves;
//        }
//    }
//    else if (game_state == 2) {
//        std::vector<std::vector<Point>> legal_moves;
//        legal_moves.reserve(1500);
//        
        //for (int r = 1; r <= num_rows; r++) {
        //    for (int c = 1; c <= num_cols; c++) {
        //        int candidate_index = (r - 1) * num_cols + (c - 1);
        //        Point p(r, c);
        //        if (grid[candidate_index] == next_player) {
        //            for (const Point& neighbor : neighbors(p)) {
        //                std::vector<Point> legal_move;
        //                if (1 <= neighbor.row && neighbor.row <= num_rows &&
        //                    1 <= neighbor.col && neighbor.col <= num_cols &&
        //                    grid[(neighbor.row - 1) * num_cols + (neighbor.col - 1)] == 0) {
        //                    legal_move.emplace_back(Point(r, c));
        //                    legal_move.emplace_back(neighbor);
        //                    legal_moves.emplace_back(legal_move);
        //                }
        //            }
        //            go_find_jumpmoves(next_player, p, { p }, legal_moves);
        //        }
        //    }
        //}
//        

//        return legal_moves;
//    }
//}

std::vector<std::vector<int>> Board::legal_moves(Player next_player, int game_state) {
    /*std::vector<Point> legal_moves;*/
    if (game_state == 1) {
        if (grid[90] == 0 || grid[105] == 0) {
            std::vector<std::vector<int>> legal_moves;
            std::vector<int> legal_move;
            legal_move.reserve(1);

            bool isBothZero = (grid[90] == 0 && grid[105] == 0);
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
                if (grid[90] == 0) {
                    legal_move.emplace_back(90);
                    /*std::cout << "90 "<< std::endl;*/
                }
                else {
                    legal_move.emplace_back(105);
                    /*std::cout << "105 " << std::endl;*/
                }
            }
            legal_moves.emplace_back(legal_move);
            return legal_moves;
        }
        else
        {
            std::vector<std::vector<int>> legal_moves;
            std::vector<int> legal_move;
            legal_move.reserve(num_rows * num_cols + 1);
            for (int candidate_index = 0; candidate_index < num_rows * num_cols; candidate_index++) {
                if (grid[candidate_index] == 0) {
                    legal_move.emplace_back(candidate_index);
                }
                /*std::cout << candidate_index << std::endl;*/
            }
            legal_moves.emplace_back(legal_move);
            return legal_moves;
        }
    }
    else if (game_state == 2) {
        std::vector<std::vector<int>> legal_moves;
        legal_moves.reserve(1500);
        std::vector<int> legal_move;

        for (int candidate_index = 0; candidate_index < num_rows * num_cols; candidate_index++) {
            if (grid[candidate_index] == next_player) {
                for (const int& neighbor : neighbors2(candidate_index)) {
                    if (grid[neighbor] == 0) {
                        legal_move.clear();
                        legal_move.emplace_back(candidate_index);
                        legal_move.emplace_back(neighbor);
                        legal_moves.emplace_back(legal_move);
                    }
                }
                go_find_jumpmoves(next_player, candidate_index, { candidate_index }, legal_moves);
            }
        }
        /* for (const auto& moves : legal_moves) {
             //for (const auto& move : moves) {
                 std::cout << move << " ";
             }
             std::cout << std::endl;
         }*/
        return legal_moves;
    }
}

//void Board::go_find_jumpmoves(Player next_player, int index, std::vector<Point> road, std::vector<std::vector<Point>>& roads) {
//    if (!is_valid_jumpmove(next_player, point)) {
//        return;
//    }
//    for (const auto& neighbor : neighbors1(point)) {
//        if (1 <= neighbor.second.row && neighbor.second.row <= num_rows &&
//            1 <= neighbor.second.col && neighbor.second.col <= num_cols &&
//            grid[(neighbor.second.row - 1) * 14 + neighbor.second.col - 1] == other(next_player)) {
//            Point s = neighbors1(neighbor.second)[neighbor.first];
//            if (1 <= s.row && s.row <= num_rows &&
//                1 <= s.col && s.col <= num_cols &&
//                grid[(s.row - 1) * num_rows + num_cols - 1] == 0) {
//                road.emplace_back(neighbor.second);
//                road.emplace_back(s);
//                roads.emplace_back(road);
//                /*std::cout <<"asd"<< s << std::endl;*/
//                grid[(s.row - 1) * num_rows + num_cols - 1] = next_player;
//                go_find_jumpmoves(next_player, s, road, roads);
//                grid[(s.row - 1) * num_rows + num_cols - 1] = 0;
//                road.pop_back();
//                road.pop_back();
//            }
//        }
//    }
//}
//
//bool Board::is_valid_jumpmove(Player next_player, Point point) {
//    /*std::cout << "www" << std::endl;*/
//    if (grid[(point.row - 1) * num_cols + (point.col - 1)] == other(next_player) ||
//        grid[(point.row - 1) * num_cols + (point.col - 1)] == 0) {
//        return false;
//    }
//    for (const auto& neighbor : neighbors1(point)) {
//        if (1 <= neighbor.second.row && neighbor.second.row <= num_rows &&
//            1 <= neighbor.second.col && neighbor.second.col <= num_cols &&
//            grid[(neighbor.second.row - 1) * 14 + neighbor.second.col - 1] == other(next_player)) {
//            Point s = neighbors1(neighbor.second)[neighbor.first];
//            if (1 <= s.row && s.row <= num_rows &&
//                1 <= s.col && s.col <= num_cols &&
//                grid[(s.row - 1) * num_rows + num_cols - 1] == 0) {
//                return true;
//            }
//        }
//    }
//    return false;
//}

//void Board::place_stone(Player next_player, std::vector<Point> pos) {
//    grid[(pos[0].row - 1) * num_cols + (pos[0].col - 1)] = next_player;
//}

void Board::go_find_jumpmoves(Player next_player, int point, std::vector<int> road, std::vector<std::vector<int>>& roads) {
    if (!is_valid_jumpmove(next_player, point)) {
        return;
    }

    for (const auto& neighbor : neighbors3(point)) {
        if (grid[neighbor.second] == other(next_player)) {
            const auto& neighbor_map = neighbors3(neighbor.second);
            auto it = neighbor_map.find(neighbor.first);
            if (it != neighbor_map.end()) {
                int s = it->second;
                if (grid[s] == 0) {
                    road.emplace_back(neighbor.second);
                    road.emplace_back(s);
                    roads.emplace_back(road);
                    /*std::cout <<"asd"<< s << std::endl;*/
                    grid[s] = next_player;
                    go_find_jumpmoves(next_player, s, road, roads);
                    grid[s] = 0;
                    road.pop_back();
                    road.pop_back();
                }
            }
        }
    }
}

bool Board::is_valid_jumpmove(Player next_player, int point) {
    /*std::cout << "www" << std::endl;*/
    if (grid[point] == other(next_player) ||
        grid[point] == 0) {
        return false;
    }
    for (const auto& neighbor : neighbors3(point)) {
        if (grid[neighbor.second] == other(next_player)) {
            const auto& neighbor_map = neighbors3(neighbor.second);
            auto it = neighbor_map.find(neighbor.first);
            if (it != neighbor_map.end()) {
                int s = it->second;
                if (grid[s] == 0) {
                    return true;
                }
            }
        }
    }
    return false;
}

void Board::place_stone(Player next_player, std::vector<int> pos) {
    grid[pos[0]] = next_player;
}

//void Board::go_take_stone(Player next_player, std::vector<Point> pos) {
//    if (grid[(pos[0].row - 1) * num_cols + (pos[0].col - 1)] != next_player) {
//        std::cerr << "Assertion failed: grid[pos[0].row - 1][pos[0].col - 1] != next_player" << std::endl;
//    }
//
//    for (const Point& po : pos) {
//        grid[(po.row - 1) * num_cols + (po.col - 1)] = 0;
//
//    }
//    grid[(pos.back().row - 1) * num_cols + (pos.back().col - 1)] = next_player;
    /*int t = 0;
    std::vector<Point> point_neighbors = neighbors(pos[0]);
    for (const Point& neighbor : point_neighbors) {
        if (pos[1] == neighbor) {
            t = 1;
            break;
        }
    }

    if (t == 1) {
        grid[pos[0].row - 1][pos[0].col - 1] = 0;
        grid[pos.back().row - 1][pos.back().col - 1] = next_player;
    }
    else {
        std::vector<std::vector<Point>> jump_moves;
        std::vector<Point> move;
        go_find_jumpmoves1(next_player, pos[0], { pos[0] }, jump_moves);

        for (const std::vector<Point>& jump_move : jump_moves) {
            if (jump_move.back() == pos.back()) {
                move = jump_move;
                break;
            }
        }

        for (const Point& mo : move) {
            grid[mo.row - 1][mo.col - 1] = 0;
        }

        grid[pos.back().row - 1][pos.back().col - 1] = next_player;*/
        //}

void Board::go_take_stone(Player next_player, std::vector<int> pos) {
    if (grid[pos[0]] != next_player) {
        std::cout << "Assertion failed: grid[pos[0].row - 1][pos[0].col - 1] != next_player" << std::endl;
    }
   /* displayGrid();
    Print_move(pos, {}, next_player);*/
    for (const int& po : pos) {
        grid[po] = 0;

    }
    grid[pos.back()] = next_player;
}

void Board::all_take_stone(Player next_player, const std::vector<int>& pos) {
    if (grid[pos[0]] != next_player) {
        std::cout << "Assertion failed: Expected grid position to be occupied by the current player." << std::endl;
        return;
    }

    int last_empty_index = -1;
    for (int i = pos.size() - 1; i >= 0; --i) {
        if (grid[pos[i]] == 0) {
            last_empty_index = i;
            break;
        }
    }

    for (int j = 0; j < pos.size(); ++j) {
        if (j == last_empty_index) {
            grid[pos[j]] = next_player;
        }
        else {
            grid[pos[j]] = 0;
        }
    }
}

//int Board::squire(const Player next_player, Point point) {
//    /*std::cout << point << std::endl;*/
//    int s = 0;
//    if (point.col - 2 >= 0 && point.row + 1 <= num_rows) {
//        if (grid[(point.row - 1) * num_cols + (point.col - 2)] == next_player &&
//            grid[point.row * num_cols + (point.col - 2)] == next_player &&
//            grid[point.row * num_cols + (point.col - 1)] == next_player) {
//            s += 1;
//            /*std::cout << 1 << std::endl;*/
//        }
//
//    }
//    if (point.col + 1 <= num_cols && point.row + 1 <= num_rows) {
//        if (grid[(point.row - 1) * num_cols + point.col] == next_player &&
//            grid[point.row * num_cols + point.col] == next_player &&
//            grid[point.row * num_cols + (point.col - 1)] == next_player) {
//            s += 1;
//            /*std::cout << 2 << std::endl;*/
//        }
//    }
//    if (point.row - 2 >= 0 && point.col - 2 >= 0) {
//        if (grid[(point.row - 1) * num_cols + (point.col - 2)] == next_player &&
//            grid[(point.row - 2) * num_cols + (point.col - 2)] == next_player &&
//            grid[(point.row - 2) * num_cols + (point.col - 1)] == next_player) {
//            s += 1;
//            /*std::cout << 3 << std::endl;*/
//        }
//    }
//    if (point.row - 2 >= 0 && point.col + 1 <= num_cols) {
//        if (grid[(point.row - 2) * num_cols + (point.col - 1)] == next_player &&
//            grid[(point.row - 2) * num_cols + point.col] == next_player &&
//            grid[(point.row - 1) * num_cols + point.col] == next_player) {
//            s += 1;
//            /*std::cout << point << std::endl;
//            std::cout << (point.row - 2) * num_cols + (point.col - 1) << (point.row - 2) * num_cols + point.col << (point.row - 1) * num_cols + point.col <<std::endl;
//            std::cout << grid[(point.row - 2) * num_cols + (point.col - 1)] << grid[(point.row - 2) * num_cols + point.col] << grid[(point.row - 1) * num_cols + point.col] << std::endl;
//            std::cout << 4 << std::endl;*/
//        }
//    }
//    return s;
//}

int Board::squire(const Player next_player, int point) {
    /*std::cout << point << std::endl;*/
    int s = 0;
    int row = point / num_cols + 1;
    int col = point % num_cols + 1;
    if (col - 2 >= 0 && row + 1 <= num_rows) {
        if (grid[(row - 1) * num_cols + (col - 2)] == next_player &&
            grid[row * num_cols + (col - 2)] == next_player &&
            grid[row * num_cols + (col - 1)] == next_player) {
            s += 1;
            /*std::cout << 1 << std::endl;*/
        }

    }
    if (col + 1 <= num_cols && row + 1 <= num_rows) {
        if (grid[(row - 1) * num_cols + col] == next_player &&
            grid[row * num_cols + col] == next_player &&
            grid[row * num_cols + (col - 1)] == next_player) {
            s += 1;
            /*std::cout << 2 << std::endl;*/
        }
    }
    if (row - 2 >= 0 && col - 2 >= 0) {
        if (grid[(row - 1) * num_cols + (col - 2)] == next_player &&
            grid[(row - 2) * num_cols + (col - 2)] == next_player &&
            grid[(row - 2) * num_cols + (col - 1)] == next_player) {
            s += 1;
            /*std::cout << 3 << std::endl;*/
        }
    }
    if (row - 2 >= 0 && col + 1 <= num_cols) {
        if (grid[(row - 2) * num_cols + (col - 1)] == next_player &&
            grid[(row - 2) * num_cols + col] == next_player &&
            grid[(row - 1) * num_cols + col] == next_player) {
            s += 1;
            /*std::cout << point << std::endl;
            std::cout << (point.row - 2) * num_cols + (point.col - 1) << (point.row - 2) * num_cols + point.col << (point.row - 1) * num_cols + point.col <<std::endl;
            std::cout << grid[(point.row - 2) * num_cols + (point.col - 1)] << grid[(point.row - 2) * num_cols + point.col] << grid[(point.row - 1) * num_cols + point.col] << std::endl;
            std::cout << 4 << std::endl;*/
        }
    }
    return s;
}

//std::vector<Point> Board::eat(Player next_player, int nums) {
//    std::vector<Point> eat_moves;
//    bool found = false;
//    for (int r = 1; r < num_rows + 1; r++) {
//        for (int c = 1; c < num_cols + 1; c++) {
//            int candidate_index = (r - 1) * num_cols + (c - 1);
//            Point p(r, c);
//            if (grid[candidate_index] == other(next_player)) {
//                eat_moves.emplace_back(p);
//                found = true;
//                break;
//            }
//        }
//        if (found) {
//            break;  // 终止外部循环
//        }
//
//    }
//    return eat_moves;
//}

std::vector<int> Board::eat(Player next_player, int nums) {
    std::vector<int> eat_moves;
    int found = 0;
    for (int candidate_index = 0; candidate_index < num_rows * num_cols; candidate_index++) {
        if (found == nums) {
            break;  // 终止外部循环
        }
        if (grid[candidate_index] == other(next_player)) {
            eat_moves.emplace_back(candidate_index);
            found++;
        }
    }
    return eat_moves;
}

std::vector<int> Board::eat1(Player next_player, int nums) {
    std::vector<int> eat_moves;
    // 使用一个vector存储索引和对应的得分
    std::vector<std::pair<float, int>> candidate_scores;

    for (int candidate_index = 0; candidate_index < num_rows * num_cols; candidate_index++) {
        // 只有对手的棋子才进行评分
        if (grid[candidate_index] == other(next_player)) {
            // 暂时置为0，模拟“吃”掉对方棋子
            grid[candidate_index] = 0;
            float opponent_score = special_moves(other(next_player));
            // 恢复棋盘状态
            grid[candidate_index] = other(next_player);
            // 将得分和索引存储起来
            candidate_scores.emplace_back(opponent_score, candidate_index);
        }
    }

    // 按得分升序排序
    std::sort(candidate_scores.begin(), candidate_scores.end());

    // 选择得分最小的nums个索引加入到eat_moves中
    for (int i = 0; i < nums && i < candidate_scores.size(); i++) {
        eat_moves.emplace_back(candidate_scores[i].second);
    }

    return eat_moves;
}

//void legal_moves(Player next_player, int game_state) {
//    /*std::vector<Point> legal_moves;*/
//    //for (int i = 0; i < grid.size(); i++) {
//    //    std::cout << "grid[" << i << "]: " << grid[i] << std::endl;
//    //}
//    if (game_state == 1) {
//        std::vector<Point> legal_moves;
//        legal_moves.reserve(num_rows * num_cols + 1);
//        for (int r = 1; r < num_rows + 1; r++) {
//            for (int c = 1; c < num_cols + 1; c++) {
//                int candidate_index = (r - 1) * num_cols + (c - 1);
//                /*std::cout << grid[candidate_index]<< std::endl;*/
//                if (grid[candidate_index] == 0) {
//                    std::cout << "grid"<<candidate_index << std::endl;
//                    legal_moves.emplace_back(Point(r, c));
//                }
//                /*std::cout << candidate_index << std::endl;*/
//            }
//        }
//  
//    }
//}

bool Board::is_full_grid() {
    for (int i = 0; i < grid.size(); i++) {
        if (grid[i] == 0) {
            return false;
        }
    }
    return true;
}

//bool Board::is_valid_move_layout(Point move) {
//    if (grid[(move.row - 1) * num_rows + move.col - 1] == 0) {
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}

bool Board::is_valid_move_layout(int move) {
    if (grid[move] == 0) {
        return true;
    }
    else
    {
        return false;
    }
}

bool Board::is_valid_move_move(Player next_player, const std::vector<int>& move) {
    /*displayGrid();*/
    /*std::cout << move[0] << "  " << grid[move[0]] << std::endl;*/
    if (grid[move[0]] != next_player) {
        return false;
    }

    std::vector<int> neighbours = neighbors2(move[0]);

    // 检查是否为直接相邻的合法移动
    if (std::find(neighbours.begin(), neighbours.end(), move[1]) != neighbours.end()) {
        return grid[move[1]] == 0;
    }

    // 检查是否为合法的跳跃移动
    std::vector<std::vector<int>> jump_moves;

    go_find_jumpmoves(next_player, move[0], { move[0] }, jump_moves);

    for (const auto& jump_move : jump_moves) {
        if (jump_move.back() == move[1]) {
            return true;
        }
    }

    return false;
}

bool Board::is_valid_eat_move(Player next_player, const std::vector<int>& moves, const std::vector<int>& eat_moves) {
    grid[moves[0]] = 0;
    int s = squire(next_player, moves.back());
    if (eat_moves.size() != s) {
        grid[moves[0]] = next_player;
        return false;
    }
    if (!eat_moves.empty()) {
        for (auto eat_move : eat_moves) {
            if (grid[eat_move] == next_player || grid[eat_move] == 0 ||
                std::any_of(moves.begin(), moves.end(), [&](int move) { return eat_move == move && grid[move] == other(next_player); })) {
                grid[moves[0]] = next_player;
                return false;
            }
        }
        grid[moves[0]] = next_player;
        return true;
    }
    grid[moves[0]] = next_player;
    return true;
}

std::vector<std::vector<int>> Board::all_legal_moves(Player next_player, int game_state) {
    /*std::vector<Point> legal_moves;*/
    if (game_state == 1) {
        if (grid[90] == 0 || grid[105] == 0) {
            std::vector<std::vector<int>> legal_moves;
            std::vector<int> legal_move;
            legal_move.reserve(1);

            bool isBothZero = (grid[90] == 0 && grid[105] == 0);
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
                if (grid[90] == 0) {
                    legal_move.emplace_back(90);
                    /*std::cout << "90 "<< std::endl;*/
                }
                else {
                    legal_move.emplace_back(105);
                    /*std::cout << "105 " << std::endl;*/
                }
            }
            legal_moves.emplace_back(legal_move);
            return legal_moves;
        }
        else
        {
            std::vector<std::vector<int>> legal_moves;
            std::vector<int> legal_move;
            legal_move.reserve(num_rows * num_cols + 1);
            for (int candidate_index = 0; candidate_index < num_rows * num_cols; candidate_index++) {
                if (grid[candidate_index] == 0) {
                    legal_move.emplace_back(candidate_index);
                }
                /*std::cout << candidate_index << std::endl;*/
            }
            legal_moves.emplace_back(legal_move);
            return legal_moves;
        }
    }
    else if (game_state == 2) {
        std::vector<std::vector<int>> legal_moves;
        legal_moves.reserve(20000);
        std::vector<int> legal_move;
        Player other_player = other(next_player);
        for (int candidate_index = 0; candidate_index < num_rows * num_cols; candidate_index++) {
            if (grid[candidate_index] != next_player) {
                continue;
            }

            for (const int& neighbor : neighbors2(candidate_index)) {
                if (grid[neighbor] != 0) {
                    continue;
                }

                legal_move.clear();
                legal_move.emplace_back(candidate_index);
                legal_move.emplace_back(neighbor);

                grid[candidate_index] = 0;
                int squire_nums = squire(next_player, neighbor);

                switch (squire_nums) {
                case 1:
                    for (int index = 0; index < 196; index++) {
                        if (grid[index] == other_player) {
                            legal_move.emplace_back(index);
                            legal_moves.emplace_back(legal_move);
                            legal_move.pop_back();
                        }
                    }
                    break;

                case 2:
                    for (int i = 0; i < 196; i++) {
                        if (grid[i] != other_player) continue;
                        legal_move.emplace_back(i);
                        for (int j = 0; j < i; j++) {
                            if (grid[j] != other_player) continue;
                            legal_move.emplace_back(j);
                            legal_moves.emplace_back(legal_move);
                            legal_move.pop_back();
                        }
                        legal_move.pop_back();
                    }
                    break;

                default:
                    legal_moves.push_back(legal_move);
                    break;
                }

                grid[candidate_index] = next_player;  // Restore the spot
            }

            all_find_jumpmoves(next_player, candidate_index, { candidate_index }, legal_moves);
        }
        /* for (const auto& moves : legal_moves) {
             for (const auto& move : moves) {
                 std::cout << move << " ";
             }
             std::cout << std::endl;
         }*/
        return legal_moves;
    }
}

void Board::all_find_jumpmoves(Player next_player, int point, std::vector<int> road, std::vector<std::vector<int>>& roads) {
    if (!is_valid_jumpmove(next_player, point)) {
        return;
    }
    Player other_player = other(next_player);
    for (const auto& neighbor : neighbors3(point)) {
        if (grid[neighbor.second] == other_player) {
            const auto& neighbor_map = neighbors3(neighbor.second);
            auto it = neighbor_map.find(neighbor.first);
            if (it != neighbor_map.end()) {
                int s = it->second;
                if (grid[s] == 0) {
                    road.emplace_back(neighbor.second);
                    road.emplace_back(s);
                    grid[neighbor.second] = 0;
                    int squire_nums = squire(next_player, s);
                    switch (squire_nums) {
                    case 1:
                        for (int index = 0; index < 196; index++) {
                            if (grid[index] == other_player) {
                                road.emplace_back(index);
                                roads.emplace_back(road);
                                road.pop_back();
                            }
                        }
                        break;

                    case 2:
                        for (int i = 0; i < 196; i++) {
                            if (grid[i] != other_player) continue;
                            road.emplace_back(i);
                            for (int j = 0; j < i; j++) {
                                if (grid[j] != other_player) continue;
                                road.emplace_back(j);
                                roads.emplace_back(road);
                                road.pop_back();
                            }
                            road.pop_back();
                        }
                        break;

                    default:
                        roads.push_back(road);
                        break;
                    }

                    /*std::cout <<"asd"<< s << std::endl;*/
                    grid[s] = next_player;
                    all_find_jumpmoves(next_player, s, road, roads);
                    grid[neighbor.second] = other_player;
                    grid[s] = 0;
                    road.pop_back();
                    road.pop_back();
                }
            }
        }
    }
}

std::vector<Point> Board::same_stone(Player next_player) {
    std::vector<Point> stones;
    stones.reserve(100);
    for (int r = 1; r < num_rows + 1; r++) {
        for (int c = 1; c < num_cols + 1; c++) {
            int candidate_index = (r - 1) * num_cols + (c - 1);
            Point stone(r, c);
            if (grid[candidate_index] == next_player) {
                stones.emplace_back(stone);
            }
        }
    }
    return stones;
}

int Board::sum_stone(Player next_player) {
    int stones = 0;
    for (int r = 0; r < num_rows * num_cols; r++) {
        if (grid[r] == next_player) {
            stones++;
        }

    }
    return stones;
}

float Board::special_moves(Player next_player) {
    std::unordered_map<int, float> nums_dict;
    for (int candidate_index = 0; candidate_index < num_rows * num_cols; candidate_index++) {
        if (grid[candidate_index] == next_player) {
            grid[candidate_index] = 0;
            for (const int& neighbor : neighbors2(candidate_index)) {
                if (grid[neighbor] == 0) {
                    int bsquires = squire(next_player, candidate_index);
                    int squires = squire(next_player, neighbor);
                    float temp_num = 0.0f;
                    switch (squires) {
                    case 1:
                        temp_num = (bsquires == 0) ? 0.5f :
                            (bsquires == 1) ? 3.5f : 4.0f;
                        break;
                    case 2:
                        temp_num = (bsquires == 0) ? 1.0f :
                            (bsquires == 1) ? 4.0f : 4.5f;
                        break;
                    default:
                        continue; // 如果squires不是1或2，就跳过当前邻居
                    }
                    // 使用[]操作符直接访问或创建映射中的元素
                    nums_dict[neighbor] = std::max(nums_dict[neighbor], temp_num);

                }
            }
            grid[candidate_index] = next_player;
            find_specialjump(next_player, candidate_index, { candidate_index }, nums_dict);
        }
    }
    int sum = 0;
    for (auto it = nums_dict.begin(); it != nums_dict.end(); ++it) {
        sum += it->second;
    }
    return sum;
}

void Board::find_specialjump(Player next_player, int point, std::vector<int> road, std::unordered_map<int, float>& roads) {
    if (!is_valid_jumpmove(next_player, point)) {
        return;
    }
    for (const auto& neighbor : neighbors3(point)) {
        if (grid[neighbor.second] == other(next_player)) {
            const auto& neighbor_map = neighbors3(neighbor.second);
            auto it = neighbor_map.find(neighbor.first);
            if (it != neighbor_map.end()) {
                int s = it->second;
                if (grid[s] == 0) {
                    int squires = squire(next_player, s);
                    float temp_num = (squires == 1) ? 0.5f : (squires == 2) ? 1.0f : 0.0f;
                    road.emplace_back(s);
                    grid[s] = next_player;
                    roads[s] = std::max(roads[s], temp_num);
                    find_specialjump(next_player, s, road, roads);
                    grid[s] = 0;
                    road.pop_back();

                }
            }
        }
    }

}

float Board::special_moves1(Player next_player) {
    std::unordered_map<int, float> nums_dict;
    for (int candidate_index = 0; candidate_index < num_rows * num_cols; candidate_index++) {
        if (grid[candidate_index] == next_player) {
            grid[candidate_index] = 0;
            for (const int& neighbor : neighbors2(candidate_index)) {
                if (grid[neighbor] == 0) {
                    bool bl;
                    int bsquires = squire(next_player, candidate_index);
                    int squires = squire(next_player, neighbor);
                    float temp_num = 0.0f;
                    switch (squires) {
                    case 1:
                        bl = singal_move(next_player, neighbor);
                        /*Print_move({ candidate_index,neighbor }, next_player);*/

                        if (bl) {
                            temp_num = (bsquires == 0) ? 2.0f :
                                (bsquires == 1) ? 4.5f : 4.5f;
                        }
                        else
                        {
                            temp_num = (bsquires == 0) ? 2.5f :
                                (bsquires == 1) ? 9.5f : 9.5f;
                        }
                        /*std::cout << temp_num << std::endl;*/
                        break;
                    case 2:
                        bl = singal_move(next_player, neighbor);
                        /*Print_move({ candidate_index,neighbor }, next_player);*/

                        if (bl) {
                            temp_num = (bsquires == 0) ? 3.0f :
                                (bsquires == 1) ? 4.5f : 4.5f;
                        }
                        else
                        {
                            temp_num = (bsquires == 0) ? 4.0f :
                                (bsquires == 1) ? 9.5f : 9.5f;
                        }
                        /*std::cout << temp_num << std::endl;*/
                        break;
                    default:
                        continue; // 如果squires不是1或2，就跳过当前邻居
                    }
                    // 使用[]操作符直接访问或创建映射中的元素
                    nums_dict[neighbor] = std::max(nums_dict[neighbor], temp_num);

                }
            }
            grid[candidate_index] = next_player;
            find_specialjump(next_player, candidate_index, { candidate_index }, nums_dict);
        }
    }
    float sum = 0;
    for (auto it = nums_dict.begin(); it != nums_dict.end(); ++it) {
        sum += it->second;
    }
    return sum;
}

void Board::find_specialjump1(Player next_player, int point, std::vector<int> road, std::unordered_map<int, float>& roads) {
    if (!is_valid_jumpmove(next_player, point)) {
        return;
    }
    for (const auto& neighbor : neighbors3(point)) {
        if (grid[neighbor.second] == other(next_player)) {
            const auto& neighbor_map = neighbors3(neighbor.second);
            auto it = neighbor_map.find(neighbor.first);
            if (it != neighbor_map.end()) {
                int s = it->second;
                if (grid[s] == 0) {
                    int squires = squire(next_player, s);
                    float temp_num = (squires == 1) ? 2.0f : (squires == 2) ? 4.5f : 0.0f;
                    road.emplace_back(s);
                    grid[s] = next_player;
                    roads[s] = std::max(roads[s], temp_num);
                    find_specialjump(next_player, s, road, roads);
                    grid[s] = 0;
                    road.pop_back();

                }
            }
        }
    }

}

std::vector<int> Board::special_moves2(Player next_player) {
    std::set<int> unique_moves; // 使用 set 来确保唯一性
    for (int candidate_index = 0; candidate_index < num_rows * num_cols; candidate_index++) {
        if (grid[candidate_index] == next_player) {
            bool added = false;  // 标志位，用于跟踪是否已添加当前索引

            for (const int& neighbor : neighbors2(candidate_index)) {
                if (grid[neighbor] == 0) {
                    grid[candidate_index] = 0;
                    int squires = squire(next_player, neighbor);
                    if (squires > 0) {
                        unique_moves.insert(candidate_index);
                        added = true; // 标记为已添加
                        grid[candidate_index] = next_player;
                        break; // 添加后不需要继续检查其他邻居
                    }
                    grid[candidate_index] = next_player;
                }
            }

            if (!added) {  // 如果还没有添加，检查特殊跳跃是否有效
                bool value = find_specialjump2(next_player, candidate_index);
                if (value) {
                    unique_moves.insert(candidate_index);
                }
            }
        }
    }
    return std::vector<int>(unique_moves.begin(), unique_moves.end()); // 将 set 转换回 vector
}

bool Board::find_specialjump2(Player next_player, int point) {
    if (!is_valid_jumpmove(next_player, point)) {
        return false;
    }

    for (const auto& neighbor : neighbors3(point)) {
        if (grid[neighbor.second] == other(next_player)) {
            const auto& neighbor_map = neighbors3(neighbor.second);
            auto it = neighbor_map.find(neighbor.first);
            if (it != neighbor_map.end()) {
                int s = it->second;
                if (grid[s] == 0) {
                    int squires = squire(next_player, s);
                    if (squires > 0) {
                        return true;
                    }
                    else {
                        grid[s] = next_player;
                        if (find_singal_jump(next_player, s)) {
                            grid[s] = 0;
                            return true;
                        }
                        grid[s] = 0;
                    }
                }
            }
        }
    }
    return false;
}

float Board::special_moves3(Player next_player) {
    std::unordered_map<int, float> nums_dict;
    for (int candidate_index = 0; candidate_index < num_rows * num_cols; candidate_index++) {
        if (grid[candidate_index] == next_player) {
            grid[candidate_index] = 0;
            for (const int& neighbor : neighbors2(candidate_index)) {
                if (grid[neighbor] == 0) {
                    bool bl;
                    int bsquires = squire(next_player, candidate_index);
                    int squires = squire(next_player, neighbor);
                    float temp_num = 0.0f;
                    switch (squires) {
                    case 1:
                        bl = singal_move(next_player, neighbor);
                        /*Print_move({ candidate_index,neighbor }, next_player);*/

                        if (!bl) {
                            temp_num = (bsquires == 0) ? 2.5f :
                                (bsquires == 1) ? 9.5f : 9.5f;
                        }

                        break;
                    case 2:
                        bl = singal_move(next_player, neighbor);
                        /*Print_move({ candidate_index,neighbor }, next_player);*/

                        if (!bl) {
                            temp_num = (bsquires == 0) ? 4.0f :
                                (bsquires == 1) ? 9.5f : 9.5f;
                        }
                        /*std::cout << temp_num << std::endl;*/
                        break;
                    default:
                        continue; // 如果squires不是1或2，就跳过当前邻居
                    }
                    // 使用[]操作符直接访问或创建映射中的元素
                    nums_dict[neighbor] = std::max(nums_dict[neighbor], temp_num);

                }
            }
            grid[candidate_index] = next_player;
            find_specialjump(next_player, candidate_index, { candidate_index }, nums_dict);
        }
    }
    float sum = 0;
    for (auto it = nums_dict.begin(); it != nums_dict.end(); ++it) {
        sum += it->second;
    }
    return sum;
}

void Board::find_specialjump3(Player next_player, int point, std::vector<int> road, std::unordered_map<int, float>& roads) {
    if (!is_valid_jumpmove(next_player, point)) {
        return;
    }
    for (const auto& neighbor : neighbors3(point)) {
        if (grid[neighbor.second] == other(next_player)) {
            const auto& neighbor_map = neighbors3(neighbor.second);
            auto it = neighbor_map.find(neighbor.first);
            if (it != neighbor_map.end()) {
                int s = it->second;
                if (grid[s] == 0) {
                    int squires = squire(next_player, s);
                    float temp_num = (squires == 1) ? 2.0f : (squires == 2) ? 4.5f : 0.0f;
                    road.emplace_back(s);
                    grid[s] = next_player;
                    roads[s] = std::max(roads[s], temp_num);
                    find_specialjump3(next_player, s, road, roads);
                    grid[s] = 0;
                    road.pop_back();

                }
            }
        }
    }

}

bool Board::singal_move(Player next_player, int point)
{
    for (const int& neighbor : neighbors2(point)) {
        if (grid[neighbor] == other(next_player)) {
            return true;
        }
    }

    bool bl = find_singal_jump(next_player, point);
    if (bl) return true;
    return false;
}

bool Board::find_singal_jump(Player next_player, int point)
{
    if (!is_valid_singal(next_player, point)) {
        return false;
    }
    for (const auto& neighbor : neighbors3(point)) {
        if (grid[neighbor.second] == next_player) {
            const auto& neighbor_map = neighbors3(neighbor.second);
            auto it = neighbor_map.find(neighbor.first);
            if (it != neighbor_map.end()) {
                int s = it->second;
                if (grid[s] == other(next_player)) {
                    return true;
                }
                else if (grid[s] == 0) {
                    grid[point] = next_player;
                    if (find_singal_jump(next_player, s)) {
                        grid[point] = 0;
                        return true;
                    }
                    grid[point] = 0;
                }
            }
        }
    }
    return false;

}

bool Board::is_valid_singal(Player next_player, int point) {
    /*std::cout << "www" << std::endl;*/
    if (grid[point] == other(next_player) ||
        grid[point] == next_player) {
        return false;
    }
    for (const auto& neighbor : neighbors3(point)) {
        if (grid[neighbor.second] == next_player) {
            const auto& neighbor_map = neighbors3(neighbor.second);
            auto it = neighbor_map.find(neighbor.first);
            if (it != neighbor_map.end()) {
                int s = it->second;
                if (grid[s] == other(next_player) || grid[s] == 0) {
                    return true;
                }
            }
        }
    }
    return false;
}

std::vector<std::vector<int>> Board::singal_moves(Player next_player, int point) {
    /*std::vector<Point> legal_moves;*/
    std::vector<std::vector<int>> singal_moves;
    singal_moves.reserve(15);
    std::vector<int> legal_move;

    if (grid[point] == next_player) {
        for (const int& neighbor : neighbors2(point)) {
            if (grid[neighbor] == 0) {
                legal_move.clear();
                legal_move.emplace_back(point);
                legal_move.emplace_back(neighbor);
                singal_moves.emplace_back(legal_move);
            }
        }
        go_find_jumpmoves(next_player, point, { point }, singal_moves);
    }

    /* for (const auto& moves : legal_moves) {
            //for (const auto& move : moves) {
                std::cout << move << " ";
            }
            std::cout << std::endl;
        }*/
    return singal_moves;

}
std::vector<std::vector<int>> Board::cut_legal_moves(Player next_player) {
    std::vector<std::vector<int>> s_moves;
    std::vector<std::vector<int>> s_moves1;
    std::vector<std::vector<int>> s_moves2;
    std::vector<std::vector<int>> moves;
    std::vector<std::vector<int>> jump_moves;
    std::vector<int> legal_moves;
    for (int candidate_index = 0; candidate_index < num_rows * num_cols; candidate_index++) {
        if (grid[candidate_index] == next_player) {
            grid[candidate_index] = 0;
            for (const int& neighbor : neighbors2(candidate_index)) {
                if (grid[neighbor] == 0) {
                    legal_moves.clear();
                    legal_moves.emplace_back(candidate_index);
                    legal_moves.emplace_back(neighbor);
                    int bsquires = squire(next_player, candidate_index);
                    if (bsquires > 0) {
                        s_moves2.emplace_back(legal_moves);
                        int squires = squire(next_player, neighbor);
                        if (squires > 0) {
                            s_moves1.emplace_back(legal_moves);
                        }
                        continue;
                    }
                    int squires = squire(next_player, neighbor);
                    if (squires > 0) {
                        s_moves.emplace_back(legal_moves);
                        continue;
                    }
                    moves.emplace_back(legal_moves);

                }
            }
            grid[candidate_index] = next_player;
            go_find_jumpmoves(next_player, candidate_index, { candidate_index }, jump_moves);
        }
    }
    if (!s_moves1.empty())
    {
        /*s_moves1.insert(s_moves1.end(), jump_moves.begin(), jump_moves.end());*/
        return s_moves1;
    }
    else if (!s_moves.empty())
    {
        /*s_moves.insert(s_moves.end(), jump_moves.begin(), jump_moves.end());*/
        return s_moves;
    }
    else if (!s_moves2.empty())
    {
        s_moves2.insert(s_moves2.end(), jump_moves.begin(), jump_moves.end());
        return s_moves2;
    }
    else if (!jump_moves.empty())
    {
        return jump_moves;
    }
    else if (!moves.empty())
    {
        moves.insert(moves.end(), jump_moves.begin(), jump_moves.end());
        return moves;
    }

    else
    {
        return std::vector<std::vector<int>>();
    }
}

std::vector<std::vector<int>> Board::fly_legal_moves(Player next_player)
{
    std::vector<std::vector<int>> legal_moves;
    legal_moves.reserve(1500);
    std::vector<int> legal_move;
    std::vector<std::vector<int>> jump_moves;
    int stones = sum_stone(next_player);
    if (stones > 14) {
        for (int candidate_index = 0; candidate_index < num_rows * num_cols; candidate_index++) {
            if (grid[candidate_index] == next_player) {
                for (const int& neighbor : neighbors2(candidate_index)) {
                    if (grid[neighbor] == 0) {
                        legal_move.clear();
                        legal_move.emplace_back(candidate_index);
                        legal_move.emplace_back(neighbor);
                        legal_moves.emplace_back(legal_move);
                    }
                }
                go_find_jumpmoves(next_player, candidate_index, { candidate_index }, jump_moves);
            }
        }
    }
    else
    {
        for (int candidate_index = 0; candidate_index < num_rows * num_cols; candidate_index++) {
            if (grid[candidate_index] == next_player) {
                for (int candidate_index1 = 0; candidate_index1 < num_rows * num_cols; candidate_index1++) {
                    if (grid[candidate_index1] == 0) {
                        legal_move.clear();
                        legal_move.emplace_back(candidate_index);
                        legal_move.emplace_back(candidate_index1);
                        legal_moves.emplace_back(legal_move);
                    }
                }
                go_find_jumpmoves(next_player, candidate_index, { candidate_index }, jump_moves);
            }
        }
    }
    for (const auto& sub_vector : jump_moves) {
        if (sub_vector.size() >= 5) {
            legal_moves.emplace_back(sub_vector);
        }
    }
    /* for (const auto& moves : legal_moves) {
         //for (const auto& move : moves) {
             std::cout << move << " ";
         }
         std::cout << std::endl;
     }*/
    return legal_moves;
    
}

float Board::fly_special_moves(Player next_player)
{
    std::unordered_map<int, float> nums_dict;
    int stones = sum_stone(next_player);
    if (stones > 14) {
        for (int candidate_index = 0; candidate_index < num_rows * num_cols; candidate_index++) {
            if (grid[candidate_index] == next_player) {
                grid[candidate_index] = 0;
                for (const int& neighbor : neighbors2(candidate_index)) {
                    if (grid[neighbor] == 0) {
                        int bsquires = squire(next_player, candidate_index);
                        int squires = squire(next_player, neighbor);
                        float temp_num = 0.0f;
                        switch (squires) {
                        case 1:
                            temp_num = (bsquires == 0) ? 0.5f :
                                (bsquires == 1) ? 3.5f : 4.0f;
                            break;
                        case 2:
                            temp_num = (bsquires == 0) ? 1.0f :
                                (bsquires == 1) ? 4.0f : 4.5f;
                            break;
                        default:
                            continue; // 如果squires不是1或2，就跳过当前邻居
                        }
                        // 使用[]操作符直接访问或创建映射中的元素
                        nums_dict[neighbor] = std::max(nums_dict[neighbor], temp_num);

                    }
                }
                grid[candidate_index] = next_player;
            }
        }
    }
    else
    {
        for (int candidate_index = 0; candidate_index < num_rows * num_cols; candidate_index++) {
            if (grid[candidate_index] == next_player) {
                grid[candidate_index] = 0;
                for (int candidate_index1 = 0; candidate_index1 < num_rows * num_cols; candidate_index1++) {
                    if (grid[candidate_index1] == 0 && candidate_index1!=candidate_index) {
                        int bsquires = squire(next_player, candidate_index);
                        int squires = squire(next_player, candidate_index1);
                        float temp_num = 0.0f;
                        switch (squires) {
                        case 1:
                            temp_num = (bsquires == 0) ? 0.5f :
                                (bsquires == 1) ? 3.5f : 4.0f;
                            break;
                        case 2:
                            temp_num = (bsquires == 0) ? 1.0f :
                                (bsquires == 1) ? 4.0f : 4.5f;
                            break;
                        case 3:
                            temp_num = (bsquires == 0) ? 1.5f :
                                (bsquires == 1) ? 4.5f : 5.0f;
                            break;
                        case 4:
                            temp_num = (bsquires == 0) ? 2.0f :
                                (bsquires == 1) ? 5.0f : 5.5f;
                            break;
                        default:
                            continue; // 如果squires不是1或2，就跳过当前邻居
                        }
                        // 使用[]操作符直接访问或创建映射中的元素
                        nums_dict[candidate_index1] = std::max(nums_dict[candidate_index1], temp_num);

                    }
                }
                grid[candidate_index] = next_player;
            }
        }
    }
    float sum = 0;
    for (auto it = nums_dict.begin(); it != nums_dict.end(); ++it) {
        sum += it->second;
    }
    return sum;
}

std::vector<std::vector<int>> Board::fly_legal_special(Player next_player)
{
    std::vector<std::vector<int>> legal_moves;
    legal_moves.reserve(1500);
    std::vector<int> legal_move;
    int stones = sum_stone(next_player);
    if (stones > 14) {
        for (int candidate_index = 0; candidate_index < num_rows * num_cols; candidate_index++) {
            if (grid[candidate_index] == next_player) {
                grid[candidate_index] = 0;
                for (const int& neighbor : neighbors2(candidate_index)) {
                    if (grid[neighbor] == 0) {
                        int squires = squire(next_player, neighbor);
                        if (squires > 0) {
                            legal_move.clear();
                            legal_move.emplace_back(candidate_index);
                            legal_move.emplace_back(neighbor);
                            legal_moves.emplace_back(legal_move);
                        }
                    }
                }
                grid[candidate_index] = next_player;
            }
        }
    }
    else
    {
        for (int candidate_index = 0; candidate_index < num_rows * num_cols; candidate_index++) {
            if (grid[candidate_index] == next_player) {
                grid[candidate_index] = 0;
                for (int candidate_index1 = 0; candidate_index1 < num_rows * num_cols; candidate_index1++) {
                    if (grid[candidate_index1] == 0 && candidate_index1!=candidate_index) {
                        int squires = squire(next_player, candidate_index1);
                        if (squires > 0) {
                            legal_move.clear();
                            legal_move.emplace_back(candidate_index);
                            legal_move.emplace_back(candidate_index1);
                            legal_moves.emplace_back(legal_move);
                        }                        
                    }
                }
                grid[candidate_index] = next_player;
            }
        }
    }
    return legal_moves;

}



GameState::GameState(Board board, Player next_player, GameState* previous, int state, int step)
    : board(board), next_player(next_player), previous_state(previous), state(state), step(step) {}


GameState GameState::go_apply_move(std::vector<int> pos) {
    Board next_board = board;
    if (state == 1) {
        next_board.place_stone(next_player, pos);
        step++;
    }
    else if (state == 2) {
        next_board = board;
        next_board.go_take_stone(next_player, pos);
        int nums = next_board.squire(next_player, pos.back());
        if (nums > 0) {
            std::vector<int> eat_stones = next_board.eat1(next_player, nums);
            for (const int& eat_stone : eat_stones) {

                next_board.grid[eat_stone] = 0;
            }
            //std::cout << "吃掉" << std::endl;
            //board.Print_move(eat_stones);
            //std::cout << "ddd" << std::endl;
        }
        step++;
    }
    //else {
    //    next_board.take_stone3(next_player, pos);
    //}
    return GameState(next_board, other(next_player), this, state, step);
}

GameState GameState::go_apply_move_mcts(std::vector<int> pos) {
    Board next_board = board;
    int new_step = 0;
    if (state == 1) {
        next_board.place_stone(next_player, pos);
        new_step = step + 1;
    }
    else if (state == 2) {
        next_board = board;
        next_board.go_take_stone(next_player, pos);
        int nums = next_board.squire(next_player, pos.back());
        if (nums > 0) {
            std::vector<int> eat_stones = next_board.eat1(next_player, nums);
            for (const int& eat_stone : eat_stones) {

                next_board.grid[eat_stone] = 0;
            }
            //std::cout << "吃掉" << std::endl;
            //board.Print_move(eat_stones);
            //std::cout << "ddd" << std::endl;
        }
        new_step = step + 1;
    }
    //else {
    //    next_board.take_stone3(next_player, pos);
    //}
    return GameState(next_board, other(next_player), this, state, new_step);
}

GameState GameState::apply_move(std::tuple<std::vector<int>, std::vector<int>>& pos) {
    Board next_board = board;
    auto [move, eat] = pos;
    if (state == 1) {
        next_board.place_stone(next_player, move);
        step++;
    }
    else if (state == 2) {
        next_board = board;
        next_board.go_take_stone(next_player, move);
        //int nums = next_board.squire(next_player, pos.back());
        //if (nums > 0) {
        //    std::vector<int> eat_stones = next_board.eat1(next_player, nums);
        //    for (const int& eat_stone : eat_stones) {

        //        next_board.grid[eat_stone] = 0;
        //    }
        if (!eat.empty()) {
            for (const int& eat_stone : eat) {
                next_board.grid[eat_stone] = 0;
            }
        }
        //std::cout << "吃掉" << std::endl;
        //board.Print_move(eat_stones);
        //std::cout << "ddd" << std::endl;
        step++;
    }
    //else {
    //    next_board.take_stone3(next_player, pos);
    //}
    return GameState(next_board, other(next_player), this, state, step);
}

GameState GameState::apply_move_mcts(std::tuple<std::vector<int>, std::vector<int>>& pos) {
    Board next_board = board;
    auto [move, eat] = pos;
    int new_step = 0;
    if (state == 1) {
        next_board.place_stone(next_player, move);
        new_step = step + 1;
    }
    else if (state == 2) {
        next_board = board;
        next_board.go_take_stone(next_player, move);
        //int nums = next_board.squire(next_player, pos.back());
        //if (nums > 0) {
        //    std::vector<int> eat_stones = next_board.eat1(next_player, nums);
        //    for (const int& eat_stone : eat_stones) {

        //        next_board.grid[eat_stone] = 0;
        //    }
        if (!eat.empty()) {
            for (const int& eat_stone : eat) {
                next_board.grid[eat_stone] = 0;
            }
        }
        //std::cout << "吃掉" << std::endl;
        //board.Print_move(eat_stones);
        //std::cout << "ddd" << std::endl;
        new_step = step + 1;
    }
    //else {
    //    next_board.take_stone3(next_player, pos);
    //}
    return GameState(next_board, other(next_player), this, state, new_step);
}


GameState GameState::all_apply_move(const std::vector<int>& pos) {
    Board next_board = board;
    if (state == 1) {
        next_board.place_stone(next_player, pos);
        step++;
    }
    else if (state == 2) {
        next_board = board;
        next_board.all_take_stone(next_player, pos);

        step++;
    }
    //else {
    //    next_board.take_stone3(next_player, pos);
    //}
    return GameState(next_board, other(next_player), this, state, step);
}

GameState GameState::new_game(int board_size) {
    Board board(board_size, board_size);
    GameState new_game(board, Player::WHITE, nullptr, 1, 0);
    return new_game;
}

void GameState::game_state(Board& next_board) {
    if (next_board.is_full_grid()) {
        std::cout << "走子" << std::endl;
        state = 2;
        next_player = Player::BLACK;
        next_board.grid[90] = 0;
        next_board.grid[105] = 0;
    }
    /*int stones = board.sum_stone(next_player);
    int stones1 = board.sum_stone(other(next_player));
    if (state == 2) {
        if (stones <= 14 || stones1 <= 14) {
            std::cout << "fei" << std::endl;
            state = 3;
        }
    }*/
}

GameState GameState::fly_apply_move(std::tuple<std::vector<int>, std::vector<int>>& pos) {
    Board next_board = board;
    auto [move, eat] = pos;
    if (state == 1) {
        next_board.place_stone(next_player, move);
        step++;
    }
    else if (state == 2) {
        next_board = board;
        next_board.go_take_stone(next_player, move);
        //int nums = next_board.squire(next_player, pos.back());
        //if (nums > 0) {
        //    std::vector<int> eat_stones = next_board.eat1(next_player, nums);
        //    for (const int& eat_stone : eat_stones) {

        //        next_board.grid[eat_stone] = 0;
        //    }
        if (!eat.empty()) {
            for (const int& eat_stone : eat) {
                next_board.grid[eat_stone] = 0;
            }
        }
        //std::cout << "吃掉" << std::endl;
        //board.Print_move(eat_stones);
        //std::cout << "ddd" << std::endl;
        step++;
    }
    else {
        next_board = board;
        next_board.go_take_stone(next_player, move);
        if (!eat.empty()) {
            for (const int& eat_stone : eat) {
                next_board.grid[eat_stone] = 0;
            }
        }
        step++;
        
    }
    return GameState(next_board, other(next_player), this, state, step);
}

//bool GameState::is_over() {
//    if (state == 2) {
//        int stones = board.sum_stone(Player::WHITE);
//        int opponent_stones = board.sum_stone(Player::BLACK);
//        float player_routes_score = board.special_moves(Player::WHITE);
//        float opponent_routes_score = board.special_moves(Player::BLACK);
//        //if (next_player == Player::BLACK) {
//        //    int routes_diff = player_routes_score - opponent_routes_score * 1.5;
//        //}
//        //else
//        //{
//        //    int routes_diff = opponent_routes_score - player_routes_score * 1.5;
//        //}
//        float routes_diff = player_routes_score - opponent_routes_score;
//        if (stones < 14 || routes_diff <= -8) {
//            winner = Player::BLACK;
//            /*std::cout << routes_diff << std::endl;*/
//            std::cout << "黑方胜" << std::endl;
//            return true;
//        }
//        if (opponent_stones < 14 || routes_diff >= 8) {
//            winner = Player::WHITE;
//            std::cout << "白方胜" << std::endl;
//            return true;
//        }
//    }
//    return false;
//}

bool GameState::is_over1() {
    if (state == 2) {
        int stones = board.sum_stone(Player::WHITE);
        int opponent_stones = board.sum_stone(Player::BLACK);
        float player_routes_score = board.special_moves1(Player::WHITE);
        float opponent_routes_score = board.special_moves1(Player::BLACK);
        float routes_diff = player_routes_score - opponent_routes_score;
        if (stones < 14 || routes_diff <= -19) {
            winner = Player::BLACK;
            std::cout << "黑方胜" << std::endl;
            return true;
        }
        if (opponent_stones < 14 || routes_diff >= 19) {
            winner = Player::WHITE;
            std::cout << "白方胜" << std::endl;
            return true;
        }
    }
    return false;
}

bool GameState::is_over() {
    if (state == 2) {
        int stones = board.sum_stone(Player::WHITE);
        int opponent_stones = board.sum_stone(Player::BLACK);
        if (stones < 14) {
            winner = Player::BLACK;
            /*std::cout << routes_diff << std::endl;*/
            std::cout << "黑方胜" << std::endl;
            return true;
        }
        if (opponent_stones < 14) {
            winner = Player::WHITE;
            std::cout << "白方胜" << std::endl;
            return true;
        }
    }
    return false;
}

Player GameState::score() {
    if (state == 2) {
        float player_routes_score = board.special_moves1(Player::WHITE);
        float opponent_routes_score = board.special_moves1(Player::BLACK);
        if (player_routes_score > opponent_routes_score) {
            return Player::WHITE;
        }
        else if (player_routes_score < opponent_routes_score) {
            return Player::BLACK;
        }
        else
        {
            int stones = board.sum_stone(Player::WHITE);
            int opponent_stones = board.sum_stone(Player::BLACK);
            if (stones > opponent_stones) {
                return Player::WHITE;
            }
            else
            {
                return Player::BLACK;
            }
        }
    }
}

std::string PointToString1(char row, int col) {
    std::stringstream ss;
    ss << "Point(" << row << ", " << col << ")";
    return ss.str();
}

void Print_move1(std::vector<int> moves, Player player) {
    std::vector<std::string> point_strings;
    for (auto& move : moves) {
        int row = move / 14 + 1;
        int col = move % 14 + 1;
        char letter = 'A' + row - 1;
        point_strings.push_back(PointToString1(letter, col));
    }

    std::cout << player << "[";
    for (size_t i = 0; i < point_strings.size(); ++i) {
        std::cout << point_strings[i];
        if (i != point_strings.size() - 1) {
            std::cout << ",";
        }
    }
}







////#include<vector>
//#include<iostream>
//#include<assert.h>
//#include<algorithm>
//#include<numeric>
//#include "gotypes.h"
//#include "goboard.h"
//
//Board::Board(int num_rows, int num_cols) : num_rows(num_rows), num_cols(num_cols) {
//    grid = std::vector<int>(num_rows * num_cols, 0);
//}
//
//void Board::displayGrid()
//{
//    for (int i = 0; i < num_rows; i++) {
//        for (int j = 0; j < num_cols; j++) {
//            std::cout << grid[i * num_cols + j] << "  ";
//        }
//        std::cout << static_cast<char>('A' + i) << std::endl;
//    }
//
//    // 打印行标识数字
//    for (int i = 0; i < num_cols; i++) {
//        if (i < 9) {
//            std::cout << i + 1 << "  ";
//        }
//        else
//        {
//            std::cout << i + 1 << " ";
//        }
//        
//    }
//    std::cout << std::endl;
//}
//
//int& Board::at(int row, int col) {
//    return grid[row * num_cols + col];
//}
//
//const int& Board::at(int row, int col) const {
//    return grid[row * num_cols + col];
//}
//
//
//
//std::vector<std::vector<Point>> Board::legal_moves(Player next_player, int game_state) {
//    /*std::vector<Point> legal_moves;*/
//    if (game_state == 1) {
//        if (grid[90] == 0 || grid[105] == 0) {
//            std::vector<std::vector<Point>> legal_moves;
//            std::vector<Point> legal_move;
//            legal_move.reserve(1);
//            if (grid[90] == 0) {
//                legal_move.emplace_back(Point(91 / num_cols + 1, 91 % num_cols));
//                /*std::cout << "90 "<< std::endl;*/
//            }
//            else {
//                legal_move.emplace_back(Point(105 / num_cols + 1, 105 % num_cols + 1));
//                /*std::cout << "104 " << std::endl;*/
//            }
//            legal_moves.emplace_back(legal_move);
//            return legal_moves;
//        }
//        else
//        {
//            std::vector<std::vector<Point>> legal_moves;
//            std::vector<Point> legal_move;
//            legal_move.reserve(num_rows * num_cols + 1);
//            for (int r = 1; r < num_rows + 1; r++) {
//                for (int c = 1; c < num_cols + 1; c++) {
//                    int candidate_index = (r - 1) * num_cols + (c - 1);
//                    if (grid[candidate_index] == 0) {
//                        legal_move.emplace_back(Point(r, c));
//                    }
//                    /*std::cout << candidate_index << std::endl;*/
//                }
//            }
//            legal_moves.emplace_back(legal_move);
//            return legal_moves;
//        }
//    }
//    else if (game_state == 2) {
//        std::vector<std::vector<Point>> legal_moves;
//        legal_moves.reserve(1500);
//        for (int r = 1; r <= num_rows; r++) {
//            for (int c = 1; c <= num_cols; c++) {
//                int candidate_index = (r - 1) * num_cols + (c - 1);
//                Point p(r, c);
//                if (grid[candidate_index] == next_player) {
//                    for (const Point& neighbor : neighbors(p)) {
//                        std::vector<Point> legal_move;
//                        if (1 <= neighbor.row && neighbor.row <= num_rows &&
//                            1 <= neighbor.col && neighbor.col <= num_cols &&
//                            grid[(neighbor.row - 1) * num_cols + (neighbor.col - 1)] == 0) {
//                            legal_move.emplace_back(Point(r, c));
//                            legal_move.emplace_back(neighbor);
//                            legal_moves.emplace_back(legal_move);
//                        }
//                    }
//                    go_find_jumpmoves(next_player, p, { p }, legal_moves);
//                }
//            }
//        }
//        return legal_moves;
//    }
//}
//
//void Board::go_find_jumpmoves(Player next_player, Point point, std::vector<Point> road, std::vector<std::vector<Point>>& roads) {
//    if (!is_valid_jumpmove(next_player, point)) {
//        return;
//    }
//    for (const auto& neighbor : neighbors1(point)) {
//        if (1 <= neighbor.second.row && neighbor.second.row <= num_rows &&
//            1 <= neighbor.second.col && neighbor.second.col <= num_cols &&
//            grid[(neighbor.second.row - 1) * 14 + neighbor.second.col - 1] == other(next_player)) {
//            Point s = neighbors1(neighbor.second)[neighbor.first];
//            if (1 <= s.row && s.row <= num_rows &&
//                1 <= s.col && s.col <= num_cols &&
//                grid[(s.row - 1) * num_rows + num_cols - 1] == 0) {
//                road.emplace_back(neighbor.second);
//                road.emplace_back(s);
//                roads.emplace_back(road);
//                /*std::cout <<"asd"<< s << std::endl;*/
//                grid[(s.row - 1) * num_rows + num_cols - 1] = next_player;
//                go_find_jumpmoves(next_player, s, road, roads);
//                grid[(s.row - 1) * num_rows + num_cols - 1] = 0;
//                road.pop_back();
//                road.pop_back();
//            }
//        }
//    }
//}
//
//bool Board::is_valid_jumpmove(Player next_player, Point point) {
//    /*std::cout << "www" << std::endl;*/
//    if (grid[(point.row - 1) * num_cols + (point.col - 1)] == other(next_player) ||
//        grid[(point.row - 1) * num_cols + (point.col - 1)] == 0) {
//        return false;
//    }
//    for (const auto& neighbor : neighbors1(point)) {
//        if (1 <= neighbor.second.row && neighbor.second.row <= num_rows &&
//            1 <= neighbor.second.col && neighbor.second.col <= num_cols &&
//            grid[(neighbor.second.row - 1) * 14 + neighbor.second.col - 1] == other(next_player)) {
//            Point s = neighbors1(neighbor.second)[neighbor.first];
//            if (1 <= s.row && s.row <= num_rows &&
//                1 <= s.col && s.col <= num_cols &&
//                grid[(s.row - 1) * num_rows + num_cols - 1] == 0) {
//                return true;
//            }
//        }
//    }
//    return false;
//}
//
//void Board::place_stone(Player next_player, std::vector<Point> pos) {
//    grid[(pos[0].row - 1) * num_cols + (pos[0].col - 1)] = next_player;
//}
//
//void Board::go_take_stone(Player next_player, std::vector<Point> pos) {
//    if (grid[(pos[0].row - 1) * num_cols + (pos[0].col - 1)] != next_player) {
//        std::cerr << "Assertion failed: grid[pos[0].row - 1][pos[0].col - 1] != next_player" << std::endl;
//    }
//
//    for (const Point& po : pos) {
//        grid[(po.row - 1) * num_cols + (po.col - 1)] = 0;
//
//    }
//    grid[(pos.back().row - 1) * num_cols + (pos.back().col - 1)] = next_player;
//    /*int t = 0;
//    std::vector<Point> point_neighbors = neighbors(pos[0]);
//    for (const Point& neighbor : point_neighbors) {
//        if (pos[1] == neighbor) {
//            t = 1;
//            break;
//        }
//    }
//
//    if (t == 1) {
//        grid[pos[0].row - 1][pos[0].col - 1] = 0;
//        grid[pos.back().row - 1][pos.back().col - 1] = next_player;
//    }
//    else {
//        std::vector<std::vector<Point>> jump_moves;
//        std::vector<Point> move;
//        go_find_jumpmoves1(next_player, pos[0], { pos[0] }, jump_moves);
//
//        for (const std::vector<Point>& jump_move : jump_moves) {
//            if (jump_move.back() == pos.back()) {
//                move = jump_move;
//                break;
//            }
//        }
//
//        for (const Point& mo : move) {
//            grid[mo.row - 1][mo.col - 1] = 0;
//        }
//
//        grid[pos.back().row - 1][pos.back().col - 1] = next_player;*/
//}
//
//int Board::squire(const Player next_player, Point point) {
//    /*std::cout << point << std::endl;*/
//    int s = 0;
//    if (point.col - 2 >= 0 && point.row + 1 <= num_rows) {
//        if (grid[(point.row - 1) * num_cols + (point.col - 2)] == next_player &&
//            grid[point.row * num_cols + (point.col - 2)] == next_player &&
//            grid[point.row * num_cols + (point.col - 1)] == next_player) {
//            s += 1;
//            /*std::cout << 1 << std::endl;*/
//        }
//
//    }
//    if (point.col + 1 <= num_cols && point.row + 1 <= num_rows) {
//        if (grid[(point.row - 1) * num_cols + point.col] == next_player &&
//            grid[point.row * num_cols + point.col] == next_player &&
//            grid[point.row * num_cols + (point.col - 1)] == next_player) {
//            s += 1;
//            /*std::cout << 2 << std::endl;*/
//        }
//    }
//    if (point.row - 2 >= 0 && point.col - 2 >= 0) {
//        if (grid[(point.row - 1) * num_cols + (point.col - 2)] == next_player &&
//            grid[(point.row - 2) * num_cols + (point.col - 2)] == next_player &&
//            grid[(point.row - 2) * num_cols + (point.col - 1)] == next_player) {
//            s += 1;
//            /*std::cout << 3 << std::endl;*/
//        }
//    }
//    if (point.row - 2 >= 0 && point.col + 1 <= num_cols) {
//        if (grid[(point.row - 2) * num_cols + (point.col - 1)] == next_player &&
//            grid[(point.row - 2) * num_cols + point.col] == next_player &&
//            grid[(point.row - 1) * num_cols + point.col] == next_player) {
//            s += 1;
//            /*std::cout << point << std::endl;
//            std::cout << (point.row - 2) * num_cols + (point.col - 1) << (point.row - 2) * num_cols + point.col << (point.row - 1) * num_cols + point.col <<std::endl;
//            std::cout << grid[(point.row - 2) * num_cols + (point.col - 1)] << grid[(point.row - 2) * num_cols + point.col] << grid[(point.row - 1) * num_cols + point.col] << std::endl;
//            std::cout << 4 << std::endl;*/
//        }
//    }
//    return s;
//}
//
//std::vector<Point> Board::eat(Player next_player, int nums) {
//    std::vector<Point> eat_moves;
//    bool found = false;
//    for (int r = 1; r < num_rows + 1; r++) {
//        for (int c = 1; c < num_cols + 1; c++) {
//            int candidate_index = (r - 1) * num_cols + (c - 1);
//            Point p(r, c);
//            if (grid[candidate_index] == other(next_player)) {
//                eat_moves.emplace_back(p);
//                found = true;
//                break;
//            }
//        }
//        if (found) {
//            break;  // 终止外部循环
//        }
//
//    }
//    return eat_moves;
//}
//
////void legal_moves(Player next_player, int game_state) {
////    /*std::vector<Point> legal_moves;*/
////    //for (int i = 0; i < grid.size(); i++) {
////    //    std::cout << "grid[" << i << "]: " << grid[i] << std::endl;
////    //}
////    if (game_state == 1) {
////        std::vector<Point> legal_moves;
////        legal_moves.reserve(num_rows * num_cols + 1);
////        for (int r = 1; r < num_rows + 1; r++) {
////            for (int c = 1; c < num_cols + 1; c++) {
////                int candidate_index = (r - 1) * num_cols + (c - 1);
////                /*std::cout << grid[candidate_index]<< std::endl;*/
////                if (grid[candidate_index] == 0) {
////                    std::cout << "grid"<<candidate_index << std::endl;
////                    legal_moves.emplace_back(Point(r, c));
////                }
////                /*std::cout << candidate_index << std::endl;*/
////            }
////        }
////  
////    }
////}
//
//bool Board::is_full_grid() {
//    for (int i = 0; i < grid.size(); i++) {
//        if (grid[i] == 0) {
//            return false;
//        }
//    }
//    return true;
//}
//
//bool Board::is_valid_move_layout(Point move) {
//    if (grid[(move.row - 1) * num_rows + move.col - 1] == 0) {
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}
//
//GameState::GameState(Board board, Player next_player, GameState* previous, int state)
//    : board(board), next_player(next_player), previous_state(previous), state(state) {}
//
//GameState GameState::go_apply_move(std::vector<Point> pos) {
//    Board next_board = board;
//    if (state == 1) {
//        next_board.place_stone(next_player, pos);
//    }
//    else if (state == 2) {
//        next_board = board;
//        next_board.go_take_stone(next_player, pos);
//        int nums = next_board.squire(next_player, pos.back());
//        if (nums > 0) {
//            std::vector<Point> eat_stones = next_board.eat(next_player, nums);
//            for (const Point& eat_stone : eat_stones) {
//                std::cout << "吃掉" << eat_stone << std::endl;
//                next_board.grid[(eat_stone.row - 1) * board.num_rows + eat_stone.col - 1] = 0;
//            }
//        }
//    }
//    //else {
//    //    next_board.take_stone3(next_player, pos);
//    //}
//    return GameState(next_board, other(next_player), this, state);
//}
//
//GameState GameState::new_game(int board_size) {
//    Board board(board_size, board_size);
//    GameState new_game(board, Player::WHITE, nullptr, 1);
//    return new_game;
//}
//
//void GameState::game_state(Board& next_board) {
//    if (next_board.is_full_grid()) {
//        state = 2;
//        next_player = Player::BLACK;
//        next_board.grid[90] = 0;
//        next_board.grid[105] = 0;
//    }
//}
//
//
//
//

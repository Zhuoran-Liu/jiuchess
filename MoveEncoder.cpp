#include "MoveEncoder.h"


MoveEncoder::MoveEncoder(std::pair<int, int> board_size, int num_planes)
    : board_width(board_size.first), board_height(board_size.second), num_planes(num_planes) {}

std::string MoveEncoder::name() const {
    return "moveplane";
}

//std::vector<std::vector<std::vector<int>>> MoveEncoder::encode(const GameState& game_state) {
//    std::vector<std::vector<std::vector<int>>> board_matrix(
//        num_planes, std::vector<std::vector<int>>(board_height, std::vector<int>(board_width, 0)));
//
//    auto next_player = game_state.next_player;
//    for (int r = 0; r < board_height; ++r) {
//        for (int c = 0; c < board_width; ++c) {
//            /*Point p(r + 1, c + 1);*/
//            int go_string = game_state.board.grid[r * board_height + c];
//            if (go_string == 0) {
//                continue;
//            }
//            else if (go_string == next_player) {
//                board_matrix[0][r][c] = 1;
//            }
//            else {
//                board_matrix[0][r][c] = -1;
//            }
//        }
//    }
//
//    if (next_player == 1) {
//        std::fill(board_matrix[1].begin(), board_matrix[1].end(), std::vector<int>(board_width, 1));
//    }
//    else {
//        std::fill(board_matrix[1].begin(), board_matrix[1].end(), std::vector<int>(board_width, 0));
//    }
//
//    return board_matrix;
//}

torch::Tensor MoveEncoder::encode(GameState& game_state) {
    /*std::cout << num_planes << game_state.board.num_rows << std::endl;*/
    torch::Tensor tensor = torch::zeros({ num_planes, game_state.board.num_rows, game_state.board.num_cols }, torch::kFloat32);
    auto tensor_accessor = tensor.accessor<float, 3>(); // 创建访问器来修改Tensor

    //std::vector<int> our_move = game_state.board.special_moves2(Player::BLACK);
    //std::vector<int> opponent_move = game_state.board.special_moves2(Player::WHITE);
    auto next_player = game_state.next_player;
    if (next_player == 1) {
        torch::Tensor second_plane = tensor.select(0, 1);
        second_plane.fill_(1);
    }
    else
    {
        torch::Tensor second_plane = tensor.select(0, 2);
        second_plane.fill_(1);
    }
    for (int y = 0; y < board_height; ++y) {
        for (int x = 0; x < board_width; ++x) {
            int go_string = game_state.board.grid[y * board_width + x];
            if (go_string == 0) {
                continue;
            }
            else if (go_string == 1) {
                tensor_accessor[0][y][x] = 1;
            }
            else {
                tensor_accessor[0][y][x] = -1;
            }
        }
    }
    //for (int index : our_move) {
    //    int y = index / game_state.board.num_cols;  // 计算行
    //    int x = index % game_state.board.num_cols;  // 计算列
    //    tensor_accessor[3][y][x] = 1;  // 标记我们的移动
    //}

    //// 填充第五平面：opponent_move
    //for (int index : opponent_move) {
    //    int y = index / game_state.board.num_cols;  // 计算行
    //    int x = index % game_state.board.num_cols;  // 计算列
    //    tensor_accessor[4][y][x] = -1;  // 标记对手的移动
    //}
    return tensor.unsqueeze(0);
}

torch::Tensor MoveEncoder::eat_encode(const GameState& game_state, std::vector<int> moves) {
    Board next_board = game_state.board;
    next_board.go_take_stone(game_state.next_player, moves);
    int nums = next_board.squire(game_state.next_player, moves.back());

    torch::Tensor tensor = torch::zeros({ num_planes, game_state.board.num_rows, game_state.board.num_cols }, torch::kFloat32);
    auto tensor_accessor = tensor.accessor<float, 3>(); // 创建访问器来修改Tensor

    auto next_player = game_state.next_player;

    auto other_player = other(next_player);
    if (next_player == 1) {
        torch::Tensor second_plane = tensor.select(0, 1);
        second_plane.fill_(1);
    }
    else
    {
        torch::Tensor second_plane = tensor.select(0, 2);
        second_plane.fill_(1);
    }
    for (int y = 0; y < board_height; ++y) {
        for (int x = 0; x < board_width; ++x) {
            int go_string = next_board.grid[y * board_width + x];
            if (go_string == 0) {
                continue;
            }
            else if (go_string == 1) {
                tensor_accessor[0][y][x] = 1;
                if (go_string != next_player) {
                    tensor_accessor[3][y][x] = other_player;
                }
            }
            else {
                tensor_accessor[0][y][x] = -1;
                if (go_string != next_player) {
                    tensor_accessor[3][y][x] = -1;
                }
            }
        }
    }

    torch::Tensor fifth_plane = tensor.select(0, 4);
    fifth_plane.fill_(static_cast<float>(nums));

    return tensor.unsqueeze(0);
}

int MoveEncoder::encode_move(const std::vector<int>& moves) const {
    int start = moves[0];
    int end = moves.back();
    int start_row = start / 14;
    int start_col = start % 14;
    int end_row = end / 14;
    int end_col = end % 14;

    if (moves.size() < 3) { // 单步动作
        int direction = start * 4; // 提前计算以避免重复

        if (start_row == end_row) {
            return direction + (start_col < end_col ? 4 : 3) - 1;
        }
        else {
            return direction + (start_row < end_row ? 2 : 1) - 1;
        }
    }
    else { // 跳跃动作
        return 196 * 4 - 1 + start * 49 + end_row / 2 * 7 + end_col / 2 + 1;
    }
}

std::vector<int> MoveEncoder::decode_move_index(int index) const {
    std::vector<int> moves;
    if (index > 196 * 4 - 1) {
        index -= (196 * 4 - 1 + 1);

        // 解码起始点
        int start = index / 49;
        index %= 49;  // 取余数以得到结束点的编码
        int start_row = start / 14 % 2;
        int start_col = start % 14 % 2;
        // 解码结束点的行和列
        int end_row = index / 7;
        int end_col = index % 7;

        // 计算结束点的一维数组索引
        int end = start_row * 14 + start_col + end_row * 28 + end_col * 2;

        // 创建vector并返回
        std::vector<int> moves;
        moves.emplace_back(start);
        moves.emplace_back(end);

        return moves;
    }
    else
    {
        int start = index / 4;
        int end_direction = index % 4;
        int end;
        switch (end_direction) {
        case 0: end = start - 14; break;
        case 1: end = start + 14; break;
        case 2: end = start - 1; break;
        case 3: end = start + 1; break;
        default: end = start; // 作为默认值
        }
        moves.emplace_back(start);
        moves.emplace_back(end);
    }

    return moves;
}

//std::vector<int> MoveEncoder::decode_point_index(int index) const {
//    std::vector<int> moves;
//    index -= (196 * 4 - 1); // 还原移动相关的编码
//    int start = index / 49;
//    index %= 49; // 得到剩余的编码，用于计算结束点
//    int end_row = (index / 7) * 2;
//    int end_col = (index % 7) * 2;
//    int end = end_row * 14 + end_col; // 计算结束点的线性索引
//
//    moves.emplace_back(start);
//    moves.emplace_back(end);
//
//    return moves;
//}


int MoveEncoder::num_points() const {
    return board_width * board_height * 53; //10388
}

std::tuple<int, int, int> MoveEncoder::shape() const {
    return std::make_tuple(num_planes, board_height, board_width);
}

//19307
int MoveEncoder::encode_eat(const std::vector<int>& moves) const {

    if (moves.size() == 1) { // 单步动作

        return moves[0];
    }
    else {
        int i = std::max(moves[0], moves[1]);
        int j = std::min(moves[0], moves[1]);

        // 使用组合公式得到编码
        return 196 + (i * (i - 1) / 2) + j;
    }
}

std::vector<int> MoveEncoder::decode_eat_index(int index) const {
    std::vector<int> moves = {};
    if (index < 196) {
        // 单步动作
        moves.emplace_back(index);
    }
    else if (index == 19306) {
        return moves;
    }
    else
    {
        // 双子吃子动作
        index -= 196;
        int i = static_cast<int>((1 + std::sqrt(1 + 8 * index)) / 2);
        int j = index - (i * (i - 1) / 2);
        moves.emplace_back(i);
        moves.emplace_back(j);
    }

    return moves;
}
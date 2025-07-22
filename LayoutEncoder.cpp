#include "LayoutEncoder.h"


LayoutEncoder::LayoutEncoder(std::pair<int, int> board_size)
    : board_width(board_size.first), board_height(board_size.second), num_planes(3) {}

std::string LayoutEncoder::name() const {
    return "layoutplane";
}

//std::vector<std::vector<std::vector<int>>> LayoutEncoder::encode(const GameState& game_state) {
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

torch::Tensor LayoutEncoder::encode(const GameState& game_state) {
    
    torch::Tensor tensor = torch::zeros({ num_planes, game_state.board.num_rows, game_state.board.num_cols }, torch::kFloat32);
    auto tensor_accessor = tensor.accessor<float, 3>(); // 创建访问器来修改Tensor

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

    return tensor.unsqueeze(0);
}

int LayoutEncoder::encode_point(const Point& point) const {
    return board_width * (point.row - 1) + (point.col - 1);
}

Point LayoutEncoder::decode_point_index(int index) const {
    int row = index / board_width;
    int col = index % board_width;
    return Point(row + 1, col + 1);
}

int LayoutEncoder::num_points() const {
    return board_width * board_height;
}

std::tuple<int, int, int> LayoutEncoder::shape() const {
    return std::make_tuple(num_planes, board_height, board_width);
}
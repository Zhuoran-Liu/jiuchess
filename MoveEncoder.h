#pragma once

#include <vector>
#include <torch/torch.h>
#include "gotypes.h"  
#include "goboard.h"
#include <cmath>
//#include <pybind11/pybind11.h>
//#include <pybind11/numpy.h>
//namespace py = pybind11;

class MoveEncoder {
public:
    int board_width;
    int board_height;
    int num_planes;
    explicit MoveEncoder(std::pair<int, int> board_size, int num_planes);
    std::string name() const;
    torch::Tensor encode(GameState& game_state);
    torch::Tensor eat_encode(const GameState& game_state, std::vector<int> moves);
    int encode_move(const std::vector<int>& point) const;
    std::vector<int> decode_move_index(int index) const;
    int encode_eat(const std::vector<int>& point) const;
    std::vector<int> decode_eat_index(int index) const;
    int num_points() const;
    std::tuple<int, int, int> shape() const;
};


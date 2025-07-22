#pragma once

#include <vector>
#include <torch/torch.h>
#include "gotypes.h"  
#include "goboard.h"
//#include <pybind11/pybind11.h>
//#include <pybind11/numpy.h>
//namespace py = pybind11;

class LayoutEncoder {
public:
    int board_width;
    int board_height;
    int num_planes;
    explicit LayoutEncoder(std::pair<int, int> board_size);
    std::string name() const;
    torch::Tensor encode(const GameState& game_state);
    int encode_point(const Point& point) const;
    Point decode_point_index(int index) const;
    int num_points() const;
    std::tuple<int, int, int> shape() const;


};


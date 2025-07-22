#pragma once
#include "gotypes.h"
#include<vector>
#include "goboard.h"
#include<thread>
#include<mutex>
#include<random>
#include "MoveEncoder.h"

struct ScoredCombination {
    std::vector<int> combination;
    float score_difference;
};
int layout_strategy(const GameState& game_state);
int compute_layout_strategy(const GameState& game_state, const int& point);

float eval_fn(GameState& game_state);
float alpha_beta_result(GameState& game_state, int max_depth, float best_black, float best_white);
//std::pair<std::vector<int>, float>walk_strategy(GameState& game_state);
std::vector<int> walk_strategy(GameState& game_state);
std::vector<int> eat_strategy(GameState& game_state, const std::vector<int>& move);
std::vector<std::pair<float, int>> eat_strategy1(GameState& game_state, const std::vector<int>& move, int nums);
std::vector<int> eat_strategy2(GameState& game_state, const std::vector<int>& move, int nums);
std::vector<int> eat_strategy3(Board& next_board,int nums2, Player player);
std::vector<int> legal_eat(GameState& game_state, const std::vector<int>& move);
float eval_fn1(GameState& game_state, int stones, float diff);
std::vector<std::vector<int>> walk_strategy1(GameState& game_state);
std::vector<std::vector<int>> walk_strategy2(GameState& game_state);
void fly_search_best_moves(GameState game_state, const std::vector<std::vector<int>>& moves, float& global_best_score, std::vector<std::vector<int>>& global_best_moves);
std::vector<int> fly_strategy(GameState& game_state);
std::vector<int> fly_eat_strategy(GameState& game_state, const std::vector<int>& move);
float fly_alpha_beta_result(GameState& game_state, int max_depth, float best_black, float best_white);
float fly_eval_fn(GameState& game_state);
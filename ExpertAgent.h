#pragma once
#include <iostream>
#include <vector>
#include <tuple>
#include <ctime>
#include <algorithm>
#include <random>
#include "goboard.h"

class ExpertAgent
{
public:
	int expert_layout(const GameState& game_state);
	int compute_expert_layout(const GameState& game_state, const int& point);
	std::tuple<std::vector<int>, std::vector<int>> walk_strategy(GameState& gamestate);
	int compute_walk_strategy(const std::vector<int>& move, GameState& gamestate);
	int compute_eat_strategy(const std::vector<int>& move, Board& board, int inital_squires, Player next_player);
	int eat_strategy(Board& board, Player player);
	std::tuple<std::vector<int>, std::vector<int>> select_move(GameState& game_state);
};

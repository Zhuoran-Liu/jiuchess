#pragma once
#include <sstream>
#include <vector>
#include<iostream>
#include<assert.h>
#include<algorithm>
#include<numeric>
#include "gotypes.h"
#include <unordered_map>
#include<set>
#include<mutex>
class Board {
public:
    int num_rows;
    int num_cols;
    std::vector<int> grid;
    /*std::mutex grid_mutex;*/
    Board(int num_rows, int num_cols);
    std::string PointToString(char row, int col);
    void Print_move(std::vector<int> moves, std::vector<int> eat_moves, Player player);
    void displayGrid();
    int& at(int row, int col);
    const int& at(int row, int col) const;
    std::vector<std::vector<int>> legal_moves(Player next_player, int game_state);
    void go_find_jumpmoves(Player next_player, int index, std::vector<int> road, std::vector<std::vector<int>>& roads);
    bool is_valid_jumpmove(Player next_player, int point);
    void place_stone(Player next_player, std::vector<int> pos);
    void go_take_stone(Player next_player, std::vector<int> pos);
    int squire(const Player next_player, int point);
    std::vector<int> eat(Player next_player, int nums);
    std::vector<int> eat1(Player next_player, int nums);
    /* std::vector<std::vector<Point>> legal_moves(Player next_player, int game_state);
     void go_find_jumpmoves(Player next_player, Point point, std::vector<Point> road, std::vector<std::vector<Point>>& roads);
     bool is_valid_jumpmove(Player next_player, Point point);
     void place_stone(Player next_player, std::vector<Point> pos);
     void go_take_stone(Player next_player, std::vector<Point> pos);
     int squire(const Player next_player, Point point);
     std::vector<Point> eat(Player next_player, int nums);*/
    bool is_full_grid();
    /* bool is_valid_move_layout(Point move);*/
    bool is_valid_move_layout(int move);
    bool is_valid_move_move(Player next_player, const std::vector<int>& move);
    bool Board::is_valid_eat_move(Player next_player, const std::vector<int>& moves, const std::vector<int>& eat_move);
    std::vector<std::vector<int>> all_legal_moves(Player next_player, int game_state);
    void all_find_jumpmoves(Player next_player, int index, std::vector<int> road, std::vector<std::vector<int>>& roads);
    void all_take_stone(Player next_player, const std::vector<int>& pos);
    std::vector<Point> same_stone(Player next_player);
    float special_moves(Player next_player);
    void find_specialjump(Player next_player, int index, std::vector<int> road, std::unordered_map<int, float>& roads);
    float special_moves1(Player next_player);
    float special_moves3(Player next_player);
    std::vector<int> special_moves2(Player next_player);
    bool find_specialjump2(Player next_player, int index);
    void find_specialjump1(Player next_player, int index, std::vector<int> road, std::unordered_map<int, float>& roads);
    void find_specialjump3(Player next_player, int index, std::vector<int> road, std::unordered_map<int, float>& roads);
    bool singal_move(Player next_player, int point);
    bool find_singal_jump(Player next_player, int point);
    bool is_valid_singal(Player next_player, int point);
    std::vector<std::vector<int>> singal_moves(Player next_player, int point);
    int sum_stone(Player next_player);
    std::vector<std::vector<int>> cut_legal_moves(Player next_player);
    std::vector<std::vector<int>> fly_legal_moves(Player next_player);
    float fly_special_moves(Player next_player);
    std::vector<std::vector<int>> fly_legal_special(Player next_player);
    // ... 其他成员函数声明 ...
};

class GameState {
public:
    Board board;
    Player next_player;
    Player winner = Player::NONE;
    GameState* previous_state;
    int state;
    int step;

    GameState(Board board, Player next_player, GameState* previous, int state, int step);
    /*GameState go_apply_move(std::vector<Point> pos);*/
    GameState go_apply_move(std::vector<int> pos);
    GameState go_apply_move_mcts(std::vector<int> pos);
    GameState apply_move(std::tuple<std::vector<int>, std::vector<int>>& pos);
    GameState apply_move_mcts(std::tuple<std::vector<int>, std::vector<int>>& pos);
    GameState all_apply_move(const std::vector<int>& pos);
    static GameState new_game(int board_size);
    void game_state(Board& next_board);
    bool is_over();
    bool is_over1();
    Player GameState::score();
    GameState fly_apply_move(std::tuple<std::vector<int>, std::vector<int>>& pos);
    // ... 其他成员函数声明 ...
};

std::string PointToString1(char row, int col);

void Print_move1(std::vector<int> moves, Player player);



//#ifndef GOBOARD_H
//#define GOBOARD_H
//
//#include <vector>
//#include "gotypes.h"
//
//
//class Board {
//public:
//    int num_rows;
//    int num_cols;
//    std::vector<int> grid;
//
//    Board(int num_rows, int num_cols);
//    void displayGrid();
//    int& at(int row, int col);
//    const int& at(int row, int col) const;
//    std::vector<std::vector<Point>> legal_moves(Player next_player, int game_state);
//    void go_find_jumpmoves(Player next_player, Point point, std::vector<Point> road, std::vector<std::vector<Point>>& roads);
//    bool is_valid_jumpmove(Player next_player, Point point);
//    void place_stone(Player next_player, std::vector<Point> pos);
//    void go_take_stone(Player next_player, std::vector<Point> pos);
//    int squire(const Player next_player, Point point);
//    std::vector<Point> eat(Player next_player, int nums);
//    bool is_full_grid();
//    bool is_valid_move_layout(Point move);
//    // ... 其他成员函数声明 ...
//};
//
//class GameState {
//public:
//    Board board;
//    Player next_player;
//    GameState* previous_state;
//    int state;
//
//    GameState(Board board, Player next_player, GameState* previous, int state);
//    GameState go_apply_move(std::vector<Point> pos);
//    static GameState new_game(int board_size);
//    void game_state(Board& next_board);
//    // ... 其他成员函数声明 ...
//};
//
//#endif // GOBOARD_H

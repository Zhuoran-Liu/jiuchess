#pragma once
#include<random>
#include <map>
#include <memory>
#include <vector>
#include "goboard.h"
#include<thread>
#include<atomic>
#include"ThreadPool.h"
#include "MoveEncoder.h"
#include <iostream>
#include <fstream>
#include<torch/script.h>
#include"Experience.h"
#include<cmath>
#include<algorithm>
#include"Net.h"
#include"Expert.h"
#include"ExpertAgent.h"
class Move_Branch {
public:
    float prior;
    std::atomic<int> visit_count;
    //int visit_count;
    std::atomic<int> incomplete_visit;
    float total_value;
    Move_Branch(float prior);
    Move_Branch() : prior(0.0), visit_count(0), total_value(0.0) {}
    Move_Branch(Move_Branch&& other) noexcept;

    // 移动赋值操作符
    Move_Branch& operator=(Move_Branch&& other) noexcept;

    // 禁用拷贝构造函数和拷贝赋值操作符
    Move_Branch(const Move_Branch&) = delete;
    Move_Branch& operator=(const Move_Branch&) = delete;
};
extern std::mutex output_mutex2;

class Move_TreeNode {
public:
    std::shared_ptr<GameState> state;
    float value;
    std::shared_ptr<Move_TreeNode> parent;
    std::map<int, int> eat_moves;
    int last_move;
    int total_visit_count;
    /*std::atomic<int> total_incomplete_visit;*/
    //std::map<Point, Move_Branch> branches;
    //std::map<Point, std::shared_ptr<Move_TreeNode>> children;
    std::map<int, Move_Branch> branches;
    std::map<int, std::shared_ptr<Move_TreeNode>> children;
    std::mutex data_lock;
    std::mutex cout_lock;

    friend class Move_Agent;
    //Move_TreeNode(std::shared_ptr<GameState> state, float value, const std::map<Point, float>& priors, std::shared_ptr<Move_TreeNode> parent, Point last_move);
    //std::vector<Point> moves();
    //void add_child(Point move, std::shared_ptr<Move_TreeNode> child_node);
    //bool has_child(Point move);
    //std::shared_ptr<Move_TreeNode> get_child(Point move);
    //void complete_visit(Point move, float value);
    //void incomplete_visit(Point move);
    //float expected_value(Point move);
    //float prior(Point move);
    //int visit_count(Point move);
    Move_TreeNode(std::shared_ptr<GameState> state, float value, const std::map<int, float>& priors, std::shared_ptr<Move_TreeNode> parent, int last_move);
    std::vector<int> moves();
    void add_child(int move, std::shared_ptr<Move_TreeNode> child_node);
    bool has_child(int move);
    std::shared_ptr<Move_TreeNode> get_child(int move);
    void complete_visit(int move, float value);
    void incomplete_visit(int move);
    float expected_value(int move);
    float prior(int move);
    int visit_count(int move);

};

//namespace py = pybind11;
class Move_Agent {
private:
    std::mutex cout_lock;
    
    torch::jit::script::Module module; // 修改后的成员变量
    std::shared_ptr<MoveEncoder> encoder;
    std::shared_ptr<MoveEncoder> encoder1;
    int num_rounds;
    float c;
    ThreadPool thread_pool;
    std::mutex model_lock;
    Net* net;
    /*Net* net1;*/
public:
    /*Move_Agent(Net* net, Net* net1, int rounds_per_move, float c, ZeroExperienceCollector& collector);*/
    Move_Agent(Net* net, const std::string& model_path, int rounds_per_move, float c);
    /* Move_Agent(int rounds_per_move = 1600, float c = 2.0);*/
    /*std::vector<Point> select_move(std::shared_ptr<GameState> game_state);*/
    std::tuple<std::vector<int>, std::vector<int>> select_move(std::shared_ptr<GameState> game_state);
    void simulate_round(std::shared_ptr<Move_TreeNode> root);
    /*std::shared_ptr<Move_TreeNode> create_node(std::shared_ptr<GameState> game_state, Point move = Point(-1, -1), std::shared_ptr<Move_TreeNode> parent = nullptr);
    Point select_branch(std::shared_ptr<Move_TreeNode> node);*/
    std::shared_ptr<Move_TreeNode> create_node(std::shared_ptr<GameState> game_state, int move = -1, std::shared_ptr<Move_TreeNode> parent = nullptr);
    std::shared_ptr<Move_TreeNode> create_node_dirichlet(std::shared_ptr<GameState> game_state, int move = -1, std::shared_ptr<Move_TreeNode> parent = nullptr);
    std::shared_ptr<Move_TreeNode> create_node_gaussian(std::shared_ptr<GameState> game_state, int move = -1, std::shared_ptr<Move_TreeNode> parent = nullptr);
    std::shared_ptr<Move_TreeNode> create_node_huadong(std::shared_ptr<GameState> game_state, int move = -1, std::shared_ptr<Move_TreeNode> parent = nullptr);
    std::shared_ptr<Move_TreeNode> p_create_node(std::shared_ptr<GameState> game_state, int move = -1, std::shared_ptr<Move_TreeNode> parent = nullptr);
    int select_branch(std::shared_ptr<Move_TreeNode> node);
    ZeroExperienceCollector& getCollector();
};





#pragma once
#include<random>
#include <map>
#include <memory>
#include <vector>
#include "goboard.h"
#include<thread>
#include<atomic>
#include"ThreadPool.h"
#include "LayoutEncoder.h"
#include <iostream>
#include <fstream>
#include<torch/script.h>
#include"Experience.h"
#include<cmath>
#include<algorithm>
#include"Net.h"

class P_Branch {
public:
    float prior;
    std::atomic<int> visit_count;
    //int visit_count;
    std::atomic<int> incomplete_visit;
    float total_value;
    P_Branch(float prior);
    P_Branch() : prior(0.0), visit_count(0), total_value(0.0) {}
    P_Branch(P_Branch&& other) noexcept;

    // 移动赋值操作符
    P_Branch& operator=(P_Branch&& other) noexcept;

    // 禁用拷贝构造函数和拷贝赋值操作符
    P_Branch(const P_Branch&) = delete;
    P_Branch& operator=(const P_Branch&) = delete;
};


class P_ZeroTreeNode {
public:
    std::shared_ptr<GameState> state;
    float value;
    std::shared_ptr<P_ZeroTreeNode> parent;
    /*Point last_move;*/
    int last_move;
    int total_visit_count;
    /*std::atomic<int> total_incomplete_visit;*/
    //std::map<Point, P_Branch> branches;
    //std::map<Point, std::shared_ptr<P_ZeroTreeNode>> children;
    std::map<int, P_Branch> branches;
    std::map<int, std::shared_ptr<P_ZeroTreeNode>> children;
    std::mutex data_lock;
    std::mutex cout_lock;

    friend class P_ZeroAgent;
    //P_ZeroTreeNode(std::shared_ptr<GameState> state, float value, const std::map<Point, float>& priors, std::shared_ptr<P_ZeroTreeNode> parent, Point last_move);
    //std::vector<Point> moves();
    //void add_child(Point move, std::shared_ptr<P_ZeroTreeNode> child_node);
    //bool has_child(Point move);
    //std::shared_ptr<P_ZeroTreeNode> get_child(Point move);
    //void complete_visit(Point move, float value);
    //void incomplete_visit(Point move);
    //float expected_value(Point move);
    //float prior(Point move);
    //int visit_count(Point move);
    P_ZeroTreeNode(std::shared_ptr<GameState> state, float value, const std::map<int, float>& priors, std::shared_ptr<P_ZeroTreeNode> parent, int last_move);
    std::vector<int> moves();
    void add_child(int move, std::shared_ptr<P_ZeroTreeNode> child_node);
    bool has_child(int move);
    std::shared_ptr<P_ZeroTreeNode> get_child(int move);
    void complete_visit(int move, float value);
    void incomplete_visit(int move);
    float expected_value(int move);
    float prior(int move);
    int visit_count(int move);

};

//namespace py = pybind11;
class P_ZeroAgent {
private:
    std::mutex cout_lock;
    
    torch::jit::script::Module model; // 修改后的成员变量
    std::shared_ptr<LayoutEncoder> encoder;
    int num_rounds;
    float c;
    ThreadPool thread_pool;
    std::mutex model_lock;
    Net* net;
public:
    P_ZeroAgent(Net* net, int rounds_per_move, float c);

    /* P_ZeroAgent(int rounds_per_move = 1600, float c = 2.0);*/
    /*std::vector<Point> select_move(std::shared_ptr<GameState> game_state);*/
    std::vector<int> select_move(std::shared_ptr<GameState> game_state);
    void simulate_round(std::shared_ptr<P_ZeroTreeNode> root);
    /*std::shared_ptr<P_ZeroTreeNode> create_node(std::shared_ptr<GameState> game_state, Point move = Point(-1, -1), std::shared_ptr<P_ZeroTreeNode> parent = nullptr);
    Point select_branch(std::shared_ptr<P_ZeroTreeNode> node);*/
    std::shared_ptr<P_ZeroTreeNode> create_node(std::shared_ptr<GameState> game_state, int move = -1, std::shared_ptr<P_ZeroTreeNode> parent = nullptr);
    std::shared_ptr<P_ZeroTreeNode> create_node_dirichlet(std::shared_ptr<GameState> game_state, int move = -1, std::shared_ptr<P_ZeroTreeNode> parent = nullptr);
    std::shared_ptr<P_ZeroTreeNode> create_node_gaussian(std::shared_ptr<GameState> game_state, int move = -1, std::shared_ptr<P_ZeroTreeNode> parent = nullptr);
    std::shared_ptr<P_ZeroTreeNode> create_node_huadong(std::shared_ptr<GameState> game_state, int move = -1, std::shared_ptr<P_ZeroTreeNode> parent = nullptr);
    std::shared_ptr<P_ZeroTreeNode> p_create_node(std::shared_ptr<GameState> game_state, int move = -1, std::shared_ptr<P_ZeroTreeNode> parent = nullptr);
    int select_branch(std::shared_ptr<P_ZeroTreeNode> node);
    
};





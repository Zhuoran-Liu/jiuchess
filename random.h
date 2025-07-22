//#ifndef RANDOM_H
//#define RANDOM_H
//
////#include "agent.h"  
//#include "goboard.h"  
//#include <vector>
//
//class ExpertBot3 {
//public:
//    std::vector<Point> select_move(GameState& game_state);
//};
//
//#endif
#pragma once

//#include "agent.h"  
#include "goboard.h"  
#include <vector>
#include"Expert.h"
#include <random>
#include"ParallelTreeNode.h"
#include"Experience.h"
#include "MoveExperience.h"
#include "MoveEncoder.h"
#include "MoveAgent.h"
#include "ExpertAgent.h"
class Bot
{
public:
    virtual std::tuple<std::vector<int>, std::vector<int>> select_move(GameState& game_state) = 0;
};


class RLBot :public Bot {
public:
    Move_Agent agent;
    P_ZeroAgent layout_agent;
    std::shared_ptr<MoveEncoder> encoder;
   
    explicit RLBot(Net* net, Net* net1, const std::string& model_path, int rounds_per_move, int rounds_per_move1, float temp) : agent(net1,model_path, rounds_per_move1, temp),
        layout_agent(net, rounds_per_move, temp),
        encoder(std::make_shared<MoveEncoder>(std::make_pair(14, 14), 3)) {}
    std::tuple<std::vector<int>, std::vector<int>> select_move(GameState& game_state) override;
    
};

class RLBot1 :public Bot {
public:
    Move_Agent agent;
    
    std::shared_ptr<MoveEncoder> encoder;

    explicit RLBot1(Net* net, const std::string& model_path, int rounds_per_move ,float temp) : agent(net, model_path, rounds_per_move, temp),
        
        encoder(std::make_shared<MoveEncoder>(std::make_pair(14, 14), 3)) {}
    std::tuple<std::vector<int>, std::vector<int>> select_move(GameState& game_state) override;

};

class ExpertBot :public Bot {
public:
    ExpertAgent agent;
 
    std::tuple<std::vector<int>, std::vector<int>> select_move(GameState& game_state) override;

};

class ExpertBotm :public Bot {
public:
   
    std::tuple<std::vector<int>, std::vector<int>> select_move(GameState& game_state) override;

};
class bisaiBot :public Bot {
public:

    std::tuple<std::vector<int>, std::vector<int>> select_move(GameState& game_state) override;

};
//class ExpertBot9 :public Bot {
//public:
//    Move_Agent agent;
//    ZeroExperienceCollector* move_collector;
//    std::shared_ptr<MoveEncoder> encoder;
//    /*explicit ExpertBot7(Net* net, Net* net1, int param1, float param2, ZeroExperienceCollector& collector) : agent(net, net1, param1, param2, collector),
//        encoder(std::make_shared<MoveEncoder>(std::make_pair(14, 14), 3)) {}*/
//    explicit ExpertBot9(Net* net, int param1, float param2, ZeroExperienceCollector& collector) : agent(net, param1, param2, collector),
//        encoder(std::make_shared<MoveEncoder>(std::make_pair(14, 14), 3)) {}
//    std::tuple<std::vector<int>, std::vector<int>> select_move(GameState& game_state) override;
//    ZeroExperienceCollector& getCollector() {
//        return agent.getCollector();
//    }
//};

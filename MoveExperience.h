#pragma once
#include <vector>
#include<torch/script.h>
#include <H5Cpp.h>
#include <string>
#include <filesystem>

class MoveExperienceCollector {
public:
    void begin_episode();
    void record_decision(const torch::Tensor& state, const int& move_actions, const int& eat_actions);
    void complete_episode(int reward);
    void clear_collector();
    /*void serialize(const std::string& filename) const;*/
    
    std::vector<torch::Tensor> states;
    /*std::vector<int> advantages;*/
    std::vector<int> move_actions;
    std::vector<int> eat_actions;
    std::vector<int> rewards;
    std::vector<torch::Tensor> _current_episode_states;
    std::vector<int> _current_episode_move_actions;
    std::vector<int> _current_episode_eat_actions;
    /*std::vector<int> _current_episode_estimated_values;*/

};

class MoveExperienceBuffer {
public:
    MoveExperienceBuffer() = default;
    MoveExperienceBuffer(const std::vector<torch::Tensor>& states,
        const std::vector<int>& move_actions,
        const std::vector<int>& eat_actions,
        const std::vector<int>& rewards);
    void serialize() const;
    MoveExperienceBuffer combine_experience(std::vector<MoveExperienceCollector>& collectors);

    std::vector<torch::Tensor> states;
    std::vector<int> move_actions;
    std::vector<int> eat_actions;
    std::vector<int> rewards;
    /*std::vector<int> advantages;*/
};

// Other functions
//MoveExperienceBuffer combine_experience(const std::vector<ZeroExperienceCollector>& collectors);
//ZeroExperienceBuffer load_experience(const H5std_string& filename);
#pragma once

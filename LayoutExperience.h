#pragma once
#include <vector>
#include<torch/script.h>
#include <H5Cpp.h>
#include <string>
#include <filesystem>

class LayoutExperienceCollector {
public:
    void begin_episode();
    void record_decision(const torch::Tensor& state, const int& layout_actions);
    void complete_episode(int reward);
    void clear_collector();
    /*void serialize(const std::string& filename) const;*/

    std::vector<torch::Tensor> states;
    /*std::vector<int> advantages;*/
    std::vector<int> layout_actions;
    std::vector<int> rewards;
    std::vector<torch::Tensor> _current_episode_states;
    std::vector<int> _current_episode_layout_actions;
    /*std::vector<int> _current_episode_estimated_values;*/

};

class LayoutExperienceBuffer {
public:
    LayoutExperienceBuffer() = default;
    LayoutExperienceBuffer(const std::vector<torch::Tensor>& states,
        const std::vector<int>& layout_actions,
        const std::vector<int>& rewards);
    void serialize() const;
    LayoutExperienceBuffer combine_experience(std::vector<LayoutExperienceCollector>& collectors);

    std::vector<torch::Tensor> states;
    std::vector<int> layout_actions;
    std::vector<int> rewards;
    /*std::vector<int> advantages;*/
};


#pragma once

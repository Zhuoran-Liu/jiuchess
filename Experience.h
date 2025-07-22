#pragma once
#include <vector>
#include<torch/script.h>
#include <H5Cpp.h>
#include <string>
#include <filesystem>
class ZeroExperienceCollector {
public:
    void begin_episode();
    void record_decision(const torch::Tensor& state, const std::vector<int>& visit_counts);
    void complete_episode(int reward);
    /*void serialize(const std::string& filename) const;*/
    void clear_experience();
    

    std::vector<torch::Tensor> states;
    std::vector<std::vector<int>> visit_counts;
    std::vector<int> rewards;
    std::vector<torch::Tensor> _current_episode_states;
    std::vector<std::vector<int>> _current_episode_visit_counts;
};

class ZeroExperienceBuffer {
public:
    ZeroExperienceBuffer() = default;
    ZeroExperienceBuffer(const std::vector<torch::Tensor>& states,
        const std::vector<std::vector<int>>& visit_counts,
        const std::vector<int>& rewards);
    void serialize() const;
    ZeroExperienceBuffer combine_experience(std::vector<ZeroExperienceCollector>& collectors);

    std::vector<torch::Tensor> states;
    std::vector<std::vector<int>> visit_counts;
    std::vector<int> rewards;
};  

// Other functions
//ZeroExperienceBuffer combine_experience(const std::vector<ZeroExperienceCollector>& collectors);
//ZeroExperienceBuffer load_experience(const H5std_string& filename);

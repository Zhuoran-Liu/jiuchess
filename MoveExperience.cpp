#include "MoveExperience.h"

void MoveExperienceCollector::begin_episode() {
    _current_episode_states.clear();
    _current_episode_move_actions.clear();
    _current_episode_eat_actions.clear();
    /*_current_episode_estimated_values.clear();*/

}

void MoveExperienceCollector::record_decision(const torch::Tensor& state, const int& move_actions, const int& eat_actions) {
    _current_episode_states.emplace_back(state);
    _current_episode_move_actions.emplace_back(move_actions);
    _current_episode_eat_actions.emplace_back(eat_actions);
    /*_current_episode_estimated_values.emplace_back(estimated_value);*/

}

void MoveExperienceCollector::complete_episode(int reward) {
    int num_states = _current_episode_states.size();
    states.insert(states.end(), _current_episode_states.begin(), _current_episode_states.end());
    move_actions.insert(move_actions.end(), _current_episode_move_actions.begin(), _current_episode_move_actions.end());
    eat_actions.insert(eat_actions.end(), _current_episode_eat_actions.begin(), _current_episode_eat_actions.end());
    rewards.insert(rewards.end(), num_states, reward);
   /* for (int i = 0; i < num_states; ++i) {
        double advantage = reward - _current_episode_estimated_values[i];
        advantages.emplace_back(advantage);
    }*/
    _current_episode_states.clear();
    _current_episode_move_actions.clear();
    _current_episode_eat_actions.clear();
    /*_current_episode_estimated_values.clear();*/
}

void MoveExperienceCollector::clear_collector() {
    states.clear();
    move_actions.clear();
    eat_actions.clear();
    rewards.clear();
}

MoveExperienceBuffer::MoveExperienceBuffer(const std::vector<torch::Tensor>& states,
    const std::vector<int>& move_actions,
    const std::vector<int>& eat_actions,
    const std::vector<int>& rewards)
    /*const std::vector<int>& advantages)*/
    : states(states), move_actions(move_actions), eat_actions(eat_actions), rewards(rewards){
    // Constructor implementation
}



MoveExperienceBuffer MoveExperienceBuffer::combine_experience(std::vector<MoveExperienceCollector>& collectors) {
    std::vector<torch::Tensor> combined_states;
    std::vector<int> combined_move_actions;
    std::vector<int> combined_eat_actions;
    /*std::vector<int> combined_advantages;*/
    std::vector<int> combined_rewards;

    for (const auto& collector : collectors) {
        combined_states.insert(combined_states.end(), collector.states.begin(), collector.states.end());
        combined_move_actions.insert(combined_move_actions.end(), collector.move_actions.begin(), collector.move_actions.end());
        combined_eat_actions.insert(combined_eat_actions.end(), collector.eat_actions.begin(), collector.eat_actions.end());
        combined_rewards.insert(combined_rewards.end(), collector.rewards.begin(), collector.rewards.end());
        /*combined_advantages.insert(combined_advantages.end(), collector.advantages.begin(), collector.advantages.end());*/

    }
    return MoveExperienceBuffer(combined_states, combined_move_actions, combined_eat_actions, combined_rewards);
}

void MoveExperienceBuffer::serialize() const {
    H5::H5File file;
    H5::Group group;

    // 检查文件是否存在
    std::string filename = "move.h5";
    if (std::filesystem::exists(filename)) {
        file.openFile(filename, H5F_ACC_RDWR);
        group = file.openGroup("/experience");
    }

    else {
        file = H5::H5File(filename, H5F_ACC_TRUNC);
        group = file.createGroup("/experience");

        // 创建states数据集
        hsize_t states_dims[4] = { 0, 3 /* C */, 14 /* H */, 14 /* W */ };
        hsize_t states_max_dims[4] = { H5S_UNLIMITED, 3, 14, 14 };
        H5::DataSpace states_space(4, states_dims, states_max_dims);
        H5::DSetCreatPropList states_prop;
        hsize_t states_chunk_dims[4] = { 1, 3, 14, 14 };
        states_prop.setChunk(4, states_chunk_dims);
        group.createDataSet("/experience/states", H5::PredType::NATIVE_FLOAT, states_space, states_prop);

        //// 创建visit_counts数据集
        //hsize_t visit_counts_dims[2] = { 0, 196 };
        //hsize_t visit_counts_max_dims[2] = { H5S_UNLIMITED, 196 };
        //H5::DataSpace visit_counts_space(2, visit_counts_dims, visit_counts_max_dims);
        //H5::DSetCreatPropList visit_counts_prop;
        //hsize_t visit_counts_chunk_dims[2] = { 1, 196 };
        //visit_counts_prop.setChunk(2, visit_counts_chunk_dims);
        //group.createDataSet("/experience/visit_counts", H5::PredType::NATIVE_INT, visit_counts_space, visit_counts_prop);

        // 创建move_move_actions数据集
        hsize_t move_actions_dims[1] = { 0 };
        hsize_t move_actions_max_dims[1] = { H5S_UNLIMITED };
        H5::DataSpace move_actions_space(1, move_actions_dims, move_actions_max_dims);
        H5::DSetCreatPropList move_actions_prop;
        hsize_t move_actions_chunk_dims[1] = { 1 };
        move_actions_prop.setChunk(1, move_actions_chunk_dims);
        group.createDataSet("/experience/move_actions", H5::PredType::NATIVE_INT, move_actions_space, move_actions_prop);

        hsize_t eat_actions_dims[1] = { 0 };
        hsize_t eat_actions_max_dims[1] = { H5S_UNLIMITED };
        H5::DataSpace eat_actions_space(1, eat_actions_dims, eat_actions_max_dims);
        H5::DSetCreatPropList eat_actions_prop;
        hsize_t eat_actions_chunk_dims[1] = { 1 };
        eat_actions_prop.setChunk(1, eat_actions_chunk_dims);
        group.createDataSet("/experience/eat_actions", H5::PredType::NATIVE_INT, eat_actions_space, eat_actions_prop);

        // 创建rewards数据集
        hsize_t rewards_dims[1] = { 0 };
        hsize_t rewards_max_dims[1] = { H5S_UNLIMITED };
        H5::DataSpace rewards_space(1, rewards_dims, rewards_max_dims);
        H5::DSetCreatPropList rewards_prop;
        hsize_t rewards_chunk_dims[1] = { 1 };
        rewards_prop.setChunk(1, rewards_chunk_dims);
        group.createDataSet("/experience/rewards", H5::PredType::NATIVE_INT, rewards_space, rewards_prop);

        /*hsize_t advantages_dims[1] = { 0 };
        hsize_t advantages_max_dims[1] = { H5S_UNLIMITED };
        H5::DataSpace advantages_space(1, advantages_dims, advantages_max_dims);
        H5::DSetCreatPropList advantages_prop;
        hsize_t advantages_chunk_dims[1] = { 1 };
        advantages_prop.setChunk(1, advantages_chunk_dims);
        group.createDataSet("/experience/advantages", H5::PredType::NATIVE_FLOAT, advantages_space, advantages_prop);*/
    }



    // 获取现有的数据集
    H5::DataSet states_dataset = group.openDataSet("/experience/states");
    H5::DataSet move_actions_dataset = group.openDataSet("/experience/move_actions");
    H5::DataSet eat_actions_dataset = group.openDataSet("/experience/eat_actions");
    H5::DataSet rewards_dataset = group.openDataSet("/experience/rewards");
    /*H5::DataSet advantages_dataset = group.openDataSet("/experience/advantages");*/

    // 扩展 states 数据集的大小
    hsize_t states_current_dims[4];
    states_dataset.getSpace().getSimpleExtentDims(states_current_dims);
    states_current_dims[0] += states.size();
    states_dataset.extend(states_current_dims);

    // 扩展 move_actions 数据集的大小
    hsize_t move_actions_current_dims[1];
    move_actions_dataset.getSpace().getSimpleExtentDims(move_actions_current_dims);
    move_actions_current_dims[0] += move_actions.size();
    move_actions_dataset.extend(move_actions_current_dims);

    hsize_t eat_actions_current_dims[1];
    eat_actions_dataset.getSpace().getSimpleExtentDims(eat_actions_current_dims);
    eat_actions_current_dims[0] += eat_actions.size();
    eat_actions_dataset.extend(eat_actions_current_dims);

    // 扩展 rewards 数据集的大小
    hsize_t rewards_current_dims[1];
    rewards_dataset.getSpace().getSimpleExtentDims(rewards_current_dims);
    rewards_current_dims[0] += rewards.size();
    rewards_dataset.extend(rewards_current_dims);

    // 扩展 advantages 数据集的大小
   /* hsize_t advantages_current_dims[1];
    advantages_dataset.getSpace().getSimpleExtentDims(advantages_current_dims);
    advantages_current_dims[0] += advantages.size();
    advantages_dataset.extend(advantages_current_dims);*/

    // 为 states 数据选择要写入的超平面
    hsize_t states_offset[4] = { states_current_dims[0] - states.size(), 0, 0, 0 };
    hsize_t states_count[4] = { states.size(), 3, 14, 14 };
    H5::DataSpace states_file_space = states_dataset.getSpace();
    states_file_space.selectHyperslab(H5S_SELECT_SET, states_count, states_offset);
    H5::DataSpace states_mem_space(4, states_count); // 源数据空间

    // 为 move_actionss 数据选择要写入的超平面
    hsize_t move_actions_offset[1] = { move_actions_current_dims[0] - move_actions.size()};
    hsize_t move_actions_count[1] = { move_actions.size()};
    H5::DataSpace move_actions_file_space = move_actions_dataset.getSpace();
    move_actions_file_space.selectHyperslab(H5S_SELECT_SET, move_actions_count, move_actions_offset);
    H5::DataSpace move_actions_mem_space(1, move_actions_count); // 源数据空间

    hsize_t eat_actions_offset[1] = { eat_actions_current_dims[0] - eat_actions.size() };
    hsize_t eat_actions_count[1] = { eat_actions.size() };
    H5::DataSpace eat_actions_file_space = eat_actions_dataset.getSpace();
    eat_actions_file_space.selectHyperslab(H5S_SELECT_SET, eat_actions_count, eat_actions_offset);
    H5::DataSpace eat_actions_mem_space(1, eat_actions_count); // 源数据空间

    // 为 rewards 数据选择要写入的超平面
    hsize_t rewards_offset[1] = { rewards_current_dims[0] - rewards.size() };
    hsize_t rewards_count[1] = { rewards.size() };
    H5::DataSpace rewards_file_space = rewards_dataset.getSpace();
    rewards_file_space.selectHyperslab(H5S_SELECT_SET, rewards_count, rewards_offset);
    H5::DataSpace rewards_mem_space(1, rewards_count); // 源数据空间

    // 为 advantages 数据选择要写入的超平面
    //hsize_t advantages_offset[1] = { advantages_current_dims[0] - advantages.size() };
    //hsize_t advantages_count[1] = { advantages.size() };
    //H5::DataSpace advantages_file_space = advantages_dataset.getSpace();
    //advantages_file_space.selectHyperslab(H5S_SELECT_SET, advantages_count, advantages_offset);
    //H5::DataSpace advantages_mem_space(1, advantages_count); // 源数据空间

    //    std::vector<float> flat_states_data(states.size() * 3 * 14 * 14);
//    for (size_t i = 0; i < states.size(); ++i) {
//        torch::Tensor state = states[i].contiguous(); // 确保连续内存
//        const float* data = state.data_ptr<float>();
//        std::copy(data, data + (3 * 14 * 14), flat_states_data.begin() + i * (3 * 14 * 14));
//    }
//    states_dataset.write(flat_states_data.data(), H5::PredType::NATIVE_FLOAT);

    // 将 states 数据写入文件
    std::vector<float> flat_states_data(states.size() * 3 * 14 * 14);
    for (size_t i = 0; i < states.size(); ++i) {
        torch::Tensor state = states[i].contiguous();
        const float* data = state.data_ptr<float>();
        std::copy(data, data + (3 * 14 * 14), flat_states_data.begin() + i * (3 * 14 * 14));
    }
    states_dataset.write(flat_states_data.data(), H5::PredType::NATIVE_FLOAT, states_mem_space, states_file_space);
    move_actions_dataset.write(move_actions.data(), H5::PredType::NATIVE_INT, move_actions_mem_space, move_actions_file_space);
    eat_actions_dataset.write(eat_actions.data(), H5::PredType::NATIVE_INT, eat_actions_mem_space, eat_actions_file_space);
    rewards_dataset.write(rewards.data(), H5::PredType::NATIVE_INT, rewards_mem_space, rewards_file_space);
    /*advantages_dataset.write(advantages.data(), H5::PredType::NATIVE_FLOAT, advantages_mem_space, advantages_file_space);*/

    // 关闭数据集
    states_dataset.close();
    move_actions_dataset.close();
    eat_actions_dataset.close();
    rewards_dataset.close();
   /* advantages_dataset.close();*/

    // 关闭组
    group.close();

    // 关闭文件
    file.close();
}
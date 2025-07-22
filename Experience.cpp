#include "Experience.h"

void ZeroExperienceCollector::begin_episode() {
    _current_episode_states.clear();
    _current_episode_visit_counts.clear();
}

void ZeroExperienceCollector::record_decision(const torch::Tensor& state, const std::vector<int>& visit_counts) {
    _current_episode_states.emplace_back(state);
    _current_episode_visit_counts.emplace_back(visit_counts);
    
}

void ZeroExperienceCollector::complete_episode(int reward) {
    int num_states = _current_episode_states.size();
    states.insert(states.end(), _current_episode_states.begin(), _current_episode_states.end());
    visit_counts.insert(visit_counts.end(), _current_episode_visit_counts.begin(), _current_episode_visit_counts.end());
    rewards.insert(rewards.end(), num_states, reward);

    _current_episode_states.clear();
    _current_episode_visit_counts.clear();
}

void ZeroExperienceCollector::clear_experience() {
    states.clear();
    visit_counts.clear();
    rewards.clear();
}

//void ZeroExperienceCollector::complete_episode(int reward) {
//    // 打开文件以追加模式
//    std::ofstream states_file("states.csv", std::ios::app);
//    std::ofstream visit_counts_file("visit_counts.csv", std::ios::app);
//    std::ofstream rewards_file("rewards.csv", std::ios::app);
//
//    if (!states_file.is_open() || !visit_counts_file.is_open() || !rewards_file.is_open()) {
//        throw std::runtime_error("Unable to open files");
//    }
//
//    // 追加 _current_episode_states 和 _current_episode_visit_counts 到文件
//    for (size_t i = 0; i < _current_episode_states.size(); ++i) {
//        states_file << _current_episode_states[i] << std::endl;
//        visit_counts_file << _current_episode_visit_counts[i] << std::endl;
//    }
//
//    // 追加 reward 到文件，重复的次数是 _current_episode_states 的大小
//    for (size_t i = 0; i < _current_episode_states.size(); ++i) {
//        rewards_file << reward << std::endl;
//    }
//
//    // 清空当前episode的数据
//    _current_episode_states.clear();
//    _current_episode_visit_counts.clear();
//
//    // 关闭文件
//    states_file.close();
//    visit_counts_file.close();
//    rewards_file.close();
//}


ZeroExperienceBuffer::ZeroExperienceBuffer(const std::vector<torch::Tensor>& states,
    const std::vector<std::vector<int>>& visit_counts,
    const std::vector<int>& rewards)
    : states(states), visit_counts(visit_counts), rewards(rewards) {
    // Constructor implementation
}

//void ZeroExperienceCollector::serialize(const std::string& filename) const {
//    try {
//        H5::H5File file;
//        std::ifstream f("test3.h5");
//
//        if (f.good()) {
//            // 文件存在，以读写模式打开
//            file = H5::H5File("test3.h5", H5F_ACC_RDWR);
//        }
//        else {
//            // 文件不存在，创建新文件
//            file = H5::H5File("test3.h5", H5F_ACC_TRUNC);
//        }
//        /*H5::H5File file("test3.h5", H5F_ACC_RDWR);*/
//
//        // 对于states，visit_counts和rewards，我们需要创建HDF5数据集
//        // 假设states是一个tensor列表，每个tensor的数据类型和形状可能不同
//        // 这里仅示例如何将一个固定类型和形状的tensor保存到HDF5
//
//        // 首先，我们创建一个组
//        /*H5::Group group = file.createGroup("/experience");*/
//        H5::Group group;
//        if (H5Lexists(file.getId(), "/experience", H5P_DEFAULT)) {
//            group = file.openGroup("/experience");
//        }
//        else {
//            group = file.createGroup("/experience");
//        }
//
//        // 序列化states
//        hsize_t states_dims[4] = { states.size(), 3 /* C */, 14 /* H */, 14 /* W */ };
//        std::string states_dataset_name = "/experience/states";
//        H5::DataSet states_dataset;
//        // 检查states数据集是否存在
//        if (H5Lexists(group.getId(), states_dataset_name.c_str(), H5P_DEFAULT)) {
//            // 数据集已存在，直接打开
//            states_dataset = group.openDataSet("/experience/states");
//        }
//        else {
//            // 数据集不存在，创建它
//            H5::DataSpace states_space(4, states_dims);
//            states_dataset = group.createDataSet(states_dataset_name, H5::PredType::NATIVE_FLOAT, states_space);
//        }
//       /* H5::DataSpace states_space(4, states_dims);
//        H5::DataSet states_dataset = group.createDataSet("/experience/states", H5::PredType::NATIVE_FLOAT, states_space);*/
//        // 将所有states的数据拷贝到一个连续的内存块中
//        std::vector<float> flat_states_data(states.size() * 3 * 14 * 14);
//        for (size_t i = 0; i < states.size(); ++i) {
//            torch::Tensor state = states[i].contiguous(); // 确保连续内存
//            const float* data = state.data_ptr<float>();
//            std::copy(data, data + (3 * 14 * 14), flat_states_data.begin() + i * (3 * 14 * 14));
//        }
//        states_dataset.write(flat_states_data.data(), H5::PredType::NATIVE_FLOAT);
//
//        // 序列化visit_counts
//        hsize_t visit_counts_dims[2] = { visit_counts.size(), 196 }; // 第一维是向量的数量，第二维是每个向量的长度
//        /*H5::DataSpace visit_counts_space(2, visit_counts_dims);
//        H5::DataSet visit_counts_dataset = group.createDataSet("/experience/visit_counts", H5::PredType::NATIVE_INT, visit_counts_space);*/
//        std::string visit_counts_dataset_name = "/experience/visit_counts";
//        H5::DataSet visit_counts_dataset;
//        if (H5Lexists(group.getId(), visit_counts_dataset_name.c_str(), H5P_DEFAULT)) {
//            // 数据集已存在，直接打开
//            visit_counts_dataset = group.openDataSet("/experience/visit_counts");
//        }
//        else {
//            // 数据集不存在，创建它
//            H5::DataSpace visit_counts_space(2, visit_counts_dims);
//            visit_counts_dataset = group.createDataSet(visit_counts_dataset_name, H5::PredType::NATIVE_INT, visit_counts_space);
//        }
//        // 假设visit_counts是一个std::vector<std::vector<int>>，每个内部vector都是长度为N的稀疏向量
//        std::vector<int> flat_visit_counts_data;
//        for (const auto& vec : visit_counts) {
//            flat_visit_counts_data.insert(flat_visit_counts_data.end(), vec.begin(), vec.end());
//        }
//
//        visit_counts_dataset.write(flat_visit_counts_data.data(), H5::PredType::NATIVE_INT);
//
//        // 序列化rewards
//        hsize_t rewards_dims[1] = { rewards.size() };
//        /*H5::DataSpace rewards_space(1, rewards_dims);
//        H5::DataSet rewards_dataset = group.createDataSet("/experience/rewards", H5::PredType::NATIVE_INT, rewards_space);*/
//        std::string rewards_dataset_name = "/experience/rewards";
//        H5::DataSet rewards_dataset;
//
//        // 检查rewards数据集是否存在
//        if (H5Lexists(group.getId(), rewards_dataset_name.c_str(), H5P_DEFAULT)) {
//            // 数据集已存在，直接打开
//            rewards_dataset = group.openDataSet("/experience/rewards");
//        }
//        else {
//            // 数据集不存在，创建它
//            H5::DataSpace rewards_space(1, rewards_dims);
//            rewards_dataset = group.createDataSet(rewards_dataset_name, H5::PredType::NATIVE_INT, rewards_space);
//        }
//
//        rewards_dataset.write(rewards.data(), H5::PredType::NATIVE_FLOAT);
//    }
//    catch (const H5::Exception& e) {
//        std::cerr << "HDF5 Exception: " << e.getCDetailMsg() << std::endl;
//    }
//    catch (const std::exception& e) {
//        std::cerr << "Exception: " << e.what() << std::endl;
//    }
//    catch (...) {
//        std::cerr << "An unknown error occurred." << std::endl;
//    }
//}

//void ZeroExperienceCollector::serialize(const std::string& filename) const {
//    try {
//        H5::H5File file;
//        std::ifstream f("test3.h5");
//
//        if (f.good()) {
//            // 文件存在，以读写模式打开
//            file = H5::H5File("test3.h5", H5F_ACC_RDWR);
//        }
//        else {
//            // 文件不存在，创建新文件
//            file = H5::H5File("test3.h5", H5F_ACC_TRUNC);
//        }
//        /*H5::H5File file("test3.h5", H5F_ACC_RDWR);*/
//
//        // 对于states，visit_counts和rewards，我们需要创建HDF5数据集
//        // 假设states是一个tensor列表，每个tensor的数据类型和形状可能不同
//        // 这里仅示例如何将一个固定类型和形状的tensor保存到HDF5
//
//        // 首先，我们创建一个组
//        /*H5::Group group = file.createGroup("/experience");*/
//        H5::Group group;
//        if (H5Lexists(file.getId(), "/experience", H5P_DEFAULT)) {
//            group = file.openGroup("/experience");
//        }
//        else {
//            group = file.createGroup("/experience");
//        }
//
//        // 序列化states
//
//        hsize_t states_dims[4] = { states.size(), 3 /* C */, 14 /* H */, 14 /* W */ };
//        std::string states_dataset_name = "/experience/states";
//        H5::DataSet states_dataset;
//
//        hsize_t max_dims[4] = { H5S_UNLIMITED, 3 /*C*/, 14 /*H*/, 14 /*W*/ };
//        hsize_t chunk_dims[4] = { 1, 3 /*C*/, 14 /*H*/, 14 /*W*/ }; // 定义chunk大小，这里假设一次添加一个state
//        H5::DSetCreatPropList plist;
//        plist.setChunk(4, chunk_dims); // 设置数据集的分块
//        plist.setFillValue(H5::PredType::NATIVE_FLOAT, &fillvalue); // 可选：设置填充值
//
//        // 检查states数据集是否存在
//        std::string states_dataset_name = "/experience/states";
//        if (H5Lexists(group.getId(), states_dataset_name.c_str(), H5P_DEFAULT)) {
//            // 数据集已存在, 直接打开
//            states_dataset = group.openDataSet(states_dataset_name);
//        }
//        else {
//            // 数据集不存在, 创建它
//            H5::DataSpace states_space(4, states_dims, max_dims); // 使用max_dims使其可扩展
//            states_dataset = group.createDataSet(states_dataset_name, H5::PredType::NATIVE_FLOAT, states_space, plist);
//        }
//
//        // 假设我们现在要添加新的states数据
//        // 首先, 获取当前数据集的大小
//        hsize_t current_dims[4];
//        H5::DataSpace space = states_dataset.getSpace();
//        space.getSimpleExtentDims(current_dims, NULL);
//
//        // 计算新的大小，这里假设添加states.size()个新状态
//        hsize_t new_dims[4] = { current_dims[0] + states.size(), 3 /*C*/, 14 /*H*/, 14 /*W*/ };
//
//        // 设置数据集的新大小
//        states_dataset.extend(new_dims);
//
//        // 定义要写入新数据的hyperslab
//        H5::DataSpace filespace = states_dataset.getSpace();
//        hsize_t start[4] = { current_dims[0], 0, 0, 0 }; // 从当前数据集的末尾开始
//        hsize_t stride[4] = { 1, 1, 1, 1 };
//        hsize_t count[4] = { 1, 1, 1, 1 }; // 每次写入一个数据块
//        hsize_t block[4] = { states.size(), 3, 14, 14 }; // 新数据块的大小
//        filespace.selectHyperslab(H5S_SELECT_SET, count, start, stride, block);
//
//        // 定义内存中的数据布局
//        H5::DataSpace memspace(4, block, NULL); // 新数据的大小
//
//        // 写入数据
//        states_dataset.write(flat_states_data.data(), H5::PredType::NATIVE_FLOAT, memspace, filespace);
//
//
//        // 检查states数据集是否存在
//        if (H5Lexists(group.getId(), states_dataset_name.c_str(), H5P_DEFAULT)) {
//            // 数据集已存在，直接打开
//            states_dataset = group.openDataSet("/experience/states");
//        }
//        else {
//            // 数据集不存在，创建它
//            H5::DataSpace states_space(4, states_dims);
//            states_dataset = group.createDataSet(states_dataset_name, H5::PredType::NATIVE_FLOAT, states_space);
//        }
//        /* H5::DataSpace states_space(4, states_dims);
//         H5::DataSet states_dataset = group.createDataSet("/experience/states", H5::PredType::NATIVE_FLOAT, states_space);*/
//         // 将所有states的数据拷贝到一个连续的内存块中
//        std::vector<float> flat_states_data(states.size() * 3 * 14 * 14);
//        for (size_t i = 0; i < states.size(); ++i) {
//            torch::Tensor state = states[i].contiguous(); // 确保连续内存
//            const float* data = state.data_ptr<float>();
//            std::copy(data, data + (3 * 14 * 14), flat_states_data.begin() + i * (3 * 14 * 14));
//        }
//        states_dataset.write(flat_states_data.data(), H5::PredType::NATIVE_FLOAT);
//
//        // 序列化visit_counts
//        hsize_t visit_counts_dims[2] = { visit_counts.size(), 196 }; // 第一维是向量的数量，第二维是每个向量的长度
//        /*H5::DataSpace visit_counts_space(2, visit_counts_dims);
//        H5::DataSet visit_counts_dataset = group.createDataSet("/experience/visit_counts", H5::PredType::NATIVE_INT, visit_counts_space);*/
//        std::string visit_counts_dataset_name = "/experience/visit_counts";
//        H5::DataSet visit_counts_dataset;
//        if (H5Lexists(group.getId(), visit_counts_dataset_name.c_str(), H5P_DEFAULT)) {
//            // 数据集已存在，直接打开
//            visit_counts_dataset = group.openDataSet("/experience/visit_counts");
//        }
//        else {
//            // 数据集不存在，创建它
//            H5::DataSpace visit_counts_space(2, visit_counts_dims);
//            visit_counts_dataset = group.createDataSet(visit_counts_dataset_name, H5::PredType::NATIVE_INT, visit_counts_space);
//        }
//        // 假设visit_counts是一个std::vector<std::vector<int>>，每个内部vector都是长度为N的稀疏向量
//        std::vector<int> flat_visit_counts_data;
//        for (const auto& vec : visit_counts) {
//            flat_visit_counts_data.insert(flat_visit_counts_data.end(), vec.begin(), vec.end());
//        }
//
//        visit_counts_dataset.write(flat_visit_counts_data.data(), H5::PredType::NATIVE_INT);
//
//        // 序列化rewards
//        hsize_t rewards_dims[1] = { rewards.size() };
//        /*H5::DataSpace rewards_space(1, rewards_dims);
//        H5::DataSet rewards_dataset = group.createDataSet("/experience/rewards", H5::PredType::NATIVE_INT, rewards_space);*/
//        std::string rewards_dataset_name = "/experience/rewards";
//        H5::DataSet rewards_dataset;
//
//        // 检查rewards数据集是否存在
//        if (H5Lexists(group.getId(), rewards_dataset_name.c_str(), H5P_DEFAULT)) {
//            // 数据集已存在，直接打开
//            rewards_dataset = group.openDataSet("/experience/rewards");
//        }
//        else {
//            // 数据集不存在，创建它
//            H5::DataSpace rewards_space(1, rewards_dims);
//            rewards_dataset = group.createDataSet(rewards_dataset_name, H5::PredType::NATIVE_INT, rewards_space);
//        }
//
//        rewards_dataset.write(rewards.data(), H5::PredType::NATIVE_FLOAT);
//    }
//    catch (const H5::Exception& e) {
//        std::cerr << "HDF5 Exception: " << e.getCDetailMsg() << std::endl;
//    }
//    catch (const std::exception& e) {
//        std::cerr << "Exception: " << e.what() << std::endl;
//    }
//    catch (...) {
//        std::cerr << "An unknown error occurred." << std::endl;
//    }
//}

ZeroExperienceBuffer ZeroExperienceBuffer::combine_experience(std::vector<ZeroExperienceCollector>& collectors) {
    std::vector<torch::Tensor> combined_states;
    std::vector<std::vector<int>> combined_visit_counts;
    std::vector<int> combined_rewards;

    for (const auto& collector : collectors) {
        combined_states.insert(combined_states.end(), collector.states.begin(), collector.states.end());
        combined_visit_counts.insert(combined_visit_counts.end(), collector.visit_counts.begin(), collector.visit_counts.end());
        combined_rewards.insert(combined_rewards.end(), collector.rewards.begin(), collector.rewards.end());

    }
    return ZeroExperienceBuffer(combined_states, combined_visit_counts, combined_rewards);
}

//void ZeroExperienceBuffer::serialize() const {
//    H5::H5File file("test5.h5", H5F_ACC_TRUNC);
//
//    // 首先，我们创建一个组
//    H5::Group group = file.createGroup("/experience");
//
//    // 序列化states
//    hsize_t states_dims[4] = { states.size(), 3 /* C */, 14 /* H */, 14 /* W */ };
//    H5::DataSpace states_space(4, states_dims);
//    H5::DataSet states_dataset = group.createDataSet("/experience/states", H5::PredType::NATIVE_FLOAT, states_space);
//    /*std::cout << states.size() << std::endl;*/
//    // 将所有states的数据拷贝到一个连续的内存块中
//    std::vector<float> flat_states_data(states.size() * 3 * 14 * 14);
//    for (size_t i = 0; i < states.size(); ++i) {
//        torch::Tensor state = states[i].contiguous(); // 确保连续内存
//        const float* data = state.data_ptr<float>();
//        std::copy(data, data + (3 * 14 * 14), flat_states_data.begin() + i * (3 * 14 * 14));
//    }
//    states_dataset.write(flat_states_data.data(), H5::PredType::NATIVE_FLOAT);
//
//    // 序列化visit_counts
//    hsize_t visit_counts_dims[2] = { visit_counts.size(), 196 }; // 第一维是向量的数量，第二维是每个向量的长度
//    H5::DataSpace visit_counts_space(2, visit_counts_dims);
//    H5::DataSet visit_counts_dataset = group.createDataSet("/experience/visit_counts", H5::PredType::NATIVE_INT, visit_counts_space);
//
//    // 假设visit_counts是一个std::vector<std::vector<int>>，每个内部vector都是长度为N的稀疏向量
//    std::vector<int> flat_visit_counts_data;
//    for (const auto& vec : visit_counts) {
//        flat_visit_counts_data.insert(flat_visit_counts_data.end(), vec.begin(), vec.end());
//    }
//
//    visit_counts_dataset.write(flat_visit_counts_data.data(), H5::PredType::NATIVE_INT);
//
//    // 序列化rewards
//    hsize_t rewards_dims[1] = { rewards.size() };
//    H5::DataSpace rewards_space(1, rewards_dims);
//    H5::DataSet rewards_dataset = group.createDataSet("/experience/rewards", H5::PredType::NATIVE_INT, rewards_space);
//    rewards_dataset.write(rewards.data(), H5::PredType::NATIVE_INT);
//}
//ZeroExperienceBuffer load_experience(const H5std_string& filename) {
//    // Load experience implementation
//    // This function needs to be implemented based on how you design serialization
//}

void ZeroExperienceBuffer::serialize() const {
    H5::H5File file;
    H5::Group group;

    // 检查文件是否存在
    std::string filename = "I:\\jiuqi\\dataset\\layout\\layout.h5";
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

        // 创建visit_counts数据集
        hsize_t visit_counts_dims[2] = { 0, 196 };
        hsize_t visit_counts_max_dims[2] = { H5S_UNLIMITED, 196 };
        H5::DataSpace visit_counts_space(2, visit_counts_dims, visit_counts_max_dims);
        H5::DSetCreatPropList visit_counts_prop;
        hsize_t visit_counts_chunk_dims[2] = { 1, 196 };
        visit_counts_prop.setChunk(2, visit_counts_chunk_dims);
        group.createDataSet("/experience/visit_counts", H5::PredType::NATIVE_INT, visit_counts_space, visit_counts_prop);

        // 创建rewards数据集
        hsize_t rewards_dims[1] = { 0 };
        hsize_t rewards_max_dims[1] = { H5S_UNLIMITED };
        H5::DataSpace rewards_space(1, rewards_dims, rewards_max_dims);
        H5::DSetCreatPropList rewards_prop;
        hsize_t rewards_chunk_dims[1] = { 1 };
        rewards_prop.setChunk(1, rewards_chunk_dims);
        group.createDataSet("/experience/rewards", H5::PredType::NATIVE_INT, rewards_space, rewards_prop);
    }



    // 获取现有的数据集
    H5::DataSet states_dataset = group.openDataSet("/experience/states");
    H5::DataSet visit_counts_dataset = group.openDataSet("/experience/visit_counts");
    H5::DataSet rewards_dataset = group.openDataSet("/experience/rewards");

    // 扩展 states 数据集的大小
    hsize_t states_current_dims[4];
    states_dataset.getSpace().getSimpleExtentDims(states_current_dims);
    states_current_dims[0] += states.size();
    states_dataset.extend(states_current_dims);

    // 扩展 visit_counts 数据集的大小
    hsize_t visit_counts_current_dims[2];
    visit_counts_dataset.getSpace().getSimpleExtentDims(visit_counts_current_dims);
    visit_counts_current_dims[0] += visit_counts.size();
    visit_counts_dataset.extend(visit_counts_current_dims);

    // 扩展 rewards 数据集的大小
    hsize_t rewards_current_dims[1];
    rewards_dataset.getSpace().getSimpleExtentDims(rewards_current_dims);
    rewards_current_dims[0] += rewards.size();
    rewards_dataset.extend(rewards_current_dims);

    // 为 states 数据选择要写入的超平面
    hsize_t states_offset[4] = { states_current_dims[0] - states.size(), 0, 0, 0 };
    hsize_t states_count[4] = { states.size(), 3, 14, 14 };
    H5::DataSpace states_file_space = states_dataset.getSpace();
    states_file_space.selectHyperslab(H5S_SELECT_SET, states_count, states_offset);
    H5::DataSpace states_mem_space(4, states_count); // 源数据空间

    // 为 visit_counts 数据选择要写入的超平面
    hsize_t visit_counts_offset[2] = { visit_counts_current_dims[0] - visit_counts.size(), 0 };
    hsize_t visit_counts_count[2] = { visit_counts.size(), 196 };
    H5::DataSpace visit_counts_file_space = visit_counts_dataset.getSpace();
    visit_counts_file_space.selectHyperslab(H5S_SELECT_SET, visit_counts_count, visit_counts_offset);
    H5::DataSpace visit_counts_mem_space(2, visit_counts_count); // 源数据空间

    // 为 rewards 数据选择要写入的超平面
    hsize_t rewards_offset[1] = { rewards_current_dims[0] - rewards.size() };
    hsize_t rewards_count[1] = { rewards.size() };
    H5::DataSpace rewards_file_space = rewards_dataset.getSpace();
    rewards_file_space.selectHyperslab(H5S_SELECT_SET, rewards_count, rewards_offset);
    H5::DataSpace rewards_mem_space(1, rewards_count); // 源数据空间

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

    std::vector<int> flat_visit_counts_data;
    for (const auto& vec : visit_counts) {
        flat_visit_counts_data.insert(flat_visit_counts_data.end(), vec.begin(), vec.end());
    }

    visit_counts_dataset.write(flat_visit_counts_data.data(), H5::PredType::NATIVE_INT, visit_counts_mem_space, visit_counts_file_space);


    // 将 visit_counts 数据写入文件
    //visit_counts_dataset.write(visit_counts.data(), H5::PredType::NATIVE_INT, visit_counts_mem_space, visit_counts_file_space);

    // 将 rewards 数据写入文件
    rewards_dataset.write(rewards.data(), H5::PredType::NATIVE_INT, rewards_mem_space, rewards_file_space);
    // 关闭数据集
    states_dataset.close();
    visit_counts_dataset.close();
    rewards_dataset.close();

    // 关闭组
    group.close();

    // 关闭文件
    file.close();
}
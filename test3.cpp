//#include <iostream>
//#include <memory>
//#include "gotypes.h"
//#include<vector>
//#include "goboard.h"
//#include "random.h"
//#include <fstream>
//#include "ThreadPool.h"
//#include "LayoutEncoder.h"
//#include "ParallelTreeNode.h"
//
//void printMatrix(const std::vector<std::vector<int>>& datas) {
//    for (int i = 0; i < 5; ++i) {
//      
//        const std::vector<int>&matrix = datas[i];
//
//        for (int row = 0; row < 14; ++row) {
//            for (int col = 0; col < 14; ++col) {
//                std::cout << matrix[row * 14 + col] << " ";
//            }
//            std::cout << std::endl;
//        }
//        std::cout << std::endl;  // 在矩阵之间添加额外的空行以便区分
//    }
//}
//std::vector<std::vector<int>> datas() {
//
//     打开HDF5文件
//    H5::H5File file("E:/data/data/exp_huadong_v1_6574_enhance.h5", H5F_ACC_RDONLY);
//    std::cout << "打开成功 " << std::endl;
//     打开数据集
//    H5::DataSet dataset = file.openDataSet("/experience/states");
//
//     获取数据空间
//    H5::DataSpace states_space = dataset.getSpace();
//
//     获取数据集的维度大小
//    hsize_t dims_out[4];
//    states_space.getSimpleExtentDims(dims_out, NULL);
//    std::cout << "数据集维度: ";
//    for (int i = 0; i < 4; i++) {
//        std::cout << dims_out[i] << " ";
//    }
//    std::cout << std::endl;
//
//     读取的步长
//    const hsize_t stride = 194;
//    const int total_blocks = dims_out[0] / stride;
//
//     定义hyperslab的选择参数
//    hsize_t offset[4] = { 193, 0, 0, 0 };
//    hsize_t count[4] = { 1, 1, 14, 14 };
//    hsize_t stride_param[4] = { stride, 1, 1, 1 };
//    hsize_t block[4] = { 1, 1, 1, 1 };
//    std::vector<std::vector<int>> datas;
//    std::vector<int> data(14 * 14); // 用于存储单个14x14数据块
//
//    for (int i = 0; i < total_blocks - 1; i++) {
//         设置偏移量
//        offset[0] = 193 + i * stride;
//
//         选择数据空间的一个超块
//        states_space.selectHyperslab(H5S_SELECT_SET, count, offset, stride_param, block);
//
//         定义内存中的数据空间
//        H5::DataSpace mem_space(4, count);
//
//         读取数据
//        dataset.read(data.data(), H5::PredType::NATIVE_INT, mem_space, states_space);
//        std::transform(data.begin(), data.end(), data.begin(), [](int val) {
//            if (val == -1) {
//                return 2;
//            }
//            else if (val == 0) {
//                return 1;
//            }
//            else {
//                return val;
//            }
//            });
//        datas.push_back(data);
//    }
//    /*printMatrix(datas);*/
//    return datas;
//}
//
//int main() {
//    std::ofstream logFile("logfile.txt");
//    std::streambuf* prevCoutBuf = std::cout.rdbuf();  // 保存旧的buf
//    std::cout.rdbuf(logFile.rdbuf());  // 重定向到logfile.txt
//
//    std::vector<std::vector<int>> data = datas();
//    std::cout << data.size() << std::endl;
//    int black_win_couts = 0;
//    int white_win_couts = 0;
//    MoveExperienceCollector blackcollector;
//    MoveExperienceCollector whitecollector;
//
//    auto model_path = "E:\\data\\model\\uppernet_v1_epoch_200.pt";
//    torch::jit::script::Module model;
//    torch::Device device(torch::kCUDA, 0);
//    model = torch::jit::load(model_path);
//    model.to(at::kCUDA);
//    
//    std::map<Player, std::unique_ptr<Bot>> bots;
//    auto encoder = std::make_shared<MoveEncoder>(std::make_pair(14, 14));
//     创建新的ExpertBot1对象并使用unique_ptr管理
//    bots[Player::WHITE] = std::make_unique<ExpertBot4>(&whitecollector,encoder);
//    bots[Player::BLACK] = std::make_unique<ExpertBot4>(&blackcollector,encoder);
//
//    /*int t = 0;*/
//    for (int i = 2377; i < data.size(); i++) {
//        auto start_time = std::chrono::high_resolution_clock::now();
//        std::cout << "第" << i + 1 << "局"<<std::endl;
//        Board board(14, 14);
//        board.grid = data[i];
//        board.grid[90] = 0;
//        board.grid[105] = 0;
//        GameState game_state(board,Player::BLACK,nullptr,2,195);
//        game_state.board.displayGrid();
//        while (!game_state.is_over())
//        /*while (t<3)*/
//        {
//            auto start_time1 = std::chrono::high_resolution_clock::now();
//            Player current_player = game_state.next_player;
//            auto [moves,eat_moves] = bots[current_player]->select_move(game_state);
//            if (moves.empty()) {
//                std::cout << current_player <<"游戏结束，没有合法的移动。" << std::endl;
//                game_state.winner = other(current_player);
//                break; 
//            }
//            auto state_tensor = encoder->encode(game_state);
//            torch::Device device(torch::kCUDA, 0);
//            state_tensor = state_tensor.to(device);
//            auto result = model.forward({ state_tensor }).toTuple()->elements();
//            torch::Tensor policy_output = result[0].toTensor().to(at::kCPU);
//            auto flat_policy_output = policy_output.view(-1);
//            auto sorted = flat_policy_output.sort(0, /* descending= */ true);
//            auto sorted_indices = std::get<1>(sorted);
//            std::cout << policy_output << std::endl;
//            std::cout << "Sorted values: " << sorted_indices<< std::endl;
//             遍历排序后的索引，查找第一个合法的动作
//            int chosen_index = -1;
//            for (int i = 0; i < sorted_indices.size(0); ++i) {
//                int index = sorted_indices[i].item<int>();
//                std::vector<int> move_predict = encoder->decode_move_index(index);
//                if (game_state.board.is_valid_move_move(game_state.next_player,move_predict)) {
//                    std::cout << "wangluo   " << i+1 << std::endl;
//                    game_state.board.Print_move(move_predict, current_player);
//                    break;
//                }
//            }
//            
//            /*std::cout << "主函数" << collector._current_episode_states.size() << std::endl;*/
//            ExpertBot1* blackBot = static_cast<ExpertBot1*>(bots[Player::BLACK]);
//            ZeroExperienceCollector& blackCollector = blackBot->getCollector();
//            /*std::cout << "函数" << blackCollector._current_episode_states.size() << std::endl;*/
//
//            auto end_time1 = std::chrono::high_resolution_clock::now();
//            auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end_time1 - start_time1).count();
//            double seconds1 = static_cast<double>(duration1) / 1000000.0;
//            std::cout << "Inference time: " << seconds1 << " s  ";
//            
//            game_state.board.Print_move(moves, current_player);
//            std::tuple<std::vector<int>, std::vector<int>> move = std::make_tuple(moves, eat_moves);
//            game_state = game_state.apply_move(move);
//            game_state.board.displayGrid(); // 显示棋盘的当前状态  
//            std::cout<<game_state.step<<std::endl;
//        }
//        
//        ExpertBot4* blackBot = static_cast<ExpertBot4*>(bots[Player::BLACK].get());
//        MoveExperienceCollector* blackCollector = blackBot->move_collector;
//        ExpertBot4* whiteBot = static_cast<ExpertBot4*>(bots[Player::WHITE].get());
//        MoveExperienceCollector* whiteCollector = whiteBot->move_collector;
//        if (game_state.winner == BLACK) {
//            blackCollector->complete_episode(1);
//            whiteCollector->complete_episode(-1);
//            black_win_couts += 1;
//        }
//        else
//        {
//            blackCollector->complete_episode(-1);
//            whiteCollector->complete_episode(1);
//            white_win_couts += 1;
//        }
//        std::cout << blackCollector->states << std::endl;
//        std::cout << blackCollector->move_actions << std::endl;
//        std::cout << blackCollector->eat_actions << std::endl;
//        std::cout << blackCollector->rewards << std::endl;
//        std::vector<MoveExperienceCollector> coll;
//        coll.reserve(2);
//        coll.emplace_back(*whiteCollector);
//        coll.emplace_back(*blackCollector);
//        /*coll.clear();*/
///*        MoveExperienceBuffer buffer;
//        buffer = buffer.combine_experience(coll);
//        buffer.serialize();*/ 
//        blackCollector->clear_collector();
//        whiteCollector->clear_collector();
//        auto end_time = std::chrono::high_resolution_clock::now();
//        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
//        double seconds = static_cast<double>(duration) / 1000000.0;
//        std::cout << "Inference time: " << seconds << " s  "<<std::endl;
//        std::cout << black_win_couts << "   " << white_win_couts << std::endl;
//    }
//    
//    /*buffer.serialize();*/
//    /*std::cout<< buffer.rewards[1] << std::endl;*/
//    
//    std::cout.rdbuf(prevCoutBuf);  // 恢复旧的buf，输出回控制台
//    logFile.close();  // 关闭日志文件
//}
//
//
//

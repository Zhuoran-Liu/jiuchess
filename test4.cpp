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
//    // 打开HDF5文件
//    H5::H5File file("E:/data/data/exp_huadong_v1_6574_enhance.h5", H5F_ACC_RDONLY);
//    std::cout << "打开成功 " << std::endl;
//    // 打开数据集
//    H5::DataSet dataset = file.openDataSet("/experience/states");
//
//    // 获取数据空间
//    H5::DataSpace states_space = dataset.getSpace();
//
//    // 获取数据集的维度大小
//    hsize_t dims_out[4];
//    states_space.getSimpleExtentDims(dims_out, NULL);
//    std::cout << "数据集维度: ";
//    for (int i = 0; i < 4; i++) {
//        std::cout << dims_out[i] << " ";
//    }
//    std::cout << std::endl;
//
//    // 读取的步长
//    const hsize_t stride = 194;
//    const int total_blocks = dims_out[0] / stride;
//
//    // 定义hyperslab的选择参数
//    hsize_t offset[4] = { 193, 0, 0, 0 };
//    hsize_t count[4] = { 1, 1, 14, 14 };
//    hsize_t stride_param[4] = { stride, 1, 1, 1 };
//    hsize_t block[4] = { 1, 1, 1, 1 };
//    std::vector<std::vector<int>> datas;
//    std::vector<int> data(14 * 14); // 用于存储单个14x14数据块
//
//    for (int i = 0; i < total_blocks - 1; i++) {
//        // 设置偏移量
//        offset[0] = 193 + i * stride;
//
//        // 选择数据空间的一个超块
//        states_space.selectHyperslab(H5S_SELECT_SET, count, offset, stride_param, block);
//
//        // 定义内存中的数据空间
//        H5::DataSpace mem_space(4, count);
//
//        // 读取数据
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
//    std::map<Player, std::unique_ptr<Bot1>> bots;
//    auto encoder = std::make_shared<MoveEncoder>(std::make_pair(14, 14));
//    // 创建新的ExpertBot1对象并使用unique_ptr管理
//    bots[Player::WHITE] = std::make_unique<ExpertBot3>();
//    bots[Player::BLACK] = std::make_unique<ExpertBot2>();
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
//            auto moves = bots[current_player]->select_move(game_state);
//            if (moves.empty()) {
//                std::cout << current_player <<"游戏结束，没有合法的移动。" << std::endl;
//                game_state.winner = other(current_player);
//                break; 
//            }
//            /*std::cout << "主函数" << collector._current_episode_states.size() << std::endl;*/
//            //ExpertBot1* blackBot = static_cast<ExpertBot1*>(bots[Player::BLACK]);
//            //ZeroExperienceCollector& blackCollector = blackBot->getCollector();
//            /*std::cout << "函数" << blackCollector._current_episode_states.size() << std::endl;*/
//
//            auto end_time1 = std::chrono::high_resolution_clock::now();
//            auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end_time1 - start_time1).count();
//            double seconds1 = static_cast<double>(duration1) / 1000000.0;
//            std::cout << "Inference time: " << seconds1 << " s  ";
//            
//            game_state.board.Print_move(moves, current_player);
//            
//            game_state = game_state.all_apply_move(moves);
//            game_state.board.displayGrid(); // 显示棋盘的当前状态  
//            std::cout<<game_state.step<<std::endl;
//        }
//        
//       
//        auto end_time = std::chrono::high_resolution_clock::now();
//        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
//        double seconds = static_cast<double>(duration) / 1000000.0;
//        std::cout << "Inference time: " << seconds << " s  "<<std::endl;
//        std::cout << black_win_couts << "   " << white_win_couts << std::endl;
//    }
//    
//
//    
//    std::cout.rdbuf(prevCoutBuf);  // 恢复旧的buf，输出回控制台
//    logFile.close();  // 关闭日志文件
//}
//
//
//

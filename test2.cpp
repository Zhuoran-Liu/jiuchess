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
//
//int main() {
//    std::ofstream logFile("logfile.txt");
//    std::streambuf* prevCoutBuf = std::cout.rdbuf();  // 保存旧的buf
//    std::cout.rdbuf(logFile.rdbuf());  // 重定向到logfile.txt
//    auto model_path = "E:\\data\\model\\resnet50_v1_1119.pt";
//    auto model_path1 = "D:\\pytorch_gpu\\chess_model_jit1.pt";
//    Net net(model_path,16);
//    Net net1(model_path1, 16);
//    int black_win_couts = 0;
//    int white_win_couts = 0;
//    ZeroExperienceCollector blackcollector;
//    ZeroExperienceCollector whitecollector;
//    /*ZeroExperienceBuffer buffer;*/
//    //std::map<Player, Bot*> bots;
//    //bots[Player::WHITE] = new ExpertBot1(model_path1, 800, 2.0f, whitecollector);
//    //bots[Player::BLACK] = new ExpertBot1(model_path1, 800, 2.0f, blackcollector);
//    std::map<Player, std::unique_ptr<Bot>> bots;
//
//    // 创建新的ExpertBot1对象并使用unique_ptr管理
//    bots[Player::WHITE] = std::make_unique<ExpertBot1>(&net1, 500, 0.03f, whitecollector);
//    bots[Player::BLACK] = std::make_unique<ExpertBot1>(&net, 500, 0.03f, blackcollector);
//    /*int t = 0;*/
//    for (int i = 0; i < 300; i++) {
//        auto start_time = std::chrono::high_resolution_clock::now();
//        std::cout << "第" << i + 1 << "局" << std::endl;
//        GameState game_state = GameState::new_game(14);
//        game_state.board.displayGrid();
//        while (!game_state.is_over())
//            /*while (t<3)*/
//        {
//            auto start_time1 = std::chrono::high_resolution_clock::now();
//            Player current_player = game_state.next_player;
//            std::vector<int> moves = bots[current_player]->select_move(game_state);
//            if (moves.empty()) {
//                std::cout << current_player << "游戏结束，没有合法的移动。" << std::endl;
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
//            game_state = game_state.go_apply_move(moves);
//            game_state.game_state(game_state.board);
//            game_state.board.displayGrid(); // 显示棋盘的当前状态  
//            std::cout << game_state.step << std::endl;
//        }
//        ExpertBot1* blackBot = static_cast<ExpertBot1*>(bots[Player::BLACK].get());
//        ZeroExperienceCollector& blackCollector = blackBot->getCollector();
//
//        ExpertBot1* whiteBot = static_cast<ExpertBot1*>(bots[Player::WHITE].get());
//        ZeroExperienceCollector& whiteCollector = whiteBot->getCollector();
//        if (game_state.winner == BLACK) {
//            blackCollector.complete_episode(1);
//            whiteCollector.complete_episode(-1);
//            black_win_couts += 1;
//        }
//        else
//        {
//            blackCollector.complete_episode(-1);
//            whiteCollector.complete_episode(1);
//            white_win_couts += 1;
//        }
//
//        std::vector<ZeroExperienceCollector> coll;
//        coll.reserve(2);
//        coll.emplace_back(whiteCollector);
//        coll.emplace_back(blackCollector);
//        /*coll.clear();*/
//        blackCollector.clear_experience();
//        whiteCollector.clear_experience();
//        /*ZeroExperienceBuffer buffer;
//        buffer = buffer.combine_experience(coll);
//        buffer.serialize();*/
//        auto end_time = std::chrono::high_resolution_clock::now();
//        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
//        double seconds = static_cast<double>(duration) / 1000000.0;
//        std::cout << "Inference time: " << seconds << " s  " << std::endl;
//        std::cout << black_win_couts << "   " << white_win_couts << std::endl;
//        //if (i == 50) {
//        //    // 在这里添加需要在重启前执行的代码，例如清理资源等
//
//        //    // 使用system调用来运行一个批处理脚本，该脚本将重启Visual Studio并运行项目
//        //    system("E:\\run1.bat");
//        //    return 0; // 结束当前程序的执行
//        //}
//    }
//
//    /*buffer.serialize();*/
//    /*std::cout<< buffer.rewards[1] << std::endl;*/
//
//    std::cout.rdbuf(prevCoutBuf);  // 恢复旧的buf，输出回控制台
//    logFile.close();  // 关闭日志文件
//}
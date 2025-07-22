//#include <fstream>
//#include <sstream>
//#include <vector>
//#include <string>
//#include <iostream>
//
//#include <memory>
//#include "gotypes.h"
//#include<vector>
//#include "goboard.h"
//#include "random.h"
//#include <fstream>
//#include "ThreadPool.h"
//#include "MoveEncoder.h"
//#include "MoveAgent.h"
//int main() {
//    std::ofstream logFile("logfile.txt");
//    std::streambuf* prevCoutBuf = std::cout.rdbuf();  // 保存旧的buf
//    std::cout.rdbuf(logFile.rdbuf());  // 重定向到logfile.txt
//   
//    int black_win_couts = 0;
//    int white_win_couts = 0;
//    int i = 0;
//    std::map<Player, std::unique_ptr<Bot>> bots;
//    bots[Player::WHITE] = std::make_unique<bisaiBot>();
//    /*bots[Player::WHITE] = std::make_unique<ExpertBot4>();*/
//    bots[Player::BLACK] = std::make_unique<bisaiBot>();
//    while (i<15) {
//        auto start_time = std::chrono::high_resolution_clock::now();
//        std::cout << "第" << i + 1 << "局" << std::endl;
//        GameState game_state = GameState::new_game(14);
//        game_state.board.displayGrid();
//        while (!game_state.is_over())
//            /*while (game_state.state == 1)*/
//            /*while (t<3)*/
//        {
//            auto start_time1 = std::chrono::high_resolution_clock::now();
//            Player current_player = game_state.next_player;
//            auto [moves, eat_moves] = bots[current_player]->select_move(game_state);
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
//            game_state.board.Print_move(moves, eat_moves, current_player);
//            std::tuple<std::vector<int>, std::vector<int>> move = std::make_tuple(moves, eat_moves);
//            game_state = game_state.fly_apply_move(move);
//            game_state.game_state(game_state.board);
//            game_state.board.displayGrid(); // 显示棋盘的当前状态  
//            std::cout << game_state.step << std::endl;
//        }
//
//        if (game_state.winner == BLACK) {
//
//            black_win_couts += 1;
//        }
//        else
//        {
//
//            white_win_couts += 1;
//        }
//        
//        auto end_time = std::chrono::high_resolution_clock::now();
//        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
//        double seconds = static_cast<double>(duration) / 1000000.0;
//        std::cout << "Inference time: " << seconds << " s  " << std::endl;
//        std::cout << black_win_couts << "   " << white_win_couts << std::endl;
//
//    }
//    std::cout.rdbuf(prevCoutBuf);  // 恢复旧的buf，输出回控制台
//    logFile.close();  // 关闭日志文件
//}
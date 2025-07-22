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
//    std::streambuf* prevCoutBuf = std::cout.rdbuf();  // ����ɵ�buf
//    std::cout.rdbuf(logFile.rdbuf());  // �ض���logfile.txt
//    std::vector<int> board_grid = { 1, 2, 1, 1, 2, 2, 2, 1, 2, 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 2, 2, 2, 1, 2, 2, 1, 2, 2, 1,
//        1, 1, 2, 1, 1, 2, 2, 1, 1, 1, 1, 2, 2, 2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 2
//        , 1, 2, 1, 1, 2, 2, 1, 2, 2, 2, 0, 0, 0, 1, 1, 2, 1, 1, 1, 1, 2, 2, 2, 1, 0, 0, 2, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//        1, 1, 1, 0, 2, 1, 1, 1, 2, 2, 2, 1, 2, 1, 1, 2, 2, 2, 0, 1, 0, 1, 1, 2, 1, 1, 2, 2, 1, 1, 1, 2, 1, 2, 1, 1, 2, 
//        2, 2, 2, 2, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 2, 2, 1, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 1, 2, 2, 2,
//        2, 1, 2, 2, 1, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 1, 1 };
//    //std::vector<int> board_grid(196,0);
//    //board_grid[2] = 1;
//    //board_grid[3] = 1;
//    //board_grid[4] = 1;
//    //board_grid[5] = 1;
//    //board_grid[16] = 1;
//    //board_grid[17] = 1;
//    //board_grid[19] = 1;
//    //board_grid[2] = 1;
//    //board_grid[3] = 1;
//    //board_grid[4] = 1;
//    //board_grid[5] = 1;
//    //board_grid[16] = 1;
//    //board_grid[17] = 1;
//    //board_grid[19] = 1;
//    std::map<Player, std::unique_ptr<Bot>> bots;
//    
//    // �����µ�ExpertBot1����ʹ��unique_ptr����
//    bots[Player::WHITE] = std::make_unique<ExpertBot5>();
//    bots[Player::BLACK] = std::make_unique<ExpertBot5>();
//
//    /*int t = 0;*/
//    for (int i = 2377; i < 2378; i++) {
//        auto start_time = std::chrono::high_resolution_clock::now();
//        std::cout << "��" << i + 1 << "��" << std::endl;
//        Board board(14, 14);
//        board.grid = board_grid;
//        board.grid[131] = 0;
//        board.special_moves1(Player::BLACK);
//        GameState game_state(board, Player::BLACK, nullptr, 2, 200);
//        game_state.board.displayGrid();
//        //Player current_player = game_state.next_player;
//        //auto [moves, eat_moves] = bots[current_player]->select_move(game_state);
//        //std::tuple<std::vector<int>, std::vector<int>> move = std::make_tuple(moves, eat_moves);
//        //game_state = game_state.apply_move(move);
//        ////game_state.board.Print_move(moves, current_player);
//        ////game_state.board.displayGrid(); // ��ʾ���̵ĵ�ǰ״̬  
//        ////std::cout << game_state.step << std::endl;
//        ////while (!game_state.is_over())
//        ////    /*while (t<3)*/
//        ////{
//        ////    auto start_time1 = std::chrono::high_resolution_clock::now();
//        ////    Player current_player = game_state.next_player;
//        ////    auto moves = bots[current_player]->select_move(game_state);
//        ////    if (moves.empty()) {
//        ////        std::cout << current_player << "��Ϸ������û�кϷ����ƶ���" << std::endl;
//        ////        game_state.winner = other(current_player);
//        ////        break;
//        ////    }
//        ////    /*std::cout << "������" << collector._current_episode_states.size() << std::endl;*/
//        ////    //ExpertBot1* blackBot = static_cast<ExpertBot1*>(bots[Player::BLACK]);
//        ////    //ZeroExperienceCollector& blackCollector = blackBot->getCollector();
//        ////    /*std::cout << "����" << blackCollector._current_episode_states.size() << std::endl;*/
//
//        ////    auto end_time1 = std::chrono::high_resolution_clock::now();
//        ////    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end_time1 - start_time1).count();
//        ////    double seconds1 = static_cast<double>(duration1) / 1000000.0;
//        ////    std::cout << "Inference time: " << seconds1 << " s  ";
//
//        //game_state.board.Print_move(moves, current_player);
//        //game_state.board.Print_move(eat_moves, current_player);
//        //    game_state = game_state.all_apply_move(moves);
//        /*    game_state.board.displayGrid(); */// ��ʾ���̵ĵ�ǰ״̬  
//        //    std::cout << game_state.step << std::endl;
//        //}
//
//
//        auto end_time = std::chrono::high_resolution_clock::now();
//        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
//        double seconds = static_cast<double>(duration) / 1000000.0;
//        std::cout << "Inference time: " << seconds << " s  " << std::endl;
//        
//    }
//
//
//
//    std::cout.rdbuf(prevCoutBuf);  // �ָ��ɵ�buf������ؿ���̨
//    logFile.close();  // �ر���־�ļ�
//}
//
//
//

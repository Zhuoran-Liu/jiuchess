//#include <iostream>
//#include <memory>
//#include "gotypes.h"
//#include<vector>
//#include "goboard.h"
//#include "random.h"
//#include <fstream>
//#include "ThreadPool.h"
//#include "MoveEncoder.h"
//#include "MoveAgent.h"
//
//int main() {
//    std::ofstream logFile("logfile.txt");
//    std::streambuf* prevCoutBuf = std::cout.rdbuf();  // 保存旧的buf
//    std::cout.rdbuf(logFile.rdbuf());  // 重定向到logfile.txt
//    /*auto model_path = "E:\\data\\model\\zouzi\\uppernet_v1xu_epoch_20.pt";*/
//    //eat_v0o_epoch_10
//
//    int black_win_couts = 0;
//    int white_win_couts = 0;
//    auto layout_model_path = "E:\\data\\model\\buju\\v\\layoutnet_v3o_epoch_20.pt";
//    auto layout_model_path1 = "E:\\data\\model\\buju\\v\\layoutnet_v0_epoch_60.pt";
//    auto move_model_path = "E:\\data\\model\\zouzi\\uppernet_v2newxu_epoch_15.pt";
//    auto move_model_path1 = "E:\\data\\model\\zouzi\\uppernet_v0_epoch_60.pt";
//    auto eat_model_path = "E:\\data\\model\\eat\\lowernet_jian_epoch_20.pt";
//    auto eat_model_path1 = "E:\\data\\model\\eat\\lowernet_qlv1_epoch_10.pt";
//    Net net(layout_model_path, 16);
//    Net net1(layout_model_path1, 16);
//    Net net2(move_model_path, 16);
//    Net net3(move_model_path1, 16);
//
//    std::map<Player, std::unique_ptr<Bot>> bots;
//    bots[Player::WHITE] = std::make_unique<RLBot>(&net, &net2, eat_model_path, 900, 200, 0.1f);
//    /*bots[Player::WHITE] = std::make_unique<ExpertBot4>();*/
//    bots[Player::BLACK] = std::make_unique<ExpertBot>();
//    /*int t = 0;*/
//    for (int j = 0; j < 1; j++) {
//        std::vector<int> grid =  {
//    //1, 1, 2, 2, 1, 2, 1, 2, 2, 1, 2, 1, 1, 2,  // A
//    //1, 2, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2,  // B
//    //2, 2, 2, 1, 2, 1, 1, 2, 1, 1, 1, 2, 1, 2,  // C
//    //2, 2, 1, 1, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1,  // D
//    //1, 1, 2, 2, 1, 1, 2, 1, 2, 2, 1, 1, 1, 2,  // E
//    //1, 1, 1, 2, 2, 1, 1, 2, 1, 1, 1, 1, 2, 2,  // F
//    //2, 2, 2, 1, 0, 0, 1, 2, 1, 2, 2, 2, 2, 1,  // G
//    //2, 1, 1, 1, 2, 2, 2, 0, 1, 1, 2, 1, 1, 1,  // H
//    //1, 2, 1, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 2,  // I
//    //1, 2, 2, 1, 2, 2, 1, 2, 1, 2, 1, 1, 2, 1,  // J
//    //2, 2, 1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 2,  // K
//    //1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 2, 1, 2, 2,  // L
//    //1, 1, 1, 2, 2, 1, 2, 1, 2, 1, 1, 1, 1, 2,  // M
//    //2, 1, 1, 2, 1, 2, 2, 2, 2, 1, 2, 1, 2, 1   // N
// 
//    1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, // A
//    2, 2, 1, 1, 0, 2, 0, 1, 2, 1, 1, 1, 0, 2, // B
//    1, 2, 2, 2, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, // C
//    2, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, // D
//    2, 1, 1, 2, 0, 1, 2, 1, 0, 1, 2, 1, 0, 2, // E
//    0, 0, 2, 2, 2, 2, 2, 0, 1, 1, 0, 1, 0, 0, // F
//    1, 2, 0, 0, 0, 2, 2, 0, 0, 0, 0, 2, 2, 1, // G
//    2, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 2, 1, 1, // H
//    2, 0, 0, 2, 1, 2, 0, 1, 2, 1, 1, 2, 2, 2, // I
//    1, 1, 0, 0, 1, 1, 1, 2, 0, 1, 0, 2, 2, 1, // J
//    1, 1, 0, 0, 2, 1, 2, 0, 0, 0, 2, 2, 1, 1, // K
//    0, 0, 2, 2, 2, 2, 2, 0, 0, 1, 2, 2, 2, 1, // L
//    0, 1, 2, 2, 1, 1, 1, 1, 2, 2, 2, 0, 0, 1, // M
//    1, 1, 1, 2, 1, 2, 1, 2, 2, 2, 1, 1, 2, 1  // N
//    };
//    Board board(14, 14);
//    board.grid = grid;
//    /*board.special_moves1(Player::WHITE);*/
//    GameState game_state(board, Player::WHITE, nullptr, 2, 249);
//    game_state.board.displayGrid();
//    auto [moves, eat_moves] = bots[Player::WHITE]->select_move(game_state);
//    game_state.board.Print_move(moves, eat_moves, Player::WHITE);
//    }
//    
//    
//    std::cout.rdbuf(prevCoutBuf);  // 恢复旧的buf，输出回控制台
//    logFile.close();  // 关闭日志文件
//}

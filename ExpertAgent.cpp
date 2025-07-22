#include "ExpertAgent.h"

const float MIN_SCORE = -9999999;
const float MAX_SCORE = 9999999;
const int layout_score[] = { 7, 35, 800, 8000, 15, 400, 15000 };
int ExpertAgent::expert_layout(const GameState& game_state) {
    std::vector<std::pair<int, int>> stones_score;
    stones_score.reserve(196);
    for (int candidate_index = 0; candidate_index < 196; candidate_index++) {
        if (game_state.board.grid[candidate_index] == 0) {
            int stone_score = compute_expert_layout(game_state, candidate_index);
            stones_score.emplace_back(candidate_index, stone_score);
        }
    }

    std::sort(stones_score.begin(), stones_score.end(),
        [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            return a.second > b.second;
        });

    std::mt19937 rng(static_cast<unsigned int>(time(nullptr))); // 随机数生成器
    std::uniform_int_distribution<int> dist; // 随机数分布

    if (stones_score.size() > 2) {
        if (stones_score[0].second - stones_score[2].second < 100) {
            dist = std::uniform_int_distribution<int>(0, 2);
            int pos = dist(rng);
            return stones_score[pos].first;
        }
        else if (stones_score[0].second - stones_score[1].second < 150) {
            dist = std::uniform_int_distribution<int>(0, 1);
            int pos = dist(rng);
            return stones_score[pos].first;
        }
        else {
            return stones_score[0].first;
        }
    }
    else {
        return stones_score[0].first;
    }
}

int ExpertAgent::compute_expert_layout(const GameState& game_state, const int& point) {
    int score_sum = 0;
    for (const std::vector<int>& neighbor : neighbors4(point)) {
        int player1_stones = 0;
        int player2_stones = 0;
        int score = 0;
        int s = 0;

        for (const int& nei : neighbor) {
            if (game_state.board.grid[nei] == game_state.next_player) {
                player1_stones += 1;
                s = 1;
            }
            if (game_state.board.grid[nei] == other(game_state.next_player)) {
                player2_stones += 1;
                s = 1;
            }
        }

        if (player1_stones == 0 && player2_stones == 0 && s == 1) {
            score = layout_score[0];
        }
        else if (player1_stones > 0 && player2_stones > 0) {
            score = 0;
        }
        else if (player1_stones > 0) {
            score = layout_score[player1_stones];
        }
        else if (player2_stones > 0) {
            score = layout_score[player2_stones + 3];
        }
        score_sum += score;
    }
    return score_sum;
}

std::tuple<std::vector<int>, std::vector<int>> ExpertAgent::walk_strategy(GameState& gamestate)
{
    std::vector<std::vector<int>> moves = gamestate.board.legal_moves(gamestate.next_player,2);
    std::vector<std::pair<std::vector<int>, int>> moves_score;
    for (const auto& move : moves) {
        int move_score = compute_walk_strategy(move, gamestate);
        moves_score.push_back({ move, move_score });
    }

    std::sort(moves_score.begin(), moves_score.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
        });

    std::vector<int> mov;
    if (moves_score[0].second <= 400) {
        mov = moves_score[std::rand() % moves_score.size()].first;
    }
    else {
        mov = moves_score[0].first;
    }
    Board new_board = gamestate.board;
    new_board.go_take_stone(gamestate.next_player, mov);
    int s = new_board.squire(gamestate.next_player, mov.back());

    if (s == 1) {
        std::vector<int> eat_stone = { eat_strategy(gamestate.board, gamestate.next_player) };
        return {mov,eat_stone};
    }
    else if (s == 2) {
        int eat_stone = eat_strategy(gamestate.board, gamestate.next_player);
        gamestate.board.grid[eat_stone] = 0;
        int eat_stone1 = eat_strategy(gamestate.board, gamestate.next_player);
        gamestate.board.grid[eat_stone] = other(gamestate.next_player);
        std::vector<int> eat_stones;
        eat_stones.push_back(eat_stone);
        eat_stones.push_back(eat_stone1);
        return { mov,eat_stones };
    }
    else {
        return { mov,{} };
    }
}

int ExpertAgent::compute_walk_strategy(const std::vector<int>& move, GameState& gamestate)
{
    int score = 0;
    int inital_squires = 0;
    if (move.size() >= 5) {
        score = (2 * (move.size() - 1) - 1) * 200;
    }
    /*score = (2 * (move.size() - 1) - 1) * 200;*/

    inital_squires = gamestate.board.squire(gamestate.next_player, move[0]);
    gamestate.board.grid[move[0]] = 0;
    int end_squires = gamestate.board.squire(gamestate.next_player, move.back());
    gamestate.board.grid[move[0]] = gamestate.next_player;

    if (inital_squires == 0) {
        if (end_squires == 0) {
            return score;
        }
        else if (end_squires == 1) {
            return score + 2000;
        }
        else if (end_squires == 2) {
            return score + 3000 * 2;
        }
    }

    if (inital_squires == 1 && end_squires == 1) {
        return 8000;
    }
    if (inital_squires == 1 && end_squires == 2) {
        return 12000;
    }
    if (inital_squires == 2 && end_squires == 1) {
        return 10000;
    }
    if (inital_squires == 2 && end_squires == 2) {
        return 20000;
    }
    if (inital_squires == 1 && end_squires == 0) {
        return 450;
    }
    if (inital_squires == 2 && end_squires == 0) {
        return 500;
    }

    return score;
}

int ExpertAgent::compute_eat_strategy(const std::vector<int>& move, Board& board, int inital_squires, Player next_player)
{
    int score = 400;
    board.grid[move[0]] = 0;
    int end_squires = board.squire(next_player, move.back());
    board.grid[move[0]] = next_player;

    if (inital_squires == 0) {
        if (end_squires == 0) {
            score = score;
        }
        else if (end_squires == 1) {
            score += 2000;
        }
        else if (end_squires == 2) {
            score += 3000 * 2;
        }
        else if (end_squires == 3) {
            score += 5000 * 2;
        }
        else if (end_squires == 4) {
            score += 6000 * 2;
        }
        return score;
    }
    if (inital_squires == 1 && end_squires == 1) {
        score = 8000;
        return score;
    }
    if (inital_squires == 1 && end_squires == 2) {
        score = 12000;
        return score;
    }
    if (inital_squires == 2 && end_squires == 1) {
        score = 10000;
        return score;
    }
    if (inital_squires == 2 && end_squires == 2) {
        score = 20000;
        return score;
    }
    if (inital_squires == 1 && end_squires == 0) {
        score = 450;
        return score;
    }
    if (inital_squires == 2 && end_squires == 0) {
        score = 500;
        return score;
    }

    return score;
}

int ExpertAgent::eat_strategy(Board& board, Player player)
{
    
    std::vector<std::pair<std::vector<int>, int>> moves_score;
    int score = 600;
    int stone_num = board.same_stone(other(player)).size();
    std::vector<std::vector<int>> legal_moves = board.legal_moves(other(player), 2);
    
    for (const auto& moves : legal_moves) {
        int inital_squires = board.squire(other(player), moves[0]);
        int tem_score = compute_eat_strategy(moves, board, inital_squires, other(player));
        if (inital_squires == 1) {
            if (stone_num > 30) {
                if (score < tem_score) {
                    score = tem_score;
                }
            }
            else {
                score = 12000;
            }
        }
        else if (inital_squires == 0) {
            if (score < tem_score) {
                score = tem_score;
            }
        }
        else if (inital_squires == 2) {
            if (score < tem_score) {
                score = tem_score;
            }
        }
       
        moves_score.push_back({ moves, score });
    }

    if (!moves_score.empty()) {
        std::sort(moves_score.begin(), moves_score.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
            });
    }

    return moves_score[0].first[0];
}




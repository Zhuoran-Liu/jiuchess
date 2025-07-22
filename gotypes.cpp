#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "gotypes.h"

//enum Player {
//    BLACK = 1,
//    WHITE = 2,
//};

Player other(Player player) {
    if (player == WHITE)
        return Player::BLACK;
    else
        return Player::WHITE;
}


Point::Point(int row, int col) : row(row), col(col) { /*std::cout << "create" << std::endl;*/ }
std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "Point(" << p.row << "," << p.col << ")";
    return os;
}

std::vector<Point> neighbors(const Point& p) {
    std::vector<Point> neighbours;
    neighbours.reserve(4);
    neighbours.emplace_back(Point(p.row - 1, p.col));  // 上
    neighbours.emplace_back(Point(p.row + 1, p.col));  // 下
    neighbours.emplace_back(Point(p.row, p.col - 1));  // 左
    neighbours.emplace_back(Point(p.row, p.col + 1));  // 右
    return neighbours;
}
std::map<std::string, Point> neighbors1(const Point& p) {
    std::map<std::string, Point> neighbourDict;
    neighbourDict["down"] = Point(p.row - 1, p.col);
    neighbourDict["top"] = Point(p.row + 1, p.col);
    neighbourDict["left"] = Point(p.row, p.col - 1);
    neighbourDict["right"] = Point(p.row, p.col + 1);
    return neighbourDict;
}

std::vector<int> neighbors2(int index) {
    std::vector<int> neighbours;
    neighbours.reserve(4); //152 10 8
    int row = index / 14;
    int col = index % 14;

    if (row > 0) neighbours.emplace_back((row - 1) * 14 + col); // 上
    if (row + 1 < 14) neighbours.emplace_back((row + 1) * 14 + col); // 下
    if (col > 0) neighbours.emplace_back(row * 14 + (col - 1)); // 左
    if (col + 1 < 14) neighbours.emplace_back(row * 14 + (col + 1)); // 右
    return neighbours;
}

std::map<int, int> neighbors3(int index) {
    std::map<int, int> neighbourDict;
    int row = index / 14;
    int col = index % 14;
    if (row > 0) neighbourDict.insert(std::make_pair(1, (row - 1) * 14 + col));
    if (row + 1 < 14) neighbourDict.insert(std::make_pair(2, (row + 1) * 14 + col));
    if (col > 0) neighbourDict.insert(std::make_pair(3, row * 14 + (col - 1)));
    if (col + 1 < 14) neighbourDict.insert(std::make_pair(4, row * 14 + (col + 1)));
    return neighbourDict;
}

std::vector<std::vector<int>> neighbors4(int index) {
    int row = index / 14;
    int col = index % 14;
    std::vector<std::vector<int>> neighbors;
    if (row > 0 && col > 0) {
        neighbors.emplace_back(std::vector<int>{(row - 1) * 14 + col - 1, (row - 1) * 14 + col, row * 14 + col - 1});
    }
    if (row < 13 && col > 0) {
        neighbors.emplace_back(std::vector<int>{(row + 1) * 14 + col - 1, (row + 1) * 14 + col, row * 14 + col - 1});
    }
    if (row > 0 && col < 13) {
        neighbors.emplace_back(std::vector<int>{(row - 1) * 14 + col + 1, (row - 1) * 14 + col, row * 14 + col + 1});
    }
    if (row < 13 && col < 13) {
        neighbors.emplace_back(std::vector<int>{(row + 1) * 14 + col + 1, (row + 1) * 14 + col, row * 14 + col + 1});
    }
    return neighbors;
}
//std::map<int, int> neighbors3(int index) {
//    std::map<std::string, int> neighbourDict;
//    int row = index / 14;
//    int col = index % 14;
//    if (row > 0) neighbourDict["top"] = (row - 1) * 14 + col;
//    if (row + 1 < 14) neighbourDict["down"] = (row + 1) * 14 + col;
//    if (col > 0) neighbourDict["left"] = row * 14 + (col - 1);
//    if (col + 1 < 14) neighbourDict["right"] = row * 14 + (col + 1);
//    return neighbourDict;
//}

//std::vector<Point> Point::neighbors() {
//    return { Point(row - 1,col), Point(row + 1,col), Point(row,col - 1), Point(row,col + 1) };
//}

//std::map<std::string, Point> neighbors1() {
//    return {
//        {"down", Point(row - 1,col)},
//        {"top", Point(row + 1, col)},
//        {"left", Point(row, col - 1)},
//        {"right", Point(row, col + 1)}
//    };
//}
//#include <vector>
//#include <map>
//#include <string>
//#include <iostream>
//#include "gotypes.h"
//
////enum Player {
////    BLACK = 1,
////    WHITE = 2,
////};
//
//Player other(Player player) {
//    if (player == WHITE)
//        return Player::BLACK;
//    else
//        return Player::WHITE;
//}
//
//
//Point::Point(int row, int col) : row(row), col(col) { /*std::cout << "create" << std::endl;*/ }
//std::ostream& operator<<(std::ostream& os, const Point& p) {
//    os << "Point(" << p.row << "," << p.col << ")";
//    return os;
//}
//
//std::vector<Point> neighbors(const Point& p) {
//    std::vector<Point> neighbours;
//    neighbours.reserve(4);
//    neighbours.emplace_back(Point(p.row - 1, p.col));  // 上
//    neighbours.emplace_back(Point(p.row + 1, p.col));  // 下
//    neighbours.emplace_back(Point(p.row, p.col - 1));  // 左
//    neighbours.emplace_back(Point(p.row, p.col + 1));  // 右
//    return neighbours;
//}
//std::map<std::string, Point> neighbors1(const Point& p) {
//    std::map<std::string, Point> neighbourDict;
//    neighbourDict["down"] = Point(p.row - 1, p.col);
//    neighbourDict["top"] = Point(p.row + 1, p.col);
//    neighbourDict["left"] = Point(p.row, p.col - 1);
//    neighbourDict["right"] = Point(p.row, p.col + 1);
//    return neighbourDict;
//}
////std::vector<Point> Point::neighbors() {
////    return { Point(row - 1,col), Point(row + 1,col), Point(row,col - 1), Point(row,col + 1) };
////}
//
////std::map<std::string, Point> neighbors1() {
////    return {
////        {"down", Point(row - 1,col)},
////        {"top", Point(row + 1, col)},
////        {"left", Point(row, col - 1)},
////        {"right", Point(row, col + 1)}
////    };
////}
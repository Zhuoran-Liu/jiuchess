#pragma once
#include <vector>
#include<map>
#include<ostream>

enum Player {
    NONE = 99999,
    BLACK = 1,
    WHITE = 2,

};

Player other(Player player);

struct Point {
    int row;
    int col;

    Point() : row(0), col(0) {}
    Point(int row, int col);
    friend std::ostream& operator<<(std::ostream& os, const Point& p);
    bool operator<(const Point& other) const {
        if (row == other.row) {
            return col < other.col;
        }
        return row < other.row;
    };

};
std::vector<Point> neighbors(const Point& p);
std::vector<int> neighbors2(int index);
std::map<std::string, Point> neighbors1(const Point& p);
std::map<int, int> neighbors3(int index);
std::vector<std::vector<int>> neighbors4(int index);

//#ifndef _GOTYPES_H
//#define  _GOTYPES_H
//#include <vector>
//#include<map>
//#include<ostream>
////#include "gotypes.h"
//enum Player {
//    BLACK = 1,
//    WHITE = 2,
//};
//
//Player other(Player player);
//
//struct Point {
//    int row;
//    int col;
//
//    Point() : row(0), col(0) {}
//    Point(int row, int col);
//    friend std::ostream& operator<<(std::ostream& os, const Point& p);
//    bool operator<(const Point& other) const {
//        if (row == other.row) {
//            return col < other.col;
//        }
//        return row < other.row;
//    };
//
//};
//std::vector<Point> neighbors(const Point& p);
//std::map<std::string, Point> neighbors1(const Point& p);
//
//#endif // 

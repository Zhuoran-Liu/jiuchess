#ifndef _TEST2_H
#define  _TEST2_H
#include<string>

class Student {
private:
	int num;
	std::string name;

public:
	Student();
	Student(int num, std::string name);
	~Student();
	void display();
};



#endif // !_TEST2_H


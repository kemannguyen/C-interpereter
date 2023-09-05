#pragma once
#include <ostream>
class ContainerClass
{
	std::string name;
	int value;

public:

	ContainerClass();
	ContainerClass(const char* name, int value);
	ContainerClass(std::string name, int value);

	ContainerClass& operator=(const ContainerClass& rhs);

	void assignVal(int input);

	std::string getName();

	int getVal();

	void Print();

	~ContainerClass() = default;
};


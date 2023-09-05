#include "ContainerClass.h"
#include <iostream>

ContainerClass::ContainerClass()
{
	name = "";
	value = 0;
}
ContainerClass::ContainerClass(const char* name, int value)
{
	this->name = name;
	this->value = value;
}
ContainerClass::ContainerClass(std::string name, int value)
{
	this->name = name;
	this->value = value;
}
ContainerClass& ContainerClass::operator=(const ContainerClass& rhs)
{
	//std::cout << "operator= " << cString;
	value = rhs.value;
	//std::cout << "--> " << cString << std::endl;
	return*this;
}
void ContainerClass::assignVal(int input)
{
	value = input;
}
std::string ContainerClass::getName() {
	return name;
}
int ContainerClass::getVal() {
	return value;
}
void ContainerClass::Print() {
	std::cout << name << " " << value << "\n";
}




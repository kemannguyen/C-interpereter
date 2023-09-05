// lab 6 interpereter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "ContainerClass.h"
#include "InterpereterClass.h"

int main()
{
	std::ifstream file("input.txt");
	std::string strline;
	std::ofstream file_out("output.txt");
	InterpereterClass t1(file_out);
	while (std::getline(file, strline))
	{
		std::cout << "Interpereting: " << strline << "\n";
		t1.tokenizer(strline);
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

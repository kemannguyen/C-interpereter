#pragma once
#include "ContainerClass.h"

#include <iostream>
#include <regex>
#include <string>
#include <sstream>
#include <fstream>
#include <bitset>

class InterpereterClass
{
	std::vector<ContainerClass> contList;

	std::vector<std::string>alltokens;
	int position = 0;
	const std::string ETX = "\u0003";
	int state = 1, ansInt;
	std::string answer;
	bool config = false, print = false, assign = false, error = false;

public:
	InterpereterClass(std::ostream& out_stream);
	std::ostream& global_out(std::ostream& outs = std::cout);
	std::string peek();
	std::string peek(int steps);
	void consume(const std::string& token);
	void tokenizer(std::string sline);
	void evaluate(std::string sline);
	void ConfigStmt();
	void AssgStmt();
	void Stmt();
	void PrintStmt();
	int parse_MathExp();
	int parse_SumExp();
	int parse_ProductExp();
	int parse_PrimaryExp();

	~InterpereterClass() = default;
private:
	bool is_int(const std::string& token);
	bool is_variable(const std::string& token);
};


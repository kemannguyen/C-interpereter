#include "InterpereterClass.h"

InterpereterClass::InterpereterClass(std::ostream& out_stream)
{
	global_out(out_stream);
}

std::ostream& InterpereterClass::global_out(std::ostream& outs)
{
	static std::ostream& gbl_out = outs;
	return gbl_out;
}

std::string InterpereterClass::peek()
{
	return peek(0);
}


std::string InterpereterClass::peek(int steps)
{
	if (position + steps >= alltokens.size()) return ETX;

	return alltokens[position + steps];
}


void InterpereterClass::consume(const std::string& token)
{
	std::string next_token = peek();
	if (next_token == ETX) {
		throw std::runtime_error("Consumed past last token\n");
	}
	if (next_token != token) {
		throw std::runtime_error("Could not consume token " + token + "\n");
	}
	++position;
}

void InterpereterClass::tokenizer(std::string sline)
{
	config = false, print = false, assign = false, error = false;
	alltokens.clear();
	position = 0;

	std::stringstream ss(sline);
	std::string token;
	std::string stmttoken;

	//tokenize
	while (std::getline(ss, token, ' '))
	{
		alltokens.push_back(token);
	}

	Stmt();
}

void InterpereterClass::evaluate(std::string sline)
{
	config = false, print = false, assign = false, error = false;
	alltokens.clear();
	position = 0;

	std::stringstream ss(sline);
	std::string token;
	std::string stmttoken;

	//tokenize
	while (std::getline(ss, token, ' '))
	{
		alltokens.push_back(token);
	}

	Stmt();
}

void InterpereterClass::ConfigStmt()
{
	//std::cout << "DEBUG CONFIG" << "\n";


	std::string next_token = peek();

	if (next_token == "config")
	{
		consume("config");
	}
	next_token = peek();

	if (next_token == "hex")
	{
		consume("hex");
		state = 0;
	}
	else if (next_token == "bin")
	{
		consume("bin");
		state = 2;
	}
	else if (next_token == "dec")
	{
		consume("dec");
		state = 1;
	}
	else
	{
		state = 1;
	}
}
void InterpereterClass::AssgStmt()
{
	bool exist = false, varCheck = false;
	int answer = 0;

	//std::cout << "DEBUG ASSIGN\n";

	std::string next_token = peek();
	std::string name;

	if (is_variable(next_token))
	{
		name = next_token;
		consume(next_token);
	}
	else
	{
		return;
	}
	next_token = peek();

	if (next_token == "=")
	{
		consume("=");
	}

	next_token = peek();
	//std::cout << "next_token in print " << next_token;

	if (is_variable(next_token))
	{
		int tempV;
		for (auto v : contList)
		{
			if (v.getName() == next_token)
			{
				//std::cout << "DeBUG list " << v.getName() << " " << v.getVal();
				//consume(v.getName());
				tempV = v.getVal();
				answer = v.getVal();
				varCheck = true;
				break;
			}
		}
		if (!varCheck)
		{
			std::cout << "variable " << next_token << " is not declared\n";
			throw std::runtime_error("variable is not declared \n");
			return;
		}
		alltokens[position] = std::to_string(tempV);
		answer = parse_MathExp();
	}
	else
	{
		answer = parse_MathExp();
	}
	for (int i = 0; i < contList.size(); i++)
	{
		if (contList[i].getName() == name)
		{
			exist = true;
		}
	}

	if (error)
	{
		std::cout << "ERROR in assign\n";
		return;
	}

	if (exist)
	{
		for (int i = 0; i < contList.size(); i++)
		{
			if (contList[i].getName() == name)
			{
				contList[i].assignVal(answer);
			}
		}
	}
	else
	{
		ContainerClass tempCon(name, answer);
		contList.push_back(tempCon);
	}

	/*	for (auto t : contList)
		{
			std::cout << "list: " << t.getName() << "  " << t.getVal() << "\n";
		}*/

}

void InterpereterClass::Stmt()
{
	for (auto v : alltokens)
	{
		if (v == "print")
		{
			print = true;
		}
		if (v == "config")
		{
			config = true;
		}
		if (v == "=")
		{
			assign = true;
		}
	}
	if (print && !config && !assign)
	{
		PrintStmt();
	}
	else if (config && !print && !assign)
	{
		ConfigStmt();
	}
	else if (assign && !config && !print)
	{
		AssgStmt();
	}
	else
	{
		error = true;
		std::cout << "Error: mixing different statments\n";
		throw std::runtime_error("Error: mixing different statments \n");
	}
}

void InterpereterClass::PrintStmt()
{
	int answer = 0;

	//std::cout << "DEBUG PRINT" << "\n";

	std::string next_token = peek();
	std::vector<std::string> print;
	bool varCheck = false;

	if (next_token == "print")
	{
		consume("print");
	}
	next_token = peek();

	//std::cout << "next_token in print " << next_token;
	if (is_variable(next_token))
	{
		for (auto v : contList)
		{
			if (v.getName() == next_token)
			{
				//std::cout << "DeBUG list " << v.getName() << " " << v.getVal();
				consume(v.getName());
				answer = v.getVal();
				varCheck = true;
				break;
			}
		}
		if (!varCheck)
		{
			std::cout << "variable " << next_token << " is not declared\n";
			throw std::runtime_error("variable is not declared \n");
			return;
		}
	}
	else
	{
		answer = parse_MathExp();
	}
	if (error)
	{
		std::cout << "ERROR in print\n";
		return;
	}

	if (state == 1)
	{
		global_out() << std::dec << answer << "\n";
	}
	else if (state == 0)
	{
		global_out() << "0x" << std::hex << answer << "\n";
	}
	else
	{
		int tempInt = answer;
		std::string returnAnswer;
		for (int i = 0; tempInt > 0; i++)
		{
			print.push_back(std::to_string(tempInt % 2));
			tempInt = tempInt / 2;
		}
		for (int i = print.size() - 1; i >= 0; i--)
		{
			returnAnswer = returnAnswer + print[i];
		}
		global_out() << returnAnswer << " or ";
		global_out() << std::bitset<32>(answer).to_string() << "\n";
	}
}

int InterpereterClass::parse_MathExp() {
	return parse_SumExp();
}


int InterpereterClass::parse_SumExp() {
	int result = parse_ProductExp();

	std::string next_token = peek();
	//std::cout << next_token << "\n";
	while (1)
	{
		if (next_token == "+")
		{
			consume("+");
			result = (result + parse_ProductExp());
		}
		else if (next_token == "-")
		{
			consume("-");
			result = (result - parse_ProductExp());
		}
		else
		{
			break;
		}
		next_token = peek();
	}
	return result;
}


int InterpereterClass::parse_ProductExp() {
	int result = parse_PrimaryExp();

	std::string next_token = peek();
	//std::cout << next_token << "\n";

	while (1)
	{
		if (next_token == "*")
		{
			consume("*");
			result = (result * parse_PrimaryExp());
		}
		else if (next_token == "/")
		{
			consume("/");
			result = (result / parse_PrimaryExp());
		}
		else
		{
			break;
		}
		next_token = peek();
	}
	return result;
}


int InterpereterClass::parse_PrimaryExp()
{
	int value = 0;
	std::string next_token = peek();
	//std::cout <<"debug primexp" << next_token << "\n";

	if (is_int(next_token))
	{
		value = std::stoi(next_token);
		consume(next_token);
	}
	else if (is_variable(next_token))
	{
		for (auto v : contList)
		{
			if (v.getName() == next_token)
			{
				//std::cout << "DeBUG list " << v.getName() << " " << v.getVal();
				consume(v.getName());
				value = v.getVal();
				break;
			}
		}
	}
	else if (next_token == "(")
	{
		consume("(");
		value = parse_MathExp();
		if (peek() == ")")
		{
			consume(")");
		}
		else
		{
			std::cout << "expected ) instead of " << next_token << "\n";
			throw std::runtime_error("Expected: )\n");
			error = true;
		}
	}
	else
	{
		std::cout << "expected int, variable or ( ) instead of " << next_token << "\n";
		throw std::runtime_error("expected int, variable or ( )");
		error = true;
	}
	//std::cout << "math val::" << value << "\n";
	return value;
}

bool InterpereterClass::is_int(const std::string& token) {
	std::regex Ints("-?[0-9]+");
	if (regex_match(token, Ints))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InterpereterClass::is_variable(const std::string& token)
{
	std::regex Var("[a-zA-z][a-zA-z0-9]*");
	if (regex_match(token, Var)) {
		return true;
	}
	else
	{
		return false;
	}
}

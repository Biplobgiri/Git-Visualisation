#include<iostream>
#include"Input.hpp"

void exec(parse_return argument)
{
	std::cout << "exec fun vitra" << std::endl;

	if (argument.validity==false)
	{
		std::cout << "wrong validity fun vitra" << std::endl;

		return;
	}

	switch (argument.command)
	{
	case INIT:
		std::cout << "init switch vitra" << std::endl;

		init_command(argument);
		break;
	case COMMIT:
		commit(argument);
		break;
	case BRANCH:
		branch(argument);
		break;
	case MERGE:
		merge(argument);
		break;
	case CHECKOUT:
		checkout(argument);
		break;
	default:
		break;
	}

}
/*
void command_input(std::string string_input)
{
	bool enter_again = true;
	std::string command_line;
	//do {
		std::cout << "Enter Command:";
		std::getline(std::cin, command_line);
		struct parse_return argument = parse(command_line.c_str());
		
		//struct parse_return argument = parse(string_input.c_str());

		exec(argument);
		std::cout << std::endl;
		std::cout << "Do you want to enter another command?" << std::endl;
		//std::cin >> enter_again;
		std::cin.get();
	}// while (enter_again);
	*/


parse_return command_input(std::string string_input)
{
	std::cout << "command_input fun vitra" << std::endl;

	//std::cout << "Enter Command:";
	//std::getline(std::cin, string_input);
	struct parse_return argument = parse(string_input.c_str());
	exec(argument);
	return argument;

}

#include<iostream>
#include"Input.hpp"

void exec(parse_return argument)
{

	if (argument.validity==false)
	{
		std::cout << "Invalid Command" << std::endl;
		std::cout<<std::endl;
		return;
	}

	switch (argument.command)
	{
	case INIT:

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



parse_return command_input(std::string string_input)
{


	struct parse_return argument = parse(string_input.c_str());
	exec(argument);
	return argument;

}

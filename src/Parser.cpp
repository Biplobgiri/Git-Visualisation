#include "Parser.hpp"
#include <iostream>
#include <string.h>
#include<cctype>

parse_return::parse_return() {
	command = ERROR;
	validity = false;
}


const char keywords[][10] = { "git", "commit", "checkout", "branch", "merge","init" };

Tokenizer::Tokenizer(const char* line)
{
	this->ptr = (char*)line;
	this->type = Token_Type::ERROR;
}


//lexing
bool tokenize(Tokenizer* tokenizer)
{
	char* current_ptr = tokenizer->ptr;
	while (*current_ptr == ' ' || *current_ptr == '\t')
	{
		current_ptr++;
	}
	tokenizer->ptr = current_ptr;


	if (std::isalpha(*current_ptr) || *current_ptr == '-') {
		while (*current_ptr != ' ' && *current_ptr != '\t' && *current_ptr)
		{

			current_ptr++;

		}
		tokenizer->data = tokenizer->ptr;
		tokenizer->data = tokenizer->data.substr(0, current_ptr - tokenizer->ptr);
		for (int i = 0; i < sizeof(keywords); i++) {
			if (tokenizer->data == keywords[i]) {
				tokenizer->type = Token_Type(i);
				tokenizer->ptr = current_ptr;
				return true;
			}
		}
		tokenizer->type = Token_Type::TEXT;
		tokenizer->ptr = current_ptr;

		return true;
	}

	if (*current_ptr == '"')
	{
		current_ptr++;
		while (*current_ptr && *current_ptr != '"')
		{
			current_ptr++;
		}
		tokenizer->data = tokenizer->ptr;
		tokenizer->data = tokenizer->data.substr(0, current_ptr - tokenizer->ptr);
		if (*current_ptr != '"')
		{
			tokenizer->type = Token_Type::ERROR;
			tokenizer->ptr = current_ptr;
			return false;
		}
		current_ptr++;
		tokenizer->data = tokenizer->ptr;
		tokenizer->data = tokenizer->data.substr(0, current_ptr - tokenizer->ptr);
		tokenizer->type = Token_Type::TEXT;
		tokenizer->ptr = current_ptr;
		return true;

	}
	tokenizer->type = Token_Type::ERROR;
	tokenizer->ptr = current_ptr;
	return false;
}

//parsing
parse_return parse(const char* line)
{
	struct parse_return argument;
	Tokenizer tokenizer = Tokenizer(line);
	int i = 0;
	//std::cout << "From parse return fn \n";
	while (tokenize(&tokenizer)) {
		i++;
		//std::cout << i << "\t" << tokenizer.data << std::endl;
		switch (tokenizer.type)
		{
		case GIT:
			if (i == 1)
				argument.validity = true;
			else {
				argument.validity = false;
				std::cout << "Invalid Command -not a git command" << std::endl;
				return argument;
			}
			break;
		case INIT:
			if (i == 2)
			{
				argument.command = INIT;
			}
			else
				argument.command = ERROR;
			break;
		case COMMIT:
			if (i == 2) {
				argument.command = tokenizer.type;
			}
			else
				argument.command = ERROR;
			break;
		case CHECKOUT:
			if (i == 2) {
				argument.command = tokenizer.type;
			}
			else
				argument.command = ERROR;
			break;

		case BRANCH:
			if (i == 2) {
				argument.command = tokenizer.type;
			}
			else
				argument.command = ERROR;
			break;
		case MERGE:
			if (i == 2) {
				argument.command = tokenizer.type;
			}
			else
				argument.command = ERROR;
			break;

		case TEXT:
			if (i >= 3) {
				argument.msg=tokenizer.data.substr(1, tokenizer.data.length() - 2);
				break;
			}
			else {
				argument.command = ERROR;
				argument.validity = false;
				break;
			}
		case ERROR:
			argument.validity = false;
			std::cout << "Invalid Command" << std::endl;
			return argument;
		default:
			break;
		}
	}
	return argument;
}
#pragma once

#include <string>

enum Token_Type {
	GIT, COMMIT, CHECKOUT, BRANCH, MERGE, INIT, TEXT, ERROR
};

struct parse_return {
	bool validity;
	Token_Type command;
	std::string msg;

	parse_return();
};

class Tokenizer {
public:
	char* ptr;
	std::string data;
	Token_Type type;
	Tokenizer(const char* line);
};

bool tokenize(Tokenizer* tokenizer);
parse_return parse(const char* line);

#include <iostream>
#include <string.h>
#include<string>
#include<cctype>


enum Token_Type {
	GIT, COMMIT, CHECKOUT, BRANCH, MERGE, TEXT, ERROR
};
enum Message_Type {
	COMMIT_MESSAGE,BRANCH_NAME,NONE
};

struct parse_return {
	bool validity;
	Token_Type command;
	std::string msg;
	Message_Type msg_type;
	parse_return() {
		validity = false;
		command = ERROR;
		msg_type = NONE;
	}
};
const char keywords[][10] = {"git", "commit", "checkout", "branch", "merge"};
class Tokenizer {
public:
	char* ptr;
	std::string data;
	Token_Type type;
	Tokenizer(const char* line)
	{
		this->ptr = (char *)line;
		this->type = Token_Type::ERROR;
	}
};
bool tokenize(Tokenizer* tokenizer)
{
	char* current_ptr = tokenizer->ptr;
	while (*current_ptr == ' ' || *current_ptr == '\t')
	{
		current_ptr++;
	}
	tokenizer->ptr = current_ptr;


	if (std::isalpha(*current_ptr) || *current_ptr=='-') {
		while (*current_ptr != ' ' && *current_ptr != '\t' && *current_ptr)
		{

			current_ptr++;

		}
		tokenizer->data = tokenizer->ptr;
		tokenizer->data = tokenizer->data.substr(0,current_ptr - tokenizer->ptr);
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
			tokenizer->data = tokenizer->data.substr(0,current_ptr - tokenizer->ptr);
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
parse_return parse(const char* line)
{
	struct parse_return argument;
	Tokenizer tokenizer = Tokenizer(line);
	int i=0;
	while (tokenize(&tokenizer)) {
		i++;
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
		case COMMIT:
			if (i==2){
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
			if (i == 2){
				argument.command = tokenizer.type;
				argument.msg_type = BRANCH_NAME;
			}
			else
				argument.command = ERROR;
			break;
		case MERGE:
			if (i==2){
				argument.command = tokenizer.type;
				argument.msg_type = BRANCH_NAME;
			}
			else
				argument.command = ERROR;
			break;

		case TEXT:
			if (tokenizer.data == "-m")
			{
				argument.msg_type = COMMIT_MESSAGE;
				
			}
			else if (tokenizer.data == "-b")
			{
				argument.msg_type = BRANCH_NAME;
				
			}
			else {
				argument.msg = tokenizer.data;
			}
			break;
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


int main() {
	std::string line = "git commit -m \"A message\"";
	struct parse_return argument=parse(line.c_str());
	if (argument.validity == false)
		return -1;
	std::cout << "Command Validity is \t" << argument.validity << std::endl;
	std::cout << "Command type is\t" << argument.command << "\n Message is " << argument.msg << std::endl;
	return 0;
}




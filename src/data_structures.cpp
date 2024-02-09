#include <iostream>
#include <string.h>
#include<string>
#include<cctype>



enum Token_Type {
	GIT, COMMIT, CHECKOUT, BRANCH, MERGE, TEXT, ERROR
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
		this->type = ERROR;
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
		tokenizer->type = TEXT;
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
			tokenizer->type = ERROR;
			tokenizer->ptr = current_ptr;
			return false;
		}
		current_ptr++;
		tokenizer->data = tokenizer->ptr;
		tokenizer->data = tokenizer->data.substr(0, current_ptr - tokenizer->ptr);
		tokenizer->type = TEXT;
		tokenizer->ptr = current_ptr;
		return true;

	}
	tokenizer->type = ERROR;
	tokenizer->ptr = current_ptr;
	return false;
}
void parse(const char* line)
{
	Tokenizer tokenizer = Tokenizer(line);
	while (tokenize(&tokenizer)) {
		switch (tokenizer.type)
		{
		case GIT:
		case COMMIT:
		case CHECKOUT:
		case BRANCH:
		case MERGE:
		case TEXT:
		case ERROR:
		default:
			break;
		}
	}
}

int main() {
	std::string line = "git commit -m \"A message\"";
	parse(line.c_str());






		return 0;

}


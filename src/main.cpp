#include <iostream>
#include <string.h>
#include<string>
#include<cctype>

#include<map>
#include<vector>
#include<cstring>


enum Token_Type {
	GIT, COMMIT, CHECKOUT, BRANCH, MERGE, TEXT, ERROR, INIT
};


struct parse_return {
	bool validity;
	Token_Type command;
	std::string msg;

	parse_return() {
		command = ERROR;
		validity = false;
	}


};
const char keywords[][10] = { "git", "commit", "checkout", "branch", "merge","init" };
class Tokenizer {
public:
	char* ptr;
	std::string data;
	Token_Type type;
	Tokenizer(const char* line)
	{
		this->ptr = (char*)line;
		this->type = Token_Type::ERROR;
	}
};

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
				argument.command = tokenizer.type;
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
			argument.msg = tokenizer.data;
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
//dsa
int main()
{
	std::string line = "git commit -m \"A message\"";
	struct parse_return argument = parse(line.c_str());
	//std::cout << argument.command << "\t" << argument.msg << "\t" << argument.validity << std::endl;
	return 0;
}


class Node {
public:
	int commit_id;
	std::string commit_name;
	std::map<std::string,  Node*> forward;
	std::map<std::string,  Node*> backward;
};
std::map< std::string, Node*> branches_status;
std::string current_branch,prev_branch;
std::map<int, Node*> commit_id_map;
std::map<int, std::string> commit_id_branchmap;
bool recent_branch_change = false;
Node* pre = NULL;
Node* next=NULL;
Node* head = NULL;//main head before checkout
Node* current_head = NULL;//after
int commit_id_val = 0;

	void init_command(parse_return parse_value)
	{
		current_branch = "MASTER";
		branches_status.insert(std::make_pair(current_branch, pre));
	}
	void commit(parse_return parse_value)
	{
		Node* node = (Node*)malloc(sizeof(Node));
		node->commit_name = parse_value.msg;
		node->commit_id = commit_id_val;
		commit_id_val++;
		if (next != NULL)
		{
			current_branch += "-child";//making a new child branch
		}
		if (pre != NULL)
		{
			pre->forward.insert(std::make_pair(current_branch, node));
		}
		if (recent_branch_change)
		{
			node->backward.insert(std::make_pair(prev_branch, pre));
			recent_branch_change = false;
		}
		node->forward.insert(std::make_pair(current_branch, next));
		node->backward.insert(std::make_pair(current_branch, pre));

		branches_status.insert(std::make_pair(current_branch, node));
		pre = node;
		commit_id_map.insert(std::make_pair(node->commit_id, node));
		commit_id_branchmap.insert(std::make_pair(node->commit_id, current_branch));

		//condition for commiting in the middle of already exisiting branch remaingn 


	}
	void branch(parse_return parse_value)
	{
		recent_branch_change = true;
		prev_branch = current_branch;
		current_branch = parse_value.msg;
		branches_status.insert(std::make_pair(current_branch, next));
	}
	void checkout(parse_return parse_value)
	{
		bool found = false;
		head = pre;
		//for commit_id
		std::map<int, Node*> ::iterator iter;
		for (iter = commit_id_map.begin(); iter!=commit_id_map.end(); iter++)
		{
			if (std::to_string(iter->first) == (parse_value.msg))
			{
				current_head = iter->second;
				found = true;
				current_branch = commit_id_branchmap[iter->first];
				break;
			}
		}
		//for branch
		if (!found) {
			std::map< std::string, Node*> ::iterator iter2;
			for (iter2 = branches_status.begin(); iter2 != branches_status.end(); iter2++)
			{
				if (iter2->first == parse_value.msg)
				{
					current_head = iter2->second;
					found = true;
					current_branch = iter2->first;
					break;
				}
			}
		}
		pre = current_head;
		//branch -b remaingin
		
		if (!found)
		{
			std::cout << "The commit id or branchname is invalid" << std::endl;
			return;
		}
	}
	void merge(parse_return parse_value)
	{
		std::map< std::string, Node*> ::iterator iter;
		for (iter = branches_status.begin(); iter != branches_status.end(); iter++)
		{
			if (iter->first == parse_value.msg && iter->first!=current_branch)
			{
				merge_commit(parse_value.msg);
			}

		}
	}
	void merge_commit(std::string branch_mergeto)
	{
		parse_return temp;
		if (pre == NULL)
		{
			std::cout << "You tried to merge early" << std::endl;
			return;
		}
		Node* node = (Node*)malloc(sizeof(Node));
		node->commit_id = commit_id_val;
		commit_id_val++;


		node->forward.insert(std::make_pair(current_branch, next));
		node->backward.insert(std::make_pair(current_branch, branches_status[current_branch]));
		node->backward.insert(std::make_pair(current_branch, branches_status[branch_mergeto]));


		branches_status[branch_mergeto] = node;
		branches_status[current_branch] = node;
		current_branch = branch_mergeto;

		pre = node;
		commit_id_map.insert(std::make_pair(node->commit_id, node));
		commit_id_branchmap.insert(std::make_pair(node->commit_id, current_branch));

	}









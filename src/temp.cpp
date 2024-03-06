#include <iostream>
#include <string.h>
#include<string>
#include<cctype>

#include<map>
#include<vector>
#include<cstring>

#include "Parser.hpp"


//dsa

class Node {
public:
	int commit_id = 0;
	std::string commit_name;

	std::map<std::string, Node*> forward;
	std::map<std::string, Node*> backward;
};

std::map< std::string, Node*> branches_status;
std::string current_branch, prev_branch;
std::map<int, Node*> commit_id_map;
std::map<int, std::string> commit_id_branchmap;//rm vec
bool recent_branch_change = false;
Node* pre = NULL;
Node* next = NULL;
Node* head = NULL;//main head before checkout
Node* current_head = NULL;//after
int commit_id_val = 0;
Node* node_array[10];
void init_command(parse_return parse_value)
{
	current_branch = "MASTER";
	branches_status[current_branch] = nullptr;
	//branches_status.insert(std::make_pair(current_branch, (Node *)NULL));
}
void commit(parse_return parse_value)
{
	Node* node = new Node();
	node->commit_name = parse_value.msg;
	node->commit_id = commit_id_val;
	node_array[commit_id_val] = node;
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

	//branches_status.insert(std::make_pair(current_branch, node));
	branches_status[current_branch] = node;
	pre = node;
	commit_id_map.insert(std::make_pair(node->commit_id, node));
	commit_id_branchmap.insert(std::make_pair(node->commit_id, current_branch));

	//condition for commiting in the middle of already exisiting branch remaingn 
	//output



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
	for (iter = commit_id_map.begin(); iter != commit_id_map.end(); iter++)
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
void merge_commit(std::string branch_mergeto)
{
	parse_return temp;
	if (pre == NULL)
	{
		std::cout << "You tried to merge early" << std::endl;
		return;
	}
	Node* node = new Node;
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
void merge(parse_return parse_value)
{
	std::map< std::string, Node*> ::iterator iter;
	for (iter = branches_status.begin(); iter != branches_status.end(); iter++)
	{
		if (iter->first == parse_value.msg && iter->first != current_branch)
		{
			merge_commit(parse_value.msg);
		}

	}
}
void exec(parse_return argument)
{
	if (!argument.validity) return;

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
void command_input()
{
	bool enter_again = true;
	std::string command_line;
	do {
		std::cout << "Enter Command:";
		std::getline(std::cin, command_line);
		struct parse_return argument = parse(command_line.c_str());
		exec(argument);
		std::cout << std::endl;
		std::cout << "Do you want to enter another command?" << std::endl;
		std::cin >> enter_again;
		std::cin.get();
	} while (enter_again);
}
int main()
{

	/*	std::string line = "git commit -m \"first commit\"";
		parse_return argument = parse(line.c_str());
		exec(argument);
		*/
	command_input();
	return 0;
}


//before separaeting file
/*
#include <iostream>
#include <string.h>
#include<string>
#include<cctype>

#include<unordered_map>
#include<map>
#include<vector>
#include<cstring>


#include "Parser.hpp"


//dsa
class CommitNode {
public:
	int commit_id = 0;
	std::string commit_name;

	std::unordered_map<std::string, CommitNode*> forward;
	std::unordered_map<std::string, CommitNode*> backward;
};

std::unordered_map< std::string, CommitNode*> branches_status;
std::string current_branch, prev_branch;

std::map<int, CommitNode*> commit_id_map;
//std::map<int, std::string> commit_id_branchmap;//rm vec
std::vector<std::string> commitid_branch;

bool recent_branch_change = false;

CommitNode* pre = NULL;
//CommitNode* next = NULL;
CommitNode* head = NULL;//main head before checkout
CommitNode* detached_head = NULL;//after
int commit_id_val = 0;

void init_command(parse_return parse_value)
{
	current_branch = "MASTER";
	branches_status[current_branch] = nullptr;
}
void commit(parse_return parse_value)
{
	CommitNode* node = new CommitNode();
	node->commit_name = parse_value.msg;
	node->commit_id = commit_id_val;
	commit_id_val++;
	if (detached_head!=NULL && detached_head->forward[current_branch] != NULL)
	{
		prev_branch = current_branch;
		recent_branch_change = true;
		current_branch += "-child";//making a new child branch
	}
	if (pre != NULL)
	{
		pre->forward[current_branch] = node;
	}
	//if to make new node when branch command is executed ig
	if (recent_branch_change)
	{
		node->backward[prev_branch] = pre;//makes a new commit where it links with the previous branch of node
		recent_branch_change = false;
	}
	node->forward[current_branch] = nullptr;
	node->backward[current_branch] = pre;

	branches_status[current_branch] = node;
	pre = node;
	commit_id_map[node->commit_id] = node;
	commitid_branch.push_back(current_branch);
	//condition for commiting in the middle of already exisiting branch remaingn
	//output

}
void branch(parse_return parse_value)
{
	//recent_branch_change = true;
	//prev_branch = current_branch;
	current_branch = parse_value.msg;
	branches_status[current_branch]=nullptr;
}
void checkout(parse_return parse_value)
{
	bool found = false;
	head = pre;
	//for commit_id
	std::map<int, CommitNode*> ::iterator iter;
	for (iter = commit_id_map.begin(); iter != commit_id_map.end(); iter++)
	{
		if (std::to_string(iter->first) == (parse_value.msg))
		{
			detached_head = iter->second;
			found = true;
			//current_branch = commit_id_branchmap[iter->first];
			break;
		}
	}
	//for branch
	if (!found) {
		std::unordered_map< std::string, CommitNode*> ::iterator iter2;
		for (iter2 = branches_status.begin(); iter2 != branches_status.end(); iter2++)
		{
			if (iter2->first == parse_value.msg)
			{
				detached_head = iter2->second;
				found = true;
				current_branch = iter2->first;
				break;
			}
		}
	}
	pre = detached_head;
	//branch -b remaingin

	if (!found)
	{
		std::cout << "The commit id or branchname is invalid" << std::endl;
		return;
	}
}
void merge_commit(std::string branch_mergeto)
{
	if (pre == NULL)
	{
		std::cout << "You tried to merge early" << std::endl;
		return;
	}
	CommitNode* traverse;
	traverse = branches_status[current_branch];
	while ( traverse != NULL)
	{
		if (traverse->forward[current_branch] == branches_status[branch_mergeto])
		{
			branches_status[current_branch] = branches_status[branch_mergeto];
			std::cout << "fast forward merging" << std::endl;
			return;
		}
		traverse = traverse->forward[current_branch];
	}

	CommitNode* node = new CommitNode;
	node->commit_id = commit_id_val;
	commit_id_val++;



	node->forward[current_branch] = nullptr;
	node->backward[current_branch] = branches_status[current_branch];
	node->backward[current_branch] = branches_status[branch_mergeto];



	branches_status[branch_mergeto] = node;
	branches_status[current_branch] = node;

	pre = node;
	commit_id_map[node->commit_id]= node;
}
void merge(parse_return parse_value)
{
	std::unordered_map< std::string, CommitNode*> ::iterator iter;
	for (iter = branches_status.begin(); iter != branches_status.end(); iter++)
	{
		if (iter->first == parse_value.msg && iter->first != current_branch)
		{
			merge_commit(parse_value.msg);
		}

	}
}
void exec(parse_return argument)
{
	if (!argument.validity) return;

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
void command_input()
{
	bool enter_again = true;
	std::string command_line;
	do {
		std::cout << "Enter Command:";
		std::getline(std::cin, command_line);
		struct parse_return argument = parse(command_line.c_str());
		exec(argument);
		std::cout << std::endl;
		std::cout << "Do you want to enter another command?" << std::endl;
		std::cin >> enter_again;
		std::cin.get();
	} while (enter_again);
}
int main()
{

	/*std::string line = "git commit -m \"first commit\"";
		parse_return argument = parse(line.c_str());
		exec(argument);
		*
command_input();
return 0;
}
*/
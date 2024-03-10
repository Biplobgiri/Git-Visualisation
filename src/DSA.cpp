#include"DSA.hpp"
#include<iostream>
#include<time.h>
#include"guiLogic.hpp"

extern 	std::map<int, int>commitid_circleid;


std::unordered_map< std::string, CommitNode*> branches_status;
std::string current_branch, prev_branch;
std::vector<int> commitidVector;
std::map<int, CommitNode*> commit_id_map;
//std::map<int, std::string> commit_id_branchmap;//rm vec
std::vector<std::string> commitid_branch;
int checkedoutid;

bool recent_branch_change = false;
bool isCheckbyid = false;

CommitNode* pre = NULL;
CommitNode* head = NULL;//main head before checkout
CommitNode* detached_head = NULL;//after
int commit_id_val = 0;

Token_Type what_executed=ERROR;



void init_command(parse_return parse_value)
{
	if (!branches_status.empty())
	{
		std::cout << "git already inited" << std::endl;
		return;
	}
	current_branch = "MASTER";
	branches_status[current_branch] = nullptr;
	what_executed = parse_value.command;
}
void commit(parse_return parse_value)
{
	CommitNode* node = new CommitNode();
	node->commit_name = parse_value.msg;

	node->commit_id = commit_id_val;
	commit_id_val++;
	

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

	commitidVector.push_back(node->commit_id);
	what_executed = parse_value.command;

	std::cout << "Commited" << std::endl;
	
	

}
void branch(parse_return parse_value)
{

	current_branch = parse_value.msg;
	branches_status[current_branch] = nullptr;
	what_executed = parse_value.command;

	std::cout << "Branched" << std::endl;


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
			checkedoutid = iter->first;
			//current_branch = commit_id_branchmap[iter->first];
			isCheckbyid = true;
			
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
				isCheckbyid = false;
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
	std::cout << "Checked Out" << std::endl;
	what_executed = parse_value.command;

}
void merge_commit(std::string branch_mergeto)
{
	//commit_id_val = rand_gen();
	if (pre == NULL)
	{
		std::cout << "You tried to merge early" << std::endl;
		return;
	}
	std::cout << "Merged" << std::endl;

	CommitNode* traverse;
	traverse = branches_status[current_branch];
	while (traverse != NULL)
	{
		if (traverse->forward[current_branch] == branches_status[branch_mergeto])
		{
			branches_status[current_branch] = branches_status[branch_mergeto];
			std::cout << "fast forward merging" << std::endl;
			return;
		}
		traverse = traverse->forward[current_branch];
	}

	//prev_branch = current_branch;
	//current_branch = branch_mergeto;

	CommitNode* node = new CommitNode;
	node->commit_id = commit_id_val;
	commit_id_val++;



	node->forward[current_branch] = nullptr;
	node->backward[current_branch] = branches_status[current_branch];
	node->backward[current_branch] = branches_status[branch_mergeto];

	branches_status[branch_mergeto] = node;
	branches_status[current_branch] = node;

	pre = node;
	commit_id_map[node->commit_id] = node;

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
	what_executed = parse_value.command;


}
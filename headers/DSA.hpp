#pragma once
#include"Parser.hpp"

#include<unordered_map>
#include<map>
#include<vector>
#include<cstring>


class CommitNode {
public:
	int commit_id = 0;
	std::string commit_name;

	std::unordered_map<std::string, CommitNode*> forward;
	std::unordered_map<std::string, CommitNode*> backward;
};




void init_command(parse_return parse_value);
void commit(parse_return parse_value);
void branch(parse_return parse_value);
void checkout(parse_return parse_value);
void merge_commit(std::string branch_mergeto);
void merge(parse_return parse_value);




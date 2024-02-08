#include<iostream>
#include<vector>
#include<string>

int main()
{
	char command[100];
	std::vector<std::string> aList;
	std::string word[10];
	std::cout << "Enter the string " << std::endl;
	std::cin.get(command,100);
	int len = strlen(command);
	int i = 0;
	for(int j=0;j<len;j++)
	{
		if (command[j] != ' ')
		{
			word[i] += command[j];
		}
		else
			i++;
	}
	for (int k = 0; k <= i; k++)
	{
		aList.push_back(word[k]);
	}
	for (std::string ind:aList)
	{
		std::cout << ind<< std::endl;
	}
	parse(aList);
	system("pause");
	return 0;
}

void parse(std::vector<std::string> command )
{
	std::vector<std::string> branches;
	bool isCommit, isBranch;
	std::string commit_message,branch_name,current_branch;
	//checking the validity of the command 
	if (command.front() != "git")
	{
		std::cout << "The command is not a git command" << std::endl;
		return ;
	}
	//for the commit command
	if (command[1] == "commit")
	{
		isCommit = true;

		if (command[2] == "-m")
		{
			commit_message = command[3];
		}
		else {
			commit_message = "\0";
		}
	}
	//for the branching command
	if (command[1] == "branch")
	{
		isBranch = true;
		branch_name = command[2];
		branches.push_back(branch_name);
	}
	//for new branching using checkout
	if (command[1] == "checkout" && command[2] == "-b")
	{
		isBranch = true;
		branch_name = command[3];
		branches.push_back(branch_name);
	}
	//for checkout
	if (command[1] == "checkout")
	{
		auto it = std::find(branches.begin(), branches.end(), command[2]);
		if (it != branches.end())
		{
			current_branch = command[2];
		}
	}
}
//dsa portion; creating the nodes and trees
class dsa {
public:
	void ran()
	{
		int a;
		
	}
};


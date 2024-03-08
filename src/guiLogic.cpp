#include"GUILogic.hpp"
#include"DSA.hpp"
#include"Input.hpp"
#include<iostream>
#include<unordered_map>
#include<map>
#include<cmath>

bool isNegDist = false;

int circle_count, line_count=0;
float dist_norm= 20.f, dist_check=200;
bool isChecked_out;
extern bool isCheckbyid;
float window_sizeY=800;



extern std::string current_branch;
extern int checkedoutid;
extern std::unordered_map< std::string, CommitNode*> branches_status;
extern Token_Type what_executed;
bool branch_endpoint = false;

struct circle_information {
	CommitNode* node;
	sf::CircleShape* circle;
	int circle_id;
	std::string branch_information;
	sf::Vector2f circlepos_struct;
	circle_information()
	{
		circle_id = 0;
		branch_information = "";
		node = nullptr;

	}
};

struct branchEndpoint {
	circle_information cirinfoEndpoint;
	std::string branchName;
	branchEndpoint() {
		cirinfoEndpoint.circle_id = -1;
	}
};
branchEndpoint EndpointInfo[3];

static struct circle_information* previous_circle=new struct circle_information;


struct circle_information* checkedOut_circle = new struct circle_information;


std::vector<circle_information> vecCircle_information;


Shapesdraw::Shapesdraw()
{
	window.create({ 800, 800 }, "Main window");
	window.setFramerateLimit(60);
	previous_circle->circlepos_struct = { 0,window_sizeY / 2 };
	previous_circle->circle = new sf::CircleShape;
	previous_circle->circle->setPosition(previous_circle->circlepos_struct);

}
Shapesdraw::~Shapesdraw()
{

}
void Shapesdraw::mainLoop()
{
	while (this->window.isOpen())
	{
		//sf::Event event;
		while (this->window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				this->window.close();
			}
		}

		updating_inputs();
		window.clear(sf::Color::Green);

		for (int i = 0; i < line_count; i++)
		{
			window.draw(line[i]);
		}
		
		for (auto& iter : vecCircle_information)
		{
			window.draw(*iter.circle);
		}


		window.display();
	}

}

void Shapesdraw::updating_inputs()
{
	parse_return command_values;
	std::string input_terminal;
	//input_terminal = userInput;
	std::getline(std::cin, input_terminal);
	command_values = command_input(input_terminal);
	std::cout << "-----------------" << what_executed << "------------------------" << std::endl;

	switch (what_executed)
	{
	case INIT:

		Shapesdraw::initDraw();
		break;
	case COMMIT:
		Shapesdraw::commitDraw(branches_status[current_branch]);
		break;
	case BRANCH:
		//Shapesdraw::branchDraw();
		break;
	case MERGE:
		Shapesdraw::mergeDraw(command_values);
		break;
	case CHECKOUT:
		Shapesdraw::checkoutDraw(command_values);
		break;
	default:
		break;

	}
	what_executed = ERROR;
}


void Shapesdraw::initDraw()
{/*
	this->line_position[0] = { 0, (float)window_sizeY / 2 };
	this->line[0].setPosition(line_position[0]);
	this->line[0].setSize(sf::Vector2f(40, 10));
	this->line[0].setFillColor(sf::Color::Red);
	this->cir_position[0].x = line_position[0].x + line[0].getSize().x;
	this->cir_position[0].y = line_position[0].y + line[0].getSize().y;*/


	sf::Vector2f o= { 0-cir_radius,400};
	sf::Vector2f n = previous_circle->circlepos_struct + sf::Vector2f{cir_radius, cir_radius};
	drawLine(o,n);
	std::cout << "initdraw bhayo" << std::endl;

}



void Shapesdraw::drawNormalCommit(sf::Vector2f cir_position, CommitNode* node)
{
	this->circle[node->commit_id].setRadius(cir_radius);
	this->circle[node->commit_id].setPosition(cir_position);
	this->circle[node->commit_id].setFillColor(sf::Color::Red);

	drawLine(previous_circle->circlepos_struct, cir_position);
}




void Shapesdraw::commitDraw(CommitNode* node)
{
	circle_information temp;
	std::cout << "branch==" << current_branch << std::endl;
	if (branch_endpoint)
	{
		this->cir_position = { checkedOut_circle->circlepos_struct.x + dist_norm, checkedOut_circle->circlepos_struct.y };
		std::cout << "inside of true commit" << std::endl;
		dist_norm = 160;
	}
	if (!isChecked_out)
	{
		this->cir_position = { previous_circle->circlepos_struct.x + dist_norm, previous_circle->circlepos_struct.y };
		std::cout << "inside of normal commit"<< std::endl;
		dist_norm = 160;
	}
	if (!branch_endpoint){
		if (isChecked_out)
		{
			this->cir_position = { checkedOut_circle->circlepos_struct.x , checkedOut_circle->circlepos_struct.y - dist_check };
			previous_circle = checkedOut_circle;
			isChecked_out = false;
			std::cout << "inside of not branch commit" << std::endl;
		}

}


	
	drawNormalCommit(cir_position,node);


	temp.node = node;
	temp.circle_id = node->commit_id;
	temp.branch_information = current_branch;
	temp.circle = &circle[node->commit_id];
	temp.circlepos_struct = this->cir_position;
	vecCircle_information.push_back(temp);
	*previous_circle = temp;
	if (current_branch == "MASTER")
	{
		EndpointInfo[0].branchName = temp.branch_information;
		EndpointInfo[0].cirinfoEndpoint = temp;
		return;
	}
	else {
		EndpointInfo[1].branchName = temp.branch_information;
		EndpointInfo[1].cirinfoEndpoint = temp;
		return;
	}


}
void Shapesdraw::checkoutDrawbyID(int checked_out_id)
{
	if (!branch_endpoint) {

		for (auto& iter : vecCircle_information)
		{
			if (iter.circle_id == checked_out_id)
			{
				*checkedOut_circle = iter;
				std::cout << "checkoutdrawbyid not" << std::endl;
			}
		}
	}

	if (branch_endpoint) {
		for (auto& iter : vecCircle_information)
		{
			if (iter.circle_id == checked_out_id)
			{
				*checkedOut_circle = iter;
				std::cout << "checkoutdrawbyid yes" << std::endl;

			}
		}
		isChecked_out = false;
		*previous_circle = *checkedOut_circle;
	}






}
void Shapesdraw::checkoutDrawbyBranch(std::string checkedout_branchName)
{
	CommitNode* temp;
#ifndef branch_endpoint
	for (auto& iter : branches_status)
	{
		if (iter.first == checkedout_branchName)
		{
			temp = iter.second;
			for (auto& iter : vecCircle_information)
			{
				if (iter.node == temp)
				{
					*checkedOut_circle = iter;
				}
			}
			return;

	}
}

#endif // !branch_endpoint


#ifdef branch_endpoint
	for (auto& iter : branches_status)
	{
		if (iter.first == checkedout_branchName)
		{
			temp = iter.second;
			for (auto& iter : vecCircle_information)
			{
				if (iter.node == temp)
				{
					*checkedOut_circle = iter;
					*previous_circle = *checkedOut_circle;
					isChecked_out = false;
					return;

				}
			}
			

		}
	}

#endif // !branch_endpoint


	
}
void Shapesdraw::checkoutDraw(parse_return command_value)
{
	isChecked_out = true;
	branch_endpoint = false;
	int commitedCircleID = stoi(command_value.msg);


	if (isCheckbyid)
	{
		for (int i=0;i<3;i++)
		{
			if (commitedCircleID == EndpointInfo[i].cirinfoEndpoint.circle_id)
			{
				std::cout << "loopvitra" << std::endl;

				branch_endpoint = true;
			}
		}

		checkoutDrawbyID(commitedCircleID);
	}
	else
	{
		for (auto& iter : EndpointInfo)
		{
			if (command_value.msg == iter.cirinfoEndpoint.branch_information)
			{
				branch_endpoint = true;
			}
		}
		checkoutDrawbyBranch(command_value.msg);
	}
}
void Shapesdraw::drawLineMerge(sf::Vector2f point1, sf::Vector2f point2) {

	point1 = point1 + sf::Vector2f(cir_radius, cir_radius);
	point2 = point2 + sf::Vector2f(cir_radius, cir_radius);

	// Calculate the dimensions of the rectangle

	float width = (point2.x - point1.x);
	float height = (point2.y - point1.y);

	if (point2.x == point1.x)
	{
		width = 5.0;

	}
	else if (point2.y == point2.y)
	{
		height = 5.0;
	}


	// Determine the position of the top-left corner of the rectangle
	float left = std::min(point1.x, point2.x);
	float top = std::min(point1.y, point2.y);

	// Create rectangle shape
	sf::RectangleShape rectangle(sf::Vector2f(width, height));
	rectangle.setPosition(left, top);
	rectangle.setFillColor(sf::Color::Yellow);
	rectangle.setOutlineColor(sf::Color::Yellow);
	rectangle.setOutlineThickness(2);
	

	line[line_count] = rectangle;
	line_count++;

}


void Shapesdraw::drawLine( sf::Vector2f point1, sf::Vector2f point2) {

	point1 = point1 + sf::Vector2f(cir_radius, cir_radius);
	point2 = point2 + sf::Vector2f(cir_radius, cir_radius);

	// Calculate the dimensions of the rectangle

		float width = abs(point2.x - point1.x);
		float height = abs(point2.y - point1.y);

		if (point2.x == point1.x)
		{
			width = 5.0;
			
		}
		else if(point2.y == point2.y)
		{
			height = 5.0;
		}


	// Determine the position of the top-left corner of the rectangle
	float left = std::min(point1.x, point2.x);
	float top = std::min(point1.y, point2.y);

	// Create rectangle shape
	sf::RectangleShape rectangle(sf::Vector2f(width, height));
	rectangle.setPosition(left, top);
	rectangle.setFillColor(sf::Color::White);
	rectangle.setOutlineColor(sf::Color::White);
	rectangle.setOutlineThickness(2);

	line[line_count] = rectangle;
	line_count++;

}
void Shapesdraw::mergeCommit(sf::Vector2f destination, sf::Vector2f current,std::string mergeTobranch)
{
	circle_information temp;
	float distance = 160;
	CommitNode* node = branches_status[mergeTobranch];

	this->cir_position = { destination.x +distance , (destination.y) };
	std::cout << "inside of true commit" << std::endl;
	dist_norm = 160;

	//drawNormalCommit(cir_position, node);
	this->circle[node->commit_id].setRadius(cir_radius);
	this->circle[node->commit_id].setPosition(cir_position);
	this->circle[node->commit_id].setFillColor(sf::Color::Red);


	drawLine(cir_position, destination);

	drawLineMerge(current, current + sf::Vector2f{this->cir_position.x - current.x, current.y});
	drawLineMerge({cir_position.x, current.y},cir_position );




	temp.node = node;
	temp.circle_id = node->commit_id;
	temp.branch_information = current_branch;
	temp.circle = &circle[node->commit_id];
	temp.circlepos_struct = this->cir_position;
	vecCircle_information.push_back(temp);
	*previous_circle = temp;
	
}


void Shapesdraw::mergeDraw(parse_return command_value)
{
	circle_information currentCircle, destinationCircle;
	currentCircle = *previous_circle;

	for (auto& iter : EndpointInfo)
	{
		if (iter.branchName == command_value.msg)
		{
			destinationCircle = iter.cirinfoEndpoint;
		}
	}
	mergeCommit(destinationCircle.circlepos_struct,currentCircle.circlepos_struct,command_value.msg);


}
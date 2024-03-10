#include"GUILogic.hpp"
#include"DSA.hpp"
#include"Input.hpp"
#include<iostream>
#include<unordered_map>
#include<map>
#include<cmath>

bool isNegDist = false;

int circle_count=0, line_count=0;
float dist_norm= 20.f, dist_check=200;
bool isChecked_out;
extern bool isCheckbyid;
float window_sizeY=1000;
sf::Font font;



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
	sf::Text text1,text2;
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
	window.create({ 1200, 1000 }, "Main window");
	window.setPosition({ 20, 0 });
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
	sf::Texture texture;
	texture.loadFromFile("bg.jpg");
	sf::Sprite sprite;
	sf::Vector2u size = texture.getSize();
	sprite.setTexture(texture);
	sprite.setOrigin(size.x / 2, size.y / 2);

	sf::Texture logo;
	logo.loadFromFile("logo.png");
	sf::Sprite spriteLogo;
	
	sf::Vector2u sizee = logo.getSize();
	spriteLogo.setTexture(logo);
	spriteLogo.setOrigin(sizee.x / 2, sizee.y / 2);
	spriteLogo.setPosition(100, 100);
	spriteLogo.scale(.25f,.25f);

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
		window.clear();
		window.draw(sprite);
		window.draw(spriteLogo);
		for (int i = 0; i < line_count; i++)
		{
			window.draw(line[i]);
		}
		
		for (auto& iter : vecCircle_information)
		{
			window.draw(*iter.circle);
			window.draw(iter.text1);
			window.draw(iter.text2);
		}



		window.display();
	}

}

void Shapesdraw::updating_inputs()
{
	parse_return command_values;
	std::string input_terminal;
	//input_terminal = userInput;
	std::cout << "*" << std::endl;
	std::getline(std::cin, input_terminal);
	
	
	command_values = command_input(input_terminal);

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
	std::cout << std::endl;
	what_executed = ERROR;
}


void Shapesdraw::initDraw()
{


	sf::Vector2f o= { 0-cir_radius,window_sizeY/2};
	sf::Vector2f n = previous_circle->circlepos_struct + sf::Vector2f{cir_radius, cir_radius};
	drawLine(o,n);

}



void Shapesdraw::drawNormalCommit(sf::Vector2f cir_position, CommitNode* node)
{
	this->circle[node->commit_id].setRadius(cir_radius);
	this->circle[node->commit_id].setPosition(cir_position);
	
	this->circle[node->commit_id].setFillColor(sf::Color::Color(191,67,48,255));


	drawLine(previous_circle->circlepos_struct, cir_position);
}




void Shapesdraw::commitDraw(CommitNode* node)
{

	circle_information temp;
	if (branch_endpoint)
	{
		this->cir_position = { checkedOut_circle->circlepos_struct.x + dist_norm, checkedOut_circle->circlepos_struct.y };
		dist_norm = 160;
	}
	if (!isChecked_out)
	{
		this->cir_position = { previous_circle->circlepos_struct.x + dist_norm, previous_circle->circlepos_struct.y };
		dist_norm = 160;
	}
	if (!branch_endpoint){
		if (isChecked_out)
		{
			this->cir_position = { checkedOut_circle->circlepos_struct.x , checkedOut_circle->circlepos_struct.y - dist_check };
			previous_circle = checkedOut_circle;
			isChecked_out = false;
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
		
	}
	else {
		EndpointInfo[1].branchName = temp.branch_information;
		EndpointInfo[1].cirinfoEndpoint = temp;
		
	}
	textDraw();


}
void Shapesdraw::checkoutDrawbyID(int checked_out_id)
{
	if (!branch_endpoint) {

		for (auto& iter : vecCircle_information)
		{
			if (iter.circle_id == checked_out_id)
			{
				*checkedOut_circle = iter;
				//std::cout << "checkoutdrawbyid not" << std::endl;
			}
		}
	}

	if (branch_endpoint) {
		for (auto& iter : vecCircle_information)
		{
			if (iter.circle_id == checked_out_id)
			{
				*checkedOut_circle = iter;
				//std::cout << "checkoutdrawbyid yes" << std::endl;

			}
		}
		isChecked_out = false;
		*previous_circle = *checkedOut_circle;
	}






}
void Shapesdraw::checkoutDrawbyBranch(std::string checkedout_branchName)
{
	CommitNode* temp;

	if (!branch_endpoint) {
	for (auto& iter : branches_status)
	{
		if (iter.first == checkedout_branchName)
		{
			temp = iter.second;
			for (auto& iterp : vecCircle_information)
			{
				if (iterp.node == temp)
				{
					*checkedOut_circle = iterp;
				}
			}
			return;

			}
		}

	}


	if (branch_endpoint) {
		for (auto& iter : branches_status)
		{
			if (iter.first == checkedout_branchName)
			{
				temp = iter.second;
				for (auto& iterp : vecCircle_information)
				{
					if (iterp.node == temp)
					{
						*checkedOut_circle = iterp;
						*previous_circle = *checkedOut_circle;
						
						return;

					}
				}


			}
		}

	}


	
}
void Shapesdraw::checkoutDraw(parse_return command_value)
{
	isChecked_out = true;
	branch_endpoint = false;
	int commitedCircleID;
	if(isCheckbyid)
		 commitedCircleID = stoi(command_value.msg);


	if (isCheckbyid)
	{
		for (int i=0;i<3;i++)
		{
			if (commitedCircleID == EndpointInfo[i].cirinfoEndpoint.circle_id)
			{

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
	rectangle.setFillColor(sf::Color::Color(172, 191, 48, 250));
	rectangle.setOutlineColor(sf::Color::Color(172, 191, 48,250));
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


	float left = std::min(point1.x, point2.x);
	float top = std::min(point1.y, point2.y);

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
	dist_norm = 160;

	//drawNormalCommit(cir_position, node);
	this->circle[node->commit_id].setRadius(cir_radius);
	this->circle[node->commit_id].setPosition(cir_position);
	this->circle[node->commit_id].setFillColor(sf::Color::Color(191, 67, 48, 255));


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

void Shapesdraw::textDraw()
{
	if (vecCircle_information.empty())
	{
		return;
	}
	if (!font.loadFromFile("arial.ttf")) {
		// Handle font loading error
		return;
	}

	for (auto& iter : vecCircle_information)
	{
		iter.text1.setString("");
		for (auto& iter2 : EndpointInfo)
		{
			if (iter2.cirinfoEndpoint.circle_id == iter.circle_id)
			{
				iter.text1.setFont(font);
				iter.text1.setString(iter.branch_information);
				iter.text1.setCharacterSize(24);
				iter.text1.setFillColor(sf::Color::White);
				iter.text1.setPosition(iter.circlepos_struct.x+cir_radius/4,
				iter.circlepos_struct.y + cir_radius-8 );
			}
		}
 // Position text beneath the circle

		iter.text2.setFont(font);
		iter.text2.setString(iter.node->commit_name + "\n" + std::to_string(iter.circle_id));
		iter.text2.setCharacterSize(24);
		iter.text2.setFillColor(sf::Color::White);
		iter.text2.setPosition(iter.circlepos_struct.x+ cir_radius / 3,
		iter.circlepos_struct.y + cir_radius * 2 +2); // Position text beneath the circle

	}

}

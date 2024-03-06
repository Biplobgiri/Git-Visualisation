#include"guiLogic.hpp"
#include"DSA.hpp"
#include"Input.hpp"
#include<iostream>
#include<unordered_map>
#include<map>
#include<cmath>


	int commit_count = 0;
	int line_count = 0;
	extern Token_Type what_executed;
	extern std::map<int, CommitNode*> commit_id_map;
	extern std::vector<int> commitidVector;
	extern std::unordered_map< std::string, CommitNode*> branches_status;
	extern int commit_id_val;
	std::map< sf::CircleShape*,int> circle_commitid;
	extern std::string current_branch;
	bool recent_checkout;
	extern CommitNode* detached_head;
	sf::CircleShape checkedout_circle;
	sf::Font font;
	struct cir_info {
		std::string branch;
		sf::CircleShape branch_circle;
		sf::Text text;
	}tempcir_info;
	std::vector<cir_info> circle_infos;





	
Shapesdraw::Shapesdraw()
{
	window.create({ 800, 800 }, "Main window");
	window.setFramerateLimit(60);


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
		window.clear(sf::Color::Black);
		for (int i = 0; i < commit_count; i++)
		{
			window.draw(circle[i]);
	
		}
		for (int i = 0; i < line_count; i++)
		{
			window.draw(line[i]);
		}
		for (auto &iter : circle_infos)
		{ 
		window.draw(iter.text);
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
		command_values=command_input(input_terminal);
		std::cout << "-----------------"<<what_executed<<"------------------------" << std::endl;
		
		switch (what_executed)
		{
		case INIT:

			Shapesdraw::initDraw();

			break;
		case COMMIT:
			Shapesdraw::commitDraw();
			break;
		case BRANCH:
			//Shapesdraw::branchDraw();
			break;
		case MERGE:
			//Shapesdraw::mergeDraw();
			break;
		case CHECKOUT:
			Shapesdraw::checkoutDraw();
			break;
		default:
			break;

		}
		what_executed = ERROR;
	}
//}

void Shapesdraw::initDraw() {

	this->line_position[0] = { 0, (float)window.getSize().y /2};
	this->line[0].setPosition(line_position[0]);
	this->line[0].setSize(sf::Vector2f(40, 10));
	this->line[0].setFillColor(sf::Color::Red);
	this->cir_center[0].x = line_position[0].x +line[0].getSize().x;
	this->cir_center[0].y = line_position[0].y + line[0].getSize().y;
	std::cout << "initdraw bhayo" << std::endl;
	
	line_count++;
}
void Shapesdraw::commitDraw()
{
	if (recent_checkout==true )
	{
		/*for (auto& iter : detached_head->forward)
		{
			if (iter.second == NULL)
			{
			recent_checkout = false;
			int tempCommitcount = detached_head->commit_id;
			int templineCount = tempCommitcount + 1;
			
			this->circle[commit_count].setRadius(cir_radius);
			this->cir_center[commit_count].x = this->line[templineCount].getPosition().x + this->line[templineCount].getSize().x;// +circle[commit_count].getRadius();
			this->cir_center[commit_count].y = this->line[templineCount].getPosition().y - this->circle[commit_count].getRadius();
			this->circle[commit_count].setPosition(cir_center[commit_count]);
			this->circle[commit_count].setFillColor(sf::Color::Green);

			this->line_position[line_count].x = this->cir_center[commit_count].x + 2 * cir_radius;
			this->line_position[line_count].y = this->cir_center[commit_count].y + cir_radius;

			this->line[line_count].setPosition(line_position[line_count]);
			this->line[line_count].setSize(sf::Vector2f(40, 10));
			this->line[line_count].setFillColor(sf::Color::Red);

			tempcir_info.branch_circle = circle[commit_count];
			tempcir_info.branch = current_branch;


			for (auto& iter : circle_infos)
			{
				if (iter.branch == tempcir_info.branch)
					iter = tempcir_info;
			}
			circle_infos.push_back(tempcir_info);



			circle_infos.push_back(tempcir_info);

			guiCommitid_shape[commitidVector.back()] = &circle[commit_count];

			textDraw();


			std::cout << "commit draw bhayo" << std::endl;
			commit_count++;
			line_count++;

			}
			return;
		}
		*/
		
		this->circle[commit_count].setRadius(cir_radius);
		this->circle[commit_count].setFillColor(sf::Color::Green);
		this->cir_center[commit_count].x = checkedout_circle.getPosition().x;
		this->cir_center[commit_count].y = checkedout_circle.getPosition().y - 6 * cir_radius;
		circle[commit_count].setPosition(cir_center[commit_count]);
		std::cout << "checmout commit" << std::endl;

		this->line_position[line_count].x = cir_center[commit_count].x + cir_radius;
		this->line_position[line_count].y = cir_center[commit_count].y + 2 * cir_radius;
		this->line[line_count].setPosition(line_position[line_count]);
		this->line[line_count].setSize(sf::Vector2f(10, 4*cir_radius));
		this->line[line_count].setFillColor(sf::Color::Yellow);

		line_count++;

		this->line_position[line_count].x = this->cir_center[commit_count].x + 2 * cir_radius;
		this->line_position[line_count].y = this->cir_center[commit_count].y + cir_radius;

		this->line[line_count].setPosition(line_position[line_count]);
		this->line[line_count].setSize(sf::Vector2f(40, 10));
		this->line[line_count].setFillColor(sf::Color::Red);

		tempcir_info.branch_circle = circle[commit_count];
		tempcir_info.branch = current_branch;

		circle_infos.push_back(tempcir_info);
		
		guiCommitid_shape[commitidVector.back()] = &circle[commit_count];

		circle_commitid[&circle[commit_count]] = commit_id_val;

		circle_infos.push_back(tempcir_info);

		textDraw();


		std::cout << "checkout draw bhayo" << std::endl;
		commit_count++;
		line_count++;
		recent_checkout = false;
		
		return;

	}
	

	this->circle[commit_count].setRadius(cir_radius);
	this->cir_center[commit_count].x = this->line[line_count - 1].getPosition().x + this->line[line_count - 1].getSize().x;// +circle[commit_count].getRadius();
	this->cir_center[commit_count].y = this->line[line_count - 1].getPosition().y-this->circle[commit_count].getRadius();
	this->circle[commit_count].setPosition(cir_center[commit_count]);
	this->circle[commit_count].setFillColor(sf::Color::Green);

	this->line_position[line_count].x = this->cir_center[commit_count].x + 2 * cir_radius;
	this->line_position[line_count].y = this->cir_center[commit_count].y + cir_radius;

	this->line[line_count].setPosition(line_position[line_count]);
	this->line[line_count].setSize(sf::Vector2f(40, 10));
	this->line[line_count].setFillColor(sf::Color::Red);

	tempcir_info.branch_circle = circle[commit_count];
	tempcir_info.branch = current_branch;


		for (auto &iter : circle_infos)
		{
			if (iter.branch == tempcir_info.branch)
				iter = tempcir_info;
		}
		circle_infos.push_back(tempcir_info);



	circle_infos.push_back(tempcir_info);

	guiCommitid_shape[commitidVector.back()] = &circle[commit_count];

	textDraw();


	std::cout << "commit draw bhayo" << std::endl;
	commit_count++;
	line_count++;
	

}
void Shapesdraw::checkoutDraw()
{

	checkedout_circle = *guiCommitid_shape[detached_head->commit_id];
	recent_checkout = true;



}
void Shapesdraw::textDraw()
{
	if (commitidVector.empty())
	{
		return;
	}
	if (!font.loadFromFile("arial.ttf")) {
		// Handle font loading error
		return ;
	}

	for (auto& iter : circle_infos)
	{
		iter.text.setFont(font);
		iter.text.setString(iter.branch);
		iter.text.setCharacterSize(24);
		iter.text.setFillColor(sf::Color::White);
		iter.text.setPosition(iter.branch_circle.getPosition().x,
			iter.branch_circle.getPosition().y + iter.branch_circle.getRadius() * 2 + 10); // Position text beneath the circle
	}

}






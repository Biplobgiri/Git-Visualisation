#pragma once


#include<SFML/Graphics.hpp>
#include"DSA.hpp"

class Shapesdraw
{
public:


	sf::CircleShape* circle = new sf::CircleShape[100];
	sf::RectangleShape* line = new sf::RectangleShape[100];

	Shapesdraw();
	~Shapesdraw();

	void updating_inputs();
	void mainLoop();
	sf::RenderWindow window;
	void initDraw();
	void commitDraw(CommitNode* node);
	//void branchDraw();
	void checkoutDraw(parse_return);
	void mergeCommit(sf::Vector2f, sf::Vector2f,std::string);

	void checkoutDrawbyID(int);
	void checkoutDrawbyBranch(std::string);

	void drawLine(sf::Vector2f, sf::Vector2f);
	void drawLineMerge(sf::Vector2f, sf::Vector2f);

	void textDraw();
	void mergeDraw(parse_return);
	sf::Event event;
	float cir_posX, cir_posY;

	//sf::Vector2f* cir_position = new sf::Vector2f[100];
	sf::Vector2f cir_position;

	float cir_radius = 50;

	float line_posX, line_posY;
	sf::Vector2f* line_position = new sf::Vector2f[100];

	void drawNormalCommit(sf::Vector2f position, CommitNode* node);
	void drawCheckedoutCommit(sf::Vector2f position, CommitNode* node);

};





#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include<map>
#include"DSA.hpp"

class Shapesdraw
{
public:
	//std::vector<sf::CircleShape> circle;
	//std::vector<sf::RectangleShape> line;
	std::map<int, sf::CircleShape*> guiCommitid_shape;

	sf::CircleShape* circle = new sf::CircleShape[100];
	sf::RectangleShape* line = new sf::RectangleShape[100];
	Shapesdraw();
	~Shapesdraw();
	void updating_inputs();
	void mainLoop();
	sf::RenderWindow window;
	void initDraw();
	void commitDraw();
	void branchDraw();
	void checkoutDraw();
	void textDraw();
	//void mergeDraw();
	sf::Event event;
	float cir_posX, cir_posY;
	sf::Vector2f* cir_position = new sf::Vector2f[100];
	float cir_radius = 50;

	float line_posX, line_posY;
	sf::Vector2f* line_position = new sf::Vector2f[100];

	void drawNormalCommit(sf::Vector2f position, float cir_radius);





};

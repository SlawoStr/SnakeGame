#pragma once
#include <SFML/Graphics.hpp>
#include <list>


class Simulation
{
public:
	Simulation(int gameSize,int gameSpeed);
	~Simulation();
	sf::RenderWindow window;
	void run();
private:
	void draw();
	void drawMesh();
	void pollEvent();
	void update();
	void resetGame();
private:
	int direction;
	int blockedMove;
	std::list<sf::Vector2f> snakeBody;
	bool gameState = true;
	sf::Vector2f fruitLocation;
};
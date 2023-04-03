#include "Simulation.h"
#include <iostream>
#include <stdlib.h>   
#include <time.h>       

Simulation::Simulation(int gameSize,int gameSpeed) : window(sf::VideoMode(gameSize*20,gameSize*20),"Snake-Game",sf::Style::Close | sf::Style::Titlebar)
{
	srand(time(NULL));
	this->direction = 0;
	this->blockedMove = 0;
	window.setFramerateLimit(gameSpeed);
	if (gameSize % 2 == 0)
	{
		snakeBody.push_front(sf::Vector2f((gameSize / 2) * 20, (gameSize / 2) * 20));
	}
	else
	{
		snakeBody.push_front(sf::Vector2f(((gameSize-1) / 2) * 20, ((gameSize-1) / 2) * 20));
	}
	for (;;)
	{
		fruitLocation = sf::Vector2f((rand() % gameSize) * 20, (rand() % gameSize) * 20);
		if (fruitLocation.x != snakeBody.front().x && fruitLocation.y != snakeBody.front().y)
		{
			break;
		}
	}
}

Simulation::~Simulation()
{
}

void Simulation::run()
{
	while (window.isOpen())
	{
		if (gameState)
		{
			window.clear(sf::Color::White);
			draw();
			window.display();
		}
		pollEvent();
	}
}

void Simulation::draw()
{
	bool first = true;
	sf::RectangleShape rect(sf::Vector2f(20.0f, 20.0f));
	for (auto const & i : snakeBody)
	{
		rect.setPosition(i);
		if (first)
		{
			rect.setFillColor(sf::Color::Blue);
			first = false;
		}
		else
		{
			rect.setFillColor(sf::Color::Green);
		}
		window.draw(rect);
	}
	rect.setPosition(fruitLocation);
	rect.setFillColor(sf::Color(199, 21, 133));
	window.draw(rect);
	drawMesh();
}

void Simulation::drawMesh()
{
	sf::Vector2u gameSize = window.getSize();
	gameSize.x /= 20;
	gameSize.y /= 20;

	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(0,0)),
		sf::Vertex(sf::Vector2f(0,0))
	};
	line[0].color = sf::Color::Red;
	line[1].color = sf::Color::Red;

	for (int i = 0; i < gameSize.x+1; i++)
	{
		line[0].position = sf::Vector2f(0, i*20);
		line[1].position = sf::Vector2f(gameSize.x*20, i*20);
		window.draw(line,2,sf::Lines);

		line[0].position = sf::Vector2f(i * 20, 0);
		line[1].position = sf::Vector2f(i * 20, gameSize.y * 20);
		window.draw(line, 2, sf::Lines);
	}
}

void Simulation::pollEvent()
{
	sf::Event e;
	sf::Vector2f mousePos;
	int newDirection=0;

	while (window.pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
		{
			window.close();
		}
		if (e.type == sf::Event::KeyPressed)
		{
			if (e.key.code == sf::Keyboard::Up)
			{
				newDirection = 1;
			}
			else if (e.key.code == sf::Keyboard::Right)
			{

				newDirection = 2;
			}
			else if (e.key.code == sf::Keyboard::Down)
			{
				newDirection = 3;
			}
			else if (e.key.code == sf::Keyboard::Left)
			{
				newDirection = 4;
			}
		}
		if (e.type == sf::Event::MouseButtonPressed && !gameState)
		{
			resetGame();
		}
	}
	if (gameState)
	{
		if (newDirection != blockedMove && newDirection != 0)
		{
			switch (newDirection)
			{
			case 1:
				blockedMove = 3;
				break;
			case 2:
				blockedMove = 4;
				break;
			case 3:
				blockedMove = 1;
				break;
			case 4:
				blockedMove = 2;
				break;
			}
			direction = newDirection;
		}
		if (direction == 0)
			return;
		update();
	}
}

void Simulation::update()
{
	sf::Vector2f listFront = snakeBody.front();
	switch (direction)
	{
	case 1:
		listFront.y = listFront.y - 20;
		break;
	case 2:
		listFront.x = listFront.x + 20;
		break;
	case 3:
		listFront.y = listFront.y + 20;
		break;
	case 4:
		listFront.x = listFront.x - 20;
		break;
	}
	if (listFront.x >= window.getSize().x || listFront.x < 0 || listFront.y >= window.getSize().y || listFront.y < 0)
	{
		gameState = false;
		std::cout << "OUT" << std::endl;
		return;
	}
	bool colisionDetection = false;
	for (auto const & i : snakeBody)
	{
		if (listFront.x == i.x && listFront.y == i.y)
		{
			colisionDetection = true;
			break;
		}
	}

	if (colisionDetection)
	{
		gameState = false;
		std::cout << "Collision" << std::endl;
		return;
	}

	if (listFront.x == fruitLocation.x && listFront.y == fruitLocation.y)
	{
		snakeBody.push_front(listFront);

		for (;;)
		{
			fruitLocation = sf::Vector2f((rand() % (window.getSize().x/20)) *20, (rand() % (window.getSize().y/20)) * 20);
			bool correctPlace = true;
			for (auto const & i : snakeBody)
			{
				if (fruitLocation.x == i.x && fruitLocation.y == i.y)
				{
					correctPlace = false;
				}
			}
			if (correctPlace)
				break;
		}
	}
	else
	{
		snakeBody.push_front(listFront);
		snakeBody.pop_back();
	}
}

void Simulation::resetGame()
{
	while (snakeBody.size() != 0)
		snakeBody.pop_back();
	if ((window.getSize().x/20) % 2 == 0)
	{
		snakeBody.push_front(sf::Vector2f(((window.getSize().x / 20) / 2) * 20, ((window.getSize().x / 20) / 2) * 20));
	}
	else
	{
		snakeBody.push_front(sf::Vector2f((((window.getSize().x / 20) - 1) / 2) * 20, (((window.getSize().x / 20) - 1) / 2) * 20));
	}
	for (;;)
	{
		fruitLocation = sf::Vector2f((rand() % (window.getSize().x / 20)) * 20, (rand() % (window.getSize().x / 20)) * 20);
		if (fruitLocation.x != snakeBody.front().x && fruitLocation.y != snakeBody.front().y)
		{
			break;
		}
	}
	this->direction = 0;
	this->blockedMove = 0;
	gameState = true;
}

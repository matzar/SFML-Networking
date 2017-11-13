//#include <SFML/Graphics.hpp>
#include "Game.h"

void main(int argc, char** argv[])
{
	
	sf::RenderWindow window(sf::VideoMode(800, 600), "Lab 1");// , sf::Style::None);
	//window.setPosition(sf::Vector2i(250, 250));
	//window.setSize(sf::Vector2u(400, 400));
	
	Input input;
	Game game(&window, &input);
	

	//sf::Texture mushroomTexture;
	//mushroomTexture.loadFromFile("Mushroom.png");
	////mushroomTexture.loadFromFile("sfml-logo-small.png");
	//sf::Sprite mushroom(mushroomTexture);
	//sf::Vector2u size = mushroomTexture.getSize();
	//mushroom.setOrigin(size.x / 2, size.y / 2);
	//mushroom.setPosition(50, 50);
	//sf::Vector2f increment(0.1f, 0.1f);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::Resized:
					window.setView(sf::View(sf::FloatRect(0.f, 0.f, (float)event.size.width, (float)event.size.height)));
					break;
				case sf::Event::KeyPressed:
					// update input class
					input.setKeyDown(event.key.code);
					break;
				case sf::Event::KeyReleased:
					//update input class
					input.setKeyUp(event.key.code);
					break;
				case sf::Event::MouseMoved:
					//update input class
					input.setMousePosition(event.mouseMove.x, event.mouseMove.y);
					break;
				case sf::Event::MouseButtonPressed:
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						//update input class
						input.setMouseLeftDown(true);
					}
					break;
				case sf::Event::MouseButtonReleased:
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						//update input class
						input.setMouseLeftDown(false);
					}
					break;
				default:
					// don't handle other events
					break;
			}
		}

		// if space is pressed
		if (input.isKeyDown(sf::Keyboard::Space))
		{
			input.setKeyUp(sf::Keyboard::Space);
			MessageBox(NULL, L"Space was pressed", L"Key press", MB_OK);
		}

		// game loop
		game.handleInput();
		game.update();
		game.render();

		//if ((mushroom.getPosition().x + (size.x / 2) > window.getSize().x && increment.x > 0 || (mushroom.getPosition().x - (size.x / 2)) < 0 && increment.x < 0))
		//{
		//	// Reverse the direction on X axis
		//	increment.x = -increment.x;
		//}
		//if ((mushroom.getPosition().y + (size.y / 2) > window.getSize().y && increment.y > 0 || (mushroom.getPosition().y - (size.y / 2)) < 0 && increment.y < 0))
		//{
		//	// Reverse the direction on Y axis
		//	increment.y = -increment.y;
		//}

		//mushroom.setPosition(mushroom.getPosition() + increment);

		//window.clear(sf::Color(16, 16, 16, 255)); // set background colour to dark grey.
		//window.draw(mushroom);	// render mushroom sprite
		//window.display();	// Swap buffers.
	}

}
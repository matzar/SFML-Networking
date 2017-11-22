#include <iostream>
#include <cstdlib>
#include "GameServer.h"
#include "GameClient.h"
#include "Menu.h"
#include "NetworkSelect.h"
#include "NetworkServer.h"
#include "NetworkClient.h"
#include "NetworkState.h"

void main(int argc, char** argv[])
{
	// NETWORK /////////////////////
	// global socket
	static sf::UdpSocket socket;
	// Choose an arbitrary port for opening sockets
	unsigned short port = 50001; 
	// IP adrress to connect to
	sf::IpAddress ip_address = "127.1.0";
	////////////////////////////////
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML-Networking"); // , sf::Style::None);
	// Set framerate to 60FPS
	window.setFramerateLimit(60);
	//window.setPosition(sf::Vector2i(250, 250));
	//window.setSize(sf::Vector2u(400, 400));
	////////////////////////////////

	// DO NOT RESTART THE CLOCK!
	sf::Clock clock;
	//float deltaTime;
	sf::Int32 offset = 0;

	// set the initial game state
	GameStateEnum game_state = GameStateEnum::MENU;
	
	bool pause = false;
	bool debug_mode = false;
	Input input;

	// state handler
	State* state = nullptr;
	State* menu = new Menu(&window, &input, &game_state);
	State* network = new NetworkSelect(&window, &input, &game_state, &socket, &ip_address, &port);
	State* network_server = new NetworkServer(&window, &input, &game_state, &socket, &ip_address, &port, &clock, &offset);
	State* network_client = new NetworkClient(&window, &input, &game_state, &socket, &ip_address, &port, &clock, &offset);
	State* game_server = new GameServer(&window, &input, &game_state, &socket, &ip_address, &port, &clock, &offset);
	State* game_client = new GameClient(&window, &input, &game_state, &socket, &ip_address, &port, &clock, &offset);
	
	//direction dir = direction::left;

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

		// if escape is pressed
		if (input.isKeyDown(sf::Keyboard::Escape))
		{
			input.setKeyUp(sf::Keyboard::Escape);
			window.close();
		}

		/*if (input.isKeyDown(sf::Keyboard::Pause))
		{
			input.setKeyUp(sf::Keyboard::Pause);
			pause = !pause;

			if (pause)
			{
				state = GameStateEnum::PAUSE;
			}
			else
			{
				switch (state)
				{
				case (GameStateEnum::GAME_CLIENT):
					state = GameStateEnum::GAME_CLIENT;
					break;
				case (GameStateEnum::GAME_SERVER):
					state = GameStateEnum::GAME_SERVER;
					break;
				}
			}
		}*/

		// game loop
		// Update/Render object based on current game state

		// abstract state class to be inherited by the state classes
		// put rendering into state classes
		// put networing into networking classes

		switch (game_state)
		{
		case (GameStateEnum::MENU) :
			state = menu;
			break;

		case (GameStateEnum::NETWORK) :
			state = network;
			break;
		// establish connection with the client
		case (GameStateEnum::NETWORK_SERVER):
			state = network_server;
			break;

		case (GameStateEnum::NETWORK_CLIENT):
			state = network_client;
			break;

		case(GameStateEnum::GAME_SERVER):
			state = game_server;
			break;

		case(GameStateEnum::GAME_CLIENT):
			state = game_client;
			break;

		case(GameStateEnum::PAUSE) :
			switch (game_state)
			{
			case (GameStateEnum::GAME_CLIENT):
				state = game_client;
				break;
			case (GameStateEnum::GAME_SERVER):
				state = game_server;
				break;
			}
			break;
			
		case(GameStateEnum::CREDITS) :
			//...
			break;
		}

		// Play the game
		state->handleInput();
		state->update();
		state->render();
	}
}
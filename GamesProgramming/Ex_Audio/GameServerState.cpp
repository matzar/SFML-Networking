#include "GameServerState.h"
#include <sstream>

GameServerState::GameServerState(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;

	debug_mode = false;
	debug_message = false;

	text.setFont(font);
	text.setCharacterSize(32);
	text.setPosition(10, 0);
	text.setString("");
	
	texture.loadFromFile("gfx/MushroomTrans.png");

	player.setSize(sf::Vector2f(32, 32));
	player.setTexture(&texture);
	player.setPosition(250, 250);
	player.setInput(input);
	//player.setVelocity(5, 5);

	enemy.setSize(sf::Vector2f(32, 32));
	enemy.setTexture(&texture);
	enemy.setPosition(window->getSize().x - 100, window->getSize().y - 100);
	//enemy.setVelocity(3, 3);

	//window->setMouseCursorVisible(false);

	level.loadTexture("gfx/marioTiles.png");
	Tile tile;
	std::vector<Tile> tiles;
	//187, 51
	for (int i = 0; i < 7; i++)
	{

		tile.setSize(sf::Vector2f(32, 32));
		tile.setAlive(true);
		tiles.push_back(tile);
	}

	tiles[0].setAlive(false);
	tiles[0].setTextureRect(sf::IntRect(187, 51, 16, 16));
	tiles[1].setTextureRect(sf::IntRect(0, 0, 16, 16));
	tiles[2].setTextureRect(sf::IntRect(17, 0, 16, 16));
	tiles[3].setTextureRect(sf::IntRect(34, 0, 16, 16));
	tiles[4].setTextureRect(sf::IntRect(0, 34, 16, 16));
	tiles[5].setTextureRect(sf::IntRect(17, 34, 16, 16));
	tiles[6].setTextureRect(sf::IntRect(34, 34, 16, 16));

	level.setTileSet(tiles);

	// Map dimensions
	sf::Vector2u mapSize(10, 6);

	// build map
	/*std::vector<int> map = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 3, 0, 0, 0, 0, 0,
		1, 2, 3, 4, 6, 1, 2, 3, 0, 0,
		4, 5, 6, 0, 0, 4, 5, 6, 0, 0
	};*/

	std::vector<int> map = {
	1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 
	4, 6, 0, 0, 1, 3, 0, 0, 0, 0,
	4, 6, 0, 0, 4, 6, 1, 3, 0, 0,
	4, 6, 1, 3, 4, 6, 4, 6, 0, 0,
	4, 6, 4, 6, 4, 6, 4, 6, 1, 3,
	4, 6, 4, 6, 4, 6, 4, 6, 4, 6,
	};

	level.setTileMap(map, mapSize);
	level.setPosition(sf::Vector2f(0, 408));
	level.buildLevel();
}

GameServerState::~GameServerState() {}

void GameServerState::handleInput()
{
	//The class that provides access to the keyboard state is sf::Keyboard.It only contains one function, isKeyPressed, which checks the current state of a key(pressed or released).It is a static function, so you don't need to instanciate sf::Keyboard to use it.
	//This function directly reads the keyboard state, ignoring the focus state of your window.This means that isKeyPressed may return true even if your window is inactive.

	// toggle debug mode to display socket messages
	if (input->isKeyDown(sf::Keyboard::D))
	{
		input->setKeyUp(sf::Keyboard::D);
		debug_mode = !debug_mode;
	}
	// toggle debug messages to display messages
	if (input->isKeyDown(sf::Keyboard::M))
	{
		input->setKeyUp(sf::Keyboard::M);
		debug_message = !debug_message;
	}

	// toggle lerp mode
	if (input->isKeyDown(sf::Keyboard::L))
	{
		input->setKeyUp(sf::Keyboard::L);
		lerp_mode = !lerp_mode;
	}

	if (input->isKeyDown(sf::Keyboard::P))
	{
		input->setKeyUp(sf::Keyboard::P);
		linear_prediction = !linear_prediction;
		quadratic_prediction = !quadratic_prediction;
	}
}

void GameServerState::render()
{
	beginDraw();

	level.render(window);
	window->draw(player);
	window->draw(enemy);
	window->draw(text);

	endDraw();
}

void GameServerState::beginDraw()
{
	window->clear(sf::Color(0, 0, 0));
}

void GameServerState::endDraw()
{
	window->display();
}

// SERVER //
void GameServerState::establishConnectionWithClient(const bool& debug_mode)
{
	// Wait for a message...
	// Receive the packet at the other end
	// TODO proper address and port
	sf::Packet packet_receive;
	switch (socket.receive(packet_receive, Network::ip_address, Network::port))
	{
	case sf::Socket::Partial:
		return;

	case sf::Socket::Done:
		// Received a packet.
		if (debug_mode) std::cout << "\nCLIENT: Got one!\n";
		//GameServerState::ip_address = Network::ip_address;
		//GameServerState::port = Network::port;
		break;

	case sf::Socket::NotReady:
		// No more data to receive (yet).
		if (debug_mode) std::cout << "\nCLIENT: No more data to receive now\n";
		return;

	case sf::Socket::Disconnected:
		established_connection = false;
		return;

	case sf::Socket::Error:
		return;

	default:
		// Something went wrong.
		if (debug_mode) std::cout << "\nCLIENT: receive didn't return Done\n";
		return;
	}

	///////////////////////////////////////////////////////////////////////////
	// RECEIVE (from the client) MUST MATCH packet_send in the NetworkClient //
	///////////////////////////////////////////////////////////////////////////
	// Extract the variables contained in the packet
	// Deal with the messages from the packet
	bool hello;
	if (packet_receive >> hello)
	{
		// The message from the client
		established_connection = hello;
	}

	/////////////////////////////////////////////////////////////////////////
	// SEND (to the client) MUST MATCH packet_receive in the NetworkClient //
	/////////////////////////////////////////////////////////////////////////
	// Message to send
	sf::Packet packet_to_send;
	// Group the variables to send into a packet
	sf::Int32 server_time = clock.getElapsedTime().asMilliseconds();
	packet_to_send << server_time << established_connection;

	// Send it over the network
	switch (socket.send(packet_to_send, Network::ip_address, Network::port))
	{
	case sf::Socket::Partial:
		while (sf::Socket::Done) { socket.send(packet_to_send, Network::ip_address, Network::port); }
		break;

	case sf::Socket::Done:
		// Received a packet.
		if (debug_mode) std::cout << "\nCLIENT: Got one!\n";
		break;

	case sf::Socket::NotReady:
		// No more data to receive (yet).
		if (debug_mode) std::cout << "\nCLIENT: No more data to receive now\n";
		return;

	case sf::Socket::Disconnected:
		established_connection = false;
		return;

	case sf::Socket::Error:
		return;

	default:
		// Something went wrong.
		if (debug_mode) std::cout << "\nCLIENT: receive didn't return Done\n";
		return;
	}

	/// Extract the variables contained in the packet
}

void GameServerState::update()
{
	//string buffer to convert numbers to string
	std::ostringstream ss; 

	ss << "LERP MODE: " << lerp_mode << " LINEAR PREDICTION: " << linear_prediction << " QUADRATIC PREDICTION: " << quadratic_prediction << "\n"
		<< "IP: " << Network::ip_address.getLocalAddress() << " PORT: " << Network::socket.getLocalPort() << " CLOCK: " << getCurrentTime(clock, offset);

	// display text
	text.setString(ss.str());

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// CHECK FOR NEW CLIENT TO CONNECT
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// establish connection
	if (!established_connection)
	{
		establishConnectionWithClient(debug_mode);
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	player.update();
	enemy.setPosition(lerp(enemy.getPosition(), player.getPosition(), 0.01));

	if (checkSphereBounding(&player, &enemy))
	{
		player.collisionRespone(&enemy);
	}

	// check collision with world
	std::vector<Tile>* world = level.getLevel();
	for (int i = 0; i < (int)world->size(); i++)
	{
		if ((*world)[i].isAlive())
		{
			// if "alive" check collision
			// world tile which are not alive don't want collision checks
			if (checkCollision(&player, &(*world)[i]))
			{
				player.collisionRespone(&(*world)[i]);
			}
		}
	}

	// server should probably keep listening and sending all the time
	runUdpServer(player, enemy, clock, debug_mode);
}


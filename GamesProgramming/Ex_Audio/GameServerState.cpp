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

	// player
	player.setSize(sf::Vector2f(32, 32));
	player.setTexture(&player_texture);
	player.setPosition(250, 250);
	player.setInput(input);
	//player.setVelocity(5, 5);

	// enemy
	enemy.setSize(sf::Vector2f(32, 32));
	enemy.setTexture(&enemy_texture);
	enemy.setPosition(window->getSize().x - 100, window->getSize().y - 100);
	//enemy.setVelocity(3, 3);

	//window->setMouseCursorVisible(false);
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

	//level.render(window);
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

void GameServerState::displayText()
{
	//string buffer to convert numbers to string
	std::ostringstream ss;

	ss << "LERP MODE: " << lerp_mode << " LINEAR PREDICTION: " << linear_prediction << " QUADRATIC PREDICTION: " << quadratic_prediction << "\n"
		<< "IP: " << Network::ip_address.getLocalAddress() << " PORT: " << Network::socket.getLocalPort() << " CLOCK: " << getCurrentTime(clock, offset);

	// display text
	text.setString(ss.str());
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
		// clear the packet if only part of the data was received
		packet_receive.clear();
		break;

	case sf::Socket::Done:
		// TODO keep track of the new sockets GET THE ADDRESS OF THE NEW CLIENT HERE
		// check if the address is new if so...
		// The insertion only takes place if no other element in the container is equivalent to the one being emplaced (elements in a set container are unique).
		//addresses.emplace(Network::port);
		// check if the address is new if so...
		if (addresses.find(Network::port) == addresses.end()) {
			// ...this is an address we've not seen before.
			latestID++;
			printf("New address, giving it ID %d\n", latestID);
			// create a pair
			addresses[Network::port] = latestID;
		}
		// Received a packet.
		if (debug_mode) std::cout << "\nCLIENT: Got one!\n";
		break;

	case sf::Socket::NotReady:
		// No more data to receive (yet).
		if (debug_mode) std::cout << "\nCLIENT: No more data to receive now\n";
		return;

	case sf::Socket::Disconnected:
		if (debug_mode) std::cout << "CLIENT: Disconnected\n";
		return;

	case sf::Socket::Error:
		// Something went wrong.
		if (debug_mode) std::cout << "\nCLIENT: receive didn't return Done\n";
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
	sf::Packet send_packet;
	// Group the variables to send into a packet
	sf::Int32 server_time = clock.getElapsedTime().asMilliseconds();
	send_packet << server_time << established_connection;
	
	// SEND THE SERVER'S TIME TO SYNC THE CLIENT'S AND THE SERVER'C CLOCKS
	for (const auto& pair : addresses)
	{
		const auto& fromAddr = pair.first;
		// Send it over the network
		switch (socket.send(send_packet, Network::ip_address, fromAddr))
		{
		case sf::Socket::Partial:
			// 
			/*
			https://www.sfml-dev.org/tutorials/2.4/network-socket.php
			if only a part of the data was sent in the call, the return status will be sf::Socket::Partial to indicate a partial send.
			If sf::Socket::Partial is returned, you must make sure to handle the partial send properly or else data corruption will occur.
			When sending raw data, you must reattempt sending the raw data at the byte offset where the previous send call stopped.
			*/
			while (socket.send(send_packet, Network::ip_address, Network::port) != sf::Socket::Done) {}
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
			if (debug_mode) std::cout << "CLIENT: Disconnected\n";
			return;

		case sf::Socket::Error:
			// Something went wrong.
			if (debug_mode) std::cout << "\nCLIENT: receive didn't return Done\n";
			return;

		default:
			// Something went wrong.
			if (debug_mode) std::cout << "\nCLIENT: receive didn't return Done\n";
			return;
		}
	}

	/// Extract the variables contained in the packet
}

void GameServerState::update()
{
	// display text
	displayText();

	// CHECK FOR NEW CLIENT TO CONNECT. Currently working only with one client.
	//if (!established_connection)
	establishConnectionWithClient(debug_mode);

	player.update();
	enemy.setPosition(lerp(enemy.getPosition(), player.getPosition(), 0.01));

	if (checkSphereBounding(&player, &enemy))
	{
		player.collisionRespone(&enemy);
	}

	// server should probably keep listening and sending all the time
	for (auto& port : addresses) { sendMessageToClient(player, enemy, clock, port.first, debug_mode); }
}


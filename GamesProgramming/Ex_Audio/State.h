#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Input.h"
#include <string.h>

enum class NetworkStateEnum { CLIENT, SERVER, NONE };
enum class GameStateEnum { MENU, NETWORK, NETWORK_CLIENT, NETWORK_SERVER, GAME_CLIENT, GAME_SERVER, PAUSE, CREDITS };

class NetworkState
{
public:
	NetworkState();
	virtual ~NetworkState();
	
	virtual void handleInput() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

protected:
	void beginDraw();
	void endDraw();

	sf::RenderWindow* window;
	Input* input;
	GameStateEnum* state;
	NetworkStateEnum* network_state;
	// Network
	sf::UdpSocket *socket;
	sf::IpAddress* ip_address;
	unsigned short* port;
	sf::Clock* clock;
	sf::Int32* offset;

	bool established_connection;
	// input handlers
	bool ready;
	bool server;
	bool client;
	bool debug_mode;
	bool debug_message;

	// display text
	sf::Text text;
	sf::Font font;
};


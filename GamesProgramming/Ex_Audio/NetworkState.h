#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Input.h"
#include "State.h"

class NetworkState : public State
{
public:
	NetworkState();
	~NetworkState();

protected:
	void beginDraw();
	void endDraw();

	

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


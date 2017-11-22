#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Input.h"
#include <string.h>
#include "NetworkState.h"

class NetworkSelect : public NetworkState
{
public:
	NetworkSelect(sf::RenderWindow* hwnd, 
		Input* in, 
		sf::UdpSocket* udp_socket, 
		sf::IpAddress* ip, 
		unsigned short *port_number);
	~NetworkSelect();

	virtual void handleInput();
	virtual void update();
	// virtual render function is the same for all Networking states and could be inherited from 
	// the NetworkState base class but for the sake of clarity it's being repeted in the derived class
	// which also makes changes to in
	virtual void render();

	/////////////
	// NETWORK //
	/////////////
	// client
	void createClientSocket();

	// server
	void createServerSocket();	
};


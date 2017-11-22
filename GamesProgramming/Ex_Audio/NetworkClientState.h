#pragma once
#include "Input.h"
#include "Network.h"
#include "NetworkState.h"

class NetworkClientState : public NetworkState, public Network
{
public:
	NetworkClientState(sf::RenderWindow* hwnd, Input* in);
	~NetworkClientState();

	virtual void handleInput();
	virtual void update();
	// virtual render function is the same for all Networking states and could be inherited from 
	// the NetworkState base class but for the sake of clarity it's being repeted in the derived class
	virtual void render();

	/////////////
	// NETWORK //
	/////////////
	// client
	sf::Int32 start_timing_latency;
	sf::Int32 end_timing_latency;
	sf::Int32 latency;
	sf::Int32 getCurrentTime();

	bool send_packet;

	void sendPacketToServer();
	void checkForIncomingPacketsFromServer();
	void establishConnectionWithServer();
};

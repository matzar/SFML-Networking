#pragma once
#include <SFML/Network.hpp>
#include "Player.h"
#include "Messages.h"
#include <queue>
#include <deque>
class LinearPrediction
{
public:
	LinearPrediction();
	~LinearPrediction();

protected:
	// number of messages to keep track of
	const unsigned int linear_message_number = 2;
	std::queue<PlayerMessage> linear_local_positions;
	void keepTrackOfLinearLocalPositoins(const Player& player, const sf::Clock& clock, const sf::Int32& offset);
	void keepTrackOfLinearLocalPositoins(sf::Vector2f& vec, const sf::Clock& clock, const sf::Int32& offset);
	void keepTrackOfLinearNetworkPositions(const PlayerMessage& player_message_receive);
	std::queue<PlayerMessage> linear_network_positions;
	sf::Vector2f predictLinearLocalPath(const sf::Clock& clock, const sf::Int32& offset);
	sf::Vector2f predictLinearNetworkPath(const sf::Clock& clock, const sf::Int32& offset);
};

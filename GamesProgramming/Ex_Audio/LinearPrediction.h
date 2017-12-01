#pragma once
#include <SFML/Network.hpp>
#include "Sprite.h"
#include "Messages.h"
#include "Prediction.h"
#include <queue>
#include <deque>

class LinearPrediction : public Prediction
{
public:
	LinearPrediction();
	~LinearPrediction();

	// number of messages to keep track of
	const unsigned int linear_message_number = 2;

	// history of sprite's local positions
	std::queue<Message> linear_local_positions;
	// history of sprite's network positions
	std::queue<Message> linear_network_positions;

	//void keepTrackOfLinearLocalPositoins(const Sprite& sprite, const sf::Int32& time);
	// for adding lepred position to the history of positions
	void keepTrackOfLinearLocalPositoins(sf::Vector2f& vec, const sf::Int32& time);

	void keepTrackOfLinearLocalPositoins(const Message & local_message);
	void keepTrackOfLinearNetworkPositions(const Message& message_receive);

	


	void linearInterpolation(Sprite & sprite, sf::Vector2f & msg0_local_position, sf::Vector2f & masg1_local_position, sf::Vector2f & msg0_network_position, sf::Vector2f & msg1_network_position, float & msg0_time, float & msg1_time, const sf::Int32 & tm, const bool & lerp_mode);
protected:
	sf::Vector2f predictLinearLocalPath(sf::Vector2f & msg0_local_position, sf::Vector2f & masg1_local_position, float & msg0_time, float & msg1_time, float & time);
	sf::Vector2f predictLinearNetworkPath(sf::Vector2f & msg0_network_position, sf::Vector2f & msg1_network_position, float & time0, float & time1, float & time);
	
};


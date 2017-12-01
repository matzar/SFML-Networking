#include "LinearPrediction.h"

LinearPrediction::LinearPrediction()
{
}


LinearPrediction::~LinearPrediction()
{
}

//void LinearPrediction::keepTrackOfLinearLocalPositoins(const Sprite& sprite, const sf::Int32& tm)
//{
//	// local message
//	Message local_message;
//	local_message.player_position.x = sprite.getPosition().x;
//	local_message.player_position.y = sprite.getPosition().y;
//	local_message.time = (float)tm;
//
//	if (linear_local_positions.size() >= linear_message_number) linear_local_positions.pop();
//	linear_local_positions.push(local_message);
//}

// for adding lepred position to the history of local postions
void LinearPrediction::keepTrackOfLinearLocalPositoins(sf::Vector2f& vec, const sf::Int32& tm)
{
	// local message
	Message local_message;
	local_message.player_position.x = vec.x;
	local_message.player_position.y = vec.y;
	local_message.time = (float)tm;
	// 
	if (linear_local_positions.size() >= linear_message_number) linear_local_positions.pop();
	linear_local_positions.push(local_message);
}

void LinearPrediction::keepTrackOfLinearLocalPositoins(const Message& local_message)
{
	if (linear_local_positions.size() >= linear_message_number) linear_local_positions.pop();
	linear_local_positions.push(local_message);
}

void LinearPrediction::keepTrackOfLinearNetworkPositions(const Message& message_receive)
{
	if (linear_network_positions.size() >= linear_message_number) linear_network_positions.pop();
	linear_network_positions.push(message_receive);
}

//sf::Vector2f LinearPrediction::predictLinearLocalPath(const sf::Int32& tm)
//{
//	float x_average_velocity, y_average_velocity;
//	Message msg0 = linear_local_positions.front();
//	Message msg1 = linear_local_positions.back();
//	float time = (float)tm;
//
//	// average velocity = (recieved_position - last_position) / (recieved_time - last_time)
//	x_average_velocity = (msg0.player_position.x - msg1.player_position.x) / (msg0.time - msg1.time);
//	y_average_velocity = (msg0.player_position.y - msg1.player_position.y) / (msg0.time - msg1.time);
//
//	//// linear model
//	float x_, y_;
//	x_ = x_average_velocity * (time - msg1.time) + msg1.player_position.x;
//	y_ = y_average_velocity * (time - msg1.time) + msg1.player_position.y;
//
//	sf::Vector2f local_player_pos(x_, y_);
//	return local_player_pos;
//}
//
//sf::Vector2f LinearPrediction::predictLinearNetworkPath(const sf::Int32& tm)
//{
//	float x_average_velocity, y_average_velocity, x_, y_;
//	Message msg0 = linear_network_positions.front();
//	Message msg1 = linear_network_positions.back();
//	float time = (float)tm;
//
//	// average velocity = (recieved_position - last_position) / (recieved_time - last_time)
//	x_average_velocity = (msg0.player_position.x - msg1.player_position.x) / (msg0.time - msg1.time);
//	y_average_velocity = (msg0.player_position.y - msg1.player_position.y) / (msg0.time - msg1.time);
//
//	// linear model
//	x_ = x_average_velocity * (time - msg1.time) + msg1.player_position.x;
//	y_ = y_average_velocity * (time - msg1.time) + msg1.player_position.y;
//
//	sf::Vector2f network_player_pos(x_, y_);
//	return network_player_pos;
//}

//sf::Vector2f LinearPrediction::predictLinearLocalPath(Message& msg0, Message& msg1, float& time)
//{
//	float x_average_velocity, y_average_velocity;
//
//	// average velocity = (recieved_position - last_position) / (recieved_time - last_time)
//	x_average_velocity = (msg0.player_position.x - msg1.player_position.x) / (msg0.time - msg1.time);
//	y_average_velocity = (msg0.player_position.y - msg1.player_position.y) / (msg0.time - msg1.time);
//
//	//// linear model
//	float x_, y_;
//	x_ = x_average_velocity * (time - msg1.time) + msg1.player_position.x;
//	y_ = y_average_velocity * (time - msg1.time) + msg1.player_position.y;
//
//	sf::Vector2f local_player_pos(x_, y_);
//	return local_player_pos;
//}
//
//sf::Vector2f LinearPrediction::predictLinearNetworkPath(Message& msg0, Message& msg1, float& time)
//{
//	float x_average_velocity, y_average_velocity, x_, y_;
//
//	// average velocity = (recieved_position - last_position) / (recieved_time - last_time)
//	x_average_velocity = (msg0.player_position.x - msg1.player_position.x) / (msg0.time - msg1.time);
//	y_average_velocity = (msg0.player_position.y - msg1.player_position.y) / (msg0.time - msg1.time);
//
//	// linear model
//	x_ = x_average_velocity * (time - msg1.time) + msg1.player_position.x;
//	y_ = y_average_velocity * (time - msg1.time) + msg1.player_position.y;
//
//	sf::Vector2f network_player_pos(x_, y_);
//	return network_player_pos;
//}

sf::Vector2f LinearPrediction::predictLinearLocalPath(sf::Vector2f& local_position_0, sf::Vector2f& local_position_1, float& time)
{
	float x_average_velocity, y_average_velocity;

	// average velocity = (recieved_position - last_position) / (recieved_time - last_time)
	x_average_velocity = (msg0.player_position.x - msg1.player_position.x) / (msg0.time - msg1.time);
	y_average_velocity = (msg0.player_position.y - msg1.player_position.y) / (msg0.time - msg1.time);

	//// linear model
	float x_, y_;
	x_ = x_average_velocity * (time - msg1.time) + msg1.player_position.x;
	y_ = y_average_velocity * (time - msg1.time) + msg1.player_position.y;

	sf::Vector2f local_player_pos(x_, y_);
	return local_player_pos;
}

sf::Vector2f LinearPrediction::predictLinearNetworkPath(sf::Vector2f& network_position_0, sf::Vector2f& network_position_1, float& time)
{
	float x_average_velocity, y_average_velocity, x_, y_;

	// average velocity = (recieved_position - last_position) / (recieved_time - last_time)
	x_average_velocity = (msg0.player_position.x - msg1.player_position.x) / (msg0.time - msg1.time);
	y_average_velocity = (msg0.player_position.y - msg1.player_position.y) / (msg0.time - msg1.time);

	// linear model
	x_ = x_average_velocity * (time - msg1.time) + msg1.player_position.x;
	y_ = y_average_velocity * (time - msg1.time) + msg1.player_position.y;

	sf::Vector2f network_player_pos(x_, y_);
	return network_player_pos;
}

// pass local position vectors and network position vectors of the sprite
void LinearPrediction::linearInterpolation(Sprite& sprite, 
	sf::Vector2f& local_position_0, sf::Vector2f& local_position_1,
	sf::Vector2f& network_position_0, sf::Vector2f& network_position_2, 
	const sf::Int32& tm, const bool& lerp_mode)
{
	float time = (float)tm;
	// TODO
	sf::Vector2f local_path = predictLinearLocalPath(local_position_0, local_position_1, time);
	sf::Vector2f network_path = predictLinearNetworkPath(network_position_0, network_position_1, time);
	//lerp path works better with 100ms lag
	sf::Vector2f lerp_position = lerp(local_path, network_path, 0.1);

	// set player_position
	lerp_mode ? sprite.setPosition(lerp_position) : sprite.setPosition(network_path);

	//	Message local_message;
	/*Message lerp_position_message;
	lerp_position_message.*/
	//	local_message.player_position.x = vec.x;
	//	local_message.player_position.y = vec.y;
	//	local_message.time = (float)tm;

	// add lerped to the history of the local posistions
	keepTrackOfLinearLocalPositoins(lerp_position, tm);
}

//void LinearPrediction::linearInterpolation(Sprite& sprite, const sf::Int32& tm, const bool& lerp_mode)
//{
//	// TODO
//	sf::Vector2f local_path = predictLinearLocalPath(tm);
//	sf::Vector2f network_path = predictLinearNetworkPath(tm);
//	//lerp path works better with 100ms lag
//	sf::Vector2f lerp_position = lerp(local_path, network_path, 0.1);
//
//	// set player_position
//	lerp_mode ? sprite.setPosition(lerp_position) : sprite.setPosition(network_path);
//
//	// add lerped to the history of the local posistions
//	keepTrackOfLinearLocalPositoins(lerp_position, tm);
//}


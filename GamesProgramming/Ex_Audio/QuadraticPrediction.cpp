#include "QuadraticPrediction.h"

QuadraticPrediction::QuadraticPrediction()
{
}


QuadraticPrediction::~QuadraticPrediction()
{
}

void QuadraticPrediction::keepTrackOfQuadraticLocalPositoins(const Sprite& player, const sf::Int32& tm)
{
	// local message
	Message local_message;
	local_message.player_position.x = player.getPosition().x;
	local_message.player_position.y = player.getPosition().y;
	local_message.time = (float)tm;
	//
	if (local_message_history.size() >= quadratic_message_number) local_message_history.pop_back();
	local_message_history.push_front(local_message);
}

void QuadraticPrediction::keepTrackOfQuadraticLocalPositoins(sf::Vector2f& vec, const sf::Int32& tm)
{
	// local message
	Message local_message;
	local_message.player_position.x = vec.x;
	local_message.player_position.y = vec.y;
	local_message.time = (float)tm;
	// 
	if (local_message_history.size() >= quadratic_message_number) local_message_history.pop_back();
	local_message_history.push_front(local_message);
}

void QuadraticPrediction::keepTrackOfQuadraticNetworkPositions(const Message& player_message_receive)
{
	if (network_message_history.size() >= quadratic_message_number) network_message_history.pop_back();
	network_message_history.push_front(player_message_receive);
}

sf::Vector2f QuadraticPrediction::predictQuadraticLocalPath(const sf::Int32& tm)
{
	// quadratic model
	float x_average_velocity_1, y_average_velocity_1,
		x_average_velocity_2, y_average_velocity_2,
		a_x, a_y,
		x_, y_;


	Message msg0 = network_message_history.at(0);
	Message msg1 = network_message_history.at(1);
	Message msg2 = network_message_history.at(2);
	float time = (float)tm;

	// average velocity = (recieved_position - last_position) / (recieved_time - last_time)
	x_average_velocity_1 = (msg0.player_position.x - msg1.player_position.x) / (msg0.time - msg1.time);
	y_average_velocity_1 = (msg0.player_position.y - msg1.player_position.y) / (msg0.time - msg1.time);

	x_average_velocity_2 = (msg1.player_position.x - msg2.player_position.x) / (msg1.time - msg2.time);
	y_average_velocity_2 = (msg1.player_position.y - msg2.player_position.y) / (msg1.time - msg2.time);

	a_x = (x_average_velocity_2 - x_average_velocity_1) / (msg2.time - msg0.time);
	a_y = (y_average_velocity_2 - y_average_velocity_1) / (msg2.time - msg0.time);

	// s = s0 + v0t + �at2
	x_ = msg2.player_position.x + (x_average_velocity_2 * (time - msg2.time)) + ((0.5 * a_x) * powf((time - msg2.time), 2));
	y_ = msg2.player_position.y + (y_average_velocity_2 * (time - msg2.time)) + ((0.5 * a_y) * powf((time - msg2.time), 2));

	sf::Vector2f local_player_pos(x_, y_);
	return local_player_pos;
}

sf::Vector2f QuadraticPrediction::predictQuadraticNetworkPath(const sf::Int32& tm)
{
	// quadratic model
	float x_average_velocity_1, y_average_velocity_1,
		x_average_velocity_2, y_average_velocity_2,
		a_x, a_y,
		x_, y_;


	Message msg0 = network_message_history.at(0);
	Message msg1 = network_message_history.at(1);
	Message msg2 = network_message_history.at(2);
	float time = (float)tm;

	// average velocity = (recieved_position - last_position) / (recieved_time - last_time)
	x_average_velocity_1 = (msg0.player_position.x - msg1.player_position.x) / (msg0.time - msg1.time);
	y_average_velocity_1 = (msg0.player_position.y - msg1.player_position.y) / (msg0.time - msg1.time);

	x_average_velocity_2 = (msg1.player_position.x - msg2.player_position.x) / (msg1.time - msg2.time);
	y_average_velocity_2 = (msg1.player_position.y - msg2.player_position.y) / (msg1.time - msg2.time);

	a_x = (x_average_velocity_2 - x_average_velocity_1) / (msg2.time - msg0.time);
	a_y = (y_average_velocity_2 - y_average_velocity_1) / (msg2.time - msg0.time);

	// s = s0 + v0t + �at2
	x_ = msg2.player_position.x + (x_average_velocity_2 * (time - msg2.time)) + ((0.5 * a_x) * powf((time - msg2.time), 2));
	y_ = msg2.player_position.y + (y_average_velocity_2 * (time - msg2.time)) + ((0.5 * a_y) * powf((time - msg2.time), 2));

	sf::Vector2f network_player_pos(x_, y_);
	return network_player_pos;
}

void QuadraticPrediction::quadraticInterpolation(Sprite& player, const sf::Int32& tm, const bool& lerp_mode)
{
	sf::Vector2f local_path = predictQuadraticLocalPath(tm);
	sf::Vector2f network_path = predictQuadraticNetworkPath(tm);
	//lerp path works better with 100ms lag
	sf::Vector2f lerp_position = lerp(local_path, network_path, 0.1);

	// set player_position
	lerp_mode ? player.setPosition(lerp_position) : player.setPosition(network_path);

	// add lerped to the history of the local posistions
	keepTrackOfQuadraticLocalPositoins(lerp_position, tm);
}
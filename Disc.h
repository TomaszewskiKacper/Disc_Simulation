#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "Base_classes.h"
//#include "Simulation.h"

class Disc : public Drawable
{
public:
	//constructors
	Disc(sf::Vector2u boundry);	//fully random disc
	Disc(float radius, sf::Vector2f position, sf::Vector2f velocity, sf::Color color, float mass);	//everything set by user

	
	void draw(sf::RenderWindow& window) override;
	void move(float dt);
	void update(float dt, sf::Vector2f center);
	void update_e_gravity(float dt);
	void update_mouse_drag(sf::Vector2f pos);
	void update_aero(float dt);
	void toggle_color(bool state);
	void set_gravity_force_multiplier(int g);
	void set_dragged(bool state);
	void set_position(sf::Vector2f pos);

	bool contains(sf::Vector2f position);


	void check_collision(Disc& other, float dt);
	void resolve_collision(Disc& other, float dt);

	sf::Vector2f get_v() const { return v; };




private:
	void boundry_check();

	sf::CircleShape disc;	//disc object
	sf::Vector2f v;	//velocity
	float m;	//mass
	sf::Vector2u bounds;
	sf::Color original_color;

	int gravity_force_multiplier;

	bool color_toggle;
	bool dragged;
};


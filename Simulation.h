#pragma once
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include <vector>
#include <random>

#include "Disc.h"

#include "Settings.h"


class Simulation
{
public:
	Simulation();
	~Simulation();

	void run();	//main loop of simulation


private:
	void handle_events();
	void update_simulation();
	void update_settings();
	void render_simulation();
	void render_settings();
	void draw_viscosity();
	void draw_gravity();
	void draw_spot(sf::Vector2f spot);
public:
	static float viscosity_at_position(sf::Vector2f position);


private:
	//ELEMENTS OF SIMULATION
	std::vector<Disc> discs;	//discs vector
	std::vector<sf::Vector2f> gravity_spots;	//gravity point vector

	//WINDOWS
	sf::RenderWindow simulation_window;
	sf::RenderWindow s_window;

	//VARIABLES
	sf::Clock clock;	//clock for delta time
	sf::Clock s_clock;	//clock for delta time

	Settings settings;
};


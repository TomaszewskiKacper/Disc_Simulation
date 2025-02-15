#include "Simulation.h"

Simulation::Simulation() :
	//Initialize windows
	simulation_window(sf::VideoMode(1800, 900), "Simulation"),
	s_window(sf::VideoMode(500, 800), "Settings"),
	settings(s_window),

	//Initialize Variables
	clock(sf::Clock()),
	s_clock(sf::Clock()){

	//random seed
	std::srand(static_cast<unsigned>(std::time(0)));	

}

Simulation::~Simulation()
{
}

void Simulation::run()
{	
	while (simulation_window.isOpen() && s_window.isOpen()) {
		handle_events();

		// HANDLE PAUSING SIMULATION
		if (!settings.isPaused()) {
			update_simulation();
		}
		else {
			clock.restart();
		}

		update_settings();
		render_simulation();
		render_settings();
	}
}

void Simulation::handle_events()
{
	//Event handler
	sf::Event event;

	//Simulation window events
	while (simulation_window.pollEvent(event)) {	
		if (event.type == sf::Event::Closed) {	//close window event
			simulation_window.close();
			s_window.close();
		}
	}

	//Mouse event for dragging discs
	if (event.type == sf::Event::MouseButtonPressed)
		if (event.mouseButton.button == sf::Mouse::Left) {
			//check if discs are under cursor
			for (auto& e : discs) 
				if (e.contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
					e.set_dragged(true);
					e.set_position(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
				}
		}
	if (event.type == sf::Event::MouseButtonReleased)
		if (event.mouseButton.button == sf::Mouse::Left) {
			//Release all
			for (auto& e : discs)
				e.set_dragged(false);
		}

	//Mouse event for creating gravity spots
	if(settings.isCreateGravity())
		if (event.type == sf::Event::MouseButtonPressed)
			if (event.mouseButton.button == sf::Mouse::Left) 
				gravity_spots.push_back(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
			




	//Settings window events
	while (s_window.pollEvent(event)) {
		settings.update(event);
		if (event.type == sf::Event::Closed) {	//close window event
			simulation_window.close();
			s_window.close();
		}
	}
}

void Simulation::update_simulation()
{
	float dt = clock.restart().asSeconds();	//Delta Time
	


	for (auto& e : discs) {
		//toggle check
		e.toggle_color(settings.isColorBySpeed());
		e.set_gravity_force_multiplier(settings.getGravity());

		e.move(dt);	//move 1st
		e.update_mouse_drag(sf::Vector2f(sf::Mouse::getPosition(simulation_window)));

		//Updates
		for(auto& attractor : gravity_spots)
			e.update(dt, attractor);	//update velocity 2nd, for every gravity spot

		e.update_aero(dt);

		if (settings.isEarthGravity())	//if earth like gravity is enabled
			e.update_e_gravity(dt);

		if (settings.isCollision()) //if collision is enabled
			for (auto& other : discs)
				if (&e != &other)	//don't check with self
					e.check_collision(other, dt);
	}
}

void Simulation::update_settings()
{
	//settings.update();

	//update disc count based on slider
	int target_count = settings.getDiscCount();
	while (discs.size() > target_count) discs.pop_back();	//delete if less
	while (discs.size() < target_count) discs.push_back(Disc(simulation_window.getSize()));	//add random if more	
}

void Simulation::render_simulation()
{
	simulation_window.clear(sf::Color::Black);	//clear window

	if (settings.isViscosityVisible())
		draw_viscosity();

	//draw discs
	for (auto& e : discs) {
		e.draw(simulation_window);
	}
	
	draw_gravity();
	
	simulation_window.display();	//display
}

void Simulation::render_settings()
{
	s_window.clear(sf::Color::Color(204,204,204,255));
	settings.draw();
	s_window.display();

}


// Function to map viscosity value to color 
sf::Color getColorForViscosity(float viscosity)
{
	float maxViscosity = 1.0f; // Set a maximum viscosity for color scaling
	float factor = std::min(viscosity / maxViscosity, 1.0f);

	//std::cout << viscosity << " " << factor << " " << 255 * factor << std::endl;
	// Interpolate between blue (low viscosity) and red (high viscosity)
	return sf::Color(
		static_cast<sf::Uint8>(255 * factor),  // Red component increases with viscosity
		static_cast<sf::Uint8>(0),            // Green remains zero
		static_cast<sf::Uint8>(255 * (1 - factor))  // Blue decreases with viscosity
	);
}

void Simulation::draw_viscosity()
{
	// Loop through each pixel in the window
	for (unsigned int x = 0; x < simulation_window.getSize().x; ++x)
	{
		for (unsigned int y = 0; y < simulation_window.getSize().y; ++y)
		{
			// Calculate the viscosity based on position
			sf::Vector2f pos(x, y);
			float viscosity = viscosity_at_position(pos);

			// Get the color based on the viscosity
			sf::Color color = getColorForViscosity(viscosity);

			// Set the pixel color (use a rectangle for better performance)
			sf::RectangleShape pixel(sf::Vector2f(1.0f, 1.0f));
			pixel.setPosition(static_cast<float>(x), static_cast<float>(y));
			pixel.setFillColor(color);

			// Draw the pixel on the window
			simulation_window.draw(pixel);
		}
	}

}


void Simulation::draw_gravity()
{
	for (auto& e : gravity_spots)
		draw_spot(e);
}

void Simulation::draw_spot(sf::Vector2f spot)
{
	sf::Vertex line1[] =
	{
		sf::Vertex(spot - sf::Vector2f(15, 15)),
		sf::Vertex(spot + sf::Vector2f(15, 15))  // Diagonal line
	};

	sf::Vertex line2[] =
	{
		sf::Vertex(spot - sf::Vector2f(15, -15)),
		sf::Vertex(spot + sf::Vector2f(15, -15))  // Diagonal line
	};

	simulation_window.draw(line1, 2, sf::Lines);
	simulation_window.draw(line2, 2, sf::Lines);
	simulation_window.draw(line2, 2, sf::Lines);
	simulation_window.draw(line2, 2, sf::Lines);
}

float Simulation::viscosity_at_position(sf::Vector2f position)
{
	float x = position.x;
	float y = position.y;

	// Calculate the distance from the origin
	float dist = std::sqrt(x * x + y * y);

	

	return dist * 0.0005f;  // Adjust scaling factor if necessary
}

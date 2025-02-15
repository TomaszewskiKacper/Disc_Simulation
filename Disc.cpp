#include "Disc.h"
#include "Simulation.h"

Disc::Disc(sf::Vector2u boundry) :
	disc(sf::CircleShape()), color_toggle(false), gravity_force_multiplier(1),dragged(false)
{
	//sf::Vector2u boundry = window.getSize();
	bounds = boundry;
	int x = boundry.x, y = boundry.y;
	int limit = std::min(x, y) / 50;	//limit so discs aren't too big

	
	//randomize disc parameters
	int r = std::rand() % limit;
	disc.setOrigin(r, r);
	disc.setRadius(r);
	disc.setFillColor(sf::Color(std::rand() % 255, std::rand() % 255, std::rand() % 255, 255));
	original_color = disc.getFillColor();
	sf::Vector2f coords(
		r + std::rand() % (x - 2*r),
		r + std::rand() % (y - 2*r));
	disc.setPosition(coords);

	int speed = limit * 3;
	v = sf::Vector2f((std::rand() % speed) - speed / 2, (std::rand() % speed) - speed / 2);
	m = std::rand() % 1000;
}

Disc::Disc(float radius, sf::Vector2f position, sf::Vector2f velocity, sf::Color color, float mass) :
	v(velocity),m(mass), original_color(color), disc(sf::CircleShape()), color_toggle(false),dragged(false), gravity_force_multiplier(1) {
	disc.setPosition(position);
	disc.setFillColor(color);
	disc.setRadius(radius);
}

sf::Color interpolateColor(const sf::Color& color1, const sf::Color& color2, float factor) {
	// Ensure the factor is between 0 and 1
	factor = std::clamp(factor, 0.0f, 1.0f);

	// Interpolate each color component (R, G, B, A)
	sf::Uint8 r = static_cast<sf::Uint8>(color1.r + factor * (color2.r - color1.r));
	sf::Uint8 g = static_cast<sf::Uint8>(color1.g + factor * (color2.g - color1.g));
	sf::Uint8 b = static_cast<sf::Uint8>(color1.b + factor * (color2.b - color1.b));
	sf::Uint8 a = static_cast<sf::Uint8>(color1.a + factor * (color2.a - color1.a));

	return sf::Color(r, g, b, a);
}

void Disc::draw(sf::RenderWindow& window)
{
	//Check for color drawind
	if (color_toggle) {
		//set color equal to speed
		float speed = sqrt(v.x * v.x + v.y * v.y);
		sf::Color c = interpolateColor(sf::Color::Blue, sf::Color::Red, std::min(speed / 500.0f, 1.0f));
		disc.setFillColor(c);
	}


	window.draw(disc);
}

void Disc::boundry_check()
{
	if (disc.getGlobalBounds().left < 0) {	//hit left wall
		v.x *= -1; // reverse direction
		sf::Vector2f correction(
			disc.getGlobalBounds().left * -1,
			0);
		disc.move(correction);
	}
	if (disc.getGlobalBounds().left + disc.getGlobalBounds().width > bounds.x) {	//hit right wall
		v.x *= -1;	//reverse direction
		sf::Vector2f correction(
			((disc.getGlobalBounds().left + disc.getGlobalBounds().width) - bounds.x) * -1,
			0);
		disc.move(correction);
	}
	if (disc.getGlobalBounds().top < 0) {	//hit left wall
		v.y *= -1; // reverse direction
		sf::Vector2f correction(
			0,
			disc.getGlobalBounds().top * -1);
		disc.move(correction);
	}
	if (disc.getGlobalBounds().top + disc.getGlobalBounds().height > bounds.y) {	//hit right wall
		v.y *= -1;	//reverse direction
		sf::Vector2f correction(
			0,
			((disc.getGlobalBounds().top + disc.getGlobalBounds().height) - bounds.y) * -1);
		disc.move(correction);
	}
}


void Disc::move(float dt)
{
	if (dragged)
		return;
	sf::Vector2f offset(
		v.x * dt,
		v.y * dt);
	disc.move(offset);
	boundry_check();
}

float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
	return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
}

void Disc::update(float dt, sf::Vector2f center)
{
	if (dragged)
		return;
	//sf::Vector2f center(bounds.x / 2, bounds.y / 2);
	sf::Vector2f direction = center - disc.getPosition();

	float dist = distance(center, disc.getPosition());
	
	if (dist > 0.001f) {//small numbers could give division by 0 problems 
		float F = 1.0f / std::pow(dist,2);	//fixed force, was 1/r, now is 1/r^2
		direction /= dist;	//normalize vector

		v += direction * (float)gravity_force_multiplier * dt;	//update velocity
	}
}

void Disc::update_e_gravity(float dt)
{
	if (dragged)
		return;
	update(dt, sf::Vector2f(disc.getPosition().x, bounds.y));
}

void Disc::update_mouse_drag(sf::Vector2f pos)
{
	if (dragged)
		disc.setPosition(pos);
}


void Disc::update_aero(float dt)
{
	if (dragged)
		return;
	float speed = sqrt(v.x * v.x + v.y * v.y);
	
	float drag = 6 * 3.14 * Simulation::viscosity_at_position(disc.getPosition()) * disc.getRadius() * speed;

	sf::Vector2f drag_force = -v * drag;

	v += drag_force/m * dt;
}

void Disc::toggle_color(bool state)
{
	if (state == color_toggle)
		return;
	//if true, change to false, and return original color
	if (color_toggle) {
		disc.setFillColor(original_color);
	}
	color_toggle = !color_toggle;
}

void Disc::set_gravity_force_multiplier(int g)
{
	gravity_force_multiplier = g;
}

void Disc::set_dragged(bool state)
{
	dragged = state;
}

void Disc::set_position(sf::Vector2f pos)
{
	disc.setPosition(pos);
}

bool Disc::contains(sf::Vector2f position)
{
	return disc.getGlobalBounds().contains(position);
}

void Disc::check_collision(Disc& other, float dt)
{
	float dist = distance(disc.getPosition(), other.disc.getPosition());
	if (dist < disc.getRadius() + other.disc.getRadius())
		resolve_collision(other, dt);
}


void Disc::resolve_collision(Disc& other, float dt) {
	sf::Vector2f pos1 = disc.getPosition();
	sf::Vector2f pos2 = other.disc.getPosition();
	float dist = distance(pos1, pos2);
	float forceMagnitude = 1000.0f / (dist * dist);  // adjust strength
	sf::Vector2f normal = pos2 - pos1;
	normal /= dist;  // Normalize the normal vector

	// Apply force to both discs, pushing them apart
	sf::Vector2f force = normal * forceMagnitude;

	float total_m = other.m + m;
	float mult1 = m / total_m;
	float mult2 = other.m / total_m;
	// Update velocities (velocity = force * dt / mass, but we ignore mass here)
	v -= force * mult1 * dt;      // Disc 1 velocity change
	other.v += force * mult2 * dt;  // Disc 2 velocity change
}




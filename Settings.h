#pragma once
#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
class Settings
{
public:
	Settings(sf::RenderWindow& window);

	void update(sf::Event& event);
	void draw(); 

	//Getters for settings
	bool isColorBySpeed() const;
	bool isPaused() const;
	bool isEarthGravity() const;
	bool isCollision() const;
	bool isViscosityVisible() const;
	bool isCreateGravity() const;

	int getDiscCount() const;
	int getGravity() const;


private:
	tgui::Gui settings_gui;

	// UI Elements
	tgui::Button::Ptr disc_color_by_speed_toggle;
	tgui::Button::Ptr pause_toggle;
	tgui::Button::Ptr earth_gravity_toggle;
	tgui::Button::Ptr collision_toggle;
	tgui::Button::Ptr viscosity_visible_toggle;
	tgui::Button::Ptr gravity_create_toggle;

	tgui::Slider::Ptr disc_count_slider;
	tgui::Slider::Ptr gravity_slider;
	tgui::EditBox::Ptr disc_count_input;
	tgui::EditBox::Ptr gravity_input;

	tgui::Label::Ptr s_label;
	tgui::Label::Ptr pause_label;
	tgui::Label::Ptr slider_label;
	tgui::Label::Ptr gravity_slider_label;
	tgui::Label::Ptr earth_gravity_label;
	tgui::Label::Ptr collision_label;
	tgui::Label::Ptr viscosity_visible_label;
	tgui::Label::Ptr gravity_create_label;


	// state vars
	bool color_discs_by_speed;
	bool is_paused;
	bool earth_gravity;
	bool is_collision;
	bool is_viscosity_visible;
	bool is_create_gravity;


	// UI settings
	int button_width;
	int ui_height;
	int slider_width;
	int slider_height;
	

};


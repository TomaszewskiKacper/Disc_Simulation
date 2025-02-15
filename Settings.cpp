#include "Settings.h"

Settings::Settings(sf::RenderWindow& window)
    : settings_gui(window),
    color_discs_by_speed(false), is_paused(false), is_collision(false), is_viscosity_visible(false),
    ui_height(25), button_width(60), slider_width(200), slider_height(20)
{
    int slot = 5;
    int offset = 5;
    int label_offset = offset+button_width+5;
    int after_slider_offset = slider_width + 15 + offset;

    //_________________________________________________________________________________
    // Toggle Button for "Disc Color by Speed"
    disc_color_by_speed_toggle = tgui::Button::create("OFF");
    disc_color_by_speed_toggle->setPosition(offset, slot);
    disc_color_by_speed_toggle->setSize(button_width, ui_height);
    disc_color_by_speed_toggle->onClick([this]() {
        color_discs_by_speed = !color_discs_by_speed;
        disc_color_by_speed_toggle->setText(color_discs_by_speed ? "ON" : "OFF");
        });
    settings_gui.add(disc_color_by_speed_toggle);

    // toggle Label
    s_label = tgui::Label::create();
    s_label->setPosition(label_offset, slot);
    s_label->setTextSize(ui_height);
    s_label->setText("Use speed as Color");
    settings_gui.add(s_label);


    slot += 30;
    //_________________________________________________________________________________
    //  Pause/Resume Button
    pause_toggle = tgui::Button::create("Pause");
    pause_toggle->setPosition(offset, slot);
    pause_toggle->setSize(button_width, ui_height);
    pause_toggle->onClick([this]() {
        is_paused = !is_paused;
        pause_toggle->setText(is_paused ? "Resume" : "Pause");
        });
    settings_gui.add(pause_toggle);

    // pause Label
    pause_label = tgui::Label::create();
    pause_label->setPosition(label_offset, slot);
    pause_label->setTextSize(ui_height);
    pause_label->setText("Pause Simulation");
    settings_gui.add(pause_label);

    slot += 30;
    
    //_________________________________________________________________________________
    //  Earth-Like Gravity Button
    earth_gravity_toggle = tgui::Button::create("OFF");
    earth_gravity_toggle->setPosition(offset, slot);
    earth_gravity_toggle->setSize(button_width, ui_height);
    earth_gravity_toggle->onClick([this]() {
        earth_gravity = !earth_gravity;
        earth_gravity_toggle->setText(earth_gravity ? "ON" : "OFF");
        });
    settings_gui.add(earth_gravity_toggle);

    // Earth-Like Gravity Label
    earth_gravity_label = tgui::Label::create();
    earth_gravity_label->setPosition(label_offset, slot);
    earth_gravity_label->setTextSize(ui_height);
    earth_gravity_label->setText("Earth Like Gravity");
    settings_gui.add(earth_gravity_label);

    slot += 30;
    //_________________________________________________________________________________

    //  Collision toggle Button
    collision_toggle = tgui::Button::create("OFF");
    collision_toggle->setPosition(offset, slot);
    collision_toggle->setSize(button_width, ui_height);
    collision_toggle->onClick([this]() {
        is_collision = !is_collision;
        collision_toggle->setText(is_collision ? "ON" : "OFF");
        });
    settings_gui.add(collision_toggle);

    // collision Label
    collision_label = tgui::Label::create();
    collision_label->setPosition(label_offset, slot);
    collision_label->setTextSize(ui_height);
    collision_label->setText("Spring Collision Between Discs");
    settings_gui.add(collision_label);

    slot += 30;

    //_________________________________________________________________________________

    //  viscosity visible toggle Button
    viscosity_visible_toggle = tgui::Button::create("OFF");
    viscosity_visible_toggle->setPosition(offset, slot);
    viscosity_visible_toggle->setSize(button_width, ui_height);
    viscosity_visible_toggle->onClick([this]() {
        is_viscosity_visible = !is_viscosity_visible;
        viscosity_visible_toggle->setText(is_viscosity_visible ? "ON" : "OFF");
        });
    settings_gui.add(viscosity_visible_toggle);

    // viscosity visibvle Label
    viscosity_visible_label = tgui::Label::create();
    viscosity_visible_label->setPosition(label_offset, slot);
    viscosity_visible_label->setTextSize(ui_height);
    viscosity_visible_label->setText("Visualise Viscosity");
    settings_gui.add(viscosity_visible_label);

    slot += 30;


    //_________________________________________________________________________________
    slot += 30;
    //_________________________________________________________________________________

    // slider Label
    slider_label = tgui::Label::create();
    slider_label->setPosition(label_offset, slot);
    slider_label->setTextSize(ui_height);
    slider_label->setText("Disc Count");
    settings_gui.add(slider_label);

    slot += 30;

    //_________________________________________________________________________________
    // Disc Count Slider
    disc_count_slider = tgui::Slider::create(0, 2000);
    disc_count_slider->setPosition(offset, slot);
    disc_count_slider->setSize(slider_width, slider_height);
    disc_count_slider->setValue(100); // Default disc count
    settings_gui.add(disc_count_slider);


    // Input for exact value
    disc_count_input = tgui::EditBox::create();
    disc_count_input->setPosition(after_slider_offset, slot); // Position next to the slider
    disc_count_input->setSize(button_width, ui_height);
    disc_count_input->setInputValidator(tgui::EditBox::Validator::Int); // Only allow numbers
    disc_count_input->setText(std::to_string(disc_count_slider->getValue())); // Sync initial value
    settings_gui.add(disc_count_input);

    // Sync input box when slider changes
    disc_count_slider->onValueChange([this](float newValue) {
        disc_count_input->setText(std::to_string(static_cast<int>(newValue))); // Update input box with new value
        });

    disc_count_input->onReturnKeyPress([this]() {
        std::string text = disc_count_input->getText().toStdString();
        try {
            int new_value = std::stoi(text);
            if (new_value >= disc_count_slider->getMinimum() && new_value <= disc_count_slider->getMaximum()) {
                disc_count_slider->setValue(new_value); // Update slider with new value
            }
        }
        catch (std::exception& e) {
            // Handle invalid input (like non-integer text)
        }
        });
    slot += 30;

    //_________________________________________________________________________________

    // slider Label
    gravity_slider_label = tgui::Label::create();
    gravity_slider_label->setPosition(label_offset, slot);
    gravity_slider_label->setTextSize(ui_height);
    gravity_slider_label->setText("Gravity Force Multiplier");
    settings_gui.add(gravity_slider_label);

    slot += 30;

    //_________________________________________________________________________________
    // Gravity Slider
    gravity_slider = tgui::Slider::create(0, 10000);
    gravity_slider->setPosition(offset, slot);
    gravity_slider->setSize(slider_width, slider_height);
    gravity_slider->setValue(100); // Default disc count
    settings_gui.add(gravity_slider);


    // Input for exact value
    gravity_input = tgui::EditBox::create();
    gravity_input->setPosition(after_slider_offset, slot); // Position next to the slider
    gravity_input->setSize(button_width, ui_height);
    gravity_input->setInputValidator(tgui::EditBox::Validator::Int); // Only allow numbers
    gravity_input->setText(std::to_string(gravity_slider->getValue())); // Sync initial value
    settings_gui.add(gravity_input);

    // Sync input box when slider changes
    gravity_slider->onValueChange([this](float newValue) {
        gravity_input->setText(std::to_string(static_cast<int>(newValue))); // Update input box with new value
        });

    gravity_input->onReturnKeyPress([this]() {
        std::string text = gravity_input->getText().toStdString();
        try {
            int new_value = std::stoi(text);
            if (new_value >= gravity_slider->getMinimum() && new_value <= gravity_slider->getMaximum()) {
                gravity_slider->setValue(new_value); // Update slider with new value
            }
        }
        catch (std::exception& e) {
            // Handle invalid input (like non-integer text)
        }
        });
    slot += 30;
    slot += 30;
    slot += 30;
    slot += 30;


    //_________________________________________________________________________________
    //  gravity spot creator Button
    gravity_create_toggle = tgui::Button::create("OFF");
    gravity_create_toggle->setPosition(offset, slot);
    gravity_create_toggle->setSize(button_width, ui_height);
    gravity_create_toggle->onClick([this]() {
        is_create_gravity = !is_create_gravity;
        gravity_create_toggle->setText(is_create_gravity ? "ON" : "OFF");
        });
    settings_gui.add(gravity_create_toggle);

    // gravity spot creator Label
    gravity_create_label = tgui::Label::create();
    gravity_create_label->setPosition(label_offset, slot);
    gravity_create_label->setTextSize(ui_height);
    gravity_create_label->setText("Create Gravity spot on mouse press");
    settings_gui.add(gravity_create_label);

    slot += 30;

}

void Settings::update(sf::Event& event)
{
    // Handles events for buttons, sliders, etc.
    settings_gui.handleEvent(event);
}



void Settings::draw()
{
    settings_gui.draw();
}

bool Settings::isColorBySpeed() const
{
    return color_discs_by_speed;
}

bool Settings::isPaused() const
{
    return is_paused;
}

bool Settings::isEarthGravity() const
{
    return earth_gravity;
}

int Settings::getDiscCount() const
{
    return static_cast<int>(disc_count_slider->getValue());
}

int Settings::getGravity() const
{
    return static_cast<int>(gravity_slider->getValue());
}

bool Settings::isCollision() const
{
    return is_collision;
}

bool Settings::isViscosityVisible() const
{
    return is_viscosity_visible;
}

bool Settings::isCreateGravity() const
{
    return is_create_gravity;
}
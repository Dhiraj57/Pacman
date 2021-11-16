#pragma once

class Pacman
{
private:
    bool animation_over;
    bool dead;
    int direction;
    int animation_timer;
    int energizer_timer;

    Position position;

public:
    Pacman();

    bool get_animation_over();
    bool get_dead();

    int get_direction();
    int get_energizer_timer();

    void draw(bool i_victory, sf::RenderWindow& i_window);
    void reset();
    void set_animation_timer(int i_animation_timer);
    void set_dead(bool i_dead);
    void set_position(int i_x, int i_y);
    void update(int i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map);

    Position get_position();

};
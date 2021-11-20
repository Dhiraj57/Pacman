#pragma once

class Ghost
{
private:
    bool movement_mode;
    bool use_door;

    int id;
    int direction;
    int frightened_mode;
    int frightened_speed_timer;
    int animation_timer;

    Position home;
    Position home_exit;
    Position position;
    Position target;

public:
    Ghost(int i_id);

    bool pacman_collision(const Position& i_pacman_position);
    float get_target_distance(int i_direction);
    void draw(bool i_flash, sf::RenderWindow& i_window);
    void reset(const Position& i_home, const Position& i_home_exit);
    void set_position(int i_x, int i_y);
    void switch_mode();
    void update(int i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, Ghost& i_ghost_0, Pacman& i_pacman);
    void update_target(int i_pacman_direction, const Position& i_ghost_0_position, const Position& i_pacman_position);

    Position get_position();
};
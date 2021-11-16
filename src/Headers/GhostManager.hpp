#pragma once

class GhostManager
{
private:
    int current_wave;
    int wave_timer;

    std::array<Ghost, 4> ghosts;

public:
    GhostManager();

    void draw(bool i_flash, sf::RenderWindow& i_window);
    void reset(int i_level, const std::array<Position, 4>& i_ghost_positions);
    void update(int i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, Pacman& i_pacman);
};

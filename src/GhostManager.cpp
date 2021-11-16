#include <array>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Headers/Global.hpp"
#include "Headers/Pacman.hpp"
#include "Headers/Ghost.hpp"
#include "Headers/GhostManager.hpp"

GhostManager::GhostManager() :
    current_wave(0),
	wave_timer(LONG_SCATTER_DURATION),
    ghosts({Ghost(0), Ghost(1), Ghost(2), Ghost(3)})
{
}

void GhostManager::draw(bool i_flash, sf::RenderWindow& i_window)
{
    for(Ghost& ghost : ghosts)
    {
        ghost.draw(i_flash, i_window);
    }
}

void GhostManager::reset(int i_level, const std::array<Position, 4>& i_ghost_positions)
{
    current_wave = 0;

    // To increase the difficulty as the game proceeds.
    wave_timer = static_cast<int>(LONG_SCATTER_DURATION / pow(2, i_level));

    for(int a=0; a < 4; a++)
    {
        ghosts[a].set_position(i_ghost_positions[a].x, i_ghost_positions[a].y);
    }

    for(Ghost& ghost : ghosts)
    {
        // We set home position and home exit position of each ghost.
        ghost.reset(ghosts[2].get_position(), ghosts[0].get_position());
    }
}

void GhostManager::update(int i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, Pacman& i_pacman)
{
    if(i_pacman.get_energizer_timer() == 0) // We won't update the wave timer when Pacman is energized.
    {
        if(wave_timer == 0)
        {
            if(current_wave < 7) // After 7, ghosts goes into permenant chase mode.
            {
                current_wave ++;

                for(Ghost& ghost : ghosts)
                {
                    // To switch between Scatter mode and Chase mode.
                    ghost.switch_mode();
                }
            }
            
            if(current_wave % 2 == 1) // Chase mode.
            {
                wave_timer = CHASE_DURATION;
            }

            // Scater mode.
            else if (current_wave == 2) 
            {
                wave_timer = static_cast<int>(LONG_SCATTER_DURATION / pow(2, i_level));
            }
            else
            {
                wave_timer = static_cast<int>(SHORT_SCATTER_DURATION / pow(2, i_level));
            }
        }

        else
        {
            wave_timer--;
        }
    }

    for(Ghost& ghost : ghosts)
    {
        ghost.update(i_level, i_map, ghosts[0], i_pacman);
    }
}
#include <iostream>
#include <array>
#include <chrono>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "src/Headers/Global.hpp"
#include "src/Headers/Pacman.hpp"
#include "src/Headers/DrawText.hpp"
#include "src/Headers/DrawMap.hpp"
#include "src/Headers/Ghost.hpp"
#include "src/Headers/ConvertSketch.hpp"
#include "src/Headers/GhostManager.hpp"
#include "src/Headers/MapCollision.hpp"

int main()
{
    bool game_won = 0;
    int lag = 0;
    int level = 0;

    //To make the game framerate-independent.
	std::chrono::time_point<std::chrono::steady_clock> previous_time;

    // Sketch which we convert into map.
    std::array<std::string, MAP_HEIGHT> map_sketch = {
		" ################### ",
		" #........#........# ",
		" #o##.###.#.###.##o# ",
		" #.................# ",
		" #.##.#.#####.#.##.# ",
		" #....#...#...#....# ",
		" ####.### # ###.#### ",
		"    #.#   0   #.#    ",
		"#####.# ##=## #.#####",
		"     .  #123#  .     ",
		"#####.# ##### #.#####",
		"    #.#       #.#    ",
		" ####.# ##### #.#### ",
		" #........#........# ",
		" #.##.###.#.###.##.# ",
		" #o.#.....P.....#.o# ",
		" ##.#.#.#####.#.#.## ",
		" #....#...#...#....# ",
		" #.######.#.######.# ",
		" #.................# ",
		" ################### "
	};

    // Converted map. (2D grid of cells)
    std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> map{};

    // Initial ghost positions.
	std::array<Position, 4> ghost_positions;

    sf::Event event;
    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * MAP_WIDTH * SCREEN_RESIZE, (FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT) * SCREEN_RESIZE), "Pacman", sf::Style::Close);
	window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * MAP_WIDTH, FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT)));

    GhostManager ghost_manager;
    Pacman pacman;

    srand(static_cast<int>(time(0)));

    map = convert_sketch(map_sketch, ghost_positions, pacman);
    ghost_manager.reset(level, ghost_positions);

    // Previous time point to calculate the lag.
	previous_time = std::chrono::steady_clock::now();

    while (window.isOpen())
    {
        // Calculation of lag.
        int delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time).count();

        // We keep adding delta into lag and when addition goes above fixed duration, we run next frame.
        lag += delta_time;
        previous_time += std::chrono::microseconds(delta_time);

        // Game loop runs only after lag goes above Frame_duration.
        while(FRAME_DURATION <= lag)
        {
            lag -= FRAME_DURATION; // Restting lag to zero.

            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            // Game mechanics which calls update functions of ghost and pacman.
            if(!game_won && !pacman.get_dead())
            {
                game_won = 1;
                pacman.update(level, map);
                ghost_manager.update(level, map, pacman);

                // Checking each cell in the map.
                for(const std::array<Cell, MAP_HEIGHT>& column : map)
                {
                    for(const Cell& cell : column)
                    {
                        if(Cell::Pellet == cell)
                        {
                            // If pellet is present, game_won = false.
                            game_won = 0;
                            break;
                        }
                    }
                }
            }

            // Restarting level if we die or starting next level if we win.
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                game_won = 0;

                if(pacman.get_dead())
                {
                    level = 0;
                }
                else
                {
                    level++;
                }

                // Funtion that converts sketch into map.
                map = convert_sketch(map_sketch, ghost_positions, pacman);

                ghost_manager.reset(level, ghost_positions);

                pacman.reset();
            }

            // Here we draw everything to our window i.e Pacman, Ghosts, Map, Pellet, Energizer.
            if(FRAME_DURATION > lag)
            {
                window.clear();

                // If pacman is dead or level won then we only draw text.
                if(!game_won && !pacman.get_dead())
                {
                    draw_map(map, window);

                    ghost_manager.draw(GHOST_FLASH_START >= pacman.get_energizer_timer(), window);

                    draw_text(0, 0, CELL_SIZE * MAP_HEIGHT, "Level:" + std::to_string(1+level), window);
                }

                pacman.draw(game_won, window);

                // Drawing text.
                if( pacman.get_animation_over())
                {
                    if(game_won)
                    {
                        draw_text(1, 0, 0, "Next level!", window);
                    }
                    else
                    {
                        draw_text(1, 0, 0, "Game over", window);
                    }
                }

                window.display();
            }
            
        }

    }

    return 0;
}
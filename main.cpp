#include <iostream>
#include <array>
#include <chrono>
#include <ctime>
#include <SFML/Graphics.hpp>

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

    //Similar to lag, used to make the game framerate-independent.
	std::chrono::time_point<std::chrono::steady_clock> previous_time;

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

    std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> map{};
	std::array<Position, 4> ghost_positions;

    sf::Event event;
    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * MAP_WIDTH * SCREEN_RESIZE, (FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT) * SCREEN_RESIZE), "Pacman", sf::Style::Close);
	window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * MAP_WIDTH, FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT)));

    GhostManager ghost_manager;
    Pacman pacman;

    srand(static_cast<int>(time(0)));

    map = convert_sketch(map_sketch, ghost_positions, pacman);
    ghost_manager.reset(level, ghost_positions);

    //Get the current time and store it in a variable.
	previous_time = std::chrono::steady_clock::now();

    while (window.isOpen())
    {
        int delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time).count();

        lag += delta_time;
        previous_time += std::chrono::microseconds(delta_time);

        while(FRAME_DURATION <= lag)
        {
            lag -= FRAME_DURATION;

            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            if(!game_won && !pacman.get_dead())
            {
                game_won = 1;
                pacman.update(level, map);
                ghost_manager.update(level, map, pacman);

                for(const std::array<Cell, MAP_HEIGHT>& column : map)
                {
                    for(const Cell& cell : column)
                    {
                        if(Cell::Pellet == cell)
                        {
                            game_won = 0;
                            break;
                        }
                        
                        if(!game_won)
                        {
                            break;
                        }
                        else
                        {
                            pacman.set_animation_timer(0);
                        }
                    }
                }
            }

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

                map = convert_sketch(map_sketch, ghost_positions, pacman);

                ghost_manager.reset(level, ghost_positions);

                pacman.reset();
            }

            if(FRAME_DURATION > lag)
            {
                window.clear();

                if(!game_won && !pacman.get_dead())
                {
                    draw_map(map, window);

                    ghost_manager.draw(GHOST_FLASH_START >= pacman.get_energizer_timer(), window);

                    draw_text(0, 0, CELL_SIZE * MAP_HEIGHT, "Level:" + std::to_string(1+level), window);
                }

                pacman.draw(game_won, window);

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
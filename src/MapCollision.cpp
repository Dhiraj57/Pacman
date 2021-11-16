#include <array>
#include <cmath>
#include <SFML/Audio.hpp>

#include "Headers/Global.hpp"
#include "Headers/MapCollision.hpp"

bool map_collision(bool i_collect_pellets, bool i_use_door, int i_x, int i_y, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map)
{
    bool output = 0;

    // Getting the exact position.
    float cell_x = i_x / static_cast<float>(CELL_SIZE);
    float cell_y = i_y / static_cast<float>(CELL_SIZE);

    // To loop through all possible positions that Pacman/Ghost can move to. i.e Checking cells from all 4 directions.
    for(int a=0; a<4; a++)
    {
        int x = 0;
        int y = 0;

        switch (a)
        {
            case 0: // Top left cell
            {
                x = static_cast<int>(floor(cell_x));
                y = static_cast<int>(floor(cell_y));
                break;
            }

            case 1: // Top right cell
            {
                x = static_cast<int>(ceil(cell_x));
                y = static_cast<int>(floor(cell_y));
                break;
            }

            case 2: // Bottom left cell
            {
                x = static_cast<int>(floor(cell_x));
                y = static_cast<int>(ceil(cell_y));
                break;
            }

            case 3: // Bottom right cell
            {
                x = static_cast<int>(ceil(cell_x));
                y = static_cast<int>(ceil(cell_y));
                break;
            }
        }

        // Making sure that the position is inside the map.
        if(x >= 0 && y >= 0 && x < MAP_WIDTH && y < MAP_HEIGHT)
        {
            if(i_collect_pellets == 0) // Here we only care about the walls.
            {
                if(Cell::Wall == i_map[x][y])
                {
                    output = 1;
                }
                else if (i_use_door == 0 && Cell::Door == i_map[x][y])
                {
                    output = 1;
                }  
            }
            else // Here we only care about the collectables.
            {
                if(Cell::Energizer == i_map[x][y])
                {
                    output = 1;
                    i_map[x][y] = Cell::Empty;
                }
                else if(Cell::Pellet == i_map[x][y])
                {
                    
                    i_map[x][y] = Cell::Empty;
                }
            }
        }
    }

    return output;
}
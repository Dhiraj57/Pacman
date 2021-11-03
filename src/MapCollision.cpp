#include <array>
#include <cmath>

#include "Headers/Global.hpp"
#include "Headers/MapCollision.hpp"

bool map_collision(bool i_collect_pellets, bool i_use_door, int i_x, int i_y, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map)
{
    bool output = 0;

    float cell_x = i_x / static_cast<float>(CELL_SIZE);
    float cell_y = i_y / static_cast<float>(CELL_SIZE);

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

        if(x >= 0 && y >= 0 && x < MAP_WIDTH && y < MAP_HEIGHT)
        {
            if(i_collect_pellets == 0)
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
            else
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
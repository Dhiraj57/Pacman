#pragma once

bool map_collision(bool i_collect_pellets, bool i_use_door, int i_x, int i_y, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map);
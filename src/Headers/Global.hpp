#pragma once

constexpr int CELL_SIZE = 16;
constexpr int MAP_HEIGHT = 21;
constexpr int MAP_WIDTH = 21;
constexpr float SCREEN_RESIZE = 2.5;

constexpr int FONT_HEIGHT = 16;
constexpr int FRAME_DURATION = 16667;

// Which cell in front of Pacman the pink ghost will target.
constexpr int GHOST_1_CHASE = 2;
// Which cell in front of Pacman the blue ghost will use to calculate the target.
constexpr int GHOST_2_CHASE = 1;
// If the distance between the orange ghost and Pacman is less than this value, the orange ghost will go into the scatter mode.
constexpr int GHOST_3_CHASE = 4;

// Number of frames in the ghost body animation.
constexpr int GHOST_ANIMATION_FRAMES = 6;
constexpr int GHOST_ANIMATION_SPEED = 4;
constexpr int GHOST_ESCAPE_SPEED = 4;
constexpr int GHOST_FRIGHTENED_SPEED = 3;
constexpr int GHOST_SPEED = 1;

constexpr int PACMAN_ANIMATION_FRAMES = 6;
constexpr int PACMAN_ANIMATION_SPEED = 4;
constexpr int PACMAN_DEATH_FRAMES = 12;
constexpr int PACMAN_SPEED = 2;


// This is in frames.
constexpr int CHASE_DURATION = 1024;
constexpr int ENERGIZER_DURATION = 512;
constexpr int GHOST_FLASH_START = 64;
constexpr int LONG_SCATTER_DURATION = 512;
constexpr int SHORT_SCATTER_DURATION = 256;


// Cell types.
enum Cell
{
	Door,
	Empty,
	Energizer,
	Pellet,
	Wall
};


struct Position
{
	int x;
	int y;

	// '==' is overloded to check position is same or not.
	bool operator==(const Position& i_position)
	{
		return this->x == i_position.x && this->y == i_position.y;
	}
};
#include <array>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Headers/Global.hpp"
#include "Headers/Pacman.hpp"
#include "Headers/MapCollision.hpp"

Pacman::Pacman() 
{
    animation_over = 0;
    dead = 0;
    direction = 0;
    energizer_timer = 0;
    position = {0,0};
}

bool Pacman::get_animation_over()
{
    return animation_over;
}

bool Pacman::get_dead()
{
    return dead;
}

int Pacman::get_direction()
{
    return direction;
}

int Pacman::get_energizer_timer()
{
    return energizer_timer;
}

void Pacman::draw(bool i_vicotry, sf::RenderWindow& i_window)
{
    int frame = static_cast<int>(floor(animation_timer / static_cast<float>(PACMAN_ANIMATION_SPEED)));

    sf::Sprite sprite;
    sf::Texture texture;

    sprite.setPosition(position.x, position.y);

    if(dead || i_vicotry)
    {
        if(animation_timer < PACMAN_DEATH_FRAMES * PACMAN_ANIMATION_SPEED)
        {
            animation_timer++;

            texture.loadFromFile("Resources/Images/PacmanDeath" + std::to_string(CELL_SIZE) + ".png");

            sprite.setTexture(texture);
            sprite.setTextureRect(sf::IntRect(CELL_SIZE * frame, 0, CELL_SIZE, CELL_SIZE));
            
            i_window.draw(sprite);
        }
        else
        {
            animation_over = 1;
        }
    }
    else
    {
        texture.loadFromFile("Resources/Images/PacmanDeath" + std::to_string(CELL_SIZE) + ".png");

        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(CELL_SIZE * frame, 0, CELL_SIZE, CELL_SIZE));

        i_window.draw(sprite);

        animation_timer = (1 + animation_timer) % (PACMAN_ANIMATION_FRAMES * PACMAN_ANIMATION_SPEED);
    }
}

void Pacman::reset()
{
    animation_over = 0;
	dead = 0;
	direction = 0;
	animation_timer = 0;
	energizer_timer = 0;
}

void Pacman::set_animation_timer(int i_animation_timer)
{
    animation_timer = i_animation_timer;
}

void Pacman::set_dead(bool i_dead)
{
    dead = i_dead;

    if(dead)
    {
        animation_timer = 0;
    }
}

void Pacman::update(int i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map)
{
    std::array<bool, 4> walls{};
    walls[0] = map_collision(0, 0, PACMAN_SPEED + position.x, position.y, i_map);
    walls[1] = map_collision(0, 0, position.x, position.y - PACMAN_SPEED, i_map);
    walls[2] = map_collision(0, 0, position.x - PACMAN_SPEED, position.y, i_map);
    walls[3] = map_collision(0, 0, position.x, position.y + PACMAN_SPEED, i_map);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        if(walls[0] == 0)
        {
            direction = 0;
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        if(walls[1] == 0)
        {
            direction = 1;
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        if(walls[2] == 0)
        {
            direction = 2;
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        if(walls[3] == 0)
        {
            direction = 3;
        }
    }

    if(walls[direction] == 0)
    {
        switch(direction)
        {
            case 0:
            {
                position.x += PACMAN_SPEED;
                break;
            }

            case 1:
            {
                position.y -= PACMAN_SPEED;
                break;
            }

            case 2:
            {
                position.x -= PACMAN_SPEED;
                break;
            }

            case 3:
            {
                position.y += PACMAN_SPEED;
                break;
            }
        }
    }

    if(-CELL_SIZE >= position.x)
    {
        position.x = CELL_SIZE * MAP_WIDTH - PACMAN_SPEED;
    }
    else if(CELL_SIZE * MAP_WIDTH <= position.x)
    {
        position.x = PACMAN_SPEED - CELL_SIZE;
    }

    if(map_collision(1, 0, position.x, position.y, i_map))
    {
        energizer_timer = static_cast<int>(ENERGIZER_DURATION / pow(2, i_level));
    }
    else
    {
        energizer_timer = std::max(0, energizer_timer - 1);
    }
}

Position Pacman::get_position()
{
    return position;
}


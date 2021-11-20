#include <array>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Headers/Global.hpp"
#include "Headers/Pacman.hpp"
#include "Headers/Ghost.hpp"
#include "Headers/MapCollision.hpp"

Ghost::Ghost(int i_id)
{
    id = i_id;
}

// We check cells from all 4 directions.
bool Ghost::pacman_collision(const Position& i_pacman_position)
{
    // We check collision by adding and subtracting one cell size from Pacmans position (We get a range).
    // If ghost position is present in that range then collision is present.
    if(position.x > i_pacman_position.x - CELL_SIZE && position.x < i_pacman_position.x + CELL_SIZE)
    {
        if(position.y > i_pacman_position.y - CELL_SIZE && position.y < i_pacman_position.y + CELL_SIZE)
        {
            return 1;
        }
    }

    return 0;
}


float Ghost::get_target_distance(int i_direction)
{
    int x = position.x;
    int y = position.y;

    // We'll imaginarily move the gohst in a given direction and calculate the distance to the target.
    switch (i_direction)
    {
        case 0:
        {
            x += GHOST_SPEED;
            break;
        }
        case 1:
        {
            y -= GHOST_SPEED;
            break;
        }
        case 2:
        {
            x -= GHOST_SPEED;
            break;
        }
        case 3:
        {
            y += GHOST_SPEED;
            break;
        }
    }

    // Used Pythagoras' theorem to calculate the distance.
    return static_cast<float>(sqrt(pow(x - target.x, 2) + pow(y - target.y, 2)));
}


void Ghost::draw(bool i_flash, sf::RenderWindow& i_window)
{
    // To get the Current frame of the animation.
    int body_frame = static_cast<int>(floor(animation_timer / static_cast<float>(GHOST_ANIMATION_SPEED)));
    // We move to next frame when the timer value passes fixed time interval (ghost animation speed).

    sf::Sprite body;
    sf::Sprite face;
    sf::Texture texture;

    texture.loadFromFile("src/Resources/Images/Ghost" + std::to_string(CELL_SIZE) + ".png");
    body.setTexture(texture);
    body.setPosition(position.x, position.y);

    // Animating ghost as per the animation frame.
    body.setTextureRect(sf::IntRect(CELL_SIZE * body_frame, 0, CELL_SIZE, CELL_SIZE));

    face.setTexture(texture);
    face.setPosition(position.x, position.y);

    if(frightened_mode == 0)
    {   
        switch (id)
        {
            case 0: 
            {
                body.setColor(sf::Color(255,0,0)); // Red
                break;
            }
            case 1:
            {
                body.setColor(sf::Color(255,188,255)); // Pink
                break;
            }
            case 2:
            {
                body.setColor(sf::Color(0,255,255)); // Cyan
                break;
            }
            case 3:
            {
                body.setColor(sf::Color(255,182,85)); // Orange
                break;
            }
        }

        face.setTextureRect(sf::IntRect(CELL_SIZE * direction, CELL_SIZE, CELL_SIZE, CELL_SIZE));
        i_window.draw(body);
    }

    else if (frightened_mode == 1)
    {
        body.setColor(sf::Color(36,36,255));
        face.setTextureRect(sf::IntRect(4 * CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));

        if(i_flash && body_frame % 2 == 0) // We change color after every 2 frames to get flash effect.
        {
            body.setColor(sf::Color(255,255,255));
            face.setColor(sf::Color(255,0,0));
        }
        else
        {
            body.setColor(sf::Color(36,36,255));
            face.setColor(sf::Color(255,255,255));
        }
        
        i_window.draw(body);
    }

    // Here we only draw face.
    else
    {
        face.setTextureRect(sf::IntRect(CELL_SIZE * direction, 2 * CELL_SIZE, CELL_SIZE, CELL_SIZE));
    }

    i_window.draw(face);

    // To loop animation timer in fixed range (0, GHOST_ANIMATION_FRAMES * GHOST_ANIMATION_SPEED).
    animation_timer = (1 + animation_timer) % (GHOST_ANIMATION_FRAMES * GHOST_ANIMATION_SPEED);
}


void Ghost::reset(const Position& i_home, const Position& i_home_exit)
{
    movement_mode = 0;
    use_door = 0 < id;

    direction = 0;
    frightened_mode = 0;
    frightened_speed_timer = 0;

    animation_timer = 0;

    home = i_home;
    home_exit = i_home_exit;
    target = i_home_exit;
}


void Ghost::set_position(int i_x, int i_y)
{
    position = {i_x, i_y};
}


void Ghost::switch_mode()
{
    movement_mode = 1 - movement_mode;
}


void Ghost::update(int i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, Ghost& i_ghost_0, Pacman& i_pacman)
{
    bool move = 0;

    // We check for all possible cells that the ghost can move to.
    int available_ways = 0;
    int speed = GHOST_SPEED;

    std::array<bool, 4> walls{};

    // Here the gohst starts and stops being frightened.
    if(frightened_mode == 0 && i_pacman.get_energizer_timer() == ENERGIZER_DURATION / pow(2, i_level))
    {
        frightened_speed_timer = GHOST_FRIGHTENED_SPEED; // To reduce ghost speed in frightened mode.
        frightened_mode = 1;
    }
    else if(i_pacman.get_energizer_timer() == 0 && frightened_mode == 1)
    {
        frightened_mode = 0;
    }

    // In case the gohst goes outside the grid.
    if(frightened_mode == 2 && position.x % GHOST_ESCAPE_SPEED == 0 && position.y % GHOST_ESCAPE_SPEED == 0)
    {
        speed = GHOST_ESCAPE_SPEED;
    }

    // To update the target position of the ghost.
    update_target(i_pacman.get_direction(), i_ghost_0.get_position(), i_pacman.get_position());

    // We check for walls in all 4 directions.
    walls[0] = map_collision(0, use_door, speed + position.x, position.y, i_map);
    walls[1] = map_collision(0, use_door, position.x, position.y - speed, i_map);
    walls[2] = map_collision(0, use_door, position.x - speed, position.y, i_map);
    walls[3] = map_collision(0, use_door, position.x, speed + position.y, i_map);


    // Here we set the direction of ghost.
    if(frightened_mode != 1)
    {
        int optimal_direction = 4;
        move = 1;

        // We check for all possible cells that the ghost can move.
        for(int a=0; a < 4; a++)
        {
            // Gohsts can't turn back (Unless they really have to).
            if(a == (direction + 2) % 4)
            {
                continue;
            }
            else if(walls[a] == 0)
            {
                if(optimal_direction == 4)
                {
                    optimal_direction = a;
                }

                available_ways++;

                if(get_target_distance(a) < get_target_distance(optimal_direction))
                {
                    // The optimal direction is the direction that's closest to the target.
                    optimal_direction = a;
                }
            }
        }

        if(available_ways > 1)
        {
            // When the gohst is at the intersection, it chooses the optimal direction.
            direction = optimal_direction;
        }
        else
        {   // If there's no other way, it turns back.
            if(optimal_direction == 4)
            {
                direction = (direction + 2) % 4;
            }
            else
            {
                direction = optimal_direction;
            }
        }
    }

    else
    {
        // To give random movement to frightened ghost.
        int random_direction = rand() % 4;

        // The gohst can move after a certain number of frames.
        if(frightened_speed_timer == 0)
        {
            move = 1;
            frightened_speed_timer = GHOST_FRIGHTENED_SPEED;

            for(int a=0; a < 4; a++)
            {
                if(a == (direction + 2) % 4)
                {
                    continue;
                }
                else if(walls[a] == 0)
                {
                    available_ways++;
                }
            }

            if( available_ways > 0)
            {
                while (walls[random_direction] == 1 || random_direction == (direction + 2) % 4)
                {
                    // We keep picking a random direction until we can use it.
                    random_direction = rand() % 4;
                }
                direction = random_direction;
            }
            else
            {
                // If there's no other way, it turns back.
                direction = (direction + 2) % 4;
            }
        }
        else
        {
            frightened_speed_timer--;
        }
    }

    // Actual movement of ghost in optimal direction.
    if(move == 1)
    {
        switch (direction)
        {
            case 0:
            {
                position.x += speed;
                break;
            }
            case 1:
            {
                position.y -= speed;
                break;
            }
            case 2:
            {
                position.x -= speed;
                break;
            }
            case 3:
            {
                position.y += speed;
                break;
            }
        }

        // Warping.
        if(-CELL_SIZE >= position.x)
        {
            position.x = CELL_SIZE * MAP_WIDTH - speed;
        }
        else if(position.x >= CELL_SIZE * MAP_WIDTH)
        {
            position.x = speed - CELL_SIZE;
        }
    }

    if(pacman_collision(i_pacman.get_position()) == 1)
    {
        if(frightened_mode == 0)
        {
            i_pacman.set_dead(1);
        }
        else
        {
            use_door = 1;
            frightened_mode = 2;
            target = home;
        }
    }
}


void Ghost::update_target(int i_pacman_direction, const Position& i_ghost_0_position, const Position& i_pacman_position)
{
    if(use_door == 1)
    {
        if(position == target)
        {
            if(home_exit == target)
            {
                use_door = 0;
            }
            else if (home == target)
            {
                frightened_mode = 0;
                target = home_exit;
            }   
        }
    }
    else
    {
        if(movement_mode == 0) // The scatter mode.
        {
            // Each gohst goes to the corner it's assigned to.
            switch (id)
            {
                case 0:
                {
                    target = {CELL_SIZE * (MAP_WIDTH - 1), 0};
                    break;
                }
                case 1:
                {
                    target = {0, 0};
                    break;
                }
                case 2:
                {
                    target = {CELL_SIZE * (MAP_WIDTH - 1), CELL_SIZE * (MAP_HEIGHT - 1)};
                    break;
                }
                case 3:
                {
                    target = {0, CELL_SIZE * (MAP_HEIGHT - 1)};
                    break;
                }
            }
        }
        else  // The chase mode.
        {
            switch (id)
            {
                case 0: // The red gohst will chase Pacman.
                {
                    target = i_pacman_position; 
                    break;
                }

                case 1: // The pink gohst will chase the 4th cell in front of Pacman.
                {
                    target = i_pacman_position;

                    switch (i_pacman_direction)
                    {
                        case 0:
                        {
                            target.x += CELL_SIZE * GHOST_1_CHASE;
                            break;
                        }
                        case 1:
                        {
                            target.y -= CELL_SIZE * GHOST_1_CHASE;
                            break;
                        }
                        case 2:
                        {
                            target.x -= CELL_SIZE * GHOST_1_CHASE;
                            break;
                        }
                        case 3:
                        {
                            target.y += CELL_SIZE * GHOST_1_CHASE;
                            break;
                        }
                    }
                    break;
                }

                case 2: // Blue
                {
                    target = i_pacman_position;

                    // Getting the second cell in front of Pacman.
                    switch (i_pacman_direction)
                    {
                        case 0:
                        {
                            target.x += CELL_SIZE * GHOST_2_CHASE;
                            break;
                        }
                        case 1:
                        {
                            target.y -= CELL_SIZE * GHOST_2_CHASE;
                            break;
                        }
                        case 2:
                        {
                            target.x -= CELL_SIZE * GHOST_2_CHASE;
                            break;
                        }
                        case 3:
                        {
                            target.y += CELL_SIZE * GHOST_2_CHASE;
                            break;
                        }
                    }

                    // We're sending a vector from the red gohst to the second cell in front of Pacman.
					// Then we're doubling it.
                    target.x += target.x - i_ghost_0_position.x;
                    target.y += target.y - i_ghost_0_position.y;
                    break;
                }

                case 3: // The orange gohst will chase Pacman until it gets close to him. Then it'll switch to the scatter mode.
                {
                    if(CELL_SIZE * GHOST_3_CHASE <= sqrt(pow(position.x - i_pacman_position.x, 2) + pow(position.y - i_pacman_position.y, 2)))
                    {
                        target = i_pacman_position;
                    }
                    else
                    {
                        target = {0, CELL_SIZE * (MAP_HEIGHT - 1)};
                    }
                }
            }
        }
    }
}


Position Ghost::get_position()
{
    return position;
}
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Headers/DrawText.hpp"
#include "Headers/Global.hpp"

void draw_text(bool i_center, int i_x, int i_y, const std::string& i_text, sf::RenderWindow& i_window)
{
    int character_x = i_x;
    int character_y = i_y;

    int character_width;

    sf::Sprite character_sprite;
    sf::Texture font_texture;

    font_texture.loadFromFile("src/Resources/Images/Font.png");

    // There are 96 characters in the font texture.
    character_width = font_texture.getSize().x / 96;
    character_sprite.setTexture(font_texture);

    if(i_center)
    {                                                                                           // To find number of characters upto '\n'.
        character_x = static_cast<int>(round(0.5f * (CELL_SIZE * MAP_WIDTH - character_width * i_text.substr(0, i_text.find_first_of('\n')).size())));
        character_y = static_cast<int>(round(0.5f * (CELL_SIZE * MAP_WIDTH - FONT_HEIGHT * (1 + std::count(i_text.begin(), i_text.end(), '\n')))));
    }                                                                                             // To count number of lines.

    for(std::string::const_iterator a = i_text.begin(); a != i_text.end(); a++)
    {
        if('\n' == *a)
        {
            if(i_center)
            {
                // To get x position of next line.                                                        // To find number of charaters between previous '\n' to next '\n' (a is position of previous '\n').
                character_x = static_cast<int>(round(0.5f * (CELL_SIZE * MAP_WIDTH - character_width * i_text.substr(1 + a - i_text.begin(), i_text.find_first_of('\n', 1 + a - i_text.begin()) - (1 + a - i_text.begin())).size())));
            }
            else
            {
                character_x = i_x;
            }

            character_y += FONT_HEIGHT;

            continue;
        }

        character_sprite.setPosition(character_x, character_y);

        // The font texture begins with a space character, which is the 32nd character.
        character_sprite.setTextureRect(sf::IntRect(character_width * (*a - 32), 0, character_width, FONT_HEIGHT));

        character_x += character_width;
        i_window.draw(character_sprite);
    }
}
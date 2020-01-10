#include <iostream>
#include <cstdlib>
 
typedef unsigned char UCHAR;
 
/*-------------------------------*/
enum class Direction
{
    left,
    up,
    right,
    down
};
 
class Snake
{
public:
    UCHAR height_position;
    UCHAR width_position;
   
    UCHAR previous_height_position{};
    UCHAR previous_width_position{};
   
    UCHAR next_height_position{};
    UCHAR next_width_position{};
 
    Snake(UCHAR map_height, UCHAR map_width)
    {
        height_position = map_height / 2;
        width_position = map_width / 2;
    }
};
 
void initialize_and_insert_map_border();
void cook_food_and_insert_on_map();
void update_snake_position_on_map();
void print_map();
 
Direction read_keyboard_input();
 
void move_snake(Direction direction);
void collect_food();
void game_over();
 
void init();
void update();
/*------------------------------*/
 
char border_element = '*';
char empty_element = ' ';
char food_element = '@';
 
constexpr UCHAR map_height{ 20 };
constexpr UCHAR map_width{ 40 };
char map_elements[map_height][map_width];
Snake snake{ map_height, map_width }; 
 
bool still_playing = true;
int actual_score{ 0 };
 
void initialize_and_insert_map_border()
{
    for(int height = 0; height < map_height; height++)
    {
        for(int width = 0; width < map_width; width++)
        {
            map_elements[height][width] = ' ';            
        }
    }
   
    for(int i = 0; i < map_width; i++)
    {
        map_elements[0][i] = border_element; // top border
        map_elements[map_height - 1][i] = border_element; // bottom border
    }
   
    for(int i = 0; i < map_height; i++)
    {
        map_elements[i][0] = border_element; // left border
        map_elements[i][map_width - 1] = border_element; // right border
    }
   
   
}
 
void cook_food_and_insert_on_map()
{
    UCHAR food_height = rand() % map_height;
    UCHAR food_width = rand() % map_width;
 
    if (map_elements[food_height][food_width] == empty_element)
        map_elements[food_height][food_width] = food_element;
    else
        cook_food_and_insert_on_map();
}
 
void update_snake_position_on_map()
{
    if (snake.previous_height_position != 0 && snake.previous_width_position != 0)
        map_elements[snake.previous_height_position][snake.previous_width_position] = empty_element;
 
    map_elements[snake.height_position][snake.width_position] = 'o';
}
 
void print_map()
{
    for (int height = 0; height < map_height; height++)
    {
        for (int width = 0; width < map_width; width++)
        {
            std::cout << map_elements[height][width];
        }
        std::cout << std::endl;
    }
 
    std::cout << "Aktualny wynik: " << actual_score << std::endl;
}
 
Direction read_keyboard_input()
{
    std::cout << "Wciśniej Enter żeby się poruszać (w,s,a,d): ";
 
    char input;
    std::cin >> input;
 
    switch (input)
    {
    case 'w':
        return Direction::up;
    case 'd':
        return Direction::right;
    case 's':
        return Direction::down;
    case 'a':
        return Direction::left;
    default:
        return Direction::up;
    }
}
 
void move_snake(Direction direction)
{
    snake.previous_height_position = snake.height_position;
    snake.previous_width_position = snake.width_position;
 
    switch (direction)
    {
    case Direction::left:
        snake.next_height_position = snake.height_position;
        snake.next_width_position = --snake.width_position;
        break;
    case Direction::up:
        snake.next_height_position = --snake.height_position;
        snake.next_width_position = snake.width_position;
        break;
    case Direction::right:
        snake.next_height_position = snake.height_position;
        snake.next_width_position = ++snake.width_position;
        break;
    case Direction::down:
        snake.next_height_position = ++snake.height_position;
        snake.next_width_position = snake.width_position;
        break;
    }
    char next_element = map_elements[snake.next_height_position][snake.next_width_position];
 
    if (next_element == empty_element)
    {
        update_snake_position_on_map();
    }
    else if(next_element == food_element)
    {
        collect_food();
        update_snake_position_on_map();
    }
    else if (next_element == border_element)
    {
        game_over();
    }
}
 
void collect_food()
{
    actual_score++;
    cook_food_and_insert_on_map();
}
 
void game_over()
{
    still_playing = false;
}
 
void init()
{
    initialize_and_insert_map_border();
    update_snake_position_on_map();
    cook_food_and_insert_on_map();
}
 
void update()
{
    system("clear"); 
 
    print_map();
   
    Direction next_move = read_keyboard_input();
    move_snake(next_move);
 
    if(still_playing)
        update(); 
    else
        std::cout << "Przegrałeś" << std::endl;
}
 
int main()
{
    init(); 
    update(); 
 
    return 0;
}
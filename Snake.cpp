#include <iostream>
#include <cstdlib>
#include <vector>
 

 
typedef unsigned char UCHAR;
 
/*-------------------------------*/
enum class Direction
{
    left,
    up,
    right,
    down
};
 
class Point2D
{
public:
    UCHAR height_position;
    UCHAR width_position;
 
    Point2D(UCHAR height_position_par, UCHAR width_position_par)
    {
        height_position = height_position_par;
        width_position = width_position_par;
    }
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
 
    std::vector<Point2D> tail{};
 
    UCHAR height_edge_of_whole_snake;
    UCHAR width_edge_of_whole_snake;
 
    Snake(UCHAR map_height, UCHAR map_width)
    {
        height_position = map_height / 2;
        width_position = map_width / 2;
    }
};
 
// map functions
void initialize_and_insert_map_border();
void spawn_food_and_insert_on_map();
void update_snake_position_on_map();
void print_map();
 
// input control
Direction read_keyboard_input();
 
// game operations
void move_snake(Direction direction);
void collect_food();
void game_over();
 
// main game functions
void init();
void update();
/*------------------------------*/
 
 
// map graphic
char border_element = '#';
char empty_element = ' ';
char food_element = 'F';
char snake_head_element = 'S';
char snake_tail_element = 'T';
 
// map parameters
constexpr UCHAR map_height{ 10 };
constexpr UCHAR map_width{ 30 };
char map_elements[map_height][map_width];
Snake snake{ map_height, map_width }; // Snake snake2 = Snake(map_width, map_height);
 
// game settings
bool still_playing = true;
int actual_score{ 0 };
 
void initialize_and_insert_map_border()
{
    for (int height = 0; height < map_height; height++)
    {
        for (int width = 0; width < map_width; width++)
        {
            map_elements[height][width] = ' ';
        }
    }
 
    for (int i = 0; i < map_width; i++)
    {
        map_elements[0][i] = border_element; // top border
        map_elements[map_height - 1][i] = border_element; // bottom border
    }
 
    for (int i = 0; i < map_height; i++)
    {
        map_elements[i][0] = border_element; // left border
        map_elements[i][map_width - 1] = border_element; // right border
    }
}
 
void spawn_food_and_insert_on_map()
{
    UCHAR food_height = rand() % map_height;
    UCHAR food_width = rand() % map_width;
 
    if (map_elements[food_height][food_width] == empty_element)
        map_elements[food_height][food_width] = food_element;
    else
        spawn_food_and_insert_on_map();
}
 
void move_tail()
{
    UCHAR previous_height_position;
    UCHAR previous_width_position;
 
    UCHAR next_height_position = snake.previous_height_position;
    UCHAR next_width_position = snake.previous_width_position;
 
    for (int i = 0; i < snake.tail.size(); i++)
    {
        if (i != 0)
        {
            next_height_position = previous_height_position;
            next_width_position = previous_width_position;
        }
 
        // save previous tail piece cords for next element
        previous_height_position = snake.tail[i].height_position;
        previous_width_position = snake.tail[i].width_position;
 
        // clear actual position
        map_elements[snake.tail[i].height_position][snake.tail[i].width_position] = empty_element;
 
        // move actual tail piece to next pos
        snake.tail[i].height_position = next_height_position;
        snake.tail[i].width_position = next_width_position;
        map_elements[snake.tail[i].height_position][snake.tail[i].width_position] = snake_tail_element;
 
        snake.height_edge_of_whole_snake = previous_height_position;
        snake.width_edge_of_whole_snake = previous_width_position;
    }
}
 
void update_snake_position_on_map()
{
    map_elements[snake.previous_height_position][snake.previous_width_position] = empty_element;
    map_elements[snake.height_position][snake.width_position] = snake_head_element;
 
    snake.height_edge_of_whole_snake = snake.previous_height_position;
    snake.width_edge_of_whole_snake = snake.previous_width_position;
 
    move_tail();
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
 
    std::cout << "Actual score: " << actual_score << std::endl;
    std::cout << "Actual tail size: " << snake.tail.size() << std::endl;
}
 
Direction read_keyboard_input()
{
    std::cout << "Enter your move (w,s,a,d): ";
 
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
    else if (next_element == food_element)
    {
        collect_food();
        update_snake_position_on_map();
    }
    else if (next_element == border_element || next_element == snake_tail_element)
    {
        game_over();
    }
}
 
void collect_food()
{
    actual_score++;
    if (snake.tail.size() == 0)
        snake.tail.push_back(Point2D(snake.previous_height_position, snake.previous_width_position));
    else
        snake.tail.push_back(Point2D(snake.height_edge_of_whole_snake, snake.width_edge_of_whole_snake));
 
    spawn_food_and_insert_on_map();
}
 
void game_over()
{
    still_playing = false;
}
 
void init()
{
    initialize_and_insert_map_border();
    map_elements[snake.height_position][snake.width_position] = snake_head_element; // insert snake on map
    spawn_food_and_insert_on_map();
}
 
void update()
{
    system("clear"); // clear console
 
    print_map();
 
    Direction next_move = read_keyboard_input();
    move_snake(next_move);
 
    if (still_playing)
        update(); // recursion
    else
        std::cout << "You have died." << std::endl;
}
 
int main()
{
    init(); // run only once
    update(); // loop function
 
    return 0;
}

#include <iostream>
#include <chrono>
#include <thread>
#include <string>

// Info for Sleep function:
// https://stackoverflow.com/questions/158585/how-do-you-add-a-timed-delay-to-a-c-program
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;
//this sets up namespaces so we don't have to spam std all over the place

const int WIDTH = 50;
const int HEIGHT = 20;
const float init_probability_alive = 0.5;
//variables that never change

// W => Windows
// M => Mac
// L => Linux
const char SYSTEM = 'W';
//OS selector

struct Cell{
    int sum = 0;
    bool alive = false;
    bool will_switch = false;
};
//struct is a collection of data, kinda like an object, this struct is named Cell

void clear_console(){
    switch(SYSTEM){
        case 'W':
            system("cls");
            //opens a command prompt
            break;
            //moves on with the program
        case 'M':
        case 'L':
            std::cout << "\x1B[2J\x1B[H";
            break;
        default:
            break;
    }
    //a better if/else statement (if W is true, it does this, if L is true, it does that)
}
//what this does is it wipes the display clean

bool is_init_alive(){
    // Get a random value between 0 and 1
    double r = ((double) rand() / (RAND_MAX));
    if (r < init_probability_alive){
        return true;
        //if r is 0
    }
    // There's no else statement here, will this still work?
    // yes, yes it will
    // if r is 1
    return false;
}
//so basically this thing is a 50/50 chance of being alive or ded

void init_game(Cell cell[HEIGHT][WIDTH]){
    for(int i = 0; i < HEIGHT; i++){
        //for how many pixels compose the Y length
        for(int j = 0; j < WIDTH; j++){
            //for how many pixels compose the X length
            bool is_alive = false;
            //is currently ded
            if(i > 0 && i < HEIGHT-1 && j > 0 && j < WIDTH-1){
                //if current Y is bigger than 0 AND current Y is less than Y limit AND the same things but with X
                is_alive = is_init_alive();
                //randomly determines if it's alive
            }
            cell[i][j].alive = is_alive;
            //confirms if this particular cell's alive or dead and applies it to the Cell
        }
    }
}

int main() {
    std::cout << "game start, type a number" << std::endl;
    int temp_seed;
    char temp;
    std::cin >> temp_seed;
    srand(temp_seed);
    //generates seed based on typed in number
    unsigned int round_count = 1;
    // big positive/zero number variable, never negative
    Cell cell[HEIGHT][WIDTH];
    //creates the x and y of the screen, each "pixel" contains a Cell object, this is known as an array
    init_game(cell);
    //creates game using the screen array we just made
    bool game_running = true;
    // Allow user to exit after x rounds
    int round_pause = 200;
    //game pauses after 200 rounds
    while(game_running){
        // Draw the grid
        bool all_dead = true;
        //everyone dies, the end
        for(int i = 0; i < HEIGHT; i++){
            for(int j = 0; j < WIDTH; j++){
                //for each item on the screen...
                if(cell[i][j].alive){
                    all_dead = false;
                    //if at least one cell is alive, all_dead is false
                }
                if(i > 0 && i < HEIGHT-1 && j > 0 && j < WIDTH-1){
                    //if current cell is on screen
                    int sum = 0;
                    //create sum variable which counts total living cells there
                    for(int k = -1; k <= 1; k++) {
                        //twice
                        for (int m = -1; m <= 1; m++)
                        //twice (total of 4 times)
                            {
                            // Check to make sure current coordinates isn't itself
                            if(k == 0 && m ==0){
                                continue;
                                //skip the rest of this loop
                            }
                            if(cell[i+k][j+m].alive){
                                sum += 1;
                                //if this cell is alive, adds 1 to sum
                            }
                        }
                    }
                    cell[i][j].sum = sum;
                    //confirms sum
                    if(cell[i][j].alive){
                        //if this cell is alive
                        if(sum > 3 || sum < 2){
                            //if sum is not 2 or 3
                            cell[i][j].will_switch = true;
                            //now cell can switch
                        }
                    }else{
                        //if dead
                        if(sum == 3){
                            cell[i][j].will_switch = true;
                            //now cell can switch
                        }
                    }
                }
            }
        }

        std::string curr_line;
        for(int i = 0; i < HEIGHT; i++){
            for(int j = 0; j < WIDTH; j++){
                //for this particular cell
                if(cell[i][j].alive){
                    //if alive, display 0
                    curr_line += "0 ";
                    //add 0 to the line
//                    std::cout << cell[i][j].sum << " ";
                }else{
                    //if dead, display .
                    curr_line += ". ";
                    //add . to the line
                }
                if (cell[i][j].will_switch)
                //if cell has the ability to switch
                {
                    cell[i][j].alive = !cell[i][j].alive;
                    //uh?
                    cell[i][j].will_switch = false;
                    //can no longer switch
                }
            }
            curr_line += '\n';
            //line break
        }

        clear_console();
//        sleep_until(system_clock::now() + 50ms);
        std::cout << curr_line << std::endl;
        //refreshes screen

        sleep_until(system_clock::now() + 50ms);
        if(all_dead){
            std::cout << "All life has been exterminated. Good job, hero." << std::endl;
            printf("It survived for %d rounds. Continue? Y/N",round_count);
            std::cin >> temp;
            if(temp == 'N' || temp == 'n'){
                printf("Good call. See ya.");
                game_running = false;
                //ends game
            }else{
                init_game(cell);
                //continues game
            }
        }
        if(round_count % round_pause == 0){
            printf("Paused at %d rounds. Enter n to escape, or anything else to continue.",round_count);
            std::cin >> temp;
            if(temp == 'N' || temp == 'n'){
                printf("Good call. See ya.\n");
                game_running = false;
                //ends game
            }
            if(temp == 'R' || temp == 'r'){
                init_game(cell);
                //continues game
            }
        }

        round_count++;
        //increase round number
//        std::cout << rand() << std::endl;
    }
    return 0;
}

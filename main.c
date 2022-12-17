
#include <stdio.h>
#include <stdlib.h>        // for srand
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#define rooms 20
#define pit1
#define pit2
#define player
#define wumpus

static int playerLocation = 0;
static int wumpusLocation = 0;
static int pit1Location = 0;
static int pit2Location = 0;

int currentPlayerLocation;
int currentWumpusLocation;
int currentPit1Location;
int currentPit2Location;
int incrementor;

bool playerAlive, wumpusAlive;


//Done => manually initialized array in ascending order
const static int wumpusMaze[20][4] = {
    {1, 2, 5, 8}, {2, 1, 3, 10}, {3, 2, 4, 12}, {4, 3, 5, 14}, {5, 1, 4, 6},
    {6, 5, 7, 15}, {7, 6, 8, 17}, {8, 1, 7, 9}, {9, 8, 10, 18}, {10, 2, 9, 11},
    {11, 10, 12, 19}, {12, 3, 11, 13}, {13, 12, 14, 20}, {14, 4, 13, 15},
    {15, 6, 14, 16}, {16, 15, 17, 20}, {17, 7, 16, 18}, {18, 9, 17, 19},
    {19, 11, 18, 20}, {20, 13, 16, 19}
};


//Done => displays the Maze layout to a player
void displayCave(void){
    
    printf( "       ______18______             \n"
            "      /      |       \\           \n"
            "     /      _9__      \\          \n"
            "    /      /    \\      \\        \n"
            "   /      /      \\      \\       \n"
            "  17     8        10     19       \n"
            "  | \\   / \\      /  \\   / |    \n"
            "  |  \\ /   \\    /    \\ /  |    \n"
            "  |   7     1---2     11  |       \n"
            "  |   |    /     \\    |   |      \n"
            "  |   6----5     3---12   |       \n"
            "  |   |     \\   /     |   |      \n"
            "  |   \\       4      /    |      \n"
            "  |    \\      |     /     |      \n"
            "  \\     15---14---13     /       \n"
            "   \\   /            \\   /       \n"
            "    \\ /              \\ /        \n"
            "    16---------------20           \n"
            "\n");
    
}




//Done => displays the provided instructions to a player
void displayInstructions(void)
{
    printf( "Hunt the Wumpus:                                             \n"
            "The Wumpus lives in a completely dark cave of 20 rooms. Each \n"
            "room has 3 tunnels leading to other rooms.                   \n"
            "                                                             \n"
            "Hazards:                                                     \n"
            "1. Two rooms have bottomless pits in them.  If you go there you fall and die.   \n"
            "2. The Wumpus is not bothered by the pits, as he has sucker feet. Usually he is \n"
            "   asleep. He will wake up if you enter his room. When you move into the Wumpus'\n"
            "   room, then he wakes and moves if he is in an odd-numbered room, but stays    \n"
            "   still otherwise.  After that, if he is in your room, he snaps your neck and  \n"
            "   you die!                                                                     \n"
            "                                                                                \n"
            "Moves:                                                                          \n"
            "On each move you can do the following, where input can be upper or lower-case:  \n"
            "1. Move into an adjacent room.  To move enter 'M' followed by a space and       \n"
            "   then a room number.                                                          \n"
            "2. Enter 'R' to reset the person and hazard locations, useful for testing.      \n"
            "3. Enter 'C' to cheat and display current board positions.                      \n"
            "4. Enter 'D' to display this set of instructions.                               \n"
            "5. Enter 'P' to print the maze room layout.                                     \n"
            "6. Enter 'G' to guess which room Wumpus is in, to win or lose the game!         \n"
            "7. Enter 'X' to exit the game.                                                  \n"
            "                                                                                \n"
            "Good luck!                                                                      \n"
            " \n\n");
    
    printf( "You are in room %d. ", currentPlayerLocation);

    //I use this code to display the appropriate message depending on user's location:
    //If a player on the Maze is close to Wumpus (1 tunnel away) => print the scary message
    //If a player is 1 tunnel away from the pit => then he/she will feel a draft
    unsigned int j = 0;
    for (j = 1; j < 4; j++){
        if (currentWumpusLocation == wumpusMaze[currentPlayerLocation - 1][j]){
            printf("\n");
            printf("You smell a stench. ");
        }
    }
    for (j = 1; j < 4; j++){
        if (currentPit1Location == wumpusMaze[currentPlayerLocation - 1][j] || currentPit2Location == wumpusMaze[currentPlayerLocation - 1][j]){
            printf("\n");
            printf("You feel a draft. ");
        }
    }
    printf("\n\n");
}


//Done => allows a user to exit a program
void quit(void){
    
    playerAlive = false;
    printf("\n");
    printf("Exiting Program ...\n");
}


//Done => accepts a digit from a user to activate the Move() function
int getUserNumber(char* displayMessage){

    int userNumber = 0;
    printf("%s: ", displayMessage);
    
    do{
        scanf("%d", &userNumber);
        
    }while(userNumber == 0);
    
    int result = userNumber;
  
    return result;
}


//Done => accepts a letter from a user to activate the switch() statement
int getUserLetter(char* displayMessage){

    char userLetterInput = '\n';
    
    printf("%s: ", displayMessage);
    
    do{
        scanf("%c", &userLetterInput);
        
    }while(userLetterInput == '\n');
    
    char result = toupper(userLetterInput);
  
    return result;
}


//Done => accepts a letter from a user to activate the switch() statement
int getLetterToMove(void){

    char userLetterInput = '\n';
    
    do{
        scanf("%c", &userLetterInput);
        
    }while(userLetterInput == '\n');
    
    char result = toupper(userLetterInput);
  
    return result;
}


//Done => produces the output when a player falls into a pit
void pitFall(void){
    
    printf("Aaaaaaaaahhhhhh....   \n");
    printf("    You fall into a pit and die. \n");
    quit();
}


//Done => produces the output depending on the Wumpus' location (If Wumpus won)
void wumpusWon(void){
    
    printf(    "You briefly feel a slimy tentacled arm as your neck is snapped. \n"
            "It is over.\n");
    quit();
}


//Done => I use this to organize my code inside Move() => produces the output depending on the Wumpus' location
void wumpusEscaped(void){

    printf( "You hear a slithering sound, as the Wumpus slips away. \n"
            "Whew, that was close! \n");
    
}


//Done => I use this to organize my code inside Move() => produces the output depending on the Wumpus' location
void invalidMove(void){
    
    printf("Invalid move.  Please retry.\n");
    //printf("\n"); //REMOVE LATER
    //printf("You are in room %d", currentPlayerLocation);
    //printf(".");
    //printf("\n\n");
}


//Done => I use this to organize my code inside Move() => produces the output depending on the Wumpus' location
void monsterMoves(void){

    printf( "You are in room %d. ", currentPlayerLocation);
    //printf(".");
    
    unsigned int j = 0;
    for(j = 1; j < 4; j++){
        if(currentWumpusLocation == wumpusMaze[currentPlayerLocation - 1][j]){
            printf("You smell a stench. ");
        }
    }
    for(j = 1; j < 4; j++){
        if(currentPit1Location == wumpusMaze[currentPlayerLocation -1][j] || currentPit2Location == wumpusMaze[currentPlayerLocation - 1][j]){
            printf("You feel a draft. ");
        }
    }
    printf("\n\n");
}


//Done => this function moves a player around the maze
void Move(int* moveNumber){
    
    int userTunnelChoice;
    static int round = 1; //new incrementor
    char userChoice = 'M';
    bool generator = true; //Generator = the heart of my loop. While it beats => we can move
 
   do{
        incrementor = 1; //incrementor
        
        //printf(".");
        //printf(" ");
        incrementor += 1;
        
        
        if(userChoice == 'M'){
               //printf("%d. ", *moveNumber);
               //printf(". Enter your move (or 'D' for directions): ");
               //printf("\n");
               int engine = 0; //Engine = traffic lights: 0 = red (Stop), 1 = green (Move)
               scanf(" %d", &userTunnelChoice);
            
               for(unsigned int j = 4; j > 0; j--){
                   if(userTunnelChoice == wumpusMaze[currentPlayerLocation - 1][j]){
                       engine = 1;
                       currentPlayerLocation = userTunnelChoice; //Move to the new location
                       *moveNumber++; //Increment the counter at the beginning of the printf()
                       generator = false;
                       break;
                       
                   }else{
                       engine = 0;
                   }
               }

           //Below: if user's move is valid, and the new location == pits'/wumpus' location (Wumpus is in the odd room number) => the function kills a player and terminates the loop. If room number is odd, then Wumpus moves to another location.
           if(engine == 1){
               if(currentPlayerLocation == currentPit1Location || currentPlayerLocation == currentPit2Location){
                   pitFall();
                   generator = false;
                   break;
               
           }else if(currentPlayerLocation == currentWumpusLocation){
               if((currentWumpusLocation % 2) == 0){
                   wumpusWon();
                   generator = false;
                   break;
                   
               } else{
                   wumpusEscaped();
                   currentWumpusLocation = wumpusMaze[currentWumpusLocation - 1][1];
                   monsterMoves();
                   continue;
               }
           }else{
               monsterMoves();
               continue;
           }
       }else{
               invalidMove();
               (*moveNumber)--;
               monsterMoves();
               break;
           }
       }
   }while(generator);
}


//Done: randomly sets all game elements on the map
void setLocations(void){
    
    srand(time(0));
    srand(1);
    //Position your Pits
    pit1Location = rand() % 20 + 1;
    currentPit1Location = pit1Location;
    pit2Location = rand() % 20 + 1;
    currentPit2Location = pit2Location;
    
    do{
        currentPit2Location = rand() % 20 + 1;
        
    }while(currentPit2Location == currentPit1Location || currentPit2Location == currentPlayerLocation || currentPit2Location == currentWumpusLocation);
    
    
    wumpusLocation = rand() % 20 + 1;
    currentWumpusLocation = wumpusLocation;
    do{
        currentWumpusLocation = rand() % 20 + 1;
        
    }while(currentWumpusLocation == currentPit2Location || currentWumpusLocation == currentPit1Location || currentWumpusLocation == currentPlayerLocation);
    
    playerLocation = rand() % 20 + 1;
    currentPlayerLocation = playerLocation;
    
    do{
        currentPlayerLocation = rand() % 20 + 1;
        
    }while(currentPlayerLocation == currentWumpusLocation || currentPlayerLocation == currentPit1Location || currentPlayerLocation == currentPit2Location);
}


//Done => sets Wumpus' location
void monstaLocations(void){
    
    do{
        currentWumpusLocation = rand() % 20 + 1;
        
    }while(currentWumpusLocation == currentPit2Location || currentWumpusLocation == currentPit1Location);
    
}


//Done => sets pits' locations
void pitsLocations(void){
    
    do{
        currentPit2Location = rand() % 20 + 1;
        currentPit1Location = rand() % 20 + 1;
        
    }while(currentPit2Location == currentPit1Location || currentPit2Location == currentWumpusLocation || currentPit1Location == currentPit2Location || currentPit1Location == currentWumpusLocation);
    
}


//Done => reset the current locations of all game elements depending on the user input
void reset(void){
    
    int resetPlayerR, resetWumpusR, resetpit1R, resetpit2R;
    int newPlayerR, newWumpusR, newPit1R, newPit2R;
   
    
    printf("Enter the room locations (1..20) for player, wumpus, pit1, and pit2: \n");
    scanf("%d %d %d %d", &resetPlayerR, &resetWumpusR, &resetpit1R, &resetpit2R);
    
    /*
        printf("Enter the room locations (1..20) for player, wumpus, pit1, and pit2: \n");
        newPlayerR = scanf("%d", &resetPlayerR);
        newWumpusR = scanf("%d", &resetWumpusR);
        newPit1R = scanf("%d", &resetpit1R);
        newPit2R = scanf("%d", &resetpit2R);*/
    //}
    
    currentPlayerLocation = resetPlayerR;
    currentWumpusLocation = resetWumpusR;
    currentPit1Location = resetpit1R;
    currentPit2Location = resetpit2R;
    
    do{
        newPit2R = rand() % 20 + 1;
    }while(currentPit2Location == currentPit1Location);
    
    do{
        newWumpusR = rand() % 20 + 1;
    }while(currentWumpusLocation == currentPit2Location);
    
    do{
        newPlayerR = rand() % 20 + 1;
    }while(currentPlayerLocation == currentWumpusLocation);
    
   
    printf("\n");
    monsterMoves();
}


//Done: see the current locations of all game elements
void cheating(void){
    
    printf("Cheating! Game elements are in the following rooms: \n"
            "Player Wumpus Pit1 Pit2  \n"
            "%4d %7d %5d %5d \n\n",
            currentPlayerLocation,
            currentWumpusLocation,
            currentPit1Location,
            currentPit2Location);
    
    printf("You are in room %d. ", currentPlayerLocation);
    
    for(unsigned int j = 1; j < 4; j++){
        if (currentWumpusLocation == wumpusMaze[currentPlayerLocation -1 ][j]){
            printf("You smell a stench. ");
        }
    }
    
    for(unsigned int j = 1; j < 4; j++){
        if (currentPit1Location == wumpusMaze[currentPlayerLocation - 1][j] || currentPit2Location == wumpusMaze[currentPlayerLocation - 1][j]){
            printf("You feel a draft. ");
        }
    }
    
    printf("\n\n");
    
}


//Done: guess the room where Wumpus is hiding => if you fail, you lose
void guess(void){
    
    int number = getUserNumber("Enter room (1..20) you think Wumpus is in");
    
    if(number == currentWumpusLocation){
        printf("You won!\n");
        quit();
    }
    else{
        printf("You lost.\n");
        quit();
    }
}


//Checks if 2 rooms are adjacent to each other. If so => return true. Else => return false
//I use it to check if we are next to pit1, pit2 or wumpus + valid moves
bool checkIfAdjacent(int tunnel1, int tunnel2){
    
    for(unsigned int j = 0; j < 3; j++){
        if (wumpusMaze[tunnel1][j] == tunnel2){
          return true;
        }
        else{
            return false;
        }
    }
    return false;
}


//Checks if the input room is valid => if so, move there. If not => stay where you are (Range: 0 to 20 + MUST be adjacent)
bool checkMove(int playersRoomChoice){
    
    if(playersRoomChoice < 1 || playersRoomChoice > rooms){
        if(checkIfAdjacent(currentPlayerLocation, playersRoomChoice) == false){
            return false;
        }
        else{
            return true;
        }
    }
   return true;
}


//Done: command controller => choose option using switch() based on the input from a player
void commands(int* moveNumber){

    //Sent as a parameter from main => catch with an asterisk
    printf("%d", *moveNumber);
    char choice = getUserLetter(". Enter your move (or 'D' for directions)");
    
    switch(choice){
        case 'X':
            quit();
            break;
        case 'P':
            printf("\n");
            displayCave();
            monsterMoves();
            return;
        case 'D':
            printf("\n");
            displayCave();
            displayInstructions();
            return;
        case 'C':
            cheating();
            return;
        case 'R':
            reset();
            return;
        case 'G':
            guess();
            break;
        case 'M':
            Move(moveNumber);
            (*moveNumber)++; //Use with an asterisk => increment by 1 each time player's move is valid
            break;
    }
}


int main(){
    
    //srand(time(0));
    int moveNumber = 1; //Increment by 1 + move switch in main()
    
    srand(1);
    playerAlive = false;
    wumpusAlive = false;
    //setLocations();
    
    //Set the random values
    currentPit1Location = rand() % 20 + 1;
    currentPit2Location = rand() % 20 + 1;
    while (currentPit2Location == currentPit1Location){
        currentPit2Location = rand() % 20 + 1;
    }
    
    currentWumpusLocation = rand() % 20 + 1;
    while (currentWumpusLocation == currentPit2Location){
        currentWumpusLocation = rand() % 20 + 1;
    }
    
    currentPlayerLocation = rand() % 20 + 1;
    while (currentPlayerLocation == currentWumpusLocation){
        currentPlayerLocation = rand() % 20 + 1;
    }
    
    printf( "You are in room %d", currentPlayerLocation);
    printf(". ");
    printf("\n");
    printf("\n");
    
    do{
        playerAlive = true;
        wumpusAlive = true;
        /*setLocations();
        monstaLocations();
        pitsLocations();
        printf("You are in room %d", currentPlayerLocation);
        printf("\n");*/
        commands(&moveNumber); //Pass with an ampersand
        
    }while (playerAlive == true);
    
    return 0;
}



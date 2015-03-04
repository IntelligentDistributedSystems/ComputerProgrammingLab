//Author: Dumitru Bogdan-Mihai
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <windows.h>
#include <ctime>
    #define xspaceship 17;
    #define yspaceship 15;
    #include "resource.h"

using namespace std;

struct Coordinates{
    int x, y;
};
    struct Coordinates   coord_player_spaceship;
    struct Coordinates   dim_matrix;
    struct Coordinates   bonus;
    struct Coordinates   malus; // malus remove your bonus

struct Present_Minim_Maxim{
    int present;
    int minim;
    int maxim;
};
    struct Present_Minim_Maxim     level;
    struct Present_Minim_Maxim     weapon; // player can fire 1, 2 or 3 rockets simultaneously

enum direction {lleft,rright,up,down};
    enum direction direction_enemy_move_x = rright;
    enum direction direction_enemy_move_Y = down;

        int i,j;
        char m[55][55]; // matricea campului de lupta
    int nr_lifes=3, nr_cheats, scor=0;
    int var;    char ch;
    int nr_Elements_HScore; // nr of saved scores
    int highScore_int[11]; // keep value of the score
        char highScore_char[11][55];
        char newScore_char[55];
        char cheats[10][15]; // strings of the cheats
        char cheat_entered[15];
    // each menu is saven in separate matrix.
    // Each menu have a number ( nr_menu ) and it keeps track of the current menu.
        char menu[16][16], menu_settings[16][22], menuHScores[15][18],menu_Instructions[15][15], menu_leved_customize[15][15];
        int nr_menu, nr_elements_menu[11], highlighted_menu_element;
    bool quit=false, sound=true, autoplay=false, level_runs=false, menu_is_displayed=true ;
    bool exists_Bonus = false, exists_Malus = false;
    bool level_was_changed = false;
    bool exists_player_rocker[36];

int poz_enemy_spaceships_x[100],poz_enemy_spaceships_y[100],nr_enemy_spaceships;
int y_limit_rand_move_enemy_spaceships;

void Finding_y_limit_rand_move_enemy_spaceships();
void Move_spaceship_random(int x, int y, int y_limit_rand_move_enemy_spaceships);
void Find_poz_enemy_spaceships_xy_AND_poz_player_spaceships();
void Move_Rand_Enemy_Spaceships();
void Move_Y_Enemy_Spaceships();
void Move_X_Enemy_Spaceships( double &i_Move_X_Enemy_Spaceships );
void Move_Enemy_Rockets();
void Move_Player_Rockets( double &i_Player_Fire );
void Move_Player_Spaceship(char direction_move_player);
void Fire_Rocket(int i,int j,char rocket);
void Enemy_fire();
void Player_Fire();
void Show_Menu( int nr_menu, int highlighted_menu_element);
void Enter ( int &nr_menu , int &highlighted_menu_element );
void Load_HScore();
void Save_HScore();
void Show_HScore();
void Swap_HScore(int a, int b);
void Sort_HScore();
void Add_New_HScore();
void Show_Matrix();
void Save_Game();
void int_to_char( int level, char *level_actual_char );
void Load_Level(int nrlevel);
void Save_Level(int nrlevel);
void Customize_Level(int nrlevel);
void Add_Bonus();
void Remove_Bonus();
void Add_Malus();
void Remove_Malus();
void PleaseWaitScreen();
void Initialization_Menu();
void Initialization_Variables();
void GotoXY( int y, int x);
void Delete_Character(int x , int y);
void Print(int x , int y, char c);
int GameOver();
void Add_Score(int bonus);
void Decrease_Life(int malus);
int Max(int c, int d);
void Auto_Play(double *time, double *i_Player_Fire, double *LAST_TIME_Player_Fire );

int main()
{
    srand ( time(NULL) );//for the function rand() to return different values every time

    std::clock_t start;
    start = std::clock();
    double time;
    time = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    // intervals for the various actions/functions of the game
        double LAST_TIME_Player_Fire = start;
        double LAST_TIME_Move_Player_Rockets = start;
        double LAST_TIME_Move_Enemy_Rockets = start;
        double LAST_TIME_Enemy_fire = start;
//      double LAST_TIME_Move_Player_Spaceship = start;
        double LAST_TIME_Move_X_Enemy_Spaceships = start;
//      double LAST_TIME_Move_Y_Enemy_Spaceships = start;
        double LAST_TIME_Move_Rand_Enemy_Spaceships = start;
        double LAST_TIME_Add_Bonus = start;
        double LAST_TIME_Add_Malus = start + 5.0;
        double LAST_TIME_modif_y_limit_rand_move_enemy_spaceships = start;

        double i_Player_Fire = 0.75;
        double i_Move_Player_Rockets = 0.015;
        double i_Move_Enemy_Rockets=0.45;
        double i_Enemy_fire=6.0;
//      double i_Move_Player_Spaceship=0.0;
        double i_Move_X_Enemy_Spaceships=0.70;
//      double i_Move_Y_Enemy_Spaceships=12.5;
        double i_Move_Rand_Enemy_Spaceships=0.2;
        double i_Add_Bonus=8.0;
        double i_Remove_Bonus=3.5;
        double i_Add_Malus=8.5;
        double i_Remove_Malus=5.0;
        double i_modif_y_limit_rand_move_enemy_spaceships=5.5;
        int i_Main = 35;

    //PleaseWaitScreen(); // useless, annoying but funny

    Initialization_Variables();
    Load_Level( level.present ); // level.present==0 and load last game ( level0 and index_level0 )
    Load_HScore();
    Initialization_Menu();

    Show_Menu(nr_menu, highlighted_menu_element);

    while(nr_lifes>0 && quit == false){
        time = ( std::clock() - start ) / (double) CLOCKS_PER_SEC ;

        if ( level_was_changed == true ){ //if the level was changed
            i_Move_X_Enemy_Spaceships=0.55;           // horizontal movement interval for enemy ships return to normal
            if( i_Enemy_fire - level.present >= 1.0 ) { // increase the speed which enemy fire
                i_Enemy_fire -= level.present;
            }
            level_was_changed = false;
        }

        if( level_runs==true ){ // DACA jucatorul jocaca jocul pr zis ( levelul ruleaza )
            GameOver(); // check if exist enemy ships

            if(autoplay==true){
                    Auto_Play(&time, &i_Player_Fire, &LAST_TIME_Player_Fire);
            }
            else{//autoplay==false
                if(( GetAsyncKeyState( 'A' ) & 0x8000 ) || ( GetAsyncKeyState( VK_LEFT ) & 0x8000 ) ){ // if "A" or LEFT key is pressed then move player spaceship to the left
                    Move_Player_Spaceship('A');
                }
                if(( GetAsyncKeyState( 'S' ) & 0x8000 ) || ( GetAsyncKeyState( VK_DOWN ) & 0x8000 ) ){
                    Move_Player_Spaceship('S');
                }
                if(( GetAsyncKeyState( 'D' ) & 0x8000 ) || ( GetAsyncKeyState( VK_RIGHT ) & 0x8000 ) ){
                    Move_Player_Spaceship('D');
                }
                if( ( GetAsyncKeyState( 'W' ) & 0x8000 ) || ( GetAsyncKeyState( VK_UP ) & 0x8000 ) ){
                    Move_Player_Spaceship('W');
                }
                if( GetAsyncKeyState( VK_SPACE ) & 0x8000 ){ // If space key is pressed
                    if(time - LAST_TIME_Player_Fire >= i_Player_Fire) { // present time - last time player fire > shooting interval
                       LAST_TIME_Player_Fire=time; // update LAST_TIME_Player_Fire
                       Player_Fire(); // bum bum bum // place 1/2/3 rocket/s on matrix above the players position. The rockets will be moved by another funciton
                    }
                }
            }
            if( GetAsyncKeyState( VK_ESCAPE ) & 0x8000 ){
                level_runs=false;
                menu_is_displayed=true;
                Show_Menu(nr_menu, highlighted_menu_element);
                if(sound == true){
                    PlaySound("audio\\pause.wav", NULL, SND_ASYNC);
                }
                Sleep(250);// Sleep(250) is important for this that would otherwise quickly swapped between game and menu and it would be very annoying
            }
            if(time-LAST_TIME_Move_Player_Rockets >= i_Move_Player_Rockets) { //On a well-established interval (i_Move_Player_Rockets) is moving player rockets (Move_Player_Rockets())
                LAST_TIME_Move_Player_Rockets=time;
                Move_Player_Rockets(i_Player_Fire);
            }
            if( time - LAST_TIME_Move_Enemy_Rockets >= i_Move_Enemy_Rockets) {
                LAST_TIME_Move_Enemy_Rockets=time;
                Move_Enemy_Rockets();
            }
            if(time-LAST_TIME_Enemy_fire >= i_Enemy_fire){
                LAST_TIME_Enemy_fire=time;
                Enemy_fire();
            }
            if(level.present<=5){ // up to level 5 enemies move only horizontally
                if(time-LAST_TIME_Move_X_Enemy_Spaceships >= i_Move_X_Enemy_Spaceships){
                    LAST_TIME_Move_X_Enemy_Spaceships=time;
                    Move_X_Enemy_Spaceships(i_Move_X_Enemy_Spaceships);
                }
            }
            else { // after lvl 5  enemies moves "random"
                if(time-LAST_TIME_Move_Rand_Enemy_Spaceships >= i_Move_Rand_Enemy_Spaceships){
                    LAST_TIME_Move_Rand_Enemy_Spaceships=time;
                    Move_Rand_Enemy_Spaceships();
                }

                if(time-LAST_TIME_modif_y_limit_rand_move_enemy_spaceships >= i_modif_y_limit_rand_move_enemy_spaceships){ // Spaceships moves random but have a lower limit that increases periodically
                    LAST_TIME_modif_y_limit_rand_move_enemy_spaceships=time;
                    if(y_limit_rand_move_enemy_spaceships < dim_matrix.y)
                        y_limit_rand_move_enemy_spaceships++;
                }
            }
            if(time-LAST_TIME_Add_Bonus >= i_Add_Bonus && exists_Bonus==false ){ // regularly appears a bonus (if the rocket touches this bonus it will gives bonuses / benefits to the player (more missiles to fire with or firing innterval descrease))
                if( weapon.present < weapon.maxim || i_Player_Fire >=0.20) {
                    LAST_TIME_Add_Bonus=time;
                    Add_Bonus();
                }
            }
                if(time-LAST_TIME_Add_Bonus >= i_Remove_Bonus && exists_Bonus==true){ // but the bonus is not staying in the sky forever ... it would be too easy: D
                    LAST_TIME_Add_Bonus=time;
                    Remove_Bonus();
                }
            if(time-LAST_TIME_Add_Malus >= i_Add_Malus && exists_Malus==false ){ // bonuses, bonuses but we need something to cancel their effect
                if ( weapon.present > 0  || i_Player_Fire!=0.75) {
                    LAST_TIME_Add_Malus=time;
                    Add_Malus();
                }
            }
                if(time-LAST_TIME_Add_Malus >= i_Remove_Malus && exists_Malus==true){
                    LAST_TIME_Add_Malus=time;
                    Remove_Malus();
                }
        }
        else
/*MENU*/if( menu_is_displayed==true ){
            Sleep(15);
            if( GetAsyncKeyState( VK_UP ) & 0x8000 ){ // W == go to the menu item above
                if(sound == true){
                    PlaySound("audio\\blink.wav", NULL, SND_ASYNC);
                }
                if(nr_elements_menu[ nr_menu ]!=0){ //When there's no menu element above the current one go to last one
                    if(highlighted_menu_element>0){
                        highlighted_menu_element--;
                    }
                    else
                        highlighted_menu_element = nr_elements_menu[ nr_menu ];

                    Show_Menu( nr_menu, highlighted_menu_element ); // redisplay the menu corresponding to the new value of the variable highlighted_menu_element
                    Sleep(50);
                }
            }
            if( GetAsyncKeyState( VK_DOWN ) & 0x8000 ) {
                if(sound == true){
                    PlaySound("audio\\blink.wav", NULL, SND_ASYNC);
                }
                if(nr_elements_menu[ nr_menu ]!=0){
                    if(highlighted_menu_element < nr_elements_menu[ nr_menu ]){
                        highlighted_menu_element++;
                    }
                    else highlighted_menu_element = 0;
                    Show_Menu( nr_menu, highlighted_menu_element );
                    Sleep(50);
                }
            }
            if( (GetAsyncKeyState( VK_RETURN ) & 0x8000) || ( GetAsyncKeyState( VK_SPACE ) & 0x8000 ) ){
                if(sound == true){
                    PlaySound("audio\\blink.wav", NULL, SND_ASYNC);
                }
                Enter( nr_menu , highlighted_menu_element ); // change the current menu or make an action
            }
            if( GetAsyncKeyState( VK_ESCAPE ) & 0x8000 ){ // close the menu and returns to the game or go to the main menu
                if(sound == true){
                    PlaySound("audio\\blink.wav", NULL, SND_ASYNC);
                }
                system("cls");
                if( nr_menu == 0 ){
                    menu_is_displayed=false;
                    level_runs=true;
                    Show_Matrix();
                }
                else {
                    nr_menu=0;
                    highlighted_menu_element=0;
                    Show_Menu( nr_menu, highlighted_menu_element );
                }
                Sleep(150);
            }
        }

        Sleep(i_Main);// primarily affects the movement of the player
    }

    if(quit==false){ // if the value of quit is false ( changed from menu )
        system("cls");
        cout<<"\n Game Over!";
        if(sound == true){
            PlaySound("audio\\gameover.wav", NULL, SND_ASYNC);
        }
        Sleep(1000);
        if( nr_lifes<=0 && ( scor > highScore_int[nr_Elements_HScore] || nr_Elements_HScore==0 || nr_Elements_HScore<10 ) ){//check for a new high score
            Add_New_HScore();
            Save_HScore();
            Sleep(1000);
            //Load_Level(1);
        }
    }
        Save_Game();
        system("cls");
        printf("\n Hope to see you soon");
        if(newScore_char[0]!='\0')
            printf(", %s!",newScore_char);
        printf("!");
        Sleep(2000);

    return 0;
}

void Finding_y_limit_rand_move_enemy_spaceships(){  //ships moves random but  with lower limit (y_limit_rand_move_enemy_spaceships). The limit is actually enemy spaceship from the lowest position Y coordinate
    int i,j;
    for(i=dim_matrix.y;i>=0;i--){
        for(j=0;j<=dim_matrix.x;j++){
            if(m[i][j]==char(4)){
                y_limit_rand_move_enemy_spaceships=i;
                return;
            }
        }
    }
}
void Move_spaceship_random(int x, int y, int y_limit_rand_move_enemy_spaceships){
    int rnd,ok=0,counter=0; // counter prevent the while running endlessly

    while(!ok && counter <= 25){
        rnd = rand()%4; // Gets values from 0 to 3 (equivalent to directions that can move each spaceship)
        counter++;
        switch( rnd ) {
            case 0: { // V
                if( x-1 >= 0 )
                    if(m[y][x-1] == char(32)){ // If the Western position is free (char (32) == '')
                        // And if there's no players rocket on column of the possible move (WARNING: this variable sometimes is lying: from a certain level even if exists rocket this variable still remains FALSE)( for different difficulty )
                        if(!exists_player_rocker[x-1])
                        {
                            m[y][x] = char(32);      // refresh the matrix in memory
                            Print(y,x,m[y][x]);      // refresh the new move on the screen
                            m[y][x-1] = char(4);      // refresh the matrix in memory
                            Print(y,x-1,m[y][x-1]); // refresh the new move on the screen
                            ok=1;                       //a move was made
                        }
                    }
                    else    if(m[y][x-1] == char(6)){ //If in the Western position is the player spaceship
                        m[y][x] = char(32);             // The enemy spaceship is removed and the life of player drops by 1
                        Print(y,x,m[y][x]);
                        Decrease_Life(1);
                    }
                break;
            }
            case 1: { // E
                if( x+1 <= dim_matrix.x )
                    if(m[y][x+1] == char(32)){
                        if(!exists_player_rocker[x+1])
                        {
                            m[y][x] = char(32);
                            Print(y,x,char(32));
                            m[y][x+1] = char(4);
                            Print(y,x+1,char(4));
                            ok=1;
                        }
                    }
                    else    if(m[y][x+1] == char(6)){
                        m[y][x] = char(32);
                        Print(y,x,m[y][x]);
                        Decrease_Life(1);
                    }
                break;
            }
            case 2: { //  S
                if( y-1 >= 0 )
                    if(m[y-1][x] == char(32)){
                        if(!exists_player_rocker[x])
                        {
                            m[y][x] = char(32);
                            Print(y,x,m[y][x]);
                            m[y-1][x] = char(4);
                            Print(y-1,x,m[y-1][x]);
                            ok=1;
                        }
                    }
                    else    if(m[y-1][x] == char(6)){
                        m[y][x] = char(32);
                        Print(y,x,m[y][x]);
                        Decrease_Life(1);
                    }
                break;
            }
            case 3: { //  N
                if( y+1 <= y_limit_rand_move_enemy_spaceships )
                    if(m[y+1][x] == char(32)){
                        if(!exists_player_rocker[x])
                        {
                            m[y][x] = char(32);
                            Print(y,x,m[y][x]);
                            m[y+1][x] = char(4);
                            Print(y+1,x,m[y+1][x]);
                            ok=1;
                        }
                    }

                    else    if(m[y+1][x] == char(6)){
                        m[y][x] = char(32);
                        Print(y,x,m[y][x]);
                        Decrease_Life(1);
                    }
                break;
            }
        }
    }
}
int X_Lowest_Enemy(){
    int distance_from_player=dim_matrix.x+1;
    int x_enemy;
    int contor;
    contor=nr_enemy_spaceships;
    while(poz_enemy_spaceships_y[nr_enemy_spaceships] == poz_enemy_spaceships_y[contor]){
        if( abs(poz_enemy_spaceships_x[contor]-coord_player_spaceship.x) < distance_from_player  ){
            distance_from_player=abs(poz_enemy_spaceships_x[contor]-coord_player_spaceship.x);
            x_enemy=poz_enemy_spaceships_x[contor];
        }
        contor--;
    }
    return x_enemy;
}
void Find_poz_enemy_spaceships_xy_AND_poz_player_spaceships(){ // necessary function for random movement (it reduces the executation time of the random function) ... It stores the coordinates of each enemy ship and coordinates of players rockets
    int i,j;
    nr_enemy_spaceships=-1;
    for(j=0;j<=dim_matrix.x;j++){
        exists_player_rocker[j]=false;
    }

    for(i=0;i<=dim_matrix.y;i++){
        for(j=0;j<=dim_matrix.x;j++){
            if(m[i][j]==char(4)){
                nr_enemy_spaceships++;
                poz_enemy_spaceships_x[nr_enemy_spaceships]=j;
                poz_enemy_spaceships_y[nr_enemy_spaceships]=i;
            }
            if(m[i][j]==char(2334) && level.present>=8){ // Until Level 7 are not taken into consideration player missiles for random movement
                exists_player_rocker[j]=true;
            }
        }
    }
}
void Move_Rand_Enemy_Spaceships(){
    int i;
    Find_poz_enemy_spaceships_xy_AND_poz_player_spaceships();
    for( i=0;i<=nr_enemy_spaceships; i++ ){
        Move_spaceship_random(poz_enemy_spaceships_x[i], poz_enemy_spaceships_y[i], y_limit_rand_move_enemy_spaceships);
    }
}
void Move_Y_Enemy_Spaceships(){//vertically

    bool exists_enemy_first_line=false, exists_enemy_last_line=false;
    if( direction_enemy_move_Y == down ) { // check if exists_enemy_first_line
        for(i=0;i<=dim_matrix.x;i++){
            if ( m[ dim_matrix.y ][i] == char(4) ) {
                exists_enemy_last_line=true;
                break;
            }
        }
        if ( exists_enemy_last_line == true ) {
            //direction_enemy_move_Y = up; // originally thought to move up when reach the bottom line but then I changed up so that when a spaceship reaches the last line will decreases player life
        }
    }/*
    else {// because never ( direction_enemy_move_Y == up )
        for(i=0;i<=dim_matrix.x;i++){
            if ( m[0][i] == char(4) ) {
                exists_enemy_first_line=true;
                break;
            }
        }
        if ( exists_enemy_first_line == true ){
            direction_enemy_move_Y = down;
        }
    }*/

    if ( direction_enemy_move_Y == down) {
        for(i=dim_matrix.y-1;i>=0;i--){
            for(j=0;j<=dim_matrix.x;j++){
                if(m[i][j]==char(4)){
                    switch (m[i+1][j]) {
                        case char(32) : {
                            m[i][j]=char(32);
                            Delete_Character(i,j);
                            m[i+1][j]=char(4);
                            Print(i+1,j,char(4));
                            break;
                        }
                        case '*' : {
                            m[i][j]=char(32);
                            Delete_Character(i,j);
                            m[i+1][j]=char(4);
                            Print(i+1,j,char(4));
                            break;
                        }
                        case char(6) : {//♠
                            m[i][j]=char(32);
                            Delete_Character(i,j);
                            Decrease_Life(1);
                            break;
                        }
                        case char(2334):{//▲
                            m[i][j]=char(32);
                            Delete_Character(i,j);
                            m[i+1][j]='*';
                            Print(i+1,j,char(4));
                            break;
                        }
                        case char(2335):{//▼
                            m[i][j]=char(32);
                            Delete_Character(i,j);
                            m[i+1][j]='*';
                            Print(i+1,j,char(4));
                            break;
                        }
                    }
                }
            }
        }
    }/*
    else {// because never ( direction_enemy_move_Y == up )
        for(i=1;i<=dim_matrix.y;i++){
            for(j=0;j<=dim_matrix.x;j++){
                if(m[i][j]==char(4)){
                    switch(m[i-1][j]) {
                        case char(32) : {
                            m[i][j]=char(32);
                            Delete_Character(i,j);
                            m[i-1][j]=char(4);
                            Print(i-1,j,char(4));
                            break;
                        }
                        case '*' : {
                            m[i][j]=char(32);
                            Delete_Character(i,j);
                            m[i-1][j]=char(4);
                            Print(i-1,j,char(4));
                            break;
                        }
                        case char(6) : {
                            m[i][j]=char(32);
                            Delete_Character(i,j);
                            Decrease_Life(1);
                            break;
                        }
                        case char(2334) : {
                            m[i][j]=char(32);
                            Delete_Character(i,j);
                            m[i-1][j]='*';
                            Print(i-1,j,'*');
                            break;
                        }
                        case char(2335) : {
                            m[i][j]=char(32);
                            Delete_Character(i,j);
                            m[i-1][j]='*';
                            Print(i-1,j,'*');
                            break;
                        }
                    }
                }
            }
        }
    }*/
}
void Move_X_Enemy_Spaceships( double &i_Move_X_Enemy_Spaceships ){ //horizontally  Move_X_Enemy_Spaceships
    int i,j;
    bool direction_was_changed=false;// when direction_was_changed=true i_Move_X_Enemy_Spaceships descrease and enemy go down

    switch (direction_enemy_move_x){//Looking for alien ship on the bottom line (if exists life decreases) + verify if exists enemy at lateral edges and change direction if they are.
        case rright : {
            for(i=0;i<=dim_matrix.x;i++){
                if(m[dim_matrix.y][i]==char(4)){
                    m[dim_matrix.y][i]=char(32);
                    Print(dim_matrix.y,i,char(32));
                    Decrease_Life(1);
                    break;
                }
                if( m[i][dim_matrix.x] == char(4) && i<= dim_matrix.y){ // O "smecherie" pt a nu mai face un alt for
                    direction_was_changed = true;
                }
            }
            if( direction_was_changed == true ) {
                direction_enemy_move_x = lleft;
                if(level.present>=3){
                    Move_Y_Enemy_Spaceships();
                }
                if(i_Move_X_Enemy_Spaceships - 0.10 >= 0.05){
                    i_Move_X_Enemy_Spaceships-=0.10;
                }
            }
            break;
        }
        case lleft : {
            for(i=0;i<=dim_matrix.x;i++){
                if(m[dim_matrix.y][i]==char(4)){
                    m[dim_matrix.y][i]=char(32);
                    Print(dim_matrix.y,i,char(32));
                    Decrease_Life(1);
                    break;
                }
                if( m[i][0] == char(4) && i<= dim_matrix.y ){ //trick for not doing another for
                    direction_was_changed = true;
                }
            }
            if( direction_was_changed == true ) {
                direction_enemy_move_x = rright;
                if(level.present>=3){
                    Move_Y_Enemy_Spaceships();
                }
                if(i_Move_X_Enemy_Spaceships - 0.10 >= 0.05){
                    i_Move_X_Enemy_Spaceships-=0.10;
                }
            }
            break;
        }
    }

    switch (direction_enemy_move_x){//actually horizontally move
        case rright : {
            for(int i=0;i<=dim_matrix.y;i++){
                for(int j=dim_matrix.x-1;j>=0;j--){
                    if( m[i][j] == char (4) ) {
                        switch ( m[i][j+1] ) {
                            case (char(32)) : {
                                 m[i][j] = char ( 32 );
                                Delete_Character(i,j);
                                Print(i,j+1, char(4));
                                m[i][j+1] = char ( 4 );
                                break;
                            }
                            case (char(2334)) : {
                                m[i][j] = char ( 32 );
                                Delete_Character(i,j);
                                Print(i,j+1, '*');
                                m[i][j+1] =  '*' ;
                                break;
                            }
                            case (char(2335)) : {
                                m[i][j] = char ( 32 );
                                Delete_Character(i,j);
                                Print(i,j+1, '*');
                                m[i][j+1] = '*';
                                break;
                            }
                            case ('*') : {
                                m[i][j] = char ( 32 );
                                Delete_Character(i,j);
                                Print(i,j+1, char(4));
                                m[i][j+1] = char ( 4 );
                                break;
                            }
                            case (char(6)) : {
                                m[i][j] = char ( 32 );
                                Delete_Character(i,j);
                                Decrease_Life(1);
                                break;
                            }
                        }
                    }
                }
            }
            break;
        }
        case lleft : {
            for(int i=0;i<=dim_matrix.y;i++){
                for(int j=1;j<=dim_matrix.x;j++){
                    if( m[i][j] == char (4) ) {
                        switch ( m[i][j-1] ) {
                            case (char(32)) : {
                                m[i][j] = char ( 32 );
                                Delete_Character(i,j);
                                Print(i,j-1, char(4));
                                m[i][j-1] = char ( 4 );
                                break;
                            }
                            case (char(2334)) : {
                                m[i][j] = char ( 32 );
                                Delete_Character(i,j);
                                Print(i,j-1, '*');
                                m[i][j-1] =  '*' ;
                                break;
                            }
                            case (char(2335)) : {
                                m[i][j] = char ( 32 );
                                Delete_Character(i,j);
                                Print(i,j-1, '*');
                                m[i][j-1] =  '*';
                                break;
                            }
                            case ('*') : {
                                m[i][j] = char ( 32 );
                                Delete_Character(i,j);
                                Print(i,j-1, char(4));
                                m[i][j-1] = char ( 4 );
                                break;
                            }
                            case (char(6)) : {
                                m[i][j] = char ( 32 );
                                Delete_Character(i,j);
                                Decrease_Life(1);
                                break;
                            }
                        }
                    }
                }
            }
            break;
        }
    }
}
void Move_Enemy_Rockets(){ // Move down each of the enemy rockets and if a rocket reaches player life will decrease
    int i,j;
    for(i=dim_matrix.y;i>=1;i--){
        for(j=0;j<dim_matrix.x;j++){
            if(m[i][j]==char(2335)){
                Delete_Character(i,j);
                m[i][j]=' ';

                switch (m[i+1][j]) {
                    case char(32) : {
                        Print(i+1,j,char(2335));
                        m[i+1][j]=char(2335);
                        break;
                    }
                    case '*' : {
                        Print(i+1,j,char(2335));
                        m[i+1][j]=char(2335);
                        break;
                    }
                    case char(2334) : {
                        Print(i+1,j,'*');
                        m[i+1][j]='*';
                        break;
                    }
                    case char(6) : {
                        Decrease_Life(1);
                        break;
                    }
                    case (char(219)) : { // blocks
                        m[i+1][j] = char(178);
                        Print(i+1,j,m[i+1][j]);
                        break;
                    }
                    case (char(178)) : {
                        m[i+1][j] = char(177);
                        Print(i+1,j,m[i+1][j]);
                        break;
                    }
                    case (char(177)) : {
                        m[i+1][j] = char(176);
                        Print(i+1,j,m[i+1][j]);
                        break;
                    }
                    case (char(176)) : {
                        m[i+1][j] = '*';
                        Print(i+1,j,m[i+1][j]);
                        break;
                    }
                }
            }
        }
    }
}
void Move_Player_Rockets( double &i_Player_Fire ){ // Move up each of the players rockets and if a rocket reaches an enemy spaceship it will destroy it
    int i,j;
    for(i=0;i<=dim_matrix.y;i++)
    {
        for(j=0;j<=dim_matrix.x;j++)
        {
            switch( m[i][j] ){
                case (char(2334)) : {
                    m[i][j]=' ';
                    Delete_Character(i,j);
                    switch (m[i-1][j]) {
                        case ( char(4) ) : {
                            m[i][j]=' ';
                            Delete_Character(i,j);
                            Add_Score(1);
                            m[i-1][j]='*';
                            Print(i-1,j,'*');
                            GameOver();
                            break;
                        }
                        case (char(2335)) : {
                            Add_Score(10);
                            m[i-1][j]='*';
                            Print(i-1,j,'*');
                            break;
                        }
                        case (char(783)) : {
                            if(sound == true){ PlaySound("audio\\powerup.wav", NULL, SND_ASYNC);}
                            Add_Score(20);
                            Remove_Bonus();
                            if( weapon.present < weapon.maxim ){
                                weapon.present ++;
                            }
                            else {
                                i_Player_Fire -= 0.10;
                            }
                            break;
                        }
                        case (char(769)) : {
                            if(sound == true){ PlaySound("audio\\bump.wav", NULL, SND_ASYNC);}
                            Remove_Malus();
                            if( weapon.present > 0 ){
                                weapon.present --;
                            }
                            else {
                                if( i_Player_Fire != 0.75){
                                    i_Player_Fire = 0.75;
                                }
                            }
                            break;
                        }
                        case (char(219)) : {
                            m[i-1][j] = char(178);
                            Print(i-1,j,char(178));
                            break;
                        }
                        case (char(178)) : {
                            m[i-1][j] = char(177);
                            Print(i-1,j,char(177));
                            break;
                        }
                        case (char(177)) : {
                            m[i-1][j] = char(176);
                            Print(i-1,j,char(176));
                            break;
                        }
                        case (char(176)) : {
                            m[i-1][j] = '*';
                            Print(i-1,j,m[i-1][j]);
                            break;
                        }
                        case (char(32)) : {
                            m[i-1][j]=char(2334);
                            Print(i-1,j,char(2334));
                            break;
                        }
                    }
                }
                case (char('*')) : {
                    m[i][j]=' ';
                    Print(i,j,char(32));
                }
            }
        }
    }
}
void Move_Player_Spaceship(char direction_move_player){ // Move_Player_Spaceship to S,N,V,E ( ~S,W,A,D )
    switch (direction_move_player) {
        case 'A': {
            if(coord_player_spaceship.x > 0){ // to not get out of the matrix surface
                if(m[coord_player_spaceship.y][coord_player_spaceship.x-1]==char(4) || m[coord_player_spaceship.y][coord_player_spaceship.x-1]==char(2335) || m[coord_player_spaceship.y][coord_player_spaceship.x-1]==char(2334) ){ // daca se atinge o spaceship extraterestra
                    // Cases where spaceship meets a rocket or an enemy
                    Decrease_Life(1);
                }
                else
                    if(m[coord_player_spaceship.y][coord_player_spaceship.x-1]!=char(219) && m[coord_player_spaceship.y][coord_player_spaceship.x-1]!=char(178) && m[coord_player_spaceship.y][coord_player_spaceship.x-1]!=char(177) && m[coord_player_spaceship.y][coord_player_spaceship.x-1]!=char(176)){
                        // otherwise simply move to the right
                        Delete_Character(coord_player_spaceship.y,coord_player_spaceship.x);
                        m[coord_player_spaceship.y][coord_player_spaceship.x]=char(32);
                        coord_player_spaceship.x--;
                        Print(coord_player_spaceship.y,coord_player_spaceship.x,char(6));
                        m[coord_player_spaceship.y][coord_player_spaceship.x]=char(6);
                    }
            }
            break;
        }

        case 'S': {
            if(coord_player_spaceship.y < dim_matrix.y){
                if(m[coord_player_spaceship.y+1][coord_player_spaceship.x]==char(4) || m[coord_player_spaceship.y+1][coord_player_spaceship.x]==char(2334) || m[coord_player_spaceship.y+1][coord_player_spaceship.x]==char(2335)){
                    Decrease_Life(1);
                }
                 else
                    if(m[coord_player_spaceship.y+1][coord_player_spaceship.x]!=char(219) && m[coord_player_spaceship.y+1][coord_player_spaceship.x]!=char(178) && m[coord_player_spaceship.y+1][coord_player_spaceship.x]!=char(177) && m[coord_player_spaceship.y+1][coord_player_spaceship.x]!=char(176)){
                        Delete_Character(coord_player_spaceship.y,coord_player_spaceship.x);
                        m[coord_player_spaceship.y][coord_player_spaceship.x]=char(32);
                        coord_player_spaceship.y++;
                        Print(coord_player_spaceship.y,coord_player_spaceship.x,char(6));
                        m[coord_player_spaceship.y][coord_player_spaceship.x]=char(6);
                    }
            }
            break;
        }

        case 'D': {
            if(coord_player_spaceship.x < dim_matrix.x){
                if(m[coord_player_spaceship.y][coord_player_spaceship.x+1]==char(4) || m[coord_player_spaceship.y][coord_player_spaceship.x+1]==char(2334) || m[coord_player_spaceship.y][coord_player_spaceship.x+1]==char(2335)){
                    Decrease_Life(1);
                }
                 else
                    if(m[coord_player_spaceship.y][coord_player_spaceship.x+1]!=char(219) && m[coord_player_spaceship.y][coord_player_spaceship.x+1]!=char(178) && m[coord_player_spaceship.y][coord_player_spaceship.x+1]!=char(177) && m[coord_player_spaceship.y][coord_player_spaceship.x+1]!=char(176)){
                        Delete_Character(coord_player_spaceship.y,coord_player_spaceship.x);
                        m[coord_player_spaceship.y][coord_player_spaceship.x]=char(32);
                        coord_player_spaceship.x++;
                        Print(coord_player_spaceship.y,coord_player_spaceship.x,char(6));
                        m[coord_player_spaceship.y][coord_player_spaceship.x]=char(6);
                    }
            }
            break;
        }

        case 'W': {
            if(coord_player_spaceship.y > dim_matrix.y-3){
                if(m[coord_player_spaceship.y-1][coord_player_spaceship.x]==char(4) || m[coord_player_spaceship.y-1][coord_player_spaceship.x]==char(2334) || m[coord_player_spaceship.y-1][coord_player_spaceship.x]==char(2335) ){
                    Decrease_Life(1);
                }
                if(m[coord_player_spaceship.y-1][coord_player_spaceship.x]!=char(219) && m[coord_player_spaceship.y-1][coord_player_spaceship.x]!=char(178) && m[coord_player_spaceship.y-1][coord_player_spaceship.x]!=char(177) && m[coord_player_spaceship.y-1][coord_player_spaceship.x]!=char(176)){
                    Delete_Character(coord_player_spaceship.y,coord_player_spaceship.x);
                    m[coord_player_spaceship.y][coord_player_spaceship.x]=char(32);
                    coord_player_spaceship.y--;
                    Print(coord_player_spaceship.y,coord_player_spaceship.x,char(6));
                    m[coord_player_spaceship.y][coord_player_spaceship.x]=char(6);
                }
            }
            break;
        }
    }
}

void Fire_Rocket(int i,int j,char rocket){// i could integrate this function in Player_Fire() function  BUT because the player can shoot 2-3 missiles once I preferred to do like that
    if( i>=0 && i<=dim_matrix.y && j>=0 && j<=dim_matrix.x){
        switch (m[i][j]) {
            case char(32) : {
                m[i][j]=rocket;
                Print(i,j,m[i][j]);
                break;
            }
            case char(2335) : {
                m[i][j]='*';
                Print(i,j,m[i][j]);
                break;
            }
            case char(2334) : {
                m[i][j]='*';
                Print(i,j,m[i][j]);
                break;
            }
            case char(219) : {
                m[i][j]=char(178);
                Print(i,j,m[i][j]);
                break;
            }
            case char(178) : {
                m[i][j]=char(177);
                Print(i,j,m[i][j]);
                break;
            }
            case char(177) : {
                m[i][j]=char(176);
                Print(i,j,m[i][j]);
                break;
            }
            case char(176) : {
                m[i][j]='*';
                Print(i,j,m[i][j]);
                break;
            }
        }
    }
}
void Enemy_fire(){ // shoot only aliens from the bottom line for not to happen to destroy themselves
    for(int i=dim_matrix.y; i>=0; i--) {
        for(int j=0;j<=dim_matrix.x; j++){
            if( m[i][j] == char (4) ){
                for(int  k=0; k <= dim_matrix.x; k++ ) {
                    if( coord_player_spaceship.x+k <= dim_matrix.x ){
                        if( m[i][coord_player_spaceship.x+k] == char(4) ){
                            if(i+1<=dim_matrix.y){
                                if( m[i+1][coord_player_spaceship.x+k] == char(32) ){
                                    Fire_Rocket(i+1, coord_player_spaceship.x+k, char(2335));
                                    i=-1;
                                    j=dim_matrix.x+1;
                                    k=dim_matrix.x+1;
                                    break;
                                }
                            }
                        }
                    }
                    if( coord_player_spaceship.x-k >= 0 ){
                        if( m[i][coord_player_spaceship.x-k] == char(4) ){
                            if(i+1<=dim_matrix.y){
                                if( m[i+1][coord_player_spaceship.x-k] == char(32) ){
                                    Fire_Rocket(i+1, coord_player_spaceship.x-k, char(2335));
                                    i=-1;
                                    j=dim_matrix.x+1;
                                    k=dim_matrix.x+1;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
void Player_Fire(){
    if(sound == true){
        PlaySound("audio\\kick.wav", NULL, SND_ASYNC);
    }
    switch(weapon.present){
        case 0: {// 1 rocket pop-up
            Fire_Rocket(coord_player_spaceship.y-1,coord_player_spaceship.x, char(2334));
            break;
        }
        case 1: {// 2 rockets pop-up
            Fire_Rocket(coord_player_spaceship.y-1,coord_player_spaceship.x-1, char(2334));
            Fire_Rocket(coord_player_spaceship.y-1,coord_player_spaceship.x+1, char(2334));
            break;
        }
        case 2: {// 3 rockets pop-up
            Fire_Rocket(coord_player_spaceship.y-1,coord_player_spaceship.x-1, char(2334));
            Fire_Rocket(coord_player_spaceship.y-1,coord_player_spaceship.x, char(2334));
            Fire_Rocket(coord_player_spaceship.y-1,coord_player_spaceship.x+1, char(2334));
            break;
        }
    }
}

void Show_Menu( int nr_menu, int highlighted_menu_element){// Each menu has the name elements saved in another array and has an identification number(nr_menu) .  So 0 is the main menu with items saved in the matrix: menu [] [], etc.
    system("cls");
    switch( nr_menu ){
       case (0) : { // main menu
           cout<<"\n Main Menu \n\n\n";
            for(i = 0; i<= nr_elements_menu[ nr_menu ] ; i++) {// Display the main menu items
                cout<<" ";
                for(j=0; j<=strlen(menu[i]);j++){
                    cout<<menu[i][j];
                    if( highlighted_menu_element == i ){  //put a space character between letters of underlined element
                        cout<<" ";
                    }
                }
                cout<<endl;
            }
            break;
        }

        case (1) : { // settings
            cout<<"\n Settings \n\n\n";
            for(int i = 0; i<= nr_elements_menu[ nr_menu ] ; i++) {
                cout<<" ";
                for(j=0; j<strlen(menu_settings[i]);j++){
                    cout<<menu_settings[i][j];
                    if( highlighted_menu_element == i ){
                        cout<<" ";
                    }
                }
                if(i==0){
                    cout<<":  ";
                    if(sound==true){
                        cout<<"On";
                    }
                    if(sound==false){
                        cout<<"Off";
                    }
                }
                else if(i==1){
                    cout<<":  ";
                    if(autoplay==true){
                        cout<<"On";
                    }
                    if(autoplay==false){
                        cout<<"Off";
                    }
                }
                cout<<endl;
            }
            break;
        }

        case (2) : { //high score
            cout<<"\n High Scores \n\n\n";
            for(i = 0; i<= nr_elements_menu[ nr_menu ] ; i++) {
                cout<<" ";
                for(j=0; j<=strlen(menuHScores[i]);j++){
                    cout<<menuHScores[i][j];
                    if( highlighted_menu_element == i ){
                        cout<<" ";
                    }
                }
                cout<<endl;
            }
            cout<<endl;
            Show_HScore();
            break;
        }
        case (3) : { // instructiuni
            cout<<"\n Instructions \n\n\n";
            cout<<" Use the W, A, S and D to control your spaceship \n\n";
            cout<<" Press SPACE to shoot the alien ships \n\n";
            cout<<" If you hit "<< char(783) <<" you will gain a bonus \n\n";
            cout<<" Try to avoid "<< char(769) <<" because it will cancel the bonuses received \n\n\n";
            for(int i = 0; i<= nr_elements_menu[ nr_menu ] ; i++) { // Just for back button
                cout<<" ";
                for(int j=0; j<=strlen(menu[i]);j++){
                    cout<<menu_Instructions[i][j];
                    if( highlighted_menu_element == i ){
                        cout<<" ";
                    }
                }
                cout<<endl;
            }
            break;
        }
        case (4) : {
            cout<<"\n Customize levels \n\n\n";
            for(i = 0; i<= nr_elements_menu[ nr_menu ] ; i++) {
                cout<<" ";
                for(j=0; j<=strlen(menu_leved_customize[i]);j++){
                    cout<<menu_leved_customize[i][j];
                    if( highlighted_menu_element == i ){
                        cout<<" ";
                    }
                }
                cout<<endl;
            }
            cout<<endl;
        }
    }
}
void Enter ( int &nr_menu , int &highlighted_menu_element ){ // When the ENTER key is pressed it takes into account the menu displayed and highlighted item (the item you press Enter)
    bool cheat_match;
    switch ( nr_menu ) {
        case (0) : { // main menu
            switch ( highlighted_menu_element) {
                case (0) : {// Resume
                    menu_is_displayed=false;
                    level_runs=true ;
                    Show_Matrix();
                    break;
                }
                case (1) : {//New Game
                    menu_is_displayed=false; level_runs=true ;
                    level.present=1;
                    Load_Level(level.present);
                    Show_Matrix();
                    break;
                }
                case (2) : { // high scores
                    nr_menu=2;
                    highlighted_menu_element=0;
                    Show_Menu(nr_menu , highlighted_menu_element);
                    Sleep(200);
                    break;
                }
                case (3) : {//instructions
                    nr_menu=3;
                    highlighted_menu_element=0;
                    Show_Menu(nr_menu , highlighted_menu_element);
                    Sleep(200);
                    break;
                }
                case (4) : {//Settings
                    nr_menu=1;
                    highlighted_menu_element=0;
                    Show_Menu(nr_menu , highlighted_menu_element);
                    Sleep(200);
                    break;
                }
                case (5) :{//Quit
                    Save_Game();
                    quit=true;
                    break;
                }
            }
            break;
        }
        case (1) : { // options
            switch(highlighted_menu_element){
                case 0 : {// volume control - if you press enter in the first position of options menu, it will be changed only the variable sound and the menu will be refreshed
                    if(sound==true)
                        sound=false;
                    else
                        sound=true;
                    Show_Menu(nr_menu,highlighted_menu_element);
                    Sleep(250);
                    break;
                }
                case 1 :{//Auto-play
                    if(autoplay==true)
                        autoplay=false;
                    else
                        autoplay=true;
                    Show_Menu(nr_menu,highlighted_menu_element);
                    Sleep(250);
                    break;
                }
                case 2 : { // cheats
                    cout<<"\nEnter your code : ";GotoXY(8,7);
                    cheat_entered[0]='\0';
                    cin.ignore();fflush(stdin);cin.get();
                    cin>>cheat_entered;
                    cheat_match=false;
                    for(int i=0;i<=nr_cheats;i++){
                        if( strcmp ( cheats[i],cheat_entered ) == 0 ){
                            switch (i){
                                case 0: {
                                    nr_lifes+=5;
                                    cout<<" Life +5 \n" ;
                                    break;
                                }
                                case 1: {
                                    scor+=100;
                                    cout<<" Score +100 \n";
                                    break;
                                }
                                case 2: {
                                    if(weapon.present < weapon.maxim){
                                        weapon.present++;
                                        cout<<" You have a new wapon \n";
                                    }
                                    break;
                                }
                                case 3: {
                                    Add_Bonus();
                                    GotoXY(9,20);
                                    cout<<" +Bonus!\n";
                                    break;
                                }
                            }
                            cheat_match=true;
                        }
                    }
                    if(!cheat_match){
                        cout<<"\n Ati gresit codul!";
                    }
                    Sleep(1250);
                    nr_menu=1;
                    highlighted_menu_element=1;
                    Show_Menu(nr_menu,highlighted_menu_element);
                    break;
                }
                case 3 : {// modificare leveluri
                    nr_menu=4;
                    highlighted_menu_element=0;
                    Show_Menu(nr_menu , highlighted_menu_element);
                    Sleep(200);
                    break;
                }
                case 4 : {//back
                    nr_menu=0;
                    highlighted_menu_element=0;
                    Show_Menu( nr_menu, highlighted_menu_element );
                    break;
                }
            }
            break;
        }

        case 2 : { // high scores
            switch (highlighted_menu_element) {
                case 0 : {//Reset high scores
                    FILE *file;
                    file = fopen("files\\HighScore.txt","w");
                    fprintf(file,"0");
                    fclose(file);
                    Load_HScore();
                    Show_Menu(nr_menu,highlighted_menu_element);
                    break;
                }
                case 1 : {//Back
                    nr_menu = 0 ;
                    highlighted_menu_element = 0 ;
                    Show_Menu(nr_menu,highlighted_menu_element);
                    break;
                }
            }
            break;
        }

        case 3 : { // instructions
            switch (highlighted_menu_element) {
                case 0 : {
                    nr_menu = 0 ;
                    highlighted_menu_element = 3 ;
                    Show_Menu(nr_menu,highlighted_menu_element);
                    Sleep(200);
                    break;
                }
            }
            break;
        }

        case 4 : { //customize levels
            switch ( highlighted_menu_element ) {
                case 10 : {// back
                    nr_menu=1;
                    highlighted_menu_element=3;
                    Show_Menu( nr_menu, highlighted_menu_element);
                    break;
                }
                default : {// from case 0 to 9
                    Customize_Level( highlighted_menu_element +1 );
                    nr_menu=1;
                    highlighted_menu_element=3;
                    Show_Menu( nr_menu, highlighted_menu_element);
                    break;
                }
            }
        }
        break;
    }
}
void Load_HScore(){     // Read from file HighScore.txt
    int aux;            // highScore_char [n] -> name of the player n
                        // highScore_int [n] -> score of the player n
    FILE *file;
    file = fopen("files\\HighScore.txt","r");
    fscanf(file, "%d", &nr_Elements_HScore); // Firs read number of elements
    for(i=1;i<=nr_Elements_HScore;i++){
        fscanf(file, "%d", &aux);
        j=0; // counter
        fscanf(file, "%c", &ch);// read char by char
        while ( ch!='=' ){ // until '=' . After = follows score value
            highScore_char[aux][j]=ch;
            j++;
            fscanf(file, "%c", &ch);
        }
        fscanf(file, "%d", &highScore_int[i]); // read score value
    }
    fclose(file);
}
void Save_HScore(){
    FILE *file;
    file = fopen("files\\HighScore.txt", "w");
    if(nr_Elements_HScore>10)
        nr_Elements_HScore=10;
    fprintf(file, "%d\n", nr_Elements_HScore);
    for(i=1;i<=nr_Elements_HScore;i++){
        fprintf(file, "%d ", i);
        for ( j=0;j<=strlen( highScore_char[i] ); j++ ){
                fprintf(file, "%c", highScore_char[i][j]);
        }
        fprintf(file, " = %d\n", highScore_int[i]);
    }
    fclose(file);
}
void Show_HScore(){ // Displays saved scores
    if(nr_Elements_HScore==0){ // IF nr_Elements_HScore = 0 ( there's no saved score)
        cout<<"\n Wasn't recorded any score ! ";
    }
    else { // nr_Elements_HScore!=0
        for( i=1;i<=nr_Elements_HScore;i++){
            cout<<"  "<<i<<". ";
            for( j=0; j<= strlen (highScore_char[i]) ; j++){
                cout<<highScore_char[i][j];
            }
            cout<<highScore_int[i]<<endl;
        }
    }
}
void Swap_HScore(int a, int b){ // function used in sorting process by Sort_HScore()
    int auxI;
    char auxC;

    auxI=highScore_int[a];
    highScore_int[a] = highScore_int[b];
    highScore_int[b] = auxI;

    for(int j=0; j<=Max( strlen(highScore_char[a]), strlen(highScore_char[b]) ); j++){
        auxC = highScore_char[a][j];
        highScore_char[a][j] = highScore_char[b][j];
        highScore_char[b][j] = auxC;
    }
}
void Sort_HScore(){ //sorting by value scores
    bool sorted=false; // bubble sort
    while( sorted==false ){
        sorted=true;
        for(int k=nr_Elements_HScore; k>1; k--){
            if( highScore_int[k]>highScore_int[k-1])
            {
                sorted=false;
                Swap_HScore( k, k-1 ); // swaps value of scores and names of the players
            }
        }
    }
}
void Add_New_HScore(){ // read and save name of the player and the new score
    system("cls");

    cout<<"\n Congratulations! \n You have a record score ! \n";
    Sleep(1000);
    cout<<" What is your name ? \n";
    fflush(stdin);
    cin>>newScore_char;
    int counter=-1;

    while(newScore_char[++counter]==' ' && counter<strlen(newScore_char))// Delete blank spaces
    {}
    strcpy(newScore_char,newScore_char+counter);// Delete blank spaces

    cin.ignore();fflush(stdin);

    nr_Elements_HScore++;
    strcpy(highScore_char[nr_Elements_HScore],newScore_char);
    highScore_int[nr_Elements_HScore]=scor;
    Sort_HScore();
    if(nr_Elements_HScore>10)
        nr_Elements_HScore=10;
    Load_Level(1);
    Save_Game();
}

void Show_Matrix(){
    system("cls"); // clear screen

    GotoXY(0,dim_matrix.x + 2); // print current level
        printf(" Level: %d" , level.present ) ;
    GotoXY(2,dim_matrix.x + 2); // print score
        printf(" Score : %d" , scor );
    GotoXY(4,dim_matrix.x + 2);
        if(nr_lifes>1){
            printf(" You have %d lifes! " , nr_lifes );
        }
        else {
            printf(" You have left just one life! " );
        }

    GotoXY(0,0);// to display the matrix from (0,0)
    for(i=0;i<=dim_matrix.y;i++){
        cout<<" ";
        for(j=0;j<=dim_matrix.x;j++ ){
            cout<<m[i][j];
        }
        cout<<"\n";
    }
    for(i=0;i<=dim_matrix.x+1;i++){//display the edges of matrix
        Print(dim_matrix.y+1,i,char(219));
        Print(-1,i,char(219));
    }
    for(i=0;i<=dim_matrix.y+1;i++){//display the edges of matrix
        Print(i,dim_matrix.x+1,char(219));
        Print(i,-1,char(219));
    }
    Print(-1,-1,char(219)); //last detail
}
void Save_Game(){ // Save the matrix, the number of lives left, score, ship's position in the file, so it can be reloaded for later.
    FILE *file_Index, *file_level0;
    file_Index = fopen( "files//index_level0.txt", "w" );
    file_level0 = fopen( "files//level0.txt", "w" );

    fprintf(file_Index,"%d %d %d %d %d", nr_lifes, scor, coord_player_spaceship.x, coord_player_spaceship.y, level.present);

    for(i=0;i<=dim_matrix.y;i++){
        for(j=0;j<=dim_matrix.x;j++){
            fprintf(file_level0, "%d ", int (m[i][j])); // Save decimal code of the chars
        }
        fprintf(file_level0, "\n");
    }
    fclose(file_Index);
    fclose(file_level0);
}

void int_to_char( int level, char *level_actual_char ){// used for creating the string with address of the file
    if(level<10){
        level_actual_char[0] = level + '0' ;
        level_actual_char[1] = '\0';
    }
    else {
        level_actual_char[0] = level/10 + '0' ;
        level_actual_char[1] = level%10 + '0' ;
        level_actual_char[2] = '\0';
    }
}
void Load_Level(int nrlevel){
    //Convention: level 0 represent last game played and saved in the files level0.txt and index_level0.txt
    int var=0;
    char file_address[150] ;
    char level_actual_char[4] ;
    FILE *file_level, *file_Index;

    strcpy(file_address, "files\\level");
    int_to_char( nrlevel, level_actual_char );
    strcat(file_address, level_actual_char);
    strcat(file_address, ".txt");
    file_level = fopen(file_address,"r");
    file_Index = fopen("files\\index_level0.txt","r");

    for(i=0;i<=dim_matrix.y;i++){
        for(j=0;j<=dim_matrix.x;j++ ){
            fscanf(file_level,"%d",&var);
            m[i][j]=char(var);
        }
    }
    if(nrlevel==0){
        fscanf(file_Index,"%d %d %d %d %d",&nr_lifes, &scor, &coord_player_spaceship.x, &coord_player_spaceship.y, &level.present);
    }
    else{
        if(nrlevel==1){
            scor=0;
            nr_lifes=3;
        }
        direction_enemy_move_x = rright;
        direction_enemy_move_Y = down;
        coord_player_spaceship.x=xspaceship;
        coord_player_spaceship.y=yspaceship;
        m[coord_player_spaceship.y][coord_player_spaceship.x]=char(6);
    }
    fclose(file_level);
    fclose(file_Index);
}
void Save_Level(int nrlevel){ // Used to customize any level right from the game
    char file_address[100] ;
    char level_actual_char[3] ;
    FILE *file_level;

    strcpy(file_address, "files\\level");
    int_to_char( nrlevel, level_actual_char );
    strcat(file_address, level_actual_char);
    strcat(file_address, ".txt");
    file_level = fopen(file_address,"w");

    for(i=0;i<=dim_matrix.y;i++){
        for(j=0;j<=dim_matrix.x;j++ ){
            fprintf(file_level, "%d ", int (m[i][j]));//Save decimal code of the character
        }
        fprintf(file_level, "\n");
    }
    fclose(file_level);
}
void Customize_Level(int nrlevel){ // You can customize the levels from game options
    int x=3,y=1,step=1;// step that the cursor moves on matrix
    Sleep(150);

    Load_Level(nrlevel);
    m[coord_player_spaceship.y][coord_player_spaceship.x]=char(6); //♠
    Show_Matrix();

    GotoXY(0,dim_matrix.x + 2);
        printf(" Use the arrows to navigate on the field " ) ;
    GotoXY(2,dim_matrix.x + 2);
        printf(" Space - Add enemy , B - Add BLOCK"  );
    GotoXY(4,dim_matrix.x + 2);
        printf(" BACKSPACE - remove element");
    GotoXY(6,dim_matrix.x + 2);
        printf(" Delete - remove all");
    GotoXY(8,dim_matrix.x + 2);
        printf(" ENTER - save the changes "  );
    GotoXY(10,dim_matrix.x + 2);
        printf(" level : %d ", nrlevel);
    GotoXY(12,dim_matrix.x + 2);
        printf(" Step = %d. Modif -/+ ", step);
    GotoXY(y,x);//

    while (1){
        if(  GetAsyncKeyState( VK_ADD ) & 0x8000 ) {
            if(step<=dim_matrix.y/2)
                step+=1;
            GotoXY(dim_matrix.y +1,dim_matrix.x + 2);
                printf(" step = %d", step);
        }
        if(  GetAsyncKeyState( VK_SUBTRACT ) & 0x8000 ) {
            if(step>0)
                step-=1;
            GotoXY(dim_matrix.y +1,dim_matrix.x + 2);
                printf(" step = %d", step);
        }
        if(  GetAsyncKeyState( VK_DELETE ) & 0x8000 ) {
            for(int k=0;k<=dim_matrix.y;k++){
                for(int l=0;l<=dim_matrix.x;l++){
                    m[k][l]=char(32);
                    Print(k,l,char(32));
                }
            }
            coord_player_spaceship.y=yspaceship;
            coord_player_spaceship.x=xspaceship;
            m[coord_player_spaceship.y][coord_player_spaceship.x]=char(6);
            Print(coord_player_spaceship.y,coord_player_spaceship.x,char(6));
            Sleep(100);
        }
        if( ( GetAsyncKeyState( 'W' ) & 0x8000 ) || ( GetAsyncKeyState( VK_UP ) & 0x8000 ) ){
            if(y>0) {
                y-=step;
                GotoXY(y,x);
            }
        }
        if( ( GetAsyncKeyState( 'S' ) & 0x8000 ) || ( GetAsyncKeyState( VK_DOWN ) & 0x8000 ) ){
            if(y<dim_matrix.y){
                y+=step;
                GotoXY(y,x);
            }
        }
        if( ( GetAsyncKeyState( 'A' ) & 0x8000 ) || ( GetAsyncKeyState( VK_LEFT ) & 0x8000 ) ){
            if(x>0){
                x-=step;
                GotoXY(y,x);
            }
        }
        if( ( GetAsyncKeyState( 'D' ) & 0x8000 ) || ( GetAsyncKeyState( VK_RIGHT ) & 0x8000 ) ){
            if(x<dim_matrix.x) {
                x+=step;
                GotoXY(y,x);
            }
        }
        if(  GetAsyncKeyState( 'B' ) & 0x8000  ){
            m[y][x]=char(219);
            Print(y,x,m[y][x]);
        }
        if(  GetAsyncKeyState( VK_BACK ) & 0x8000 ) {
            if( m[y][x]!=char(6) ) {
                Print(y,x,char(32));
                m[y][x]=char(32);
            }
        }
        if(  GetAsyncKeyState( VK_SPACE ) & 0x8000 ) {
            if( m[y][x]!=char(6) ) {
                Print(y,x,char(4));
                m[y][x]=char(4);
            }
        }
        if(  GetAsyncKeyState( VK_RETURN ) & 0x8000 ) {
            break;
        }
        Sleep(100);
    }
    Save_Level(nrlevel);
    level.present=1;
    Load_Level(level.present);
    system("cls");GotoXY(1,1); printf("Your level was saved!");
    Sleep(1500);
}

void Add_Bonus(){// bonus appears on the second line only if it is empty
    bool empty_line = true;
    for( int i = 0 ;i<= dim_matrix.x; i++ ){
        if( m[1][i] == char (4) ){
            empty_line = false;
            break;
        }
    }
    if(empty_line==true){
        bonus.x = rand()%( dim_matrix.x-2 ) +1;
        m[ 1 ][ bonus.x ] = char(783);
        Print( 1,bonus.x,char(783) );
        exists_Bonus = true;
    }
}
void Remove_Bonus(){
    if(bonus.x!=NULL){
        m[1][bonus.x]= char(32);
        Print(1,bonus.x,char(32));
        exists_Bonus=false;
    }
}
void Add_Malus(){
    bool empty_line = true;
    for( int i = 0 ;i<= dim_matrix.x; i++ ){
        if( m[1][i] == char (4) ){
            empty_line = false;
            break;
        }
    }
    if(empty_line==true){
        malus.x = rand()%( dim_matrix.x-2 ) +1;
        m[ 1 ][ malus.x ] = char(769);
        Print( 1,malus.x,char(769) );
        exists_Malus = true;
    }
}
void Remove_Malus(){
    if(malus.x!=NULL){
        m[1][malus.x]= char(32);
        Print(1,malus.x,char(32));
        exists_Malus=false;
    }
}

void PleaseWaitScreen(){ // annoying but funny
    char wait[] = { "Please wait!" };
    GotoXY(2,10);
    printf("%s", wait);

    GotoXY(7,0);
    cout<<char(219);
    GotoXY(7,30);
    cout<<char(219);
    for(int i=0;i<=30;i++){
        GotoXY(6,i);
        cout<<char(219);
        GotoXY(8,i);
        cout<<char(219);
    }
    for(int i=1;i<30;i++){
        GotoXY(7,i);
        cout<<char(177);
        Sleep(58-(i+i));
        if(i==28) {
            Sleep(1000);
        }
        if(i==29) {
            Sleep(100);
        }
    }
}

void Initialization_Menu(){
    //Main menu:
        strcpy(menu[0]," Resume ");
        strcpy(menu[1]," New Game ");
        strcpy(menu[2]," High Scores ");
        strcpy(menu[3]," Instructions ");
        strcpy(menu[4]," Settings ");
        strcpy(menu[5]," Quit ");
    //settings:
        strcpy(menu_settings[0]," Sound ");
        strcpy(menu_settings[1]," AutoPlay ");
        strcpy(menu_settings[2]," Secret Codes ");
        strcpy(menu_settings[3]," Customize levels ");
        strcpy(menu_settings[4]," Back ");
    //high scores:
        strcpy(menuHScores[0]," Reset scores ");
        strcpy(menuHScores[1]," Back ");
    //instructions :
        strcpy(menu_Instructions[0]," Back ");
    //Customize levels :
        strcpy(menu_leved_customize[0]," Level 1 ");
        strcpy(menu_leved_customize[1]," Level 2 ");
        strcpy(menu_leved_customize[2]," Level 3 ");
        strcpy(menu_leved_customize[3]," Level 4 ");
        strcpy(menu_leved_customize[4]," Level 5 ");
        strcpy(menu_leved_customize[5]," Level 6 ");
        strcpy(menu_leved_customize[6]," Level 7 ");
        strcpy(menu_leved_customize[7]," Level 8 ");
        strcpy(menu_leved_customize[8]," Level 9 ");
        strcpy(menu_leved_customize[9]," Level 10 ");
        strcpy(menu_leved_customize[10]," Back ");
    //Cheats:
        strcpy(cheats[0],"addlife");
        strcpy(cheats[1],"addscore");
        strcpy(cheats[2],"addgun");
        strcpy(cheats[3],"addbonus");
        nr_cheats=3;

    nr_elements_menu[0]=5; // main menu
    nr_elements_menu[1]=4; // settings
    nr_elements_menu[2]=1; // high score
    nr_elements_menu[3]=0; // instructions
    nr_elements_menu[4]=10; // Customize levels
}
void Initialization_Variables(){// global variables
    coord_player_spaceship.x=17;
    coord_player_spaceship.y=15;
    dim_matrix.x=35;
    dim_matrix.y=15;
    level.minim=0;
    level.present=0;
    level.maxim=10;
    weapon.minim=0;
    weapon.present=0;
    weapon.maxim=2;
    bonus.x = NULL;
    malus.x= NULL;
}

void GotoXY( int y, int x){ // move the cursor to the line y and column x
   COORD coord;
   coord.X = x+1;// +1 because of the border
   coord.Y = y+1;
   SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), coord );
}
void Delete_Character(int x , int y){
    GotoXY(x,y);
    printf(" ");
}
void Print(int x , int y, char c){   //Displays char c on the line X and Y column
    GotoXY(x,y);
    printf("%c",c);
}
int GameOver(){
    for(int i=0;i<=dim_matrix.y;i++){
        for(int j=0;j<=dim_matrix.x;j++ ){
            if(m[i][j]==char(4)){ // if on the matrix exists enemies game is not over
                return 0;
            }
        }
    }// if there's no enemy
    system("cls");
    cout<<"\n  Congratulations! You have finished level "<<level.present;
    if(level.present==level.maxim){
        cout<<" \n You have finished all levels ";
        if(sound == true){
            PlaySound("audio\\world_clear.wav", NULL, SND_ASYNC);
        }
        quit=true;// end the wail from main function
        if( scor > highScore_int[nr_Elements_HScore] || nr_Elements_HScore==0 || nr_Elements_HScore<10 ) {// check for new h score
            Sleep(2500);
            Add_New_HScore();
            Save_HScore();
            }
        Sleep(2500);
    }
    else {// next level
        if(sound == true){
            PlaySound("audio\\stage_clear.wav", NULL, SND_ASYNC);
        }
        Sleep(2500);
        level.present++;
        level_was_changed=true;
        Load_Level(level.present);
        Show_Matrix();
        Sleep(1000);
    }
}

void Add_Score(int bonus){
    if(sound == true){
        PlaySound("audio\\fireworks.wav", NULL, SND_ASYNC);
    }
    scor+=bonus;
    GotoXY(2,dim_matrix.x+10);// refresh the score on the screen
        cout<<scor;
}
void Decrease_Life(int malus){
    nr_lifes-=malus;
    if(sound == true){
        PlaySound("audio\\pipe.wav", NULL, SND_ASYNC);
    }
    GotoXY(4,dim_matrix.x + 2);
    if(nr_lifes>1){
        printf(" You have %d lifes! " , nr_lifes );
    }
    else {
        if(nr_lifes==1){
            printf(" You have left just one life! " );
        }
        else
            printf(" You run out of lives! " );
    }
    for(int i=0;i<=9;i++){// blinking effect
        Sleep(65);
        if( i%2==0 ){
            GotoXY(coord_player_spaceship.y,coord_player_spaceship.x);
            printf(" ");
        }
        else {
            GotoXY(coord_player_spaceship.y,coord_player_spaceship.x);
            printf("%c",char(6));
        }
    }
}


void Auto_Play(double *time, double *i_Player_Fire, double *LAST_TIME_Player_Fire )
{
    bool exists_enemy_spaceship=false;// if on column of the player exists enemy spaceship
    bool exists_enemy_rocket_x_player=false;//if on column of the player exists enemy rocket
    bool exists_block=false;


    if(!exists_Bonus){
        if(level.present>=6){
            Find_poz_enemy_spaceships_xy_AND_poz_player_spaceships();
            if( X_Lowest_Enemy() < coord_player_spaceship.x ){
                if(m[coord_player_spaceship.y][coord_player_spaceship.x-1]==char(32))
                    if(m[coord_player_spaceship.y-1][coord_player_spaceship.x-1]!=char(2335))
                        if(m[coord_player_spaceship.y-2][coord_player_spaceship.x-1]!=char(2335))
                            Move_Player_Spaceship('A');
            }
            if( X_Lowest_Enemy() > coord_player_spaceship.x ){
                if(m[coord_player_spaceship.y][coord_player_spaceship.x+1]==char(32))
                    if(m[coord_player_spaceship.y-1][coord_player_spaceship.x+1]!=char(2335))
                        if(m[coord_player_spaceship.y-2][coord_player_spaceship.x+1]!=char(2335))
                            Move_Player_Spaceship('D');
            }
            if( X_Lowest_Enemy() == coord_player_spaceship.x ){
                if(*time-*LAST_TIME_Player_Fire >= *i_Player_Fire) { // time present - last time fired > shooting interval
                    *LAST_TIME_Player_Fire=*time;
                    Player_Fire(); // bum bum bum
                }
           }
        }
        else {
            exists_enemy_spaceship=false;
            exists_enemy_rocket_x_player=false;
            exists_block=false;
            for( int i=0; i<=dim_matrix.y && !exists_block ; i++ ){ // check for value of the var exists_enemy_spaceship && exists_block && exists_enemy_rocket_x_player
                if(m[i][coord_player_spaceship.x] == char(219)||m[i][coord_player_spaceship.x] == char(178)||m[i][coord_player_spaceship.x] == char(177)||m[i][coord_player_spaceship.x] == char(176))
                    exists_block=true;
                if( m[i][coord_player_spaceship.x] == char(4) ){
                    exists_enemy_spaceship=true;
                    if(!exists_block){
                        if(*time-*LAST_TIME_Player_Fire >= *i_Player_Fire) { // time present - last time fired > shooting interval
                            *LAST_TIME_Player_Fire=*time;
                            Player_Fire(); // bum bum bum
                        }
                    }
                }
                if(m[i][coord_player_spaceship.x] == char(2335))// enemy rocket
                {// avoid enemy rocket if it is classified as dangerous
                    exists_enemy_rocket_x_player=true;
                    if(i>=coord_player_spaceship.y-5){
                        if(weapon.present==1){
                            if( m[coord_player_spaceship.y][ coord_player_spaceship.x+1] == char(32) ){
                                Move_Player_Spaceship('D');
                                if(!exists_block){
                                    if(*time-*LAST_TIME_Player_Fire >= *i_Player_Fire) { // time present - last time fired > shooting interval
                                        *LAST_TIME_Player_Fire=*time;
                                        Player_Fire(); // bum bum bum
                                    }
                                }
                            }
                            else
                                if( m[coord_player_spaceship.y][ coord_player_spaceship.x+1] == char(32) ){
                                    Move_Player_Spaceship('A');
                                    if(!exists_block){
                                        if(*time-*LAST_TIME_Player_Fire >= *i_Player_Fire) { // time present - last time fired > shooting interval
                                            *LAST_TIME_Player_Fire=*time;
                                            Player_Fire(); // bum bum bum
                                        }
                                    }
                                }
                        }
                    }
                }
            }
            if(exists_enemy_spaceship==false){// if on player column is not a enemy move left or right to the most nearest
                for(int i=0; i<=dim_matrix.x; i++){
                    for(int j=0; j<=dim_matrix.y; j++){
                        if( coord_player_spaceship.x+i <= dim_matrix.x ){
                            if( m[j][ coord_player_spaceship.x+i ] == char(4) ){
                                if(m[coord_player_spaceship.y-1][coord_player_spaceship.x+1]!=char(2335))
                                    if(m[coord_player_spaceship.y-2][coord_player_spaceship.x+1]!=char(2335))
                                        if(m[coord_player_spaceship.y][coord_player_spaceship.x+1]==char(32)){
                                            Move_Player_Spaceship('D');
                                            i=dim_matrix.x+1;
                                            j=dim_matrix.y+1;
                                        }
                                }
                        }
                        if( coord_player_spaceship.x-i >= 0){
                            if( m[j][ coord_player_spaceship.x-i] == char(4) ){
                                if(m[coord_player_spaceship.y-1][coord_player_spaceship.x-1]!=char(2335))
                                    if(m[coord_player_spaceship.y-2][coord_player_spaceship.x-1]!=char(2335))
                                        if(m[coord_player_spaceship.y][coord_player_spaceship.x-1]==char(32)){
                                            Move_Player_Spaceship('A');
                                            i=dim_matrix.x+1;
                                            j=dim_matrix.y+1;
                                        }
                            }
                        }
                    }
                }
            }
        }
    }
    else {//if exists bonus go and get it
        int corectaree=0;
        if(weapon.present==1){
            if(
                direction_enemy_move_x==lleft)
                corectaree=1;
            else
                corectaree=-1;
        }
        if( coord_player_spaceship.x==bonus.x+corectaree ){
            if(*time-*LAST_TIME_Player_Fire >= *i_Player_Fire) { // time present - last time fired > shooting interval
                *LAST_TIME_Player_Fire=*time;
                Player_Fire(); // bum bum bum
            }
        }
        else{
            if(coord_player_spaceship.x<bonus.x+corectaree ){
                if(m[coord_player_spaceship.y-1][coord_player_spaceship.x+1]!=char(2335))
                    if(m[coord_player_spaceship.y-2][coord_player_spaceship.x+1]!=char(2335))
                        if( m[coord_player_spaceship.y][coord_player_spaceship.x+1]==char(32) )
                            Move_Player_Spaceship('D');
            }
            else{
                if(m[coord_player_spaceship.y-1][coord_player_spaceship.x-1]!=char(2335))
                    if(m[coord_player_spaceship.y-2][coord_player_spaceship.x-1]!=char(2335))
                        if( m[coord_player_spaceship.y][coord_player_spaceship.x-1]==char(32) )
                            Move_Player_Spaceship('A');
            }
        }
    }
}

int Max(int c, int d){
    if(c>d)
        return c;
    return d;
}

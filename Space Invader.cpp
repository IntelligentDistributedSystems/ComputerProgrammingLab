#include<iostream>
#include<fstream>
#include<string>
#include<stdlib.h>
#include <Windows.h>
#include<conio.h>
#include<iomanip>
#include <stdio.h>
#include <ctime>

using namespace std;

void init_map1();
void init_map2();
void init_map3();
void display_map();
void new_game();
void display_meniu();
void move_player_right();
void move_player_left();
void move_player_up();
void move_player_down();
void GotoXY(int x, int y);
void delete_coord(int x, int y);
void move_enemy();
void move_enemy_right();
void move_enemy_left();
void move_enemy_down();
void player_shoot();
void player_shoot_2();
void player_shoot_3();
void player_bullet();
void enemy_shoot();
void enemy_bullet();
void end_war();
void init_high_score();
void number_enemy();
void init_map_resume();
void resume_save_map();
void save_top_score();
void display_top_score();
void display_bonus();
void display_malus();


char map[20][40];
int i, j;
int k_meniu=0;
bool end_game=false;
int speed_game=100;
char direction_enemy;
int lives=3;
int k_enemy=0;
int down;
int last_line_enemy;
long high_score,v_high_score[10];
long score=0;
int resume=0;
int level;
int var_random;
bool bonus=false;
bool malus=false;
int col_bonus;
int col_malus;
int k_bonus=0,k_malus=0;
int k_shoot=1;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); //help change color

HANDLE hOut;

struct coordonate{ // memorises player's position
    int x,y;
};

struct coordonate poz_player;

int main()
{
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY); //change color in green

    srand ( time(NULL) ); // prevent random numbers to be the same every time

    display_meniu();

	return 0;
}
//Initialize the map for level 1
void init_map1()
{
	FILE *map1;
	map1=fopen("Map1.txt", "r");
	while ( !feof(map1) ){
		for (i = 0; i < 20; i++){
			for (j = 0; j < 40; j++){
				fscanf(map1, "%c", &map[i][j]);
			}
		}
	}
	fclose(map1);
}
//Initialize the map for level 2
void init_map2()
{
	FILE *map2;
	map2=fopen("Map2.txt", "r");
	while ( !feof(map2) ){
		for (i = 0; i < 20; i++){
			for (j = 0; j < 40; j++){
				fscanf(map2, "%c", &map[i][j]);
			}
		}
	}
	fclose(map2);
}
//Initialize the map for level 3
void init_map3()
{
	FILE *map3;
	map3=fopen("Map3.txt", "r");
	while ( !feof(map3) ){
		for (i = 0; i < 20; i++){
			for (j = 0; j < 40; j++){
				fscanf(map3, "%c", &map[i][j]);
			}
		}
	}
	fclose(map3);
}
//Initialize the map after resume
void init_map_resume()
{
    FILE *resume_map;
	resume_map=fopen("Resume.txt", "r");
	while ( !feof(resume_map) ){
		for (i = 0; i < 20; i++){
			for (j = 0; j < 40; j++){
				fscanf(resume_map, "%c", &map[i][j]);
			}
		}
	}
	fclose(resume_map);

        ifstream dir("direction.txt");  //reads the direction of movement of the enemy
        dir>>direction_enemy;
        dir.close();

        ifstream scor("score.txt");    //reads the score
        scor>>score;
        scor.close();

        ifstream lev("level.txt");   //reads the level
        lev>>level;
        lev.close();

        ifstream VR("variabila_random.txt");  // reads the random variable
        VR>>var_random;
        VR.close();

        ifstream shoot("shoot.txt");  // reads the number of bullets that you shoot
        shoot>>k_shoot;
        shoot.close();

        ifstream lif("life.txt");  // reads your life
        lif>>lives;
        lif.close();
}
//Save the map to return to menu
void resume_save_map()
{
    ofstream fisier("Resume.txt");
    for (i = 0; i < 20; i++){
        for (j = 0; j < 40; j++){
            fisier<< map[i][j];
        }
    }
    fisier.close();

    ofstream dir("direction.txt"); //Saves enemies direction: left or right
    dir<<direction_enemy;
    dir.close();

    ofstream lev("level.txt"); //Saves the level
    lev<<level;
    lev.close();

    ofstream scor("score.txt"); //Saves the score
    scor<<score;
    scor.close();

    ofstream VR("variabila_random.txt"); //Saves random variable
    VR<<var_random;            //defines how often enemies shoot
    VR.close();

    ofstream shoot("shoot.txt");   //saves the number of bullets that you shoot
    shoot<<k_shoot;
    shoot.close();

    ofstream lif("life.txt");  // saves your life
    lif<<lives;
    lif.close();
}


void init_high_score()              //initializes the highest score and the vector top scores
{
    ifstream Top_Score_file("Top-Score.txt");
    for( int q=0;q<10;q++ ){
        Top_Score_file >> v_high_score[q];
    }
    Top_Score_file.close();
    high_score=v_high_score[0];
}

//Saves highest 10 scores
void save_top_score()
{
    ofstream Top_Score_file("Top-Score.txt");
    for( int q=0;q<10;q++ ){
        Top_Score_file << v_high_score[q]<<" ";
    }
    Top_Score_file.close();
}

void display_top_score()
{

    COORD l = GetLargestConsoleWindowSize(hOut);     // helps to write in the middle of console
	int l1 = l.X;

            if( score > high_score ){   //check if is the highest score
                high_score=score;

                for( int q=9;q>=1;q-- ){
                    v_high_score[q]=v_high_score[q-1];
                }

                v_high_score[0]=high_score; //Put your score on second place
                save_top_score();

                SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY); //change color in red
                cout<<"\n\n\n\n\n\n           NEW HIGH SCORE : "<<high_score;
                Sleep(3000);    //  stops system for 3 seconds
                SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

                system("cls");   //clean the screen
                cout<<"\n\n\n\n\n\n";
                cout << setw(l1 / 4) << " " << "Top score\n\n"; //write in the middle of console

                int ok=0;
                for( int q=0;q<10;q++ ){    //search your score and write it with red
                    if( score == v_high_score[q] && ok==0 ){
                        ok=1;
                        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                        cout << setw(l1 / 4) << " " << v_high_score[q] <<"\n";
                        SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    }
                    else if( v_high_score[q] != 0 ){
                        cout << setw(l1 / 4) << " " << v_high_score[q] <<"\n";
                    }
                }
                Sleep(4000);
            }
          else if( score == high_score ){ //check if your score is equal with the highest score
                for( int q=9;q>=2;q-- ){
                    v_high_score[q]=v_high_score[q-1];
            }
            v_high_score[1]=score; //Put your score on second place
            save_top_score();

            system("cls");
            cout<<"\n\n\n\n\n\n";
            cout << setw(l1 / 4) << " " << "Top score\n\n";

            int ok=0;
            for( int q=0;q<10;q++ ){
                if( score != v_high_score[q+1] && ok==0  ){
                    ok=1;
                    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                    cout << setw(l1 / 4) << " " << v_high_score[q] <<"\n";
                    SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                }
                else if( v_high_score[q] != 0 ){
                    cout << setw(l1 / 4) << " " << v_high_score[q] <<"\n";
                }
            }
            Sleep(4000);
        }
        else{
            for( int q=1;q<10;q++ ){            // Put your score in the top scores and write it with red
                if( score > v_high_score[q] ){
                    for( int w=9;w>=q+1;w-- ){
                        v_high_score[w]=v_high_score[w-1];
                    }
                    v_high_score[q]=score;
                    save_top_score();

                    cout<<"\n\n\n\n\n\n";
                    cout << setw(l1 / 4) << " " << "Top score\n\n";

                    int ok=0;
                    for( int q=0;q<10;q++ ){
                        if( score == v_high_score[q] && ok==0 ){
                            ok=1;
                            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                            cout << setw(l1 / 4) << " " << v_high_score[q] <<"\n";
                            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                        }
                        else if( v_high_score[q] != 0 ){
                            cout << setw(l1 / 4) << " " << v_high_score[q] <<"\n";
                        }
                    }
                    Sleep(4000);
                    break;
                }
                else if( score == v_high_score[q] ){
                        for( int w=9;w>=q+1;w-- ){
                            v_high_score[w]=v_high_score[w-1];
                        }
                        v_high_score[q]=score;
                        save_top_score();

                        cout<<"\n\n\n\n\n\n";
                        cout << setw(l1 / 4) << " " << "Top score\n\n";

                        int ok=0;
                        for( int q=0;q<10;q++ ){
                            if( score == v_high_score[q] && score != v_high_score[q+1] && ok==0  ){
                                ok=1;
                                SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                                cout << setw(l1 / 4) << " " << v_high_score[q] <<"\n";
                                SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                            }
                            else if( v_high_score[q] != 0 ){
                                cout << setw(l1 / 4) << " " << v_high_score[q] <<"\n";
                            }
                        }
                        Sleep(4000);
                        break;
                }
            }
        }
        system("cls");
        resume=0;
        exit(0);
}

void display_map()
{
	system("cls");
	for (i = 0; i < 20; i++){
		for (j = 0; j < 40; j++){
            if (map[i][j]=='W'){  // memorises player's position
                poz_player.x=i;
                poz_player.y=j;
            }
            printf("%c", map[i][j]);
		}
		cout<<endl;
	}
}

void display_bonus()
{
    int bon;
    bon=rand()%80;       //generate a random number between 0 and 79

    if( bon == 30 ){
        col_bonus=rand()%39;
        if( col_bonus != 0 ){
            map[1][col_bonus]=(char) 2;
            GotoXY(1,col_bonus);      //move the cursor to the given coordinates
            cout<<(char) 2;
            bonus=true;
        }
    }
}

void display_malus()
{
    int mal;
    mal=rand()%80;

    if( mal == 30 ){
        col_malus=rand()%39;
        if( col_malus != 0 ){
            map[1][col_malus]=(char) 5;
            GotoXY(1,col_malus);
            cout<<(char) 5;
            malus=true;
        }
    }
}

void display_meniu()
{
	COORD l = GetLargestConsoleWindowSize(hOut);
	int l1 = l.X;

	system("cls");

	cout << "\n\n\n\n\n";

	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << setw(l1 / 4) << " " << "NEW GAME\n";
	SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << setw(l1 / 4) << " " << "RESUME\n";
    cout << setw(l1 / 4) << " " << "TOP SCORE\n";
	cout << setw(l1 / 4) << " " << "EXIT\n";

	  while (getch() ){ // k_meniu - memorize the position from the menu

		if (k_meniu == 0 && GetAsyncKeyState(VK_UP)){  //switching from the first position to the last position in the menu
			k_meniu = 3;
			getch();
		}
		else if (k_meniu == 3 && GetAsyncKeyState(VK_DOWN)){  //switching from the last position to the first position in the menu
			k_meniu = 0;
			getch();
		}
		else if (GetAsyncKeyState(VK_DOWN)){  //if the key pressed is down arrow then
			k_meniu++;
			getch();
		}
		else if (GetAsyncKeyState(VK_UP)){   //if the key pressed is up arrow then
			k_meniu--;
			getch();
		}
		else if ( GetAsyncKeyState(VK_RETURN) ){   //if the key pressed is enter then
			if (k_meniu == 0){ // new game is selected
				resume=0;
				level=1;
				new_game();
			}

			if (k_meniu == 1){ // resume is selected
                resume=1;
                new_game();
			}

            if (k_meniu == 2){   // top scores is selected
				system("cls");

				int var;
				ifstream Top_Score_file("Top-Score.txt");
				cout << "\n\n\n\n\n";
                while( !Top_Score_file.eof() ){  // display the top scores
                Top_Score_file >> var ;

                    if(var != 0 ){
                        cout << setw(l1 / 4) << " " << var <<"\n";
                    }
                }
                Top_Score_file.close();

                while(getch() ){
                    if( GetAsyncKeyState(VK_ESCAPE) ){ //if the key pressed is ESC then go back to menu
                        break;
                    }
                }
			}

			if (k_meniu == 3){  // exit is selected
				system("cls");
				exit(0);
			}
		}

		if (k_meniu == 0){     //if new game is selected then write it with red
			system("cls");
			cout << "\n\n\n\n\n";

			SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << setw(l1 / 4) << " " << "NEW GAME\n";
			SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		}
		else{
			system("cls");
			cout << "\n\n\n\n\n";
			cout << setw(l1 / 4) << " " << "NEW GAME\n";
		}

		if (k_meniu == 1){  //if resume is selected then write it with red
			SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << setw(l1 / 4) << " " << "RESUME\n";
			SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		}
		else{
			cout << setw(l1 / 4) << " " << "RESUME\n";
		}

		if (k_meniu == 2){    //if top scores is selected then write it with red
			SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << setw(l1 / 4) << " " << "TOP SCORE\n";
			SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		}
		else{
			cout << setw(l1 / 4) << " " << "TOP SCORE\n";
		}

        if (k_meniu == 3){     //if exit is selected then write it with red
			SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << setw(l1 / 4) << " " << "EXIT\n";
			SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		}
		else{
			cout << setw(l1 / 4) << " " << "EXIT\n";
		}
	}
}

void new_game()
{
    end_game=false;
    init_high_score(); //initializes the highest score and vector top scores

    if( resume==0 ){    //  means that new game is selected

        if( level==1 ){
            init_map1();         //Initialize the map for level 1
            direction_enemy='r';  //enemies moving to right the first time
            var_random=25;      //defines how often shoot enemies
            system("cls");
            cout<<"\n\n\n\n\n\n\n\n\n\n";
            cout<<"                                   LEVEL 1";
            GotoXY(500,500);   //move the cursor into an area where is not visible
            Sleep(3000);
            system("cls");
        }
        else if( level==2 ){
            init_map2();         //Initialize the map for level 2
            direction_enemy='r';
            var_random=15;
        }
        else if( level==3 ){
            init_map3();         //Initialize the map for level 3
            direction_enemy='r';
            var_random=7;
        }
    }else if(resume==1){     // I chose resume
        init_map_resume();
    }

    display_map();

    GotoXY(7,50);             //move the cursor to the given coordinates
    cout<<"Lives: "<<lives;

    GotoXY(11,50);
    cout<<"Score: "<<score;

    GotoXY(15,50);
    cout<<"High Score: "<<high_score;

	while (!GetAsyncKeyState(VK_ESCAPE) && end_game==false ){

            for( i=0;i<20;i++ ){
                for( j=0;j<40;j++ ){

                    switch(map[i][j]){
                        case '^':{
                            player_bullet();   //control the movement of player bullet
                            break;
                        }

                        case 'x':{            //x - represents explosion
                            delete_coord(i,j);    //delete the given coordinates
                            break;
                        }
                        case 'X':{            //X - represents your spaceship's explosion
                            if( lives!=0 ){
                                map[i][j]='W';
                                GotoXY(i,j);
                                cout<<'W';
                                lives--;
                                GotoXY(7,50);
                                cout<<"Lives: "<<lives;
                            }
                            break;
                        }
                        case 2:{           //represents the bonus
                            k_bonus++;
                            if( k_bonus == 50 ){
                                delete_coord(1,col_bonus);
                                k_bonus=0;
                                bonus=false;
                            }
                        }
                        case 5:{         //represents the malus
                            k_malus++;
                            if( k_malus == 50 ){
                                delete_coord(1,col_malus);
                                k_bonus=0;
                                malus=false;
                            }
                        }
                    }
                }
            }

            for( i=19;i>0;i-- ){         //Search the enemy bullet
                for( j=0;j<40;j++ ){
                    if( map[i][j]=='*' )
                        enemy_bullet();  //control the movement of enemy bullet
                }
            }
                SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                if ( GetAsyncKeyState(VK_RIGHT) ){   //if the key pressed is right arrow then
                    move_player_right();
                }
                if ( GetAsyncKeyState(VK_LEFT) ){    //if the key pressed is left arrow then
                    move_player_left();
                }
                if ( GetAsyncKeyState(VK_UP) ){     //if the key pressed is up arrow then
                    move_player_up();
                }
                if ( GetAsyncKeyState(VK_DOWN) ){   //if the key pressed is down arrow then
                    move_player_down();
                }

                if( GetAsyncKeyState(VK_SPACE) ){  //if the key pressed is space then

                    switch(k_shoot){       // number of bullets that you shoot

                        case 1:{
                            player_shoot();
                            break;
                        }
                        case 2:{
                            player_shoot_2();
                            break;
                        }
                        case 3:{
                            player_shoot_3();
                            break;
                        }
                    }
                }

        move_enemy();      // make the movement of enemies

        int ok=0;
        for( i=20;i>=0;i-- ){          //identify the last line of enemies because only the last line shoots
            for( j=0;j<40;j++ ){
                if( map[i][j]=='Y' ){
                    last_line_enemy=i;
                    ok=1;
                    break;
                }
            }
            if(ok==1){
                break;
            }
        }

        enemy_shoot();

        if( bonus==false && malus==false ){
            display_bonus();
        }

        if( bonus==false && malus==false ){
            display_malus();
        }


        number_enemy();     //count how many enemies are left

        GotoXY(500,500);    //move the cursor into an area where is not visible

        Sleep(speed_game);

        end_war();
	}//end while

	if( end_game==false ){    //saves the map when it's pressed ESC
        resume=1;

        resume_save_map();
        display_meniu();
    }
}

void end_war()      //check if the game was over
{
    SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);


    COORD l = GetLargestConsoleWindowSize(hOut);
	int l1 = l.X;


    for( int p=0;p<40;p++ ){       //Check if the enemies have reached to the last line
        if( map[18][p]=='Y' ){
            Sleep(2000);
            end_game=true;
            system("cls");

            cout<<"\n\n\n\n     The enemy was too strong \n\n";
            Sleep(3000);
            cout<<"     The planet was conquered!";
            Sleep(3000);

            system("cls");
            display_top_score();
        }
    }

    if( lives==0 && k_enemy==0 ){   //You destroyed your enemies and you died
        Sleep(3000);
        end_game=true;
        system("cls");

        cout<<"\n\n\n\n     You won,but you pay with your life \n\n";
        Sleep(3000);
        cout<<"     Your sacrifice will not be forgotten!";
        Sleep(3000);

        system("cls");
        display_top_score();

    }else if( lives==0 ){  //you died
        Sleep(3000);
        end_game=true;
        system("cls");

        cout<<"\n\n\n\n     You Lose! \n\n";
        Sleep(3000);
        cout<<"     The planet was conquered!";
        Sleep(3000);

        system("cls");
        display_top_score();

    }else if( k_enemy==0 ){   // you destroy all your enemies
        Sleep(3000);
        system("cls");

        if( level==1 ){   //go to the level 2
            level=2;
            cout<<"\n\n\n\n\n\n\n\n\n\n";
            cout<<"                                   LEVEL 2";
            GotoXY(500,500);
            Sleep(3000);
            system("cls");
            resume=0;
            new_game();

        }else if( level==2 ){       //go to the level 3
            level=3;
            cout<<"\n\n\n\n\n\n\n\n\n\n";
            cout<<"                                   LEVEL 3";
            GotoXY(500,500);
            Sleep(3000);
            system("cls");
            resume=0;
            new_game();
        }
        end_game=true;       //game is over and you won

        cout<<"\n\n\n\n     You Won! \n\n";
        Sleep(3000);
        cout<<"     Congratulations!";
        Sleep(3000);

        system("cls");
        display_top_score();
    }
}

void move_player_right()
{
    switch( map[poz_player.x][poz_player.y+1] ){
            case ' ':{                                //move your spaceship to the next right position
                delete_coord(poz_player.x,poz_player.y);
                poz_player.y++;
                GotoXY(poz_player.x,poz_player.y);
                cout<<'W';
                map[poz_player.x][poz_player.y]='W';
                break;
            }
            case 'Y':{    //move your spaceship to the right and hit the enemy
                SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                delete_coord(poz_player.x,poz_player.y);
                poz_player.y++;
                GotoXY(poz_player.x,poz_player.y);
                cout<<'X';                            //X - represents your spaceship explosion
                map[poz_player.x][poz_player.y]='X';

                SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                score=score+100;
                GotoXY(11,50);
                cout<<"Score: "<<score;
                break;
            }
            case '*':{          //move your spaceship to the right and hit the enemy's bullet
                SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                delete_coord(poz_player.x,poz_player.y);
                poz_player.y++;
                GotoXY(poz_player.x,poz_player.y);
                cout<<'X';
                map[poz_player.x][poz_player.y]='X';
                SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                break;
            }
    }
}

void move_player_left()
{
    switch( map[poz_player.x][poz_player.y-1] ){
            case ' ':{                                   //move your spaceship to the next left position
                delete_coord(poz_player.x,poz_player.y);
                poz_player.y--;
                GotoXY(poz_player.x,poz_player.y);
                cout<<'W';
                map[poz_player.x][poz_player.y]='W';
                break;
            }
            case 'Y':{                 //move your spaceship to the left and hit the enemy
                SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                delete_coord(poz_player.x,poz_player.y);
                poz_player.y--;
                GotoXY(poz_player.x,poz_player.y);
                cout<<'X';
                map[poz_player.x][poz_player.y]='X';

                SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                score=score+100;
                GotoXY(11,50);
                cout<<"Score: "<<score;
                break;
            }
            case '*':{                    //move your spaceship to the left and hit the enemy's bullet
                SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                delete_coord(poz_player.x,poz_player.y);
                poz_player.y--;
                GotoXY(poz_player.x,poz_player.y);
                cout<<'X';
                map[poz_player.x][poz_player.y]='X';
                SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                break;
            }
    }
}

void move_player_up()
{
    if( poz_player.x>14 ){          //move your spaceship to the next up position
        switch( map[poz_player.x-1][poz_player.y] ){
            case ' ':{
                delete_coord(poz_player.x,poz_player.y);
                poz_player.x--;
                GotoXY(poz_player.x,poz_player.y);
                cout<<'W';
                map[poz_player.x][poz_player.y]='W';
                break;
            }
            case 'Y':{           //move your spaceship to the next up position and hit the enemy
                SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                delete_coord(poz_player.x,poz_player.y);
                poz_player.x--;
                GotoXY(poz_player.x,poz_player.y);
                cout<<'X';
                map[poz_player.x][poz_player.y]='X';

                SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                score=score+100;
                GotoXY(11,50);
                cout<<"Score: "<<score;
                break;
            }
            case '*':{       //move your spaceship to the next up position and hit the enemy's bullet
                SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                delete_coord(poz_player.x,poz_player.y);
                poz_player.x--;
                GotoXY(poz_player.x,poz_player.y);
                cout<<'X';
                map[poz_player.x][poz_player.y]='X';
                SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                break;
            }
        }
    }
}

void move_player_down()
{
    switch( map[poz_player.x+1][poz_player.y] ){
            case ' ':{                              //move your spaceship to the next down position
                delete_coord(poz_player.x,poz_player.y);
                poz_player.x++;
                GotoXY(poz_player.x,poz_player.y);
                cout<<'W';
                map[poz_player.x][poz_player.y]='W';
                break;
            }
            case 'Y':{                        //move your spaceship to the next down position and hit the enemy
                SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                delete_coord(poz_player.x,poz_player.y);
                poz_player.x++;
                GotoXY(poz_player.x,poz_player.y);
                cout<<'X';
                map[poz_player.x][poz_player.y]='X';

                SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                score=score+100;
                GotoXY(11,50);
                cout<<"Score: "<<score;
                break;
            }
            case '*':{            //move your spaceship to the next down position and hit the enemy's bullet
                SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                delete_coord(poz_player.x,poz_player.y);
                poz_player.x++;
                GotoXY(poz_player.x,poz_player.y);
                cout<<'X';
                map[poz_player.x][poz_player.y]='X';
                SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                break;
            }
        }
}

void move_enemy()      // make the movement of enemies
{
    for( i=0;i<20;i++ ){      //Search the enemies
        for( j=0;j<40;j++ ){

            if( map[i][j]=='Y' ){

                    if( direction_enemy=='r' ){
                       move_enemy_right();
                    }
                    if( direction_enemy=='l' ){
                        move_enemy_left();
                    }
            }
        }
    }

    down=0;
    for( int a=0;a<20;a++ ){     //check if the enemies have reached to the map's edge
        if( map[a][2]=='Y' || map[a][36]=='Y' ){
                down=1;
        }
    }

    if(down==1){                    //Change the direction
        if(direction_enemy=='r'){
            direction_enemy='l';
        }
        else if(direction_enemy=='l'){
            direction_enemy='r';
        }
       move_enemy_down();       //move each enemy to the next down position
    }
}

void move_enemy_right()
{
    delete_coord(i,j);        //delete the given coordinates
    j++;
    switch( map[i][j] ){

        case '^':{              //move the enemy to the next right position and hit your bullet
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[i][j]='x';
            GotoXY(i,j);
            cout<<'x';

            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            score=score+100;
            GotoXY(11,50);
            cout<<"Score: "<<score;
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);

            break;
            }
            case '|':{                //move the enemy to the next right position and hit the wall
                SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                map[i][j]='x';
                GotoXY(i,j);
                cout<<'x';
                SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                break;
            }
                case '-':{            //move the enemy to the next right position and hit the wall
                    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                    map[i][j]='x';
                    GotoXY(i,j);
                    cout<<'x';
                    SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    break;
                }
                case 'W':{           //move the enemy to the next right position and hit your spaceship
                    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                    map[i][j]='X';
                    GotoXY(i,j);
                    cout<<'X';

                    SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    score=score+100;
                    GotoXY(11,50);
                    cout<<"Score: "<<score;
                    break;
                }
                case ' ':{           //move the enemy to the next right position
                    SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    map[i][j]='Y';
                    GotoXY(i,j);
                    cout<<'Y';
                    break;
                }
            }
}

void move_enemy_left()
{
    delete_coord(i,j);      //delete the given coordinates
    j--;
    switch( map[i][j] ){
        case '^':{               //move the enemy to the next left position and hit your bullet
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[i][j]='x';
            GotoXY(i,j);
            cout<<'x';

            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            score=score+100;
            GotoXY(11,50);
            cout<<"Score: "<<score;
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);

            break;
        }
        case '|':{           //move the enemy to the next left position and hit the wall
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[i][j]='x';
            GotoXY(i,j);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case '-':{          //move the enemy to the next left position and hit the wall
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[i][j]='x';
            GotoXY(i,j);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case 'W':{           //move the enemy to the next left position and hit your spaceship
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[i][j]='X';
            GotoXY(i,j);
            cout<<'X';

            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            score=score+100;
            GotoXY(11,50);
            cout<<"Score: "<<score;
            break;
        }
        case ' ':{           //move the enemy to the next left position
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            map[i][j]='Y';
            GotoXY(i,j);
            cout<<'Y';
            break;
        }
    }
}

void move_enemy_down()      //move each enemy to the next down position
{
    for( i=19;i>=0;i-- ){
            for( j=0;j<40;j++ ){
                if(map[i][j]=='Y'){
                        switch( map[i+1][j] ){

                            case '^':{               // move the enemy to the next down position and hits your bullet
                                SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                                delete_coord(i,j);
                                map[i+1][j]='x';
                                GotoXY(i+1,j);
                                cout<<'x';

                                SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                                score=score+100;
                                GotoXY(11,50);
                                cout<<"Score: "<<score;
                                SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                                break;
                            }

                            case '|':{         // move the enemy to the next down position and hits the wall
                                SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                                delete_coord(i,j);
                                map[i+1][j]='x';
                                GotoXY(i+1,j);
                                cout<<'x';
                                SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                                break;
                            }
                            case '-':{         // move the enemy to the next down position and hits the wall
                                SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                                delete_coord(i,j);
                                map[i+1][j]='x';
                                GotoXY(i+1,j);
                                cout<<'x';
                                SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                                break;
                            }
                            case 'W':{        // move the enemy to the next down position and hits your spaceship
                                SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                                delete_coord(i,j);
                                map[i+1][j]='X';
                                GotoXY(i+1,j);
                                cout<<'X';

                                SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                                score=score+100;
                                GotoXY(11,50);
                                cout<<"Score: "<<score;
                                break;
                            }
                            case ' ':{             // move the enemy to the next down position
                                SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                                delete_coord(i,j);
                                map[i+1][j]='Y';
                                GotoXY(i+1,j);
                                cout<<'Y';
                                break;
                            }
                        }
                    }
            }
        }
}

void player_shoot()
{
    switch( map[poz_player.x-1][poz_player.y]){

        case ' ':{              //your spaceship shoot
            SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y]='^';
            GotoXY(poz_player.x-1,poz_player.y);
            cout<<'^';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case '*':{           //your spaceship shoot and hits the enemy's bullet
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y]='x';
            GotoXY(poz_player.x-1,poz_player.y);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case 'Y':{         //your spaceship shoot and hits the enemy
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y]='x';
            GotoXY(poz_player.x-1,poz_player.y);
            cout<<'x';

            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            score=score+100;
            GotoXY(11,50);
            cout<<"Score: "<<score;
            break;
        }
        case '|':{              //your spaceship shoot and hits the wall
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y]='x';
            GotoXY(poz_player.x-1,poz_player.y);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case '-':{             //your spaceship shoot and hits the wall
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y]='x';
            GotoXY(poz_player.x-1,poz_player.y);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
    }
}

void player_shoot_2()
{
    switch( map[poz_player.x-1][poz_player.y-1]){

        case ' ':{              //your spaceship shoot
            SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y-1]='^';
            GotoXY(poz_player.x-1,poz_player.y-1);
            cout<<'^';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case '*':{           //your spaceship shoot and hits the enemy's bullet
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y-1]='x';
            GotoXY(poz_player.x-1,poz_player.y-1);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case 'Y':{         //your spaceship shoot and hits the enemy
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y-1]='x';
            GotoXY(poz_player.x-1,poz_player.y-1);
            cout<<'x';

            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            score=score+100;
            GotoXY(11,50);
            cout<<"Score: "<<score;
            break;
        }
        case '|':{              //your spaceship shoot and hits the wall
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y-1]='x';
            GotoXY(poz_player.x-1,poz_player.y-1);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case '-':{             //your spaceship shoot and hits the wall
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y-1]='x';
            GotoXY(poz_player.x-1,poz_player.y-1);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
    }

    switch( map[poz_player.x-1][poz_player.y+1]){

        case ' ':{              //your spaceship shoot
            SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y+1]='^';
            GotoXY(poz_player.x-1,poz_player.y+1);
            cout<<'^';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case '*':{           //your spaceship shoot and hits the enemy's bullet
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y+1]='x';
            GotoXY(poz_player.x-1,poz_player.y+1);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case 'Y':{         //your spaceship shoot and hits the enemy
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y+1]='x';
            GotoXY(poz_player.x-1,poz_player.y+1);
            cout<<'x';

            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            score=score+100;
            GotoXY(11,50);
            cout<<"Score: "<<score;
            break;
        }
        case '|':{              //your spaceship shoot and hits the wall
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y+1]='x';
            GotoXY(poz_player.x-1,poz_player.y+1);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case '-':{             //your spaceship shoot and hits the wall
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y+1]='x';
            GotoXY(poz_player.x-1,poz_player.y+1);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
    }
}

void player_shoot_3()
{
    switch( map[poz_player.x-1][poz_player.y-1]){

        case ' ':{              //your spaceship shoot
            SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y-1]='^';
            GotoXY(poz_player.x-1,poz_player.y-1);
            cout<<'^';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case '*':{           //your spaceship shoot and hits the enemy's bullet
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y-1]='x';
            GotoXY(poz_player.x-1,poz_player.y-1);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case 'Y':{         //your spaceship shoot and hits the enemy
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y-1]='x';
            GotoXY(poz_player.x-1,poz_player.y-1);
            cout<<'x';

            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            score=score+100;
            GotoXY(11,50);
            cout<<"Score: "<<score;
            break;
        }
        case '|':{              //your spaceship shoot and hits the wall
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y-1]='x';
            GotoXY(poz_player.x-1,poz_player.y-1);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case '-':{             //your spaceship shoot and hits the wall
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y-1]='x';
            GotoXY(poz_player.x-1,poz_player.y-1);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
    }

    switch( map[poz_player.x-1][poz_player.y]){

        case ' ':{              //your spaceship shoot
            SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y]='^';
            GotoXY(poz_player.x-1,poz_player.y);
            cout<<'^';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case '*':{           //your spaceship shoot and hits the enemy's bullet
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y]='x';
            GotoXY(poz_player.x-1,poz_player.y);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case 'Y':{         //your spaceship shoot and hits the enemy
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y]='x';
            GotoXY(poz_player.x-1,poz_player.y);
            cout<<'x';

            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            score=score+100;
            GotoXY(11,50);
            cout<<"Score: "<<score;
            break;
        }
        case '|':{              //your spaceship shoot and hits the wall
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y]='x';
            GotoXY(poz_player.x-1,poz_player.y);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case '-':{             //your spaceship shoot and hits the wall
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y]='x';
            GotoXY(poz_player.x-1,poz_player.y);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
    }

    switch( map[poz_player.x-1][poz_player.y+1]){

        case ' ':{              //your spaceship shoot
            SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y+1]='^';
            GotoXY(poz_player.x-1,poz_player.y+1);
            cout<<'^';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case '*':{           //your spaceship shoot and hits the enemy's bullet
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y+1]='x';
            GotoXY(poz_player.x-1,poz_player.y+1);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case 'Y':{         //your spaceship shoot and hits the enemy
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y+1]='x';
            GotoXY(poz_player.x-1,poz_player.y+1);
            cout<<'x';

            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            score=score+100;
            GotoXY(11,50);
            cout<<"Score: "<<score;
            break;
        }
        case '|':{              //your spaceship shoot and hits the wall
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y+1]='x';
            GotoXY(poz_player.x-1,poz_player.y+1);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case '-':{             //your spaceship shoot and hits the wall
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            map[poz_player.x-1][poz_player.y+1]='x';
            GotoXY(poz_player.x-1,poz_player.y+1);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
    }
}

void player_bullet()  //control the movement of bullet
{
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
    switch( map[i-1][j] ){
        case ' ':{          //move the bullet on the next position
            SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_INTENSITY);  //make the bullet blue
            delete_coord(i,j);   //delete given coordinates
            map[i-1][j]='^';
            GotoXY(i-1,j);
            cout<<'^';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case '#':{
            delete_coord(i,j);    //the bullet get out of the map
            break;
        }

        case '*':{            //your bullet hits the enemy's bullet
            delete_coord(i,j);
            map[i-1][j]='x';   //x - represents explosion
            GotoXY(i-1,j);
            cout<<'x';
            break;
        }
        case '|':{             //your bullet hits the wall
            delete_coord(i,j);
            map[i-1][j]='x';
            GotoXY(i-1,j);
            cout<<'x';
            break;
        }
        case '-':{             //your bullet hits the wall
            delete_coord(i,j);
            map[i-1][j]='x';
            GotoXY(i-1,j);
            cout<<'x';
            break;
        }
        case 'Y':{              //your bullet hits the enemy
            delete_coord(i,j);
            map[i-1][j]='x';
            GotoXY(i-1,j);
            cout<<'x';

            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            score=score+100;
            GotoXY(11,50);
            cout<<"Score: "<<score;
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);

            break;
        }
        case 2:{
            delete_coord(i,j);
            map[i-1][j]='x';
            GotoXY(i-1,j);
            cout<<'x';

            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            score=score+500;
            GotoXY(11,50);
            cout<<"Score: "<<score;
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);

            if( k_shoot != 3 ){
                k_shoot++;
            }

            k_bonus=0;
            bonus=false;

            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case 5:{
            delete_coord(i,j);
            map[i-1][j]='x';
            GotoXY(i-1,j);
            cout<<'x';

            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            score=score+500;
            GotoXY(11,50);
            cout<<"Score: "<<score;
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);

            if( k_shoot != 1 ){
                k_shoot--;
            }

            k_malus=0;
            malus=false;

            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    }
}

void enemy_shoot()
{
    int aux;
    for( j=0;j<40;j++ ){
        if( map[last_line_enemy][j]=='Y' ){
            aux=rand()%var_random;        //generate a random number between 0 and var_random-1
            if( aux==5 ){
                switch( map[last_line_enemy+1][j] ){

                    case ' ':{             //the enemy shoot
                        SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                        map[last_line_enemy+1][j]='*';
                        GotoXY(last_line_enemy+1,j);
                        cout<<'*';
                        SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                        break;
                    }
                    case '^':{               //the enemy shoot and hits your bullet
                        SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                        map[last_line_enemy+1][j]='x';
                        GotoXY(last_line_enemy+1,j);
                        cout<<'x';
                        SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                        break;
                    }
                    case '|':{                 //the enemy shoot and hits the wall
                        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                        map[last_line_enemy+1][j]='x';
                        GotoXY(last_line_enemy+1,j);
                        cout<<'x';
                        SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                        break;
                    }
                    case '-':{                 //the enemy shoot and hits the wall
                        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                        map[last_line_enemy+1][j]='x';
                        GotoXY(last_line_enemy+1,j);
                        cout<<'x';
                        SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                        break;
                    }
                    case 'W':{                //the enemy shoot and hits your spaceship
                        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
                        map[last_line_enemy+1][j]='X';
                        GotoXY(last_line_enemy+1,j);
                        cout<<'X';
                        SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                        break;
                    }
                }
            }
        }
    }
}

void enemy_bullet()     //control the movement of enemy bullet
{
    switch( map[i+1][j] ){
        case ' ':{                      //move the bullet on the next position
            SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_INTENSITY);    //make the bullet blue
            delete_coord(i,j);
            map[i+1][j]='*';
            GotoXY(i+1,j);
            cout<<'*';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case '#':{            //the bullet get out of the map
            delete_coord(i,j);
            break;
        }

        case '^':{              //the enemy's bullet hits your bullet
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            delete_coord(i,j);
            map[i+1][j]='x';
            GotoXY(i+1,j);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case '|':{           //the enemy's bullet hits the wall
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            delete_coord(i,j);
            map[i+1][j]='x';
            GotoXY(i+1,j);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case '-':{         //the enemy's bullet hits the wall
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            delete_coord(i,j);
            map[i+1][j]='x';
            GotoXY(i+1,j);
            cout<<'x';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
        case 'W':{        //the enemy's bullet hits you
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            delete_coord(i,j);
            map[i+1][j]='X';
            GotoXY(i+1,j);
            cout<<'X';
            SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        }
    }
}

void number_enemy(){        //count how many enemies are left
    k_enemy=0;
    for( i=0;i<20;i++ ){
        for( j=0;j<40;j++ ){
            if( map[i][j]=='Y' ){
                k_enemy++;
            }
        }
    }
}

void delete_coord(int x, int y)   //delete the given coordinates
{
	GotoXY(x,y);
	cout<<" ";
	map[x][y]=' ';
}

void GotoXY( int y, int x)     //move the cursor to the given coordinates
{
   COORD coord;
   coord.X = x;
   coord.Y = y;
   SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), coord );
}

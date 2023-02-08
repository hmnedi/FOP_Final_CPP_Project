#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include "console_commands.h"
#include "storyline.h"
#include "board.h"
#include "timer.h"
using namespace std;

string name;
void new_game();
void saved_games();
void stats();
void menu();
void fight_panel(int, string);
int score(int c, int n, int i, int l);
bool game_finish(int, int);
void save_stats(bool, string, int, int, string, int);
void fight_setup_firsttime(char board[19][19], int n, string name, string gamename);

int main() {
    srand(time(0));

    ifstream prof_read("src/log/profile.ahoy");
    if (!prof_read) {
        ofstream profile("src/log/profile.ahoy");
        name = first_intro();
        profile << name;
        profile.close();
        Sleep(500);
        clear_console();
    } else {
        getline(prof_read, name);
        prof_read.close();
    }

    menu();

    getch();
}

void new_game(){
    clear_console();
    if (!fight_intro(name)) menu();
    else {
        cout << "How big are you going " << name << "?" << endl << "> ";
        int n;
        cin >> n;

        if (n >= 20) n=19;
        else if (n<5) n=5;
        else if (n%2==0) n++;

        string gamename;
        cout << "What shall be the name of this battle?\n> ";
        cin >> gamename;

        ofstream files;
        files.open("src/log/battlenames.ahoy", ios_base::app);
        files << gamename << "\n";
        files.close();


        Sleep(250);
        clear_console();
        fight_panel(n, gamename);
    }
}

void saved_games(){
    clear_console();
    cout << "choose the battle you want to load:\n";
    ifstream battleNames;
    battleNames.open("src/log/battlenames.ahoy");
    string element;
    while (battleNames >> element) {
        cout << "- " << element << endl;
        Sleep(100);
    }
    battleNames.close();
    cout << "\n- quit\n> ";
    string input;
    cin >> input;
    if (input == "quit"){
        clear_console();
        menu();
    }

    ifstream the_battle;
    the_battle.open("src/games/" + input + ".ahoy");

    string matrix;
    int n, playerScore, enmyScore, clock, aliveEnmyShip, alivePlayerShip, yC, eC, yI, eI;
    bool turn, isFinish;

    the_battle >> matrix >> n >> playerScore >> enmyScore >> clock >> turn >> alivePlayerShip >> aliveEnmyShip >> yC >> eC >> yI >> eI >> isFinish;
    the_battle.close();

    char board[19][19];
    string_to_matrix(matrix, n, board);


    string timeClock = timer(0, clock);
    update_panel(board, n, playerScore, enmyScore, name, input, timeClock);

    if (isFinish){
        if (turn) cout << "YOU LOST";
        else cout << "YOU WON";
        cout << "\n\nPress anything to go back...";
        getch();
        clear_console();
        menu();
    }
    else {
        // todo: make a good structure for the gameplay function but for now copy paste timeee
        time_t clockStart, clockUpdate, res;
        time(&clockStart);

        while(!game_finish(aliveEnmyShip, alivePlayerShip)){
            bool hit;
            if (turn){
                if (player_shoot(board, n)){
                    yC++;
                    aliveEnmyShip--;
                    hit = true;
                }
                else {
                    yI++;
                    hit = false;
                }
                turn = false;
            }
            else {
                if (enemy_shoot(board, n)){
                    eC++;
                    alivePlayerShip--;
                    hit = true;
                }
                else {
                    eI++;
                    hit = false;
                }
                turn = true;
            }

            time(&clockUpdate);
            res = difftime(clockUpdate, clockStart);
            string holdClock = timer(res, clock);

            update_panel(board, n, score(yC, n, yI, aliveEnmyShip), score(eC, n, eI, alivePlayerShip), name, input, holdClock);
            save_game(board, n, input, score(yC, n, yI, aliveEnmyShip), score(eC, n, eI, alivePlayerShip), int(res), turn, alivePlayerShip, aliveEnmyShip, yC, eC, yI, eI);


            cout << "=> ";
            if (turn) cout << "PurpleBeard ";
            else cout << name << " ";

            if (hit) cout << "HIT" << endl;
            else cout << "MISS" << endl;
            Sleep(300);
        }

        bool win;
        if (aliveEnmyShip > 0) {
            delayed_message_char("You lost captain...");
            win = false;
        }
        else if (alivePlayerShip > 0) {
            delayed_message_char("You won captain!!!");
            win = true;
        }

        save_game(board, n, input, score(yC, n, yI, aliveEnmyShip), score(eC, n, eI, alivePlayerShip), int(res), turn, alivePlayerShip, aliveEnmyShip, yC, eC, yI, eI, true);
        save_stats(win, name, score(yC, n, yI, aliveEnmyShip), score(eC, n, eI, alivePlayerShip), input, n);
        cout << endl << "Press anything to go back to menu...";
        getch();
        clear_console();
        menu();
        }
}

void stats(){
    clear_console();

    ifstream stats("src/log/stats.ahoy");
    string tmp;
    while(getline(stats, tmp)){
        cout << tmp << endl;
        Sleep(100);
    }
    stats.close();
    cout << endl << "--------------------------------------------" << endl;
    cout << "press anything to go back...";

    getch();
    clear_console();
    menu();
}

void menu(){
    cout << "             *-----------------* " << endl;
    cout << "     *-------*                 |" << endl;
    cout << "    /  ======== MENU ======== / " << endl;
    cout << "   |    1- New Game           | " << endl;
    cout << "   |    2- Saved Games        \\ " << endl;
    cout << "    \\   3- Stats               | " << endl;
    cout << "    /                          | " << endl;
    cout << "    |  This is the holy-       | " << endl;
    cout << "    |   envelope, the one      / " << endl;
    cout << "    /   and only thing that   / " << endl;
    cout << "   |    matters.             / " << endl;
    cout << "   |                        | " << endl;
    cout << "   |    Choose one of the   | " << endl;
    cout << "    \\   following options:  | " << endl;
    cout << "     |                       | " << endl;
    cout << "     *------*                / " << endl;
    cout << "            *---------------* " << endl;



    cout << endl << "> ";

    bool inputCorrect = false;
    char c;
    do {
        cin >> c;
        if (c == '1'){
            new_game();
            inputCorrect = true;
        } else if (c == '2'){
            saved_games();
            inputCorrect = true;
        } else if (c == '3'){
            stats();
            inputCorrect = true;
        } else {
            cout << "please enter from the list:\n> ";
        }
    } while (!inputCorrect);
}

void fight_panel(int n, string gamename){
    int yC=0, yI=0, aliveEnmyShip=n;
    int eC=0, eI=0, alivePlayerShip=n;
    cout << name << " Score: " << 0 << "\t\t" << "PurpleBeard Score: " << 0 << endl;

    char board[19][19];
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            if (j == int(n/2)) board[i][j] = '|';
            else board[i][j] = '#';
        }
    }
    update_panel(board, n, 10, 10, name, gamename);
    fight_setup_firsttime(board, n, name, gamename);


    time_t clockStart, clockUpdate, res;
    time(&clockStart);

    bool turn = true;
    while(!game_finish(aliveEnmyShip, alivePlayerShip)){
        bool hit;
        if (turn){
            if (player_shoot(board, n)){
                yC++;
                aliveEnmyShip--;
                hit = true;
            }
            else {
                yI++;
                hit = false;
            }
            turn = false;
        }
        else {
            if (enemy_shoot(board, n)){
                eC++;
                alivePlayerShip--;
                hit = true;
            }
            else {
                eI++;
                hit = false;
            }
            turn = true;
        }

        time(&clockUpdate);
        res = difftime(clockUpdate, clockStart);
        string holdClock = timer(res, 0);

        update_panel(board, n, score(yC, n, yI, aliveEnmyShip), score(eC, n, eI, alivePlayerShip), name, gamename, holdClock);
        save_game(board, n, gamename, score(yC, n, yI, aliveEnmyShip), score(eC, n, eI, alivePlayerShip), int(res), turn, alivePlayerShip, aliveEnmyShip, yC, eC, yI, eI);


        cout << "=> ";
        if (turn) cout << "PurpleBeard ";
        else cout << name << " ";

        if (hit) cout << "HIT" << endl;
        else cout << "MISS" << endl;
        Sleep(300);
    }

    bool win;
    if (aliveEnmyShip > 0) {
        delayed_message_char("You lost captain...");
        win = false;
    }
    else if (alivePlayerShip > 0) {
        delayed_message_char("You won captain!!!");
        win = true;
    }

    save_game(board, n, gamename, score(yC, n, yI, aliveEnmyShip), score(eC, n, eI, alivePlayerShip), int(res), turn, alivePlayerShip, aliveEnmyShip, yC, eC, yI, eI, true);
    save_stats(win, name, score(yC, n, yI, aliveEnmyShip), score(eC, n, eI, alivePlayerShip), gamename, n);
    cout << endl << "Press anything to go back to menu...";
    getch();
    clear_console();
    menu();

}

int score(int c, int n, int i, int l){
    int res = c + (n/(i+1)) + l;
    return res;
}

bool game_finish(int a, int b){
    if (a==0 || b==0) return true;
    return false;
}

void save_stats(bool win, string name, int playerScore, int enmyScore, string gamename, int n){
    string res = name;
    if (win) res += " won";
    else res += " lost";
    res += " the " + to_string(n) + "*" + to_string(n) + " " + gamename + " battle with the score of " + to_string(playerScore) + " to " + to_string(enmyScore) + ".";
    ofstream sta;
    sta.open("src/log/stats.ahoy", ios_base::app);
    sta << res << "\n";
    sta.close();
}

void fight_setup_firsttime(char board[19][19], int n, string name, string gamename){
    delayed_message_char("Alright Captain! Let's do this...\n");
    for (int i=1; i<=n; i++){
        fast_delayed_msg("Enter the (x, y) coordinate of the ship number " + to_string(i) + ":\n> ");
        int x, y;
        cin >> x >> y;
        if (y >= int(n/2) || check_duplicate(board, x, y) || x >= n){
            while (y >= int(n/2) || check_duplicate(board, x, y) || x >= n) {
                if (y >= int(n/2) || x >= n){
                    cout << "you can't place your ship in enemy's territory, enter the right coordinate:\n> ";
                } else if (check_duplicate(board, x, y)){
                    cout << "you have already places a ship there, choose another coordinate:\n> ";
                }
                cin >> x >> y;
            }
        }
        board[x][y] = '*';
        update_panel(board, n, 10, 10, name, gamename);
    }

    set_up_enemy(board, n);
    save_game(board, n, gamename, 10, 10, 0, 1, n, n, 0, 0, 0, 0);

    update_panel(board, n, 10, 10, name, gamename);
    game_begins_intro();
    update_panel(board, n, 10, 10, name, gamename);

}

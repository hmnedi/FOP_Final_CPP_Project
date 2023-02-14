using namespace std;

void show_matrix(char mat[19][19], int n){
    cout << endl;
    cout << "    ";
    for(int j=0; j<n; j++) {
        if (j == int(n/2)) cout << "  ";
        else if (j > int(n/2)) cout << j-1 << " ";
        else cout << j << " ";
    }
    cout << endl;
    for (int i=0; i<n; i++){
        if (i<10) cout << i << "   ";
        else cout << i << "  ";
        for (int j=0; j<n; j++){
            if (j>int(n/2) && mat[i][j]=='*') cout << "#";
            else cout << mat[i][j];

            if (j<10) cout << " ";
            else cout << "  ";
        }
        cout << endl;
    }
}

void update_panel(char mat[19][19], int n, int yourScore, int enmyScore, string name, string gamename, string time="0:0"){
    clear_console();
    cout << "********************** " << gamename << " **********************" << endl;
    cout << name << " Score: " << yourScore << "     " << time << "     " << "PurpleBeard Score: " << enmyScore<< endl;
    show_matrix(mat, n);
    cout << endl <<  "----------------------------------------------------" << endl;
}



bool check_duplicate(char mat[19][19], int x, int y){
    if (mat[x][y] == '*') return true;
    return false;
}

void save_game(char mat[19][19], int n, string gamename, int playerScore, int enmyScore, int clock, bool turn, int playerAliveShip, int enmyAliveShip, int yC, int eC, int yI, int eI, bool isFinish=false){
    ofstream board_matrix;
    board_matrix.open("src/games/" + gamename + ".ahoy");
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            board_matrix << mat[i][j];
        }
    }
    board_matrix << "\n";
    board_matrix << n << "\n";
    board_matrix << playerScore << "\n";
    board_matrix << enmyScore << "\n";
    board_matrix << clock << "\n";
    board_matrix << turn << "\n";
    board_matrix << playerAliveShip << "\n";
    board_matrix << enmyAliveShip << "\n";
    board_matrix << yC << "\n";
    board_matrix << eC<< "\n";
    board_matrix << yI << "\n";
    board_matrix << eI<< "\n";
    board_matrix << isFinish << "\n";
    board_matrix.close();
}

void set_up_enemy (char enmyMat[19][19], int n){
    for(int i=0; i<n; i++){
        int x, y;
        do {
            x = rand() % n;
            y = (rand() % (n - (int(n/2)+1))) + (int(n/2)+1);
        } while(enmyMat[x][y] == '*');

        enmyMat[x][y] = '*';
    }
}

void game_begins_intro() {
    cout << endl << "The game is going to start in..." << endl;
    for (int i=0; i<3;i++){
        cout << i << "..." << endl;
        Sleep(1000);
    }
    cout << "now";
    Sleep(700);
}

bool player_shoot(char mat[19][19], int n){
    fast_delayed_msg("choose where do you want to shoot:\n> ");
    int x, y;
    cin >> x >> y;
    y++;

    while (y <= int(n/2) || x >= n || y >= n) {
        cout << "aim inside the enemy's area:\n> ";
        cin >> x >> y;
        y++;
    }

    while (mat[x][y] == '0' || mat[x][y] == '1'){
        cout << "you can't shoot the same place twice:\n> ";
        cin  >> x >> y;
        y++;
    }

    if (mat[x][y] == '*'){
        mat[x][y] = '1';
        return true;
    }
    else if (mat[x][y] == '#') {
        mat[x][y] = '0';
        return false;
    }
}


bool enemy_shoot(char mat[19][19], int n){
    cout << "enemy is shooting..." << endl;
    Sleep(2500);
    int x = rand() % n;
    int y = rand() % int(n/2);

    while (mat[x][y] == '0' || mat[x][y] == '1'){
        x = rand() % n; // random_range = [0, n-1]
        y = rand() % int(n/2);
    }

    if (mat[x][y] == '*'){
        mat[x][y] = '1';
        return true;
    }
    else {
        mat[x][y] = '0';
        return false;
    }
}

void string_to_matrix(string s, int n, char mat[19][19]){
    int i=0, j=0;
    for (int k=0; k<s.size(); k++){
        mat[i][j] = s[k];
        if (j == n-1){
            i++;
            j = 0;
        }
        else {
            j++;
        }
    }
}

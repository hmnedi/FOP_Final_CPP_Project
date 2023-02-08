using namespace std;

void delayed_message_char(string s){
    for(int i=0; i<s.size(); i++){
        cout << s[i];
        Sleep(rand() % (40 - 20) + 20);
        if (s[i] == ' '){
            Sleep(50);
        }
    }
}

void fast_delayed_msg(string s){
    for(int i=0; i<s.size(); i++){
        cout << s[i];
        Sleep(rand() % (20 - 5) + 5);
        if (s[i] == ' '){
            Sleep(20);
        }
    }
}


void clear_console() {
    system("cls");
}



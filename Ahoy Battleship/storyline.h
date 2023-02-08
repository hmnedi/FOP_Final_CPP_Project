using namespace std;


    // todo: add music to chars g etting typed

string first_intro(){
    delayed_message_char("Ahoy Captain!!! Welcome aboard...\nWhat shall we call you my captain?");
    cout << endl << "> ";
    string name;
    getline(cin, name);
    delayed_message_char("Ahoy " + name + "!!!");
    return name;
}

bool fight_intro(string name){
    delayed_message_char("Ahoy " + name + "!!!\nit seems we have gotten into a brutal fight with PurpleBeard The Legendary Pirate!\nyou have to take control of the ship and win this battle... there's no time... hurry up...\n");
    delayed_message_char("Are you ready " + name + "?\n> ");
    string tmp;
    cin >> tmp;
    if (tmp[0] == 'y' || tmp[0] == 'Y' || tmp[0] == 'a' || tmp[0] == 'A'){
        delayed_message_char("Welcome abooooard... ");
        return true;
    } else {
        clear_console();
        return false;
    }
}

using namespace std;
string timer(int n, int oldTime){
    n += oldTime;
    int mins=0, sec;
    if (n>=60){
        mins = n/60;
    }
    sec = (n%60);

    string s = to_string(mins) + ":" + to_string(sec);
    return s;
}


#include <FastLED.h>

#define NUM_LEDS 25
#define DATA_PIN 3
#define n 5

CRGB leds[NUM_LEDS];
char board[n][n];
int ledPos[n][n] = {
  { 0, 9, 10, 19, 20 },
  { 1, 8, 11, 18, 21 },
  { 2, 7, 12, 17, 22 },
  { 3, 6, 13, 16, 23 },
  { 4, 5, 14, 15, 24 }
};

bool gamesDone = false, turn=true;
int AlivePlayer = n, AliveEnemy = n;


void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(9600);
  randomSeed(analogRead(5));  // randomize using noise from analog pin 5

// turning leds off
  for (int k = 0; k < NUM_LEDS; k++) {
    leds[k] = CRGB::Black;
    FastLED.show();
  }

  // filling board
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (j == n / 2) board[i][j] = '|';
      else board[i][j] = '#';
    }
  }


  // setting up enemy ships
  for (int i = 0; i < n; i++) {
    int x, y;
    // check for duplicated enemies
    do {
      x = random(n);
      y = random((n / 2) + 1, n);
    } while (board[x][y] == '*');

    board[x][y] = '*';
  }


  // seting up players ship
  set_player_ships();


  // just load it before anything
  show_matrix();
  convert_matrix_to_leds();
  delay(1000);
}

void loop() {

  // TODO: add OLED and count the numbres and print there MISS, HIT, Scores too...

  while(!gamesDone){
    int x, y;
    if (turn){
      // player shoot
      Serial.println("Choose a place to shoot: ");
      while (Serial.available() == 0);  // wait for user input
      x = Serial.parseInt(SKIP_ALL, '\n');
      y = Serial.parseInt(SKIP_ALL, '\n');
      Serial.print("> you shot ");
      Serial.print(x);
      Serial.print(", ");
      Serial.print(y);
      Serial.println(".");

      turn = false;
    }
    else {
      // enemy shoots
      do{
        x = random(n);
        y = random(n/2);
      } while(board[x][y] == '0' || board[x][y] == '1');

      turn = true;
      delay(1850); // little time to feel the game
    }

    if (board[x][y] == '*'){
      Serial.println("HIT");
      board[x][y] = '1';
      if (!turn) AliveEnemy--;
      else AlivePlayer--;
    }
    else {
      Serial.println("MISS");
      board[x][y] = '0';
    }

    show_matrix();
    convert_matrix_to_leds();

    if (AlivePlayer == 0){
      Serial.println("YOu LOST");
      gamesDone = true;
    }
    else if (AliveEnemy == 0) {
      Serial.println("YOu WON");
      gamesDone = true;
    }
  }


}

void convert_matrix_to_leds() {
  for (int k = 0; k < NUM_LEDS; k++) {
    leds[k] = CRGB::Black;
    FastLED.show();
  }
  
  for(int i=0; i<n; i++){
    for(int j=0; j<n; j++){
      int k = ledPos[i][j];

      if (board[i][j] == '|') {
        leds[k] = CRGB::Yellow;
      } 
      else if (board[i][j] == '*' && j<n/2) {
        leds[k] = CRGB::IndianRed;
      } 
      else if (board[i][j] == '1') {
        if (j>n/2) leds[k] = CRGB::Green;
        else leds[k] = CRGB::DarkRed; //   leds[3] = CRGB(220, 0, 20);
      } 
      else if (board[i][j] == '0') {
        leds[k] = CRGB::DarkSlateGrey;
      }
      FastLED.show();
    }
  }
}

void show_matrix() {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (j>n/2 && board[i][j] == '*') Serial.print('#');
      else Serial.print(board[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println();
}

void set_player_ships() {
  for (int i = 0; i < n; i++) {
    Serial.print("Enter ship's position number ");
    Serial.print(i);
    Serial.println(": ");
    while (Serial.available() == 0);  // wait for user input
    int x, y;
    x = Serial.parseInt(SKIP_ALL, '\n');
    y = Serial.parseInt(SKIP_ALL, '\n');
    Serial.println();
    Serial.print("> ship number ");
    Serial.print(i);
    Serial.print(" is placed at ");
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.println(".");
    board[x][y] = '*';
  }
}

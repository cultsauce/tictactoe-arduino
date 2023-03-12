
#include "TicTacToe.h"

TicTacToe::TicTacToe(Adafruit_SSD1306 *disp) {
  display = disp;
  pinMode(SW, INPUT_PULLUP);
}

void TicTacToe::print_board_base () {
  display->clearDisplay();
  display->setCursor(0, 5);
  display->setTextSize(1);
  display->setTextColor(SSD1306_WHITE);
  display->print("won  ");
  display->println(win_states[0]);
  display->setCursor(0, 15);
  display->print("tie  ");
  display->println(win_states[1]);
  display->setCursor(0, 25);
  display->print("lost ");
  display->println(win_states[2]);

  display->drawRect(58,0,64,64, WHITE);
  
  display->drawLine(79,1,79,63, WHITE);
  display->drawLine(100,1,100,63, WHITE);

  display->drawLine(59,22,121,22,WHITE);
  display->drawLine(59,43,121,43,WHITE);
  display->display ();
}

void TicTacToe::print_board () {
  display->setTextSize(2);
  uint8_t offset_x, offset_y = 2;
  for (uint8_t i = 0; i < 3; i++) {
    offset_x = 64;
    for (uint8_t j = 0; j < 3; j++) {
      display->fillRect(offset_x, offset_y, 14, 14, BLACK);
      display->setCursor(offset_x, offset_y);
      display->print((char) board[3*i +j]);
      offset_x += 21;
    }
    offset_y += 21;
  }
  display->display ();
}

void TicTacToe::choose_player() {
  player = 0;
  opponent = 0;
  display->clearDisplay ();
  display->setTextColor(WHITE);
  display->setTextSize(1);
  display->setCursor(25, 29);
  display->println("X     |     O");
  display->display();
  uint16_t vry;
  while (1) {
    vry = analogRead(VRY);
    if (vry > TH_HIGH) {
      player = 111;
      opponent = 120;
      display->drawLine(96, 37, 102, 37, WHITE);
      display->drawLine(24, 37, 30, 37, BLACK);
      display->display();
    }
    else if (vry < TH_LOW) {
      player = 120;
      opponent = 111;
      display->drawLine(96, 37, 102, 37, BLACK);
      display->drawLine(24, 37, 30, 37, WHITE);
      display->display();
    }
    if (digitalRead(SW) == 0 && player != 0 && opponent != 0) break;
  }
}

void TicTacToe::loading_screen () {
  display->clearDisplay();
  display->invertDisplay(true);
  display->setCursor(25, 10);
  display->setTextSize(3);
  display->setTextColor(WHITE);
  display->println("x o");
  display->setCursor(25, 30);
  display->println("o x");
  display->display();
}

bool TicTacToe::won(uint8_t player_symbol, uint8_t *winpos_pos = nullptr) {
  for (int i = 0; i < 8; i++) {
    if (board[win_pos[i][0]] == player_symbol && 
        board[win_pos[i][1]] == player_symbol && 
        board[win_pos[i][2]] == player_symbol)
    {
      if (winpos_pos != nullptr) *winpos_pos = i;
      return true;
    }
  }
  return false;
}

uint8_t TicTacToe::game_over(uint8_t *winpos_pos = nullptr)
{
  if (won(player, winpos_pos))  return 1;
  else if (won(opponent, winpos_pos)) return 3;
  for (int i = 0; i < 9; i++) {
    if ((board[i] != player) && (board[i] != opponent)) {
      return 0;
    }
  }
  return 2;
}

void TicTacToe::player_move() {
  uint8_t a, m = 0, pos = 4;
  while (!m)
  {
    uint16_t vrx = analogRead(VRX);
    uint16_t vry = analogRead(VRY);
    if (vry < TH_LOW) {
      ((pos != 0) && (pos != 3) && (pos != 6)) ? pos -= 1: pos = pos;
    }
    else if (vry > TH_HIGH) {
      ((pos != 2) && (pos != 5) && (pos != 8)) ? pos += 1: pos = pos;
    }
    else if (vrx > TH_HIGH) {
      ((pos != 0) && (pos != 1) && (pos != 2)) ?  pos -= 3: pos = pos;
    }
    else if (vrx < TH_LOW) {
      ((pos != 6) && (pos != 7) && (pos != 8)) ? pos += 3: pos = pos;
    }
    
    a = board[pos];
    if ((board[pos] != opponent) && (board[pos] != player)) {
      board[pos] = player;
      print_board ();
      while (!input_changed());
    }
    else {
      while (!input_changed()) {
        board[pos] = EMPTY_SPACE;
        print_board();
        delay(100);
        if (input_changed ()) break;
        board[pos] = a;
        print_board();
        delay(100);
      }
    }
    board[pos] = a;

    if (digitalRead(SW) == 0 && (board[pos] != opponent) && (board[pos] != player)) {
      m = 1;
      board[pos] = player;
    }
    delay(150);
  }
}

int TicTacToe::minimax(bool is_maximizing, uint8_t depth, uint8_t &best_move) {
  uint8_t state = game_over();
  if (state || depth >= MAX_DEPTH) {
      if (state == 3) {
          return 10 - depth;
      }
      else if (state == 1) {
          return depth - 10;
      }
      else {
          return 0;
      }
  }

  if (is_maximizing) {
      int best_score = -666;
      for (uint8_t i = 0; i < 9; i++) {
          if (board[i] == player || board[i] == opponent) continue;

          board[i] = opponent;
          int new_score = minimax(false, depth++, best_move);

          int this_score;
          if (new_score > best_score) {
              best_score = new_score;
              best_move = i;
          }
          board[i] = EMPTY_SPACE;
      }
      return best_score;
  }
  else {
      int best_score = 666;
      for (uint8_t i = 0; i < 9; i++) {
          if (board[i] == player || board[i] == opponent) continue;

          board[i] = player;
          int new_score = minimax(true, depth++, best_move);
          
          if (new_score < best_score) {
              best_score = new_score;
              best_move = i;
          }
          board[i] = EMPTY_SPACE;
      }
      return best_score;
  }
}

void TicTacToe::arduino_move () {
  uint8_t pos;
  minimax(true, 0, pos);
  board[pos] = opponent;
  print_board ();
}

bool TicTacToe::input_changed() {
  uint16_t vrx = analogRead(VRX);
  uint16_t vry = analogRead(VRY);
  return (vrx < TH_LOW || vrx > TH_HIGH || vry < TH_LOW || vry > TH_HIGH || !digitalRead(SW));
}

void TicTacToe::win_animation (uint8_t win_pos_idx, uint8_t winner) {
  for (uint8_t _=0; _ < 3; _++) {
    for (uint8_t i = 0; i < 3; i++) {
      board[win_pos[win_pos_idx][i]] = EMPTY_SPACE;
    }
    print_board ();
    delay (100);
    for (uint8_t i = 0; i < 3; i++) {
      board[win_pos[win_pos_idx][i]] = winner;
    }
    print_board ();
    delay (100);
  }
}


void TicTacToe::play () {
  win_states[0] = 0; win_states[1] = 0; win_states[2] = 0; 

  loading_screen ();
  while (!input_changed ());
  display->invertDisplay(false);
  choose_player ();

  delay(150);
  for (;;) {
    new_game ();
    delay (150);
  }
}

void TicTacToe::new_game() {
  for (int i = 0; i < 9; ++i) {
    board[i] = EMPTY_SPACE;
  }
  print_board_base ();
  
  srand(millis());
  if (rand() % 2) {
    board[rand() % 9] = opponent;
  }
  print_board ();

  uint8_t state, position;
  while (true) {

    player_move ();
    state = game_over (&position);
    if (state) {
      win_states[state-1]++;
      if (state == 1) {
        win_animation (position, player);
      }
      break;
    }
    delay (150);
    
    arduino_move ();
    state = game_over (&position);
    if (state) {
      win_states[state-1]++;
      if (state == 3) {
        win_animation (position, opponent);
      }
      break;
    }
    delay(150);
  }
}

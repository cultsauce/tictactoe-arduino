#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#ifndef TCC
#define TCC
#define EMPTY_SPACE 32
#define MAX_DEPTH 5

#define VRX A0
#define VRY A1
#define SW 4
#define TH_LOW 100
#define TH_HIGH 1000


class TicTacToe 
{
    private:
    const Adafruit_SSD1306 *display;
    uint8_t win_states[3];
    uint8_t opponent, player;
    uint8_t board[9];
    const int win_pos[8][3]={{0,1,2}, {3,4,5}, {6,7,8}, {0,3,6}, {1,4,7}, {2,5,8}, {0,4,8}, {2,4,6}};

    void print_board_base ();
    void print_board ();
    void loading_screen ();
    void win_animation (uint8_t win_pos_idx, uint8_t winner);
    void choose_player();
    void player_move();
    void new_game();
    void arduino_move ();

    bool won (uint8_t player_symbol, uint8_t *winpos_pos = nullptr);
    bool input_changed();

    int minimax(bool is_maximizing, uint8_t depth, uint8_t &move);
    uint8_t game_over (uint8_t *winpos_pos = nullptr);

    public:
    TicTacToe(Adafruit_SSD1306 *disp);
    void play();


};
#endif //TCC

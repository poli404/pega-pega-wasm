#include "wasm4.h"
#include <math.h>
#include <stdio.h>

typedef struct {
  int x;
  int y; // position
  int life; // after being caught, takes one down
  char type; // 'c' chaser or 'p' prey
} Player;

Player player1 = {142, 142, 3, 'p'};
Player player2 = {10, 30, 3, 'c'};

const uint8_t smiley[] = {
    0b11000011,
    0b10000001,
    0b00100100,
    0b00100100,
    0b00000000,
    0b00100100,
    0b10011001,
    0b11000011,
};

void start() {
    trace("start");
}

void drawBackground() {
    //BORDA DA ARENA DE COMBATE
    text("Pega-Pega: the game", 5, 5);
    *DRAW_COLORS = 0x4;
    rect(0, 20, 10, 140);
    rect(0, 20, 154, 10);
    rect(0, 150, 154, 10);
    rect(150, 20, 10, 160);
    //rect(76, 76, 76, 10);
    //rect(0, 100, 100, 10);
}

void draw(int px, int py, int cx, int cy) {
    *DRAW_COLORS = 0x2;
    blit(smiley, px, py, 8, 8, BLIT_1BPP);
    *DRAW_COLORS = 0x3;
    blit(smiley, cx, cy, 8, 8, BLIT_1BPP);
}

void verify_borders(Player *play) {
    if ((*play).x < 10) {(*play).x = 10;}
    if ((*play).x > 142) {(*play).x = 142;}
    if ((*play).y < 30) {(*play).y = 30;}
    if ((*play).y > 142) {(*play).y = 142;}
}

void move(Player *play, uint8_t *gamer) {
  if (*gamer & BUTTON_LEFT) {
        (*play).x--;
    } else if (*gamer & BUTTON_RIGHT) {
        (*play).x++;
    } else if (*gamer & BUTTON_UP) {
        (*play).y--;
    } else if (*gamer & BUTTON_DOWN) {
        (*play).y++;
    }
}

void collision(Player *chaser, Player *prey) {
    if ((*chaser).x == (*prey).x) {
        (*prey).life--;
    }
}

void endGame(){
    *DRAW_COLORS = 0x4;
    rect(0, 0, 154, 154);
    *DRAW_COLORS = 0x1;
    text("GAME OVER!", 0, 76);
}

void update () {   // desenha e limpa a tela
    PALETTE[3] = 0xeb6b6f;
    PALETTE[2] = 0x4a2480;
    PALETTE[1] = 0xc53a9d;
    PALETTE[0] = 0xfff6d3;

    uint8_t gamepad = *GAMEPAD1;
    uint8_t gamepad2 = *GAMEPAD2;
    
    drawBackground();

    Player *chaser = &player2;
    Player *prey = &player1;

    if ((*prey).life > 0){
        move(chaser, &gamepad2);
        verify_borders(chaser);
        move(prey, &gamepad);
        verify_borders(prey);
        //collision(chaser, prey);

        
        draw(player1.x, player1.y, player2.x, player2.y);
    } else {
        endGame();
    }
}

/*
int collision(){
    if ((x_bolinha == x_barrinha || x_bolinha == x_barrinha+8) && (y_bolinha == y_barrinha || y_bolinha == y_barrinha+8)) {
        return 1;
    } else {
        return 0;
    }
}
*/

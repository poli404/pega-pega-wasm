#include "wasm4.h"
#include <math.h>
#include <stdio.h>

typedef struct {
  int x;
  int y; // position
  int life; // after being caught, takes one down
} Player;

// STARTER CONFIGURATION
int initial = 1;
Player player1 = {142, 142, 3};
Player player2 = {10, 30, 3};
Player *prey = &player1;
Player *chaser = &player2;

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
    // Borders
    text("Pega-Pega: the game", 5, 5);
    *DRAW_COLORS = 0x4;
    rect(0, 20, 10, 140);
    rect(0, 20, 154, 10);
    rect(0, 150, 154, 10);
    rect(150, 20, 10, 160);
}

void draw(int px, int py, int cx, int cy) {
    *DRAW_COLORS = 0x2;
    blit(smiley, px, py, 8, 8, BLIT_1BPP);
    *DRAW_COLORS = 0x3;
    blit(smiley, cx, cy, 8, 8, BLIT_1BPP);
}

void verify_borders(Player *play) {
    if ((*play).x < 10) { (*play).x = 10; tone(400, 5, 80, TONE_PULSE1);} // left
    if ((*play).x > 142) {(*play).x = 142; tone(400, 5, 80, TONE_PULSE1);} // right
    if ((*play).y < 30) {(*play).y = 30; tone(400, 5, 80, TONE_PULSE1);} // up
    if ((*play).y > 142) {(*play).y = 142; tone(400, 5, 80, TONE_PULSE1);} // down
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

int collision(Player *chaser, Player *prey) {
	int dx = (*chaser).x - (*prey).x;
	int dy = (*chaser).y - (*prey).y;
	int sr = 8;
	return (dx * dx + dy * dy) < (sr * sr);
}

void endGame(){
    *DRAW_COLORS = 0x4;
    rect(0, 0, 200, 200);
    *DRAW_COLORS = 0x2;
    text("GAME OVER!", 30, 60);
    if (player1.life == 0) {*DRAW_COLORS = 0x2; text("Player 1 perdeu!", 15, 90);}
    else if (player2.life == 0) {*DRAW_COLORS = 0x2; text("Player 2 perdeu!", 15, 90);}
}

void update () {   // desenha e limpa a tela
    PALETTE[3] = 0xDB8DD0;
    PALETTE[2] = 0xC562AF;
    PALETTE[1] = 0xB33791;
    PALETTE[0] = 0xFEC5F6;

    if (initial) { // initial screen
        uint8_t gamepad = *GAMEPAD1; // prey
        *DRAW_COLORS = 0x3;
        rect(0, 0, 200, 200);
        *DRAW_COLORS = 0x1;
        text("Pega-Pega: the game", 5, 20);
        text("Jogador do canto\ninferior, FUJA!", 15, 60);
        text("Jogador do canto\nsuperior, PEGUE!", 15, 90);
        if (BUTTON_1 & gamepad) {
            initial = 0;
        }
    } else {
        uint8_t gamepad = *GAMEPAD1; // prey
        uint8_t gamepad2 = *GAMEPAD2; // chaser
        
        drawBackground();

        if ((*prey).life > 0 && (*chaser).life > 0){
            move(chaser, &gamepad2);
            verify_borders(chaser);
            move(prey, &gamepad);
            verify_borders(prey);
            if (collision(chaser, prey)) {
                (*prey).life--;
                tone(262 | (523 << 8), 30, 80, TONE_PULSE1 | TONE_MODE3);
                (*chaser).x = 142;
                (*chaser).y = 142;
                (*prey).x = 10;
                (*prey).y = 30;
                // switches the chaser and the prey
                Player *change = chaser;
                chaser = prey;
                prey = change;
            }

            draw(player1.x, player1.y, player2.x, player2.y);
        } else {
            endGame();
        }

    }
}
#include "wasm4.h"
#include <math.h>

typedef struct {
  int x;
  int y; // position
  int life; // after being caught, takes one down
  int dx;
  int dy; // última direção de movimento
} Player;

typedef struct {
// representa posições e direções dos tiros
    int x;
    int y;
    int dx; // direcao
    int dy;
    int active;
} Shot;

Shot shot1 = {0, 0, 0, 0, 0};
Shot shot2 = {0, 0, 0, 0, 0};

// para fazer vários tiros, criar array de shots e iterar sobre eles, mas isso parece ser mt dificil

Player player1 = {142, 142, 3, 0, 0};
Player player2 = {10, 30, 3, 0, 0};

int initial = 1;

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

// substitui implementacao do sprintf que era muito grande
// pra conseguir mostrar os pontos na tela
void int_to_str(char *buffer, int n) {
    int i = 0;
    if (n == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
    return;
    }
    while (n > 0) {
        buffer[i++] = (n % 10) + '0';
        n /= 10;
    }
    buffer[i] = '\0';
    for (int j = 0; j < i / 2; ++j) {
        char tmp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = tmp;
    }
}

void start() {
    trace("start");
}

void drawBackground() {
    //BORDA DA ARENA DE COMBATE
    //text("Pega-Pega: the game", 5, 5);
    *DRAW_COLORS = 0x4;
    rect(0, 20, 10, 140);
    rect(0, 20, 154, 10);
    rect(0, 150, 154, 10);
    rect(150, 20, 10, 160);
}

void drawPlayers() {
    *DRAW_COLORS = 0x2;
    blit(smiley, player1.x, player1.y, 8, 8, BLIT_1BPP);
    *DRAW_COLORS = 0x3;
    blit(smiley, player2.x, player2.y, 8, 8, BLIT_1BPP);
}

void drawLives() {
    *DRAW_COLORS = 0x2;
    text("P1:", 20, 5);
    char buffer[100];
    int_to_str(buffer, player1.life);
    text(buffer, 50, 5);

    *DRAW_COLORS = 0x3;
    text("P2:", 100, 5);
    char buffer2[100];
    int_to_str(buffer2, player2.life);
    text(buffer2, 130, 5);
}

// tela final
void endGame(){
    *DRAW_COLORS = 0x1;
    rect(0, 0, 200, 200);
    *DRAW_COLORS = 0x1;
    //text("GAME OVER!", 40, 76);
    if (player1.life == 0) {*DRAW_COLORS = 0x3; text("Player 2 won!", 30, 76);}
    if (player2.life == 0) {*DRAW_COLORS = 0x2; text("Player 1 won!", 30, 76);}
}

void verify_borders(Player *play) {
    if ((*play).x < 10) {(*play).x = 10;}
    if ((*play).x > 142) {(*play).x = 142;}
    if ((*play).y < 30) {(*play).y = 30;}
    if ((*play).y > 142) {(*play).y = 142;}
}

void move(Player *play, uint8_t *gamer) {
// atualiza posição e registra direções
  if (*gamer & BUTTON_LEFT) {
        (*play).x--;
        (*play).dx = -1;
        (*play).dy = 0;
    } else if (*gamer & BUTTON_RIGHT) {
        (*play).x++;
        (*play).dx = 1;
        (*play).dy = 0;
    } else if (*gamer & BUTTON_UP) {
        (*play).y--;
        (*play).dx = 0;
        (*play).dy = -1;
    } else if (*gamer & BUTTON_DOWN) {
        (*play).y++;
        (*play).dx = 0;
        (*play).dy = 1;
    }
}

void fireShot(Player *shooter, Shot *shot) {
    if (shot->active == 0 && (shooter->dx != 0 || shooter->dy != 0)) {
        shot->x = shooter->x + 4;
        shot->y = shooter->y + 4;
        // define a direção do tiro
        shot->dx = shooter->dx * 2;
        shot->dy = shooter->dy * 2;
        shot->active = 1;
        // faz barulinho de tiro :)
        tone(400 | (523 << 8), 20, 100, TONE_PULSE1 | TONE_MODE3);
    }
}

void updateShot(Player *player, Shot *shot) {
    // verificar se tiro está ativo
    if (!shot->active) {
        return;
    }
    // move o tiro, atualizando a posição
    shot->x += shot->dx;
    shot->y += shot->dy;
    // verifica se saiu do amarelo
    if (shot->x < 10 || shot->x > 142 || shot->y < 30 || shot->y > 142) {
        shot->active = 0;
        return;
    }
    // verifica se deu colisão com outro jogador
    if (shot->x >= player->x && shot->x <= (player->x + 8) &&
        shot->y >= player->y && shot->y <= (player->y + 8)) {
        shot->active = 0;
        player->life--;

        // barulinho de acertou o tiro
        tone(60, 10, 100, TONE_PULSE1 | TONE_PAN_LEFT | TONE_PAN_RIGHT);
        return;
    }
    // desenhar o tiro
    *DRAW_COLORS = 0x4;
	rect(shot->x, shot->y, 2, 2);
}

void update() {   // desenha e limpa a tela
    PALETTE[3] = 0xe870d0;
    PALETTE[2] = 0xc10090;
    PALETTE[1] = 0xd32eaf;
    PALETTE[0] = 0xffb4f6;
    
    uint8_t gamepad = *GAMEPAD1;
    uint8_t gamepad2 = *GAMEPAD2;
    
    if (initial) {
        *DRAW_COLORS = 0x4;
        text("Pega-Pega: the game", 5, 60);
        text("chase each other\naround and fire!!", 15, 80);
        if ((gamepad && BUTTON_1) || (gamepad2 && BUTTON_1)) {
            initial = 0;
        }
    } else {
      
      drawBackground();

      Player *p1 = &player1;
      Player *p2 = &player2;

      if ((*p1).life > 0 && (*p2).life > 0) {
          move(p2, &gamepad2);
          verify_borders(p2);
          move(p1, &gamepad);
          verify_borders(p1);
          // tiro do p1
          if (gamepad & BUTTON_1) {
              fireShot(p1, &shot1);
          }
          // tiro do p2
          if (gamepad2 & BUTTON_1) {
              fireShot(p2, &shot2);
          }
          updateShot(p2, &shot1); // p2 atira no p1
          updateShot(p1, &shot2); // p1 atira no p2
          
          drawPlayers();
          // mostrar vidas na tela
          drawLives();
      }
      else {
          endGame();
      }
    }
}

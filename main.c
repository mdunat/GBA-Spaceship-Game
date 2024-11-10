#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"

#include "court.h"
#include "BFS1.h"
#include "startscrn.h"
#include "asteroid1.h"
#include "Spaceship.h"
#include "Spaceshipboost.h"
#include "Alienship.h"
#include "fball1.h"
#include "fball2.h"
#include "fball3.h"
#include "fball1rev.h"
#include "fball2rev.h"
#include "fball3rev.h"
#include "win.h"
#include "lose.h"

enum gba_state {
  START,
  MENU,
  PLAY,
  WIN,
  LOSE,
};

int main(void) {
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  enum gba_state state = START;

  struct object ship;
  struct object asteroid;
  struct object comet;
  struct difficulty difficulty;

  while (1) {
    currentButtons = BUTTONS;
    
    REG_DISPCNT = MODE3 | BG2_ENABLE;

    waitForVBlank();

    // Beginning the states:

    switch (state) {
      case START:
        drawFullScreenImageDMA(startscrn);
        // static int p = 0;
        // p++;

        // if (p == 40)
        //   setPixel(78, 67, RED);
        //   setPixel(78, 66, RED);
        // else if (p == ) {
        //   setPixel(0,0,BLACK);
        // }

        // start of new copy:

        ship.x = 102;
        ship.y = 115;
        ship.width = 35;
        ship.height = 35;
        ship.cond = 0;

        asteroid.x = 60;
        asteroid.y = -60;
        asteroid.width = 60;
        asteroid.height = 60;
        asteroid.cond = 0;

        comet.x = 150;
        comet.y = -60;
        comet.width = 18;
        comet.height = 60;
        comet.cond = 1;


        // if (KEY_DOWN(BUTTON_START, currentButtons)) {
        //   fillScreenDMA(COLOR(0,0,0));
        //   drawImageDMA(ship.y, ship.x, 35, 35, Spaceship);
        //   state = PLAY;
        // }
        if (KEY_DOWN(BUTTON_START, currentButtons)) {
          fillScreenDMA(COLOR(0,0,0));
          char men_text[] = "select difficulty:";
          char men1[] = "easy";
          char men2[] = "medium";
          char men3[] = "hard";
          drawString(60, 65, men_text, WHITE);
          drawString(75, 60, men1, WHITE);
          drawString(75, 100, men2, YELLOW);
          drawString(75, 155, men3, WHITE);
          char star[] = "press start";
          drawString(120, 85, star, WHITE);
          state = MENU;
        }

        break;
      case MENU:
        setPixel(0, 0, BLACK);
        static int counter = 0;
        counter++;
        if (counter == 45) {
          drawRectDMA(120, 85, 70, 15, BLACK);
        }
        if (counter == 65) {
          char star[] = "press start";
          drawString(120, 85, star, WHITE);
          counter = 0;
        }

        static int yel = 2;
        char men1[] = "easy";
        char men2[] = "medium";
        char men3[] = "hard";

        if (KEY_JUST_PRESSED(BUTTON_LEFT, currentButtons, previousButtons)) {
          if (yel == 2) {
            drawRectDMA(75, 60, 95, 10, BLACK);
            drawString(75, 60, men1, YELLOW);
            drawString(75, 100, men2, WHITE);
            yel = 1;
          } else if (yel == 3) {
            drawRectDMA(75, 100, 100, 10, BLACK);
            drawString(75, 100, men2, YELLOW);
            drawString(75, 155, men3, WHITE);
            yel = 2;
          }
        }

        if (KEY_JUST_PRESSED(BUTTON_RIGHT, currentButtons, previousButtons)) {
          if (yel == 1) {
            drawRectDMA(75, 60, 95, 10, BLACK);
            drawString(75, 60, men1, WHITE);
            drawString(75, 100, men2, YELLOW);
            yel = 2;
          } else if (yel == 2) {
            drawRectDMA(75, 100, 100, 10, BLACK);
            drawString(75, 100, men2, WHITE);
            drawString(75, 155, men3, YELLOW);
            yel = 3;
          }
        }

        difficulty.a_speed = 1;
        difficulty.c_speed = 2;
        difficulty.time = 30;

        if (yel == 1) {
          difficulty.a_speed = 1;
          difficulty.c_speed = 1;
          difficulty.time = 15;
        } else if (yel == 2) {
          difficulty.a_speed = 1;
          difficulty.c_speed = 2;
          difficulty.time = 30;
        } else if (yel == 3) {
          difficulty.a_speed = 2;
          difficulty.c_speed = 2;
          difficulty.time = 45;
        }
        

        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          state = START;
          yel = 2;
        }

        if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
          fillScreenDMA(COLOR(0,0,0));
          drawImageDMA(ship.y, ship.x, 35, 35, Spaceship);
          state = PLAY;
          yel = 2;
        }

        break;
      case PLAY:
        drawRectDMA(143, 10, 20, 20, BLACK); // black box for timer

        // asteroids:

        if (asteroid.y <= 160 && comet.cond == 1) {
          drawRectDMA(asteroid.y - 1, asteroid.x, 60, 60, BLACK);
          drawImageDMA(asteroid.y, asteroid.x, 60, 60, asteroid1);
          asteroid.y += difficulty.a_speed; // change speed of asteroid
        } else {
          asteroid.y = -60;
          asteroid.x = (asteroid.x + 125) % 180;
          asteroid.cond = 1;
          comet.cond = 0;
        }

        if (comet.y <= 160 && asteroid.cond == 1) {
          static int flameType = 0;

          if (flameType <= 10 && flameType >= 0) {
            drawRectDMA(comet.y - 2, comet.x, 18, 60, BLACK);
            drawImageDMA(comet.y, comet.x, 18, 60, fball1rev);
          } else if (flameType <= 20 && flameType > 10) {
            drawRectDMA(comet.y - 2, comet.x, 18, 60, BLACK);
            drawImageDMA(comet.y, comet.x, 18, 60, fball2rev);
          } else {
            drawRectDMA(comet.y - 2, comet.x, 18, 60, BLACK);
            drawImageDMA(comet.y, comet.x, 18, 60, fball3rev);
          }
          
          comet.y += difficulty.c_speed; // change the speed of comet here: 2 og
          flameType = (flameType + 1) % 31;
        } else {
          comet.y = -60;
          comet.x = (comet.x + 234) % 210;
          comet.cond = 1;
          asteroid.cond = 0;
        }

        // Spaceship

        int lrud = 0;

        if KEY_DOWN(BUTTON_LEFT, currentButtons) {
          if (ship.x != 0) {
            ship.x -= 1;
            lrud = 1;
          }
        } else if KEY_DOWN(BUTTON_RIGHT, currentButtons) {
          if (ship.x != 205) {
            ship.x += 1;
            lrud = 2;
          }
        } else if KEY_DOWN(BUTTON_UP, currentButtons) { 
          if (ship.y != 0) {
            ship.y -= 1;
            lrud = 3;
          }
          // lrud = 3;
        } else if KEY_DOWN(BUTTON_DOWN, currentButtons) {
          if (ship.y != 125) {
            ship.y += 1;
            lrud = 4;
          }
          // lrud = 4;
        }
        
        if (lrud != 0) {
          if (lrud == 1) {
            drawRectDMA(ship.y, ship.x + 1, 35, 35, BLACK);
          } else if (lrud == 2) {
            drawRectDMA(ship.y, ship.x - 1, 35, 35, BLACK);
          } else if (lrud == 3) {
            drawRectDMA(ship.y + 1, ship.x, 35, 35, BLACK);
          } else if (lrud == 4) {
            drawRectDMA(ship.y - 1, ship.x, 35, 35, BLACK);
          }
          drawImageDMA(ship.y, ship.x, 35, 35, Spaceship);
        }

        // Collisions:

        int sleft = ship.x;
        int sright = ship.x + ship.width;
        int stop = ship.y;
        int sbottom = ship.y + ship.height;

        int aleft = asteroid.x + 10;
        int aright = asteroid.x + asteroid.width - 10;
        int atop = asteroid.y + 10;
        int abottom = asteroid.y + asteroid.width - 10;

        int cleft = comet.x + 3;
        int cright = comet.x + comet.width - 3;
        int ctop = comet.y + 5;
        int cbottom = comet.y + comet.height - 5;

        if (sleft < aright && sleft > aleft) {
          if ((stop < abottom && stop > atop) || (sbottom > atop && sbottom < abottom)) {
            state = LOSE;
          }
        } else if (sright > aleft && sright < aright) {
          if ((stop < abottom && stop > atop) || (sbottom > atop && sbottom < abottom)) {
            state = LOSE;
          }
        } else if (stop < abottom && stop > atop) {
          if ((sright > aleft && sright < aright) || (sleft < aright && sleft > aleft)) {
            state = LOSE;
          }
        } else if (sbottom > atop && sbottom < abottom) {
          if ((sright > aleft && sright < aright) || (sleft < aright && sleft > aleft)) {
            state = LOSE;
          }
        }

        if (sleft < cright && sleft > cleft) {
          if ((stop < cbottom && stop > ctop) || (sbottom > ctop && sbottom < cbottom)) {
            state = LOSE;
          }
        } else if (sright > cleft && sright < cright) {
          if ((stop < cbottom && stop > ctop) || (sbottom > ctop && sbottom < cbottom)) {
            state = LOSE;
          }
        } else if (stop < cbottom && stop > ctop) {
          if ((sright > cleft && sright < cright) || (sleft < cright && sleft > cleft) || (sright > cright && sleft < cleft)) {
            state = LOSE;
          }
        } else if (sbottom > ctop && sbottom < cbottom) {
          if ((sright > cleft && sright < cright) || (sleft < cright && sleft > cleft)) {
            state = LOSE;
          }
        }

        // Timer :
        
        
        // static int count = 30;
        static int pace = 1;

        // int ones = count % 10;
        // int tens = (count / 10) % 10;
        int ones = difficulty.time % 10;
        int tens = (difficulty.time / 10) % 10;

        
        char zero = 48;
        char a = zero + tens;
        char b = zero + ones;
        
        drawChar(143, 10, a, WHITE);
        drawChar(143, 20, b, WHITE);
        
        if (difficulty.time != 0 && pace == 60) {
        // if (count != 0 && pace == 60) {
          difficulty.time--;
          // count--;
          pace = 0;
        } else if (difficulty.time == 0) {
          state = WIN;
          drawFullScreenImageDMA(win);
        }

        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          state = START;
          // count = 30;
        }
        
        pace++;

        break;

      case WIN:
        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          state = START;
        }
        
        // count = 30;
        break;
      case LOSE:
        drawFullScreenImageDMA(lose);
        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          state = START;
        }
        
        // count = 30;
        break;
    }

    previousButtons = currentButtons; // Store the current state of the buttons
  }

  UNUSED(previousButtons); // You can remove this once previousButtons is used

  return 0;
}
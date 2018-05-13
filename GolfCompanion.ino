#include "src/utils/Arduboy2Ext.h"
#include "src/utils/Name.h"
#include "src/utils/Utils.h"
#include "src/images/Images.h"
#include "src/fonts/Font4x6.h"
 
Arduboy2Ext arduboy;
Font4x6 font4x6 = Font4x6();

Sprites sprite;
Name name;


//globals
uint8_t fadeWidth;
GameState state = GameState::VSBoot;
uint16_t backdropx = 0;
uint16_t backdropy = 0;
uint8_t playerNumber = 1;
uint8_t playerIndex = 1;    // used to control which player we are entering
uint8_t parHole1 = 1;


Game game;


void setup() {
  arduboy.begin();
  arduboy.setFrameRate(60);
  name.retrieve(123);
}

void resetFade()
{
  fadeWidth = 0;
}

// Resets the fade in effect
void resetFadeIn()
{
  fadeWidth = WIDTH;
}

// fade in function
bool fadeIn()
{
  for(uint8_t i = 0; i < (HEIGHT / 2); ++i)
  {
    arduboy.drawFastHLine(0, (i * 2), fadeWidth, BLACK);
    arduboy.drawFastHLine((WIDTH - fadeWidth), (i * 2) + 1, fadeWidth, BLACK);
  }

  // If fade isn't complete, decrease the counter
  if(fadeWidth > 0)
  {
    fadeWidth = fadeWidth - 4;
    return false;
  }
  else
    return true;
}

// fade out function
bool fadeOut()
{
  for(uint8_t i = 0; i < (HEIGHT / 2); ++i)
  {
    arduboy.drawFastHLine(0, (i * 2), fadeWidth, BLACK);
    arduboy.drawFastHLine((WIDTH - fadeWidth), (i * 2) + 1, fadeWidth, BLACK);
  }
  // If fade isn't complete, increase the counter
  if(fadeWidth < WIDTH)
  {
    ++fadeWidth;
    ++fadeWidth;
    return false;
  }
  else
    return true;
  
}

void drawbottomgrass()
{
  sprite.drawExternalMask(0, 46, grasstee, grassteemask, 0, 0);
}

void drawpopwindow()
{
  arduboy.fillRect(0, 26, 128, 15, BLACK);
  arduboy.drawLine(0, 27, 128, 27, WHITE);
  arduboy.drawLine(0, 39, 128, 39, WHITE);
}

void drawHeader()
{
    sprite.drawExternalMask(15, 2, GolfCompanionHeader, GolfCompanionHeadermask, 0,0);
}

void numberPlayers()
{
  scrollingBackground();
  drawbottomgrass();
  drawHeader();
  drawpopwindow();
  font4x6.setCursor(6, 29);
  font4x6.print("How many players? ");
  font4x6.println(playerNumber);
  if (arduboy.justPressed(UP_BUTTON) && playerNumber < 4)
  {
    ++playerNumber;
  }
  else if (arduboy.justPressed(DOWN_BUTTON)&& playerNumber > 1)
  {
    --playerNumber;
  }
  else if (arduboy.justPressed(A_BUTTON))
  {
   state = GameState::PlayerNames_Init;  
  }
}

void playerNames()
{
  scrollingBackground();
  drawbottomgrass();
  drawHeader();
  drawpopwindow();

  font4x6.setCursor(6, 29);
  font4x6.print(F("Player "));
  font4x6.print(playerIndex);
  font4x6.print(F(" name? "));

  for (uint8_t x = 0; x < NAME_LENGTH; x++)
  {

    font4x6.setCursor(95 + (x * NAME_CHARACTER_SPACING), 29);

    if (name.getCharIndex() == x)
    {
      arduboy.drawLine(95 + (x * NAME_CHARACTER_SPACING), 29 + 9, 95 + (x * NAME_CHARACTER_SPACING) + NAME_UNDERLINE_WIDTH, 29 + 9, WHITE);
    }

    font4x6.print(name.getChar(x));

  }


  // Handle buttons ..

  uint8_t charIndex = name.getCharIndex();

  if (arduboy.justPressed(UP_BUTTON))       { name.incChar(charIndex); }
  if (arduboy.justPressed(DOWN_BUTTON))     { name.decChar(charIndex); }
  if (arduboy.justPressed(LEFT_BUTTON))     { name.decCharIndex(); } 
  if (arduboy.justPressed(RIGHT_BUTTON))    { name.incCharIndex(); } 

  if (arduboy.justPressed(B_BUTTON))        { 

    if (playerIndex > 1) {

      state = GameState::PlayerNames_Init;
      playerIndex--;

    }
    else {

      state = GameState::NumberOfHoles;

    }

  }

  if (arduboy.justPressed(A_BUTTON))        { 
    
    name.save(EEPROM_PLAYER_NAMES + ((playerIndex - 1) * (NAME_LENGTH + 1)));

    if (playerIndex < playerNumber) {

      // Enter the next player's name ..

      state = GameState::PlayerNames_Init;
      playerIndex++;
    
    }
    else {

      state = GameState::NumberOfHoles;

    }
    
  }

}

void numberHoles()
{
  scrollingBackground();
  drawbottomgrass();
  drawHeader();
  drawpopwindow();
  font4x6.setCursor(6, 29);
  font4x6.print("How many Holes? ");
  font4x6.println(game.numberOfHoles);
  if (arduboy.justPressed(UP_BUTTON)&& game.numberOfHoles < 18)
  {
    ++game.numberOfHoles;
  }
  else if (arduboy.justPressed(DOWN_BUTTON)&& game.numberOfHoles > 1)
  {
    --game.numberOfHoles;
  }
  else if (arduboy.justPressed(A_BUTTON))
  {
   state = GameState::InGame_Init; 
  }
}

// void parHoles()
// {
  
//   scrollingBackground();
//   drawbottomgrass();
//   drawHeader();
//   drawpopwindow();
//   font4x6.setCursor(6, 29);
//   font4x6.print("PAR for hole #");
//   font4x6.print(game.numberOfHoles);
//   font4x6.print(" : ");
//   font4x6.print(parHole1);
//   if (arduboy.justPressed(UP_BUTTON)&& parHole1 < 20)
//   {
//     ++parHole1;
//   }
//   else if (arduboy.justPressed(DOWN_BUTTON)&& parHole1 > 1)
//   {
//     --parHole1;
//   }
//   else if (arduboy.justPressed(A_BUTTON))
//   {
//    state = GameState::InGame_Init; 
//   }
// }

void scrollingBackground()
{
  arduboy.drawBitmap(backdropx, backdropy, GolfBackground, 128, 64, WHITE);
  arduboy.drawBitmap(backdropx + 128, backdropy, GolfBackground, 128, 64, WHITE);
  if(arduboy.everyXFrames(5)) // when running at 60fps
  {
    --backdropx;
      if( backdropx < -128 )
  {
    backdropx = 0;
  }
  }
}


void loop()
{
  if (!(arduboy.nextFrame()))
    return;

  arduboy.clear();
  arduboy.pollButtons();
  arduboy.clear();

    switch (state)
    {

    case GameState::VSBoot:
      vsBoot();
      break;

    case GameState::SplashScreen:
      splashScreen();
      break;

    case GameState::NumberOfPlayers:
      numberPlayers();
      break;

    case GameState::PlayerNames_Init:
      state = GameState::PlayerNames;
      name.retrieve(EEPROM_PLAYER_NAMES + ((playerIndex - 1) * (NAME_LENGTH + 1)));

    case GameState::PlayerNames:
      playerNames();
      break;

    case GameState::NumberOfHoles:
      numberHoles();
      break;

    case GameState::InGame_Init:
      state = GameState::InGame;
      name.retrieve(EEPROM_PLAYER_NAMES + (0 * (NAME_LENGTH + 1)));    strcpy(game.name1, name.getString());
      name.retrieve(EEPROM_PLAYER_NAMES + (1 * (NAME_LENGTH + 1)));    strcpy(game.name2, name.getString());
      name.retrieve(EEPROM_PLAYER_NAMES + (2 * (NAME_LENGTH + 1)));    strcpy(game.name3, name.getString());
      name.retrieve(EEPROM_PLAYER_NAMES + (3 * (NAME_LENGTH + 1)));    strcpy(game.name4, name.getString());

    case GameState::InGame:
      ingame();
      break;

    case GameState::FinalScore:
      finalScoreDisplay();
      break;
      
    } 
  
  arduboy.display();
}

void vsBoot()
{
  // Vsoft logo display
  arduboy.drawBitmap(0, 0, bootlogo, 128, 64, WHITE);

  if(fadeOut())
  {
    resetFade();
    resetFadeIn();
    state = GameState::SplashScreen;
  }
}

void splashScreen()
{
  scrollingBackground();
  drawbottomgrass();
  sprite.drawExternalMask(30, 24, GolfCompanionSplash, GolfCompanionSplashmask, 0,0);
  fadeIn();

  // If 'A' button is pressed move to gameplay
  if (arduboy.justPressed(A_BUTTON))
  {
    state = GameState::NumberOfPlayers; 
    resetFadeIn();
  }
}

uint8_t cursorX = 0;
uint8_t cursorY = 0;
uint8_t cursorMode = 0;

#define GRID_VERT_SPACING 11
#define GRID_HORZ_SPACING 16
#define GRID_HOLE_1_X 40
#define GRID_TOTAL_X 100
#define GRID_PAR_Y 11

void ingame()
{

  font4x6.setCursor(0, 0);
  font4x6.print(F("Hole"));
  font4x6.setCursor(100, 0);
  font4x6.print(F("Total"));
  font4x6.setCursor(0, 11);
  font4x6.print(F("Par"));

  font4x6.setCursor(0, GRID_PAR_Y + (1 * GRID_VERT_SPACING));
  font4x6.print(game.name1);
  font4x6.setCursor(0, GRID_PAR_Y + (2 * GRID_VERT_SPACING));
  font4x6.print(game.name2);
  font4x6.setCursor(0, GRID_PAR_Y + (3 * GRID_VERT_SPACING));
  font4x6.print(game.name3);
  font4x6.setCursor(0, GRID_PAR_Y + (4 * GRID_VERT_SPACING));
  font4x6.print(game.name4);

/*
  arduboy.drawLine(0, GRID_PAR_Y - 2, WIDTH, GRID_PAR_Y - 2, WHITE);
  arduboy.drawLine(0, GRID_PAR_Y + (1 * GRID_VERT_SPACING) - 2, WIDTH, GRID_PAR_Y + (1 * GRID_VERT_SPACING) - 2, WHITE);
  arduboy.drawLine(0, GRID_PAR_Y + (2 * GRID_VERT_SPACING) - 2, WIDTH, GRID_PAR_Y + (2 * GRID_VERT_SPACING) - 2, WHITE);
  arduboy.drawLine(0, GRID_PAR_Y + (3 * GRID_VERT_SPACING) - 2, WIDTH, GRID_PAR_Y + (3 * GRID_VERT_SPACING) - 2, WHITE);
  arduboy.drawLine(0, GRID_PAR_Y + (4 * GRID_VERT_SPACING) - 2, WIDTH, GRID_PAR_Y + (4 * GRID_VERT_SPACING) - 2, WHITE);

  arduboy.drawLine(GRID_HOLE_1_X, 0, GRID_HOLE_1_X, HEIGHT, WHITE);
  arduboy.drawLine(GRID_HOLE_1_X + (1 * GRID_HORZ_SPACING), 0, GRID_HOLE_1_X + (1 * GRID_HORZ_SPACING), HEIGHT, WHITE);
  arduboy.drawLine(GRID_HOLE_1_X + (2 * GRID_HORZ_SPACING), 0, GRID_HOLE_1_X + (2 * GRID_HORZ_SPACING), HEIGHT, WHITE);
  arduboy.drawLine(GRID_HOLE_1_X + (3 * GRID_HORZ_SPACING), 0, GRID_HOLE_1_X + (3 * GRID_HORZ_SPACING), HEIGHT, WHITE);
*/

  arduboy.drawHorizontalDottedLine(0, WIDTH, GRID_PAR_Y - 2, 2);
  arduboy.drawHorizontalDottedLine(0, WIDTH, GRID_PAR_Y + (1 * GRID_VERT_SPACING) - 2, 2);
  arduboy.drawHorizontalDottedLine(0, WIDTH, GRID_PAR_Y + (2 * GRID_VERT_SPACING) - 2, 2);
  arduboy.drawHorizontalDottedLine(0, WIDTH, GRID_PAR_Y + (3 * GRID_VERT_SPACING) - 2, 2);
  arduboy.drawHorizontalDottedLine(0, WIDTH, GRID_PAR_Y + (4 * GRID_VERT_SPACING) - 2, 2);

  arduboy.drawVerticalDottedLine(0, HEIGHT, GRID_HOLE_1_X, 2);
  arduboy.drawVerticalDottedLine(0, HEIGHT, GRID_HOLE_1_X + (1 * GRID_HORZ_SPACING), 2);
  arduboy.drawVerticalDottedLine(0, HEIGHT, GRID_HOLE_1_X + (2 * GRID_HORZ_SPACING), 2);
  arduboy.drawVerticalDottedLine(0, HEIGHT, GRID_HOLE_1_X + (3 * GRID_HORZ_SPACING), 2);

  font4x6.setCursor(GRID_HOLE_1_X + 3, 0);
  font4x6.print(game.currentHoleNumber - 2);
  font4x6.setCursor(GRID_HOLE_1_X + (1 * GRID_HORZ_SPACING) + 3, 0);
  font4x6.print(game.currentHoleNumber - 1);
  font4x6.setCursor(GRID_HOLE_1_X + (2 * GRID_HORZ_SPACING) + 3, 0);
  font4x6.print(game.currentHoleNumber);


  font4x6.setCursor(GRID_HOLE_1_X + 3, GRID_PAR_Y);
  font4x6.print(game.holes[game.currentHoleNumber - 3].par);
  font4x6.setCursor(GRID_HOLE_1_X + (1 * GRID_HORZ_SPACING) + 3, GRID_PAR_Y);
  font4x6.print(game.holes[game.currentHoleNumber - 2].par);
  font4x6.setCursor(GRID_HOLE_1_X + (2 * GRID_HORZ_SPACING) + 3, GRID_PAR_Y);
  font4x6.print(game.holes[game.currentHoleNumber - 1].par);

  font4x6.setCursor(GRID_HOLE_1_X + 3, GRID_PAR_Y + (1 * GRID_VERT_SPACING));
  font4x6.print(game.holes[game.currentHoleNumber - 3].player1Score);
  font4x6.setCursor(GRID_HOLE_1_X + 3, GRID_PAR_Y + (2 * GRID_VERT_SPACING));
  font4x6.print(game.holes[game.currentHoleNumber - 3].player2Score);
  font4x6.setCursor(GRID_HOLE_1_X + 3, GRID_PAR_Y + (3 * GRID_VERT_SPACING));
  font4x6.print(game.holes[game.currentHoleNumber - 3].player3Score);
  font4x6.setCursor(GRID_HOLE_1_X + 3, GRID_PAR_Y + (4 * GRID_VERT_SPACING));
  font4x6.print(game.holes[game.currentHoleNumber - 3].player4Score);

  font4x6.setCursor(GRID_HOLE_1_X + (1 * GRID_HORZ_SPACING) + 3, GRID_PAR_Y + (1 * GRID_VERT_SPACING));
  font4x6.print(game.holes[game.currentHoleNumber - 2].player1Score);
  font4x6.setCursor(GRID_HOLE_1_X + (1 * GRID_HORZ_SPACING) + 3, GRID_PAR_Y + (2 * GRID_VERT_SPACING));
  font4x6.print(game.holes[game.currentHoleNumber - 2].player2Score);
  font4x6.setCursor(GRID_HOLE_1_X + (1 * GRID_HORZ_SPACING) + 3, GRID_PAR_Y + (3 * GRID_VERT_SPACING));
  font4x6.print(game.holes[game.currentHoleNumber - 2].player3Score);
  font4x6.setCursor(GRID_HOLE_1_X + (1 * GRID_HORZ_SPACING) + 3, GRID_PAR_Y + (4 * GRID_VERT_SPACING));
  font4x6.print(game.holes[game.currentHoleNumber - 2].player4Score);

  font4x6.setCursor(GRID_HOLE_1_X + (2 * GRID_HORZ_SPACING) + 3, GRID_PAR_Y + (1 * GRID_VERT_SPACING));
  font4x6.print(game.holes[game.currentHoleNumber - 1].player1Score);
  font4x6.setCursor(GRID_HOLE_1_X + (2 * GRID_HORZ_SPACING) + 3, GRID_PAR_Y + (2 * GRID_VERT_SPACING));
  font4x6.print(game.holes[game.currentHoleNumber - 1].player2Score);
  font4x6.setCursor(GRID_HOLE_1_X + (2 * GRID_HORZ_SPACING) + 3, GRID_PAR_Y + (3 * GRID_VERT_SPACING));
  font4x6.print(game.holes[game.currentHoleNumber - 1].player3Score);
  font4x6.setCursor(GRID_HOLE_1_X + (2 * GRID_HORZ_SPACING) + 3, GRID_PAR_Y + (4 * GRID_VERT_SPACING));
  font4x6.print(game.holes[game.currentHoleNumber - 1].player4Score);

  font4x6.setCursor(GRID_TOTAL_X + 3, GRID_PAR_Y + (1 * GRID_VERT_SPACING));
  font4x6.print(game.total.player1Score);
  font4x6.setCursor(GRID_TOTAL_X + 3, GRID_PAR_Y + (2 * GRID_VERT_SPACING));
  font4x6.print(game.total.player2Score);
  font4x6.setCursor(GRID_TOTAL_X + 3, GRID_PAR_Y + (3 * GRID_VERT_SPACING));
  font4x6.print(game.total.player3Score);
  font4x6.setCursor(GRID_TOTAL_X + 3, GRID_PAR_Y + (4 * GRID_VERT_SPACING));
  font4x6.print(game.total.player4Score);

  switch (cursorMode) {

    case 0: // Move cursor around ..
      
      arduboy.drawRect(GRID_HOLE_1_X + (cursorX * GRID_HORZ_SPACING) + 1, GRID_PAR_Y + (1 * GRID_VERT_SPACING) + ((cursorY - 1) * GRID_VERT_SPACING) - 1, (GRID_HORZ_SPACING - 1), (GRID_VERT_SPACING - 1));

      if (arduboy.justPressed(UP_BUTTON) && cursorY > 0) cursorY--;
      if (arduboy.justPressed(DOWN_BUTTON) && cursorY < 4) cursorY++;

      if (arduboy.justPressed(LEFT_BUTTON)) {
        if (cursorX > 0) {
          cursorX--;
        }
        else {
          if (game.currentHoleNumber > 3) game.currentHoleNumber--;
        }
      }


      if (arduboy.justPressed(RIGHT_BUTTON)) {
        if (cursorX < 2) {
          cursorX++;
        }
        else {
          if (game.currentHoleNumber < game.numberOfHoles) game.currentHoleNumber++;
        }
      }

      if (arduboy.justPressed(A_BUTTON)) cursorMode = 1;

      break;

    case 1: // Change score ..

      arduboy.drawLine(GRID_HOLE_1_X + (cursorX * GRID_HORZ_SPACING) + 2, GRID_PAR_Y + (1 * GRID_VERT_SPACING) + (cursorY * GRID_VERT_SPACING) - 3, GRID_HOLE_1_X + ((cursorX + 1) * GRID_HORZ_SPACING) - 2, GRID_PAR_Y + (1 * GRID_VERT_SPACING) + (cursorY * GRID_VERT_SPACING) - 3);


      // Hole Par ..

      if (arduboy.justPressed(UP_BUTTON) && cursorY == 0) {

        uint8_t par = game.holes[game.currentHoleNumber - (3 - cursorX)].par;
        if (par <= 9) game.holes[game.currentHoleNumber - (3 - cursorX)].par++;

      }

      if (arduboy.justPressed(DOWN_BUTTON) && cursorY == 0) {

        uint8_t par = game.holes[game.currentHoleNumber - (3 - cursorX)].par;
        if (par > 1) game.holes[game.currentHoleNumber - (3 - cursorX)].par--;

      }


      // Player scores ..

      if (arduboy.justPressed(UP_BUTTON) && cursorY > 0) {

        uint8_t playerScore = game.holes[game.currentHoleNumber - (3 - cursorX)].getPlayerScore(cursorY);
        uint8_t totalScore = game.total.getPlayerScore(cursorY);

        if (playerScore <= 9) {

          game.holes[game.currentHoleNumber - (3 - cursorX)].setPlayerScore(cursorY, playerScore + 1);
          game.total.setPlayerScore(cursorY, totalScore + 1);

        }

      }

      if (arduboy.justPressed(DOWN_BUTTON) && cursorY > 0) {

        uint8_t playerScore = game.holes[game.currentHoleNumber - (3 - cursorX)].getPlayerScore(cursorY);
        uint8_t totalScore = game.total.getPlayerScore(cursorY);

        if (playerScore > 0) {
          
          game.holes[game.currentHoleNumber - (3 - cursorX)].setPlayerScore(cursorY, playerScore - 1);
          game.total.setPlayerScore(cursorY, totalScore - 1);

        }

      }

      if (arduboy.justPressed(A_BUTTON)) cursorMode = 0;

      break;

  }


  // Handle keypress

}

void finalScoreDisplay()
{
  font4x6.print("Here are the final scores");
}

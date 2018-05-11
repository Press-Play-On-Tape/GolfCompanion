#include <Arduboy2.h>
#include "Name.h"
#include "Images.h"
 
Arduboy2 arduboy;
Sprites sprite;
Name name;

//globals
uint8_t fadeWidth;
uint8_t state = 0;
uint16_t backdropx = 0;
uint16_t backdropy = 0;
uint8_t playerNumber = 1;
uint8_t holeNumber = 1;
uint8_t parHole1 = 1;



void setup() {
  arduboy.begin();
  arduboy.setFrameRate(60);

  name.retrieve(123);

  // initialize things here

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
  arduboy.setCursor(6, 30);
  arduboy.print("How many players? ");
  arduboy.println(playerNumber);
  if (arduboy.justPressed(UP_BUTTON)&& playerNumber < 4)
  {
    ++playerNumber;
  }
  else if (arduboy.justPressed(DOWN_BUTTON)&& playerNumber > 1)
  {
    --playerNumber;
  }
  else if (arduboy.justPressed(A_BUTTON))
  {
   state = 3; 
  }
}

void playerNames()
{
  scrollingBackground();
  drawbottomgrass();
  drawHeader();
  drawpopwindow();
  arduboy.setCursor(6, 30);
  arduboy.print("Player 1 name? ");
  for (uint8_t x = 0; x < NAME_LENGTH; x++)
  {

    arduboy.setCursor(95 + (x * NAME_CHARACTER_SPACING), 30);

    if (name.getCharIndex() == x)
    {
      arduboy.drawLine(95 + (x * NAME_CHARACTER_SPACING), 30 + 9, 95 + (x * NAME_CHARACTER_SPACING) + NAME_UNDERLINE_WIDTH, 30 + 9, WHITE);
    }

    arduboy.print(name.getChar(x));

  }


  // Handle buttons ..

  uint8_t charIndex = name.getCharIndex();

  if (arduboy.justPressed(UP_BUTTON))       { name.incChar(charIndex); }
  if (arduboy.justPressed(DOWN_BUTTON))     { name.decChar(charIndex); }
  if (arduboy.justPressed(LEFT_BUTTON))     { name.decCharIndex(); } 
  if (arduboy.justPressed(RIGHT_BUTTON))    { name.incCharIndex(); } 

  if (arduboy.justPressed(A_BUTTON))        { name.save(123); state = 4; }


}

void numberHoles()
{
  scrollingBackground();
  drawbottomgrass();
  drawHeader();
  drawpopwindow();
  arduboy.setCursor(6, 30);
  arduboy.print("How many Holes? ");
  arduboy.println(holeNumber);
  if (arduboy.justPressed(UP_BUTTON)&& holeNumber < 18)
  {
    ++holeNumber;
  }
  else if (arduboy.justPressed(DOWN_BUTTON)&& holeNumber > 1)
  {
    --holeNumber;
  }
  else if (arduboy.justPressed(A_BUTTON))
  {
   state = 5; 
  }
}

void parHoles()
{
  
  scrollingBackground();
  drawbottomgrass();
  drawHeader();
  drawpopwindow();
  arduboy.setCursor(6, 30);
  arduboy.print("PAR for hole #");
  arduboy.print(holeNumber);
  arduboy.print(" : ");
  arduboy.print(parHole1);
  if (arduboy.justPressed(UP_BUTTON)&& parHole1 < 20)
  {
    ++parHole1;
  }
  else if (arduboy.justPressed(DOWN_BUTTON)&& parHole1 > 1)
  {
    --parHole1;
  }
  else if (arduboy.justPressed(A_BUTTON))
  {
   state = 5; 
  }
}

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

    case 0:
      vsBoot();
      break;

    case 1:
      splashScreen();
      break;

    case 2:
      numberPlayers();
      break;

    case 3:
      playerNames();
      break;

    case 4:
      numberHoles();
      break;

    case 5:
      parHoles();
      break;

    case 6:
      ingame();
      break;

    case 7:
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
    state = 1;
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
    state = 2; 
    resetFadeIn();
  }
}

void ingame()
{
  arduboy.print("Here are the final scores");
}

void finalScoreDisplay()
{
  arduboy.print("Here are the final scores");
}

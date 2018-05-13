#pragma once

#include <Arduboy2.h>

//Starting location in EEPROM for Players Names
#define EEPROM_PLAYER_NAMES 150    

// Name 1 ->   EPROM Loc 150 - 153
// 0 at 154
// Name 2 ->   EPROM Loc 155 - 158
// 0 at 159
// Name 3 ->   EPROM Loc 160 - 163
// 0 at 164
// Name 4 ->   EPROM Loc 165 - 168
// 0 at 169




struct Hole {

  uint8_t par = 4;
  
  uint8_t player1Score;
  uint8_t player2Score;
  uint8_t player3Score;
  uint8_t player4Score;

  uint8_t getPlayerScore(uint8_t index) {

    switch (index) {

      case 1: return player1Score;
      case 2: return player2Score;
      case 3: return player3Score;
      case 4: return player4Score;
      default: return 0;
    }

  }

  void setPlayerScore(uint8_t index, uint8_t val) {

    switch (index) {

      case 1: player1Score = val; break;
      case 2: player2Score = val; break;
      case 3: player3Score = val; break;
      case 4: player4Score = val; break;

    }

  }

};

struct Game {

  uint8_t numberOfHoles = 18;
  uint8_t currentHoleNumber = 3;

  char name1[NAME_LENGTH + 1];
  char name2[NAME_LENGTH + 1];
  char name3[NAME_LENGTH + 1];
  char name4[NAME_LENGTH + 1];

  Hole holes[18];
  Hole total;

};


enum class GameState : uint8_t {

  VSBoot,
  SplashScreen,
  NumberOfPlayers,
  PlayerNames_Init,
  PlayerNames,
  NumberOfHoles,
//  ParHoles,
  InGame_Init,
  InGame,
  FinalScore

};
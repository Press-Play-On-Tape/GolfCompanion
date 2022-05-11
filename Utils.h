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

enum class GameState : uint8_t {

  SplashScreen,
  NumberOfPlayers,
  PlayerNames_Init,
  PlayerNames,
  NumberOfHoles,
  ParHoles,
  InGame,
  FinalScore

};
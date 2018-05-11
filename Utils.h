#pragma once

#include <Arduboy2.h>

enum class GameState : uint8_t {

  VSBoot,
  SplashScreen,
  NumberOfPlayers,
  PlayerNames_Init,
  PlayerNames,
  NumberOfHoles,
  ParHoles,
  InGame,
  FinalScore

};
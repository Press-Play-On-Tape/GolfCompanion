
// ----------------------------------------------------------------------------
//  Initialise state ..
//
void ppot_Init() {

    game.setState(GameState::PPOT);

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void ppot() { 

    if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) {
     fadeIn();
    }
    if(fadeOut())
    {
      resetFade();
      resetFadeIn();
      game.setState(GameState::SplashScreen);
    }
    Sprites::drawOverwrite(32, 17, PPOT, 0);

    uint8_t y = 17; // Default pixel position 1 is hidden in the top line of the image
    switch (arduboy.frameCount % 48) {

        case 12 ... 23:
            y = 30; // Move pixel down to position 2
            /*-fallthrough*/
        case 0 ... 11:
            Sprites::drawOverwrite(91, 25, PPOT_Arrow, 0); // Flash 'Play' arrow
            break;

        case 24 ... 35:
            y = 31; // Move pixel down to position 3
            break;

        default: // 36 ... 47:
            y = 32; // Move pixel down to position 4
            break;

    }

    arduboy.drawPixel(52, y, WHITE); // Falling pixel represents the tape spooling
    if (y % 2 == 0) { // On even steps of pixel movement, update the spindle image
        Sprites::drawOverwrite(45, 28, PPOT_Spindle, 0);
    }

    arduboy.display();

}

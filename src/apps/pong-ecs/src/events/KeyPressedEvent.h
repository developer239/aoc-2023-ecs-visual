#pragma once

#include <SDL.h>

#include "events/Event.h"

class KeyPressedEvent : public Events::EventBase {
 public:
  SDL_Keycode symbol;

  explicit KeyPressedEvent(SDL_Keycode symbol) {
    this->symbol = symbol;
  }
};

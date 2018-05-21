// -*- mode: c++ -*-
// Copyright 2018 Laszlo Attila Toth
// See "COPYING" for license details

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "built by TLA"
#endif

#include "Kaleidoscope.h"

#include "Kaleidoscope-OneShot.h"
#include "Kaleidoscope-ShapeShifter.h"
#include <Kaleidoscope-LED-ActiveModColor.h>
#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-LEDControl.h"

// Support for "Numpad" mode, which is mostly just the Numpad specific LED mode
#include "Kaleidoscope-NumPad.h"

#include "LED-Off.h"
#include "Kaleidoscope-LEDEffect-SolidColor.h"
#include "Kaleidoscope-LEDEffect-Rainbow.h"

// Support for the "Boot greeting" effect, which pulses the 'LED' button for 10s
// when the keyboard is connected to a computer (or that computer is powered on)
#include "Kaleidoscope-LEDEffect-BootGreeting.h"

// Support for an LED mode that lights up the keys as you press them
#include "Kaleidoscope-LED-Stalker.h"

// MACRO NAMES
enum { MACRO_VERSION_INFO,
       MACRO_ANY
     };

//
// Shortening and mapping names for the keymap layers
//

// Mouse-related
#define MW(x) Key_mouseWarp ## x
#define MM(x) Key_mouse ## x
#define MS(x) Key_mouseScroll ## x


// Others
#define KK(x) Key_Keypad ## x
#define KC(x) Consumer_ ## x

// Renames
#define Key_Esc       Key_Escape
#define Key_PrtScr    Key_PrintScreen
#define Key_Ins       Key_Insert
#define Key_Del       Key_Delete
#define Key_PageDn    Key_PageDown
#define Key_LArrow    Key_LeftArrow
#define Key_RArrow    Key_RightArrow
#define Key_DnArrow   Key_DownArrow

#define Consumer_VolDec        Consumer_VolumeDecrement
#define Consumer_VolInc        Consumer_VolumeIncrement

//
// End of shortening and mapping
//

/** The Model 01's key layouts are defined as 'keymaps'.
       https://github.com/keyboardio/Kaleidoscope/blob/master/src/key_defs_keyboard.h
       https://github.com/keyboardio/Kaleidoscope/blob/master/src/key_defs_consumerctl.h
       https://github.com/keyboardio/Kaleidoscope/blob/master/src/key_defs_sysctl.h
       https://github.com/keyboardio/Kaleidoscope/blob/master/src/key_defs_keymaps.h

*/

// layers
enum { MINE, MINE_SHIFT, DVORAK, NUMPAD, FUNCTION, NAV, AUX };

/* This comment temporarily turns off astyle's indent enforcement
     so we can make the keymaps actually resemble the physical key layout better
*/
// *INDENT-OFF*

KEYMAPS(

  [MINE] = KEYMAP_STACKED
  (___,          Key_1, Key_2, Key_3, Key_4, Key_5, Key_LEDEffectNext,
   Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_PageDown, Key_Y, Key_X, Key_C, Key_V, Key_B, Key_Escape,
   Key_LeftControl, Key_Space, Key_LeftGui, Key_LeftShift,
   OSL(NAV),

   LockLayer(DVORAK), Key_6, Key_7, Key_8,     Key_9,      Key_0,         LockLayer(NUMPAD),
   Key_Enter,         Key_Z, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,
                      Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
   OSL(FUNCTION),     Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,
   OSM(LeftAlt), OSM(RightShift), Key_Space, Key_Backspace,
   OSL(AUX)
   ),

  [MINE_SHIFT] = KEYMAP_STACKED
  (___, Key_6, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___,
   ___,

   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
        ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___,
   ___),

  [DVORAK] = KEYMAP_STACKED
  (___,          Key_1,         Key_2,     Key_3,      Key_4, Key_5, Key_LEDEffectNext,
   Key_Backtick, Key_Quote,     Key_Comma, Key_Period, Key_P, Key_Y, Key_Tab,
   Key_PageUp,   Key_A,         Key_O,     Key_E,      Key_U, Key_I,
   Key_PageDown, Key_Semicolon, Key_Q,     Key_J,      Key_K, Key_X, Key_Escape,
   Key_LeftControl, Key_Space, Key_LeftGui, Key_LeftShift,
   OSL(NAV),

   ___,            Key_6, Key_7, Key_8, Key_9, Key_0, LockLayer(NUMPAD),
   Key_Enter,      Key_F, Key_G, Key_C, Key_R, Key_L, Key_Slash,
                   Key_D, Key_H, Key_T, Key_N, Key_S, Key_Minus,
   OSL(FUNCTION),  Key_B, Key_M, Key_W, Key_V, Key_Z, Key_Equals,
   OSM(RightShift), Key_LeftAlt, Key_Space, Key_Backspace,
   OSL(AUX)
   ),

  [NUMPAD] =  KEYMAP_STACKED
  (___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___,
   ___,

   M(MACRO_VERSION_INFO),  ___, KK(7), KK(8),   KK(9),        KK(Subtract), ___,
   ___,                    ___, KK(4), KK(5),   KK(6),        KK(Add),      LSHIFT(Key_7),
                           ___, KK(1), KK(2),   KK(3),        Key_Equals,   Key_Quote,
   ___,                    ___, KK(0), KK(Dot), KK(Multiply), KK(Divide),   Key_Enter,
   ___, ___, ___, ___,
   ___),

  [FUNCTION] =  KEYMAP_STACKED
  (___,      Key_F1,      Key_F2,   Key_F3, Key_F4,   Key_F5, XXX,
   Key_Tab,  XXX,         MM(Up),   XXX,    MM(BtnR), MW(End), MW(NE),
   Key_Home, MM(L),       MM(Dn),   MM(R),  MM(BtnL), MW(NW),
   Key_End,  Key_PrtScr,  Key_Ins,  XXX,    MM(BtnM), MW(SW), MW(SE),
   ___, Key_Del, Key_Enter, ___,
   ___,

   Consumer_ScanPreviousTrack, Key_F6,                 Key_F7,                   Key_F8,                   Key_F9,          Key_F10,          Key_F11,
   Consumer_PlaySlashPause,    Consumer_ScanNextTrack, Key_LeftCurlyBracket,     Key_RightCurlyBracket,    Key_LeftBracket, Key_RightBracket, Key_F12,
                               Key_LeftArrow,          Key_DownArrow,            Key_UpArrow,              Key_RightArrow,  XXX,              XXX,
   Key_PcApplication,          Consumer_Mute,          Consumer_VolumeDecrement, Consumer_VolumeIncrement, XXX,    Key_Backslash,    Key_Pipe,
   Key_RightControl, OSM(LeftAlt), Key_Delete , OSM(RightControl),
   ___),

  [NAV] =  KEYMAP_STACKED
  (___,  Key_F1, Key_F2, Key_F3, Key_F4, Key_F5, XXX,
   XXX,  XXX,    MW(NW), MM(Up), MW(NE), MS(Up), MW(End),
   XXX,  MS(R),  MM(L),  MM(Dn), MM(R),  MS(Dn),
   XXX,  MS(L),  MW(SW), XXX,    MW(SE), XXX,    MM(BtnL),
   ___, Key_Del, Key_Enter, ___,
   ___,

   XXX,        Key_F6,     Key_F7,     Key_F8,      Key_F9,     Key_F10, Key_F11,
   MM(BtnM),   Key_PageUp, Key_Home,   Key_UpArrow, Key_End,    XXX,     Key_F12,
               Key_PageDn, Key_LArrow, Key_DnArrow, Key_RArrow, XXX,     XXX,
   MM(BtnR),   XXX,        XXX,        XXX,         XXX,        XXX,     XXX,
   Key_RightControl, OSM(LeftAlt), Key_Delete , OSM(RightControl),
   ___),

  [AUX] =  KEYMAP_STACKED
  (___,                   Key_F1,        Key_F2,    Key_F3,           Key_F4,     Key_F5,            KC(Mute),
   KC(PlaySlashPause),    XXX,           XXX,       Key_Semicolon,    XXX,        XXX,              KC(VolDec),
   KC(ScanPreviousTrack), Key_Quote,     Key_Minus, Key_RightBracket, Key_Equals, Key_0,
   KC(ScanNextTrack),     Key_Backslash, XXX,       XXX,              XXX,        Key_LeftBracket,  KC(VolInc),
   ___, Key_Del, Key_Enter, ___,
   ___,

   M(MACRO_ANY),        Key_F6,        Key_F7,                 Key_F8,       Key_F9,     Key_F10,         Key_F11,
   KC(PlaySlashPause),  Key_Minus,     Key_RightBracket,       Key_Backtick, Key_Equals, Key_0,           Key_F12,
                        Key_Backslash, LALT(Key_J),            LALT(Key_I), XXX, Key_LeftBracket, XXX,
   Key_PcApplication,   XXX, LALT(LCTRL(LGUI(LSHIFT(Key_M)))), XXX,          XXX,        OSL(FUNCTION),   XXX,
   Key_RightControl, OSM(LeftAlt), Key_Delete , OSM(RightControl),
   ___),

) // KEYMAPS()

/* Re-enable astyle's indent enforcement */
// *INDENT-ON*

SHSH_LAYERS(
  SHSH_LAYER(MINE, MINE_SHIFT),
  SHSH_LAYER(DVORAK, MINE_SHIFT)
);

static void versionInfoMacro(uint8_t keyState) {
  if (keyToggledOn(keyState)) {
    Macros.type(PSTR("Keyboardio Model 01 - Kaleidoscope "));
    Macros.type(PSTR(BUILD_INFORMATION));
  }
}

static void anyKeyMacro(uint8_t keyState) {
  static Key lastKey;
  if (keyToggledOn(keyState))
    lastKey.keyCode = Key_A.keyCode + (uint8_t)(millis() % 36);

  if (keyIsPressed(keyState))
    kaleidoscope::hid::pressKey(lastKey);
}

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  switch (macroIndex) {

    case MACRO_VERSION_INFO:
      versionInfoMacro(keyState);
      break;

    case MACRO_ANY:
      anyKeyMacro(keyState);
      break;
  }
  return MACRO_NONE;
}

// These 'solid' color effect definitions define a rainbow of
// LED color modes calibrated to draw 500mA or less on the
// Keyboardio Model 01.
static kaleidoscope::LEDSolidColor solidOrange(140, 70, 0);
static kaleidoscope::LEDSolidColor solidIndigo(0, 0, 170);

// First, tell Kaleidoscope which plugins you want to use.
// The order can be important. For example, LED effects are
// added in the order they're listed here.
KALEIDOSCOPE_INIT_PLUGINS(
  // The boot greeting effect pulses the LED button for 10 seconds after the keyboard is first connected
  BootGreetingEffect,

  // LEDControl provides support for other LED modes
  LEDControl,
  LEDOff,
  LEDRainbowEffect,
  solidOrange, solidIndigo,

  // The stalker effect lights up the keys you've pressed recently
  StalkerEffect,

  // The numpad plugin is responsible for lighting up the 'numpad' mode
  // with a custom LED effect
  NumPad,

  // The macros plugin adds support for macros
  Macros,

  OneShot,
  ActiveModColorEffect,
  ShapeShifter,

  // The MouseKeys plugin lets you add keys to your keymap which move the mouse.
  MouseKeys
)

void setup() {
  // First, call Kaleidoscope's internal setup function
  Kaleidoscope.setup();

  NumPad.numPadLayer = NUMPAD;

  // We set the brightness of the rainbow effects to 150 (on a scale of 0-255)
  // This draws more than 500mA, but looks much nicer than a dimmer effect
  LEDRainbowEffect.brightness(150);

  StalkerEffect.variant = STALKER(BlazingTrail);

  // We want to make sure that the firmware starts with LED effects off
  // This avoids over-taxing devices that don't have a lot of power to share
  // with USB devices
  LEDOff.activate();

  ActiveModColorEffect.highlight_color = CRGB(0x00, 0xff, 0xff);
  //SHSH_USE_LAYERS();
}

void loop() {
  Kaleidoscope.loop();
}

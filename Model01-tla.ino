// -*- mode: c++ -*-
// Copyright 2018 Laszlo Attila Toth
// See "COPYING" for license details

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "built by TLA"
#endif

#define WITH_SHIFT_LAYERS 0
#define WITH_ACTIVE_MODE_LED 0

#include "Kaleidoscope.h"

#include "Kaleidoscope-OneShot.h"
#if WITH_SHIFT_LAYERS
#include "Kaleidoscope-ShapeShifter.h"
#endif
#if WITH_ACTIVE_MODE_LED
#include <Kaleidoscope-LED-ActiveModColor.h>
#endif
#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-LEDControl.h"

// Support for "Numpad" mode, which is mostly just the Numpad specific LED mode
#include "Kaleidoscope-NumPad.h"

#include "LED-Off.h"
#include "Kaleidoscope-LEDEffect-SolidColor.h"

// Support for the "Boot greeting" effect, which pulses the 'LED' button for 10s
// when the keyboard is connected to a computer (or that computer is powered on)
#include "Kaleidoscope-LEDEffect-BootGreeting.h"

// Support for host power management (suspend & wakeup)
#include "Kaleidoscope-HostPowerManagement.h"

// MACRO NAMES
enum { MACRO_VERSION_INFO,
       MACRO_ANY,
       MACRO_CTRL_ALT_LCLICK,
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
#define Key_BSl       Key_Backslash

#define Consumer_VolDec        Consumer_VolumeDecrement
#define Consumer_VolInc        Consumer_VolumeIncrement

// accents
// ..A: acute
// ..U: umlaut
// ..DA: double acute
// KH..: Hungarian Key
#define KH_AA  RALT(Key_A)
#define KH_EA  RALT(Key_S)
#define KH_IA  RALT(Key_Q)
#define KH_OA  RALT(Key_L)
#define KH_UA  RALT(Key_H)
#define KH_OU  RALT(Key_T)
#define KH_UU  RALT(Key_Quote)
#define KH_ODA RALT(Key_J)
#define KH_UDA RALT(Key_F)

// Specific keys for my apps
#define LAUNCHPAD     LALT(LCTRL(LGUI(LSHIFT(Key_M))))
#define PSB           M(MACRO_CTRL_ALT_LCLICK)
#define INS_NOFMT     LALT(LGUI(LSHIFT(Key_V)))

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
enum {
  MINE,
#if WITH_SHIFT_LAYERS
  MINE_SHIFT,
#endif
  DVORAK,
  NUMPAD,
  FUNCTION,
  NAV,
  AUX
};

/* This comment temporarily turns off astyle's indent enforcement
     so we can make the keymaps actually resemble the physical key layout better
*/
// *INDENT-OFF*

KEYMAPS(

  [MINE] = KEYMAP_STACKED
  (___,          Key_1, Key_2, Key_3, Key_4, Key_5, Key_6,
   Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,
   Key_LeftControl, Key_Backspace, OSM(LeftGui), Key_LeftShift,
   OSL(NAV),

   LockLayer(DVORAK), Key_6, Key_7, Key_8,     Key_9,      Key_0,         LockLayer(NUMPAD),
   Key_Enter,         Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,
                      Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
   OSL(FUNCTION),     Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,
   OSM(RightAlt), OSM(RightShift), Key_Space, Key_Backspace,
   OSL(AUX)
   ),
#if WITH_SHIFT_LAYERS
  [MINE_SHIFT] = KEYMAP_STACKED
  (___, Key_6, LSHIFT(RALT(Key_C)), ___, ___, ___, ___,
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
#endif

  [DVORAK] = KEYMAP_STACKED
  (___,          Key_1,         Key_2,     Key_3,      Key_4, Key_5, Key_6,
   Key_Backtick, Key_Quote,     Key_Comma, Key_Period, Key_P, Key_Y, Key_Tab,
   Key_PageUp,   Key_A,         Key_O,     Key_E,      Key_U, Key_I,
   Key_PageDown, Key_Semicolon, Key_Q,     Key_J,      Key_K, Key_X, Key_Escape,
   Key_LeftControl, Key_Backspace, OSM(LeftGui), Key_LeftShift,
   OSL(NAV),

   ___,            Key_6, Key_7, Key_8, Key_9, Key_0, LockLayer(NUMPAD),
   Key_Enter,      Key_F, Key_G, Key_C, Key_R, Key_L, Key_Slash,
                   Key_D, Key_H, Key_T, Key_N, Key_S, Key_Minus,
   OSL(FUNCTION),  Key_B, Key_M, Key_W, Key_V, Key_Z, Key_Equals,
   OSM(RightAlt), OSM(RightShift), Key_Space, Key_Backspace,
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
   Key_RightControl, OSM(LeftAlt), Key_Enter, ___,
   ___),

  [NAV] =  KEYMAP_STACKED
  (___,  Key_F1, Key_F2, Key_F3, Key_F4, Key_F5, Key_LEDEffectNext,
   XXX,  XXX,    MW(NW), MM(Up), MW(NE), MS(Up), MW(End),
   PSB,  MS(R),  MM(L),  MM(Dn), MM(R),  MS(Dn),
   XXX,  MS(L),  MW(SW), XXX,    MW(SE), XXX,    MM(BtnL),
   ___, Key_Del, Key_Enter, ___,
   ___,

   XXX,        Key_F6,     Key_F7,     Key_F8,      Key_F9,     Key_F10, Key_F11,
   MM(BtnM),   Key_PageUp, Key_Home,   Key_UpArrow, Key_End,    XXX,     Key_F12,
               Key_PageDn, Key_LArrow, Key_DnArrow, Key_RArrow, XXX,     XXX,
   MM(BtnR),   XXX,        XXX,        XXX,         XXX,        XXX,     XXX,
   Key_RightControl, OSM(LeftAlt), Key_Enter, ___,
   ___),

  [AUX] =  KEYMAP_STACKED
  (___,                   Key_F11, Key_F12, Key_F13,  Key_F14,    Key_F15,          KC(Mute),
   KC(PlaySlashPause),    XXX,     XXX,     KH_EA,    Key_BSl,    Key_RightBracket, KC(VolDec),
   KC(ScanPreviousTrack), KH_AA,   XXX,     XXX,      XXX,        Key_LeftBracket, 
   KC(ScanNextTrack),     XXX,     XXX,     XXX,      INS_NOFMT,  XXX,              KC(VolInc),
   ___, Key_Del, Key_Enter, ___,
   ___,

   M(MACRO_ANY),        Key_F16,   Key_F17,   Key_F18,  Key_F19,  Key_F20,    XXX,
   KC(PlaySlashPause),  KH_UU,     KH_UA,     KH_IA,    KH_OA,    KH_OU,      XXX,
                        KH_UDA,    KH_AA,     KH_EA,    XXX,      KH_ODA,     XXX,
   Key_PcApplication,   XXX,       LAUNCHPAD, XXX,      XXX,      KH_OU,      XXX,
   Key_RightControl, OSM(LeftAlt), Key_Enter, ___,
   ___),

) // KEYMAPS()

/* Re-enable astyle's indent enforcement */
// *INDENT-ON*

#if WITH_SHIFT_LAYERS
SHSH_LAYERS(
  SHSH_LAYER(MINE, MINE_SHIFT),
  SHSH_LAYER(DVORAK, MINE_SHIFT)
);
#endif

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

    case MACRO_CTRL_ALT_LCLICK:
      if (keyIsPressed(keyState)) {
        return MACRO(D(LeftControl), D(LeftAlt), D(mouseBtnL) /*, U(LeftControl), U(LeftAlt)*/);
      }
  }
  return MACRO_NONE;
}

// These 'solid' color effect definitions define a rainbow of
// LED color modes calibrated to draw 500mA or less on the
// Keyboardio Model 01.
static kaleidoscope::LEDSolidColor solidColor(90, 30, 10);

/** toggleLedsOnSuspendResume toggles the LEDs off when the host goes to sleep,
   and turns them back on when it wakes up.
*/
void toggleLedsOnSuspendResume(kaleidoscope::HostPowerManagement::Event event) {
  switch (event) {
    case kaleidoscope::HostPowerManagement::Suspend:
      LEDControl.paused = true;
      LEDControl.set_all_leds_to({0, 0, 0});
      LEDControl.syncLeds();
      break;
    case kaleidoscope::HostPowerManagement::Resume:
      LEDControl.paused = false;
      LEDControl.refreshAll();
      break;
    case kaleidoscope::HostPowerManagement::Sleep:
      break;
  }
}

/** hostPowerManagementEventHandler dispatches power management events (suspend,
   resume, and sleep) to other functions that perform action based on these
   events.
*/
void hostPowerManagementEventHandler(kaleidoscope::HostPowerManagement::Event event) {
  toggleLedsOnSuspendResume(event);
}

// First, tell Kaleidoscope which plugins you want to use.
// The order can be important. For example, LED effects are
// added in the order they're listed here.
KALEIDOSCOPE_INIT_PLUGINS(
  // The boot greeting effect pulses the LED button for 10 seconds after the keyboard is first connected
  BootGreetingEffect,

  // LEDControl provides support for other LED modes
  LEDControl,
  LEDOff,
  solidColor,

  // The numpad plugin is responsible for lighting up the 'numpad' mode
  // with a custom LED effect
  NumPad,

  // The macros plugin adds support for macros
  Macros,

  OneShot,
#if WITH_ACTIVE_MODE_LED
  ActiveModColorEffect,
#endif
#if WITH_SHIFT_LAYERS
  ShapeShifter,
#endif

  // The HostPowerManagement plugin enables waking up the host from suspend,
  // and allows us to turn LEDs off when it goes to sleep.
  HostPowerManagement,

  // The MouseKeys plugin lets you add keys to your keymap which move the mouse.
  MouseKeys
)

void setup() {
  // First, call Kaleidoscope's internal setup function
  Kaleidoscope.setup();

  NumPad.numPadLayer = NUMPAD;

  // We want the keyboard to be able to wake the host up from suspend.
  HostPowerManagement.enableWakeup();

  // We want to make sure that the firmware starts with LED effects off
  // This avoids over-taxing devices that don't have a lot of power to share
  // with USB devices
  LEDOff.activate();

#if WITH_ACTIVE_MODE_LED
  ActiveModColorEffect.highlight_color = CRGB(0x00, 0xff, 0xff);
#endif
#if WITH_SHIFT_LAYERS
  SHSH_USE_LAYERS();
#endif
}

void loop() {
  Kaleidoscope.loop();
}

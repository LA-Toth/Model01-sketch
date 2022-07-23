// -*- mode: c++ -*-
// Copyright 2018-2022 Laszlo Attila Toth
// See "COPYING" for license details

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "built by TLA"
#endif

#define WITH_ACTIVE_MODE_LED 1
#define CHANGE_CTRL_AND_CMD_KEYS 0

#include "Kaleidoscope.h"

#include "Kaleidoscope-OneShot.h"
#if WITH_ACTIVE_MODE_LED
#include <Kaleidoscope-LED-ActiveModColor.h>
#endif
#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-LEDControl.h"

// Support for "Numpad" mode, which is mostly just the Numpad specific LED mode
#include "Kaleidoscope-NumPad.h"

//#include "LED-Off.h"
#include "Kaleidoscope-LEDEffect-SolidColor.h"

// Support for the "Boot greeting" effect, which pulses the 'LED' button for 10s
// when the keyboard is connected to a computer (or that computer is powered on)
#include "Kaleidoscope-LEDEffect-BootGreeting.h"

// Support for host power management (suspend & wakeup)
#include "Kaleidoscope-HostPowerManagement.h"

// Addditional constants
#include "constants.h"

// MACRO NAMES
enum { MACRO_VERSION_INFO,
       MACRO_ANY,
       MACRO_CTRL_ALT_LCLICK,
       MACRO_SWITCH_ZY,
       MACRO_PRESS_Z,
       MACRO_PRESS_Y,
       MACRO_CONST_01,
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

//
// FN layers and the function keys
//
// map the two fn keys to specific layer - it might be changed here
#define LFN           NAV
#define RFN           AUX

// function keys
#define KFX(x)        Key_F ## x
#define Key_F110      Key_F20
// LFN must belong to F1..F10 and RFN to F11..F20
// KFX0 is for 1..10 and KFX1 is for offset by 10, 11..20
#define KFX0(x)       KFX(x)
#define KFX1(x)       KFX(1 ## x)
// AUX layer's Fx keys are KAFX
// NAV layer's Fx keys are KNFX
#define KAFX(x)       KFX1(x)
#define KNFX(x)       KFX0(x)


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
#define KPcApp        Key_PcApplication
#define Key_RBracket  Key_RightBracket
#define Key_RBracket  Key_RightBracket
#define Key_Led       Key_LEDEffectNext
#define KM_Z          M(MACRO_PRESS_Z)
#define KM_Y          M(MACRO_PRESS_Y)

#define Consumer_VolDec        Consumer_VolumeDecrement
#define Consumer_VolInc        Consumer_VolumeIncrement

#define Key_BUTTERFLY     Key_BSl

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
// Windows
#define KH_AAW  Key_Quote
#define KH_EAW  Key_Semicolon
#define KH_IAW  Key_NonUsBackslashAndPipe
#define KH_OAW  Key_Equals
#define KH_UAW  Key_RightBracket
#define KH_OUW  Key_0
#define KH_UUW  Key_Minus
#define KH_ODAW Key_LeftBracket
#define KH_UDAW Key_BSl

// Specific keys for my apps
#define LAUNCHPAD     LALT(LCTRL(LGUI(LSHIFT(Key_M))))
#define PSB           M(MACRO_CTRL_ALT_LCLICK)
#define INS_NOFMT     LALT(LGUI(LSHIFT(Key_V)))
#define LOCK          LCTRL(LGUI(Key_Q))
#define ASL           LALT(LSHIFT(Key_L))
#define CCF9          LCTRL(LGUI(Key_F9))

// Windows
#define WPD           LCTRL(LGUI(Key_LArrow))
#define WND           LCTRL(LGUI(Key_RArrow))

#if CHANGE_CTRL_AND_CMD_KEYS
#define Key_CLeftGui    Key_LeftControl
#define Key_CLeftCtrl   Key_LeftGui
#else
#define Key_CLeftGui    Key_LeftGui
#define Key_CLeftCtrl   Key_LeftControl
#endif

// The CTRL to SHIFT and SHIFT to CTRL
#define LEFT_4_KEYS     Key_CLeftCtrl, Key_LeftAlt , Key_CLeftGui, Key_LeftShift
#define RIGHT_4_KEYS    Key_RightAlt, OSM(RightShift), Key_Space, Key_Backspace


//#define SW_OTHER        LockLayer(OTHER)
#define SW_OTHER        OSL(OTHER)

//
// End of shortening and mapping
//

/** The Model 01's key layouts are defined as 'keymaps'.
       https://github.com/keyboardio/Kaleidoscope/blob/master/src/key_defs_keyboard.h
       https://github.com/keyboardio/Kaleidoscope/blob/master/src/key_defs_consumerctl.h
       https://github.com/keyboardio/Kaleidoscope/blob/master/src/key_defs_sysctl.h
       https://github.com/keyboardio/Kaleidoscope/blob/master/src/key_defs_keymaps.h

*/


// switch Key_Y and Key_Z
bool switch_z_y = true;

// layers
enum {
  MINE,
  OTHER,
  NUMPAD,
  NAV,
  AUX, // close to original Hungarian layout
  RSIDE, // One side only to work with other input devices
  AUXFROMOTHER, // The AUX with Windows-compatible accented chars opened from OTHER layer.
};

/* This comment temporarily turns off astyle's indent enforcement
     so we can make the keymaps actually resemble the physical key layout better
*/
// *INDENT-OFF*

KEYMAPS(

  [MINE] = KEYMAP_STACKED
  (SW_OTHER,     Key_1, Key_2, Key_3, Key_4, Key_5, Key_LeftBracket,
   Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_PageDown, Key_Y, Key_X, Key_C, Key_V, Key_B, Key_Escape,
   LEFT_4_KEYS,
   OSL(LFN),

   Key_RightBracket,  Key_6, Key_7, Key_8,     Key_9,      Key_0,         LockLayer(NUMPAD),
   Key_Enter,         Key_Z, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,
                      Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
   Key_BUTTERFLY,     Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,
   RIGHT_4_KEYS,
   OSL(RFN)
   ),

#ifdef USE_DVORAK
  [OTHER] = KEYMAP_STACKED
  (___,          Key_1,         Key_2,     Key_3,      Key_4, Key_5, Key_LeftBracket,
   Key_Backtick, Key_Quote,     Key_Comma, Key_Period, Key_P, Key_Y, Key_Tab,
   Key_PageUp,   Key_A,         Key_O,     Key_E,      Key_U, Key_I,
   Key_PageDown, Key_Semicolon, Key_Q,     Key_J,      Key_K, Key_X, Key_Escape,
   LEFT_4_KEYS,
   OSL(LFN),

   Key_RightBracket, Key_6, Key_7, Key_8, Key_9, Key_0, LockLayer(NUMPAD),
   Key_Enter,      Key_F, Key_G, Key_C, Key_R, Key_L, Key_Slash,
                   Key_D, Key_H, Key_T, Key_N, Key_S, Key_Minus,
   Key_BUTTERFLY,  Key_B, Key_M, Key_W, Key_V, Key_Z, Key_Equals,
   RIGHT_4_KEYS,
   OSL(RFN)
   ),
#else
  [OTHER] = KEYMAP_STACKED
  (___,          Key_1, Key_2, Key_3, Key_4, Key_5, Key_LeftBracket,
   Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,
   LEFT_4_KEYS,
   OSL(LFN),

   Key_RightBracket,  Key_6, Key_7, Key_8,     Key_9,      Key_0,         LockLayer(NUMPAD),
   Key_Enter,         Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,
                      Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
   Key_BUTTERFLY,     Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,
   RIGHT_4_KEYS,
   OSL(AUXFROMOTHER)
   ),
#endif

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

  [NAV] =  KEYMAP_STACKED
  (___,  KNFX(1),  KNFX(2),  KNFX(3),  KNFX(4),  KNFX(5),  M(MACRO_SWITCH_ZY),
   WPD,  WND,      MW(NW),   MM(Up),   MW(NE),   MS(Up),   LALT(Key_Tab),
   ___,  MS(R),    MM(L),    MM(Dn),   MM(R),    MS(Dn),
   XXX,  MS(L),    MW(SW),   XXX,      MW(SE),   MW(End),      MM(BtnL),
   ___, Key_Del, Key_Enter, ___,
   ___,

   Key_Led,    KNFX(6),     KNFX(7),     KNFX(8),      KNFX(9),     KNFX(10),  Key_F11,
   MM(BtnM),   Key_PageUp,  Key_Home,    Key_UpArrow,  Key_End,     XXX,       Key_F12,
               Key_PageDn,  Key_LArrow,  Key_DnArrow,  Key_RArrow,  XXX,       XXX,
   MM(BtnR),   XXX,         XXX,         XXX,          XXX,         XXX,       M(MACRO_CONST_01),
   ___, OSM(LeftAlt), ___, Key_Del,
   ___),

   [AUX] =  KEYMAP_STACKED
  (___,                   KAFX(1), KAFX(2), KAFX(3),  KAFX(4),    KAFX(5),          KC(Mute),
   KC(PlaySlashPause),    XXX,     KH_UDA,  KH_EA,    Key_BSl,    Key_RightBracket, KC(VolDec),
   KC(ScanPreviousTrack), KH_AA,   KH_UU,   KH_ODA,   KH_OU,      Key_LeftBracket,
   KC(ScanNextTrack),     XXX,     Key_PrtScr, XXX,   INS_NOFMT,  XXX,              KC(VolInc),
   ___, Key_Del, Key_Enter, ___,
   ___,

   M(MACRO_ANY),        KAFX(6),   KAFX(7),   KAFX(8),  KAFX(9),  KH_OU,       KH_UU,
   KC(PlaySlashPause),  KH_UDA,    KH_UA,     KH_IA,    KH_OA,    KH_UDA,      KH_ODA,
                        KH_UDA,    XXX,       XXX,      KH_EA,    KH_EA,       KH_AA,
   Key_PcApplication,   ASL,       LAUNCHPAD, XXX,      CCF9,     KH_UDA,      LOCK,
   ___, OSM(LeftAlt), ___, Key_Del,
   ___),

  [RSIDE] = KEYMAP_STACKED
  (___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___,
   ___,

   ___, ___, ___, ___, ___, ___, ___,
   Key_Enter, ___,    Key_E, ___, ___, ___, ___,
               Key_L, Key_I, Key_Y, ___, ___, ___,
   Key_Space,  Key_W, Key_B, Key_S, Key_M, ___, ___,
   Key_RightGui, Key_RightShift, Key_RightAlt, Key_RightControl,
   ___),

  // Same as AUX except that it works on Windows
  [AUXFROMOTHER] =  KEYMAP_STACKED
  (___,                   KAFX(1), KAFX(2), KAFX(3),  KAFX(4),    KAFX(5),          KC(Mute),
   KC(PlaySlashPause),    XXX,     KH_UDAW, KH_EAW,   Key_BSl,    Key_RightBracket, KC(VolDec),
   KC(ScanPreviousTrack), KH_AAW,  KH_UUW,  KH_ODAW,  KH_OUW,     Key_LeftBracket,
   KC(ScanNextTrack),     XXX,     Key_PrtScr, XXX,   INS_NOFMT,  XXX,              KC(VolInc),
   ___, Key_Del, Key_Enter, ___,
   ___,

   M(MACRO_ANY),        KAFX(6),   KAFX(7),   KAFX(8),  KAFX(9),  KH_OUW,       KH_UUW,
   KC(PlaySlashPause),  KH_UDAW,   KH_UAW,    KH_IAW,   KH_OAW,   KH_UDAW,      KH_ODAW,
                        KH_UDAW,   XXX,       XXX,      KH_EAW,   KH_EAW,       KH_AAW,
   Key_PcApplication,   ASL,       LAUNCHPAD, XXX,      CCF9,     KH_UDAW,      LOCK,
   ___, OSM(LeftAlt), ___, Key_Del,
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
  bool toggledOn = false;
  if (keyToggledOn(keyState)) {
    lastKey.setKeyCode(Key_A.getKeyCode() + (uint8_t)(millis() % 36));
    toggledOn = true;
  }

  if (keyIsPressed(keyState))
    Kaleidoscope.hid().keyboard().pressKey(lastKey, toggledOn);
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
      break;

     case MACRO_SWITCH_ZY:
        switch_z_y = !switch_z_y;
        break;

     case MACRO_PRESS_Z:
        Kaleidoscope.hid().keyboard().pressKey(switch_z_y ? Key_Y : Key_Z);
        break;

     case MACRO_PRESS_Y:
        Kaleidoscope.hid().keyboard().pressKey(switch_z_y ? Key_Z : Key_Y);
        break;

     case MACRO_CONST_01:
      if (keyToggledOn(keyState)) {
        Macros.type(PSTR(MTLA_C_01));
      }
  }
  return MACRO_NONE;
}

// These 'solid' color effect definitions define a rainbow of
// LED color modes calibrated to draw 500mA or less on the
// Keyboardio Model 01.
static kaleidoscope::plugin::LEDSolidColor solidColor(90, 30, 10);

/** toggleLedsOnSuspendResume toggles the LEDs off when the host goes to sleep,
   and turns them back on when it wakes up.
*/
void toggleLedsOnSuspendResume(kaleidoscope::HostPowerManagement::Event event) {
  switch (event) {
    case kaleidoscope::plugin::HostPowerManagement::Suspend:
      LEDControl.disable();
      break;
    case kaleidoscope::plugin::HostPowerManagement::Resume:
      LEDControl.enable();
      break;
    case kaleidoscope::plugin::HostPowerManagement::Sleep:
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

  OneShot,

  // The macros plugin adds support for macros
  Macros,

#if WITH_ACTIVE_MODE_LED
  ActiveModColorEffect,
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

#include QMK_KEYBOARD_H
#include "print.h"

#define COMBO_ONLY_FROM_LAYER _DVORAK

#define CONSOLE_ENBLE
#define SHIFT_HELD_MAX 1000

const key_override_t caret_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_AT, KC_CIRC);
const key_override_t grave_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_HASH, KC_GRV);
const key_override_t percent_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_AMPR, KC_PERC);
const key_override_t tilde_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_DLR, KC_TILD);
const key_override_t zero_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_ASTR, KC_0);
const key_override_t one_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_LPRN, KC_1);
const key_override_t two_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_RPRN, KC_2);
const key_override_t three_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_RCBR, KC_3);
const key_override_t four_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_PLUS, KC_4);
const key_override_t five_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_LCBR, KC_5);
const key_override_t six_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_RBRC, KC_6);
const key_override_t seven_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_LBRC, KC_7);
const key_override_t eigth_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_EXLM, KC_8);
const key_override_t nine_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_EQL, KC_9);

// This globally defines all key overrides to be used
const key_override_t **key_overrides = (const key_override_t *[]){
  &caret_key_override,
  &grave_key_override,
  &percent_key_override,
  &tilde_key_override,
  &zero_key_override,
  &one_key_override,
  &two_key_override,
  &three_key_override,
  &four_key_override,
  &five_key_override,
  &six_key_override,
  &seven_key_override,
  &eigth_key_override,
  &nine_key_override,
  NULL // Null terminate the array of overrides!
};

bool is_shift_active = false; // ADD this near the begining of keymap.c
bool is_alt_active = false;
bool is_ctrl_active = false;

bool is_emacs_edit = true;     // toggle between GNU editing keys and standard keys
  
uint16_t shift_held_timer = 0;
uint16_t alt_held_timer = 0;
uint16_t ctrl_held_timer = 0;

uint8_t mod_state;
uint8_t def_layer = 0;

enum layer_names {
  _DVORAK = 0,
  _QWERTY,
  _EEDIT,
  _EFUNC,
  _KEYPAD,
  _ADJUST
};

enum custom_keycodes {
  DVORAK = SAFE_RANGE,
  QWERTY,
  EEDIT,
  EFUNC,
  KEYPAD,
  ADJUST,
  /* General KEYS */
  GK_DFTL,
  /* EMACSm custom keys */
  EM_MODE,
  EM_UNDO,
  EM_CUT,
  EM_COPY,
  EM_PASTE,
  EM_SELBUF,
  EM_MARK,
  EM_OLINE,
  EM_DLINE,
  EM_MACRO,
  EM_FORMAT,
  EM_COMMENT,
  EM_CMD,
  EM_SHL,
  EM_XCMD,
  EM_GOTO,
  EM_BOF,   // Begining of buffer
  EM_EOF,   // End of buffer
  EM_SAVE,  // Save / write Buffer
  EM_OPEN,  // Open buffer
  EM_KBUF,  // Kill buffer
  EM_RBUF,  // Revert buffer
  EM_NBUF,  // reName buffer
  EM_FIND,  // Find / Replace
  EM_SWIN,  // Split window (Vertical/Horizontal)
  EM_MWIN,  // Merge windowo
  EM_OWIN,  // Other window
  EM_AWIN,  // Adjust window sizex
  EM_PRVBUF, // Previous buffer
  EM_NXTBUF, // Next buffer
  EM_POPN,  // Projectile open
  EM_PDIR,  // Projectile dired
  EM_PFIL,  // Projectile find file
  EM_PIBF,  // Projectile ibuffer
  EM_PRUN,  // Projectile run
  EM_PCPL,  // Prejectile compile
  EM_PSHL, // Projectile shell
  EM_PRPL, // Prejectile replace
  EM_PNXT, // Projectile next buffer
  EM_PPRV, // Projectile prev buffer
  EM_WOPN, // Perspective Open
  EM_WSAV, // Perspective Save
  EM_WNXT, // Perspective Next
  EM_WPRV, // Perspective Prev
  EM_ZTXT, // Zoom Text
  EM_FDEL, // Delete word forward
  EM_BDEL,  // Delele word backword
  EM_SPRV, // SEXP Prev
  EM_SNXT, // SEXP Next
  EM_EPRV, // Error Prev
  EM_ENXT, // Errer Next
  EM_MAGIT
};

enum combo_events {
  SET_EDIT_MODE,
  COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH; // remove the COMBO_COUNT define and use this instead!

const uint16_t PROGMEM set_edit_mode[] = {KC_X, KC_M, COMBO_END};

combo_t key_combos[] = {
  [SET_EDIT_MODE] = COMBO_ACTION(set_edit_mode),
};
/* COMBO_ACTION(x) is same as COMBO(x, KC_NO) */

// Aliases to make the keymap more uniform
#define GUI_HOME GUI_T(KC_HOME)
#define GUI_PGUP GUI_T(KC_PGUP)
#define KPD_END LT(_KEYPAD, KC_END)
#define KPD_PGDN LT(_KEYPAD, KC_PGDN)
#define FUNC_BSPC LT(_EFUNC, KC_BSPC)
#define FUNC_SPC LT(_EFUNC, KC_SPC)
#define EDIT_CAPS LT(_EEDIT, KC_CAPS)
#define TG_MODE TG(_EEDIT)

#define MACLOCK LGUI(LCTL(KC_Q))


// clang-format off

/*

  Function Keys on All Layers (Keypad toggles):
  ,-----------------------------------------------------------------.
  |  ESC |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   F7   |  F8   |
  `-----------------------------------------------------------------'
  ,-----------------------------------------------------------------.
  |  F9  |  F10 |  F11 |  F12 | PScr | SLck | Paus | Keypad | RESET |
  `-----------------------------------------------------------------'

  Dvorak layer:
  ,-------------------------------------------.,-------------------------------------------.
  |   =    |   1  |   2  |   3  |   4  |   5  ||   6  |   7  |   8  |   9  |   0  |   \    |
  |--------+------+------+------+------+------||------+------+------+------+------+--------|
  | Tab    |   '  |   ,  |   .  |   P  |   Y  ||   F  |   G  |   C  |   R  |   L  |   /    |
  |--------+------+------+------+------+------||------+------+------+------+------+--------|
  | CapsLk |   A  |   O  |   E  |   U  |   I  ||   D  |   H  |   T  |   N  |   S  |   -    |
  |--------+------+------+------+------+------||------+------+------+------+------+--------|
  | LShift |   Z  |   X  |   C  |   V  |   X  ||   B  |   M  |   W  |   V  |   Z  | RShift |
  `--------+------+------+------+------+------'`------+------+------+------+------+--------'
  |   `  |  INS | Left | Rght |              |  Up  |  Dn  |   [  |   ]  |
  `---------------------------'              `---------------------------'
  ,--------------.,--------------.
  | LCtl  | LAlt || RGUI | RCtl  |
  ,------|-------|------||------+-------+-------.
  |      |       | Home || PgUp | Enter |       |
  | BkSp |  Del  |------||------|   /   | Space |
  |      |       | End  || PgDn | KeyPd |       |
  `---------------------'`----------------------'

  QWERTY layer:
  ,-------------------------------------------.,-------------------------------------------.
  |   =    |   1  |   2  |   3  |   4  |   5  ||   6  |   7  |   8  |   9  |   0  |   -    |
  |--------+------+------+------+------+------||------+------+------+------+------+--------|
  | Tab    |   Q  |   W  |   E  |   R  |   T  ||   Y  |   U  |   I  |   O  |   P  |   \    |
  |--------+------+------+------+------+------||------+------+------+------+------+--------|
  | CapsLk |   A  |   S  |   D  |   F  |   G  ||   H  |   J  |   K  |   L  |   ;  |   '    |
  |--------+------+------+------+------+------||------+------+------+------+------+--------|
  | LShift |   Z  |   X  |   C  |   V  |   B  ||   N  |   M  |   ,  |   .  |   /  | RShift |\\`--------+------+------+------+------+------'`------+------+------+------+------+--------'
  |   `  |  INS | Left | Rght |              |  Up  |  Dn  |   [  |   ]  |
  `---------------------------'              `---------------------------'
  ,--------------.,--------------.
  | LCtl  | LAlt || RGUI | RCtl  |
  ,------|-------|------||------+-------+-------.
  |      |       | Home || PgUp | Enter |       |
  | BkSp |  Del  |------||------|   /   | Space |
  |      |       | End  || PgDn | KeyPd |       |
  `---------------------'`----------------------'

  Colemak layer:
  ,-------------------------------------------.,-------------------------------------------.
  |   =    |   1  |   2  |   3  |   4  |   5  ||   6  |   7  |   8  |   9  |   0  |   -    |
  |--------+------+------+------+------+------||------+------+------+------+------+--------|
  | Tab    |   Q  |   W  |   F  |   P  |   G  ||   J  |   L  |   U  |   Y  |   ;  |   \    |
  |--------+------+------+------+------+------||------+------+------+------+------+--------|
  | BkSpc  |   A  |   R  |   S  |   T  |   D  ||   H  |   N  |   E  |   I  |   O  |   '    |
  |--------+------+------+------+------+------||------+------+------+------+------+--------|
  | LShift |   Z  |   X  |   C  |   V  |   B  ||   K  |   M  |   ,  |   .  |   /  | RShift |
  `--------+------+------+------+------+------'`------+------+------+------+------+--------'
  |   `  |  INS | Left | Rght |              |  Up  |  Dn  |   [  |   ]  |
  `---------------------------'              `---------------------------'
  ,--------------.,--------------.
  | LCtl  | LAlt || RGUI | RCtl  |
  ,------|-------|------||------+-------+-------.
  |      |       | Home || PgUp | Enter |       |
  | BkSp |  Del  |------||------|   /   | Space |
  |      |       | End  || PgDn | KeyPd |       |
  `---------------------'`----------------------'

  Keypad layer:
  ,-------------------------------------------.,-------------------------------------------.
  | Power  |      |      |      |      |      ||      | NmLk | KP = | KP / | KP * |        |
  |--------+------+------+------+------+------||------+------+------+------+------+--------|
  | Sleep  |      |      |      |      |      ||      | KP 7 | KP 8 | KP 9 | KP - |        |
  |--------+------+------+------+------+------||------+------+------+------+------+--------|
  | Wake   |      | Mute | Vol- | Vol+ |      ||      | KP 4 | KP 5 | KP 6 | KP + |        |
  |--------+------+------+------+------+------||------+------+------+------+------+--------|
  |        | Stop | Prev | Play | Next | Sel  ||      | KP 1 | KP 2 | KP 3 |KP Ent|        |
  `--------+------+------+------+------+------'`------+------+------+------+------+--------'
  |      |QWERTY|Colemk|Dvorak|              |      |      | KP . |KP Ent|
  `---------------------------'              `---------------------------'
  ,-------------.,-------------.
  |      |      ||      |MacLck|
  ,------|------|------||------+------+------.
  |      |      |      ||      |      |      |
  |      |      |------||------|      | KP 0 |
  |      |      |MacLck||      |      |      |
  `--------------------'`----x----------------'
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_DVORAK] = LAYOUT (
		      // Left Hand
		      KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,
		      KC_AMPR, KC_LBRC, KC_LCBR, KC_RCBR, KC_LPRN, KC_EQL,
		      KC_TAB,  KC_SCLN, KC_COMM, KC_DOT,  KC_P,    KC_Y,
		      TG_MODE, KC_A,    KC_O,    KC_E,    KC_U,    KC_I,
		      KC_LSFT, KC_QUOT, KC_Q,    KC_J,    KC_K,    KC_X,
		      KC_DLR,  KC_INS,  KC_LEFT, KC_RGHT,
		      // Left Thumb
		      KC_LCTL, KC_LALT,
		      GUI_HOME,
		      FUNC_BSPC, KC_DEL,  KPD_END,

		      // Right Hand
		      KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PSCR, KC_SLCK, KC_PAUS, OSL(_ADJUST),  RESET,
		      KC_ASTR, KC_RPRN, KC_PLUS, KC_RBRC, KC_EXLM, KC_HASH,
		      KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    KC_SLSH,
		      KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_MINS,
		      KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,    KC_RSFT,
		      KC_UP,   KC_DOWN, KC_BSLS, KC_AT,
		      // Right Thumb
		      KC_RALT, KC_RCTL,
		      GUI_PGUP,
		      KPD_PGDN, KC_ENTER, FUNC_SPC
		      ),


  [_QWERTY] = LAYOUT (
		      // Left Hand
		      KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,
		      KC_EQL,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,
		      KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,
		      TG_MODE, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,
		      KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,
		      KC_GRV,  KC_INS,  KC_LEFT, KC_RGHT,
		      // Left Thumb
		      KC_LCTL, KC_LALT,
		      KC_HOME,
		      KC_BSPC, KC_DEL,  KPD_END,

		      // Right Hand
		      KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PSCR, KC_SLCK, KC_PAUS, OSL(_ADJUST),  RESET,
		      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
		      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS ,
		      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
		      KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
		      KC_UP,   KC_DOWN, KC_LBRC, KC_RBRC,
		      // Right Thumb
		      KC_RGUI, KC_RCTL,
		      KC_PGUP,
		      KPD_PGDN, KC_ENTER, KC_SPC
		      ),


  [_EEDIT] = LAYOUT (
		     // Left Hand
		     KC_ESC,     KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,   KC_F6,   KC_F7,   KC_F8,
		     LCTL(KC_G), KC_1,       EM_SPRV,    EM_SNXT,    KC_4,       KC_5,
		     KC_TAB,     EM_FORMAT,  EM_BDEL,    KC_BSPC,    EM_FDEL,    EM_MARK,
		     GK_DFTL,    LALT(KC_X), EM_OLINE,   KC_DEL,     EM_DLINE,   EM_SELBUF,
		     KC_LSFT,    EM_COMMENT, EM_CUT,     EM_COPY,    EM_PASTE,   EM_UNDO,
		                 EM_KBUF,    KC_INS,     KC_LEFT,    KC_RGHT,
		     // Left Thumb
		     KC_LCTL, KC_LALT,
		     KC_HOME,
		     MO(_EFUNC), KC_DEL,  KC_END,

		     // Right Hand
		     KC_F9,   KC_F10,     KC_F11,    KC_F12,     KC_PSCR, KC_SLCK, KC_PAUS, TG(_ADJUST),  RESET,
		     EM_MWIN, EM_SWIN,    EM_ZTXT,   EM_AWIN,    EM_CMD,  EM_RBUF,
		     KC_PGUP, EM_BOF,     KC_UP,     EM_EOF,     EM_OPEN, EM_MACRO,
		     KC_PGDN, KC_LEFT,    KC_DOWN,   KC_RIGHT,   EM_SAVE, EM_NBUF,
		     EM_FIND, EM_PRVBUF,  EM_OWIN,   EM_NXTBUF,  EM_GOTO, KC_RSFT,
		              KC_UP,      KC_DOWN,   KC_LBRC,    EM_MAGIT,
		     // Right Thumb
		     KC_RGUI, KC_RCTL,
		     KC_PGUP,
		     KC_PGDN, KC_ENTER, MO(_EFUNC)
		     ),

  [_EFUNC] = LAYOUT (
		     // Left Hand
		     KC_ESC,     KC_F1,      KC_F2,    KC_F3,    KC_F4,    KC_F5,   KC_F6,   KC_F7,   KC_F8,
		     LCTL(KC_G), KC_NO,      EM_SPRV,  EM_SNXT,  KC_NO,    KC_NO,
		     KC_NO,      EM_FORMAT,  KC_NO,    KC_NO,    KC_NO,    EM_WOPN,
		     KC_CAPS,    LCTL(KC_X), EM_WPRV,  KC_NO,    EM_WNXT,  EM_WSAV,
		     _______,    EM_COMMENT, EM_CUT,   EM_COPY,  EM_PASTE, EM_UNDO,
		                 EM_KBUF,    KC_INS,  KC_LEFT,  KC_RGHT,
		     // Left Thumb
		     KC_LCTL, KC_LALT,
		     KC_HOME,
		     KC_BSPC, KC_DEL,  KC_END,

		     // Right Hand
		     KC_F9,   KC_F10,  KC_F11,   KC_F12,   KC_PSCR,  KC_SLCK, KC_PAUS, TG(_ADJUST),  RESET,
		     EM_MWIN, EM_SWIN, EM_ZTXT,  EM_AWIN,  EM_CMD,   EM_RBUF,
		     EM_PFIL, EM_PRPL, EM_PCPL,  EM_PRUN,  EM_OPEN,  EM_MACRO,
		     EM_PDIR, EM_PSHL, EM_POPN,  EM_PNXT,  EM_SAVE,  EM_NBUF,
		     EM_PIBF, EM_PPRV, EM_OWIN,  EM_PNXT,  EM_GOTO,  _______,
                     KC_UP,   KC_DOWN, KC_LBRC,  EM_MAGIT,
		     // Right Thumb
		     KC_RGUI, KC_RCTL,
		     KC_PGUP,
		     KC_PGDN, KC_ENTER, KC_SPC
		     ),

  [_KEYPAD] = LAYOUT (
		      // Left Hand
		      _______, _______, _______, _______, _______, _______, _______, _______, _______,
		      KC_PWR,  _______, _______, _______, _______, _______,
		      KC_SLEP, _______, _______, _______, _______, _______,
		      KC_WAKE, _______, KC_MUTE, KC_VOLD, KC_VOLU, _______,
		      _______, KC_MSTP, KC_MPRV, KC_MPLY, KC_MNXT, KC_MSEL,
		      _______, _______, _______, _______, 
		      // Left Thumb
		      _______, _______,
		      _______,
		      _______, _______, _______,

		      // Right Hand
		      _______, _______, _______, _______, _______, _______, _______, _______, RESET,
		      KC_CAPS, KC_NLCK, KC_PEQL, KC_PSLS, KC_PAST, _______,
		      _______, KC_P1,   KC_P2,   KC_P3,   KC_PMNS, _______,
		      _______, KC_P4,   KC_P5,   KC_P6,   KC_PPLS, _______,
		      _______, KC_P7,   KC_P8,   KC_P9,   KC_PENT, _______,
		      _______, _______, KC_PDOT, KC_PENT,
		      // Right Thumb
		      _______, _______,
		      _______,
		      _______, _______, KC_P0
		      ),

  [_ADJUST] = LAYOUT (
		      // Left Hand
		      KC_NO,  EEP_RST,   KC_NO,  KC_NO,   KC_NO,    KC_NO, KC_NO, KC_NO, KC_NO,
		      KC_NO,   KC_NO,  KC_NO,  KC_NO,   KC_NO,    KC_NO,
		      KC_NO,   KC_NO,  KC_NO,  KC_NO,   KC_NO,    KC_NO,
		      KC_NO,   DVORAK, QWERTY, EEDIT,   EFUNC,    KEYPAD,
		      _______, KC_NO,  KC_NO,  KC_NO,   KC_NO,    KC_NO,
		      KC_NO,  KC_NO,  KC_LEFT, KC_RIGHT, 
		      // Left Thumb
		      KC_NO, KC_NO,
		      KC_NO,
		      KC_NO, EEP_RST, KC_NO,

		      // Right Hand
		      KC_NO,  KC_NO,   KC_NO,   KC_NO,  KC_NO, KC_NO, KC_NO, KC_NO, RESET,
		      KC_NO,  KC_NO,   KC_NO,   KC_NO,  KC_NO, KC_NO,
		      KC_NO,  KC_NO,   KC_NO,   KC_NO,  KC_NO, KC_NO,
		      KC_NO,  KC_NO,   KC_NO,   KC_NO,  KC_NO, KC_NO,
		      KC_NO,  KC_NO,   KC_NO,   KC_NO,  KC_NO, _______,
		      KC_UP,   KC_DOWN, KC_NO,  KC_NO,
		      // Right Thumb
		      KC_NO, KC_NO,
		      KC_NO,
		      KC_NO, KC_ENTER, KC_NO
		      )

};

// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef CONSOLE_ENABLE
  uprintf("KL: kc: 0x%04X, col: %u, row: %u, pressed: %b, time: %u, interrupt: %b, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif
    
  switch (keycode) {
  case DVORAK:
    //set_single_persistent_default_layer(_DVORAK);
    layer_move(_DVORAK);
    def_layer = _DVORAK;

#ifdef CONSOLE_ENABLE
    uprintf("Layer On: DVORAK \n");
#endif
    return false;
    break;
  case QWERTY:
    //set_single_persistent_default_layer(_QWERTY);
    layer_move(_QWERTY);
    def_layer = _QWERTY;
    
#ifdef CONSOLE_ENABLE
    uprintf("Layer On: QWERTY \n");
#endif
    return false;
    break;
  case EEDIT:
    layer_move(_EEDIT);
    
#ifdef CONSOLE_ENABLE
    uprintf("Layer On: EEDIT \n");
#endif
    return false;
    break;
  case EFUNC:
    layer_move(_EFUNC);
    
#ifdef CONSOLE_ENABLE
    uprintf("Layer On: EFUNC \n");
#endif
    return false;
    break;
  case KEYPAD:
    layer_move(_KEYPAD);

#ifdef CONSOLE_ENABLE
    uprintf("Layer On: KEYPAD \n");
#endif
    
    return false;
    break;
  case ADJUST:
    layer_move(_ADJUST);

#ifdef CONSOLE_ENABLE
    uprintf("Layer On: ADJUST \n");
#endif
    
    return false;
    break;

    /*==== General ====*/
  case GK_DFTL:
#ifdef CONSOLE_ENABLE
    uprintf("Text editing mode. \n");
#endif
    // set default layer
    layer_move(def_layer);
      
    return false;
    break;
    
    /*==== MOD Keys ====*/
  case KC_LSFT:
  case KC_RSFT:
    if (record->event.pressed) {
#ifdef CONSOLE_ENABLE
      uprintf("Shift key Held!!!\n");
#endif
      is_shift_active = true;
      shift_held_timer = timer_read();
    } else{
      if (get_mods() & MOD_MASK_SHIFT) {
	// MOD is still enabled
      } else {
      }
      
#ifdef CONSOLE_ENABLE
      uprintf("Shift key Relased!!!\n");
#endif
      is_shift_active = false;
      shift_held_timer=0;
      
    }
    return true;
    break;

  case KC_LALT:
  case KC_RALT:
    if (record->event.pressed) {
#ifdef CONSOLE_ENABLE
      uprintf("Alt key Held!!!\n");
#endif
      is_alt_active = true;
      alt_held_timer = timer_read();
    } else{
      if (get_mods() & MOD_MASK_SHIFT) {
	// MOD is still enabled
      } else {
      }
      
#ifdef CONSOLE_ENABLE
      uprintf("Alt key Relased!!!\n");
#endif
      is_alt_active = false;
      alt_held_timer=0;
      
    }
    return true;
    break;

  case KC_LCTL:
  case KC_RCTL:
    if (record->event.pressed) {
#ifdef CONSOLE_ENABLE
      uprintf("Ctrl key Held!!!\n");
#endif 
      is_ctrl_active = true;
      ctrl_held_timer = timer_read();
    } else{
      if (get_mods() & MOD_MASK_SHIFT) {
	// MOD is still enabled
      } else {
      }
      
#ifdef CONSOLE_ENABLE
      uprintf("Ctrl key Relased!!!\n");
#endif
      is_ctrl_active = false;
      ctrl_held_timer=0;
      
    }
    return true;
    break;

    /*==== EMACS stuff ====*/
  case EM_UNDO:
    if (record->event.pressed) {
      mod_state = get_mods();
      if (mod_state & MOD_MASK_SHIFT) {
	//Redo
	if(is_emacs_edit){
	  // Ctrl+X u
	  send_string( SS_LCTL("x") "z");
	} else {
	  // Ctrl+Z
	  //send_string( SS_LALT("z") );
	}
      } else {
	//send UNDO shortcut keys
	if(is_emacs_edit){
	  // Ctrl+X u
	  send_string( SS_LCTL("x") "u");
	} else {
	  // Ctrl+Z
	  send_string( SS_LALT("z") );
	}
      }
    } else{
    }
    return true;
    break;
  case EM_CUT:
    if (record->event.pressed) {
      mod_state = get_mods();
      if (mod_state & MOD_MASK_SHIFT) {
	//decrease text size
      } else {
	//send UNDO shortcut keys
	if(is_emacs_edit){
	  // Ctrl+X u
	  send_string( SS_LCTL("w"));
	} else {
	  // Ctrl+Z
	  send_string( SS_LCTL("x") );
	}
      }
    } else{
    }
    return true;
    break;
  case EM_COPY:
    if (record->event.pressed) {
      mod_state = get_mods();
      if (mod_state & MOD_MASK_SHIFT) {
	//decrease text size
      } else {
	//send UNDO shortcut keys
	if(is_emacs_edit){
	  // Ctrl+X u
	  send_string( SS_LALT("w"));
	} else {
	  // Ctrl+Z
	  send_string( SS_LCTL("c") );
	}
      }
    } else{
    }
    return true;
    break;
  case EM_PASTE:
    if (record->event.pressed) {
      mod_state = get_mods();
      if (mod_state & MOD_MASK_SHIFT) {
	//decrease text size
      } else {
	//send UNDO shortcut keys
	if(is_emacs_edit){
	  // Ctrl+X u
	  send_string( SS_LCTL("y"));
	} else {
	  // Ctrl+Z
	  send_string( SS_LCTL("v") );
	}
      }
    } else{
    }
    return true;
    break;
  case EM_SELBUF:
    if (record->event.pressed) {
      mod_state = get_mods();
      if (mod_state & MOD_MASK_SHIFT) {
	//mark sexp
	if(is_emacs_edit){
	  // Ctrl+X u
	  send_string( SS_LCTL(SS_LALT("@")) );
	} else {
	  // Ctrl+Z
	  //send_string( SS_LALT("z") );
	}
	
      } else {
	//send UNDO shortcut keys
	if(is_emacs_edit){
	  // Ctrl+X u
	  send_string( SS_LCTL("x") "h");
	} else {
	  // Ctrl+Z
	  send_string( SS_LCTL("a") );
	}
      }
    } else{
    }
    return true;
    break;
  case EM_BOF:
    if (record->event.pressed) {
      mod_state = get_mods();
      if (mod_state & MOD_MASK_SHIFT) {
	// Begining of buffer
	if(get_mods() & MOD_MASK_SHIFT) del_mods(MOD_MASK_SHIFT);
	if(is_emacs_edit){
	  // Alt+<
	  send_string( SS_LALT("<") );
	} else {
	  // Ctrl+Home
	  send_string( SS_LCTL(SS_TAP(X_HOME)) );
	}
	
      } else {
	// Begining of line
	if(is_emacs_edit){
	  // Ctrl+a
	  send_string( SS_LCTL("a") );
	} else {
	  // Home
	  send_string( SS_TAP(X_HOME) );
	}
      }
    } else{
    }
    return true;
    break;
  case EM_EOF:
    if (record->event.pressed) {
      mod_state = get_mods();
      if (mod_state & MOD_MASK_SHIFT) {
	//End of buffer
	if(get_mods() & MOD_MASK_SHIFT) del_mods(MOD_MASK_SHIFT);
	if(is_emacs_edit){
	  // Alt+>
	  send_string( SS_LALT(">"));
	} else {
	  // Ctrl+End
	  send_string( SS_LCTL( SS_TAP(X_END)) );
	}	
      } else {
	//End of line 
	if(is_emacs_edit){
	  // Ctrl+e
	  send_string( SS_LCTL("e"));
	} else {
	  // End
	  send_string( SS_LCTL(SS_TAP(X_END)) );
	}	
      }
    } else{
    }
    return true;
    break; 
  case EM_KBUF:
    if (record->event.pressed) {
      mod_state = get_mods();
      if (mod_state & MOD_MASK_SHIFT) {
	//write
	if(is_emacs_edit){
	  // Ctrl+x Ctrl+w
	  // send_string( SS_LCTL("x") SS_LCTL("w"));
	} else {
	  // Ctrl+s
	  //send_string( SS_LCTL("a"));
	}	
      } else {
	//Kill Buffer
	if(is_emacs_edit){
	  // Ctrl+x Ctrl+s
	  send_string( SS_LCTL("x") "k" );
	} else {
	  // End
	  //send_string( SS_LCTL("s"));
	}	
      }
    } else{
    }
    return true;
    break; 
  case EM_RBUF:
    if (record->event.pressed) {
      mod_state = get_mods();
      if (mod_state & MOD_MASK_SHIFT) {
	//write
	if(is_emacs_edit){
	  // Ctrl+x Ctrl+w
	  // send_string( SS_LCTL("x") SS_LCTL("w"));
	} else {
	  // Ctrl+s
	  //send_string( SS_LCTL("a"));
	}	
      } else {
	//Revert Buffer
	if(is_emacs_edit){
	  // Ctrl+x Ctrl+s
	  send_string( SS_LCTL("x") "x" "g");
	} else {
	  // End
	  //send_string( SS_LCTL("s"));
	}	
      }
    } else{
    }
    return true;
    break; 
  case EM_NBUF:
    if (record->event.pressed) {
      mod_state = get_mods();
      if (mod_state & MOD_MASK_SHIFT) {
	//
	if(is_emacs_edit){
	  // Ctrl+x Ctrl+w
	  //send_string( SS_LCTL("x") SS_LCTL("w"));
	} else {
	  // Ctrl+s
	  //send_string( SS_LCTL("a"));
	}	
      } else {
	//Rename buffer
	if(is_emacs_edit){
	  // Ctrl+x Ctrl+s
	  send_string( SS_LCTL("x") "x" "r");
	} else {
	  // End
	  //send_string( SS_LCTL("s"));
	}	
      }
    } else{
    }
    return true;
    break; 
  case EM_SAVE:
    if (record->event.pressed) {
      mod_state = get_mods();
      if (mod_state & MOD_MASK_SHIFT) {
	//write
	if(is_emacs_edit){
	  // Ctrl+x Ctrl+w
	  send_string( SS_LCTL("x") SS_LCTL("w"));
	} else {
	  // Ctrl+s
	  send_string( SS_LCTL("a"));
	}	
      } else {
	//save
	if(is_emacs_edit){
	  // Ctrl+x Ctrl+s
	  send_string( SS_LCTL("x") SS_LCTL("s"));
	} else {
	  // End
	  send_string( SS_LCTL("s"));
	}	
      }
    } else{
    }
    return true;
    break; 
  case EM_OPEN:
    if (record->event.pressed) {
      //
      mod_state = get_mods();
      if (mod_state & MOD_MASK_SHIFT) {
	//New file
	if(is_emacs_edit){
	  // Ctrl+x Ctrl+face
	  send_string( SS_LCTL("x") SS_LCTL("f"));
	} else {
	  // Ctrl+n
	  send_string( SS_LCTL("n"));
	}	
      } else {
	// Open File
	if(is_emacs_edit){
	  // Ctrl+x Ctrl+f
	  send_string( SS_LCTL("x") SS_LCTL("f"));
	} else {
	  // End
	  send_string( SS_LCTL("o"));
	}	
      }
    } else{
    }
    return true;
    break; 
  case EM_FIND:
    if (record->event.pressed) {
      mod_state = get_mods();
      if (mod_state & MOD_MASK_SHIFT) {
	//Query Replace
	if(is_emacs_edit){
	  // 
	  send_string( SS_LALT("%") );
	} else {
	  // Ctrl+r
	  send_string( SS_LCTL("r"));
	}	

      } else {
	//i-search
       	if(is_emacs_edit){
	  // Ctrl+x Ctrl+f
	  send_string( SS_LCTL("s") );
	} else {
	  // End
	  send_string( SS_LCTL("f"));
	}	
      }
    } else{
    }
    return true;
    break; 
  case EM_CMD:
    if (record->event.pressed) {
      //delete other windows
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_LALT( "!")  );
      } else {
	// 
	//send_string( SS_LCTL("f"));
      }	
      
    } else{
    }
    return true;
    break; 
  case EM_XCMD:
    if (record->event.pressed) {
      //delete other windows
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_LALT( "x")  );
      } else {
	// 
	//send_string( SS_LCTL("f"));
      }	
      
    } else{
    }
    return true;
    break; 
  case EM_SWIN:
    if (record->event.pressed) {
      mod_state = get_mods();
      if (mod_state & MOD_MASK_SHIFT) {
	//split Vertical
       	if(is_emacs_edit){
	  // Ctrl+x Ctrl+f
	  send_string( SS_LCTL( "x") "3"  );
	} else {
	  // 
	  //send_string( SS_LCTL("f"));
	}		
      } else {
	//split Horizontal
       	if(is_emacs_edit){
	  // Ctrl+x Ctrl+f
	  send_string( SS_LCTL("x") "2" );
	} else {
	  // 
	  //send_string( SS_LCTL("f"));
	}	

      }
    } else{
    }
    return true;
    break; 
  case EM_MWIN:
    if (record->event.pressed) {
      //delete other windows
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_LCTL( "x") "1" );
      } else {
	// 
	//send_string( SS_LCTL("f"));
      }	
      
    } else{
    }
    return true;
    break; 
  case EM_AWIN:
    if (record->event.pressed) {
      //mod_state = get_mods();
      if (is_shift_active) {
	if(get_mods() & MOD_MASK_SHIFT) del_mods(MOD_MASK_SHIFT);

	// increasd read timer
	shift_held_timer = timer_read(); 
 	//decrease text size 
	//send_string( SS_LALT("x") "text-scale-decrease" SS_TAP(X_ENTER) );
	send_string( SS_LCTL("x") "{");
      } else {
	//increse window size
	send_string( SS_LCTL("x") "^");
      }
    } else{
      //del_mods(MOD_MASK_SHIFT);
    }
    return true;
    break; 
  case EM_OWIN:
    if (record->event.pressed) {
      //Other Window
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_LCTL( "x") "o" );
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
      
    } else{
    }
    return true;
    break; 
  case EM_NXTBUF:
    if (record->event.pressed) {
      //Perspective Next
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_LCTL("x") SS_TAP(X_RGHT));
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	      
    } else{
    }
    return true;
    break; 
  case EM_PRVBUF:
    if (record->event.pressed) {
      //Perspective Prev
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_LCTL("x") SS_TAP(X_LEFT) );
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	      
    } else{
    }
    return true;
    break; 
  case EM_POPN:
    if (record->event.pressed) {
      //Projecile open workspace
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_TAP(X_F5) "w");
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
      
    } else{
    }
    return true;
    break; 
  case EM_PDIR:
    if (record->event.pressed) {
      //Projectile dired
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_TAP(X_F5) "D");
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
    } else{
    }
    return true;
    break; 
  case EM_PFIL:
    if (record->event.pressed) {
      //Projectile find file
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_TAP(X_F5) "f");
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	

    } else{
    }
    return true;
    break; 
  case EM_PIBF:
    if (record->event.pressed) {
      //Projecile ibuffer
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_TAP(X_F5) "I");
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
      
    } else{
    }
    return true;
    break; 
  case EM_PRUN:
    if (record->event.pressed) {
      //Projecile ibuffer
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_TAP(X_F5) "u");
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
      
    } else{
    }
    return true;
    break; 
  case EM_PCPL:
    if (record->event.pressed) {
      //Projecile ibuffer
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_TAP(X_F5) "c");
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
      
    } else{
    }
    return true;
    break; 
  case EM_PSHL:
    if (record->event.pressed) {
      //Projecile next buffer 
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_TAP(X_F5) "x" "s");
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
    } else{
    }
    return true;
    break; 
  case EM_PRPL:
    if (record->event.pressed) {
      //Projecile next buffer 
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_TAP(X_F5) "r" );
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
    } else{
    }
    return true;
    break; 
  case EM_PNXT:
    if (record->event.pressed) {
      //Projecile next buffer 
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_TAP(X_F5) SS_TAP(X_RIGHT) );
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
    } else{
    }
    return true;
    break; 
  case EM_PPRV:
    if (record->event.pressed) {
      //Projecile prev buffer
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_TAP(X_F5) SS_TAP(X_LEFT) );
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	      
    } else{
    }
    return true;
    break; 
  case EM_WOPN:
    if (record->event.pressed) {
      //Perspective Open
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_TAP(X_F6) SS_LCTL("l")  );
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	      
    } else{
    }
    return true;
    break; 
  case EM_WSAV:
    if (record->event.pressed) {
      //Perspective Save
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_TAP(X_F6) SS_LCTL("s"));
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	      
    } else{
    }
    return true;
    break;
  case EM_WNXT:
    if (record->event.pressed) {
      //Perspective Next
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_TAP(X_F6) "n");
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	      
    } else{
    }
    return true;
    break; 
  case EM_WPRV:
    if (record->event.pressed) {
      //Perspective Prev
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_TAP(X_F6) "p");
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	      
    } else{
    }
    return true;
    break; 
  case EM_ZTXT:
    if (record->event.pressed) {
      //mod_state = get_mods();
      if (is_shift_active) {
	/*         if (!is_shift_active) { */
	/* #ifdef CONSOLE_ENABLE */
	/*     uprintf("Shift key Held!!!"); */
	/* #endif */
	/* 	   is_shift_active = true; */
	/*         } */
	if(get_mods() & MOD_MASK_SHIFT) del_mods(MOD_MASK_SHIFT);

	// increasd read timer
	shift_held_timer = timer_read(); 
 	//decrease text size 
	//send_string( SS_LALT("x") "text-scale-decrease" SS_TAP(X_ENTER) );
	send_string( SS_LCTL(SS_LALT("-")));
       	//set_mods(MOD_MASK_SHIFT);
      } else {
	//increse text size
	//send_string( SS_LALT("x") "text-scale-increase" SS_TAP(X_ENTER) );
	send_string( SS_LCTL(SS_LALT("=")));
      }
    } else{
      //del_mods(MOD_MASK_SHIFT);
    }
    return true;
    break; 
  case EM_FDEL:
    if (record->event.pressed) {
      //Delete word forward
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_LCTL(SS_TAP(X_DEL) ));
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
      
    } else{
    }
    return true;
    break;
  case EM_BDEL:
    if (record->event.pressed) {
      //Delete word backward
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_LCTL( SS_TAP(X_BSPC)) );
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
      
    } else{
    }
    return true;
    break;
  case EM_GOTO:
    if (record->event.pressed) {
      //Delete word forward
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_LALT("g") SS_LALT("g") );
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
      
    } else{
    }
    return true;
    break;
  case EM_OLINE:
    if (record->event.pressed) {
      //Open new line
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_LCTL("o") );
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
      
    } else{
    }
    return true;
    break;
  case EM_DLINE:
    if (record->event.pressed) {
      //Delete line
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_LCTL("k") );
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
      
    } else{
    }
    return true;
    break;
  case EM_ENXT:
    if (record->event.pressed) {
      //Next Error
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_LALT("g") SS_LALT("n") );
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
      
    } else{
    }
    return true;
    break;
  case EM_EPRV:
    if (record->event.pressed) {
      //Previous Erroc
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_LALT("g") SS_LALT("p") );
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
      
    } else{
    }
    return true;
    break;
  case EM_SNXT:
    if (record->event.pressed) {
      //Next sexp
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_LALT( SS_LALT("f")) );
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
      
    } else{
    }
    return true;
    break;
  case EM_SPRV:
    if (record->event.pressed) {
      //Previous Erroc
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_LALT( SS_LALT("b") ));
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
      
    } else{
    }
    return true;
    break;
  case EM_MAGIT:
    if (record->event.pressed) {
      //Previous Erroc
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_LALT( "x" ) "magit-status"  SS_TAP(X_ENTER));
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
      
    } else{
    }
    return true;
    break;
  case EM_FORMAT:
    if (record->event.pressed) {
      //Previous Erroc
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_LALT( "x" ) "indent-according-to-mode" SS_TAP(X_ENTER));
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
      
    } else{
    }
    return true;
    break;
  case EM_COMMENT:
    if (record->event.pressed) {
      //Previous Erroc
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_LALT( "x" ) "comment-or-uncomment-region" SS_TAP(X_ENTER));
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
      
    } else{
    }
    return true;
    break;
  default:
    return true;
   
  }
 
}

void process_combo_event(uint16_t combo_index, bool pressed) {
  switch(combo_index) {
  case SET_EDIT_MODE:
    if (pressed) {
      is_emacs_edit = !is_emacs_edit;
#ifdef CONSOLE_ENABLE
      uprintf("EMACS Editing: %d \n", is_emacs_edit);
#endif 
    }
    break;
  }
}

void numlock_on(void) {
  led_t led_state = host_keyboard_led_state();
  bool b = led_state.num_lock;
  if (!b) {
      register_code(KC_NUMLOCK);
      unregister_code(KC_NUMLOCK);
  }
}

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  //debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;

  set_single_persistent_default_layer(_DVORAK);
  numlock_on();
}

// EEPROM settings
void eeconfig_init_user(void) {
  // set default layer
  set_single_persistent_default_layer(_DVORAK);
}

// Runs just one time when the keyboard initializes.
void matrix_init_user(void){};

void matrix_scan_user(void) { // The very important timer.
  if (is_shift_active) {
    if (timer_elapsed(shift_held_timer) > SHIFT_HELD_MAX) {
#ifdef CONSOLE_ENABLE
      //uprintf("Shift key timout!!!\n");
#endif
      //if(get_mods() & MOD_MASK_SHIFT) del_mods(MOD_MASK_SHIFT);
      //is_shift_active = false;
      //shift_held_timer=0;
    }
  }
}


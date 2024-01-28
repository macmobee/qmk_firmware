#include QMK_KEYBOARD_H
#include "print.h"

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

#define CONSOLE_ENBLE
#define SHIFT_HELD_MAX 1000

bool is_shift_active = false; // ADD this near the begining of keymap.c
bool is_alt_active = false;
bool is_ctrl_active = false;

uint16_t shift_held_timer = 0;
uint16_t alt_held_timer = 0;
uint16_t ctrl_held_timer = 0;

uint8_t mod_state;

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
  /* EMACS custom keys */
  EM_BOF,  // Begining of buffer
  EM_EOF,  // End of buffer
  EM_SAVE, // Save / write Buffer
  EM_OPEN, // Open buffer
  EM_FIND, // Find / Replace
  EM_SWIN, // Split window (Vertical/Horizontal)
  EM_MWIN, // Merge window
  EM_OWIN, // Other window
  EM_POPN, // Projectile open
  EM_PDIR, // Projectile dired
  EM_PFIL, // Projectile find file
  EM_PIBF, // Projectile ibuffer
  EM_PNXT, // Projectile next buffer
  EM_PPRV, // Projectile prev buffer
  EM_WOPN, // Perspective Open
  EM_WSAV, // Perspective Save

  EM_WNXT, // Perspective Next
  EM_WPRV, // Perspective Prev
  EM_ZTXT, // Zoom Text
  EM_FDEL, // Delete word forward
  EM_BDEL  // Delele word backword
};

// Aliases to make the keymap more uniform
#define GUI_HOME GUI_T(KC_HOME)
#define GUI_PGUP GUI_T(KC_PGUP)
#define KPD_END LT(_KEYPAD, KC_END)
#define KPD_PGDN LT(_KEYPAD, KC_PGDN)
#define FUNC_SPC LT(_EFUNC, KC_SPC)

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
	| LShift |   Z  |   X  |   C  |   V  |   B  ||   N  |   M  |   ,  |   .  |   /  | RShift |
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
           KC_CAPS, KC_A,    KC_O,    KC_E,    KC_U,    KC_I,
           KC_LSFT, KC_QUOT, KC_Q,    KC_J,    KC_K,    KC_X,
                    KC_DLR,  KC_INS,  KC_LEFT, KC_RGHT,
           // Left Thumb
                    KC_LCTL, KC_LALT,
                             GUI_HOME,
           KC_BSPC, KC_DEL,  KPD_END,

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
           KPD_PGDN, KC_ENTER, KC_SPC
    ),


[_QWERTY] = LAYOUT (
           // Left Hand
           KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,
           KC_EQL,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,
           KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,
           KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,
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
	   LCTL(KC_G),  KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,   KC_F6,   KC_F7,   KC_F8,
           KC_EQL,  KC_1,       KC_2,       KC_3,       KC_4,       KC_5,
           KC_TAB,  KC_Q,       KC_NO,      KC_BSPC,    KC_P,       KC_G,
           KC_BSPC, LCTL(KC_X), KC_O,       KC_DEL,     KC_T,       KC_D,
           KC_LSFT, LCTL(KC_Z), LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), _______,
                    KC_GRV,     KC_INS,     KC_LEFT,    KC_RGHT,
           // Left Thumb
                    KC_LCTL, KC_LALT,
                             KC_HOME,
           KC_BSPC, KC_DEL,  KC_END,

           // Right Hand
           KC_F9,   KC_F10,  KC_F11,   KC_F12,   KC_PSCR, KC_SLCK, KC_PAUS, TG(_ADJUST),  RESET,
           KC_H,    KC_N,    EM_ZTXT,  KC_I,     KC_O,    KC_BSLS,
	   KC_PGUP, KC_HOME, KC_UP,    KC_END,   EM_OPEN,    KC_SLSH,
           KC_PGDN, KC_LEFT, KC_DOWN,  KC_RIGHT, EM_SAVE, KC_MINS,
           KC_K,    KC_M,    KC_COMM,  KC_DOT,  KC_SLSH, KC_RSFT,
                    KC_UP,   KC_DOWN,  KC_LBRC, KC_RBRC,
           // Right Thumb
           KC_RGUI, KC_RCTL,
           KC_PGUP,
           KC_PGDN, KC_ENTER, KC_SPC
    ),

[_EFUNC] = LAYOUT (
           // Left Hand
           KC_ESC,  KC_F1,   KC_F2,    KC_F3,    KC_F4,    KC_F5,   KC_F6,   KC_F7,   KC_F8,
           KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO,    KC_NO,
           KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO,    KC_NO,
           KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO,    KC_NO,
           _______,   KC_NO,   KC_NO,    KC_NO,    KC_NO,    KC_NO,
                    KC_GRV,  KC_INS,   KC_LEFT,  KC_RGHT,
           // Left Thumb
                    KC_LCTL, KC_LALT,
                             KC_HOME,
           KC_BSPC, KC_DEL,  KC_END,

           // Right Hand
           KC_F9,   KC_F10,  KC_F11,   KC_F12,   KC_PSCR,  KC_SLCK, KC_PAUS, TG(_ADJUST),  RESET,
           KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO,    KC_NO,
           KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO,    KC_NO,
           KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO,    KC_NO,
           KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO,    _______,
                    KC_UP,   KC_DOWN,  KC_LBRC,  KC_RBRC,
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
           _______, KC_NLCK, KC_PEQL, KC_PSLS, KC_PAST, _______,
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
	   EEP_RST, KC_NO,  KC_NO,  KC_NO,   KC_NO,    KC_NO, KC_NO, KC_NO, KC_NO,
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
    layer_on(_DVORAK);
    layer_off(_QWERTY);
    layer_off(_EEDIT);
    layer_off(_EFUNC);
    layer_off(_KEYPAD);
    layer_off(_ADJUST);
    
    return false;
    break;
  case QWERTY:
    //set_single_persistent_default_layer(_QWERTY);

    layer_off(_DVORAK);
    layer_on(_QWERTY);
    layer_off(_EEDIT);
    layer_off(_EFUNC);
    layer_off(_KEYPAD);
    layer_off(_ADJUST);
    
    return false;
    break;
  case EEDIT:
    //set_single_persistent_default_layer(_EEDIT);
    
    layer_off(_DVORAK);
    layer_off(_QWERTY);
    layer_on(_EEDIT);
    layer_off(_EFUNC);
    layer_off(_KEYPAD);
    layer_off(_ADJUST);
    
    return false;
    break;
  case EFUNC:
    //set_single_persistent_default_layer(_EFUNC);

    layer_off(_DVORAK);
    layer_off(_QWERTY);
    layer_off(_EEDIT);
    layer_on(_EFUNC);
    layer_off(_KEYPAD);
    layer_off(_ADJUST);
    
    return false;
    break;
  case KEYPAD:
    //set_single_persistent_default_layer(_KEYPAD);

    layer_off(_DVORAK);
    layer_off(_QWERTY);
    layer_off(_EEDIT);
    layer_off(_EFUNC);
    layer_on(_KEYPAD);
    layer_off(_ADJUST);
    
    return false;
    break;
  case ADJUST:
    //set_single_persistent_default_layer(_ADJUST);

    layer_off(_DVORAK);
    layer_off(_QWERTY);
    layer_off(_EEDIT);
    layer_off(_EFUNC);
    layer_off(_KEYPAD);
    layer_on(_ADJUST);
    
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
  case EM_BOF:
    if (record->event.pressed) {
      mod_state = get_mods();
      if (mod_state & MOD_MASK_SHIFT) {
	//decrease text size
      } else {
	//increse text size
      }
    } else{
    }
    return true;
    break;
  case EM_EOF:
    if (record->event.pressed) {
      mod_state = get_mods();
      if (mod_state & MOD_MASK_SHIFT) {
	//decrease text size
      } else {
	//increse text size
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
      } else {
	//save
      }
    } else{
    }
    return true;
    break; 
  case EM_OPEN:
    if (record->event.pressed) {
      //increse text size
    } else{
    }
    return true;
    break; 
  case EM_FIND:
    if (record->event.pressed) {
      mod_state = get_mods();
      if (mod_state & MOD_MASK_SHIFT) {
	//Replace
      } else {
	//i-search
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
      } else {
	//split Horizontal
      }
    } else{
    }
    return true;
    break; 
  case EM_MWIN:
    if (record->event.pressed) {
      //delete other windows
    } else{
    }
    return true;
    break; 
  case EM_OWIN:
    if (record->event.pressed) {
      //increse text size
    } else{
    }
    return true;
    break; 
  case EM_POPN:
    if (record->event.pressed) {
      //Projecile open workspace
    } else{
    }
    return true;
    break; 
  case EM_PDIR:
    if (record->event.pressed) {
      //Projectile dired
    } else{
    }
    return true;
    break; 
  case EM_PFIL:
    if (record->event.pressed) {
      //Projectile find file
    } else{
    }
    return true;
    break; 
  case EM_PIBF:
    if (record->event.pressed) {
      //Projecile ibuffer 
    } else{
    }
    return true;
    break; 
  case EM_PNXT:
    if (record->event.pressed) {
      //Projecile ibuffer 
    } else{
    }
    return true;
    break; 
  case EM_PPRV:
    if (record->event.pressed) {
      //Projecile ibuffer 
    } else{
    }
    return true;
    break; 
  case EM_WOPN:
    if (record->event.pressed) {
      //Perspective Open 
    } else{
    }
    return true;
    break; 
  case EM_WSAV:
    if (record->event.pressed) {
      //Perspective Open
    } else{
    }
    return true;
    break;
  case EM_WNXT:
    if (record->event.pressed) {
      //Perspective Next 
    } else{
    }
    return true;
    break; 
  case EM_WPRV:
    if (record->event.pressed) {
      //Perspective Prev 
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
	send_string( SS_LALT("x") "text-scale-decrease" SS_TAP(X_ENTER) );
       	//set_mods(MOD_MASK_SHIFT);
      } else {
	//increse text size
	send_string( SS_LALT("x") "text-scale-increase" SS_TAP(X_ENTER) );
      }
    } else{
      //del_mods(MOD_MASK_SHIFT);
    }
    return true;
    break; 
  case EM_FDEL:
    if (record->event.pressed) {
      //Delete word forward
    } else{
    }
    return true;
    break;
  case EM_BDEL:
    if (record->event.pressed) {
      //Delete word backward
    } else{
    }
    return true;
    break;
  default:
    return true;
   
  }
 
};


void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  //debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;

  set_single_persistent_default_layer(_DVORAK);
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

/* Copyright 2019
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#ifdef TAPPING_TOGGLE
  #undef TAPPING_TOGGLE
  #define TAPPING_TOGGLE 2
#endif


#define CONSOLE_ENBLE
#define SHIFT_HELD_MAX 1000

enum layer_names {
  _EEDIT  = 0,
  _EFILE,
  _EFUNC,
  _EPROJ ,
  _WINKEYS,
  _DWM,
  _ADJUST
};

enum custom_keycodes {
  EEDIT  = SAFE_RANGE,
  EFILE ,
  EFUNC,
  EPROJ,
  WINKEY,
  DWM,
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
  EM_EVAL,  // Evaluate buffer
  EM_BOF,   // Begining of buffer
  EM_EOF,   // End of buffer
  EM_SAVE,  // Save / write Buffer
  EM_OPEN,  // Open buffer
  EM_KBUF,  // Kill buffer
  EM_RBUF,  // Revert buffer
  EM_NBUF,  // reName buffer
  EM_FIND,  // Find / Replace
  EM_SWIN,  // Split window (Vertical/Horizontal)
  EM_MWIN,  // Merge windows
  EM_OWIN,  // Other window
  EM_AWIN,  // Adjust window sizex
  EM_EWIN,  // Expand window
  EM_RWIN,  // Reduce window
  EM_PRVBUF, // Previous buffer
  EM_NXTBUF, // Next buffer
  EM_POPN,  // Projectile open
  EM_PDIR,  // Projectile dired
  EM_PFIL,  // Projectile find file
  EM_POFL,  // Projectile find other file
  EM_PRFL,  // Projectlie recent file
  EM_PTGL,  // Projectile Toggle implmentation/test
  EM_PREF,  // Projectile find references
  EM_PIBF,  // Projectile ibuffer
  EM_PRUN,  // Projectile run
  EM_PCPL,  // Prejectile compile
  EM_PSHL, // Projectile shell
  EM_PRPL, // Prejectile replace
  EM_PLCL, // Projecile edit .dir-local
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
  EM_KFRM, // Kill edit frame
  EM_OPFL, // Open file at point
  EM_OAFL, // Open alternate file
  EM_MAGIT
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

/*


 *--------------------------------------------------------------------------------------------------------*
 | ctrl-g              | em_sprv       |  em_bof           | em_eprv       | em_magit       | ctrl-s      |
 *--------------------------------------------------------------------------------------------------------*
 |                     | em_snxt       |  em_eof           | em_enxt       | em_macro       |             |
 *--------------------------------------------------------------------------------------------------------*
 | ctrl-;              | ctrl-w        | alt-w             | ctrl-y        | ctrl-x u       | ctrl-k      |
 *--------------------------------------------------------------------------------------------------------*
 | alt-x               | em_comment    | em_format         | em_selbuf     | em_goto        | em_save     |
 *--------------------------------------------------------------------------------------------------------*
 | ADJUST              | WIN           | EMACS             | DWM           | KEYPAD         | SHIFT       |
 *--------------------------------------------------------------------------------------------------------*


 *----------------------------------------------------------------------------------------------------------*
 | ctrl-g              | ctrl-x 0      | ctrl-1            | ctrl-2        | ctrl-3         | eval-buffer   |
 *----------------------------------------------------------------------------------------------------------*
 | ctrl-x #            | ctrl-x ctrl-f | ctrl-x ctrl-s     | ctrl-x ctrl-w | ctrl-x k       | file-at-point |
 *----------------------------------------------------------------------------------------------------------*
 |                     |               |  revert           |               | kill           |               |
 *----------------------------------------------------------------------------------------------------------*
 |                     | ctrl-x  left  | ctrl-x o          | ctrl-x right  | ctrl-x ctrl-b  |               |
 *----------------------------------------------------------------------------------------------------------*
 | ADJUST              | WIN           | EMACS             | DWM           | KEYPAD         | SHIFT         |
 *----------------------------------------------------------------------------------------------------------*


 *--------------------------------------------------------------------------------------------------------*
 |                     | F5 E          | F5 t              | F5 ?          | F5 a           |             |
 *--------------------------------------------------------------------------------------------------------*
 | F5 f                | F5 f          | F5 c              | F5 u          | F6 ctls-l      | F6 right    |
 *--------------------------------------------------------------------------------------------------------*
 | F5 D                | F5 e          | F5 w              | F5 x s        | F6 ctrl-s      | F6 left     |
 *--------------------------------------------------------------------------------------------------------*
 | F5 I                | F5 left       |                   | F5 right      | F5 v           |             |
 *--------------------------------------------------------------------------------------------------------*
 | ADJUST              | WIN           | EMACS             | DWM           | KEYPAD         | SHIFT       |
 *--------------------------------------------------------------------------------------------------------*w

 *--------------------------------------------------------------------------------------------------------*
 | ESC                 |               |                   |               |                | ctrl-f      |
 *--------------------------------------------------------------------------------------------------------*
 |                     | ctrl-o        |  ctrl-s           |               |                |             |
 *--------------------------------------------------------------------------------------------------------*
 |                     | ctrl-x        |  ctrl-c           | ctrl-v        | ctrl-z         |             |
 *--------------------------------------------------------------------------------------------------------*
 | super-r             |               |                   |               |                | super-l     |
 *--------------------------------------------------------------------------------------------------------*
 | ADJUST              | WIN           | EMACS             | DWM           | KEYPAD         | SHIFT       |
 *--------------------------------------------------------------------------------------------------------*

 *--------------------------------------------------------------------------------------------------------------*
 |                     | supert-t      | super-f           | super-m       | super-space    | super-shift-space |
 *--------------------------------------------------------------------------------------------------------------*
 | super-shift-c       | super-0       | super-shift-c     |               | super-h        | super-c           |
 *--------------------------------------------------------------------------------------------------------------*
 | super-ctrl-shift-c  | super-shift-0 | super-shift-enter | super-enter   | super-n        | super-t           |
 *--------------------------------------------------------------------------------------------------------------*
 | super-p             | super-,       |                   | super-.       |                | super-ctrl-l      |
 *--------------------------------------------------------------------------------------------------------------*
 | ADJUST              | WIN           | EMACS             | DWM           | KEYPAD         | SHIFT             |
 *--------------------------------------------------------------------------------------------------------------*
 

 *--------------------------------------------------------------------------------------------------------*
 | EEP_RESET           | EM-EDIT       | EM-Projectile     |  DWM          |  WIN-EDIT      |             |
 *--------------------------------------------------------------------------------------------------------*
 | RESET               | M-rewind      |  M-Select         |  M-fastfwd    | brightness +   | volume +    |
 *--------------------------------------------------------------------------------------------------------*
 |                     | M-prev        |  M-play           |  M-next       | brigthness -   | volume -    |
 *--------------------------------------------------------------------------------------------------------*
 | BL-toggle           | BL-incr       |  BL-Step          |  BL-decr      | BR-toggle      | mute        |
 *--------------------------------------------------------------------------------------------------------*
 | ADJUST              | WIN           | EMACS             | DWM           | KEYPAD         | SHIFT       |
 *--------------------------------------------------------------------------------------------------------*
 
 */


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_EEDIT] = LAYOUT_grid(
	      LCTL(KC_G),    EM_SPRV,      EM_BOF,     EM_EPRV,    EM_MACRO,   EM_FIND,
	      EM_BDEL,       EM_SNXT,      EM_EOF,     EM_ENXT,    KC_NO,      EM_FDEL,
	      LCTL(KC_SCLN), LCTL(KC_W),   LALT(KC_W), LCTL(KC_Y), EM_UNDO,    LCTL(KC_K),  
	      LALT(KC_X),    EM_COMMENT,   EM_FORMAT,  EM_SELBUF,  EM_GOTO,    EM_SAVE,
	      TT(_ADJUST),   TT(_WINKEYS), TT(_DWM),   TT(_EPROJ), TT(_EFILE), TT(_EFUNC)            
	),
	[_EFILE] = LAYOUT_grid(
	      EM_KFRM,       EM_MWIN,      EM_OWIN,    EM_SWIN,       EM_EWIN,    EM_EVAL,     
	      EM_CMD,        KC_UNDO,      KC_CUT,     LSFT(EM_SWIN), EM_RWIN,    LSFT(EM_EVAL), 
	      EM_SHL,        EM_OPEN,      EM_RBUF,    KC_NO,         EM_KBUF,    EM_OPFL, 
	      KC_LSFT,       EM_PRVBUF,    EM_OWIN,    EM_NXTBUF,     KC_NO,      EM_OAFL,   
	      TT(_ADJUST),   TT(_WINKEYS), TT(_DWM),   TT(_EPROJ),    TT(_EEDIT), TT(_EFUNC)            
	),
	[_EFUNC] = LAYOUT_grid(
	      KC_NO,         KC_NO,        KC_NO,      KC_NO,      KC_NO,      EM_MAGIT,
	      KC_NO,         KC_NO,        KC_NO,      KC_NO,      KC_NO,      KC_NO,
	      KC_NO,         KC_LEFT,      KC_UP,      KC_DOWN,    KC_RGHT,    KC_NO,
	      KC_NO,         KC_NO,        KC_NO,      KC_NO,      KC_NO,      KC_NO,
	      TT(_ADJUST),   TT(_WINKEYS), TT(_DWM),   TT(_EEDIT), TT(_EFILE), TT(_EFUNC)            
	),
	[_EPROJ] = LAYOUT_grid(
	      KC_NO,         EM_PLCL,      EM_PTGL,     EM_PREF,    EM_POFL,    KC_NO,
	      EM_PFIL,       EM_PRPL,      EM_PCPL,     EM_PRUN,    EM_WOPN,    EM_WPRV,
	      EM_PDIR,       EM_PRFL,      EM_POPN,     EM_PSHL,    EM_WSAV,    EM_WNXT,
	      EM_PIBF,       EM_PPRV,      EM_OWIN,     EM_PNXT,    KC_NO,      KC_NO, 
	      TT(_ADJUST),   TT(_WINKEYS), TT(_EEDIT),  TT(_EPROJ), TT(_EFILE), TT(_EFUNC)            
	),
	[_WINKEYS] = LAYOUT_grid(
	      KC_ESC,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      LCTL(KC_F),
	      KC_NO,       LCTL(KC_O), LCTL(KC_S), KC_NO,      KC_NO,      KC_NO,
	      KC_NO,       LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), LCTL(KC_Z), KC_NO,
	      RGUI(KC_R),  KC_NO,      KC_NO,      KC_NO,      KC_NO,      RGUI(KC_L),
	      TT(_ADJUST), TT(_EEDIT), TT(_DWM),   TT(_EPROJ), TT(_EFILE), TT(_EFUNC)            
	),
	[_DWM] = LAYOUT_grid(
              KC_NO,            RGUI(KC_T),       RGUI(KC_F),         RGUI(KC_M),    RGUI(KC_SPC), RGUI(RSFT(KC_ENT)),
	      KC_NO,            RGUI(KC_0),       KC_NO,              KC_NO,         RGUI(KC_H),   RGUI(KC_C),
	      RGUI(RSFT(KC_C)), RGUI(RSFT(KC_0)), RGUI(RSFT(KC_ENT)), RGUI(KC_ENT),  RGUI(KC_N),    RGUI(KC_T),
	      RGUI(KC_P),       RGUI(KC_DOT),     KC_NO,              RGUI(KC_COMM), KC_NO,        LCTL(LALT(KC_L)),
	      TT(_EEDIT),       TT(_WINKEYS),     TT(_DWM),           TT(_EPROJ),    TT(_EFILE),   TT(_EFUNC)            
	),
	[_ADJUST] = LAYOUT_grid(
	      RESET,       DEBUG,        KC_NO,      KC_NO,      KC_NO,      RESET,
	      KC_MPRV,     KC_MRWD,      KC_MPLY,    KC_STOP,    KC_MFFD,    KC_MNXT,
	      KC_MSEL,     KC_VOLD,      KC_MUTE,    KC_NO,      KC_VOLU,    KC_EJCT,
	      BL_TOGG,     BL_DEC,       BL_STEP,    BL_INC,     BL_BRTG,    KC_NO,
	      EEP_RST,     TT(_WINKEYS), TT(_DWM), TT(_EPROJ), TT(_EFILE), TT(_EFUNC)            
	)
};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef CONSOLE_ENABLE
  uprintf("KL: kc: 0x%04X, col: %u, row: %u, pressed: %b, time: %u, interrupt: %b, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif
    
  switch (keycode) {
    
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
  case EM_POFL:
    if (record->event.pressed) {
      //Projectile find file
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_TAP(X_F5) "a");
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	

    } else{
    }
    return true;
    break; 
  case EM_PRFL:
    if (record->event.pressed) {
      //Projectile find file
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_TAP(X_F5) "e");
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	

    } else{
    }
    return true;
    break; 
  case EM_PLCL:
    if (record->event.pressed) {
      //Projectile find file
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_TAP(X_F5) "E");
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	

    } else{
    }
    return true;
    break; 
  case EM_PREF:
    if (record->event.pressed) {
      //Projectile find file
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_TAP(X_F5) "?");
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	

    } else{
    }
    return true;
    break; 
  case EM_PTGL:
    if (record->event.pressed) {
      //Projectile find file
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_TAP(X_F5) "t");
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
	send_string( SS_TAP(X_F6) "p");
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
	send_string( SS_TAP(X_F6) "n");
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
  case EM_KFRM:
    if (record->event.pressed) {
      //Previous Erroc
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_LCTL( "x" ) "#" SS_TAP(X_ENTER));
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
      
    } else{
    }
    return true;
    break;

  case EM_OPFL:
    if (record->event.pressed) {
      //Previous Erroc
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_LALT( "x" ) "find-file-at-point" SS_TAP(X_ENTER));
      } else {
	//
	//send_string( SS_LCTL("f"));
      }	
      
    } else{
    }
    return true;
    break;
  case EM_OAFL:
    if (record->event.pressed) {
      //Previous Erroc
      if(is_emacs_edit){
	// Ctrl+x Ctrl+f
	send_string( SS_LALT( "x" ) "find-alternate-file" SS_TAP(X_ENTER));
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

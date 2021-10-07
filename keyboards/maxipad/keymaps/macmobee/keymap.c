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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT_grid(
	      LSFT(KC_NO),  KC_TRNS,   TT(2),     TO(1),       KC_NO,      TT(1),     
	      KC_PGDN,     KC_UNDO,   KC_CUT,    KC_COPY,     KC_PSTE,    KC_G, 
	      KC_HOME,     KC_A,      KC_O,      KC_E,        KC_U,       KC_S, 
	      KC_PGUP,     KC_D,      KC_GT,     KC_W,        KC_LT,      KC_Z,   
	      KC_ESC,      KC_SCLN,   KC_CIRC,   KC_QUOT,     KC_Y,       KC_FIND            
	),
	[1] = LAYOUT_grid(
	      KC_NO,       KC_NO,     TO(0),     TO(2),       KC_NO,      KC_NO,
	      KC_NO,       KC_PENT,   KC_P0,     KC_PCMM,     KC_PPLS,    KC_NO,
	      KC_NO,       KC_P7,     KC_P8,     KC_P9,       KC_PMNS,    KC_NO,
	      KC_NO,       KC_P4,     KC_P5,     KC_P6,       KC_PAST,    KC_NO,
	      KC_NO,       KC_P1,     KC_P2,     KC_P3,       KC_PSLS,    KC_NO
	),
	[2] = LAYOUT_grid(
	      KC_NO,       KC_NO,     TO(1),      TO(3),      KC_NO,      KC_TRNS,
	      KC_NO,       KC_F16,    KC_F17,     KC_F18,     KC_F19,     KC_F20,
	      KC_NO,       KC_F11,    KC_F12,     KC_F13,     KC_F14,     KC_F15,
	      KC_NO,       KC_F6,     KC_F7,      KC_F8,      KC_F9,      KC_F10, 
	      KC_NO,       KC_F1,     KC_F2,      KC_F3,      KC_F4,      KC_F5  
	),
	[3] = LAYOUT_grid(
	      KC_NO,       KC_NO,     TO(2),      TO(4),      KC_NO,      KC_NO,
	      KC_NO,       KC_NO,     KC_NO,      KC_NO,      KC_NO,      KC_NO,
	      KC_NO,       KC_LEFT,   KC_UP,      KC_DOWN,    KC_RGHT,    KC_NO,
	      KC_NO,       KC_NO,     KC_NO,      KC_NO,      KC_NO,      KC_NO,
	      KC_NO,       KC_NO,     KC_NO,      KC_NO,      KC_NO,      KC_NO
	),
	[4] = LAYOUT_grid(
	       KC_NO,       KC_NO,     TO(3),      TO(5),      KC_NO,      KC_NO,
	      KC_NO,       KC_NO,     KC_NO,      KC_NO,      KC_NO,      KC_NO,
	      KC_NO,       KC_NO,     KC_NO,      KC_NO,      KC_NO,      KC_NO,
	      KC_NO,       KC_NO,     KC_NO,      KC_NO,      KC_NO,      KC_NO,
	      KC_NO,       KC_NO,     KC_NO,      KC_NO,      KC_NO,      KC_NO
	),
	[5] = LAYOUT_grid(
	      KC_NO,       KC_NO,     TO(4),      TO(6),      KC_NO,      KC_NO,
	      KC_NO,       KC_NO,     KC_NO,      KC_NO,      KC_NO,      KC_NO,
	      KC_NO,       KC_NO,     KC_NO,      KC_NO,      KC_NO,      KC_NO,
	      KC_NO,       KC_NO,     KC_NO,      KC_NO,      KC_NO,      KC_NO,
	      KC_NO,       KC_NO,     KC_NO,      KC_NO,      KC_NO,      KC_NO
	),
	[6] = LAYOUT_grid(
	      EEP_RST,     DEBUG,     TO(5),      KC_TRNS,    KC_NO,      RESET,
	      BL_TOGG,     BL_DEC,    BL_STEP,    BL_BRTG,    BL_INC,     KC_NO,
	      KC_MPRV,     KC_MRWD,   KC_MPLY,    KC_STOP,    KC_MFFD,    KC_MNXT,
	      KC_MSEL,     KC_VOLD,   KC_MUTE,    KC_NO,      KC_VOLU,    KC_EJCT,
	      KC_NO,       KC_NO,     KC_NO,      KC_NO,      KC_NO,      KC_NO
	)
};

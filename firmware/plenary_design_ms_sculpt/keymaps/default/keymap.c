#include "analog.h"
#include "os_detection.h"
#include QMK_KEYBOARD_H

static uint32_t powerup_timer;
static uint32_t last_adc_check = 0;  // Timer for ADC check
static uint8_t base_layer = LAYER_PC_BASE; // Keyboard matrix layer
static bool last_media_switch = false;

enum custom_keycodes {
    KC_L_WIN = SAFE_RANGE,
    KC_R_WIN,
    MED_F5,
    MED_F6,
    MED_F7,
};

void matrix_init_user(void) {
    adcInit();

    // Initialize GP1 as an output, to drive the Red Caps LED
    setPinOutput(LED_CAPS_LOCK_PIN);

    // Initialize powerup_timer at startup
    powerup_timer = timer_read32();
}

bool get_media_switch_state(void) {
    uint16_t adc_value = analogReadPinAdc(ADC_PIN, 0);

    // Physical switch flipped or not
    bool media_switch = (adc_value > 100) ? true : false;

    return media_switch;
}

void matrix_scan_user(void) {
    // Fn/media switch
    // Only check the ADC value once every 1000 ms (1 second)
    if (timer_elapsed32(last_adc_check) >= 1000) {
        last_adc_check = timer_read32();  // Reset the timer

        bool media_switch = get_media_switch_state();

        if (media_switch != last_media_switch) {
            uprintf("Media switch changed to %u\n", media_switch);

            uint8_t media_layer = base_layer + 1;
            if (media_switch) {
                uprintf("Turning on layer %u\n", media_layer);
                layer_on(media_layer);
            } else {
                uprintf("Turning off layer %u\n", media_layer);
                layer_off(media_layer);
            }

            last_media_switch = media_switch;
        }
    }
}

bool led_update_user(led_t led_state) {
    // Check the state of Caps Lock
    if (led_state.caps_lock) {
        // Turn on the LED if Caps Lock is active
        writePinHigh(LED_CAPS_LOCK_PIN);
    } else {
        // Turn off the LED if Caps Lock is inactive
        writePinLow(LED_CAPS_LOCK_PIN);
    }
    return true; // Return true to allow other LED updates to occur
}

void custom_key_combo(uint16_t key1, uint16_t key2, uint16_t key3) {
    register_code(key1);
    register_code(key2);
    if (key3 != KC_NO) {
        register_code(key3);
    }
    unregister_code(key1);
    unregister_code(key2);
    if (key3 != KC_NO) {
        unregister_code(key3);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (record->event.pressed) {
        switch (keycode) {
            case KC_L_WIN:
                custom_key_combo(KC_LCTL, KC_LGUI, KC_LEFT);
                break;
            case KC_R_WIN:
                custom_key_combo(KC_RCTL, KC_RGUI, KC_RIGHT);
                break;
            case MED_F5:
                custom_key_combo(KC_LSFT, KC_LGUI, KC_NO);
                break;
            case MED_F6:
                custom_key_combo(KC_LALT, KC_LGUI, KC_NO);
                break;
            case MED_F7:
                custom_key_combo(KC_LCTL, KC_LGUI, KC_NO);
                break;
        }

        // ghosting logic
        uint8_t rows_active[MATRIX_ROWS] = {0};  // Track active rows
        uint8_t cols_active[MATRIX_COLS] = {0};  // Track active columns

        // Step 1: Detect active keys and update rows/columns
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            for (uint8_t col = 0; col < MATRIX_COLS; col++) {
                if (matrix_is_on(row, col)) {
                    rows_active[row]++;
                    cols_active[col]++;
                }
            }
        }

        // Step 2: Check for ghosting condition
        uint8_t row = record->event.key.row;
        uint8_t col = record->event.key.col;

        if (rows_active[row] >= 2 && cols_active[col] >= 2) {
            // Ghosting condition detected, log and ignore the key press
            uprintf("Ghosting detected: Ignoring keycode %u at row %u, col %u\n", keycode, row, col);
            return false; // Ignore this key press
        }
    }


    return true;
}

bool process_detected_host_os_user(os_variant_t os) {
    switch (os) {
        case OS_MACOS:
        case OS_IOS:
            uprintf("Detected OS: iOS or macOS\n");
            base_layer = LAYER_MAC_BASE;
            break;
        case OS_WINDOWS:
            base_layer = LAYER_PC_BASE;
            uprintf("Detected OS: Windows\n");
            break;
        case OS_LINUX:
            base_layer = LAYER_PC_BASE;
            uprintf("Detected OS: Linux\n");
            break;
        case OS_UNSURE:
            base_layer = LAYER_PC_BASE;
            uprintf("Detected OS: Unsure\n");
            break;
        default:
            base_layer = LAYER_PC_BASE;
            uprintf("Detected OS: Unknown\n");
            break;
    }

    // Set the initial layer
    layer_move(base_layer);

    last_media_switch = get_media_switch_state();

    return true;  // Return true to allow further processing
}

// PC      -> Mac Layout changes
// KC_LGUI -> KC_LALT
// KC_LALT -> KC_LGUI
// KC_APP  -> KC_RALT
// KC_RALT -> KC_RGUI

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [LAYER_PC_BASE] = LAYOUT_SCULPT(
        // Row 0
        KC_NO,   KC_PAUS, KC_NO,   KC_DEL,            KC_0,    KC_9,    KC_NO,   KC_8,    KC_BSPC,    KC_7,    KC_TAB,  KC_Q,     KC_2,    KC_1,    KC_NO,   KC_NO,   KC_NO,   KC_NO,
        // Row 1
        KC_NO,   KC_PGUP, KC_NO,   KC_F12,            KC_LBRC, KC_MINS, KC_NO,   KC_RBRC, KC_INS,     KC_Y,    KC_F5,   KC_F3,    KC_W,    KC_4,    KC_NO,   KC_F6,   KC_NO,   KC_NO,
        // Row 2
        KC_NO,   KC_HOME, KC_NO,   KC_CALC,           KC_P,    KC_O,    KC_NO,   KC_I,    KC_NO,      KC_U,    KC_R,    KC_E,     KC_CAPS, KC_3,    KC_NO,   KC_T,    KC_NO,   KC_NO,
        // Row 3
        KC_NO,   KC_SCRL, KC_NO,   KC_ENT,            KC_SCLN, KC_L,    KC_NO,   KC_K,    KC_BSLS,    KC_J,    KC_F,    KC_D,     KC_NUBS, KC_A,    KC_NO,   KC_LGUI, KC_NO,   KC_NO,
        // Row 4
        KC_NO,   KC_NO,   KC_NO,   KC_APP,            KC_SLSH, KC_QUOT, KC_RALT, KC_NO,   KC_LEFT,    KC_H,    KC_G,    KC_F4,    KC_S,    KC_ESC,  KC_NO,   KC_NO,   KC_LALT, KC_NO,
        // Row 5
        KC_NO,   KC_END,  KC_RSFT, KC_PGDN,           KC_NO,   KC_DOT,  KC_NO,   KC_COMM, KC_NO,      KC_M,    KC_V,    KC_C,     KC_X,    KC_Z,    KC_LSFT, KC_NO,   KC_NO,   KC_NO,
        // Row 6
        KC_RCTL, KC_RGHT, KC_NO,   KC_UP,             KC_DOWN, KC_NO,   KC_NO,   KC_NO,   KC_SPC,     KC_N,    KC_B,    KC_SPC,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_LCTL,
        // Row 7
        KC_NO,   KC_PSCR, KC_NO,   KC_F11,            KC_EQL,  KC_F9,   KC_NO,   KC_F8,   KC_F10,     KC_F7,   KC_5,    KC_F2,    KC_F1,   KC_GRV,  KC_NO,   KC_6,    KC_NO,   KC_NO
    ),
    // Media Fn PC
    [LAYER_MEDIA_FN_PC] = LAYOUT_SCULPT(
        // Row 0
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        // Row 1
        KC_TRNS, KC_TRNS, KC_TRNS, KC_KB_VOLUME_UP,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, MED_F5,  KC_L_WIN, KC_TRNS, KC_TRNS, KC_TRNS, MED_F6,  KC_TRNS, KC_TRNS,
        // Row 2
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        // Row 3
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        // Row 4
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_R_WIN, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        // Row 5
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        // Row 6
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        // Row 7
        KC_TRNS, KC_TRNS, KC_TRNS, KC_KB_VOLUME_DOWN, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_KB_MUTE, MED_F7,  KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    // Mac base layer
    [LAYER_MAC_BASE] = LAYOUT_SCULPT(
        // Row 0
        KC_NO,   KC_PAUS, KC_NO,   KC_DEL,            KC_0,    KC_9,    KC_NO,   KC_8,    KC_BSPC,    KC_7,    KC_TAB,  KC_Q,     KC_2,    KC_1,    KC_NO,   KC_NO,   KC_NO,   KC_NO,
        // Row 1
        KC_NO,   KC_PGUP, KC_NO,   KC_F12,            KC_LBRC, KC_MINS, KC_NO,   KC_RBRC, KC_INS,     KC_Y,    KC_F5,   KC_F3,    KC_W,    KC_4,    KC_NO,   KC_F6,   KC_NO,   KC_NO,
        // Row 2
        KC_NO,   KC_HOME, KC_NO,   KC_CALC,           KC_P,    KC_O,    KC_NO,   KC_I,    KC_NO,      KC_U,    KC_R,    KC_E,     KC_CAPS, KC_3,    KC_NO,   KC_T,    KC_NO,   KC_NO,
        // Row 3
        KC_NO,   KC_SCRL, KC_NO,   KC_ENT,            KC_SCLN, KC_L,    KC_NO,   KC_K,    KC_BSLS,    KC_J,    KC_F,    KC_D,     KC_NUBS, KC_A,    KC_NO,   KC_LGUI, KC_NO,   KC_NO,
        // Row 4
        KC_NO,   KC_NO,   KC_NO,   KC_RALT,           KC_SLSH, KC_QUOT, KC_RGUI, KC_NO,   KC_LEFT,    KC_H,    KC_G,    KC_F4,    KC_S,    KC_ESC,  KC_NO,   KC_NO,   KC_LGUI, KC_NO,
        // Row 5
        KC_NO,   KC_END,  KC_RSFT, KC_PGDN,           KC_NO,   KC_DOT,  KC_NO,   KC_COMM, KC_NO,      KC_M,    KC_V,    KC_C,     KC_X,    KC_Z,    KC_LSFT, KC_NO,   KC_NO,   KC_NO,
        // Row 6
        KC_RCTL, KC_RGHT, KC_NO,   KC_UP,             KC_DOWN, KC_NO,   KC_NO,   KC_NO,   KC_SPC,     KC_N,    KC_B,    KC_SPC,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_LCTL,
        // Row 7
        KC_NO,   KC_PSCR, KC_NO,   KC_F11,            KC_EQL,  KC_F9,   KC_NO,   KC_F8,   KC_F10,     KC_F7,   KC_5,    KC_F2,    KC_F1,   KC_GRV,  KC_NO,   KC_6,    KC_NO,   KC_NO
    ),
    // Media FN Mac
    [LAYER_MEDIA_FN_MAC] = LAYOUT_SCULPT(
        // Row 0
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        // Row 1
        KC_TRNS, KC_TRNS, KC_TRNS, KC_KB_VOLUME_UP,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, MED_F5,  KC_L_WIN, KC_TRNS, KC_TRNS, KC_TRNS, MED_F6,  KC_TRNS, KC_TRNS,
        // Row 2
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        // Row 3
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        // Row 4
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_R_WIN, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        // Row 5
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        // Row 6
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        // Row 7
        KC_TRNS, KC_TRNS, KC_TRNS, KC_KB_VOLUME_DOWN, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_KB_MUTE, MED_F7,  KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    )
};
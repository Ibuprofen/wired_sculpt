#pragma once

#define MATRIX_ROWS 8
#define MATRIX_COLS 18

// Define the row and column pins according to your layout
#define MATRIX_ROW_PINS { GP28, GP27, GP26, GP25, GP24, GP23, GP22, GP20 }
#define MATRIX_COL_PINS { GP19, GP18, GP17, GP16, GP15, GP14, GP13, GP12, GP11, GP10, GP9, GP8, GP7, GP6, GP5, GP4, GP3, GP2 }

// Specify the diode direction (typically COL2ROW or ROW2COL)
#define DIODE_DIRECTION COL2ROW

// Define debounce time (e.g., 8 ms)
#define DEBOUNCE 8

#define BOOTMAGIC_ROW 6
#define BOOTMAGIC_COLUMN 11

#define ADC_PIN GP29
#define LED_CAPS_LOCK_PIN GP1

#define LAYER_PC_BASE 0
#define LAYER_MEDIA_FN_PC 1
#define LAYER_MAC_BASE 2
#define LAYER_MEDIA_FN_MAC 3

// Custom LAYOUT macro for 8x18 matrix
#define LAYOUT_SCULPT( \
    k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0A, k0B, k0C, k0D, k0E, k0F, k0G, k0H, \
    k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1A, k1B, k1C, k1D, k1E, k1F, k1G, k1H, \
    k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2A, k2B, k2C, k2D, k2E, k2F, k2G, k2H, \
    k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k3A, k3B, k3C, k3D, k3E, k3F, k3G, k3H, \
    k40, k41, k42, k43, k44, k45, k46, k47, k48, k49, k4A, k4B, k4C, k4D, k4E, k4F, k4G, k4H, \
    k50, k51, k52, k53, k54, k55, k56, k57, k58, k59, k5A, k5B, k5C, k5D, k5E, k5F, k5G, k5H, \
    k60, k61, k62, k63, k64, k65, k66, k67, k68, k69, k6A, k6B, k6C, k6D, k6E, k6F, k6G, k6H, \
    k70, k71, k72, k73, k74, k75, k76, k77, k78, k79, k7A, k7B, k7C, k7D, k7E, k7F, k7G, k7H  \
) { \
    { k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0A, k0B, k0C, k0D, k0E, k0F, k0G, k0H }, \
    { k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1A, k1B, k1C, k1D, k1E, k1F, k1G, k1H }, \
    { k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2A, k2B, k2C, k2D, k2E, k2F, k2G, k2H }, \
    { k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k3A, k3B, k3C, k3D, k3E, k3F, k3G, k3H }, \
    { k40, k41, k42, k43, k44, k45, k46, k47, k48, k49, k4A, k4B, k4C, k4D, k4E, k4F, k4G, k4H }, \
    { k50, k51, k52, k53, k54, k55, k56, k57, k58, k59, k5A, k5B, k5C, k5D, k5E, k5F, k5G, k5H }, \
    { k60, k61, k62, k63, k64, k65, k66, k67, k68, k69, k6A, k6B, k6C, k6D, k6E, k6F, k6G, k6H }, \
    { k70, k71, k72, k73, k74, k75, k76, k77, k78, k79, k7A, k7B, k7C, k7D, k7E, k7F, k7G, k7H }  \
}

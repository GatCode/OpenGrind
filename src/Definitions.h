#define DISPLAY_ADDR 0x3C // I2C Address - use 0x3C or 0x3D depending on your display
#define ENC_CLK 2 // Encoder CLK Input Pin
#define ENC_DT 3 // Encoder DT Input Pin
#define ENC_SW 4 // Encoder Switch Input Pin
#define ENC_TOL 2 // 2 Clicks of Tolerance
#define ENC_SW_LONG_PRESS_DUR 5000 // Encoder Switch Long Press Activation Time in ms
#define ENC_THRESHOLD 2 // How responsive should the dose switching be?
#define START_BTN 5 // Start Button Input Pin
#define SSR 6 // Start Button Input Pins
#define SINGLEDOSE_DEFAULT_TIME 2.00 // Default Single Dose Time in S
#define DOUBLEDOSE_DEFAULT_TIME 4.00 // Default Double Dose Time in S
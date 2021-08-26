//Constant

#define LED_PWM_HIGH_BOUND 200  //pwm value for max brightness
#define LED_PWM_LOW_BOUND 255 //pwm value for min brightness   


//OLED display parameter
#define LED_BRIGHTNESS_FONT_XPOS 15
#define LED_BRIGHTNESS_FONT_YPOS 5

#define LED_METER_BAR_XPOS 7
#define LED_METER_BAR_YPOS 25
#define LED_METER_BAR_WIDTH 110
#define LED_METER_BAR_HEIGHT 1
#define LED_METER_BAR_CURSOR_W 3 //Width
#define LED_METER_BAR_CURSOR_H 7 //Height
#define LED_METER_BAR_SEPERATION_W 1 //Width
#define LED_METER_BAR_SEPERATION_H LED_METER_BAR_CURSOR_H //Height

// PIN map
#define ENCODER_S1_PIN 2 //Define you encoder connection pin Here
#define ENCODER_S2_PIN 3
#define ENCODER_KEY_PIN 10
#define LED_PIN 9
#define LED_EN_PIN 8


//EEPROM ADDRESS
#define EEPROM_LED_BRIGHTNESS_ADDR 0

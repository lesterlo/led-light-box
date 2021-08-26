// LED Light Box  (led-light-box.ino)
// Copyright (c) 2021 Lester Lo 
// MIT License

#include "config.h"

#include "logo.h"

// Include Libraries
#include <MsTimer2.h>
#include <NSEncoder.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>



NSEncoder_btn enc(ENCODER_S1_PIN, ENCODER_S2_PIN, ENCODER_KEY_PIN, 4, true);

Adafruit_SSD1306 oled(128, 32, &Wire, -1);//OLED 128x64 No Reset Pin

int led_brightness = 10; //CLip to 0-100%
bool light_status = false;
bool reset_brightness_flag = false;


//Timer isr
void isr_mstimer2()
{
    enc.btn_task();
}

//Set the easter egg
void showBitmap(void) {
  oled.clearDisplay();
  oled.drawBitmap(10, 0, pig_rabbit_logoBitmap, 128, 32, WHITE);
  oled.display();
  delay(3000);
}

void setup() {
    Serial.begin(115200);

    // OLED config
    if(!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    for(;;); // Don't proceed, loop forever
    }
    oled.clearDisplay();
    oled.display();

    //Setup the Pin Mode
    pinMode(LED_EN_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    

    // Endcoder setup

    // Set the button monitoring parameter
    enc.setBtnDebounceInterval(150);
    enc.setBtnPressInterval(500);
    enc.setBtnHoldInterval(1000);

    //You must setup this timer to enable button monitoring
    MsTimer2::set(enc.getBtnDebounceInterval(), isr_mstimer2); //Set 30 ms clock
    MsTimer2::start();

}

void loop() {
  
    NSEncoder_btn::BTN_STATE enc_btn;
  
    int enc_diff = 0; //The encoder diff counter
    // Handle Brightness
    if((enc_diff = enc.get_diffPosition()) != 0) //Get the position diff value
    {
        led_brightness += enc_diff; // Add the encoder position diff

        //Clip the led value to 0-100%
        led_brightness = led_brightness > 100 ? 100 : led_brightness;
        led_brightness = led_brightness <  0  ?  0 : led_brightness;
    }

    //Write the PWM value to the LED
    analogWrite(LED_PIN, map(led_brightness, 0, 100, LED_PWM_LOW_BOUND, LED_PWM_HIGH_BOUND));


    //Button Jobs
    enc_btn = enc.get_Button(); //Get the button status
    switch(enc_btn)
    {
        case NSEncoder_btn::OPEN:
        
        break;
        
        case NSEncoder_btn::PRESSED:
            if(light_status)
            {
                // Turn OFF the LED
                light_status = false;
                digitalWrite(LED_EN_PIN, LOW);
            }
            else
            {
                // Turn ON the LED
                light_status = true;
                digitalWrite(LED_EN_PIN, HIGH);
            }
   
        break;

        case NSEncoder_btn::RELEASED:
            //Reset the brightness to 50%
            led_brightness = 50; //DO the reset procedure
            reset_brightness_flag = false;
        break;
    
        case NSEncoder_btn::HELD:
            //Recognize the RESET action 
            reset_brightness_flag = true;
        break;
        
        case NSEncoder_btn::DOUBLE_PRESSED:
            showBitmap();
        break;
    }
    
    // OLED Jobs
    oled.clearDisplay(); //Clear the display first
    
    //Show the LED Brightness
    oled.setTextSize(2); // Draw 2X-scale text
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(LED_BRIGHTNESS_FONT_XPOS, LED_BRIGHTNESS_FONT_YPOS);
    //Show the message
    if(reset_brightness_flag)
    {   //Show the reset status
        oled.print(F("  RESET "));
    }
    else
    {   //Show the LED brightness value
        oled.print(F("LED: "));
        oled.print(led_brightness);
        oled.print(F("%"));
    }

    //Draw the oled outbound, show when led is disabled
    if(light_status == false)
        oled.drawRect(0, 0, 128, 32, SSD1306_WHITE);


    //Draw the brightness meter bar
    oled.fillRect(LED_METER_BAR_XPOS, LED_METER_BAR_YPOS, LED_METER_BAR_WIDTH, LED_METER_BAR_HEIGHT, SSD1306_WHITE);

    // Draw the meter bar seperation
    for(int i=0; i<=100; i+=20)
    {
        oled.fillRect(
            map(i,0,100,LED_METER_BAR_XPOS,LED_METER_BAR_XPOS+LED_METER_BAR_WIDTH), 
            LED_METER_BAR_YPOS-LED_METER_BAR_CURSOR_H/2, 
            LED_METER_BAR_SEPERATION_W,
            LED_METER_BAR_SEPERATION_H, 
            SSD1306_WHITE);
    }

    //Draw the meter cursor
    oled.fillRect(
        map(led_brightness,0,100,LED_METER_BAR_XPOS,LED_METER_BAR_XPOS+LED_METER_BAR_WIDTH), 
        LED_METER_BAR_YPOS-LED_METER_BAR_CURSOR_H/2, 
        LED_METER_BAR_CURSOR_W,
        LED_METER_BAR_CURSOR_H, 
        SSD1306_WHITE);

    //Display the OLED drawing
    oled.display();
}
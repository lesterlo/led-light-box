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

int led_brightness = 30; //CLip to 0-100%
bool light_status = false;


//Timer isr
void isr_mstimer2()
{
    enc.btn_task();
}

void showBitmap(void) {
  oled.clearDisplay();
  oled.drawBitmap(0, 0, pig_rabbit_logoBitmap, 128, 32, WHITE);
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

    pinMode(LED_EN_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    

  // Endcoder setup

  // Set the button monitoring parameter
  // Commented code for using default interval in this example

    enc.setBtnDebounceInterval(80);
    enc.setBtnPressInterval(500);
    enc.setBtnHoldInterval(1000);

    //You must setup this timer to enable button monitoring
    MsTimer2::set(enc.getBtnDebounceInterval(), isr_mstimer2); //Set 30 ms clock
    MsTimer2::start();

}

void loop() {
  
    NSEncoder_btn::BTN_STATE enc_btn;
  
    int enc_diff = 0;
    // Handle Brightness
    if((enc_diff = enc.get_diffPosition()) != 0) //Get the position diff value
    {
        led_brightness += enc_diff; // Add diff

    //Clip the led value to 0-100%
        led_brightness = led_brightness > 100 ? 100 : led_brightness;
        led_brightness = led_brightness <  0  ?  0 : led_brightness;

    }

    analogWrite(LED_PIN, map(led_brightness, 0, 100, LED_PWM_LOW_BOUND, LED_PWM_HIGH_BOUND));


    //Button Jobs

    enc_btn = enc.get_Button();
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
        led_brightness = 50;
        break;
    
        case NSEncoder_btn::HELD:

        break;
        
        case NSEncoder_btn::DOUBLE_PRESSED:
        showBitmap();
        break;
    }
    
    // OLED Jobs
    oled.clearDisplay();
    
    //Show the LED Brightness
    oled.setTextSize(2); // Draw 2X-scale text
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(0, 0);
    oled.print(F("LED: "));
    oled.print(led_brightness);
    oled.print(F("%"));

    //TEMP
    //  if(light_status)
    //     oled.setTextColor(SSD1306_WHITE);
    // else
    //     oled.setTextColor(SSD1306_BLACK, SSD1306_WHITE);


    oled.display();
}
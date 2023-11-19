#include <Arduino.h>
#include <FastLED.h>

#define LEDpin 5
#define LEDnumber 22

CRGB leds[LEDnumber];

void flashMagenta();
void halfColours();
void showState(string currentState);

void setup(){
    Serial.begin(9600);
    FastLED.addLeds<WS2812, LEDpin,GRB>(leds, LEDnumber);
    pinMode(LEDpin,OUTPUT);
}

void loop(){
    //if intaking
    flashMagenta();

}

void flashMagenta(){
    for(i=0; i<=LEDnumber; i++){
        leds[i] = CRGB(255,0,255);
    }
    static unsigned long magentaMillis = millis();
    if (millis() - magentaMillis > 50){
    digitalWrite(magentaMillis,!digitalRead(LEDpin));
    magentaMillis = millis();
    }
}
void halfColours(){//half the leds with red and blue
    for (i=0; i<=LEDnumber/2;i++){
        leds[i] = CRGB(255,0,0);
        leds[i + LEDnumber/2]=CRGB(0,0,255);
    }
    delay(wait);
    FastLED.show();
}

//swapping states
enum class LEDstates : uint8_t
{
    HALF,
    FLASHING,
    PULSING,
}

static LEDstates currentState = LEDstates::HALF;

switch(currentState){
    case LEDstates::HALF:
        showState("HALF STATE");
        //if intaking{currentState = LEDstates::FLASHING}
        break;
    
    case LEDstates::FLASHING:
        showState("FLASHING STATE");
        //if
        break;
}

void showState(String currentState){
    static String previousState = "";

    if(currentState!= previousState)
    {
        Serial.print(currentState);
        previousState = currentState;
    }
}
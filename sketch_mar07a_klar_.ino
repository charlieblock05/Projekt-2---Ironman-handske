// Nödvändiga bibliotek
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Definera den pin som används för NeoPixel ringen
#define LED_PIN 8

// Definara antalet leds i ringen
#define LED_COUNT 24


Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Definiera konstanter för pinsen som används för lasern och knappen
const int laserPin = 13;
const int buttonPin = 7;

// Initiera en variabel för den nuvarande LED indexen
int n = 0;

// Kör vid start
void setup() {
  

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  // Initiera NeoPixel ringen
  strip.begin();
  
  // rensar existerande pixel färger
  strip.show();
  
  // sätter ljusstyrkan av ledsen till 10%
  strip.setBrightness(10);

  // Initiera laser och knapp pinsen
  pinMode(laserPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

// Detta är loopen som kommer gå hela tiden, antingen trycks knappen ned och då lyser lasern och theater chase 
// animationen sker, eller så går det en vit ring runt konstant.
void loop() {

  // Det under kommer loopa om knappen trycks ned (low)
  if (digitalRead(buttonPin) == LOW) {
    
    // Kallar på theater chase animationen
    theaterChase(strip.Color(255, 0, 0), 15);
    
    // rensar ringen
    strip.clear();
  }
  else {
    
    strip.setPixelColor(n, strip.Color(127, 127, 127));  // sätter färgen av nästa led till vit
  strip.show(); // uppdaterar NeoPixeloch visar nya led animationen
  n++;
  delay(50); // bestämmer snabbheten av ringen 

    // Detta gör så att när led ringen har nått slutet så rensas ringen och börjar om iegn
    if (n > 23) {
      n = 0;
      strip.clear();
    }

    // Kollar status av knappen och sätter på eller av knappen
    int digitalVal = digitalRead(buttonPin);

    if (HIGH == digitalVal) {
      digitalWrite(laserPin, LOW);
    }
    else {
      digitalWrite(laserPin, HIGH);
    }
  }
}

// Den här funktionen sätter alla pixlar till rätt färg och rätt delay tid
void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);          //  Välj färg
    strip.show();                           //  Uppdatera ringen
    delay(wait);                            //  Paus
  }
}


// Den här funktionen utför "theather chase" animationen som aktiveras när knappen hålls ned
void theaterChase(uint32_t color, int wait) {
  for (int a = 0; a < 5; a++) {                     // Repetera blinkandet 5 gånger
    for (int b = 0; b < 3; b++) {
      strip.clear();                                // Stänger av pixlarna
      for (int c = b; c < strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color);              // Sätter färg
      }
      strip.show();                                 // Updaterar ringen till ny amination
      delay(wait);                                  // Paus
    }
  }
}

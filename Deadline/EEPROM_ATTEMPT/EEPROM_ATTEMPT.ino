// imported all the libraries regardless of whether or not they are being used.
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <EEPROM.h>
#include <avr/eeprom.h>
// #include <TimerOne.h>

// REMEMBER TO MAKE A VARIABLE TO STORE THE LAST SEEN STATE AS A VARIABLE STORED IN EEPROM SO THAT WHEN ARDUINO IS UNPLUGGED LAST SCREEN IS SEEN (ENDGAME)

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

//defninition of states for the fsm of the arduino:
//Menu States
#define MENU 0

//Floor Select States
#define FLOOR_SELECT 1
#define FLOOR_GROUND 2
#define FLOOR_FIRST 3


// Bedroom 1 States
#define BEDROOM_1 4
#define BEDROOM_1_LIGHT 5
#define BEDROOM_1_LIGHT_LEVEL 6
#define BEDROOM_1_LIGHT_ON_HH 7
#define BEDROOM_1_LIGHT_ON_MM 8
#define BEDROOM_1_LIGHT_OFF 9
#define BEDROOM_1_LIGHT_OFF_HH 10
#define BEDROOM_1_LIGHT_OFF_MM 11
#define BEDROOM_1_HEAT 12
#define BEDROOM_1_HEAT_LEVEL 13
#define BEDROOM_1_HEAT_ON 14
#define BEDROOM_1_HEAT_ON_HH 15
#define BEDROOM_1_HEAT_ON_MM 16
#define BEDROOM_1_HEAT_OFF 17
#define BEDROOM_1_HEAT_OFF_HH 18
#define BEDROOM_1_HEAT_OFF_MM 19

//Bedroom 2 States
#define BEDROOM_2 20
#define BEDROOM_2_LIGHT 21
#define BEDROOM_2_LIGHT_LEVEL 22
#define BEDROOM_2_LIGHT_ON_HH 23
#define BEDROOM_2_LIGHT_ON_MM 24
#define BEDROOM_2_LIGHT_OFF 25
#define BEDROOM_2_LIGHT_OFF_HH 26
#define BEDROOM_2_LIGHT_OFF_MM 27
#define BEDROOM_2_HEAT 28
#define BEDROOM_2_HEAT_LEVEL 29
#define BEDROOM_2_HEAT_ON 30
#define BEDROOM_2_HEAT_ON_HH 31
#define BEDROOM_2_HEAT_ON_MM 32
#define BEDROOM_2_HEAT_OFF 33
#define BEDROOM_2_HEAT_OFF_HH 34
#define BEDROOM_2_HEAT_OFF_MM 35

// Bathroom States
#define BATHROOM 36
#define BATHROOM_LIGHT 37
#define BATHROOM_LIGHT_LEVEL 38
#define BATHROOM_LIGHT_ON_HH 39
#define BATHROOM_LIGHT_ON_MM 40
#define BATHROOM_LIGHT_OFF 41
#define BATHROOM_LIGHT_OFF_HH 42
#define BATHROOM_LIGHT_OFF_MM 43
#define BATHROOM_HEAT 44
#define BATHROOM_HEAT_LEVEL 45
#define BATHROOM_HEAT_ON 46
#define BATHROOM_HEAT_ON_HH 47
#define BATHROOM_HEAT_ON_MM 48
#define BATHROOM_HEAT_OFF 49
#define BATHROOM_HEAT_OFF_HH 50
#define BATHROOM_HEAT_OFF_MM 51

// Kitchen States
#define KITCHEN 52
#define KITCHEN_LIGHT 53
#define KITCHEN_LIGHT_LEVEL 54
#define KITCHEN_LIGHT_ON_HH 55
#define KITCHEN_LIGHT_ON_MM 56
#define KITCHEN_LIGHT_OFF 57
#define KITCHEN_LIGHT_OFF_HH 58
#define KITCHEN_LIGHT_OFF_MM 59
#define KITCHEN_HEAT 60
#define KITCHEN_HEAT_LEVEL 61
#define KITCHEN_HEAT_ON 62
#define KITCHEN_HEAT_ON_HH 63
#define KITCHEN_HEAT_ON_MM 64
#define KITCHEN_HEAT_OFF 65
#define KITCHEN_HEAT_OFF_HH 66
#define KITCHEN_HEAT_OFF_MM 67

// Hall States
#define HALL 68
#define HALL_LIGHT 69
#define HALL_LIGHT_LEVEL 70
#define HALL_LIGHT_ON_HH 71
#define HALL_LIGHT_ON_MM 72
#define HALL_LIGHT_OFF 73
#define HALL_LIGHT_OFF_HH 74
#define HALL_LIGHT_OFF_MM 75
#define HALL_HEAT 76
#define HALL_HEAT_LEVEL 77
#define HALL_HEAT_ON 78
#define HALL_HEAT_ON_HH 79
#define HALL_HEAT_ON_MM 80
#define HALL_HEAT_OFF 81
#define HALL_HEAT_OFF_HH 82
#define HALL_HEAT_OFF_MM 83

// Living Room States
#define LIVING_ROOM 84
#define LIVING_ROOM_LIGHT 85
#define LIVING_ROOM_LIGHT_LEVEL 86
#define LIVING_ROOM_LIGHT_ON_HH 87
#define LIVING_ROOM_LIGHT_ON_MM 88
#define LIVING_ROOM_LIGHT_OFF 89
#define LIVING_ROOM_LIGHT_OFF_HH 90
#define LIVING_ROOM_LIGHT_OFF_MM 91
#define LIVING_ROOM_HEAT 92
#define LIVING_ROOM_HEAT_LEVEL 93
#define LIVING_ROOM_HEAT_ON 94
#define LIVING_ROOM_HEAT_ON_HH 95
#define LIVING_ROOM_HEAT_ON_MM 96
#define LIVING_ROOM_HEAT_OFF 97
#define LIVING_ROOM_HEAT_OFF_HH 98
#define LIVING_ROOM_HEAT_OFF_MM 99





void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  Serial.println("BASIC");

}

static byte name0x0[] = { B00000, B00000, B00010, B00100, B01000, B00100, B00010, B00000 };
static byte name0x1[] = { B00000, B00000, B01000, B00100, B00010, B00100, B01000, B00000 };
static byte name0x2[] = { B00000, B00000, B00100, B00100, B00100, B11111, B01110, B00100 };
static byte name0x3[] = { B00100, B01110, B11111, B00100, B00100, B00100, B00000, B00000 };

uint8_t lightLevel = 0;
uint8_t heatLevel = 0;
uint8_t HH = 0;
uint8_t MM = 0;

uint8_t currentState = 0;

void loop() {


  // put your main code here, to run repeatedly:
  uint8_t b = lcd.readButtons();
  lcd.createChar(0, name0x0);
  lcd.createChar(1, name0x1);
  lcd.createChar(2, name0x2);
  lcd.createChar(3, name0x3);





  // temporary switch...case structure:
  switch (currentState) {

    // Menu State
    case MENU:
      lcd.setCursor(6, 0);
      lcd.print("MENU");
      lcd.setCursor(0, 1);
      lcd.write(0);
      lcd.setCursor(15, 1);
      lcd.write(1);
      lcd.setCursor(6, 1);
      lcd.print("House");
      if (b & BUTTON_SELECT) {
        currentState = FLOOR_SELECT;
        lcd.clear();
      }
      break;


    // Floor Selection State quite messy from here on separating each scenario into invididual states
    case FLOOR_SELECT:
      lcd.setCursor(0, 1);
      lcd.write(0);
      lcd.setCursor(15, 1);
      lcd.write(1);
      lcd.setCursor(2, 0);
      lcd.print("SELECT FLOOR");
      lcd.setCursor(1, 1);
      lcd.print("GRND");
      lcd.setCursor(11, 1);
      lcd.print("FRST");
      if (b & BUTTON_LEFT) {
        lcd.clear();
        currentState = FLOOR_GROUND;

      }
      else if (b & BUTTON_RIGHT) {
        lcd.clear();
        currentState = FLOOR_FIRST;
      }
      break;

    case FLOOR_GROUND: // kitchen
      lcd.setCursor(0, 1);
      lcd.write(0);
      lcd.setCursor(15, 1);
      lcd.write(1);
      lcd.setCursor(2, 0);
      lcd.print("GROUND FLOOR");
      lcd.setCursor(1, 1);
      lcd.print("KIT");
      lcd.setCursor(7, 1);
      lcd.print("HLL");
      lcd.setCursor(12, 1);
      lcd.print("LRM");
      if (b & BUTTON_SELECT) {
        lcd.clear();
        currentState = HALL;
        EEPROM.get(0, lightLevel);
        EEPROM.get(5, heatLevel);

      }
      else if (b & BUTTON_RIGHT) {
        lcd.clear();
        currentState = LIVING_ROOM;
        EEPROM.get(20, lightLevel);
        EEPROM.get(25, heatLevel);

      }
      else if (b & BUTTON_LEFT) {
        lcd.clear();
        currentState = KITCHEN;
        EEPROM.get(10, lightLevel);
        EEPROM.get(15, heatLevel);

      }
      break;

    case FLOOR_FIRST: // BEDROOM 1 SELECT
      lcd.setCursor(0, 1);
      lcd.write(0);
      lcd.setCursor(15, 1);
      lcd.write(1);
      lcd.setCursor(2, 0);
      lcd.print("FIRST FLOOR");
      lcd.setCursor(1, 1);
      lcd.print("BDR2");
      lcd.setCursor(6, 1);
      lcd.print("BDR1");
      lcd.setCursor(11, 1);
      lcd.print("BATH");
      if (b & BUTTON_SELECT) {
        lcd.clear();
        currentState = BEDROOM_1;
        EEPROM.get(30, lightLevel);
        EEPROM.get(35, heatLevel);

      }
      else if (b & BUTTON_RIGHT) {
        lcd.clear();
        currentState = BATHROOM;
        EEPROM.get(50, lightLevel);
        EEPROM.get(55, heatLevel);

      }
      else if (b & BUTTON_LEFT) {
        lcd.clear();
        currentState = BEDROOM_2;
        EEPROM.get(40, lightLevel);
        EEPROM.get(45, heatLevel);

      }
      break;

    case HALL:
      lcd.setCursor(0, 1);
      lcd.write(0);
      lcd.setCursor(15, 1);
      lcd.write(1);
      lcd.setCursor(6, 0);
      lcd.print("HALL");
      lcd.setCursor(1, 1);
      lcd.print("Light");
      lcd.setCursor(11, 1);
      lcd.print("Heat");
      if (b & BUTTON_RIGHT) {
        lcd.clear();
        currentState = HALL_HEAT;

      }
      else if (b & BUTTON_LEFT) {
        lcd.clear();
        currentState = HALL_LIGHT;

      }
      break;

    case HALL_LIGHT:
      lcd.setCursor(3, 0);
      lcd.print("HALL LIGHT");
      lcd.setCursor(0, 1);
      lcd.write(0);
      lcd.setCursor(15, 1);
      lcd.write(1);
      lcd.setCursor(1, 1);
      lcd.print("OFF");
      lcd.setCursor(13, 1);
      lcd.print("ON");
      lcd.setCursor(6, 1);
      lcd.print("LEVEL");

      if (b & BUTTON_SELECT) {
        lcd.clear();
        currentState = HALL_LIGHT_LEVEL;

      }
      else if (b & BUTTON_RIGHT) {
        lcd.clear();
        currentState = HALL_LIGHT_ON_HH;
        EEPROM.get(1, HH);
        EEPROM.get(2, MM);
      }
      else if (b & BUTTON_LEFT) {
        lcd.clear();
        currentState = HALL_LIGHT_OFF_HH;
        EEPROM.get(3, HH);
        EEPROM.get(4, MM);
      }
      break;

    case HALL_LIGHT_LEVEL:
      lcd.setCursor(3, 0);
      lcd.print("HALL LIGHT");
      // bottom row setup
      lcd.setCursor(0, 1);
      lcd.write(2);
      lcd.setCursor(15, 1);
      lcd.write(3);
      lcd.setCursor(6, 1);
      lcd.print(lightLevel); lcd.print("%");
      if (b & BUTTON_SELECT) {
        EEPROM.put(0, lightLevel);
        Serial.print("Ground/Hall/Light/Main/Level:"); Serial.println(lightLevel);
        lcd.clear();
      }

      if (b & BUTTON_UP) {
        lightLevel++;
        if (lightLevel < 100 && lightLevel > 0) {
          lcd.setCursor(6, 1);
          lcd.print("    ");
          lcd.setCursor(6, 1);
          lcd.print(lightLevel); lcd.print("%");
        }
        else if (lightLevel >= 100) {
          lightLevel = 100;
        }
      }
      else if (b & BUTTON_DOWN) {
        lightLevel--;
        if (lightLevel <= 255 && lightLevel >= 101) {
          lightLevel = 0;
        }
        if (lightLevel > 0) {
          lcd.setCursor(6, 1);
          lcd.print("    ");
          lcd.setCursor(6, 1);
          lcd.print(lightLevel); lcd.print("%");
        }
      }

      break;

    // Light ON state HOURS
    case HALL_LIGHT_ON_HH:
      lcd.setCursor(1, 0);
      lcd.print("HALL LIGHT ON");
      lcd.setCursor(0, 1);
      lcd.write(2);
      lcd.setCursor(15, 1);
      lcd.write(3);
      lcd.setCursor(4, 1);
      lcd.write(1);
      lcd.setCursor(5, 1);
      if (HH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(HH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(HH);
      }
      lcd.print(":");
      if (MM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(MM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(MM);
      }
      if (b & BUTTON_UP) {
        HH++;
        if (HH >= 10 && HH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(HH);
          // delay(250);
        }
        else if (HH < 9 && HH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(HH);
          // delay(250);
        }
        else if (HH >= 24) {
          HH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        HH--;
        if (HH <= 255 && HH >= 24) {
          HH = 23;
        }
        if (HH >= 10 && HH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(HH);
          // delay(250);
        }
        else if (HH < 9 && HH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(HH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        EEPROM.put(1, HH);
        lcd.clear();
        currentState = HALL_LIGHT_ON_MM;




      }
      break;


    // Light ON state MINUTES
    case HALL_LIGHT_ON_MM:
      lcd.setCursor(1, 0);
      lcd.print("HALL LIGHT ON");
      lcd.setCursor(0, 1);
      lcd.write(2);
      lcd.setCursor(15, 1);
      lcd.write(3);
      lcd.setCursor(10, 1);
      lcd.write(0);
      lcd.setCursor(5, 1);
      if (HH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(HH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(HH);
      }
      lcd.print(":");
      if (MM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(MM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(MM);
      }
      if (b & BUTTON_UP) {
        MM++;
        if (MM >= 10 && MM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(MM);
          // delay(250);
        }
        else if (MM < 9 && MM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(MM);
          // delay(250);
        }
        else if (MM > 60) {
          MM = 0;
          MM++;
        }
      }
      if (b & BUTTON_DOWN) {
        MM--;
        if (MM <= 255 && MM >= 60) {
          MM = 59;
        }
        if (MM >= 10 && MM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(MM);
          // delay(250);
        }
        else if (MM < 9 && MM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(MM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        lcd.clear();
        currentState = HALL_LIGHT_ON_HH;
        EEPROM.put(2, MM);

      }
      if (b & BUTTON_SELECT) {
        EEPROM.put(1, HH);
        EEPROM.put(2, MM);
        Serial.print("Ground/Hall/Light/Main/On:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
        lcd.clear();
      }

      break;

    // Light OFF state HOURS
    case HALL_LIGHT_OFF_HH:
      lcd.setCursor(1, 0);
      lcd.print("HALL LIGHT OFF");
      lcd.setCursor(0, 1);
      lcd.write(2);
      lcd.setCursor(15, 1);
      lcd.write(3);
      lcd.setCursor(4, 1);
      lcd.write(1);
      lcd.setCursor(5, 1);
      if (HH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(HH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(HH);
      }
      lcd.print(":");
      if (MM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(MM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(MM);
      }
      if (b & BUTTON_UP) {
        HH++;
        if (HH >= 10 && HH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(HH);
          // delay(250);
        }
        else if (HH < 9 && HH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(HH);
          // delay(250);
        }
        else if (HH >= 24) {
          HH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        HH--;
        if (HH <= 255 && HH >= 24) {
          HH = 23;
        }
        if (HH >= 10 && HH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(HH);
          // delay(250);
        }
        else if (HH < 9 && HH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(HH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        lcd.clear();
        currentState = HALL_LIGHT_OFF_MM;
        EEPROM.put(3, HH);



      }
      break;


    // Light OFF state MINUTES
    case HALL_LIGHT_OFF_MM:
      lcd.setCursor(1, 0);
      lcd.print("HALL LIGHT OFF");
      lcd.setCursor(0, 1);
      lcd.write(2);
      lcd.setCursor(15, 1);
      lcd.write(3);
      lcd.setCursor(10, 1);
      lcd.write(0);
      lcd.setCursor(5, 1);
      if (HH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(HH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(HH);
      }
      lcd.print(":");
      if (MM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(MM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(MM);
      }
      if (b & BUTTON_UP) {
        MM++;
        if (MM >= 10 && MM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(MM);
          // delay(250);
        }
        else if (MM < 9 && MM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(MM);
          // delay(250);
        }
        else if (MM > 59) {
          MM = 0;
          MM++;
        }
      }
      if (b & BUTTON_DOWN) {
        MM--;
        if (MM <= 255 && MM >= 60) {
          MM = 59;
        }
        if (MM >= 10 && MM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(MM);
          // delay(250);
        }
        else if (MM < 9 && MM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(MM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        lcd.clear();
        currentState = HALL_LIGHT_OFF_HH;
        EEPROM.put(4, MM);


        if (b & BUTTON_SELECT) {
          EEPROM.put(3, HH);
          EEPROM.put(4, MM);
          Serial.print("Ground/Hall/Light/Main/Off:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
          lcd.clear();
        }
        break;


      // Heat ON, OFF or LEVEL selection state
      case HALL_HEAT:
        lcd.setCursor(0, 0);
        lcd.print("HALL HEAT");
        lcd.setCursor(0, 1);
        lcd.write(0);
        lcd.setCursor(15, 1);
        lcd.write(1);
        lcd.setCursor(1, 1);
        lcd.print("OFF");
        lcd.setCursor(13, 1);
        lcd.print("ON");
        lcd.setCursor(6, 1);
        lcd.print("LEVEL");


        if (b & BUTTON_SELECT) {
          lcd.clear();
          currentState = HALL_HEAT_LEVEL;

        }
        else if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = HALL_HEAT_ON_HH;
          EEPROM.get(6, HH);
          EEPROM.get(7, MM);

        }
        else if (b & BUTTON_LEFT) {
          lcd.clear();
          currentState = HALL_HEAT_OFF_HH;
          EEPROM.get(8, HH);
          EEPROM.get(9, MM);

        }
        break;

      // Heat LEVEL state
      case HALL_HEAT_LEVEL:
        lcd.setCursor(3, 0);
        lcd.print("HALL HEAT");
        // bottom row setup
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(6, 1);
        lcd.print(heatLevel); lcd.print("%");
        if (b & BUTTON_SELECT) {
          EEPROM.put(5, heatLevel);
          Serial.print("Ground/Hall/Heat/Main/Level:"); Serial.println(heatLevel);
          lcd.clear();
        }

        if (b & BUTTON_UP) {
          heatLevel++;
          if (heatLevel < 100 && heatLevel > 0) {
            lcd.setCursor(6, 1);
            lcd.print("    ");
            lcd.setCursor(6, 1);
            lcd.print(heatLevel); lcd.print("%");
          }
          else if (heatLevel >= 100) {
            heatLevel = 100;
          }
        }
        else if (b & BUTTON_DOWN) {
          heatLevel--;
          if (heatLevel <= 255 && heatLevel >= 101) {
            heatLevel = 0;
          }
          if (heatLevel > 0) {
            lcd.setCursor(6, 1);
            lcd.print("   ");
            lcd.setCursor(6, 1);
            lcd.print(heatLevel); lcd.print("%");
          }
        }
        break;


      // Heat ON state HOURS
      case HALL_HEAT_ON_HH:
        lcd.setCursor(1, 0);
        lcd.print("HALL HEAT ON");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(4, 1);
        lcd.write(1);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          HH++;
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH >= 24) {
            HH = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          HH--;
          if (HH <= 255 && HH >= 24) {
            HH = 23;
          }
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }



        }
        if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = HALL_HEAT_ON_MM;
          EEPROM.put(6, HH);


        }
        break;

      // Heat ON state MINUTES
      case HALL_HEAT_ON_MM:
        lcd.setCursor(1, 0);
        lcd.print("HALL HEAT ON");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.write(0);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          MM++;
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM >= 60) {
            MM = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          MM--;

          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
          if (MM <= 255 && MM >= 60) {
            MM = 59;
          }
        }
        if (b & BUTTON_LEFT) {
          lcd.clear();
          currentState = HALL_HEAT_ON_HH;
          EEPROM.put(7, MM);

        }
        if (b & BUTTON_SELECT) {
          EEPROM.put(6, HH);
          EEPROM.put(7, MM);
          Serial.print("Ground/Hall/Heat/Main/On:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
          lcd.clear();
        }
        break;

      // Heat OFF state HOURS
      case HALL_HEAT_OFF_HH:
        lcd.setCursor(1, 0);
        lcd.print("HALL HEAT OFF");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(4, 1);
        lcd.write(1);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          HH++;
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH >= 24) {
            HH = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          HH--;
          if (HH <= 255 && HH >= 24) {
            HH = 23;
          }
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }


        }
        if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = HALL_LIGHT_OFF_MM;
          EEPROM.put(8, HH);


        }
        break;

      // Heat OFF state MINUTES
      case HALL_HEAT_OFF_MM:
        lcd.setCursor(1, 0);
        lcd.print("HALL HEAT OFF");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.write(0);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          MM++;
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM > 59) {
            MM = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          MM--;
          if (MM <= 255 && MM >= 60) {
            MM = 59;
          }
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
        }
        if (b & BUTTON_LEFT) {
          lcd.clear();
          currentState = HALL_LIGHT_OFF_HH;
          EEPROM.put(9, MM);

        }
        if (b & BUTTON_SELECT) {
          EEPROM.put(8, HH);
          EEPROM.put(9, MM);
          Serial.print("Ground/Hall/Heat/Main/Off:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
          lcd.clear();
        }
        break;

      case KITCHEN:
        lcd.setCursor(0, 1);
        lcd.write(0);
        lcd.setCursor(15, 1);
        lcd.write(1);
        lcd.setCursor(4, 0);
        lcd.print("KITCHEN");
        lcd.setCursor(1, 1);
        lcd.print("Light");
        lcd.setCursor(11, 1);
        lcd.print("Heat");
        if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = KITCHEN_HEAT;

        }
        else if (b & BUTTON_LEFT) {
          lcd.clear();
          currentState = KITCHEN_LIGHT;

        }
        break;

      case KITCHEN_LIGHT:
        lcd.setCursor(1, 0);
        lcd.print("KITCHEN LIGHT");
        lcd.setCursor(0, 1);
        lcd.write(0);
        lcd.setCursor(15, 1);
        lcd.write(1);
        lcd.setCursor(1, 1);
        lcd.print("OFF");
        lcd.setCursor(13, 1);
        lcd.print("ON");
        lcd.setCursor(6, 1);
        lcd.print("LEVEL");


        if (b & BUTTON_SELECT) {
          currentState = KITCHEN_LIGHT_LEVEL;

        }
        else if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = KITCHEN_LIGHT_ON_HH;
          EEPROM.get(11, HH);
          EEPROM.get(12, MM);

        }
        else if (b & BUTTON_LEFT) {
          lcd.clear();
          currentState = KITCHEN_LIGHT_OFF_HH;
          EEPROM.get(13, HH);
          EEPROM.get(14, MM);

        }
        break;

      case KITCHEN_LIGHT_LEVEL:
        lcd.setCursor(1, 0);
        lcd.print("KITCHEN LIGHT");
        // bottom row setup
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(6, 1);
        lcd.print(lightLevel); lcd.print("%");
        if (b & BUTTON_SELECT) {
          EEPROM.put(10, lightLevel);
          Serial.print("Ground/Kitchen/Light/Main/Level:"); Serial.println(lightLevel);
          lcd.clear();
        }

        if (b & BUTTON_UP) {
          lightLevel++;
          if (lightLevel < 101) {
            lcd.setCursor(6, 1);
            lcd.print("    ");
            lcd.setCursor(6, 1);
            lcd.print(lightLevel); lcd.print("%");
          }
          else {
            lightLevel = 100;
          }
        }
        else if (b & BUTTON_DOWN) {
          lightLevel--;
          if (lightLevel <= 255 && lightLevel >= 101) {
            lightLevel = 0;
          }
          if (lightLevel > -1) {
            lcd.setCursor(6, 1);
            lcd.print("    ");
            lcd.setCursor(6, 1);
            lcd.print(lightLevel); lcd.print("%");
          }
        }
        break;

      // Light ON state HOURS
      case KITCHEN_LIGHT_ON_HH:
        lcd.setCursor(1, 0);
        lcd.print("KTCHN LIGHT ON");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(4, 1);
        lcd.write(1);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          HH++;
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH >= 24) {
            HH = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          HH--;
          if (HH <= 255 && HH >= 24) {
            HH = 23;
          }
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }


        }
        if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = KITCHEN_LIGHT_ON_MM;
          EEPROM.put(11, HH);


        }
        break;


      // Light ON state MINUTES
      case KITCHEN_LIGHT_ON_MM:
        lcd.setCursor(1, 0);
        lcd.print("KTCHN LIGHT ON");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.write(0);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          MM++;
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM > 60) {
            MM = 0;
            MM++;
          }
        }
        if (b & BUTTON_DOWN) {
          MM--;
          if (MM <= 255 && MM >= 60) {
            MM = 59;
          }
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
        }
        if (b & BUTTON_LEFT) {
          lcd.clear();
          currentState = KITCHEN_LIGHT_ON_HH;
          EEPROM.put(12, MM);

        }
        if (b & BUTTON_SELECT) {
          EEPROM.put(11, HH);
          EEPROM.put(12, MM);
          Serial.print("Ground/Kitchen/Light/Main/On:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
          lcd.clear();
        }
        break;

      // Light OFF state HOURS
      case KITCHEN_LIGHT_OFF_HH:
        lcd.setCursor(1, 0);
        lcd.print("KTCHN LIGHT OFF");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(4, 1);
        lcd.write(1);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          HH++;
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH >= 24) {
            HH = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          HH--;
          if (HH <= 255 && HH >= 24) {
            HH = 23;
          }
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }


        }
        if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = KITCHEN_LIGHT_OFF_MM;
          EEPROM.put(13, HH);


        }
        break;


      // Light OFF state MINUTES
      case KITCHEN_LIGHT_OFF_MM:
        lcd.setCursor(1, 0);
        lcd.print("KTCHN LIGHT OFF");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.write(0);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          MM++;
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM > 59) {
            MM = 0;
            MM++;
          }
        }
        if (b & BUTTON_DOWN) {
          MM--;
          if (MM <= 255 && MM >= 60) {
            MM = 59;
          }
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
        }
        if (b & BUTTON_LEFT) {
          currentState = KITCHEN_LIGHT_OFF_HH;
          EEPROM.put(14, MM);

        }
        if (b & BUTTON_SELECT) {
          EEPROM.put(13, HH);
          EEPROM.put(14, MM);
          Serial.print("Ground/Kitchen/Light/Main/Off:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
          lcd.clear();
        }
        break;


      // Heat ON, OFF or LEVEL selection state
      case KITCHEN_HEAT:
        lcd.setCursor(0, 0);
        lcd.print("BEDROOM 1 HEAT");
        lcd.setCursor(0, 1);
        lcd.write(0);
        lcd.setCursor(15, 1);
        lcd.write(1);
        lcd.setCursor(1, 1);
        lcd.print("OFF");
        lcd.setCursor(13, 1);
        lcd.print("ON");
        lcd.setCursor(6, 1);
        lcd.print("LEVEL");


        if (b & BUTTON_SELECT) {
          lcd.clear();
          currentState = KITCHEN_HEAT_LEVEL;

        }
        else if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = KITCHEN_HEAT_ON_HH;
          EEPROM.get(16, HH);
          EEPROM.get(17, MM);

        }
        else if (b & BUTTON_LEFT) {
          lcd.clear();
          currentState = KITCHEN_HEAT_OFF_HH;
          EEPROM.get(18, HH);
          EEPROM.get(19, MM);

        }
        break;



      // Heat LEVEL state
      case KITCHEN_HEAT_LEVEL:
        lcd.setCursor(0, 0);
        lcd.print("KITCHEN HEAT");
        // bottom row setup
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(7, 1);
        lcd.print(heatLevel); lcd.print("%");
        if (b & BUTTON_SELECT) {
          EEPROM.put(15, heatLevel);
          Serial.print("Ground/Kitchen/Heat/Main/Level:"); Serial.println(heatLevel);
          lcd.clear();
        }

        if (b & BUTTON_UP) {
          heatLevel++;
          if (heatLevel < 101) {
            lcd.setCursor(6, 1);
            lcd.print("    ");
            lcd.setCursor(6, 1);
            lcd.print(heatLevel); lcd.print("%");
          }
          else {
            heatLevel = 100;
          }
        }
        else if (b & BUTTON_DOWN) {
          heatLevel--;
          if (heatLevel <= 255 && heatLevel >= 101) {
            heatLevel = 0;
          }
          if (heatLevel > -1) {
            lcd.setCursor(6, 1);
            lcd.print("   ");
            lcd.setCursor(6, 1);
            lcd.print(heatLevel); lcd.print("%");
          }
        }
        break;


      // Heat ON state HOURS
      case KITCHEN_HEAT_ON_HH:
        lcd.setCursor(1, 0);
        lcd.print("KTCHN HEAT ON");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(4, 1);
        lcd.write(1);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          HH++;
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH >= 24) {
            HH = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          HH--;
          if (HH <= 255 && HH >= 24) {
            HH = 23;
          }
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }


        }
        if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = KITCHEN_LIGHT_ON_MM;
          EEPROM.put(16, HH);


        }
        break;

      // Heat ON state MINUTES
      case KITCHEN_HEAT_ON_MM:
        lcd.setCursor(1, 0);
        lcd.print("KTCHN HEAT ON");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.write(0);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          MM++;
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM > 59) {
            MM = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          MM--;
          if (MM <= 255 && MM >= 60) {
            MM = 59;
          }
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
        }
        if (b & BUTTON_LEFT) {
          lcd.clear();
          currentState = KITCHEN_LIGHT_ON_HH;
          EEPROM.put(17, MM);

        }
        if (b & BUTTON_SELECT) {
          EEPROM.put(16, HH);
          EEPROM.put(17, MM);
          Serial.print("Ground/Kitchen/Heat/Main/On:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
          lcd.clear();
        }
        break;

      // Heat OFF state HOURS
      case KITCHEN_HEAT_OFF_HH:
        lcd.setCursor(1, 0);
        lcd.print("KTCHN HEAT OFF");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(4, 1);
        lcd.write(1);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          HH++;
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH >= 24) {
            HH = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          HH--;
          if (HH <= 255 && HH >= 24) {
            HH = 23;
          }
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }


        }
        if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = KITCHEN_LIGHT_OFF_MM;
          EEPROM.put(18, HH);

        }
        break;

      // Heat OFF state MINUTES
      case KITCHEN_HEAT_OFF_MM:
        lcd.setCursor(1, 0);
        lcd.print("KTCHN HEAT OFF");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.write(0);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          MM++;
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM > 59) {
            MM = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          MM--;
          if (MM <= 255 && MM >= 60) {
            MM = 59;
          }
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
        }
        if (b & BUTTON_LEFT) {
          lcd.clear();
          currentState = KITCHEN_LIGHT_OFF_HH;
          EEPROM.put(19, MM);

        }
        if (b & BUTTON_SELECT) {
          EEPROM.put(18, HH);
          EEPROM.put(19, MM);
          Serial.print("Ground/Kitchen/Heat/Main/Off:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
          lcd.clear();
        }
        break;

      case LIVING_ROOM:
        lcd.setCursor(0, 1);
        lcd.write(0);
        lcd.setCursor(15, 1);
        lcd.write(1);
        lcd.setCursor(4, 0);
        lcd.print("LIVING ROOM");
        lcd.setCursor(1, 1);
        lcd.print("Light");
        lcd.setCursor(11, 1);
        lcd.print("Heat");
        if (b & BUTTON_RIGHT) {
          currentState = LIVING_ROOM_HEAT;

        }
        else if (b & BUTTON_LEFT) {
          currentState = LIVING_ROOM_LIGHT;

        }
        break;

      case LIVING_ROOM_LIGHT:
        lcd.setCursor(1, 0);
        lcd.print("LIVNG ROOM LIGHT");
        lcd.setCursor(0, 1);
        lcd.write(0);
        lcd.setCursor(15, 1);
        lcd.write(1);
        lcd.setCursor(1, 1);
        lcd.print("OFF");
        lcd.setCursor(13, 1);
        lcd.print("ON");
        lcd.setCursor(6, 1);
        lcd.print("LEVEL");


        if (b & BUTTON_SELECT) {
          lcd.clear();
          currentState = LIVING_ROOM_LIGHT_LEVEL;

        }
        else if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = LIVING_ROOM_LIGHT_ON_HH;
          EEPROM.get(21, HH);
          EEPROM.get(22, MM);

        }
        else if (b & BUTTON_LEFT) {
          lcd.clear();
          currentState = LIVING_ROOM_LIGHT_OFF_HH;
          EEPROM.get(23, HH);
          EEPROM.get(24, MM);

        }
        break;

      case LIVING_ROOM_LIGHT_LEVEL:
        lcd.setCursor(1, 0);
        lcd.print("LIVNG ROOM LIGHT");
        // bottom row setup
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(6, 1);
        lcd.print(lightLevel); lcd.print("%");
        if (b & BUTTON_SELECT) {
          EEPROM.put(20, lightLevel);
          Serial.print("Ground/Living Room/Light/Main/Level:"); Serial.println(lightLevel);
          lcd.clear();
        }

        if (b & BUTTON_UP) {
          lightLevel++;
          if (lightLevel < 101) {
            lcd.setCursor(6, 1);
            lcd.print("    ");
            lcd.setCursor(6, 1);
            lcd.print(lightLevel); lcd.print("%");
          }
          else {
            lightLevel = 100;
          }
        }
        else if (b & BUTTON_DOWN) {
          lightLevel--;
          if (lightLevel <= 255 && lightLevel >= 101) {
            lightLevel = 0;
          }
          if (lightLevel > -1) {
            lcd.setCursor(6, 1);
            lcd.print("    ");
            lcd.setCursor(6, 1);
            lcd.print(lightLevel); lcd.print("%");
          }
        }
        break;

      // Light ON state HOURS
      case LIVING_ROOM_LIGHT_ON_HH:
        lcd.setCursor(1, 0);
        lcd.print("LVNG RM LIGHT ON");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(4, 1);
        lcd.write(1);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          HH++;
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH >= 24) {
            HH = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          HH--;
          if (HH <= 255 && HH >= 24) {
            HH = 23;
          }
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }


        }
        if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = LIVING_ROOM_LIGHT_ON_MM;
          EEPROM.put(21, HH);


        }
        break;


      // Light ON state MINUTES
      case LIVING_ROOM_LIGHT_ON_MM:
        lcd.setCursor(1, 0);
        lcd.print("LVNG RM LIGHT ON");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.write(0);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          MM++;
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM > 60) {
            MM = 0;
            MM++;
          }
        }
        if (b & BUTTON_DOWN) {
          MM--;
          if (MM <= 255 && MM >= 60) {
            MM = 59;
          }
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
        }
        if (b & BUTTON_LEFT) {
          lcd.clear();
          currentState = LIVING_ROOM_LIGHT_ON_HH;
          EEPROM.put(22, MM);

        }
        if (b & BUTTON_SELECT) {
          EEPROM.put(21, HH);
          EEPROM.put(22, MM);
          Serial.print("Ground/Living Room/Light/Main/On:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
          lcd.clear();
        }
        break;

      // Light OFF state HOURS
      case LIVING_ROOM_LIGHT_OFF_HH:
        lcd.setCursor(0, 0);
        lcd.print("LVNG RM LIGHT OFF");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(4, 1);
        lcd.write(1);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          HH++;
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH >= 24) {
            HH = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          HH--;
          if (HH <= 255 && HH >= 24) {
            HH = 23;
          }
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }


        }
        if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = LIVING_ROOM_LIGHT_OFF_MM;
          EEPROM.put(23, HH);


        }

      // Light OFF state MINUTES
      case LIVING_ROOM_LIGHT_OFF_MM:
        lcd.setCursor(0, 0);
        lcd.print("LVNG RM LIGHT OFF");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.write(0);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          MM++;
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM > 59) {
            MM = 0;
            MM++;
          }
        }
        if (b & BUTTON_DOWN) {
          MM--;
          if (MM <= 255 && MM >= 60) {
            MM = 59;
          }
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
        }
        if (b & BUTTON_LEFT) {
          lcd.clear();
          currentState = LIVING_ROOM_LIGHT_OFF_HH;
          EEPROM.put(24, MM);

        }
        if (b & BUTTON_SELECT) {
          EEPROM.put(23, HH);
          EEPROM.put(24, MM);
          Serial.print("Ground/Living Room/Light/Main/Off:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
          lcd.clear();
        }
        break;


      // Heat ON, OFF or LEVEL selection state
      case LIVING_ROOM_HEAT:
        lcd.setCursor(0, 0);
        lcd.print("LIVING ROOM HEAT");
        lcd.setCursor(0, 1);
        lcd.write(0);
        lcd.setCursor(15, 1);
        lcd.write(1);
        lcd.setCursor(1, 1);
        lcd.print("OFF");
        lcd.setCursor(13, 1);
        lcd.print("ON");
        lcd.setCursor(6, 1);
        lcd.print("LEVEL");


        if (b & BUTTON_SELECT) {
          lcd.clear();
          currentState = LIVING_ROOM_HEAT_LEVEL;

        }
        else if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = LIVING_ROOM_HEAT_ON_HH;
          EEPROM.get(26, HH);
          EEPROM.get(27, MM);

        }
        else if (b & BUTTON_LEFT) {
          EEPROM.get(28, HH);
          EEPROM.get(29, MM);
          lcd.clear();
          currentState = LIVING_ROOM_HEAT_OFF_HH;

        }
        break;



      // Heat LEVEL state
      case LIVING_ROOM_HEAT_LEVEL:
        lcd.setCursor(0, 0);
        lcd.print("LIVNG ROOM HEAT");
        // bottom row setup
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(7, 1);
        lcd.print(heatLevel); lcd.print("%");
        if (b & BUTTON_SELECT) {
          EEPROM.put(25, heatLevel);
          Serial.print("Ground/Living Room/Heat/Main/Level:"); Serial.println(heatLevel);
          lcd.clear();
        }

        if (b & BUTTON_UP) {
          heatLevel++;
          if (heatLevel < 101) {
            lcd.setCursor(6, 1);
            lcd.print("    ");
            lcd.setCursor(6, 1);
            lcd.print(heatLevel); lcd.print("%");
          }
          else {
            heatLevel = 100;
          }
        }
        else if (b & BUTTON_DOWN) {
          heatLevel--;
          if (heatLevel <= 255 && heatLevel >= 101) {
            heatLevel = 0;
          }
          if (heatLevel > -1) {
            lcd.setCursor(6, 1);
            lcd.print("   ");
            lcd.setCursor(6, 1);
            lcd.print(heatLevel); lcd.print("%");
          }
        }
        break;


      // Heat ON state HOURS
      case LIVING_ROOM_HEAT_ON_HH:
        EEPROM.get(26, HH);
        EEPROM.get(27, MM);
        lcd.setCursor(0, 0);
        lcd.print("LVNG RM HEAT ON");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(4, 1);
        lcd.write(1);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          HH++;
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH >= 24) {
            HH = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          HH--;
          if (HH <= 255 && HH >= 24) {
            HH = 23;
          }
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }


        }
        if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = LIVING_ROOM_LIGHT_ON_MM;
          EEPROM.put(26, HH);


        }
        break;

      // Heat ON state MINUTES
      case LIVING_ROOM_HEAT_ON_MM:
        lcd.setCursor(0, 0);
        lcd.print("LVNG RM HEAT ON");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.write(0);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          MM++;
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM > 59) {
            MM = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          MM--;
          if (MM <= 255 && MM >= 60) {
            MM = 59;
          }
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
        }
        if (b & BUTTON_LEFT) {
          lcd.clear();
          currentState = LIVING_ROOM_LIGHT_ON_HH;
          EEPROM.put(27, MM);

        }
        if (b & BUTTON_SELECT) {
          EEPROM.put(26, HH);
          EEPROM.put(27, MM);
          Serial.print("Ground/Living Room/Heat/Main/On:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
          lcd.clear();
        }
        break;

      // Heat OFF state HOURS
      case LIVING_ROOM_HEAT_OFF_HH:
        lcd.setCursor(0, 0);
        lcd.print("LVNG RM HEAT OFF");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(4, 1);
        lcd.write(1);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          HH++;
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH >= 24) {
            HH = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          HH--;
          if (HH <= 255 && HH >= 24) {
            HH = 23;
          }
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }


        }
        if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = LIVING_ROOM_LIGHT_OFF_MM;
          EEPROM.put(28, HH);


        }
        break;

      // Heat OFF state MINUTES
      case LIVING_ROOM_HEAT_OFF_MM:
        lcd.setCursor(0, 0);
        lcd.print("LVNG RM HEAT OFF");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.write(0);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          MM++;
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM > 59) {
            MM = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          MM--;
          if (MM <= 255 && MM >= 60) {
            MM = 59;
          }
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
        }
        if (b & BUTTON_LEFT) {
          lcd.clear();
          currentState = LIVING_ROOM_LIGHT_OFF_HH;
          EEPROM.put(29, MM);

        }
        if (b & BUTTON_SELECT) {
          EEPROM.put(28, HH);
          EEPROM.put(29, MM);
          Serial.print("Ground/Living Room/Heat/Main/Off:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
          lcd.clear();
        }
        break;

      case BEDROOM_1:
        lcd.setCursor(0, 1);
        lcd.write(0);
        lcd.setCursor(15, 1);
        lcd.write(1);
        lcd.setCursor(4, 0);
        lcd.print("BEDROOM 1");
        lcd.setCursor(1, 1);
        lcd.print("Light");
        lcd.setCursor(11, 1);
        lcd.print("Heat");
        if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = BEDROOM_1_HEAT;

        }
        else if (b & BUTTON_LEFT) {
          lcd.clear();
          currentState = BEDROOM_1_LIGHT;

        }
        break;

      case BEDROOM_1_LIGHT:
        lcd.setCursor(1, 0);
        lcd.print("BEDROOM 1 LIGHT");
        lcd.setCursor(0, 1);
        lcd.write(0);
        lcd.setCursor(15, 1);
        lcd.write(1);
        lcd.setCursor(1, 1);
        lcd.print("OFF");
        lcd.setCursor(13, 1);
        lcd.print("ON");
        lcd.setCursor(6, 1);
        lcd.print("LEVEL");


        if (b & BUTTON_SELECT) {
          lcd.clear();
          currentState = BEDROOM_1_LIGHT_LEVEL;

        }
        else if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = BEDROOM_1_LIGHT_ON_HH;
          EEPROM.get(31, HH);
          EEPROM.get(32, MM);

        }
        else if (b & BUTTON_LEFT) {
          lcd.clear();
          currentState = BEDROOM_1_LIGHT_OFF_HH;
          EEPROM.get(33, HH);
          EEPROM.get(34, MM);

        }
        break;

      case BEDROOM_1_LIGHT_LEVEL:
        lcd.setCursor(1, 0);
        lcd.print("BEDROOM 1 LIGHT");
        // bottom row setup
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(6, 1);
        lcd.print(lightLevel); lcd.print("%");
        if (b & BUTTON_SELECT) {
          EEPROM.put(30, lightLevel);
          Serial.print("First/Bedroom 1/Light/Main/Level:"); Serial.println(lightLevel);
          lcd.clear();
        }

        if (b & BUTTON_UP) {
          lightLevel++;
          if (lightLevel < 101) {
            lcd.setCursor(6, 1);
            lcd.print("    ");
            lcd.setCursor(6, 1);
            lcd.print(lightLevel); lcd.print("%");
          }
          else {
            lightLevel = 100;
          }
        }
        else if (b & BUTTON_DOWN) {
          lightLevel--;
          if (lightLevel <= 255 && lightLevel >= 101) {
            lightLevel = 0;
          }
          if (lightLevel > -1) {
            lcd.setCursor(6, 1);
            lcd.print("    ");
            lcd.setCursor(6, 1);
            lcd.print(lightLevel); lcd.print("%");
          }
        }
        break;

      // Light ON state HOURS
      case BEDROOM_1_LIGHT_ON_HH:
        lcd.setCursor(0, 0);
        lcd.print("BEDRM1 LIGHT ON");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(4, 1);
        lcd.write(1);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          HH++;
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH >= 24) {
            HH = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          HH--;
          if (HH <= 255 && HH >= 24) {
            HH = 23;
          }
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }


        }
        if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = BEDROOM_1_LIGHT_ON_MM;
          EEPROM.put(31, HH);


        }
        break;


      // Light ON state MINUTES
      case BEDROOM_1_LIGHT_ON_MM:
        lcd.setCursor(0, 0);
        lcd.print("BEDRM1 LIGHT ON");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.write(0);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          MM++;
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM > 60) {
            MM = 0;
            MM++;
          }
        }
        if (b & BUTTON_DOWN) {
          MM--;
          if (MM <= 255 && MM >= 60) {
            MM = 59;
          }
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
        }
        if (b & BUTTON_LEFT) {
          lcd.clear();
          currentState = BEDROOM_1_LIGHT_ON_HH;
          EEPROM.put(32, MM);

        }
        if (b & BUTTON_SELECT) {
          EEPROM.put(31, HH);
          EEPROM.put(32, MM);
          Serial.print("First/Bedroom 1/Light/Main/On:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
          lcd.clear();
        }
        break;

      // Light OFF state HOURS
      case BEDROOM_1_LIGHT_OFF_HH:
        lcd.setCursor(0, 0);
        lcd.print("BEDRM 1 LIGHT OFF");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(4, 1);
        lcd.write(1);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          HH++;
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH >= 24) {
            HH = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          HH--;
          if (HH <= 255 && HH >= 24) {
            HH = 23;
          }
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }


        }
        if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = BEDROOM_1_LIGHT_OFF_MM;
          EEPROM.put(33, HH);

        }
        break;


      // Light OFF state MINUTES
      case BEDROOM_1_LIGHT_OFF_MM:
        lcd.setCursor(0, 0);
        lcd.print("BEDRM 1 LIGHT OFF");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.write(0);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          MM++;
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM > 59) {
            MM = 0;
            MM++;
          }
        }
        if (b & BUTTON_DOWN) {
          MM--;
          if (MM <= 255 && MM >= 60) {
            MM = 59;
          }
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
        }
        if (b & BUTTON_LEFT) {
          lcd.clear();
          currentState = BEDROOM_1_LIGHT_OFF_HH;
          EEPROM.put(34, MM);

        }
        if (b & BUTTON_SELECT) {
          EEPROM.put(33, HH);
          EEPROM.put(34, MM);
          Serial.print("First/Bedroom 1/Light/Main/Off:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
          lcd.clear();
        }
        break;


      // Heat ON, OFF or LEVEL selection state
      case BEDROOM_1_HEAT:
        lcd.setCursor(0, 0);
        lcd.print("BEDROOM 1 HEAT");
        lcd.setCursor(0, 1);
        lcd.write(0);
        lcd.setCursor(15, 1);
        lcd.write(1);
        lcd.setCursor(1, 1);
        lcd.print("OFF");
        lcd.setCursor(13, 1);
        lcd.print("ON");
        lcd.setCursor(6, 1);
        lcd.print("LEVEL");


        if (b & BUTTON_SELECT) {
          lcd.clear();
          currentState = BEDROOM_1_HEAT_LEVEL;

        }
        else if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = BEDROOM_1_HEAT_ON_HH;
          EEPROM.get(36, HH);
          EEPROM.get(37, MM);

        }
        else if (b & BUTTON_LEFT) {
          lcd.clear();
          currentState = BEDROOM_1_HEAT_OFF_HH;
          EEPROM.get(38, HH);
          EEPROM.get(39, MM);

        }
        break;



      // Heat LEVEL state
      case BEDROOM_1_HEAT_LEVEL:
        lcd.setCursor(0, 0);
        lcd.print("BEDROOM 1 LIGHT");
        // bottom row setup
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(7, 1);
        lcd.print(heatLevel); lcd.print("%");
        if (b & BUTTON_SELECT) {
          EEPROM.put(35, heatLevel);
          Serial.print("First/Bedroom 1/Light/Main/Level:"); Serial.println(heatLevel);
          lcd.clear();
        }

        if (b & BUTTON_UP) {
          heatLevel++;
          if (heatLevel < 101) {
            lcd.setCursor(6, 1);
            lcd.print("    ");
            lcd.setCursor(6, 1);
            lcd.print(heatLevel); lcd.print("%");
          }
          else {
            heatLevel = 100;
          }
        }
        else if (b & BUTTON_DOWN) {
          heatLevel--;
          if (heatLevel <= 255 && heatLevel >= 101) {
            heatLevel = 0;
          }
          if (heatLevel > -1) {
            lcd.setCursor(6, 1);
            lcd.print("   ");
            lcd.setCursor(6, 1);
            lcd.print(heatLevel); lcd.print("%");
          }
        }
        break;


      // Heat ON state HOURS
      case BEDROOM_1_HEAT_ON_HH:
        lcd.setCursor(0, 0);
        lcd.print("BEDRM1 HEAT ON");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(4, 1);
        lcd.write(1);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          HH++;
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH >= 24) {
            HH = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          HH--;
          if (HH <= 255 && HH >= 24) {
            HH = 23;
          }
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }


        }
        if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = BEDROOM_1_LIGHT_ON_MM;
          EEPROM.put(36, HH);


        }
        break;

      // Heat ON state MINUTES
      case BEDROOM_1_HEAT_ON_MM:
        lcd.setCursor(0, 0);
        lcd.print("BEDRM1 HEAT ON");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.write(0);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          MM++;
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM > 59) {
            MM = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          MM--;
          if (MM <= 255 && MM >= 60) {
            MM = 59;
          }
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
        }
        if (b & BUTTON_LEFT) {
          lcd.clear();
          currentState = BEDROOM_1_LIGHT_ON_HH;
          EEPROM.put(37, MM);

        }
        if (b & BUTTON_SELECT) {
          EEPROM.put(36, HH);
          EEPROM.put(37, MM);
          Serial.print("First/Bedroom 1/Heat/Main/On:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
          lcd.clear();
        }
        break;

      // Heat OFF state HOURS
      case BEDROOM_1_HEAT_OFF_HH:
        lcd.setCursor(0, 0);
        lcd.print("BEDRM1 HEAT OFF");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(4, 1);
        lcd.write(1);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          HH++;
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH >= 24) {
            HH = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          HH--;
          if (HH <= 255 && HH >= 24) {
            HH = 23;
          }
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }


        }
        if (b & BUTTON_RIGHT) {
          lcd.clear();
          currentState = BEDROOM_1_LIGHT_OFF_MM;
          EEPROM.put(38, HH);


        }
        break;

      // Heat OFF state MINUTES
      case BEDROOM_1_HEAT_OFF_MM:
        lcd.setCursor(0, 0);
        lcd.print("BEDRM1 HEAT OFF");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.write(0);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          MM++;
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM > 59) {
            MM = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          MM--;
          if (MM <= 255 && MM >= 60) {
            MM = 59;
          }
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
        }
        if (b & BUTTON_LEFT) {
          lcd.clear();
          currentState = BEDROOM_1_LIGHT_OFF_HH;
          EEPROM.put(39, HH);

        }
        if (b & BUTTON_SELECT) {
          EEPROM.put(38, HH);
          EEPROM.put(39, HH);
          Serial.print("First/Bedroom 1/Heat/Main/Off:"); Serial.print(HH); Serial.print("."); Serial.println(HH);
          lcd.clear();
        }
        break;


      /* case BEDROOM_2:
         lcd.setCursor(0, 1);
         lcd.write(0);
         lcd.setCursor(15, 1);
         lcd.write(1);
         lcd.setCursor(4, 0);
         lcd.print("BEDROOM 2 ROOM");
         lcd.setCursor(1, 1);
         lcd.print("Light");
         lcd.setCursor(11, 1);
         lcd.print("Heat");
         if (b & BUTTON_RIGHT) {
           currentState = BEDROOM_2_HEAT;

         }
         else if (b & BUTTON_LEFT) {
           currentState = BEDROOM_2_LIGHT;

         }
         break;

        case BEDROOM_2_LIGHT:
         lcd.setCursor(1, 0);
         lcd.print("BEDROOM 2 LIGHT");
         lcd.setCursor(0, 1);
         lcd.write(0);
         lcd.setCursor(15, 1);
         lcd.write(1);
         lcd.setCursor(1, 1);
         lcd.print("OFF");
         lcd.setCursor(13, 1);
         lcd.print("ON");
         lcd.setCursor(6, 1);
         lcd.print("LEVEL");


         if (b & BUTTON_SELECT) {
           currentState = BEDROOM_2_LIGHT_LEVEL;

         }
         else if (b & BUTTON_RIGHT) {
           currentState = BEDROOM_2_LIGHT_ON_HH;
           EEPROM.get(41, HH);
          EEPROM.get(42, MM);

         }
         else if (b & BUTTON_LEFT) {
           currentState = BEDROOM_2_LIGHT_OFF_HH;
           EEPROM.get(43, HH);
          EEPROM.get(44, MM);

         }
         break;

        case BEDROOM_2_LIGHT_LEVEL:
         lcd.setCursor(1, 0);
         lcd.print("BEDROOM 2 LIGHT");
         // bottom row setup
         lcd.setCursor(0, 1);
         lcd.write(2);
         lcd.setCursor(15, 1);
         lcd.write(3);
         lcd.setCursor(6, 1);
         lcd.print(lightLevel); lcd.print("%");
         if (b & BUTTON_SELECT) {
           EEPROM.put(40, lightLevel);
           Serial.print("First/Bedroom 2/Light/Main/Level:"); Serial.println(lightLevel);
           lcd.clear();
         }

         if (b & BUTTON_UP) {
           lightLevel++;
           if (lightLevel < 101) {
             lcd.setCursor(6, 1);
             lcd.print("    ");
             lcd.setCursor(6, 1);
             lcd.print(lightLevel); lcd.print("%");
           }
           else {
             lightLevel = 100;
           }
         }
         else if (b & BUTTON_DOWN) {
           lightLevel--;
           if (lightLevel <= 255 && lightLevel >= 101) {
             lightLevel = 0;
           }
           if (lightLevel > -1) {
             lcd.setCursor(6, 1);
             lcd.print("    ");
             lcd.setCursor(6, 1);
             lcd.print(lightLevel); lcd.print("%");
           }
         }
         break;

        // Light ON state HOURS
        case BEDROOM_2_LIGHT_ON_HH:
         lcd.setCursor(0, 0);
         lcd.print("BEDRM2 LIGHT ON");
         lcd.setCursor(0, 1);
         lcd.write(2);
         lcd.setCursor(15, 1);
         lcd.write(3);
         lcd.setCursor(4, 1);
         lcd.write(1);
         lcd.setCursor(5, 1);
         if (HH < 10) {
           lcd.setCursor(5, 1);
           lcd.print("0");
           lcd.print(HH);
         }
         else {
           lcd.setCursor(5, 1);
           lcd.print(HH);
         }
         lcd.print(":");
         if (MM < 10) {
           lcd.setCursor(8, 1);
           lcd.print("0");
           lcd.print(MM);
         }
         else {
           lcd.setCursor(8, 1);
           lcd.print(MM);
         }
         if (b & BUTTON_UP) {
           HH++;
           if (HH >= 10 && HH < 24) {
             lcd.setCursor(5, 1);
             lcd.print(HH);
             // delay(250);
           }
           else if (HH < 9 && HH < 24) {
             lcd.setCursor(6, 1);
             lcd.print(HH);
             // delay(250);
           }
           else if (HH >= 24) {
             HH = 0;
           }
         }
         if (b & BUTTON_DOWN) {
           HH--;
           if (HH <= 255 && HH >= 24) {
             HH = 23;
           }
           if (HH >= 10 && HH < 24) {
             lcd.setCursor(5, 1);
             lcd.print(HH);
             // delay(250);
           }
           else if (HH < 9 && HH < 24) {
             lcd.setCursor(6, 1);
             lcd.print(HH);
             // delay(250);
           }


         }
         if (b & BUTTON_RIGHT) {
           currentState = BEDROOM_2_LIGHT_ON_MM;
           EEPROM.put(41, HH);


         }
         break;


        // Light ON state MINUTES
        case BEDROOM_2_LIGHT_ON_MM:
         lcd.setCursor(0, 0);
         lcd.print("BEDRM2 LIGHT ON");
         lcd.setCursor(0, 1);
         lcd.write(2);
         lcd.setCursor(15, 1);
         lcd.write(3);
         lcd.setCursor(10, 1);
         lcd.write(0);
         lcd.setCursor(5, 1);
         if (HH < 10) {
           lcd.setCursor(5, 1);
           lcd.print("0");
           lcd.print(HH);
         }
         else {
           lcd.setCursor(5, 1);
           lcd.print(HH);
         }
         lcd.print(":");
         if (MM < 10) {
           lcd.setCursor(8, 1);
           lcd.print("0");
           lcd.print(MM);
         }
         else {
           lcd.setCursor(8, 1);
           lcd.print(MM);
         }
         if (b & BUTTON_UP) {
           MM++;
           if (MM >= 10 && MM < 60) {
             lcd.setCursor(8, 1);
             lcd.print(MM);
             // delay(250);
           }
           else if (MM < 9 && MM < 60) {
             lcd.setCursor(9, 1);
             lcd.print(MM);
             // delay(250);
           }
           else if (MM > 60) {
             MM = 0;
             MM++;
           }
         }
         if (b & BUTTON_DOWN) {
           MM--;
           if (MM <= 255 && MM >= 60) {
             MM = 59;
           }
           if (MM >= 10 && MM < 60) {
             lcd.setCursor(8, 1);
             lcd.print(MM);
             // delay(250);
           }
           else if (MM < 9 && MM < 60) {
             lcd.setCursor(9, 1);
             lcd.print(MM);
             // delay(250);
           }
         }
         if (b & BUTTON_LEFT) {
           currentState = BEDROOM_2_LIGHT_ON_HH;
           EEPROM.put(42, MM);

         }
         if (b & BUTTON_SELECT) {
           EEPROM.put(41, HH);
           EEPROM.put(42, MM);
           Serial.print("First/Bedroom 2/Light/Main/On:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
           lcd.clear();
         }
         break;

        // Light OFF state HOURS
        case BEDROOM_2_LIGHT_OFF_HH:
         lcd.setCursor(0, 0);
         lcd.print("BEDRM2 LIGHT OFF");
         lcd.setCursor(0, 1);
         lcd.write(2);
         lcd.setCursor(15, 1);
         lcd.write(3);
         lcd.setCursor(4, 1);
         lcd.write(1);
         lcd.setCursor(5, 1);
         if (HH < 10) {
           lcd.setCursor(5, 1);
           lcd.print("0");
           lcd.print(HH);
         }
         else {
           lcd.setCursor(5, 1);
           lcd.print(HH);
         }
         lcd.print(":");
         if (MM < 10) {
           lcd.setCursor(8, 1);
           lcd.print("0");
           lcd.print(MM);
         }
         else {
           lcd.setCursor(8, 1);
           lcd.print(MM);
         }
         if (b & BUTTON_UP) {
           HH++;
           if (HH >= 10 && HH < 24) {
             lcd.setCursor(5, 1);
             lcd.print(HH);
             // delay(250);
           }
           else if (HH < 9 && HH < 24) {
             lcd.setCursor(6, 1);
             lcd.print(HH);
             // delay(250);
           }
           else if (HH >= 24) {
             HH = 0;
           }
         }
         if (b & BUTTON_DOWN) {
           HH--;
           if (HH <= 255 && HH >= 24) {
             HH = 23;
           }
           if (HH >= 10 && HH < 24) {
             lcd.setCursor(5, 1);
             lcd.print(HH);
             // delay(250);
           }
           else if (HH < 9 && HH < 24) {
             lcd.setCursor(6, 1);
             lcd.print(HH);
             // delay(250);
           }


         }
         if (b & BUTTON_RIGHT) {
           currentState = BEDROOM_2_LIGHT_OFF_MM;
           EEPROM.put(43, HH);


         }
         break;


        // Light OFF state MINUTES
        case BEDROOM_2_LIGHT_OFF_MM:
         lcd.setCursor(0, 0);
         lcd.print("BEDRM2 LIGHT OFF");
         lcd.setCursor(0, 1);
         lcd.write(2);
         lcd.setCursor(15, 1);
         lcd.write(3);
         lcd.setCursor(10, 1);
         lcd.write(0);
         lcd.setCursor(5, 1);
         if (HH < 10) {
           lcd.setCursor(5, 1);
           lcd.print("0");
           lcd.print(HH);
         }
         else {
           lcd.setCursor(5, 1);
           lcd.print(HH);
         }
         lcd.print(":");
         if (MM < 10) {
           lcd.setCursor(8, 1);
           lcd.print("0");
           lcd.print(MM);
         }
         else {
           lcd.setCursor(8, 1);
           lcd.print(MM);
         }
         if (b & BUTTON_UP) {
           MM++;
           if (MM >= 10 && MM < 60) {
             lcd.setCursor(8, 1);
             lcd.print(MM);
             // delay(250);
           }
           else if (MM < 9 && MM < 60) {
             lcd.setCursor(9, 1);
             lcd.print(MM);
             // delay(250);
           }
           else if (MM > 59) {
             MM = 0;
             MM++;
           }
         }
         if (b & BUTTON_DOWN) {
           MM--;
           if (MM <= 255 && MM >= 60) {
             MM = 59;
           }
           if (MM >= 10 && MM < 60) {
             lcd.setCursor(8, 1);
             lcd.print(MM);
             // delay(250);
           }
           else if (MM < 9 && MM < 60) {
             lcd.setCursor(9, 1);
             lcd.print(MM);
             // delay(250);
           }
         }
         if (b & BUTTON_LEFT) {
           currentState = BEDROOM_2_LIGHT_OFF_HH;
           EEPROM.put(44, MM);

         }
         if (b & BUTTON_SELECT) {
           EEPROM.put(43, HH);
           EEPROM.put(44, MM);
           Serial.print("First/Bedroom 2/Light/Main/Off:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
           lcd.clear();
         }
         break;


        // Heat ON, OFF or LEVEL selection state
        case BEDROOM_2_HEAT:
         lcd.setCursor(0, 0);
         lcd.print("BEDROOM 2 HEAT");
         lcd.setCursor(0, 1);
         lcd.write(0);
         lcd.setCursor(15, 1);
         lcd.write(1);
         lcd.setCursor(1, 1);
         lcd.print("OFF");
         lcd.setCursor(13, 1);
         lcd.print("ON");
         lcd.setCursor(6, 1);
         lcd.print("LEVEL");


         if (b & BUTTON_SELECT) {
           currentState = BEDROOM_2_HEAT_LEVEL;

         }
         else if (b & BUTTON_RIGHT) {
           currentState = BEDROOM_2_HEAT_ON_HH;
           EEPROM.get(46, HH);
          EEPROM.get(47, MM);

         }
         else if (b & BUTTON_LEFT) {
           currentState = BEDROOM_2_HEAT_OFF_HH;
           EEPROM.get(48, HH);
          EEPROM.get(49, MM);

         }
         break;



        // Heat LEVEL state
        case BEDROOM_2_HEAT_LEVEL:
         lcd.setCursor(0, 0);
         lcd.print("BEDROOM 2 HEAT");
         // bottom row setup
         lcd.setCursor(0, 1);
         lcd.write(2);
         lcd.setCursor(15, 1);
         lcd.write(3);
         lcd.setCursor(7, 1);
         lcd.print(lightLevel); lcd.print("%");
         if (b & BUTTON_SELECT) {
           EEPROM.put(45, heatLevel);
           Serial.print("First/Living Room/Light/Main/Level:"); Serial.println(heatLevel);
           lcd.clear();
         }

         if (b & BUTTON_UP) {
           heatLevel++;
           if (heatLevel < 101) {
             lcd.setCursor(6, 1);
             lcd.print("    ");
             lcd.setCursor(6, 1);
             lcd.print(heatLevel); lcd.print("%");
           }
           else {
             heatLevel = 100;
           }
         }
         else if (b & BUTTON_DOWN) {
           heatLevel--;
           if (heatLevel <= 255 && heatLevel >= 101) {
             heatLevel = 0;
           }
           if (heatLevel > -1) {
             lcd.setCursor(6, 1);
             lcd.print("   ");
             lcd.setCursor(6, 1);
             lcd.print(heatLevel); lcd.print("%");
           }
         }
         break;


        // Heat ON state HOURS
        case BEDROOM_2_HEAT_ON_HH:
         lcd.setCursor(0, 0);
         lcd.print("BEDRM2 HEAT ON");
         lcd.setCursor(0, 1);
         lcd.write(2);
         lcd.setCursor(15, 1);
         lcd.write(3);
         lcd.setCursor(4, 1);
         lcd.write(1);
         lcd.setCursor(5, 1);
         if (HH < 10) {
           lcd.setCursor(5, 1);
           lcd.print("0");
           lcd.print(HH);
         }
         else {
           lcd.setCursor(5, 1);
           lcd.print(HH);
         }
         lcd.print(":");
         if (MM < 10) {
           lcd.setCursor(8, 1);
           lcd.print("0");
           lcd.print(MM);
         }
         else {
           lcd.setCursor(8, 1);
           lcd.print(MM);
         }
         if (b & BUTTON_UP) {
           HH++;
           if (HH >= 10 && HH < 24) {
             lcd.setCursor(5, 1);
             lcd.print(HH);
             // delay(250);
           }
           else if (HH < 9 && HH < 24) {
             lcd.setCursor(6, 1);
             lcd.print(HH);
             // delay(250);
           }
           else if (HH >= 24) {
             HH = 0;
           }
         }
         if (b & BUTTON_DOWN) {
           HH--;
           if (HH <= 255 && HH >= 24) {
             HH = 23;
           }
           if (HH >= 10 && HH < 24) {
             lcd.setCursor(5, 1);
             lcd.print(HH);
             // delay(250);
           }
           else if (HH < 9 && HH < 24) {
             lcd.setCursor(6, 1);
             lcd.print(HH);
             // delay(250);
           }


         }
         if (b & BUTTON_RIGHT) {
           currentState = BEDROOM_2_LIGHT_ON_MM;
           EEPROM.put(46, HH);

         }
         break;

        // Heat ON state MINUTES
        case BEDROOM_2_HEAT_ON_MM:
         lcd.setCursor(0, 0);
         lcd.print("BEDRM2 LIGHT ON");
         lcd.setCursor(0, 1);
         lcd.write(2);
         lcd.setCursor(15, 1);
         lcd.write(3);
         lcd.setCursor(10, 1);
         lcd.write(0);
         lcd.setCursor(5, 1);
         if (HH < 10) {
           lcd.setCursor(5, 1);
           lcd.print("0");
           lcd.print(HH);
         }
         else {
           lcd.setCursor(5, 1);
           lcd.print(HH);
         }
         lcd.print(":");
         if (MM < 10) {
           lcd.setCursor(8, 1);
           lcd.print("0");
           lcd.print(MM);
         }
         else {
           lcd.setCursor(8, 1);
           lcd.print(MM);
         }
         if (b & BUTTON_UP) {
           MM++;
           if (MM >= 10 && MM < 60) {
             lcd.setCursor(8, 1);
             lcd.print(MM);
             // delay(250);
           }
           else if (MM < 9 && MM < 60) {
             lcd.setCursor(9, 1);
             lcd.print(MM);
             // delay(250);
           }
           else if (MM > 59) {
             MM = 0;
           }
         }
         if (b & BUTTON_DOWN) {
           MM--;
           if (MM <= 255 && MM >= 60) {
             MM = 59;
           }
           if (MM >= 10 && MM < 60) {
             lcd.setCursor(8, 1);
             lcd.print(MM);
             // delay(250);
           }
           else if (MM < 9 && MM < 60) {
             lcd.setCursor(9, 1);
             lcd.print(MM);
             // delay(250);
           }
         }
         if (b & BUTTON_LEFT) {
           currentState = BEDROOM_2_LIGHT_ON_HH;
           EEPROM.put(47, MM);

         }
         if (b & BUTTON_SELECT) {
           EEPROM.put(46, HH);
           EEPROM.put(47, MM);
           Serial.print("First/Bedroom 2/Heat/Main/On:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
           lcd.clear();
         }
         break;

        // Heat OFF state HOURS
        case BEDROOM_2_HEAT_OFF_HH:
         lcd.setCursor(0, 0);
         lcd.print("BEDRM2 HEAT OFF");
         lcd.setCursor(0, 1);
         lcd.write(2);
         lcd.setCursor(15, 1);
         lcd.write(3);
         lcd.setCursor(4, 1);
         lcd.write(1);
         lcd.setCursor(5, 1);
         if (HH < 10) {
           lcd.setCursor(5, 1);
           lcd.print("0");
           lcd.print(HH);
         }
         else {
           lcd.setCursor(5, 1);
           lcd.print(HH);
         }
         lcd.print(":");
         if (MM < 10) {
           lcd.setCursor(8, 1);
           lcd.print("0");
           lcd.print(MM);
         }
         else {
           lcd.setCursor(8, 1);
           lcd.print(MM);
         }
         if (b & BUTTON_UP) {
           HH++;
           if (HH >= 10 && HH < 24) {
             lcd.setCursor(5, 1);
             lcd.print(HH);
             // delay(250);
           }
           else if (HH < 9 && HH < 24) {
             lcd.setCursor(6, 1);
             lcd.print(HH);
             // delay(250);
           }
           else if (HH >= 24) {
             HH = 0;
           }
         }
         if (b & BUTTON_DOWN) {
           HH--;
           if (HH <= 255 && HH >= 24) {
             HH = 23;
           }
           if (HH >= 10 && HH < 24) {
             lcd.setCursor(5, 1);
             lcd.print(HH);
             // delay(250);
           }
           else if (HH < 9 && HH < 24) {
             lcd.setCursor(6, 1);
             lcd.print(HH);
             // delay(250);
           }


         }
         if (b & BUTTON_RIGHT) {
           currentState = BEDROOM_2_LIGHT_OFF_MM;
           EEPROM.put(48, HH);


         }
         break;

        // Heat OFF state MINUTES
        case BEDROOM_2_HEAT_OFF_MM:
         lcd.setCursor(0, 0);
         lcd.print("BEDRM2 HEAT OFF");
         lcd.setCursor(0, 1);
         lcd.write(2);
         lcd.setCursor(15, 1);
         lcd.write(3);
         lcd.setCursor(10, 1);
         lcd.write(0);
         lcd.setCursor(5, 1);
         if (HH < 10) {
           lcd.setCursor(5, 1);
           lcd.print("0");
           lcd.print(HH);
         }
         else {
           lcd.setCursor(5, 1);
           lcd.print(HH);
         }
         lcd.print(":");
         if (MM < 10) {
           lcd.setCursor(8, 1);
           lcd.print("0");
           lcd.print(MM);
         }
         else {
           lcd.setCursor(8, 1);
           lcd.print(MM);
         }
         if (b & BUTTON_UP) {
           MM++;
           if (MM >= 10 && MM < 60) {
             lcd.setCursor(8, 1);
             lcd.print(MM);
             // delay(250);
           }
           else if (MM < 9 && MM < 60) {
             lcd.setCursor(9, 1);
             lcd.print(MM);
             // delay(250);
           }
           else if (MM > 59) {
             MM = 0;
           }
         }
         if (b & BUTTON_DOWN) {
           MM--;
           if (MM <= 255 && MM >= 60) {
             MM = 59;
           }
           if (MM >= 10 && MM < 60) {
             lcd.setCursor(8, 1);
             lcd.print(MM);
             // delay(250);
           }
           else if (MM < 9 && MM < 60) {
             lcd.setCursor(9, 1);
             lcd.print(MM);
             // delay(250);
           }
         }
         if (b & BUTTON_LEFT) {
           currentState = BEDROOM_2_LIGHT_OFF_HH;
           EEPROM.put(49, MM);

         }
         if (b & BUTTON_SELECT) {
           EEPROM.put(48, HH);
           EEPROM.put(49, MM);
           Serial.print("First/Bedroom 2/Heat/Main/Off:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
           lcd.clear();
         }
         break;

      case BATHROOM:
        lcd.setCursor(0, 1);
        lcd.write(0);
        lcd.setCursor(15, 1);
        lcd.write(1);
        lcd.setCursor(4, 0);
        lcd.print("BATHROOM");
        lcd.setCursor(1, 1);
        lcd.print("Light");
        lcd.setCursor(11, 1);
        lcd.print("Heat");
        if (b & BUTTON_RIGHT) {
          currentState = BATHROOM_HEAT;

        }
        else if (b & BUTTON_LEFT) {
          currentState = BATHROOM_LIGHT;

        }
        break;

      case BATHROOM_LIGHT:
        lcd.setCursor(1, 0);
        lcd.print("BATHROOM LIGHT");
        lcd.setCursor(0, 1);
        lcd.write(0);
        lcd.setCursor(15, 1);
        lcd.write(1);
        lcd.setCursor(1, 1);
        lcd.print("OFF");
        lcd.setCursor(13, 1);
        lcd.print("ON");
        lcd.setCursor(6, 1);
        lcd.print("LEVEL");


        if (b & BUTTON_SELECT) {
          currentState = BATHROOM_LIGHT_LEVEL;

        }
        else if (b & BUTTON_RIGHT) {
          currentState = BATHROOM_LIGHT_ON_HH;
          EEPROM.get(51, HH);
          EEPROM.get(52, MM);

        }
        else if (b & BUTTON_LEFT) {
          currentState = BATHROOM_LIGHT_OFF_HH;
          EEPROM.get(53, HH);
          EEPROM.get(54, MM);

        }
        break;

      case BATHROOM_LIGHT_LEVEL:
        lcd.setCursor(1, 0);
        lcd.print("BATHROOM LIGHT");
        // bottom row setup
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(6, 1);
        lcd.print(lightLevel); lcd.print("%");
        if (b & BUTTON_SELECT) {
          EEPROM.put(50, lightLevel);
          Serial.print("First/Living Room/Light/Main/Level:"); Serial.println(lightLevel);
          lcd.clear();
        }

        if (b & BUTTON_UP) {
          lightLevel++;
          if (lightLevel < 101) {
            lcd.setCursor(6, 1);
            lcd.print("    ");
            lcd.setCursor(6, 1);
            lcd.print(lightLevel); lcd.print("%");
          }
          else {
            lightLevel = 100;
          }
        }
        else if (b & BUTTON_DOWN) {
          lightLevel--;
          if (lightLevel <= 255 && lightLevel >= 101) {
            lightLevel = 0;
          }
          if (lightLevel > -1) {
            lcd.setCursor(6, 1);
            lcd.print("    ");
            lcd.setCursor(6, 1);
            lcd.print(lightLevel); lcd.print("%");
          }
        }
        break;

      // Light ON state HOURS
      case BATHROOM_LIGHT_ON_HH:
        lcd.setCursor(0, 0);
        lcd.print("BATHRM LIGHT ON");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(4, 1);
        lcd.write(1);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          HH++;
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH >= 24) {
            HH = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          HH--;
          if (HH <= 255 && HH >= 24) {
            HH = 23;
          }
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }


        }
        if (b & BUTTON_RIGHT) {
          currentState = BATHROOM_LIGHT_ON_MM;
          EEPROM.put(51, HH);


        }
        break;


      // Light ON state MINUTES
      case BATHROOM_LIGHT_ON_MM:
        lcd.setCursor(0, 0);
        lcd.print("BATHRM LIGHT ON");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.write(0);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          MM++;
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM > 60) {
            MM = 0;
            MM++;
          }
        }
        if (b & BUTTON_DOWN) {
          MM--;
          if (MM <= 255 && MM >= 60) {
            MM = 59;
          }
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
        }
        if (b & BUTTON_LEFT) {
          currentState = BATHROOM_LIGHT_ON_HH;
          EEPROM.put(52, MM);

        }
        if (b & BUTTON_SELECT) {
          EEPROM.put(51, HH);
          EEPROM.put(52, MM);
          Serial.print("First/Bathroom/Light/Main/On:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
          lcd.clear();
        }
        break;

      // Light OFF state HOURS
      case BATHROOM_LIGHT_OFF_HH:
        lcd.setCursor(0, 0);
        lcd.print("BATHRM LIGHT OFF");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(4, 1);
        lcd.write(1);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          HH++;
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH >= 24) {
            HH = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          HH--;
          if (HH <= 255 && HH >= 24) {
            HH = 23;
          }
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }


        }
        if (b & BUTTON_RIGHT) {
          currentState = BATHROOM_LIGHT_OFF_MM;
          EEPROM.put(53, HH);


        }
        break;


      // Light OFF state MINUTES
      case BATHROOM_LIGHT_OFF_MM:
        lcd.setCursor(0, 0);
        lcd.print("BATHRM LIGHT OFF");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.write(0);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          MM++;
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM > 59) {
            MM = 0;
            MM++;
          }
        }
        if (b & BUTTON_DOWN) {
          MM--;
          if (MM <= 255 && MM >= 60) {
            MM = 59;
          }
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
        }
        if (b & BUTTON_LEFT) {
          currentState = BATHROOM_LIGHT_OFF_HH;
          EEPROM.put(54, MM);

        }
        if (b & BUTTON_SELECT) {
          EEPROM.put(53, HH);
          EEPROM.put(54, MM);
          Serial.print("First/Bathroom/Light/Main/Off:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
          lcd.clear();
        }
        break;


      // Heat ON, OFF or LEVEL selection state
      case BATHROOM_HEAT:
        lcd.setCursor(0, 0);
        lcd.print("BATHROOM HEAT");
        lcd.setCursor(0, 1);
        lcd.write(0);
        lcd.setCursor(15, 1);
        lcd.write(1);
        lcd.setCursor(1, 1);
        lcd.print("OFF");
        lcd.setCursor(13, 1);
        lcd.print("ON");
        lcd.setCursor(6, 1);
        lcd.print("LEVEL");


        if (b & BUTTON_SELECT) {
          currentState = BATHROOM_HEAT_LEVEL;

        }
        else if (b & BUTTON_RIGHT) {
          currentState = BATHROOM_HEAT_ON_HH;
          EEPROM.get(56, HH);
          EEPROM.get(57, MM);

        }
        else if (b & BUTTON_LEFT) {
          currentState = BATHROOM_HEAT_OFF_HH;
          EEPROM.get(58, HH);
          EEPROM.get(59, MM);

        }
        break;



      // Heat LEVEL state
      case BATHROOM_HEAT_LEVEL:
        lcd.setCursor(0, 0);
        lcd.print("BATHROOM LIGHT");
        // bottom row setup
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(7, 1);
        lcd.print(heatLevel); lcd.print("%");
        if (b & BUTTON_SELECT) {
          EEPROM.put(55, heatLevel);
          Serial.print("First/Living Room/Light/Main/Level:"); Serial.println(heatLevel);
          lcd.clear();
        }

        if (b & BUTTON_UP) {
          heatLevel++;
          if (heatLevel < 101) {
            lcd.setCursor(6, 1);
            lcd.print("    ");
            lcd.setCursor(6, 1);
            lcd.print(heatLevel); lcd.print("%");
          }
          else {
            heatLevel = 100;
          }
        }
        else if (b & BUTTON_DOWN) {
          heatLevel--;
          if (heatLevel <= 255 && heatLevel >= 101) {
            heatLevel = 0;
          }
          if (heatLevel > -1) {
            lcd.setCursor(6, 1);
            lcd.print("   ");
            lcd.setCursor(6, 1);
            lcd.print(heatLevel); lcd.print("%");
          }
        }
        break;


      // Heat ON state HOURS
      case BATHROOM_HEAT_ON_HH:
        lcd.setCursor(0, 0);
        lcd.print("BATHRM HEAT ON");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(4, 1);
        lcd.write(1);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          HH++;
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH >= 24) {
            HH = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          HH--;
          if (HH <= 255 && HH >= 24) {
            HH = 23;
          }
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }


        }
        if (b & BUTTON_RIGHT) {
          currentState = BATHROOM_LIGHT_ON_MM;
          EEPROM.put(56, HH);


        }
        break;

      // Heat ON state MINUTES
      case BATHROOM_HEAT_ON_MM:
        lcd.setCursor(0, 0);
        lcd.print("BATHRM HEAT ON");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.write(0);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          MM++;
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM > 59) {
            MM = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          MM--;
          if (MM <= 255 && MM >= 60) {
            MM = 59;
          }
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
        }
        if (b & BUTTON_LEFT) {
          currentState = BATHROOM_LIGHT_ON_HH;
          EEPROM.put(57, MM);

        }
        if (b & BUTTON_SELECT) {
          EEPROM.put(56, HH);
          EEPROM.put(57, MM);
          Serial.print("First/Bathroom/Heat/Main/On:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
          lcd.clear();
        }
        break;

      // Heat OFF state HOURS
      case BATHROOM_HEAT_OFF_HH:
        lcd.setCursor(0, 0);
        lcd.print("BATHRM HEAT OFF");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(4, 1);
        lcd.write(1);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          HH++;
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH >= 24) {
            HH = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          HH--;
          if (HH <= 255 && HH >= 24) {
            HH = 23;
          }
          if (HH >= 10 && HH < 24) {
            lcd.setCursor(5, 1);
            lcd.print(HH);
            // delay(250);
          }
          else if (HH < 9 && HH < 24) {
            lcd.setCursor(6, 1);
            lcd.print(HH);
            // delay(250);
          }


        }
        if (b & BUTTON_RIGHT) {
          currentState = BATHROOM_LIGHT_OFF_MM;
          EEPROM.put(58, HH);


        }
        break;

      // Heat OFF state MINUTES
      case BATHROOM_HEAT_OFF_MM:
        lcd.setCursor(0, 0);
        lcd.print("BATHRM HEAT OFF");
        lcd.setCursor(0, 1);
        lcd.write(2);
        lcd.setCursor(15, 1);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.write(0);
        lcd.setCursor(5, 1);
        if (HH < 10) {
          lcd.setCursor(5, 1);
          lcd.print("0");
          lcd.print(HH);
        }
        else {
          lcd.setCursor(5, 1);
          lcd.print(HH);
        }
        lcd.print(":");
        if (MM < 10) {
          lcd.setCursor(8, 1);
          lcd.print("0");
          lcd.print(MM);
        }
        else {
          lcd.setCursor(8, 1);
          lcd.print(MM);
        }
        if (b & BUTTON_UP) {
          MM++;
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM > 59) {
            MM = 0;
          }
        }
        if (b & BUTTON_DOWN) {
          MM--;
          if (MM <= 255 && MM >= 60) {
            MM = 59;
          }
          if (MM >= 10 && MM < 60) {
            lcd.setCursor(8, 1);
            lcd.print(MM);
            // delay(250);
          }
          else if (MM < 9 && MM < 60) {
            lcd.setCursor(9, 1);
            lcd.print(MM);
            // delay(250);
          }
        }
        if (b & BUTTON_LEFT) {
          currentState = BATHROOM_LIGHT_OFF_HH;
          EEPROM.put(59, MM);
        }
        if (b & BUTTON_SELECT) {
          EEPROM.put(58, HH);
          EEPROM.put(59, MM);
          Serial.print("First/Bathroom/Heat/Main/On:"); Serial.print(HH); Serial.print("."); Serial.println(MM);
          lcd.clear();
        }
        break; */
      }
  }
}

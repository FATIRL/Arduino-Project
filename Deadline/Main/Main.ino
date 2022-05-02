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



uint8_t currentState = 79;
static byte name0x0[] = { B00000, B00000, B00010, B00100, B01000, B00100, B00010, B00000 };
static byte name0x1[] = { B00000, B00000, B01000, B00100, B00010, B00100, B01000, B00000 };
static byte name0x2[] = { B00000, B00000, B00100, B00100, B00100, B11111, B01110, B00100 };
static byte name0x3[] = { B00100, B01110, B11111, B00100, B00100, B00100, B00000, B00000 };

// Hall variables
uint8_t hallLightLevel = 0; 
uint8_t hallHeatLevel = 0;
uint8_t hallLightOnHH = 0;
uint8_t hallLightOnMM = 0;
uint8_t hallLightOffHH = 0;
uint8_t hallLightOffMM = 0;
uint8_t hallHeatOnHH = 0;
uint8_t hallHeatOnMM = 0;
uint8_t hallHeatOffHH = 0;
uint8_t hallHeatOffMM = 0;

// Kitchen variables
uint8_t kitchenLightLevel = 100;
uint8_t kitchenHeatLevel = 42;
uint8_t kitchenLightOnHH = 23;
uint8_t kitchenLightOnMM = 55;
uint8_t kitchenLightOffHH = 0;
uint8_t kitchenLightOffMM = 0;
uint8_t kitchenHeatOnHH = 23;
uint8_t kitchenHeatOnMM = 55;
uint8_t kitchenHeatOffHH = 0;
uint8_t kitchenHeatOffMM = 0;

// Living room variables
uint8_t livingRoomLightLevel = 100;
uint8_t livingRoomHeatLevel = 42;
uint8_t livingRoomLightOnHH = 23;
uint8_t livingRoomLightOnMM = 55;
uint8_t livingRoomLightOffHH = 0;
uint8_t livingRoomLightOffMM = 0;
uint8_t livingRoomHeatOnHH = 23;
uint8_t livingRoomHeatOnMM = 55;
uint8_t livingRoomHeatOffHH = 0;
uint8_t livingRoomHeatOffMM = 0;

// Bedroom 1 variables
uint8_t bedroom1LightLevel = 100;
uint8_t bedroom1HeatLevel = 42;
uint8_t bedroom1LightOnHH = 23;
uint8_t bedroom1LightOnMM = 55;
uint8_t bedroom1LightOffHH = 0;
uint8_t bedroom1LightOffMM = 0;
uint8_t bedroom1HeatOnHH = 23;
uint8_t bedroom1HeatOnMM = 55;
uint8_t bedroom1HeatOffHH = 0;
uint8_t bedroom1HeatOffMM = 0;

// Bedroom 2 variables
uint8_t bedroom2LightLevel = 100;
uint8_t bedroom2HeatLevel = 42;
uint8_t bedroom2LightOnHH = 23;
uint8_t bedroom2LightOnMM = 55;
uint8_t bedroom2LightOffHH = 0;
uint8_t bedroom2LightOffMM = 0;
uint8_t bedroom2HeatOnHH = 23;
uint8_t bedroom2HeatOnMM = 55;
uint8_t bedroom2HeatOffHH = 0;
uint8_t bedroom2HeatOffMM = 0;

// Bathroom variables
uint8_t bathroomLightLevel = 100;
uint8_t bathroomHeatLevel = 42;
uint8_t bathroomLightOnHH = 23;
uint8_t bathroomLightOnMM = 55;
uint8_t bathroomLightOffHH = 0;
uint8_t bathroomLightOffMM = 0;
uint8_t bathroomHeatOnHH = 23;
uint8_t bathroomHeatOnMM = 55;
uint8_t bathroomHeatOffHH = 0;
uint8_t bathroomHeatOffMM = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  Serial.println("BASIC");
  /*
  EEPROM.get(0, hallLightLevel);
  EEPROM.get(1, hallLightOnHH);
  EEPROM.get(2, hallLightOnMM);
  EEPROM.get(3, hallLightOffHH);
  EEPROM.get(4, hallLightOffMM);
  EEPROM.get(5, hallHeatLevel);
  EEPROM.get(6, hallHeatOnHH);
  EEPROM.get(7, hallHeatOnMM);
  EEPROM.get(8, hallHeatOffHH);
  EEPROM.get(9, hallHeatOffMM);
  EEPROM.get(10, kitchenLightLevel);
  EEPROM.get(11, kitchenLightOnHH);
  EEPROM.get(12, kitchenLightOnMM);
  EEPROM.get(13, kitchenLightOffHH);
  EEPROM.get(14, kitchenLightOffHH);
  EEPROM.get(15, kitchenHeatLevel);
  EEPROM.get(16, kitchenHeatOnHH);
  EEPROM.get(17, kitchenHeatOnMM);
  EEPROM.get(18, kitchenHeatOffHH);
  EEPROM.get(19, kitchenHeatOffMM);
  EEPROM.get(20, livingRoomLightLevel);
  EEPROM.get(21, livingRoomLightOnHH);
  EEPROM.get(22, livingRoomLightOnMM);
  EEPROM.get(23, livingRoomLightOffHH);
  EEPROM.get(24, livingRoomLightOffMM);
  EEPROM.get(25, livingRoomHeatLevel);
  EEPROM.get(26, livingRoomHeatOnHH);
  EEPROM.get(27, livingRoomHeatOnMM);
  EEPROM.get(28, livingRoomHeatOffHH);
  EEPROM.get(29, livingRoomHeatOffMM);
  EEPROM.get(30, bedroom1LightLevel);
  EEPROM.get(31, bedroom1LightOnHH);
  EEPROM.get(32, bedroom1LightOnMM);
  EEPROM.get(33, bedroom1LightOffHH);
  EEPROM.get(34, bedroom1LightOffMM);
  EEPROM.get(35, bedroom1HeatLevel);
  EEPROM.get(36, bedroom1HeatOnHH);
  EEPROM.get(37, bedroom1HeatOnMM);
  EEPROM.get(38, bedroom1HeatOffHH);
  EEPROM.get(39, bedroom1HeatOffMM);
  EEPROM.get(40, bedroom2LightLevel);
  EEPROM.get(41, bedroom2LightOnHH);
  EEPROM.get(42, bedroom2LightOnMM);
  EEPROM.get(43, bedroom2LightOffHH);
  EEPROM.get(44, bedroom2LightOffMM);
  EEPROM.get(45, bedroom2HeatLevel);
  EEPROM.get(46, bedroom2HeatOnHH);
  EEPROM.get(47, bedroom2HeatOnMM);
  EEPROM.get(48, bedroom2HeatOffHH);
  EEPROM.get(49, bedroom2HeatOffMM);
  EEPROM.get(50, bathroomLightLevel);
  EEPROM.get(51, bathroomLightOnHH);
  EEPROM.get(52, bathroomLightOnMM);
  EEPROM.get(53, bathroomLightOffHH);
  EEPROM.get(54, bathroomLightOffMM);
  EEPROM.get(55, bathroomHeatLevel);
  EEPROM.get(56, bathroomHeatOnHH);
  EEPROM.get(57, bathroomHeatOnMM);
  EEPROM.get(58, bathroomHeatOffHH);
  EEPROM.get(59, bathroomHeatOffMM); 
  */
}



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
        currentState = FLOOR_GROUND;
        lcd.clear();
      }
      else if (b & BUTTON_RIGHT) {
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
        currentState = HALL;
        lcd.clear();
      }
      else if (b & BUTTON_RIGHT) {
        currentState = LIVING_ROOM;
        lcd.clear();
      }
      else if (b & BUTTON_LEFT) {
        currentState = KITCHEN;
        lcd.clear();
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
        currentState = BEDROOM_1;
        lcd.clear();
      }
      else if (b & BUTTON_RIGHT) {
        currentState = BATHROOM;
        lcd.clear();
      }
      else if (b & BUTTON_LEFT) {
        currentState = BEDROOM_2;
        lcd.clear();
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
        currentState = HALL_HEAT;
        lcd.clear();
      }
      else if (b & BUTTON_LEFT) {
        currentState = HALL_LIGHT;
       lcd.clear();
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
        currentState = HALL_LIGHT_LEVEL;
        lcd.clear();
      }
      else if (b & BUTTON_RIGHT) {
        currentState = HALL_LIGHT_ON_HH;
        lcd.clear();
      }
      else if (b & BUTTON_LEFT) {
        currentState = HALL_LIGHT_OFF_HH;
        lcd.clear();
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
      lcd.print(hallLightLevel); lcd.print("%");
      /* if (b & BUTTON_SELECT) {
        EEPROM.update(0, hallLightLevel);
        Serial.print("Ground/Hall/Light/Main/Level:"); Serial.println(EEPROM.read(0));
        lcd.clear();
      } */

      if (b & BUTTON_UP) {
        hallLightLevel++;
        if (hallLightLevel < 100 && hallLightLevel > 0) {
          lcd.setCursor(6, 1);
          lcd.print("    ");
          lcd.setCursor(6, 1);
          lcd.print(hallLightLevel); lcd.print("%");
        }
        else if (hallLightLevel >= 100) {
          hallLightLevel = 100;
        }
      }
      else if (b & BUTTON_DOWN) {
        hallLightLevel--;
        if (hallLightLevel <= 255 && hallLightLevel >= 101) {
          hallLightLevel = 0;
        }
        if (hallLightLevel > 0) {
          lcd.setCursor(6, 1);
          lcd.print("    ");
          lcd.setCursor(6, 1);
          lcd.print(hallLightLevel); lcd.print("%");
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
      if (hallLightOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(hallLightOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(hallLightOnHH);
      }
      lcd.print(":");
      if (hallLightOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(hallLightOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(hallLightOnMM);
      }
      if (b & BUTTON_UP) {
        hallLightOnHH++;
        if (hallLightOnHH >= 10 && hallLightOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(hallLightOnHH);
          // delay(250);
        }
        else if (hallLightOnHH < 9 && hallLightOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(hallLightOnHH);
          // delay(250);
        }
        else if (hallLightOnHH >= 24) {
          hallLightOnHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        hallLightOnHH--;
        if (hallLightOnHH <= 255 && hallLightOnHH >= 24) {
          hallLightOnHH = 23;
        }
        if (hallLightOnHH >= 10 && hallLightOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(hallLightOnHH);
          // delay(250);
        }
        else if (hallLightOnHH < 9 && hallLightOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(hallLightOnHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = HALL_LIGHT_ON_MM;
        lcd.clear();

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
      if (hallLightOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(hallLightOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(hallLightOnHH);
      }
      lcd.print(":");
      if (hallLightOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(hallLightOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(hallLightOnMM);
      }
      if (b & BUTTON_UP) {
        hallLightOnMM++;
        if (hallLightOnMM >= 10 && hallLightOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(hallLightOnMM);
          // delay(250);
        }
        else if (hallLightOnMM < 9 && hallLightOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(hallLightOnMM);
          // delay(250);
        }
        else if (hallLightOnMM > 60) {
          hallLightOnMM = 0;
          hallLightOnMM++;
        }
      }
      if (b & BUTTON_DOWN) {
        hallLightOnMM--;
        if (hallLightOnMM <= 255 && hallLightOnMM >= 60) {
          hallLightOnMM = 59;
        }
        if (hallLightOnMM >= 10 && hallLightOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(hallLightOnMM);
          // delay(250);
        }
        else if (hallLightOnMM < 9 && hallLightOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(hallLightOnMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = HALL_LIGHT_ON_HH;
        lcd.clear();
      }
      /* if (b & BUTTON_SELECT) {
        EEPROM.update(1, hallLightOnHH);
        EEPROM.update(2, hallLightOnMM);
        Serial.print("Ground/Hall/Light/Main/On:"); Serial.print(EEPROM.read(1)); Serial.print("."); Serial.println(EEPROM.read(2));
        lcd.clear(); 
      }*/

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
      if (hallLightOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(hallLightOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(hallLightOffHH);
      }
      lcd.print(":");
      if (hallLightOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(hallLightOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(hallLightOffMM);
      }
      if (b & BUTTON_UP) {
        hallLightOffHH++;
        if (hallLightOffHH >= 10 && hallLightOffHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(hallLightOffHH);
          // delay(250);
        }
        else if (hallLightOffHH < 9 && hallLightOffHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(hallLightOffHH);
          // delay(250);
        }
        else if (hallLightOffHH >= 24) {
          hallLightOffHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        hallLightOffHH--;
        if (hallLightOffHH <= 255 && hallLightOffHH >= 24) {
          hallLightOffHH = 23;
        }
        if (hallLightOffHH >= 10 && hallLightOffHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(hallLightOffHH);
          // delay(250);
        }
        else if (hallLightOffHH < 9 && hallLightOffHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(hallLightOffHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = HALL_LIGHT_OFF_MM;
        lcd.clear();

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
      if (hallLightOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(hallLightOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(hallLightOffHH);
      }
      lcd.print(":");
      if (hallLightOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(hallLightOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(hallLightOffMM);
      }
      if (b & BUTTON_UP) {
        hallLightOffMM++;
        if (hallLightOffMM >= 10 && hallLightOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(hallLightOffMM);
          // delay(250);
        }
        else if (hallLightOffMM < 9 && hallLightOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(hallLightOffMM);
          // delay(250);
        }
        else if (hallLightOffMM > 59) {
          hallLightOffMM = 0;
          hallLightOffMM++;
        }
      }
      if (b & BUTTON_DOWN) {
        hallLightOffMM--;
        if (hallLightOffMM <= 255 && hallLightOffMM >= 60) {
          hallLightOffMM = 59;
        }
        if (hallLightOffMM >= 10 && hallLightOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(hallLightOffMM);
          // delay(250);
        }
        else if (hallLightOffMM < 9 && hallLightOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(hallLightOffMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = HALL_LIGHT_OFF_HH;
        lcd.clear();
      }

      if (b & BUTTON_RIGHT) {
        currentState = HALL_LIGHT_ON_HH;
      }
   /*   if (b & BUTTON_SELECT) {
      EEPROM.update(3, hallLightOffHH);
        EEPROM.update(4, hallLightOffMM);
        Serial.print("Ground/Hall/Light/Main/Off:"); Serial.print(EEPROM.read(3)); Serial.print("."); Serial.println(EEPROM.read(4));
        lcd.clear(); 
      } */
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
        currentState = HALL_HEAT_LEVEL;
        lcd.clear();
      }
      else if (b & BUTTON_RIGHT) {
        currentState = HALL_HEAT_ON_HH;
        lcd.clear();
      }
      else if (b & BUTTON_LEFT) {
        currentState = HALL_HEAT_OFF_HH;
        lcd.clear();
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
      lcd.print(hallHeatLevel); lcd.print("%");
   /*   if (b & BUTTON_SELECT) {
        EEPROM.update(5, hallHeatLevel);
        Serial.print("Ground/Hall/Heat/Main/Level:"); Serial.println(EEPROM.read(5));
        lcd.clear();
      } */

      if (b & BUTTON_UP) {
        hallHeatLevel++;
        if (hallHeatLevel < 100 && hallHeatLevel > 0) {
          lcd.setCursor(6, 1);
          lcd.print("    ");
          lcd.setCursor(6, 1);
          lcd.print(hallHeatLevel); lcd.print("%");
        }
        else if (hallHeatLevel >= 100) {
          hallHeatLevel = 100;
        }
      }
      else if (b & BUTTON_DOWN) {
        hallHeatLevel--;
        if (hallHeatLevel <= 255 && hallHeatLevel >= 101) {
          hallHeatLevel = 0;
        }
        if (hallHeatLevel > 0) {
          lcd.setCursor(6, 1);
          lcd.print("   ");
          lcd.setCursor(6, 1);
          lcd.print(hallHeatLevel); lcd.print("%");
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
      if (hallHeatOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(hallHeatOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(hallHeatOnHH);
      }
      lcd.print(":");
      if (hallHeatOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(hallHeatOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(hallHeatOnMM);
      }
      if (b & BUTTON_UP) {
        hallHeatOnHH++;
        if (hallHeatOnHH >= 10 && hallHeatOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(hallHeatOnHH);
          // delay(250);
        }
        else if (hallHeatOnHH < 9 && hallHeatOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(hallHeatOnHH);
          // delay(250);
        }
        else if (hallHeatOnHH >= 24) {
          hallHeatOnHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        hallHeatOnHH--;
        if (hallHeatOnHH <= 255 && hallHeatOnHH >= 24) {
          hallHeatOnHH = 23;
        }
        if (hallHeatOnHH >= 10 && hallHeatOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(hallHeatOnHH);
          // delay(250);
        }
        else if (hallHeatOnHH < 9 && hallHeatOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(hallHeatOnHH);
          // delay(250);
        }



      }
      if (b & BUTTON_RIGHT) {
        currentState = HALL_HEAT_ON_MM;
        lcd.clear();

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
      if (hallHeatOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(hallHeatOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(hallHeatOnHH);
      }
      lcd.print(":");
      if (hallHeatOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(hallHeatOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(hallHeatOnMM);
      }
      if (b & BUTTON_UP) {
        hallHeatOnMM++;
        if (hallHeatOnMM >= 10 && hallHeatOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(hallHeatOnMM);
          // delay(250);
        }
        else if (hallHeatOnMM < 9 && hallHeatOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(hallHeatOnMM);
          // delay(250);
        }
        else if (hallHeatOnMM >= 60) {
          hallHeatOnMM = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        hallHeatOnMM--;

        if (hallHeatOnMM >= 10 && hallHeatOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(hallHeatOnMM);
          // delay(250);
        }
        else if (hallHeatOnMM < 9 && hallHeatOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(hallHeatOnMM);
          // delay(250);
        }
        if (hallHeatOnMM <= 255 && hallHeatOnMM >= 60) {
          hallHeatOnMM = 59;
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = HALL_HEAT_ON_HH;
        lcd.clear();
      }
   /*   if (b & BUTTON_SELECT) {
      EEPROM.update(6, hallHeatOnHH);
        EEPROM.update(7, hallHeatOnMM);
        Serial.print("Ground/Hall/Heat/Main/On:"); Serial.print(EEPROM.read(6)); Serial.print("."); Serial.println(EEPROM.read(7));
        lcd.clear(); 
      } */
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
      if (hallHeatOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(hallHeatOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(hallHeatOffHH);
      }
      lcd.print(":");
      if (hallHeatOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(hallHeatOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(hallHeatOffMM);
      }
      if (b & BUTTON_UP) {
        hallHeatOffHH++;
        if (hallHeatOffHH >= 10 && hallHeatOffHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(hallHeatOffHH);
          // delay(250);
        }
        else if (hallHeatOffHH < 9 && hallHeatOffHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(hallHeatOffHH);
          // delay(250);
        }
        else if (hallHeatOffHH >= 24) {
          hallHeatOffHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        hallHeatOffHH--;
        if (hallHeatOffHH <= 255 && hallHeatOffHH >= 24) {
          hallHeatOffHH = 23;
        }
        if (hallHeatOffHH >= 10 && hallHeatOffHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(hallHeatOffHH);
          // delay(250);
        }
        else if (hallHeatOffHH < 9 && hallHeatOffHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(hallHeatOffHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = HALL_LIGHT_OFF_MM;
        lcd.clear();

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
      if (hallHeatOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(hallHeatOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(hallHeatOffHH);
      }
      lcd.print(":");
      if (hallHeatOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(hallHeatOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(hallHeatOffMM);
      }
      if (b & BUTTON_UP) {
        hallHeatOffMM++;
        if (hallHeatOffMM >= 10 && hallHeatOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(hallHeatOffMM);
          // delay(250);
        }
        else if (hallHeatOffMM < 9 && hallHeatOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(hallHeatOffMM);
          // delay(250);
        }
        else if (hallHeatOffMM > 59) {
          hallHeatOffMM = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        hallHeatOffMM--;
        if (hallHeatOffMM <= 255 && hallHeatOffMM >= 60) {
          hallHeatOffMM = 59;
        }
        if (hallHeatOffMM >= 10 && hallHeatOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(hallHeatOffMM);
          // delay(250);
        }
        else if (hallHeatOffMM < 9 && hallHeatOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(hallHeatOffMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = HALL_LIGHT_OFF_HH;
        lcd.clear();
      }
 /*     if (b & BUTTON_SELECT) {
        EEPROM.update(8, hallHeatOffHH);
        EEPROM.update(9, hallHeatOffMM);
        Serial.print("Ground/Hall/Heat/Main/Off:"); Serial.print(EEPROM.read(8)); Serial.print("."); Serial.println(EEPROM.read(9));
        lcd.clear(); 
      }*/ 
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
        currentState = KITCHEN_HEAT;
        lcd.clear();
      }
      else if (b & BUTTON_LEFT) {
        currentState = KITCHEN_LIGHT;
        lcd.clear();
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
        lcd.clear();
      }
      else if (b & BUTTON_RIGHT) {
        currentState = KITCHEN_LIGHT_ON_HH;
        lcd.clear();
      }
      else if (b & BUTTON_LEFT) {
        currentState = KITCHEN_LIGHT_OFF_HH;
        lcd.clear();
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
      lcd.print(kitchenLightLevel); lcd.print("%");
  /*    if (b & BUTTON_SELECT) {
        EEPROM.update(10, kitchenLightLevel);
        Serial.print("Ground/Kitchen/Light/Main/Level:"); Serial.println(EEPROM.read(10));
        lcd.clear(); 
      } */

      if (b & BUTTON_UP) {
        kitchenLightLevel++;
        if (kitchenLightLevel < 101) {
          lcd.setCursor(6, 1);
          lcd.print("    ");
          lcd.setCursor(6, 1);
          lcd.print(kitchenLightLevel); lcd.print("%");
        }
        else {
          kitchenLightLevel = 100;
        }
      }
      else if (b & BUTTON_DOWN) {
        kitchenLightLevel--;
        if (hallLightLevel <= 255 && hallLightLevel >= 101) {
          hallLightLevel = 0;
        }
        if (kitchenLightLevel > -1) {
          lcd.setCursor(6, 1);
          lcd.print("    ");
          lcd.setCursor(6, 1);
          lcd.print(kitchenLightLevel); lcd.print("%");
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
      if (kitchenLightOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(kitchenLightOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(kitchenLightOnHH);
      }
      lcd.print(":");
      if (kitchenLightOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(kitchenLightOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(kitchenLightOnMM);
      }
      if (b & BUTTON_UP) {
        kitchenLightOnHH++;
        if (kitchenLightOnHH >= 10 && kitchenLightOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(kitchenLightOnHH);
          // delay(250);
        }
        else if (kitchenLightOnHH < 9 && kitchenLightOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(kitchenLightOnHH);
          // delay(250);
        }
        else if (kitchenLightOnHH >= 24) {
          kitchenLightOnHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        kitchenLightOnHH--;
        if (kitchenLightOnHH <= 255 && kitchenLightOnHH >= 24) {
          kitchenLightOnHH = 23;
        }
        if (kitchenLightOnHH >= 10 && kitchenLightOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(kitchenLightOnHH);
          // delay(250);
        }
        else if (kitchenLightOnHH < 9 && kitchenLightOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(kitchenLightOnHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = KITCHEN_LIGHT_ON_MM;
        lcd.clear();

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
      if (kitchenLightOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(kitchenLightOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(kitchenLightOnHH);
      }
      lcd.print(":");
      if (kitchenLightOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(kitchenLightOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(kitchenLightOnMM);
      }
      if (b & BUTTON_UP) {
        kitchenLightOnMM++;
        if (kitchenLightOnMM >= 10 && kitchenLightOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(kitchenLightOnMM);
          // delay(250);
        }
        else if (kitchenLightOnMM < 9 && kitchenLightOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(kitchenLightOnMM);
          // delay(250);
        }
        else if (kitchenLightOnMM > 60) {
          kitchenLightOnMM = 0;
          kitchenLightOnMM++;
        }
      }
      if (b & BUTTON_DOWN) {
        kitchenLightOnMM--;
        if (kitchenLightOnMM <= 255 && kitchenLightOnMM >= 60) {
          kitchenLightOnMM = 59;
        }
        if (kitchenLightOnMM >= 10 && kitchenLightOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(kitchenLightOnMM);
          // delay(250);
        }
        else if (kitchenLightOnMM < 9 && kitchenLightOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(kitchenLightOnMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = KITCHEN_LIGHT_ON_HH;
        lcd.clear();
      }
 /*     if (b & BUTTON_SELECT) {
        EEPROM.update(11, kitchenLightOnHH);
        EEPROM.update(12, kitchenLightOnMM);
        Serial.print("Ground/Kitchen/Light/Main/On:"); Serial.print(EEPROM.read(11)); Serial.print("."); Serial.println(EEPROM.read(12));
        lcd.clear();
      }*/
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
      if (kitchenLightOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(kitchenLightOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(kitchenLightOffHH);
      }
      lcd.print(":");
      if (kitchenLightOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(kitchenLightOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(kitchenLightOffMM);
      }
      if (b & BUTTON_UP) {
        kitchenLightOffHH++;
        if (kitchenLightOffHH >= 10 && kitchenLightOffHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(kitchenLightOffHH);
          // delay(250);
        }
        else if (kitchenLightOffHH < 9 && kitchenLightOffHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(kitchenLightOffHH);
          // delay(250);
        }
        else if (kitchenLightOffHH >= 24) {
          kitchenLightOffHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        kitchenLightOffHH--;
        if (kitchenLightOffHH <= 255 && kitchenLightOffHH >= 24) {
          kitchenLightOffHH = 23;
        }
        if (kitchenLightOffHH >= 10 && kitchenLightOffHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(kitchenLightOffHH);
          // delay(250);
        }
        else if (kitchenLightOffHH < 9 && kitchenLightOffHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(kitchenLightOffHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = KITCHEN_LIGHT_OFF_MM;
        lcd.clear();

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
      if (kitchenLightOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(kitchenLightOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(kitchenLightOffHH);
      }
      lcd.print(":");
      if (kitchenLightOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(kitchenLightOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(kitchenLightOffMM);
      }
      if (b & BUTTON_UP) {
        kitchenLightOffMM++;
        if (kitchenLightOffMM >= 10 && kitchenLightOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(kitchenLightOffMM);
          // delay(250);
        }
        else if (kitchenLightOffMM < 9 && kitchenLightOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(kitchenLightOffMM);
          // delay(250);
        }
        else if (kitchenLightOffMM > 59) {
          kitchenLightOffMM = 0;
          kitchenLightOffMM++;
        }
      }
      if (b & BUTTON_DOWN) {
        kitchenLightOffMM--;
        if (kitchenLightOffMM <= 255 && kitchenLightOffMM >= 60) {
          kitchenLightOffMM = 59;
        }
        if (kitchenLightOffMM >= 10 && kitchenLightOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(kitchenLightOffMM);
          // delay(250);
        }
        else if (kitchenLightOffMM < 9 && kitchenLightOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(kitchenLightOffMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = KITCHEN_LIGHT_OFF_HH;
        lcd.clear();
      }
    /*  if (b & BUTTON_SELECT) {
        EEPROM.update(13, kitchenLightOffHH);
        EEPROM.update(14, kitchenLightOffHH);
        Serial.print("Ground/Kitchen/Light/Main/Off:"); Serial.print(EEPROM.read(13)); Serial.print("."); Serial.println(EEPROM.read(14));
        lcd.clear();
      } */
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
        currentState = KITCHEN_HEAT_LEVEL;
        lcd.clear();
      }
      else if (b & BUTTON_RIGHT) {
        currentState = KITCHEN_HEAT_ON_HH;
        lcd.clear();
      }
      else if (b & BUTTON_LEFT) {
        currentState = KITCHEN_HEAT_OFF_HH;
        lcd.clear();
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
      lcd.print(kitchenHeatLevel); lcd.print("%");
  /*    if (b & BUTTON_SELECT) {
        EEPROM.update(15, kitchenHeatLevel);
        Serial.print("Ground/Kitchen/Heat/Main/Level:"); Serial.println(EEPROM.read(15));
        lcd.clear();
      }*/

      if (b & BUTTON_UP) {
        kitchenHeatLevel++;
        if (kitchenHeatLevel < 101) {
          lcd.setCursor(6, 1);
          lcd.print("    ");
          lcd.setCursor(6, 1);
          lcd.print(kitchenHeatLevel); lcd.print("%");
        }
        else {
          kitchenHeatLevel = 100;
        }
      }
      else if (b & BUTTON_DOWN) {
        kitchenHeatLevel--;
        if (kitchenHeatLevel <= 255 && kitchenHeatLevel >= 101) {
          kitchenHeatLevel = 0;
        }
        if (kitchenHeatLevel > -1) {
          lcd.setCursor(6, 1);
          lcd.print("   ");
          lcd.setCursor(6, 1);
          lcd.print(kitchenHeatLevel); lcd.print("%");
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
      if (kitchenHeatOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(kitchenHeatOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(kitchenHeatOnHH);
      }
      lcd.print(":");
      if (kitchenHeatOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(kitchenHeatOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(kitchenHeatOnMM);
      }
      if (b & BUTTON_UP) {
        kitchenHeatOnHH++;
        if (kitchenHeatOnHH >= 10 && kitchenHeatOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(kitchenHeatOnHH);
          // delay(250);
        }
        else if (kitchenHeatOnHH < 9 && kitchenHeatOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(kitchenHeatOnHH);
          // delay(250);
        }
        else if (kitchenHeatOnHH >= 24) {
          kitchenHeatOnHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        kitchenHeatOnHH--;
        if (kitchenHeatOnHH <= 255 && kitchenHeatOnHH >= 24) {
          kitchenHeatOnHH = 23;
        }
        if (kitchenHeatOnHH >= 10 && kitchenHeatOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(kitchenHeatOnHH);
          // delay(250);
        }
        else if (kitchenHeatOnHH < 9 && kitchenHeatOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(kitchenHeatOnHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = KITCHEN_LIGHT_ON_MM;
        lcd.clear();

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
      if (kitchenHeatOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(kitchenHeatOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(kitchenHeatOnHH);
      }
      lcd.print(":");
      if (kitchenHeatOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(kitchenHeatOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(kitchenHeatOnMM);
      }
      if (b & BUTTON_UP) {
        kitchenHeatOnMM++;
        if (kitchenHeatOnMM >= 10 && kitchenHeatOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(kitchenHeatOnMM);
          // delay(250);
        }
        else if (kitchenHeatOnMM < 9 && kitchenHeatOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(kitchenHeatOnMM);
          // delay(250);
        }
        else if (kitchenHeatOnMM > 59) {
          kitchenHeatOnMM = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        kitchenHeatOnMM--;
        if (kitchenHeatOnMM <= 255 && kitchenHeatOnMM >= 60) {
          kitchenHeatOnMM = 59;
        }
        if (kitchenHeatOnMM >= 10 && kitchenHeatOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(kitchenHeatOnMM);
          // delay(250);
        }
        else if (kitchenHeatOnMM < 9 && kitchenHeatOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(kitchenHeatOnMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = KITCHEN_LIGHT_ON_HH;
        lcd.clear();
      }
  /*    if (b & BUTTON_SELECT) {
        EEPROM.update(16, kitchenHeatOnHH);
        EEPROM.update(17, kitchenHeatOnMM);
        Serial.print("Ground/Kitchen/Heat/Main/On:"); Serial.print(EEPROM.read(16)); Serial.print("."); Serial.println(EEPROM.read(17));
        lcd.clear();
      }*/
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
      if (kitchenHeatOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(kitchenHeatOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(kitchenHeatOffHH);
      }
      lcd.print(":");
      if (kitchenHeatOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(kitchenHeatOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(kitchenHeatOffMM);
      }
      if (b & BUTTON_UP) {
        kitchenHeatOffHH++;
        if (kitchenHeatOffHH >= 10 && kitchenHeatOffHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(kitchenHeatOffHH);
          // delay(250);
        }
        else if (kitchenHeatOffHH < 9 && kitchenHeatOffHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(kitchenHeatOffHH);
          // delay(250);
        }
        else if (kitchenHeatOffHH >= 24) {
          kitchenHeatOffHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        kitchenHeatOffHH--;
        if (kitchenHeatOffHH <= 255 && kitchenHeatOffHH >= 24) {
          kitchenHeatOffHH = 23;
        }
        if (kitchenHeatOffHH >= 10 && kitchenHeatOffHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(kitchenHeatOffHH);
          // delay(250);
        }
        else if (kitchenHeatOffHH < 9 && kitchenHeatOffHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(kitchenHeatOffHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = KITCHEN_LIGHT_OFF_MM;
        lcd.clear();
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
      if (kitchenHeatOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(kitchenHeatOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(kitchenHeatOffHH);
      }
      lcd.print(":");
      if (kitchenHeatOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(kitchenHeatOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(kitchenHeatOffMM);
      }
      if (b & BUTTON_UP) {
        kitchenHeatOffMM++;
        if (kitchenHeatOffMM >= 10 && kitchenHeatOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(kitchenHeatOffMM);
          // delay(250);
        }
        else if (kitchenHeatOffMM < 9 && kitchenHeatOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(kitchenHeatOffMM);
          // delay(250);
        }
        else if (kitchenHeatOffMM > 59) {
          kitchenHeatOffMM = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        kitchenHeatOffMM--;
        if (kitchenHeatOffMM <= 255 && kitchenHeatOffMM >= 60) {
          kitchenHeatOffMM = 59;
        }
        if (kitchenHeatOffMM >= 10 && kitchenHeatOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(kitchenHeatOffMM);
          // delay(250);
        }
        else if (kitchenHeatOffMM < 9 && kitchenHeatOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(kitchenHeatOffMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = KITCHEN_LIGHT_OFF_HH;
        lcd.clear();
      }
   /*   if (b & BUTTON_SELECT) {
        EEPROM.update(18, kitchenHeatOffHH);
        EEPROM.update(19, kitchenHeatOffMM);
        Serial.print("Ground/Kitchen/Heat/Main/Off:"); Serial.print(EEPROM.read(18)); Serial.print("."); Serial.println(EEPROM.read(19));
        lcd.clear();
      }*/
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
        lcd.clear();
      }
      else if (b & BUTTON_LEFT) {
        currentState = LIVING_ROOM_LIGHT;
        lcd.clear();
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
        currentState = LIVING_ROOM_LIGHT_LEVEL;
        lcd.clear();
      }
      else if (b & BUTTON_RIGHT) {
        currentState = LIVING_ROOM_LIGHT_ON_HH;
        lcd.clear();
      }
      else if (b & BUTTON_LEFT) {
        currentState = LIVING_ROOM_LIGHT_OFF_HH;
        lcd.clear();
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
      lcd.print(livingRoomLightLevel); lcd.print("%");
   /*   if (b & BUTTON_SELECT) {
        EEPROM.update(20, livingRoomLightLevel);
        Serial.print("Ground/Living Room/Light/Main/Level:"); Serial.println(EEPROM.read(20));
        lcd.clear(); 
      }*/

      if (b & BUTTON_UP) {
        livingRoomLightLevel++;
        if (livingRoomLightLevel < 101) {
          lcd.setCursor(6, 1);
          lcd.print("    ");
          lcd.setCursor(6, 1);
          lcd.print(livingRoomLightLevel); lcd.print("%");
        }
        else {
          livingRoomLightLevel = 100;
        }
      }
      else if (b & BUTTON_DOWN) {
        livingRoomLightLevel--;
        if (livingRoomLightLevel <= 255 && livingRoomLightLevel >= 101) {
          livingRoomLightLevel = 0;
        }
        if (livingRoomLightLevel > -1) {
          lcd.setCursor(6, 1);
          lcd.print("    ");
          lcd.setCursor(6, 1);
          lcd.print(livingRoomLightLevel); lcd.print("%");
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
      if (livingRoomLightOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(livingRoomLightOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(livingRoomLightOnHH);
      }
      lcd.print(":");
      if (livingRoomLightOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(livingRoomLightOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(livingRoomLightOnMM);
      }
      if (b & BUTTON_UP) {
        livingRoomLightOnHH++;
        if (livingRoomLightOnHH >= 10 && livingRoomLightOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(livingRoomLightOnHH);
          // delay(250);
        }
        else if (livingRoomLightOnHH < 9 && livingRoomLightOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(livingRoomLightOnHH);
          // delay(250);
        }
        else if (livingRoomLightOnHH >= 24) {
          livingRoomLightOnHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        livingRoomLightOnHH--;
        if (livingRoomLightOnHH <= 255 && livingRoomLightOnHH >= 24) {
          livingRoomLightOnHH = 23;
        }
        if (livingRoomLightOnHH >= 10 && livingRoomLightOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(livingRoomLightOnHH);
          // delay(250);
        }
        else if (livingRoomLightOnHH < 9 && livingRoomLightOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(livingRoomLightOnHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = LIVING_ROOM_LIGHT_ON_MM;
        lcd.clear();

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
      if (livingRoomLightOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(livingRoomLightOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(livingRoomLightOnHH);
      }
      lcd.print(":");
      if (livingRoomLightOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(livingRoomLightOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(livingRoomLightOnMM);
      }
      if (b & BUTTON_UP) {
        livingRoomLightOnMM++;
        if (livingRoomLightOnMM >= 10 && livingRoomLightOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(livingRoomLightOnMM);
          // delay(250);
        }
        else if (livingRoomLightOnMM < 9 && livingRoomLightOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(livingRoomLightOnMM);
          // delay(250);
        }
        else if (livingRoomLightOnMM > 60) {
          livingRoomLightOnMM = 0;
          livingRoomLightOnMM++;
        }
      }
      if (b & BUTTON_DOWN) {
        livingRoomLightOnMM--;
        if (livingRoomLightOnMM <= 255 && livingRoomLightOnMM >= 60) {
          livingRoomLightOnMM = 59;
        }
        if (livingRoomLightOnMM >= 10 && livingRoomLightOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(livingRoomLightOnMM);
          // delay(250);
        }
        else if (livingRoomLightOnMM < 9 && livingRoomLightOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(livingRoomLightOnMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = LIVING_ROOM_LIGHT_ON_HH;
        lcd.clear();
      }
 /*     if (b & BUTTON_SELECT) {
        EEPROM.update(21, livingRoomLightOnHH);
        EEPROM.update(22, livingRoomLightOnMM);
        Serial.print("Ground/Living Room/Light/Main/On:"); Serial.print(EEPROM.read(21)); Serial.print("."); Serial.println(EEPROM.read(22));
        lcd.clear();
      }*/
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
      if (livingRoomLightOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(livingRoomLightOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(livingRoomLightOnHH);
      }
      lcd.print(":");
      if (livingRoomLightOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(livingRoomLightOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(livingRoomLightOnMM);
      }
      if (b & BUTTON_UP) {
        livingRoomLightOnHH++;
        if (livingRoomLightOnHH >= 10 && livingRoomLightOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(livingRoomLightOnHH);
          // delay(250);
        }
        else if (livingRoomLightOnHH < 9 && livingRoomLightOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(livingRoomLightOnHH);
          // delay(250);
        }
        else if (livingRoomLightOnHH >= 24) {
          livingRoomLightOnHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        livingRoomLightOnHH--;
        if (livingRoomLightOnHH <= 255 && livingRoomLightOnHH >= 24) {
          livingRoomLightOnHH = 23;
        }
        if (livingRoomLightOnHH >= 10 && livingRoomLightOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(livingRoomLightOnHH);
          // delay(250);
        }
        else if (livingRoomLightOnHH < 9 && livingRoomLightOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(livingRoomLightOnHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = LIVING_ROOM_LIGHT_OFF_MM;
        lcd.clear();

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
      if (livingRoomLightOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(livingRoomLightOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(livingRoomLightOffHH);
      }
      lcd.print(":");
      if (livingRoomLightOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(livingRoomLightOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(livingRoomLightOffMM);
      }
      if (b & BUTTON_UP) {
        livingRoomLightOffMM++;
        if (livingRoomLightOffMM >= 10 && livingRoomLightOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(livingRoomLightOffMM);
          // delay(250);
        }
        else if (livingRoomLightOffMM < 9 && livingRoomLightOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(livingRoomLightOffMM);
          // delay(250);
        }
        else if (livingRoomLightOffMM > 59) {
          livingRoomLightOffMM = 0;
          livingRoomLightOffMM++;
        }
      }
      if (b & BUTTON_DOWN) {
        livingRoomLightOffMM--;
        if (livingRoomLightOffMM <= 255 && livingRoomLightOffMM >= 60) {
          livingRoomLightOffMM = 59;
        }
        if (livingRoomLightOffMM >= 10 && livingRoomLightOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(livingRoomLightOffMM);
          // delay(250);
        }
        else if (livingRoomLightOffMM < 9 && livingRoomLightOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(livingRoomLightOffMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = LIVING_ROOM_LIGHT_OFF_HH;
        lcd.clear();
      }
  /*    if (b & BUTTON_SELECT) {
        EEPROM.update(23, livingRoomLightOffHH);
        EEPROM.update(24, livingRoomLightOffMM);
        Serial.print("Ground/Living Room/Light/Main/Off:"); Serial.print(EEPROM.read(23)); Serial.print("."); Serial.println(EEPROM.read(24));
        lcd.clear();
      }*/
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
        currentState = LIVING_ROOM_HEAT_LEVEL;
        lcd.clear();
      }
      else if (b & BUTTON_RIGHT) {
        currentState = LIVING_ROOM_HEAT_ON_HH;
        lcd.clear();
      }
      else if (b & BUTTON_LEFT) {
        currentState = LIVING_ROOM_HEAT_OFF_HH;
        lcd.clear();
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
      lcd.print(livingRoomHeatLevel); lcd.print("%");
 /*     if (b & BUTTON_SELECT) {
        EEPROM.update(25, livingRoomHeatLevel);
        Serial.print("Ground/Living Room/Heat/Main/Level:"); Serial.println(EEPROM.read(25));
        lcd.clear();
      }*/

      if (b & BUTTON_UP) {
        livingRoomHeatLevel++;
        if (livingRoomHeatLevel < 101) {
          lcd.setCursor(6, 1);
          lcd.print("    ");
          lcd.setCursor(6, 1);
          lcd.print(livingRoomHeatLevel); lcd.print("%");
        }
        else {
          livingRoomHeatLevel = 100;
        }
      }
      else if (b & BUTTON_DOWN) {
        livingRoomHeatLevel--;
        if (livingRoomHeatLevel <= 255 && livingRoomHeatLevel >= 101) {
          livingRoomHeatLevel = 0;
        }
        if (livingRoomHeatLevel > -1) {
          lcd.setCursor(6, 1);
          lcd.print("   ");
          lcd.setCursor(6, 1);
          lcd.print(livingRoomHeatLevel); lcd.print("%");
        }
      }
      break;


    // Heat ON state HOURS
    case LIVING_ROOM_HEAT_ON_HH:
      lcd.setCursor(0, 0);
      lcd.print("LVNG RM HEAT ON");
      lcd.setCursor(0, 1);
      lcd.write(2);
      lcd.setCursor(15, 1);
      lcd.write(3);
      lcd.setCursor(4, 1);
      lcd.write(1);
      lcd.setCursor(5, 1);
      if (livingRoomHeatOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(livingRoomHeatOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(livingRoomHeatOnHH);
      }
      lcd.print(":");
      if (livingRoomHeatOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(livingRoomHeatOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(livingRoomHeatOnMM);
      }
      if (b & BUTTON_UP) {
        livingRoomHeatOnHH++;
        if (livingRoomHeatOnHH >= 10 && livingRoomHeatOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(livingRoomHeatOnHH);
          // delay(250);
        }
        else if (livingRoomHeatOnHH < 9 && livingRoomHeatOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(livingRoomHeatOnHH);
          // delay(250);
        }
        else if (livingRoomHeatOnHH >= 24) {
          livingRoomHeatOnHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        livingRoomHeatOnHH--;
        if (livingRoomHeatOnHH <= 255 && livingRoomHeatOnHH >= 24) {
          livingRoomHeatOnHH = 23;
        }
        if (livingRoomHeatOnHH >= 10 && livingRoomHeatOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(livingRoomHeatOnHH);
          // delay(250);
        }
        else if (livingRoomHeatOnHH < 9 && livingRoomHeatOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(livingRoomHeatOnHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = LIVING_ROOM_LIGHT_ON_MM;
        lcd.clear();

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
      if (livingRoomHeatOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(livingRoomHeatOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(livingRoomHeatOnHH);
      }
      lcd.print(":");
      if (livingRoomHeatOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(livingRoomHeatOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(livingRoomHeatOnMM);
      }
      if (b & BUTTON_UP) {
        livingRoomHeatOnMM++;
        if (livingRoomHeatOnMM >= 10 && livingRoomHeatOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(livingRoomHeatOnMM);
          // delay(250);
        }
        else if (livingRoomHeatOnMM < 9 && livingRoomHeatOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(livingRoomHeatOnMM);
          // delay(250);
        }
        else if (livingRoomHeatOnMM > 59) {
          livingRoomHeatOnMM = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        livingRoomHeatOnMM--;
        if (livingRoomHeatOnMM <= 255 && livingRoomHeatOnMM >= 60) {
          livingRoomHeatOnMM = 59;
        }
        if (livingRoomHeatOnMM >= 10 && livingRoomHeatOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(livingRoomHeatOnMM);
          // delay(250);
        }
        else if (livingRoomHeatOnMM < 9 && livingRoomHeatOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(livingRoomHeatOnMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = LIVING_ROOM_LIGHT_ON_HH;
        lcd.clear();
      }
/*      if (b & BUTTON_SELECT) {
        EEPROM.update(26, livingRoomHeatOnHH);
        EEPROM.update(27, livingRoomHeatOnMM);
        Serial.print("Ground/Living Room/Heat/Main/On:"); Serial.print(EEPROM.read(26)); Serial.print("."); Serial.println(EEPROM.read(27));
        lcd.clear();
      }*/
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
      if (livingRoomHeatOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(livingRoomHeatOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(livingRoomHeatOffHH);
      }
      lcd.print(":");
      if (livingRoomHeatOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(livingRoomHeatOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(livingRoomHeatOffMM);
      }
      if (b & BUTTON_UP) {
        livingRoomHeatOffHH++;
        if (livingRoomHeatOffHH >= 10 && livingRoomHeatOffHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(livingRoomHeatOffHH);
          // delay(250);
        }
        else if (livingRoomHeatOffHH < 9 && livingRoomHeatOffHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(livingRoomHeatOffHH);
          // delay(250);
        }
        else if (livingRoomHeatOffHH >= 24) {
          livingRoomHeatOffHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        livingRoomHeatOffHH--;
        if (livingRoomHeatOffHH <= 255 && livingRoomHeatOffHH >= 24) {
          livingRoomHeatOffHH = 23;
        }
        if (livingRoomHeatOffHH >= 10 && livingRoomHeatOffHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(livingRoomHeatOffHH);
          // delay(250);
        }
        else if (livingRoomHeatOffHH < 9 && livingRoomHeatOffHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(livingRoomHeatOffHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = LIVING_ROOM_LIGHT_OFF_MM;
        lcd.clear();

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
      if (livingRoomHeatOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(livingRoomHeatOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(livingRoomHeatOffHH);
      }
      lcd.print(":");
      if (livingRoomHeatOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(livingRoomHeatOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(livingRoomHeatOffMM);
      }
      if (b & BUTTON_UP) {
        livingRoomHeatOffMM++;
        if (livingRoomHeatOffMM >= 10 && livingRoomHeatOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(livingRoomHeatOffMM);
          // delay(250);
        }
        else if (livingRoomHeatOffMM < 9 && livingRoomHeatOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(livingRoomHeatOffMM);
          // delay(250);
        }
        else if (livingRoomHeatOffMM > 59) {
          livingRoomHeatOffMM = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        livingRoomHeatOffMM--;
        if (livingRoomHeatOffMM <= 255 && livingRoomHeatOffMM >= 60) {
          livingRoomHeatOffMM = 59;
        }
        if (livingRoomHeatOffMM >= 10 && livingRoomHeatOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(livingRoomHeatOffMM);
          // delay(250);
        }
        else if (livingRoomHeatOffMM < 9 && livingRoomHeatOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(livingRoomHeatOffMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = LIVING_ROOM_LIGHT_OFF_HH;
        lcd.clear();
      }
 /*     if (b & BUTTON_SELECT) {
        EEPROM.update(28, livingRoomHeatOffHH);
        EEPROM.update(29, livingRoomHeatOffMM);
        Serial.print("Ground/Living Room/Heat/Main/Off:"); Serial.print(EEPROM.read(28)); Serial.print("."); Serial.println(EEPROM.read(29));
        lcd.clear();
      }*/
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
        currentState = BEDROOM_1_HEAT;
        lcd.clear();
      }
      else if (b & BUTTON_LEFT) {
        currentState = BEDROOM_1_LIGHT;
        lcd.clear();
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
        currentState = BEDROOM_1_LIGHT_LEVEL;
        lcd.clear();
      }
      else if (b & BUTTON_RIGHT) {
        currentState = BEDROOM_1_LIGHT_ON_HH;
        lcd.clear();
      }
      else if (b & BUTTON_LEFT) {
        currentState = BEDROOM_1_LIGHT_OFF_HH;
        lcd.clear();
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
      lcd.print(bedroom1LightLevel); lcd.print("%");
 /*     if (b & BUTTON_SELECT) {
        EEPROM.update(30, bedroom1LightLevel);
        Serial.print("First/Bedroom 1/Light/Main/Level:"); Serial.println(EEPROM.read(30));
        lcd.clear();
      }*/

      if (b & BUTTON_UP) {
        bedroom1LightLevel++;
        if (bedroom1LightLevel < 101) {
          lcd.setCursor(6, 1);
          lcd.print("    ");
          lcd.setCursor(6, 1);
          lcd.print(bedroom1LightLevel); lcd.print("%");
        }
        else {
          bedroom1LightLevel = 100;
        }
      }
      else if (b & BUTTON_DOWN) {
        bedroom1LightLevel--;
        if (bedroom1LightLevel <= 255 && bedroom1LightLevel >= 101) {
          bedroom1LightLevel = 0;
        }
        if (bedroom1LightLevel > -1) {
          lcd.setCursor(6, 1);
          lcd.print("    ");
          lcd.setCursor(6, 1);
          lcd.print(bedroom1LightLevel); lcd.print("%");
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
      if (bedroom1LightOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bedroom1LightOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bedroom1LightOnHH);
      }
      lcd.print(":");
      if (bedroom1LightOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bedroom1LightOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bedroom1LightOnMM);
      }
      if (b & BUTTON_UP) {
        bedroom1LightOnHH++;
        if (bedroom1LightOnHH >= 10 && bedroom1LightOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bedroom1LightOnHH);
          // delay(250);
        }
        else if (bedroom1LightOnHH < 9 && bedroom1LightOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bedroom1LightOnHH);
          // delay(250);
        }
        else if (bedroom1LightOnHH >= 24) {
          bedroom1LightOnHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        bedroom1LightOnHH--;
        if (bedroom1LightOnHH <= 255 && bedroom1LightOnHH >= 24) {
          bedroom1LightOnHH = 23;
        }
        if (bedroom1LightOnHH >= 10 && bedroom1LightOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bedroom1LightOnHH);
          // delay(250);
        }
        else if (bedroom1LightOnHH < 9 && bedroom1LightOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bedroom1LightOnHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = BEDROOM_1_LIGHT_ON_MM;
        lcd.clear();

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
      if (bedroom1LightOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bedroom1LightOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bedroom1LightOnHH);
      }
      lcd.print(":");
      if (bedroom1LightOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bedroom1LightOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bedroom1LightOnMM);
      }
      if (b & BUTTON_UP) {
        bedroom1LightOnMM++;
        if (bedroom1LightOnMM >= 10 && bedroom1LightOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bedroom1LightOnMM);
          // delay(250);
        }
        else if (bedroom1LightOnMM < 9 && bedroom1LightOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bedroom1LightOnMM);
          // delay(250);
        }
        else if (bedroom1LightOnMM > 60) {
          bedroom1LightOnMM = 0;
          bedroom1LightOnMM++;
        }
      }
      if (b & BUTTON_DOWN) {
        bedroom1LightOnMM--;
        if (bedroom1LightOnMM <= 255 && bedroom1LightOnMM >= 60) {
          bedroom1LightOnMM = 59;
        }
        if (bedroom1LightOnMM >= 10 && bedroom1LightOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bedroom1LightOnMM);
          // delay(250);
        }
        else if (bedroom1LightOnMM < 9 && bedroom1LightOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bedroom1LightOnMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = BEDROOM_1_LIGHT_ON_HH;
        lcd.clear();
      }
  /*    if (b & BUTTON_SELECT) {
        EEPROM.update(31, bedroom1LightOnHH);
        EEPROM.update(32, bedroom1LightOnMM);
        Serial.print("First/Bedroom 1/Light/Main/On:"); Serial.print(EEPROM.read(31)); Serial.print("."); Serial.println(EEPROM.read(32));
        lcd.clear();
      }*/
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
      if (bedroom1LightOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bedroom1LightOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bedroom1LightOffHH);
      }
      lcd.print(":");
      if (bedroom1LightOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bedroom1LightOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bedroom1LightOffMM);
      }
      if (b & BUTTON_UP) {
        bedroom1LightOffHH++;
        if (bedroom1LightOffHH >= 10 && bedroom1LightOffHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bedroom1LightOffHH);
          // delay(250);
        }
        else if (bedroom1LightOffHH < 9 && bedroom1LightOffHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bedroom1LightOffHH);
          // delay(250);
        }
        else if (bedroom1LightOffHH >= 24) {
          bedroom1LightOffHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        bedroom1LightOffHH--;
        if (bedroom1LightOffHH <= 255 && bedroom1LightOffHH >= 24) {
          bedroom1LightOffHH = 23;
        }
        if (bedroom1LightOffHH >= 10 && bedroom1LightOffHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bedroom1LightOffHH);
          // delay(250);
        }
        else if (bedroom1LightOffHH < 9 && bedroom1LightOffHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bedroom1LightOffHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = BEDROOM_1_LIGHT_OFF_MM;
        lcd.clear();

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
      if (bedroom1LightOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bedroom1LightOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bedroom1LightOffHH);
      }
      lcd.print(":");
      if (bedroom1LightOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bedroom1LightOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bedroom1LightOffMM);
      }
      if (b & BUTTON_UP) {
        bedroom1LightOffMM++;
        if (bedroom1LightOffMM >= 10 && bedroom1LightOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bedroom1LightOffMM);
          // delay(250);
        }
        else if (bedroom1LightOffMM < 9 && bedroom1LightOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bedroom1LightOffMM);
          // delay(250);
        }
        else if (bedroom1LightOffMM > 59) {
          bedroom1LightOffMM = 0;
          bedroom1LightOffMM++;
        }
      }
      if (b & BUTTON_DOWN) {
        bedroom1LightOffMM--;
        if (bedroom1LightOffMM <= 255 && bedroom1LightOffMM >= 60) {
          bedroom1LightOffMM = 59;
        }
        if (bedroom1LightOffMM >= 10 && bedroom1LightOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bedroom1LightOffMM);
          // delay(250);
        }
        else if (bedroom1LightOffMM < 9 && bedroom1LightOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bedroom1LightOffMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = BEDROOM_1_LIGHT_OFF_HH;
        lcd.clear();
      }
/*      if (b & BUTTON_SELECT) {
        EEPROM.update(33, bedroom1LightOffHH);
        EEPROM.update(34, bedroom1LightOffMM);
        Serial.print("First/Bedroom 1/Light/Main/Off:"); Serial.print(EEPROM.read(33)); Serial.print("."); Serial.println(EEPROM.read(34));
        lcd.clear();
      }*/
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
        currentState = BEDROOM_1_HEAT_LEVEL;
        lcd.clear();
      }
      else if (b & BUTTON_RIGHT) {
        currentState = BEDROOM_1_HEAT_ON_HH;
        lcd.clear();
      }
      else if (b & BUTTON_LEFT) {
        currentState = BEDROOM_1_HEAT_OFF_HH;
        lcd.clear();
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
      lcd.print(bedroom1HeatLevel); lcd.print("%");
 /*     if (b & BUTTON_SELECT) {
        EEPROM.update(35, bedroom1HeatLevel);
        Serial.print("First/Bedroom 1/Light/Main/Level:"); Serial.println(EEPROM.read(35));
        lcd.clear();
      }*/

      if (b & BUTTON_UP) {
        bedroom1HeatLevel++;
        if (bedroom1HeatLevel < 101) {
          lcd.setCursor(6, 1);
          lcd.print("    ");
          lcd.setCursor(6, 1);
          lcd.print(bedroom1HeatLevel); lcd.print("%");
        }
        else {
          bedroom1HeatLevel = 100;
        }
      }
      else if (b & BUTTON_DOWN) {
        bedroom1HeatLevel--;
        if (bedroom1HeatLevel <= 255 && bedroom1HeatLevel >= 101) {
          bedroom1HeatLevel = 0;
        }
        if (bedroom1HeatLevel > -1) {
          lcd.setCursor(6, 1);
          lcd.print("   ");
          lcd.setCursor(6, 1);
          lcd.print(bedroom1HeatLevel); lcd.print("%");
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
      if (bedroom1HeatOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bedroom1HeatOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bedroom1HeatOnHH);
      }
      lcd.print(":");
      if (bedroom1HeatOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bedroom1HeatOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bedroom1HeatOnMM);
      }
      if (b & BUTTON_UP) {
        bedroom1HeatOnHH++;
        if (bedroom1HeatOnHH >= 10 && bedroom1HeatOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bedroom1HeatOnHH);
          // delay(250);
        }
        else if (bedroom1HeatOnHH < 9 && bedroom1HeatOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bedroom1HeatOnHH);
          // delay(250);
        }
        else if (bedroom1HeatOnHH >= 24) {
          bedroom1HeatOnHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        bedroom1HeatOnHH--;
        if (bedroom1HeatOnHH <= 255 && bedroom1HeatOnHH >= 24) {
          bedroom1HeatOnHH = 23;
        }
        if (bedroom1HeatOnHH >= 10 && bedroom1HeatOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bedroom1HeatOnHH);
          // delay(250);
        }
        else if (bedroom1HeatOnHH < 9 && bedroom1HeatOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bedroom1HeatOnHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = BEDROOM_1_LIGHT_ON_MM;
        lcd.clear();

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
      if (bedroom1HeatOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bedroom1HeatOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bedroom1HeatOnHH);
      }
      lcd.print(":");
      if (bedroom1HeatOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bedroom1HeatOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bedroom1HeatOnMM);
      }
      if (b & BUTTON_UP) {
        bedroom1HeatOnMM++;
        if (bedroom1HeatOnMM >= 10 && bedroom1HeatOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bedroom1HeatOnMM);
          // delay(250);
        }
        else if (bedroom1HeatOnMM < 9 && bedroom1HeatOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bedroom1HeatOnMM);
          // delay(250);
        }
        else if (bedroom1HeatOnMM > 59) {
          bedroom1HeatOnMM = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        bedroom1HeatOnMM--;
        if (bedroom1HeatOnMM <= 255 && bedroom1HeatOnMM >= 60) {
          bedroom1HeatOnMM = 59;
        }
        if (bedroom1HeatOnMM >= 10 && bedroom1HeatOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bedroom1HeatOnMM);
          // delay(250);
        }
        else if (bedroom1HeatOnMM < 9 && bedroom1HeatOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bedroom1HeatOnMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = BEDROOM_1_LIGHT_ON_HH;
        lcd.clear();
      }
 /*     if (b & BUTTON_SELECT) {
        EEPROM.update(36, bedroom1HeatOnHH);
        EEPROM.update(37, bedroom1HeatOnMM);
        Serial.print("First/Bedroom 1/Heat/Main/On:"); Serial.print(EEPROM.read(36)); Serial.print("."); Serial.println(EEPROM.read(37));
        lcd.clear();
      }*/
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
      if (bedroom1HeatOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bedroom1HeatOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bedroom1HeatOffHH);
      }
      lcd.print(":");
      if (bedroom1HeatOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bedroom1HeatOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bedroom1HeatOffMM);
      }
      if (b & BUTTON_UP) {
        bedroom1HeatOffHH++;
        if (bedroom1HeatOffHH >= 10 && bedroom1HeatOffHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bedroom1HeatOffHH);
          // delay(250);
        }
        else if (bedroom1HeatOffHH < 9 && bedroom1HeatOffHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bedroom1HeatOffHH);
          // delay(250);
        }
        else if (bedroom1HeatOffHH >= 24) {
          bedroom1HeatOffHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        bedroom1HeatOffHH--;
        if (bedroom1HeatOffHH <= 255 && bedroom1HeatOffHH >= 24) {
          bedroom1HeatOffHH = 23;
        }
        if (bedroom1HeatOffHH >= 10 && bedroom1HeatOffHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bedroom1HeatOffHH);
          // delay(250);
        }
        else if (bedroom1HeatOffHH < 9 && bedroom1HeatOffHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bedroom1HeatOffHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = BEDROOM_1_LIGHT_OFF_MM;
        lcd.clear();

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
      if (bedroom1HeatOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bedroom1HeatOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bedroom1HeatOffHH);
      }
      lcd.print(":");
      if (bedroom1HeatOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bedroom1HeatOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bedroom1HeatOffMM);
      }
      if (b & BUTTON_UP) {
        bedroom1HeatOffMM++;
        if (bedroom1HeatOffMM >= 10 && bedroom1HeatOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bedroom1HeatOffMM);
          // delay(250);
        }
        else if (bedroom1HeatOffMM < 9 && bedroom1HeatOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bedroom1HeatOffMM);
          // delay(250);
        }
        else if (bedroom1HeatOffMM > 59) {
          bedroom1HeatOffMM = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        bedroom1HeatOffMM--;
        if (bedroom1HeatOffMM <= 255 && bedroom1HeatOffMM >= 60) {
          bedroom1HeatOffMM = 59;
        }
        if (bedroom1HeatOffMM >= 10 && bedroom1HeatOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bedroom1HeatOffMM);
          // delay(250);
        }
        else if (bedroom1HeatOffMM < 9 && bedroom1HeatOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bedroom1HeatOffMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = BEDROOM_1_LIGHT_OFF_HH;
        lcd.clear();
      }
  /*    if (b & BUTTON_SELECT) {
        EEPROM.update(38, bedroom1HeatOffHH);
        EEPROM.update(39, bedroom1HeatOffMM);
        Serial.print("First/Bedroom 1/Heat/Main/Off:"); Serial.print(EEPROM.read(38)); Serial.print("."); Serial.println(EEPROM.read(39));
        lcd.clear();
      }*/
      break;

    case BEDROOM_2:
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
        lcd.clear();
      }
      else if (b & BUTTON_LEFT) {
        currentState = BEDROOM_2_LIGHT;
        lcd.clear();
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
        lcd.clear();
      }
      else if (b & BUTTON_RIGHT) {
        currentState = BEDROOM_2_LIGHT_ON_HH;
        lcd.clear();
      }
      else if (b & BUTTON_LEFT) {
        currentState = BEDROOM_2_LIGHT_OFF_HH;
        lcd.clear();
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
      lcd.print(bedroom2LightLevel); lcd.print("%");
 /*     if (b & BUTTON_SELECT) {
        EEPROM.update(40, bedroom2LightLevel);
        Serial.print("First/Bedroom 2/Light/Main/Level:"); Serial.println(EEPROM.read(40));
        lcd.clear();
      }*/

      if (b & BUTTON_UP) {
        bedroom2LightLevel++;
        if (bedroom2LightLevel < 101) {
          lcd.setCursor(6, 1);
          lcd.print("    ");
          lcd.setCursor(6, 1);
          lcd.print(bedroom2LightLevel); lcd.print("%");
        }
        else {
          bedroom2LightLevel = 100;
        }
      }
      else if (b & BUTTON_DOWN) {
        bedroom2LightLevel--;
        if (bedroom2LightLevel <= 255 && bedroom2LightLevel >= 101) {
          bedroom2LightLevel = 0;
        }
        if (bedroom2LightLevel > -1) {
          lcd.setCursor(6, 1);
          lcd.print("    ");
          lcd.setCursor(6, 1);
          lcd.print(bedroom2LightLevel); lcd.print("%");
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
      if (bedroom2LightOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bedroom2LightOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bedroom2LightOnHH);
      }
      lcd.print(":");
      if (bedroom2LightOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bedroom2LightOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bedroom2LightOnMM);
      }
      if (b & BUTTON_UP) {
        bedroom2LightOnHH++;
        if (bedroom2LightOnHH >= 10 && bedroom2LightOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bedroom2LightOnHH);
          // delay(250);
        }
        else if (bedroom2LightOnHH < 9 && bedroom2LightOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bedroom2LightOnHH);
          // delay(250);
        }
        else if (bedroom2LightOnHH >= 24) {
          bedroom2LightOnHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        bedroom2LightOnHH--;
        if (bedroom2LightOnHH <= 255 && bedroom2LightOnHH >= 24) {
          bedroom2LightOnHH = 23;
        }
        if (bedroom2LightOnHH >= 10 && bedroom2LightOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bedroom2LightOnHH);
          // delay(250);
        }
        else if (bedroom2LightOnHH < 9 && bedroom2LightOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bedroom2LightOnHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = BEDROOM_2_LIGHT_ON_MM;
        lcd.clear();

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
      if (bedroom2LightOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bedroom2LightOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bedroom2LightOnHH);
      }
      lcd.print(":");
      if (bedroom2LightOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bedroom2LightOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bedroom2LightOnMM);
      }
      if (b & BUTTON_UP) {
        bedroom2LightOnMM++;
        if (bedroom2LightOnMM >= 10 && bedroom2LightOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bedroom2LightOnMM);
          // delay(250);
        }
        else if (bedroom2LightOnMM < 9 && bedroom2LightOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bedroom2LightOnMM);
          // delay(250);
        }
        else if (bedroom2LightOnMM > 60) {
          bedroom2LightOnMM = 0;
          bedroom2LightOnMM++;
        }
      }
      if (b & BUTTON_DOWN) {
        bedroom2LightOnMM--;
        if (bedroom2LightOnMM <= 255 && bedroom2LightOnMM >= 60) {
          bedroom2LightOnMM = 59;
        }
        if (bedroom2LightOnMM >= 10 && bedroom2LightOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bedroom2LightOnMM);
          // delay(250);
        }
        else if (bedroom2LightOnMM < 9 && bedroom2LightOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bedroom2LightOnMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = BEDROOM_2_LIGHT_ON_HH;
        lcd.clear();
      }
  /*    if (b & BUTTON_SELECT) {
        EEPROM.update(41, bedroom2LightOnHH);
        EEPROM.update(42, bedroom2LightOnMM);
        Serial.print("First/Bedroom 2/Light/Main/On:"); Serial.print(EEPROM.read(41)); Serial.print("."); Serial.println(EEPROM.read(42));
        lcd.clear();
      }*/
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
      if (bedroom2LightOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bedroom2LightOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bedroom2LightOffHH);
      }
      lcd.print(":");
      if (bedroom2LightOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bedroom2LightOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bedroom2LightOffMM);
      }
      if (b & BUTTON_UP) {
        bedroom2LightOffHH++;
        if (bedroom2LightOffHH >= 10 && bedroom2LightOffHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bedroom2LightOffHH);
          // delay(250);
        }
        else if (bedroom2LightOffHH < 9 && bedroom2LightOffHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bedroom2LightOffHH);
          // delay(250);
        }
        else if (bedroom2LightOffHH >= 24) {
          bedroom2LightOffHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        bedroom2LightOffHH--;
        if (bedroom2LightOffHH <= 255 && bedroom2LightOffHH >= 24) {
          bedroom2LightOffHH = 23;
        }
        if (bedroom2LightOffHH >= 10 && bedroom2LightOffHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bedroom2LightOffHH);
          // delay(250);
        }
        else if (bedroom2LightOffHH < 9 && bedroom2LightOffHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bedroom2LightOffHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = BEDROOM_2_LIGHT_OFF_MM;
        lcd.clear();

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
      if (bedroom2LightOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bedroom2LightOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bedroom2LightOffHH);
      }
      lcd.print(":");
      if (bedroom2LightOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bedroom2LightOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bedroom2LightOffMM);
      }
      if (b & BUTTON_UP) {
        bedroom2LightOffMM++;
        if (bedroom2LightOffMM >= 10 && bedroom2LightOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bedroom2LightOffMM);
          // delay(250);
        }
        else if (bedroom2LightOffMM < 9 && bedroom2LightOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bedroom2LightOffMM);
          // delay(250);
        }
        else if (bedroom2LightOffMM > 59) {
          bedroom2LightOffMM = 0;
          bedroom2LightOffMM++;
        }
      }
      if (b & BUTTON_DOWN) {
        bedroom2LightOffMM--;
        if (bedroom2LightOffMM <= 255 && bedroom2LightOffMM >= 60) {
          bedroom2LightOffMM = 59;
        }
        if (bedroom2LightOffMM >= 10 && bedroom2LightOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bedroom2LightOffMM);
          // delay(250);
        }
        else if (bedroom2LightOffMM < 9 && bedroom2LightOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bedroom2LightOffMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = BEDROOM_2_LIGHT_OFF_HH;
        lcd.clear();
      }
  /*    if (b & BUTTON_SELECT) {
        EEPROM.update(43, bedroom2LightOffHH);
        EEPROM.update(44, bedroom2LightOffMM);
        Serial.print("First/Bedroom 2/Light/Main/Off:"); Serial.print(EEPROM.read(43)); Serial.print("."); Serial.println(EEPROM.read(44));
        lcd.clear();
      }*/
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
        lcd.clear();
      }
      else if (b & BUTTON_RIGHT) {
        currentState = BEDROOM_2_HEAT_ON_HH;
        lcd.clear();
      }
      else if (b & BUTTON_LEFT) {
        currentState = BEDROOM_2_HEAT_OFF_HH;
        lcd.clear();
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
      lcd.print(bedroom2LightLevel); lcd.print("%");
 /*     if (b & BUTTON_SELECT) {
        EEPROM.update(45, bedroom2HeatLevel);
        Serial.print("First/Living Room/Light/Main/Level:"); Serial.println(EEPROM.read(45));
        lcd.clear();
      }*/

      if (b & BUTTON_UP) {
        bedroom2HeatLevel++;
        if (bedroom2HeatLevel < 101) {
          lcd.setCursor(6, 1);
          lcd.print("    ");
          lcd.setCursor(6, 1);
          lcd.print(bedroom2HeatLevel); lcd.print("%");
        }
        else {
          bedroom2HeatLevel = 100;
        }
      }
      else if (b & BUTTON_DOWN) {
        bedroom2HeatLevel--;
        if (bedroom2HeatLevel <= 255 && bedroom2HeatLevel >= 101) {
          bedroom2HeatLevel = 0;
        }
        if (bedroom2HeatLevel > -1) {
          lcd.setCursor(6, 1);
          lcd.print("   ");
          lcd.setCursor(6, 1);
          lcd.print(bedroom2HeatLevel); lcd.print("%");
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
      if (bedroom2HeatOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bedroom2HeatOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bedroom2HeatOnHH);
      }
      lcd.print(":");
      if (bedroom2HeatOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bedroom2HeatOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bedroom2HeatOnMM);
      }
      if (b & BUTTON_UP) {
        bedroom2HeatOnHH++;
        if (bedroom2HeatOnHH >= 10 && bedroom2HeatOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bedroom2HeatOnHH);
          // delay(250);
        }
        else if (bedroom2HeatOnHH < 9 && bedroom2HeatOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bedroom2HeatOnHH);
          // delay(250);
        }
        else if (bedroom2HeatOnHH >= 24) {
          bedroom2HeatOnHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        bedroom2HeatOnHH--;
        if (bedroom2HeatOnHH <= 255 && bedroom2HeatOnHH >= 24) {
          bedroom2HeatOnHH = 23;
        }
        if (bedroom2HeatOnHH >= 10 && bedroom2HeatOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bedroom2HeatOnHH);
          // delay(250);
        }
        else if (bedroom2HeatOnHH < 9 && bedroom2HeatOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bedroom2HeatOnHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = BEDROOM_2_LIGHT_ON_MM;
        lcd.clear();

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
      if (bedroom2HeatOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bedroom2HeatOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bedroom2HeatOnHH);
      }
      lcd.print(":");
      if (bedroom2HeatOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bedroom2HeatOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bedroom2HeatOnMM);
      }
      if (b & BUTTON_UP) {
        bedroom2HeatOnMM++;
        if (bedroom2HeatOnMM >= 10 && bedroom2HeatOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bedroom2HeatOnMM);
          // delay(250);
        }
        else if (bedroom2HeatOnMM < 9 && bedroom2HeatOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bedroom2HeatOnMM);
          // delay(250);
        }
        else if (bedroom2HeatOnMM > 59) {
          bedroom2HeatOnMM = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        bedroom2HeatOnMM--;
        if (bedroom2HeatOnMM <= 255 && bedroom2HeatOnMM >= 60) {
          bedroom2HeatOnMM = 59;
        }
        if (bedroom2HeatOnMM >= 10 && bedroom2HeatOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bedroom2HeatOnMM);
          // delay(250);
        }
        else if (bedroom2HeatOnMM < 9 && bedroom2HeatOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bedroom2HeatOnMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = BEDROOM_2_LIGHT_ON_HH;
        lcd.clear();
      }
 /*     if (b & BUTTON_SELECT) {
        EEPROM.update(46, bedroom2HeatOnHH);
        EEPROM.update(47, bedroom2HeatOnMM);
        Serial.print("First/Bedroom 2/Heat/Main/On:"); Serial.print(EEPROM.read(46)); Serial.print("."); Serial.println(EEPROM.read(47));
        lcd.clear();
      }*/
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
      if (bedroom2HeatOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bedroom2HeatOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bedroom2HeatOffHH);
      }
      lcd.print(":");
      if (bedroom2HeatOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bedroom2HeatOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bedroom2HeatOffMM);
      }
      if (b & BUTTON_UP) {
        bedroom2HeatOffHH++;
        if (bedroom2HeatOffHH >= 10 && bedroom2HeatOffHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bedroom2HeatOffHH);
          // delay(250);
        }
        else if (bedroom2HeatOffHH < 9 && bedroom2HeatOffHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bedroom2HeatOffHH);
          // delay(250);
        }
        else if (bedroom2HeatOffHH >= 24) {
          bedroom2HeatOffHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        bedroom2HeatOffHH--;
        if (bedroom2HeatOffHH <= 255 && bedroom2HeatOffHH >= 24) {
          bedroom2HeatOffHH = 23;
        }
        if (bedroom2HeatOffHH >= 10 && bedroom2HeatOffHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bedroom2HeatOffHH);
          // delay(250);
        }
        else if (bedroom2HeatOffHH < 9 && bedroom2HeatOffHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bedroom2HeatOffHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = BEDROOM_2_LIGHT_OFF_MM;
        lcd.clear();

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
      if (bedroom2HeatOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bedroom2HeatOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bedroom2HeatOffHH);
      }
      lcd.print(":");
      if (bedroom2HeatOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bedroom2HeatOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bedroom2HeatOffMM);
      }
      if (b & BUTTON_UP) {
        bedroom2HeatOffMM++;
        if (bedroom2HeatOffMM >= 10 && bedroom2HeatOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bedroom2HeatOffMM);
          // delay(250);
        }
        else if (bedroom2HeatOffMM < 9 && bedroom2HeatOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bedroom2HeatOffMM);
          // delay(250);
        }
        else if (bedroom2HeatOffMM > 59) {
          bedroom2HeatOffMM = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        bedroom2HeatOffMM--;
        if (bedroom2HeatOffMM <= 255 && bedroom2HeatOffMM >= 60) {
          bedroom2HeatOffMM = 59;
        }
        if (bedroom2HeatOffMM >= 10 && bedroom2HeatOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bedroom2HeatOffMM);
          // delay(250);
        }
        else if (bedroom2HeatOffMM < 9 && bedroom2HeatOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bedroom2HeatOffMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = BEDROOM_2_LIGHT_OFF_HH;
        lcd.clear();
      }
  /*    if (b & BUTTON_SELECT) {
        EEPROM.update(48, bedroom2HeatOffHH);
        EEPROM.update(49, bedroom2HeatOffMM);
        Serial.print("First/Bedroom 2/Heat/Main/Off:"); Serial.print(EEPROM.read(48)); Serial.print("."); Serial.println(EEPROM.read(49));
        lcd.clear();
      }*/
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
        lcd.clear();
      }
      else if (b & BUTTON_LEFT) {
        currentState = BATHROOM_LIGHT;
        lcd.clear();
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
        lcd.clear();
      }
      else if (b & BUTTON_RIGHT) {
        currentState = BATHROOM_LIGHT_ON_HH;
        lcd.clear();
      }
      else if (b & BUTTON_LEFT) {
        currentState = BATHROOM_LIGHT_OFF_HH;
        lcd.clear();
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
      lcd.print(bathroomLightLevel); lcd.print("%");
/*      if (b & BUTTON_SELECT) {
        EEPROM.update(50, bathroomLightLevel);
        Serial.print("First/Living Room/Light/Main/Level:"); Serial.println(EEPROM.read(50));
        lcd.clear();
      }*/

      if (b & BUTTON_UP) {
        bathroomLightLevel++;
        if (bathroomLightLevel < 101) {
          lcd.setCursor(6, 1);
          lcd.print("    ");
          lcd.setCursor(6, 1);
          lcd.print(bathroomLightLevel); lcd.print("%");
        }
        else {
          bathroomLightLevel = 100;
        }
      }
      else if (b & BUTTON_DOWN) {
        bathroomLightLevel--;
        if (bathroomLightLevel <= 255 && bathroomLightLevel >= 101) {
          bathroomLightLevel = 0;
        }
        if (bathroomLightLevel > -1) {
          lcd.setCursor(6, 1);
          lcd.print("    ");
          lcd.setCursor(6, 1);
          lcd.print(bathroomLightLevel); lcd.print("%");
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
      if (bathroomLightOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bathroomLightOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bathroomLightOnHH);
      }
      lcd.print(":");
      if (bathroomLightOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bathroomLightOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bathroomLightOnMM);
      }
      if (b & BUTTON_UP) {
        bathroomLightOnHH++;
        if (bathroomLightOnHH >= 10 && bathroomLightOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bathroomLightOnHH);
          // delay(250);
        }
        else if (bathroomLightOnHH < 9 && bathroomLightOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bathroomLightOnHH);
          // delay(250);
        }
        else if (bathroomLightOnHH >= 24) {
          bathroomLightOnHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        bathroomLightOnHH--;
        if (bathroomLightOnHH <= 255 && bathroomLightOnHH >= 24) {
          bathroomLightOnHH = 23;
        }
        if (bathroomLightOnHH >= 10 && bathroomLightOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bathroomLightOnHH);
          // delay(250);
        }
        else if (bathroomLightOnHH < 9 && bathroomLightOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bathroomLightOnHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = BATHROOM_LIGHT_ON_MM;
        lcd.clear();

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
      if (bathroomLightOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bathroomLightOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bathroomLightOnHH);
      }
      lcd.print(":");
      if (bathroomLightOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bathroomLightOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bathroomLightOnMM);
      }
      if (b & BUTTON_UP) {
        bathroomLightOnMM++;
        if (bathroomLightOnMM >= 10 && bathroomLightOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bathroomLightOnMM);
          // delay(250);
        }
        else if (bathroomLightOnMM < 9 && bathroomLightOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bathroomLightOnMM);
          // delay(250);
        }
        else if (bathroomLightOnMM > 60) {
          bathroomLightOnMM = 0;
          bathroomLightOnMM++;
        }
      }
      if (b & BUTTON_DOWN) {
        bathroomLightOnMM--;
        if (bathroomLightOnMM <= 255 && bathroomLightOnMM >= 60) {
          bathroomLightOnMM = 59;
        }
        if (bathroomLightOnMM >= 10 && bathroomLightOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bathroomLightOnMM);
          // delay(250);
        }
        else if (bathroomLightOnMM < 9 && bathroomLightOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bathroomLightOnMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = BATHROOM_LIGHT_ON_HH;
        lcd.clear();
      }
  /*    if (b & BUTTON_SELECT) {
        EEPROM.update(51, bathroomLightOnHH);
        EEPROM.update(52, bathroomLightOnMM);
        Serial.print("First/Bathroom/Light/Main/On:"); Serial.print(EEPROM.read(51)); Serial.print("."); Serial.println(EEPROM.read(52));
        lcd.clear();
      }*/
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
      if (bathroomLightOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bathroomLightOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bathroomLightOffHH);
      }
      lcd.print(":");
      if (bathroomLightOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bathroomLightOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bathroomLightOffMM);
      }
      if (b & BUTTON_UP) {
        bathroomLightOffHH++;
        if (bathroomLightOffHH >= 10 && bathroomLightOffHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bathroomLightOffHH);
          // delay(250);
        }
        else if (bathroomLightOffHH < 9 && bathroomLightOffHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bathroomLightOffHH);
          // delay(250);
        }
        else if (bathroomLightOffHH >= 24) {
          bathroomLightOffHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        bathroomLightOffHH--;
        if (bathroomLightOffHH <= 255 && bathroomLightOffHH >= 24) {
          bathroomLightOffHH = 23;
        }
        if (bathroomLightOffHH >= 10 && bathroomLightOffHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bathroomLightOffHH);
          // delay(250);
        }
        else if (bathroomLightOffHH < 9 && bathroomLightOffHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bathroomLightOffHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = BATHROOM_LIGHT_OFF_MM;
        lcd.clear();

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
      if (bathroomLightOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bathroomLightOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bathroomLightOffHH);
      }
      lcd.print(":");
      if (bathroomLightOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bathroomLightOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bathroomLightOffMM);
      }
      if (b & BUTTON_UP) {
        bathroomLightOffMM++;
        if (bathroomLightOffMM >= 10 && bathroomLightOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bathroomLightOffMM);
          // delay(250);
        }
        else if (bathroomLightOffMM < 9 && bathroomLightOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bathroomLightOffMM);
          // delay(250);
        }
        else if (bathroomLightOffMM > 59) {
          bathroomLightOffMM = 0;
          bathroomLightOffMM++;
        }
      }
      if (b & BUTTON_DOWN) {
        bathroomLightOffMM--;
        if (bathroomLightOffMM <= 255 && bathroomLightOffMM >= 60) {
          bathroomLightOffMM = 59;
        }
        if (bathroomLightOffMM >= 10 && bathroomLightOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bathroomLightOffMM);
          // delay(250);
        }
        else if (bathroomLightOffMM < 9 && bathroomLightOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bathroomLightOffMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = BATHROOM_LIGHT_OFF_HH;
        lcd.clear();
      }
  /*   if (b & BUTTON_SELECT) {
        EEPROM.update(53, bathroomLightOffHH);
        EEPROM.update(54, bathroomLightOffMM);
        Serial.print("First/Bathroom/Light/Main/Off:"); Serial.print(EEPROM.read(53)); Serial.print("."); Serial.println(EEPROM.read(54));
        lcd.clear();
      }*/
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
        lcd.clear();
      }
      else if (b & BUTTON_RIGHT) {
        currentState = BATHROOM_HEAT_ON_HH;
        lcd.clear();
      }
      else if (b & BUTTON_LEFT) {
        currentState = BATHROOM_HEAT_OFF_HH;
        lcd.clear();
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
      lcd.print(bathroomHeatLevel); lcd.print("%");
/*      if (b & BUTTON_SELECT) {
        EEPROM.update(55, bathroomHeatLevel);
        Serial.print("First/Living Room/Light/Main/Level:"); Serial.println(EEPROM.read(55));
        lcd.clear();
      }*/

      if (b & BUTTON_UP) {
        bathroomHeatLevel++;
        if (bathroomHeatLevel < 101) {
          lcd.setCursor(6, 1);
          lcd.print("    ");
          lcd.setCursor(6, 1);
          lcd.print(bathroomHeatLevel); lcd.print("%");
        }
        else {
          bathroomHeatLevel = 100;
        }
      }
      else if (b & BUTTON_DOWN) {
        bathroomHeatLevel--;
        if (bathroomHeatLevel <= 255 && bathroomHeatLevel >= 101) {
          bathroomHeatLevel = 0;
        }
        if (bathroomHeatLevel > -1) {
          lcd.setCursor(6, 1);
          lcd.print("   ");
          lcd.setCursor(6, 1);
          lcd.print(bathroomHeatLevel); lcd.print("%");
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
      if (bathroomHeatOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bathroomHeatOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bathroomHeatOnHH);
      }
      lcd.print(":");
      if (bathroomHeatOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bathroomHeatOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bathroomHeatOnMM);
      }
      if (b & BUTTON_UP) {
        bathroomHeatOnHH++;
        if (bathroomHeatOnHH >= 10 && bathroomHeatOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bathroomHeatOnHH);
          // delay(250);
        }
        else if (bathroomHeatOnHH < 9 && bathroomHeatOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bathroomHeatOnHH);
          // delay(250);
        }
        else if (bathroomHeatOnHH >= 24) {
          bathroomHeatOnHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        bathroomHeatOnHH--;
        if (bathroomHeatOnHH <= 255 && bathroomHeatOnHH >= 24) {
          bathroomHeatOnHH = 23;
        }
        if (bathroomHeatOnHH >= 10 && bathroomHeatOnHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bathroomHeatOnHH);
          // delay(250);
        }
        else if (bathroomHeatOnHH < 9 && bathroomHeatOnHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bathroomHeatOnHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = BATHROOM_LIGHT_ON_MM;
        lcd.clear();

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
      if (bathroomHeatOnHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bathroomHeatOnHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bathroomHeatOnHH);
      }
      lcd.print(":");
      if (bathroomHeatOnMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bathroomHeatOnMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bathroomHeatOnMM);
      }
      if (b & BUTTON_UP) {
        bathroomHeatOnMM++;
        if (bathroomHeatOnMM >= 10 && bathroomHeatOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bathroomHeatOnMM);
          // delay(250);
        }
        else if (bathroomHeatOnMM < 9 && bathroomHeatOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bathroomHeatOnMM);
          // delay(250);
        }
        else if (bathroomHeatOnMM > 59) {
          bathroomHeatOnMM = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        bathroomHeatOnMM--;
        if (bathroomHeatOnMM <= 255 && bathroomHeatOnMM >= 60) {
          bathroomHeatOnMM = 59;
        }
        if (bathroomHeatOnMM >= 10 && bathroomHeatOnMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bathroomHeatOnMM);
          // delay(250);
        }
        else if (bathroomHeatOnMM < 9 && bathroomHeatOnMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bathroomHeatOnMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = BATHROOM_LIGHT_ON_HH;
        lcd.clear();
      }
/*      if (b & BUTTON_SELECT) {
        EEPROM.update(56, bathroomHeatOnHH);
        EEPROM.update(57, bathroomHeatOnMM);
        Serial.print("First/Bathroom/Heat/Main/On:"); Serial.print(EEPROM.read(56)); Serial.print("."); Serial.println(EEPROM.read(57));
        lcd.clear();
      }*/
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
      if (bathroomHeatOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bathroomHeatOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bathroomHeatOffHH);
      }
      lcd.print(":");
      if (bathroomHeatOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bathroomHeatOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bathroomHeatOffMM);
      }
      if (b & BUTTON_UP) {
        bathroomHeatOffHH++;
        if (bathroomHeatOffHH >= 10 && bathroomHeatOffHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bathroomHeatOffHH);
          // delay(250);
        }
        else if (bathroomHeatOffHH < 9 && bathroomHeatOffHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bathroomHeatOffHH);
          // delay(250);
        }
        else if (bathroomHeatOffHH >= 24) {
          bathroomHeatOffHH = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        bathroomHeatOffHH--;
        if (bathroomHeatOffHH <= 255 && bathroomHeatOffHH >= 24) {
          bathroomHeatOffHH = 23;
        }
        if (bathroomHeatOffHH >= 10 && bathroomHeatOffHH < 24) {
          lcd.setCursor(5, 1);
          lcd.print(bathroomHeatOffHH);
          // delay(250);
        }
        else if (bathroomHeatOffHH < 9 && bathroomHeatOffHH < 24) {
          lcd.setCursor(6, 1);
          lcd.print(bathroomHeatOffHH);
          // delay(250);
        }


      }
      if (b & BUTTON_RIGHT) {
        currentState = BATHROOM_LIGHT_OFF_MM;
        lcd.clear();

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
      if (bathroomHeatOffHH < 10) {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.print(bathroomHeatOffHH);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(bathroomHeatOffHH);
      }
      lcd.print(":");
      if (bathroomHeatOffMM < 10) {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.print(bathroomHeatOffMM);
      }
      else {
        lcd.setCursor(8, 1);
        lcd.print(bathroomHeatOffMM);
      }
      if (b & BUTTON_UP) {
        bathroomHeatOffMM++;
        if (bathroomHeatOffMM >= 10 && bathroomHeatOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bathroomHeatOffMM);
          // delay(250);
        }
        else if (bathroomHeatOffMM < 9 && bathroomHeatOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bathroomHeatOffMM);
          // delay(250);
        }
        else if (bathroomHeatOffMM > 59) {
          bathroomHeatOffMM = 0;
        }
      }
      if (b & BUTTON_DOWN) {
        bathroomHeatOffMM--;
        if (bathroomHeatOffMM <= 255 && bathroomHeatOffMM >= 60) {
          bathroomHeatOffMM = 59;
        }
        if (bathroomHeatOffMM >= 10 && bathroomHeatOffMM < 60) {
          lcd.setCursor(8, 1);
          lcd.print(bathroomHeatOffMM);
          // delay(250);
        }
        else if (bathroomHeatOffMM < 9 && bathroomHeatOffMM < 60) {
          lcd.setCursor(9, 1);
          lcd.print(bathroomHeatOffMM);
          // delay(250);
        }
      }
      if (b & BUTTON_LEFT) {
        currentState = BATHROOM_LIGHT_OFF_HH;
        lcd.clear();
      }
      if (b & BUTTON_SELECT) {
        EEPROM.put(58, bathroomHeatOffHH);
        EEPROM.put(59, bathroomHeatOffMM);
        Serial.print("First/Bathroom/Heat/Main/On:"); Serial.print(bathroomHeatOffHH); Serial.print("."); Serial.println(bathroomHeatOffMM);
        lcd.clear();
      }
      break;
  }
}

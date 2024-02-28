#include <esp_now.h>
#include <WiFi.h>
#include <iostream>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd_i2c(0x27, 16, 2);

// Function to display messages on OLED for belle
void belle_show(int state) {
  if (state != 12){
    lcd_i2c.clear();
    lcd_i2c.setCursor(0, 0);
  }

  if (state == 1) {
    lcd_i2c.setCursor(4, 0);
    lcd_i2c.print("Register");
  } else if (state == 2) {
    lcd_i2c.setCursor(1, 0);
    lcd_i2c.print("Create Password");
  } else if (state == 3) {
    lcd_i2c.setCursor(4, 0);
    lcd_i2c.print("Register");
    lcd_i2c.setCursor(4, 1);
    lcd_i2c.print("Complete");
  } else if (state == 4) {
    lcd_i2c.setCursor(2, 0);
    lcd_i2c.print("Forget Card");
  } else if (state == 5) {
    lcd_i2c.setCursor(5, 0);
    lcd_i2c.print("Success");
  } else if (state == 6) {
    lcd_i2c.setCursor(1, 0);
    lcd_i2c.print("Reset Complete");
  } else if (state == 7) {
    lcd_i2c.setCursor(1, 0);
    lcd_i2c.print("Confirm reset");
    lcd_i2c.setCursor(0, 1);
    lcd_i2c.print("# Reset,* Cancel");
  } else if (state == 8) {
    lcd_i2c.setCursor(5, 0);
    lcd_i2c.print("Reset");
  } else if (state == 9) {
    lcd_i2c.setCursor(2, 0);
    lcd_i2c.print("Cancel Reset");
  } else if (state == 10) {
    lcd_i2c.setCursor(0, 0);
    lcd_i2c.print("Clear Password");
  } else if (state == 11) {
    lcd_i2c.setCursor(4, 0);
    lcd_i2c.print("Password");
    lcd_i2c.setCursor(2, 1);
    lcd_i2c.print("Incorrect");
    delay(2000); 
    lcd_i2c.setCursor(0, 0);
    lcd_i2c.print("Please try again");
  } else if (state == 12) {
    lcd_i2c.setCursor(0, 1);
    lcd_i2c.print("Enter Password");
  }
}

// Function to display messages on OLED for ta
void ta_show(int state) {
  lcd_i2c.clear();
  lcd_i2c.setCursor(0, 0);

  if (state == 1) {
    lcd_i2c.print("ta 1");
  } else if (state == 2) {
    lcd_i2c.print("ta 2");
  } else if (state == 3) {
    lcd_i2c.print("ta 3");
  }
}

bool compareMac(const uint8_t * a,const uint8_t * b){
  for(int i=0;i<6;i++){
    if(a[i]!=b[i])
      return false;    
  }
  return true;
}

// REPLACE WITH YOUR ESP RECEIVER'S MAC ADDRESS
uint8_t belleAddress[] = {0x3C, 0x61, 0x05, 0x03, 0xCA, 0x04};
uint8_t taAddress[] = {0xE8, 0xDB, 0x84, 0x01, 0x07, 0x90};

// ta struct
typedef struct oled_send {
  int oledState;
} oled_send;

oled_send taData;
oled_send taBoard;

// belle struct
typedef struct keypad_oled{
  int text; 
}keypad_oled;

keypad_oled belleData;
keypad_oled belleBoard;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  if(compareMac(mac_addr,taAddress)){
    memcpy(&taData, incomingData, sizeof(taData));
    Serial.printf("--------From Ta : สแกน---------\n");
    taBoard.oledState = taData.oledState;
    printf("%d\n", taBoard.oledState);
    Serial.printf("---------------------------------\n");
    Serial.println();
    ta_show(taBoard.oledState); 
  }
  if(compareMac(mac_addr,belleAddress)){
    memcpy(&belleData, incomingData, sizeof(belleData));
    Serial.printf("--------From belle : ปุ่มกด---------\n");
    belleBoard.text = belleData.text;
    printf("%d\n", belleBoard.text);
    Serial.printf("---------------------------------\n");
    Serial.println();
    belle_show(belleBoard.text); 
  }
  
}

void setup()
{
  lcd_i2c.init(); // initialize the lcd
  lcd_i2c.backlight();
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
 
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
   
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
  
}

void loop()
{
//  Serial.printf("-----------Print Oled-----------\n");
//  String recv_taBoard = taBoard.oledState;
//  printf("taBoard: %d\n", recv_taBoard);
//  String recv_belleBoard = belleBoard.text;
//  printf("belleBoard: %d\n", recv_belleBoard);
//  Serial.printf("---------------------------------\n");
//  Serial.println();
//  delay(1000);
//  belle_show(1);
//  delay(2000);                  // display the above for two seconds
//
//  ta_show(2);
//  delay(2000);                      // display the above for two seconds
}

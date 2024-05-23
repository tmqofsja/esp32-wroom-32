  /**********************************************************************************
 *  TITLE: WiFi + Bluetooth + Manual (Switch) control 8 relays using ESP32 & Blynk | Realtime Feedback |
 *  Click on the following links to learn more. 
 *  YouTube Video: https://youtu.be/ifKMi-3uf9Q
 *  Related Blog : https://iotcircuithub.com/esp32-projects/
 *  by Subhajit (Tech StudyCell)
 *  Preferences--> Aditional boards Manager URLs : 
 *  https://dl.espressif.com/dl/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json
 *  
 *  Download Board ESP32: https://github.com/espressif/arduino-esp32
 *  Download the libraries
 *  BluetoothSerial: https://github.com/espressif/arduino-esp32/tree/master/libraries/BluetoothSerial
 *  AceButton Library: https://github.com/bxparks/AceButton
 *  Blynk library  : https://github.com/blynkkk/blynk-library/releases/latest
 *  Install the libraries at Arduino IDE -> Sketch - Include Library -> Add Zip Library
 **********************************************************************************/
#define BLYNK_TEMPLATE_ID "TMPL6iG7QPoNJ"
#define BLYNK_TEMPLATE_NAME "WOOK"
#define BLYNK_AUTH_TOKEN "QVt9r1_jzU115_eZcZgPwB_be8A2uUGT"

#include <Arduino.h>
#include <BlynkSimpleEsp32.h>
#include <BluetoothSerial.h> 
#include <AceButton.h>
#include <RCSwitch.h>
#include <IRremote.hpp>
//#include <PinDefinitionsAndMore.h>
#include "RF433.h"

using namespace ace_button;

void button1Handler(AceButton*, uint8_t , uint8_t );
void button2Handler(AceButton*, uint8_t , uint8_t );
void button3Handler(AceButton*, uint8_t , uint8_t );
void button4Handler(AceButton*, uint8_t , uint8_t );
void button5Handler(AceButton*, uint8_t , uint8_t );
void button6Handler(AceButton*, uint8_t , uint8_t );
void button7Handler(AceButton*, uint8_t , uint8_t );
void button8Handler(AceButton*, uint8_t , uint8_t );
void button9Handler(AceButton*, uint8_t , uint8_t ); 


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run
`make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

BlynkTimer timer;

RCSwitch mySwitch = RCSwitch(); //RF433 

// define the GPIO connected with Relays and switches
#define RelayPin1 23  //D23
#define RelayPin2 22  //D22
#define RelayPin3 21  //D21
#define RelayPin4 19  //D19
#define RelayPin5 18  //D18
#define RelayPin6 5    //D5
#define RelayPin7 25  //D25
#define RelayPin8 26  //D26

#define SwitchPin1 13  //D13
#define SwitchPin2 12  //D12
#define SwitchPin3 14  //D14
#define SwitchPin4 27  //D27
#define SwitchPin5 33  //D33
#define SwitchPin6 32  //D32
#define SwitchPin7 16  //34  //D15
#define SwitchPin8  4  //D4
#define SwitchPin9 17  //36  //16  //D16 All ON OFF  (lolin_d32_pro 1 /lolin_d32 16)
#define Rec_Int    15  //17  // D17 RF433 수신 pin  (lolin_d32_pro 3 /lolin_d32 17)
#define wifiLed     2  //D2
//#define IR_RECEIVE_PIN 15  //D15 IR 수신 pin
#define IR_RECEIVE_PIN 0 //D0 IR 수신 pin
#define EEPROM_SIZE 9

void RF433();

int wifiFlag = 0;
char bt_data; // variable for storing bluetooth data 

int toggleState[ ] = { 1, 1, 1, 1, 1, 1, 1, 1, 1};
int RelayPin[ ] = { RelayPin1, RelayPin2, RelayPin3, RelayPin4, RelayPin5, RelayPin6, RelayPin7, RelayPin8};
int VPIN_BUTTON[ ] = { V0, V1, V2, V3, V4, V5, V6, V7, V8};
int SwitchPin[ ] = { SwitchPin1, SwitchPin2, SwitchPin3, SwitchPin4, SwitchPin5, SwitchPin6, SwitchPin7,
                    SwitchPin8, SwitchPin9};
int i = 0;

ButtonConfig config1;
AceButton button1(&config1);
ButtonConfig config2;
AceButton button2(&config2);
ButtonConfig config3;
AceButton button3(&config3);
ButtonConfig config4;
AceButton button4(&config4);
ButtonConfig config5;
AceButton button5(&config5);
ButtonConfig config6;
AceButton button6(&config6);
ButtonConfig config7;
AceButton button7(&config7);
ButtonConfig config8;
AceButton button8(&config8);
ButtonConfig config9;
AceButton button9(&config9);

void handleEvent1(AceButton*, uint8_t, uint8_t);
void handleEvent2(AceButton*, uint8_t, uint8_t);
void handleEvent3(AceButton*, uint8_t, uint8_t);
void handleEvent4(AceButton*, uint8_t, uint8_t);
void handleEvent5(AceButton*, uint8_t, uint8_t);
void handleEvent6(AceButton*, uint8_t, uint8_t);
void handleEvent7(AceButton*, uint8_t, uint8_t);
void handleEvent8(AceButton*, uint8_t, uint8_t);
void handleEvent9(AceButton*, uint8_t, uint8_t);

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "KT_GiGA_2G_C772";
char pass[] = "12345678asd";

void relayOnOff(int relay){

    switch(relay){
      case 0: BK_Box(relay,toggleState, RelayPin,VPIN_BUTTON); break;
      case 1: BK_Box(relay,toggleState, RelayPin,VPIN_BUTTON); break;
      case 2: BK_Box(relay,toggleState, RelayPin,VPIN_BUTTON); break;
      case 3: BK_Box(relay,toggleState, RelayPin,VPIN_BUTTON); break;
      case 4: BK_Box(relay,toggleState, RelayPin,VPIN_BUTTON); break;
      case 5: BK_Box(relay,toggleState, RelayPin,VPIN_BUTTON); break;
      case 6: BK_Box(relay,toggleState, RelayPin,VPIN_BUTTON); break;
      case 7: BK_Box(relay,toggleState, RelayPin,VPIN_BUTTON); break;
      case 8: toggleState[relay] = !toggleState[relay];
                if (!toggleState[relay]){
                  for( i=0;i <8;i++){
                    digitalWrite(RelayPin[i], LOW);
                    EEPROM_Write( i,0);
                    Blynk.virtualWrite(VPIN_BUTTON[i], 0);
                  }
                } else {
                  for( i=0;i <8;i++){
                    digitalWrite(RelayPin[i], HIGH);
                    EEPROM_Write( i,1);
                    Blynk.virtualWrite(VPIN_BUTTON[i], 1);
                  }
                }
                  EEPROM_Write( relay,toggleState[relay]);                
                  Blynk.virtualWrite(VPIN_BUTTON[relay], toggleState[relay]);
                  if (!toggleState[relay])
                    Serial.println("Device9 ON");
                  else
                    Serial.println("Device9 OFF");
                    delay(100);
      break;      
      default : break;      
    }       
}

BLYNK_WRITE(V0) {
  toggleState[V0] = !param.asInt();
  relayOnOff(V0);
}

BLYNK_WRITE(V1) {
  toggleState[V1] = !param.asInt();
  relayOnOff(V1);
}

BLYNK_WRITE(V2) {
  toggleState[V2] = !param.asInt();
  relayOnOff(V2);
}

BLYNK_WRITE(V3) {
  toggleState[V3] = !param.asInt();
  relayOnOff(V3);
}

BLYNK_WRITE(V4) {
  toggleState[V4] = !param.asInt();
  relayOnOff(V4);
}

BLYNK_WRITE(V5) {
  toggleState[V5] = !param.asInt();
  relayOnOff(V5);
}

BLYNK_WRITE(V6) {
  toggleState[V6] = !param.asInt();
  relayOnOff(V6);
}

BLYNK_WRITE(V7) {
  toggleState[V7] = !param.asInt();
  relayOnOff(V7);
}

BLYNK_WRITE(V8) {
  toggleState[V8] = !param.asInt();
  relayOnOff(V8);
}

void all_Switch_ON(){
  for( i=0;i <8;i++){
    digitalWrite(RelayPin[i], LOW);
    toggleState[i] = 0;
    Blynk.virtualWrite(VPIN_BUTTON[i], toggleState[i]);
  }
}

void all_Switch_OFF(){
  for( i=0;i <8;i++){
    digitalWrite(RelayPin[i], HIGH);
    toggleState[i] = 1;
    Blynk.virtualWrite(VPIN_BUTTON[i], toggleState[i]);
  } 
}

void ir_remote(){
  if (IrReceiver.decode()) {
    Serial.print("IR = ");  
    Serial.println(IrReceiver.decodedIRData.decodedRawData,HEX);
      switch(IrReceiver.decodedIRData.decodedRawData){
          case 0xF30CFF00:  relayOnOff(0); break;
          case 0xE718FF00:  relayOnOff(1); break;
          case 0xA15EFF00:  relayOnOff(2); break;
          case 0xF708FF00:  relayOnOff(3); break;
          case 0xE31CFF00:  relayOnOff(4); break;
          case 0xA55AFF00:  relayOnOff(5); break;
          case 0xBD42FF00:  relayOnOff(6); break;
          case 0xAD52FF00:  relayOnOff(7); break;
          case 0xB54AFF00:  relayOnOff(8); break;
          default : break;         
        } 
        IrReceiver.resume(); // Enable receiving of the next value    
  } 
}

void Bluetooth_handle()
{
  bt_data = SerialBT.read();
  Serial.print("Bluetooth = "); 
  Serial.println(bt_data);
  delay(20);

  switch(bt_data) {
    case 'a': relayOnOff(0); break;
    case 'b': relayOnOff(1); break;
    case 'c': relayOnOff(2); break;
    case 'd': relayOnOff(3); break;
    case 'e': relayOnOff(4); break;
    case 'f': relayOnOff(5); break;
    case 'g': relayOnOff(6); break;
    case 'h': relayOnOff(7); break;
    case 'z': relayOnOff(8); break;
    default : break;
  }
}

void checkBlynkStatus() { // called every 3 seconds by SimpleTimer

  bool isconnected = Blynk.connected();
    if (isconnected == false) {
      wifiFlag = 1;
      digitalWrite(wifiLed, LOW); //Turn off WiFi LED
    }
    if (isconnected == true) {
      wifiFlag = 0;
      digitalWrite(wifiLed, HIGH); //Turn on WiFi LED
    }
}

void setup()
{
  Serial.begin(115200);

  mySwitch.enableReceive(Rec_Int);  // RF433  Receiver on interrupt 0 => that is pin #2

// initialize EEPROM with predefined size
  EEPROM.begin(EEPROM_SIZE);
  
  btStart();  //Serial.println("Bluetooth On");
  SerialBT.begin("WOOK"); //Bluetooth device name 
  Serial.println("The device started, now you can pair it with bluetooth!");
  delay(5000);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver

  //IrReceiver.begin(IR_RECEIVE_PIN1, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN); // IR Start the receiver

  for(i = 0 ; i < 8 ; i++){
    pinMode(RelayPin[i], OUTPUT);
  }
  for(i = 0 ; i < 9 ; i++){
    pinMode(SwitchPin[i], INPUT_PULLUP);
  }

  pinMode(wifiLed, OUTPUT);

  //During Starting all Relays should TURN OFF

  for(i = 0 ; i < 8 ; i++){
      toggleState[i] = EEPROM.read(i);
      digitalWrite(RelayPin[i], toggleState[i]);
      Serial.print("ToggleState " + String(i+1) + " = " + String(toggleState[i]));
      Serial.println();
  }  
  toggleState[i] = EEPROM.read(i);
      Serial.print("ToggleState " + String(i+1) + " = " + String(toggleState[i]));
  
  config1.setEventHandler(button1Handler);
  config2.setEventHandler(button2Handler);
  config3.setEventHandler(button3Handler);
  config4.setEventHandler(button4Handler);
  config5.setEventHandler(button5Handler);
  config6.setEventHandler(button6Handler);
  config7.setEventHandler(button7Handler);
  config8.setEventHandler(button8Handler);
  config9.setEventHandler(button9Handler);

  button1.init(SwitchPin[0]);
  button2.init(SwitchPin[1]);
  button3.init(SwitchPin[2]);
  button4.init(SwitchPin[3]);
  button5.init(SwitchPin[4]);
  button6.init(SwitchPin[5]);
  button7.init(SwitchPin[6]);
  button8.init(SwitchPin[7]);
  button9.init(SwitchPin[8]);  

  WiFi.begin(auth, pass);
  timer.setInterval(3000L, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
  Blynk.begin(auth, ssid, pass);
  delay(2000);
}

void loop()
{  

  ir_remote();
  
  if (WiFi.status())
    {
     Blynk.run();
    }

  timer.run(); // Initiates SimpleTimer

  if (SerialBT.available()){
    Bluetooth_handle();
  }

  if (mySwitch.available()) { // Message availiable.메시지 는 변조.‎  > 0
    RF433();
  }
    button1.check();
    button2.check();
    button3.check();
    button4.check();
    button5.check();
    button6.check();
    button7.check();
    button8.check();
    button9.check();
    }

void button1Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT1");
  switch (eventType) {
    case AceButton::kEventPressed:
         Serial.println("kEventPressed");
         relayOnOff(0); //digitalWrite(RelayPin1, LOW);
         break;
    case AceButton::kEventReleased:
         Serial.println("kEventReleased");
         relayOnOff(0); //digitalWrite(RelayPin1, HIGH);
    break;
  }
}

void button2Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT2");
  switch (eventType) {
    case AceButton::kEventPressed:
         Serial.println("kEventPressed");
         relayOnOff(1);//digitalWrite(RelayPin2, LOW);
    break;
    case AceButton::kEventReleased:
         Serial.println("kEventReleased");
         relayOnOff(1);digitalWrite(RelayPin2, HIGH);
    break;
  }
}

void button3Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT3");
  switch (eventType) {
    case AceButton::kEventPressed:
         Serial.println("kEventPressed");
         relayOnOff(2);//digitalWrite(RelayPin3, LOW);
    break;
    case AceButton::kEventReleased:
         Serial.println("kEventReleased");
         relayOnOff(2);//digitalWrite(RelayPin3, HIGH);
    break;
  }
}

void button4Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT4");
  switch (eventType) {
    case AceButton::kEventPressed:
         Serial.println("kEventPressed");
         relayOnOff(3);//digitalWrite(RelayPin4, LOW);
    break;
    case AceButton::kEventReleased:
         Serial.println("kEventReleased");
         relayOnOff(3);//digitalWrite(RelayPin4, HIGH);
    break;
  }
}

void button5Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT5");
  switch (eventType) {
    case AceButton::kEventPressed:
         Serial.println("kEventPressed");
         relayOnOff(4);//digitalWrite(RelayPin5, LOW);
    break;
    case AceButton::kEventReleased:
         Serial.println("kEventReleased");
         relayOnOff(4);//digitalWrite(RelayPin5, HIGH);
    break;
  }
}

void button6Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT6");
  switch (eventType) {
    case AceButton::kEventPressed:
         Serial.println("kEventPressed");
         relayOnOff(5);
    break;
    case AceButton::kEventReleased:
         Serial.println("kEventReleased");
         relayOnOff(5);
    break;
  }
}

void button7Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT7");
  switch (eventType) {
    case AceButton::kEventPressed:
         Serial.println("kEventPressed");
         relayOnOff(6);
    break;
    case AceButton::kEventReleased:
         Serial.println("kEventReleased");
         relayOnOff(6);
    break;
  }
}

void button8Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT8");
  switch (eventType) {
    case AceButton::kEventPressed:
         Serial.println("kEventPressed");
         relayOnOff(7);//digitalWrite(RelayPin8, LOW);
    break;
    case AceButton::kEventReleased:
         Serial.println("kEventReleased");
         relayOnOff(7);//digitalWrite(RelayPin8, HIGH);
    break;
  }
}

void button9Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT9");
  switch (eventType) {
    case AceButton::kEventPressed:
         Serial.println("kEventPressed");
         relayOnOff(8);
    break;
    case AceButton::kEventReleased:
         Serial.println("kEventReleased");
         relayOnOff(8);
    break;
  }
}

void RF433() {
  unsigned long int RF_data =  mySwitch.getReceivedValue();
  Serial.print("RF433 = ");
  Serial.println(RF_data);

  digitalWrite(wifiLed,HIGH);
  delay(50);
  digitalWrite(wifiLed,LOW);
  delay(50);
  digitalWrite(wifiLed,HIGH);

switch(RF_data){

  case 15886440: mySwitch.disableReceive(); //데이터 수신 을 비활성화이 포트를 비활성화 할 수있는 인터럽트를 비활성화합니다.‎
                 relayOnOff(0);
                 break;
  case 15886436: mySwitch.disableReceive();
                 relayOnOff(1);
                 break;
  case 15886434: mySwitch.disableReceive();
                 relayOnOff(2);
                 break; 
  case 15886433: mySwitch.disableReceive();
                 relayOnOff(3);
                 break;
  case 4157736: mySwitch.disableReceive();
                 relayOnOff(4);
                 break;                
  case 4157730: mySwitch.disableReceive();
                 relayOnOff(5);
                 break;                
  case 8662824: mySwitch.disableReceive();
                 relayOnOff(6);
                 break;                
  case 8662818: mySwitch.disableReceive();
                 relayOnOff(7);
                 break;                                                                                                    
  }

unsigned long time_now = millis();
  int ck = 500;

  while(millis()<time_now + ck){;}
    mySwitch.resetAvailable();
    mySwitch.enableReceive(Rec_Int);

}

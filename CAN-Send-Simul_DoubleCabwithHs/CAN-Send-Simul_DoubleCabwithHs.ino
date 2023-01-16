#include <mcp_can.h>
#include <SPI.h>

const int butState1 = PB12;   //Neutral
const int butState2 = PB8;   //Reverse
const int butState3 = PB3;    //Drive
const int butState4 = PB6;    //Parking

const int butState5 = PB14;   //2H
const int butState6 = PA15;   //4H
const int butState7 = PB5;    //4L

const int butState8 = PB13;   //Hi Lamp
//const int butState9 = PA10; //Lo Lamp
const int butState10 = PB4;   //Hand Brake
const int butState11 = PB9;   //------

const int butState12 = PB15;  //L Blink
const int butState13 = PA8;   //R Blink

//------------define Potentiometer----------//
#define poten PA0

//-----------But State-------------//
int butNeu = 0;
int butRev = 0;
int butPark = 0;
int butDrive = 0;

int but2H = 0;
int but4H = 0;
int but4L = 0;

int butLblink = 0;
int butRblink = 0;

int butHi = 0;
int butLo = 0;
int butHandBrk = 0;

int pot;

//Value pin

int Speed;
int RPM;
int Battery;
int MCU_Temp;
int Bat_Temp;
int Mtr_Temp;
int Vcc;

/* ------------- Bits in Byte ------------ */

byte gear = 0x00000000;
byte hs = 0x00000000;
byte beam = 0x00000000;
byte door = 0x00000000;

/* ------------- Check Sending Data Status --------- */

byte sndStat1;
byte sndStat2;
byte sndStat3;
byte sndStat4;

//#define CAN0_INT PA2
MCP_CAN CAN(PA4);     // Set CS to pin 10

void setup()
{
  Serial.begin(115200);
  //while (!Serial) {};
  pinMode(butState1, INPUT_PULLUP);
  //pinMode(butState2, INPUT_PULLUP);
  pinMode(butState3, INPUT_PULLUP);
  pinMode(butState4, INPUT_PULLUP);
  pinMode(butState5, INPUT_PULLUP);
  pinMode(butState6, INPUT_PULLUP);
  pinMode(butState7, INPUT_PULLUP);
  pinMode(butState8, INPUT_PULLUP);
  //pinMode(butState9, INPUT_PULLUP);
  pinMode(butState10, INPUT_PULLUP);
  pinMode(butState11, INPUT_PULLUP);
  pinMode(butState12, INPUT_PULLUP);
  pinMode(butState13, INPUT_PULLUP);

  pinMode(poten, INPUT);

  //INPUT PinMode

  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if (CAN.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515...");

  CAN.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted

  //pinMode(CAN0_INT, INPUT);

  Serial.println("MCP2515 Library Receive Example...");
}


byte torque[8] = {0x00, 0x7D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  //0x0CF00400

byte speed[8] = {0x00, 0x00, 0x36, 0x7F, 0x01, 0x00, 0x00, 0x00};   //0x0CF00500

byte brake[8] = {0x00, 0x00, 0x36, 0x04, 0x03, 0x02, 0x01, 0x12};   //0x0C50A49C

byte bem[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};     //0x0D000020

byte accur[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17};   //0x0D000022

void loop()
{
  //Reading INPUT pins
  butLblink = ! digitalRead(butState12);
  butRblink = ! digitalRead(butState13);

  butNeu = ! digitalRead(butState1);
  butDrive = ! digitalRead(butState3);
  //butRev =! digitalRead(butState2);
  butPark = ! digitalRead(butState4);

  but2H = ! digitalRead(butState5);
  but4H = ! digitalRead(butState6);
  but4L = ! digitalRead(butState7);

  butHi = ! digitalRead(butState8);
  butLo = ! digitalRead(butState10);
  butHandBrk = ! digitalRead(butState11);

  /* but2H =! digitalRead();
    but4H =! digitalRead();
    but4L =! digitalRead(); */

  pot = analogRead(poten);

  //---------- Mapping Data ----------//
  Speed = map(pot, 0, 1023, 0, 100);
  Battery = map(pot, 0, 1023, 0, 100);
  RPM = map(pot, 0, 1023, 0, 5000);
  MCU_Temp = map(pot, 0, 1023, -40, 200);
  Mtr_Temp = map(pot, 0, 1023, -40, 200);

  Vcc = map(pot, 0, 1023, 0, 24);
  Bat_Temp = map(pot, 0, 1023, -20, 100);

  /* ------------- Divide Data to Byte ---------------*/

  byte RPMH = highByte(RPM);
  byte RPML = lowByte(RPM);

  byte VccH = highByte(Vcc);
  byte VccL = lowByte(Vcc);

  /* ------------- Store INPUT in Bits ----------------- */
  //Gear Shift and Brakes
   bitWrite(gear, 7, 0);  //Safety Belt
   bitWrite(gear, 6, butHandBrk);  //FootBrake
   bitWrite(gear, 5, butPark);  //HandBrake
   bitWrite(gear, 4, 0);  //Sport
   //bitWrite(gear, 3, butRev);  //Reverse
   bitWrite(gear, 2, butDrive);  //Drive
   bitWrite(gear, 1, butNeu);  //Neutral
   bitWrite(gear, 0, 0);  //Standby

   //Blinkers and H's
   bitWrite(hs, 7, butLblink);    //R blink
   bitWrite(hs, 6, butRblink);    //Left Blink
   bitWrite(hs, 5, but4L);    //4L
   bitWrite(hs, 4, but4H);    //4H
   bitWrite(hs, 3, but2H);    //2H

   //Beams
   bitWrite(beam, 7, butHi);  //Hi Beam
   bitWrite(beam, 5, butLo);  //Lo Beam

   //Door
   bitWrite(door, 7, 0);  //Door

   //0x0CF00500
   speed[0] = gear;
   speed[1] = hs;
   speed[2] = Speed;
   speed[3] = Mtr_Temp;
   speed[5] = Battery;
   speed[4] = MCU_Temp;

   //0x0CF00400
   torque[3] = RPMH;
   torque[4] = RPML;

   //0x0D000022
   accur[0] = VccH;
   accur[1] = VccL;

   //0x0D000020
   bem[0] = beam;
   bem[1] = door;


   // send data:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
   //sendMsgBuff(id, ext, dlc, data)
   sndStat1 = CAN.sendMsgBuf(0x0CF00400, 1, 8, torque);
   sndStat2 = CAN.sendMsgBuf(0x0CF00500, 1, 8, speed);
   sndStat3 = CAN.sendMsgBuf(0x0D000020, 1, 8, bem);
   sndStat4 = CAN.sendMsgBuf(0x0D000022, 1, 8, accur);

   msgCheck();

   //delay(100);   // send data per 100ms */

   Serial.print(but2H);
   Serial.print("\t");
   Serial.print(but4H);
   Serial.print("\t");
   Serial.print(but4L);

   Serial.println(); 
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

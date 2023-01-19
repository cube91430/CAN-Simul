#include <SPI.h>      // Library for using SPI Communication
#include <mcp2515.h>  // Library for using CAN Communication

#include <TaskScheduler.h>

struct can_frame canMsg1;
struct can_frame canMsg2;
struct can_frame canMsg3;
struct can_frame canMsg4;
MCP2515 mcp2515(53);

//input
int input31;  //L Blink
int input30;  //R Blink

/////
int input29;  //Parking
int input28;  //Neutral
int input27;  //Reverse
int input26;  //Drive
int input25;  //Hand Brake
int input24;  //FootBrake
int input23;  //2H
int input22;  //4H
int input21;  //4L
////////////
int input20;  //Safety Belt
int input19;  //High Beam
int input18;  //Low Beam
int input15;  //Front Left Door
int input14;  //Front Right Door
int input13;  //Back Left Door
int input12;  //Back Right Door
int input11;  //
int input9;   //
//////////
int analog1;  //Battery
int analog0;  //Throttle
int analog4;  //Wiper
int analog8;  //
int analog5;  //
int analog9;  //
int analog6;  //
int analog2;  //
int analog3;  //
int analog7;  //
//////

int PRND;
float throttle;
float kecepatan;
float distance;
float suhuMotor;
float persentaseBaterai;
float SoC;
float kapasitasBaterai;

//byte BINARY
byte command_1 = 0x0000000;
byte command_2 = 0x0000000;
byte beams = 0x0000000;
byte hinges = 0x0000000;


//float odometer;
int fault;
float baterai;
bool highBeam, lowBeam, handBrake, footBrake, leftSign, rightSign, door, doorFL, doorFR, doorRL, doorRR, seatBelt;
int fourWD;  //2H,4H,4L

//int outputled = ;
// Callback methods prototypes
void t1Callback();
void t2Callback();
void t3Callback();
void inputCallback();
void commCallback();
void comProcessCallback();

uint8_t t1_counter = 0;
uint8_t t2_counter = 0;
uint8_t t3_counter = 0;
uint8_t t4_counter = 0;
char receivedChar;
//Tasks

Scheduler runner;
// Task(unsigned long aInterval=0, long aIterations=0, TaskCallback aCallback=NULL, Scheduler* aScheduler=NULL, bool aEnable=false, TaskOnEnable aOnEnable=NULL, TaskOnDisable aOnDisable=NULL
Task t1(50, -1, &t1Callback, &runner, true);        //0CFF09D0 // VCU -> TCU
Task t2(10, -1, &t2Callback, &runner, true);        //0CFF10D0 // VCU -> TCU
Task input(50, -1, &inputCallback, &runner, true);  //read pin input
Task com(200, -1, &commCallback, &runner, true);
Task comProcess(0, TASK_ONCE, NULL, &runner, false, &comProcessCallback);

void t1Callback() {
  canMsg1.data[7] = t1_counter;
  t1_counter++;
  mcp2515.sendMessage(&canMsg1);
}

void t2Callback() {
  canMsg2.data[7] = t2_counter;
  t2_counter++;
  mcp2515.sendMessage(&canMsg2);
}

void t3Callback() {
  t3_counter++;
  canMsg3.data[6] = t3_counter;
  mcp2515.sendMessage(&canMsg3);
}

void t4Callback() {
  t4_counter++;
  canMsg4.data[6] = t4_counter;
  mcp2515.sendMessage(&canMsg4);
}

void inputCallback() {
  //if input low ==  input ON
  input31 = digitalRead(31);  //L Blink
  input30 = digitalRead(30);  //R Blink
  input29 = digitalRead(29);  //Parking
  input28 = digitalRead(28);  //Neutral
  input27 = digitalRead(27);  //Reverse
  input26 = digitalRead(26);  //Drive
  input25 = digitalRead(25);  //Hand Brake
  input24 = digitalRead(24);  //Foot Brake
  input23 = digitalRead(23);  //2H
  input22 = digitalRead(22);  //4H
  input21 = digitalRead(21);  //4L
  input20 = digitalRead(20);  //Safety Belt
  input19 = digitalRead(19);  //High Beam
  input18 = digitalRead(18);  //Low Beam
  input15 = digitalRead(15);  //Front Left Door
  input14 = digitalRead(14);  //Front Right Door
  input13 = digitalRead(13);  //Back Left Door
  input12 = digitalRead(12);  //Back Right Door
  input11 = digitalRead(11);
  input9 = digitalRead(9);
  analog1 = analogRead(A1);  //Battery
  analog0 = analogRead(A0);  //Speed
  analog4 = analogRead(A4);  //wiper
  analog8 = analogRead(A8);  //RPM
  analog5 = analogRead(A5);
  analog9 = analogRead(A9);
  analog6 = analogRead(A6);
  analog2 = analogRead(A2);
  analog3 = analogRead(A3);
  analog7 = analogRead(A7);
  baterai = (41 * 10.0 / 25) * (analog1 / 1024.0);

  //1st System Status

  bitWrite(command_1, 7, input20);  //Safety Belt
  bitWrite(command_1, 6, input24);  //Foot Brake
  bitWrite(command_1, 5, input25);  //Hand Brake
  //bitWrite(command_1, 4, input28);  //Sport
  bitWrite(command_1, 3, input27);  //Reverse
  bitWrite(command_1, 2, input26);  //Drive
  bitWrite(command_1, 1, input28);  //Neutral
  //bitWrite(command_1, 0, input24);  //Standby

  //2nd System Status

  bitWrite(command_2, 7, input30);  //Right Blinkers
  bitWrite(command_2, 6, input31);  //Left Blinkers
  bitWrite(command_2, 5, input21);  //4L
  bitWrite(command_2, 4, input22);  //4H
  bitWrite(command_2, 3, input23);  //2H

  //Hi Beam and Lo Beam

  bitWrite(beams, 7, input19);  //High Beam
  bitWrite(beams, 5, input18);  //Low Beam

  //Door
  bitWrite(hinges, 5, input12);  //Back Right Door
  bitWrite(hinges, 4, input13);  //Back Left Door
  bitWrite(hinges, 3, input14);  //Front Right Door
  bitWrite(hinges, 2, input15);  //Front Left Door

  Serial.println(baterai);
}


void commCallback() {
  if (Serial.available() > 0) {
    receivedChar = Serial.read();
    comProcess.enableDelayed();
  }
}

void comProcessCallback() {
  //System Status - CAN Data
  canMsg1.data[0] = command_1;
  canMsg1.data[1] = command_2;
  canMsg1.data[2] = analog0;
  canMsg1.data[3] = 0x94;  //Motor Temperature
  canMsg1.data[4] = 0x94;  //Inverter Temperature
  canMsg1.data[5] = analog1;
  canMsg1.data[6] = 0x94;  //Battery Temperature

  //RPM - CAN Data
  canMsg2.data[3] = 0x13;  //Rpm HIGH
  canMsg2.data[4] = 0x7D;  //Rpm LOW

  //Lights and Door - CAN Data
  canMsg3.data[0] = beams;
  canMsg3.data[1] = hinges;

  //Vcc - CAN Data
  canMsg4.data[0] = 0x01;
  canMsg4.data[1] = 0x02;

  /*
  switch(receivedChar){
    case '1': //vehicle ready
    canMsg1.data[1] = 0x04;
    break;
    case '2': //vehicle not ready
    canMsg1.data[1] = 0x00;
    break;
    case '3': //Brake Pressed
    canMsg1.data[5] = 0x01;
    break;
    case '4': //Brake not Pressed
    canMsg1.data[5] = 0x00;
    break;
    case '5': //alow gear shift
    canMsg2.data[3] = canMsg2.data[3] | 0b00000001;
    break;
    case '6': //not alow gear shift
    canMsg2.data[3] = canMsg2.data[3] & 0b11111100;
    break;
    case '7': //D
    canMsg2.data[3] = canMsg2.data[3] & 0x0F;
    canMsg2.data[3] = canMsg2.data[3] | 0xE0;
    break;
    case '8': //R
    canMsg2.data[3] = canMsg2.data[3] & 0x0F;
    canMsg2.data[3] = canMsg2.data[3] | 0xD0;
    break;
    case '9': //P
    canMsg2.data[3] = canMsg2.data[3] & 0x0F;
    canMsg2.data[3] = canMsg2.data[3] | 0xF0;
    break;
    case 'a': //N
    canMsg2.data[3] = canMsg2.data[3] & 0x0F;
    break;
    case 'b': //motor enable, torque mode
    canMsg3.data[0] = 0x21;
    break;
    case 'c': //motor disable
    canMsg3.data[0] = 0x00;
    canMsg3.data[4] = 0x88;
    canMsg3.data[5] = 0x13;
    break;
    case 'd': //motor accell
    canMsg3.data[4] = 0x94;
    canMsg3.data[5] = 0x13;
    break;
    case 'e': //motor deaccell
    canMsg3.data[4] = 0x88;
    canMsg3.data[5] = 0x13;
    break;
  }
  */
}

void setup() {
  Serial.begin(115200);
  SPI.begin();  // Begins SPI communication
  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ);  // Sets CAN at speed 500KBPS and Clock 8MHz
  mcp2515.setNormalMode();
  pinMode(31, INPUT_PULLUP);
  pinMode(30, INPUT_PULLUP);
  pinMode(29, INPUT_PULLUP);
  pinMode(28, INPUT_PULLUP);
  pinMode(27, INPUT_PULLUP);
  pinMode(26, INPUT_PULLUP);
  pinMode(25, INPUT_PULLUP);
  pinMode(24, INPUT_PULLUP);
  pinMode(23, INPUT_PULLUP);
  pinMode(22, INPUT_PULLUP);
  pinMode(21, INPUT_PULLUP);
  pinMode(20, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
  pinMode(18, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);

  canMsg1.can_id = 0x0CF00500 | CAN_EFF_FLAG;  // CAN id  - Controls
  canMsg1.can_dlc = 8;                         // CAN data length as 8
  canMsg1.data[0] = 0x00;
  canMsg1.data[1] = 0x00;
  canMsg1.data[2] = 0x00;
  canMsg1.data[3] = 0x00;
  canMsg1.data[4] = 0x00;
  canMsg1.data[5] = 0x00;
  canMsg1.data[6] = 0x00;
  canMsg1.data[7] = 0x00;

  canMsg2.can_id = 0x0CF00400 | CAN_EFF_FLAG;  // CAN id - System Status
  canMsg2.can_dlc = 8;
  canMsg2.data[0] = 0x00;
  canMsg2.data[1] = 0x00;
  canMsg2.data[2] = 0x00;
  canMsg2.data[3] = 0x00;
  canMsg2.data[4] = 0x00;
  canMsg2.data[5] = 0x00;
  canMsg2.data[6] = 0x00;
  canMsg2.data[7] = 0x00;

  canMsg3.can_id = 0x0D000020 | CAN_EFF_FLAG;  // CAN id - System Status
  canMsg2.can_dlc = 8;
  canMsg2.data[0] = 0x00;
  canMsg2.data[1] = 0x00;
  canMsg2.data[2] = 0x00;
  canMsg2.data[3] = 0x00;
  canMsg2.data[4] = 0x00;
  canMsg2.data[5] = 0x00;
  canMsg2.data[6] = 0x00;
  canMsg2.data[7] = 0x00;

  canMsg4.can_id = 0x0D000022 | CAN_EFF_FLAG;  // CAN id - System Status
  canMsg2.can_dlc = 8;
  canMsg2.data[0] = 0x00;
  canMsg2.data[1] = 0x00;
  canMsg2.data[2] = 0x00;
  canMsg2.data[3] = 0x00;
  canMsg2.data[4] = 0x00;
  canMsg2.data[5] = 0x00;
  canMsg2.data[6] = 0x00;
  canMsg2.data[7] = 0x00;

  /*
  canMsg3.can_id = 0x0CFF0AD0| CAN_EFF_FLAG; // CAN id 
  canMsg3.can_dlc = 8;
  canMsg3.data[0] = 0x00; 
  canMsg3.data[1] = 0x00; 
  canMsg3.data[2] = 0x00; 
  canMsg3.data[3] = 0x00; 
  canMsg3.data[4] = 0x88;
  canMsg3.data[5] = 0x13;
  canMsg3.data[6] = 0x00;
  canMsg3.data[7] = 0x00;
*/
}


//START
//1. vehicle ready
//2. brake pressed
//3. motor enable
//4. gear D
//5. motor accel
//STOP
//1. motor deaccell
//2. motor disable
//3. gear N

void loop() {
  runner.execute();
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/


/*
#include <SPI.h> //Library for using SPI Communication
#include <mcp2515.h> //Library for using CAN Communication

struct can_frame canMsg;
MCP2515 mcp2515(10);
const uint8_t PotentiometerPin = A0;
int throttle;
int throttleData;
int maju;
//id controller 1 = 100001 = 0x000186A1 
//id controller 2 = 100002 = 0x000186A2
//id controller 3 = 100003 = 0x000186A3

//MPC2515 – VCC -> +5V
//MPC2515 – GND -> GND
//MPC2515 – CS -> D10 (SPI_SS)
//MPC2515 – SO -> D12 (SPI_MISO)
//MPC2515 - SI -> D11 (SPI_MOSI)
//MPC2515 – SCK -> D13 (SPI_SCK)
//MPC2515 – INT -> D2

//potensiometer     -> A0  
//input maju mundur -> D3  
//led indikator     -> D9  

void setup(){
  while (!Serial);
  Serial.begin(115200);
  SPI.begin(); //Begins SPI communication
  pinMode(3, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz
  mcp2515.setNormalMode();
}

void loop(){
  int gas = analogRead(PotentiometerPin);
  throttle = map(gas, 0, 1023, 0, 100);     
  
 
  
  maju = 1; //Gets fwd value
  int inputMaju = digitalRead(3);
  if (inputMaju == HIGH) {
    maju = 1;  
    digitalWrite(9, LOW);
  } else {
    maju = -1;  
    digitalWrite(9, HIGH);
  }
  
  throttleData = throttle*maju;
  
  canMsg.can_id = 0x000186A1; //CAN id as 100.001
  canMsg.can_dlc = 8; //CAN data length as 8
  canMsg.data[0] = 0x00; //Update humidity value in [0]
  canMsg.data[1] = throttleData ; //range 0-100 dan 150-250
  canMsg.data[2] = 0x00; //Rest all with 0
  canMsg.data[3] = 0x00;
  canMsg.data[4] = 0x00;
  canMsg.data[5] = 0x00;
  canMsg.data[6] = 0x00;
  canMsg.data[7] = 0x00;
  mcp2515.sendMessage(&canMsg); //Sends the CAN message
  delay(50);
  Serial.print(throttle);
  Serial.print("      ");
  Serial.println(throttleData);
}

*/

#include <SPI.h> // Library for using SPI Communication
#include <mcp2515.h> // Library for using CAN Communication

#include <TaskScheduler.h>

struct can_frame canMsg1;
struct can_frame canMsg2;
struct can_frame canMsg3;
MCP2515 mcp2515(53);

//input
int input31;
int input30;

//Bin
byte prnd = 0x00000000;
byte Hs = 0x00000000;

/////
int input29;
int input28;
int input27;
int input26;
int input25;
int input24;
int input23;
int input22;
int input21;
////////////
int input20;
int input19;
int input18;
int input15;
int input14;
int input13;
int input12;
int input11;
int input9;
//////////
int analog1;
int analog0;
int analog4;
int analog8;
int analog5;
int analog9;
int analog6;
int analog2;
int analog3;
int analog7;
//////
int PRND;
float throttle;
float kecepatan;
float distance;
float suhuMotor;
float persentaseBaterai;
float SoC;
float kapasitasBaterai;
//float odometer;
int fault;
float baterai;
bool highBeam,lowBeam,handBrake,footBrake,leftSign,rightSign,door,doorFL,doorFR,doorRL,doorRR,seatBelt;
int fourWD; //2H,4H,4L

//int outputled = ;
// Callback methods prototypes
void t1Callback();
void t2Callback();
void t3Callback();
void inputCallback();
void commCallback();
void comProcessCallback();

uint8_t t1_counter=0;
uint8_t t2_counter=0;
uint8_t t3_counter=0;
char receivedChar;
//Tasks

Scheduler runner;
// Task(unsigned long aInterval=0, long aIterations=0, TaskCallback aCallback=NULL, Scheduler* aScheduler=NULL, bool aEnable=false, TaskOnEnable aOnEnable=NULL, TaskOnDisable aOnDisable=NULL
Task t1(50, -1, &t1Callback, &runner, true); //0CFF09D0 // VCU -> TCU
Task t2(10, -1, &t2Callback, &runner, true); //0CFF10D0 // VCU -> TCU  
Task input(50, -1, &inputCallback, &runner, true); //read pin input
Task com(200, -1, &commCallback, &runner, true);
Task comProcess(0, TASK_ONCE, NULL, &runner, false, &comProcessCallback);

void t1Callback() {
  canMsg1.data[7]=t1_counter;
  t1_counter++;
  mcp2515.sendMessage(&canMsg1);
}

void t2Callback() {
  canMsg2.data[7]=t2_counter;
  t2_counter++;
  mcp2515.sendMessage(&canMsg2);
}

void t3Callback() {
  t3_counter++;
  canMsg3.data[6]=t3_counter;
  mcp2515.sendMessage(&canMsg3);
}

void inputCallback() {
  //if input low ==  input ON
  input31 = digitalRead(31);
  input30 = digitalRead(30);
  input29 = digitalRead(29);
  input28 = digitalRead(28);
  input27 = digitalRead(27);
  input26 = digitalRead(26);
  input25 = digitalRead(25);
  input24 = digitalRead(24);
  input23 = digitalRead(23);
  input22 = digitalRead(22);
  input21 = digitalRead(21);
  input20 = digitalRead(20);
  input19 = digitalRead(19);
  input18 = digitalRead(18);
  input15 = digitalRead(15);
  input14 = digitalRead(14);
  input13 = digitalRead(13);
  input12 = digitalRead(12);
  input11 = digitalRead(11);
  input9 = digitalRead(9);
  analog1= analogRead(A1);
  analog0= analogRead(A0);
  analog4= analogRead(A4);
  analog8= analogRead(A8);
  analog5= analogRead(A5);
  analog9= analogRead(A9);
  analog6= analogRead(A6);
  analog2= analogRead(A2);
  analog3= analogRead(A3);
  analog7= analogRead(A7);
  baterai = (41*10.0/25)*(analog1/1024.0);
  
  Serial.println(baterai);

/*
  Serial.print(input31);
  Serial.print(input30);   
  Serial.print(input29);
  Serial.print(input28);
  Serial.print(input27);
  Serial.print(input26);
  Serial.print(input25);
  Serial.print(input24);
  Serial.print(input23);
  Serial.print(input22);
  Serial.print(input21);
  Serial.print(input20);
  Serial.print(input19);
  Serial.print(input18);
  Serial.print(input15);
  Serial.print(input14);
  Serial.print(input13);
  Serial.print(input12);
  Serial.print(input11);
  Serial.println(input9);
*/
    
  //canMsg3.data[6]=t3_counter;
  //mcp2515.sendMessage(&canMsg3);
}


void commCallback() {
  if (Serial.available() > 0) {
        receivedChar = Serial.read();
        comProcess.enableDelayed();
    }
}

void comProcessCallback(){
    canMsg3.data[4] = 0x94;
    canMsg3.data[5] = 0x13;
    //canMsg3.data[4] = 100;
    //canMsg3.data[5] = 0x13;

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

void setup () {
  Serial.begin(9600);
  SPI.begin(); // Begins SPI communication
  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ); // Sets CAN at speed 500KBPS and Clock 8MHz
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

  canMsg1.can_id = 0x0CFF09D0| CAN_EFF_FLAG; // CAN id 
  canMsg1.can_dlc = 8; // CAN data length as 8
  canMsg1.data[0] = 0x00; 
  canMsg1.data[1] = 0x00; 
  canMsg1.data[2] = 0x00; 
  canMsg1.data[3] = 0x00; 
  canMsg1.data[4] = 0x00;
  canMsg1.data[5] = 0x00;
  canMsg1.data[6] = 0x00;
  canMsg1.data[7] = 0x00;

  canMsg2.can_id = 0x0CFF10D0| CAN_EFF_FLAG; // CAN id 
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

void loop () {
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

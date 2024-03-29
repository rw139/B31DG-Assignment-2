//inclusion of ticker and monitor libraries
#include <B31DGMonitor.h>
#include <Ticker.h>

//defining I/O pins
#define t1out 1   //T1 digital out pin
#define t2in 2    //T2 digital in pin
#define t3in 3    //T3 digital in pin
#define t4in 4    //T4 analogue in pin
#define t4out 10  //T4 LED pin out

//calling monitor and ticker
B31DGCyclicExecutiveMonitor monitor;
Ticker FrameTick;

/*  Task computation times:
---------------------------------------------------------------------------------------------------------
    Task1 - 284us
    Task2 - 2005us
    Task3 - 2005us
    Task4 - 57us
    Task5 - 1283us
---------------------------------------------------------------------------------------------------------    
*/

//variables for timing using micros()
unsigned long tm;
unsigned long newtm;

//freqency calculation on Task2 & 3 square waves
unsigned long freqhigh;
unsigned int freq1;
unsigned int freq2;
int mapf1;
int mapf2;


//Task4 avg analogue value calculation
int a[4];
int counter = 0;
int avg = 0;

//frame counter
unsigned long framecount = 0;


void setup() {
  //set baud rate 9600
  Serial.begin(9600);

  //initialise output pins for Task1 & 4
  pinMode(t1out,OUTPUT);
  pinMode(t4out,OUTPUT);

  //initialise input pins for Task2, 3 & 4
  pinMode(t2in,INPUT);
  pinMode(t3in,INPUT);
  pinMode(t4in,INPUT);
  
  //4ms window ticker
  FrameTick.attach_ms(4, Frame);
  delayMicroseconds(2000);
  //begin monitor of task timings
  monitor.startMonitoring();

  
}

void loop() {
  /*
  //task timing loop
  //takes average time of 1000 task runs 
  for(int i = 0;i<1000; i++){
  tm = micros();
  Task1();
  newtm = newtm + (micros()-tm);
  }
  newtm = newtm/1000;
  Serial.println(newtm);
  newtm = 0;
  */
}

void Task1(){
  //Task1 pulse sequence
  monitor.jobStarted(1);
  digitalWrite(t1out, HIGH);
  delayMicroseconds(200);
  digitalWrite(t1out, LOW);
  delayMicroseconds(50);
  digitalWrite(t1out, HIGH);
  delayMicroseconds(30);
  digitalWrite(t1out, LOW);
  monitor.jobEnded(1);
}

void Task2(){
  //measure square wave freq (Hz)
  monitor.jobStarted(2);
  freqhigh = pulseIn(t2in, HIGH, 3000);
  freq1 = 1000000 / (freqhigh*2);
  monitor.jobEnded(2);
}

void Task3(){
  //measure square wave freq (Hz)
  monitor.jobStarted(3);
  freqhigh = pulseIn(t3in, HIGH, 3000);
  freq2 = 1000000 / (freqhigh*2);
  monitor.jobEnded(3);
}

void Task4(){ 
  monitor.jobStarted(4);
  //counter 0-3
  if(counter >= 3){
    counter = 0;
  }

  //read analogue signal on pin
  a[counter] = analogRead(t4in);

  //average value
  avg = (a[0]+a[1]+a[2]+a[3])/4;

  counter++;
  
  //LED high if above 2047 (max 4096)
  if(avg > 2047){
    digitalWrite(t4out, HIGH);
  }
  else{
    digitalWrite(t4out, LOW);
  }
  monitor.jobEnded(4);
}

void Task5(){
  //running Task5 every 100ms - counter % 25 (removes need for 200ms/50 case frame cycle)
  if(framecount % 25 == 0){
  monitor.jobStarted(5);
  //setting outliers of freq range as 0 or 99
  mapf1 = map(freq1, 333, 1000, 0, 99);
  mapf2 = map(freq2, 500, 1000, 0, 99);

  mapf1 = constrain(mapf1, 0, 99);
  mapf2 = constrain(mapf2, 0, 99);
  
  //print T2 & 3 values
  Serial.printf("%d, %d \n", mapf1, mapf2);
  monitor.jobEnded(5);
  }
  else{

  }
}

void Frame(){   
   unsigned int slot = framecount % 10;
  switch (slot) {
     case 0:  Task1();                Task3();                Task5();  break;
     case 1:  Task1();    Task2();                 Task4();             break;
     case 2:  Task1();                Task3();                          break;
     case 3:  Task1();                                                  break;
     case 4:  Task1();                Task3();                          break;
     case 5:  Task1();                                        Task5();  break;
     case 6:  Task1();                Task3();                          break;
     case 7:  Task1();    Task2();                 Task4();             break;
     case 8:  Task1();                Task3();                          break;
     case 9:  Task1();                                                  break;
  }
  
  framecount++;
  
  
}

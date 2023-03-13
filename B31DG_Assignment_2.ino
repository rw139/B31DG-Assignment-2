#include <B31DGMonitor.h>
#include <Ticker.h>

#define t1out 1 //T1 digital out pin
#define t2in 2  //T2 digital in pin
#define t3in 3  //T3 digital in pin
#define t4in 4  //T4 analogue in pin
#define t4out 10  //T4 LED pin out

B31DGCyclicExecutiveMonitor monitor;
Ticker FrameTick;

const int task1p = 4;
const int task2p = 20;
const int task3p = 8;
const int task4p = 20;
const int task5p = 100;

//variables for using micros()
unsigned long tm;
unsigned long newtm;

//freqency calculation on T2 & 3 square waves
float sighigh;
float siglow;
int freq1;
int freq2;

int framecount = 0;

//T4 avg analogue value calculation
int a[4];
int counter = 0;
int avg = 0;

unsigned long frameCounter = 0;

void setup() {

  //set baud rate 9600
  Serial.begin(9600);
 
  FrameTick.attach_ms(4, Frame);
 
  //initialise output pin for T1 & 4
  pinMode(t1out,OUTPUT);
  pinMode(t4out,OUTPUT);

  //initialise input pins for T2, 3 & 4
  pinMode(t2in,INPUT);
  pinMode(t3in,INPUT);
  pinMode(t4in,INPUT);
  Task1();
  delayMicroseconds(2000);
  monitor.startMonitoring();
}

void loop() {
  /*
  //task timing loop 
  for(int i = 0;i<1000; i++){
  tm = micros();
  Task2();
  newtm = newtm + (micros()-tm);
  }
  newtm = newtm/1000;
  Serial.println(newtm);
  newtm = 0;
  */
  Frame();
}

void Task1(){//284us
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

void Task2(){//2005ms
  //measure square wave freq (Hz)
  monitor.jobStarted(2);
  sighigh = pulseIn(t2in, HIGH, 1000);
  siglow = pulseIn(t2in, LOW), 1000;
  freq1 = 1000000 / (sighigh + siglow);
  monitor.jobEnded(2);
}

void Task3(){//2005ms
  //measure square wave freq (Hz)
  monitor.jobStarted(3);
  sighigh = pulseIn(t3in, HIGH, 1000);
  siglow = pulseIn(t3in, LOW, 1000);
  freq2 = 1000000 / (sighigh + siglow);
  monitor.jobEnded(3);
}

void Task4(){ //57us
  //reset counter after 4th reading
  monitor.jobStarted(4);
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

void Task5(){//1283us
  //setting outliers of freq range as 0 or 1000Hz
  monitor.jobStarted(5);
   if(freq1 > 1000){
    freq1 = 1000;    
  }
  else if(freq2 > 1000){
    freq2 = 1000;
  }
  else if(freq1 < 333){
    freq1 = 0;
  }
  else if(freq2 < 333){
    freq2 = 0;
  }

  //calculating for 0-99 range
  freq1 = freq1/333*33;
  freq2 = freq2/333*33;

  //print T2 & 3 values
  Serial.printf("%d, %d \n", freq1, freq2);
  monitor.jobEnded(5);
}

void Frame(){   
   unsigned int slot = framecount % 10;
   switch (slot) {
     case 0:  Task1();    Task2();                                      break;
     case 1:  Task1();                Task3();     Task4();             break;
     case 2:  Task1();                                        Task5();  break;
     case 3:  Task1();                Task3();                          break;
     case 4:  Task1();                                                  break;
     case 5:  Task1();    Task2();    Task3();                          break;
     case 6:  Task1();                             Task4();             break;
     case 7:  Task1();                Task3();                          break;
     case 8:  Task1();                                                  break;
     case 9:  Task1();                Task3();                          break;
     case 10: Task1();    Task2();                                      break;
     case 11: Task1();                Task3();     Task4();             break;
     case 12: Task1();                                                  break;
     case 13: Task1();                Task3();                          break;
     case 14: Task1();                                                  break;
     case 15: Task1();    Task2();    Task3();                          break;
     case 16: Task1();                             Task4();             break;
     case 17: Task1();                Task3();                          break;
     case 18: Task1();                                                  break;
     case 19: Task1();                Task3();                          break;
     case 20: Task1();    Task2();                                      break;
     case 21: Task1();                Task3();     Task4();             break;
     case 22: Task1();                                                  break;
     case 23: Task1();                Task3();                          break;
     case 24: Task1();                                                  break;
     case 25: Task1();    Task2();    Task3();                          break;
     case 26: Task1();                             Task4();             break;
     case 27: Task1();                Task3();                Task5();  break;
     case 28: Task1();                                                  break;
     case 29: Task1();                Task3();                          break;
     case 30: Task1();    Task2();                                      break;
     case 31: Task1();                Task3();     Task4();             break;
     case 32: Task1();                                                  break;
     case 33: Task1();                Task3();                          break;
     case 34: Task1();                                                  break;
     case 35: Task1();    Task2();    Task3();                          break;
     case 36: Task1();                             Task4();             break;
     case 37: Task1();                Task3();                          break;
     case 38: Task1();                                                  break;
     case 39: Task1();                Task3();                          break;
     case 40: Task1();    Task2();                                      break;
     case 41: Task1();                Task3();     Task4();             break;
     case 42: Task1();                                                  break;
     case 43: Task1();                Task3();                          break;
     case 44: Task1();                                                  break;
     case 45: Task1();    Task2();    Task3();                          break;
     case 46: Task1();                             Task4();             break;
     case 47: Task1();                Task3();                          break;
     case 48: Task1();                                                  break;
     case 49: Task1();                Task3();                          break;
  }
  
  framecount++;
}
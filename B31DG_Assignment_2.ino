#define t1out 1 //T1 digital out pin
#define t2in 2  //T2 digital in pin
#define t3in 3  //T3 digital in pin
#define t4in 4  //T4 analogue in pin
#define t4out 5  //T4 LED pin out

B31DGCyclicExecutiveMonitorMonitor;

//define [Ci, Ti, Di]
const t1[] = [0.28, 4];
const t2[] = [4.5, 20];
const t3[] = [3, 8];
const t4[] = [0.5, 20];
const t5[] = [1, 100];

//freqency calculation on T2 & 3 square waves
float sighigh;
float siglow;
int freq1;
int freq2;

//T4 avg analogue value calculation
int a[4];
int counter;
int avg;

void setup() {

  //set baud rate 9600
  Serial.begin(9600);

  B31DGCyclicExecutiveMonitor::startMonitoring(),
  
  //initialise output pin for T1 & 4
  pinMode(t1out,OUTPUT);
  pinMode(t4out,OUTPUT);
  
  //initialise input pins for T2, 3 & 4
  pinMode(t2in,INPUT);
  pinMode(t3in,INPUT);
  pinMode(t4in,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void T1(){
  //T1 pulse sequence
  digitalWrite(t1out, HIGH);
  delay(0.2);
  digitalWrite(t1out, LOW);
  delay(0.05);
  digitalWrite(t1out, HIGH);
  delay(0.03);
  digitalWrite(t1out, LOW);
}

void T2(){
  //measure square wave freq (Hz)
  sighigh = pulseIn(t2in, HIGH);
  siglow = pulseIn(t2in, LOW);
  freq1 = 1000000 / (sighigh + siglow);
}

void T3(){
  //measure square wave freq (Hz)
  sighigh = pulseIn(t3in, HIGH);
  siglow = pulseIn(t3in, LOW);
  freq2 = 1000000 / (sighigh + siglow);
}

void T4(){
  //reset counter after 4th reading
  if(counter == 3){
    counter = 0;
  }

  //read analogue signal on pin
  a[counter] = analogRead(t3in);

  //average value
  avg = (a[0]+a[1]+a[2]+a[3])/4;

  if(avg > 2047){
    digitalWrite(t4out, HIGH);
  }
}

void T5(){
  //setting freq range as 333-1000Hz
   if(freq1 > 1000){
    freq1 = 1000;    
  }
  else if(freq2 > 1000){
    freq2 = 1000;
  }
  else if(freq1 < 333){
    freq1 = 333;
  }
  else if(freq2 < 333){
    freq2 = 333;
  }

  //calculating for 0-99 range
  freq1 = freq1/333*33;
  freq2 = freq2/333*33;

  //print T2 & 3 values
  Serial.printf("%d, %d \n", freq1, freq2);
}
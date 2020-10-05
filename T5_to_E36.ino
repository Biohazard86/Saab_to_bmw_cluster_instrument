/*
Biohazard_86

Saab Trionic 5 to Bmw E36 cluster RPM conversion by davidbarrios@usal.es

Most of the Saab dashboards works with 2 or 4 pulses per rev. 

BMW E36 cluster uses 3 pulses per rev

Maximum pulses per rev is:
7000*2 = 14.000 in the Saab output of the ECU
7000*3 = 21.000 to the BMW cluster

21krpm / 60secs = 350 rev per sec 
1/350hz = 2.8571*10^-3 
In the worst case we have to read a pulse every 2.8571ms (Period) or 350 pulses every second = 350Hz (Frecuency).
Arduino allows up to 8Mhz input/output frecuency


*/


// Define the pulses per revolution of both cars
#define bmw_pulses_rev  3
#define saab_pulses_rev  2
#define max_rpm_output_allowed 7100 // to cant broke the E36 cluster putting 100k rpm on the cluster
int rev_limit_1 = 1800;
int rev_limit_2 = 5500;
int rev_limit_3 = 6100;


//Define the pins
int input_saab = 2;   // In the pin 13 of the arduino we have the ECU output from the Saab ECU
int output_bmw = 3;    // In the pin 3 of the arduino we have the dashboard input

// We have differents lights for the rev limiter. For example, 1st for the max torque point. 2nd for the max hp output and 3rd if you're near the rpm limiter
int lightrevlim1 = 4;
int lightrevlim2 = 5;
int lightrevlim3 = 6;


//Variable
double actual_rpm = 0;
unsigned long time_per_pulse_micro = 0; //microseconds
double time_per_pulse_ms = 0; //miliseconds
double frek = 0;
unsigned int time_1 = 0;
unsigned int time_2 = 0;
int counter = 0;
int counter2 = 0;
unsigned int t_lights_1 = 0;
unsigned int t_lights_2 = 0;
volatile bool flag = false;
bool flag_serial = false;
bool flag_lights = false;
char data;


void setup() {
  Serial.begin(9600);   //Serial port
  pinMode(input_saab, INPUT);  //We define the pin 1 called input_saab as an input
  pinMode(output_bmw, OUTPUT);  //We define the pin 3 called output_bmw as an output

  //............

  pinMode(lightrevlim1, OUTPUT);  //led 1
  pinMode(lightrevlim2, OUTPUT);  // led 2
  pinMode(lightrevlim3, OUTPUT);  // led 3


  // Serial output
  Serial.print("\n\n\n---------------------------------------------\n");
  Serial.print("    SAAB Trionic 5 to BMW E36 cluster\n");
  Serial.print("               Biohazard_86\n");
  Serial.print("--------------------------------------------\n");


 counter = 0;  
}

void rpm_fun() {
  if(counter == 0){
    time_1=micros();
    counter++;
    flag=false;
  }
  else{
    counter = 0;
    time_2=micros();
    flag=true;
    }
  }


void lights_func(){
  if(actual_rpm > rev_limit_3){
      digitalWrite(lightrevlim3, HIGH);   //
    }
  else{
      digitalWrite(lightrevlim3, LOW);
    }
  
  

  if(actual_rpm > rev_limit_2){
      digitalWrite(lightrevlim2, HIGH);   //
    }
  else{
      digitalWrite(lightrevlim2, LOW);
    }
  
  


  if(actual_rpm > rev_limit_1){
      digitalWrite(lightrevlim1, HIGH);   //
    }
  else{
      digitalWrite(lightrevlim1, LOW);
    }
  
  

}


void loop() {
  
  attachInterrupt(0, rpm_fun, RISING);
  if(flag){
    detachInterrupt(0);
    time_per_pulse_micro = (time_2 - time_1);
    
    time_per_pulse_ms = time_per_pulse_micro / 1000.00;
    
    frek = pow((time_per_pulse_ms / 1000), -1); 
    
    actual_rpm = (frek * 60)/ saab_pulses_rev;

    counter2++;
    if(counter2>0){
      //Serial.print("\n RREK:   ");
      //Serial.print("\n");
      //Serial.print(frek);
      
      //Serial.print("\n TPP micro:  ");
      //Serial.print("\n");
      //Serial.print(time_per_pulse_micro);

    
      //Serial.print("\n TPP ms:  ");
      //Serial.print("\n");
      //Serial.print(time_per_pulse_ms);

    
      //Serial.print("\n RPM:   ");
      Serial.print("\n");
      Serial.print(actual_rpm);

      counter2=0;
    }
    
    flag=false;
    }

    // 10 times per second
    if(flag_lights == false){
      t_lights_1 = millis();
      flag_lights = true;
      }
    if(flag_lights){
      if(t_lights_1 > 100){
        lights_func();
        flag_lights == false;
      }
      
      }

    

    
  
}

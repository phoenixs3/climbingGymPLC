//-----Pin definitions------//

#define valveOneIn        19
#define valveTwoIn        18
#define valveThreeIn      8

#define greenButtonOne    23
#define greenButtonTwo    21
#define redButtonOne      22
#define redButtonTwo      20

#define valveOneMotor     10
#define valveTwoMotor     9
#define valveThreeMotor   6

#define fanOne            13
#define fanTwo            11

#define greenLedOne       5
#define greenLedTwo       3
#define redLedOne         7
#define redLedTwo         0

///////////////////////////////

//-----Program variables-----//
bool buttonstates[4];
bool lastbuttonstates[4];
unsigned long pressedTime[4];
unsigned long releasedTime[4];
bool isPressing[4];
bool isLongDetected[4];


const int LONG_PRESS_TIME  = 2000;  //Uints in milliseconds
const int SHORT_PRESS_TIME = 20;

unsigned long previousMillisPrint = 0;        // will store last time LED was updated
const long printInterval = 300;           // interval at which to blink (milliseconds)

///////////////////////////////

void setup() {
  Serial.begin(9600);
  Serial.print("Init...");
  pinMode(valveOneIn,   INPUT);
  pinMode(valveTwoIn,   INPUT);
  pinMode(valveThreeIn, INPUT);
  pinMode(greenButtonOne, INPUT);
  pinMode(greenButtonTwo, INPUT);
  pinMode(redButtonOne,   INPUT);
  pinMode(redButtonTwo,   INPUT);
  pinMode(valveOneMotor,   OUTPUT);
  pinMode(valveTwoMotor,   OUTPUT);
  pinMode(valveThreeMotor, OUTPUT);
  pinMode(fanOne, OUTPUT);
  pinMode(fanTwo, OUTPUT);
  pinMode(greenLedOne, OUTPUT);
  pinMode(greenLedTwo, OUTPUT);
  pinMode(redLedOne,   OUTPUT);
  pinMode(redLedTwo,   OUTPUT);
  Serial.println("done");
}

void loop() {
  unsigned long currentMillis = millis();
    if (currentMillis - previousMillisPrint >= printInterval) {
    previousMillisPrint = currentMillis;
    printDebug();
  }
  detectButtonpresses();
  readInputs();
}


void detectButtonpresses(){
  for (int i=0; i<4; i++){
    if(lastbuttonstates[i] == LOW && buttonstates[i] == HIGH) {        //Rising edge detection
      pressedTime[i] = millis();
      isPressing[i] = true;
      isLongDetected[i] = false;
    } else if(lastbuttonstates[i] == HIGH && buttonstates[i] == LOW) { //Falling edge detection
      isPressing[i] = false;
      releasedTime[i] = millis();
      long pressDuration = releasedTime[i] - pressedTime[i];
      if(pressDuration < SHORT_PRESS_TIME){
        Serial.print(i);
        Serial.println(" short press detected");
      }
    }
    if(isPressing[i] == true && isLongDetected[i] == false) {
      long pressDuration = millis() - pressedTime[i];
      if(pressDuration > LONG_PRESS_TIME) {
        Serial.print(i);
        Serial.println(" long press detected");
        isLongDetected[i] = true;
      }
    }
    lastbuttonstates[i] = buttonstates[i];
  }
}

void readInputs(){
  buttonstates[0] = !digitalRead(greenButtonOne);   //invert green buttons as they are normally closed (NC)
  buttonstates[1] = !digitalRead(greenButtonTwo);
  buttonstates[2] = digitalRead(redButtonOne);
  buttonstates[3] = digitalRead(redButtonTwo);
}

void printDebug(){
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("///////////    Climbing Gym PLC    ///////////");
  Serial.println("");

  Serial.println("Input Status:");

  Serial.print("- Green Button 1 State: ");
  Serial.println(buttonstates[0]);
  Serial.print("- Green Button 2 State: ");
  Serial.println(buttonstates[1]);
  Serial.print("- Red Button 1 State: ");
  Serial.println(buttonstates[2]);
  Serial.print("- Red Button 2 State: ");
  Serial.println(buttonstates[3]);


  Serial.println("");
  Serial.println("");

  Serial.println("Valve Control");
  Serial.println("- todo");

  //valves are closed by writing pin high

  Serial.println("");
  Serial.println("");
  
  Serial.println("Fan Control:");
  Serial.println("- todo");

  //fan speed 1: 26 analogwrite)
  //speed 2: 51
  //speed 3: 76
  
  Serial.println("");
  Serial.println("");

  Serial.println("");
  Serial.println("//////////////////////////////////////////////");
}

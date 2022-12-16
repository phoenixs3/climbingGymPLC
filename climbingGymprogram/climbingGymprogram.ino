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

//-----User definable variables-----//
const int LONG_PRESS_TIME  = 2000;  //Units in milliseconds
const int SHORT_PRESS_TIME = 1000;
const int movevalveserrorthreshold = 300000;   //5 minutes
const long printInterval = 300;   //also used for error state red light flash interval
const int fanspeed0 = 0;
const int fanspeed1 = 26;
const int fanspeed2 = 51;
const int fanspeed3 = 76;
const bool bypassvalvefeedback = 1;
//////////////////////////////////////

//-----Program variables-----//
bool buttonstates[4];
bool lastbuttonstates[4];
unsigned long pressedTime[4];
unsigned long releasedTime[4];
bool isPressing[4];
bool isLongDetected[4];

unsigned long previousMillisPrint, greenButtonOptionOneMillis, greenButtonOptionTwoMillis, redButtonOptionOneMillis, redButtonOptionTwoMillis = 0;

bool dogreenButtonOptionOne, dogreenButtonOptionTwo, doredButtonOptionOne, doredButtonOptionTwo = true;
bool startgreenButtonOptionOne, startgreenButtonOptionTwo, startredButtonOptionOne, startredButtonOptionTwo = false;
int movevalvestimer = 0;
bool errorState = false;
bool redState = false;
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
    //printDebug();
    if(errorState){errorFlash();}
  }
  detectButtonpresses();
  readInputs();
  if(dogreenButtonOptionOne && !errorState){greenButtonOptionOne();}
  if(dogreenButtonOptionTwo && !errorState){greenButtonOptionTwo();}
  if(doredButtonOptionOne && !errorState){redButtonOptionOne();}
  if(doredButtonOptionTwo && !errorState){redButtonOptionTwo();}
  
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
        if(i == 0 || i == 1){ //If either green button short press
          startgreenButtonOptionOne = true; 
          dogreenButtonOptionOne = true;
          dogreenButtonOptionTwo = false;
          doredButtonOptionOne = false;
          doredButtonOptionTwo = false;
          movevalvestimer = 0;
          flashgreenlights();
        }
        if(i == 3 || i == 2){ //If either red button short press
          startredButtonOptionOne = true; 
          dogreenButtonOptionOne = false;
          dogreenButtonOptionTwo = false;
          doredButtonOptionOne = true;
          doredButtonOptionTwo = false;
          movevalvestimer = 0;
          flashredlights();
        }
        Serial.print(i);
        Serial.println(" short press detected");
      }
    }
    if(isPressing[i] == true && isLongDetected[i] == false) {
      long pressDuration = millis() - pressedTime[i];
      if(pressDuration > LONG_PRESS_TIME) {
        if(i == 0 || i == 1){     //If either green button long press
          startgreenButtonOptionTwo = true; 
          dogreenButtonOptionOne = false;
          dogreenButtonOptionTwo = true;
          doredButtonOptionOne = false;
          doredButtonOptionTwo = false;
          movevalvestimer = 0;
          flashgreenlights();
        }
        if(i == 3 || i == 2){ //If either red button Long press
          startredButtonOptionTwo = true; 
          dogreenButtonOptionOne = false;
          dogreenButtonOptionTwo = false;
          doredButtonOptionOne = false;
          doredButtonOptionTwo = true;
          movevalvestimer = 0;
          flashservicelights();
        Serial.print(i);
        Serial.println(" long press detected");
        isLongDetected[i] = true;
      }
    }
    lastbuttonstates[i] = buttonstates[i];
  }
}
}

void greenButtonOptionOne(){
  if(startgreenButtonOptionOne && !errorState){   //Starting procedure only done once
    Serial.println("Doing greenButtonOptionOne");
    fanSpeed(fanspeed1);
    if(!movevalves(true, false, true)){                     //Keep trying to move valves if they arent in position
        movevalvestimer++;
        delay(1);
    } else {startgreenButtonOptionOne = false; greenButtonOptionOneMillis = millis();}
      //if(movevalvestimer > movevalveserrorthreshold){errorState = true;}
  }
  if((millis() - greenButtonOptionOneMillis) > 14400000){fanSpeed(0);}                         //After 4 hrs turn off fans
  if((millis() - greenButtonOptionOneMillis) > 72000000){startgreenButtonOptionOne = true;}    //After 20hrs start routine again
}

void greenButtonOptionTwo(){
  if(startgreenButtonOptionTwo && !errorState){   //Starting procedure only done once
    Serial.println("Doing greenButtonOptionTwo");
    fanSpeed(fanspeed1);
    if(!movevalves(true, false, true)){
      movevalvestimer++;
      delay(1);
    } else {startgreenButtonOptionTwo = false; greenButtonOptionTwoMillis = millis();}
    
    //startgreenButtonOptionTwo = false;
  }
  if((millis() - greenButtonOptionTwoMillis) > 7200000){fanSpeed(1);}                         //After 2 hrs turn off fans
  if((millis() - greenButtonOptionTwoMillis) > 36000000){startgreenButtonOptionTwo = true;}    //After 10hrs start routine again
}

void redButtonOptionOne(){
  if(startredButtonOptionOne){   //Starting procedure only done once
    Serial.println("Doing redButtonOptionOne");
    fanSpeed(fanspeed0);
    if(!movevalves(true, false, true)){
      movevalvestimer++;
      delay(1);
    } else {startredButtonOptionOne = false; redButtonOptionOneMillis = millis();}
    //startredButton = false;
  }
  if((millis() - redButtonOptionOneMillis) > 3600000){fanSpeed(0), startredButtonOptionOne = true;}                         //Loop after 1hr  
}

void redButtonOptionTwo(){
  if(startredButtonOptionTwo){
    Serial.println("Doing redButtonOptionTwo");
    fanSpeed(fanspeed0);
    if(!movevalves(true, true, true)){
      movevalvestimer++;
      delay(1);
    } else {startredButtonOptionTwo = false; redButtonOptionTwoMillis = millis();}
  }
  if((millis() - redButtonOptionTwoMillis > 3600000)){fanSpeed(0), startredButtonOptionTwo = true;}
}

void errorFlash(){
  redState = !redState;
  digitalWrite(redLedOne, redState);
  digitalWrite(redLedTwo, redState);
}

void flashgreenlights(){
    digitalWrite(greenLedOne, HIGH);
    digitalWrite(greenLedTwo, HIGH);
    delay(500);
    digitalWrite(greenLedOne, LOW);
    digitalWrite(greenLedTwo, LOW);
    delay(500);
    digitalWrite(greenLedOne, HIGH);
    digitalWrite(greenLedTwo, HIGH);
    delay(500);
    digitalWrite(greenLedOne, LOW);
    digitalWrite(greenLedTwo, LOW);
    delay(500);
    digitalWrite(greenLedOne, HIGH);
    digitalWrite(greenLedTwo, HIGH);
}

void flashredlights(){
    digitalWrite(redLedOne, HIGH);
    digitalWrite(redLedTwo, HIGH);
    delay(500);
    digitalWrite(redLedOne, LOW);
    digitalWrite(redLedTwo, LOW);
    delay(500);
    digitalWrite(redLedOne, HIGH);
    digitalWrite(redLedTwo, HIGH);
    delay(500);
    digitalWrite(redLedOne, LOW);
    digitalWrite(redLedTwo, LOW);
    delay(500);
    digitalWrite(redLedOne, HIGH);
    digitalWrite(redLedTwo, HIGH);
}

void flashservicelights(){
    digitalWrite(redLedOne, LOW);
    digitalWrite(redLedTwo, LOW);
    digitalWrite(greenLedOne, HIGH);
    digitalWrite(greenLedTwo, HIGH);
    delay(500);
    digitalWrite(redLedOne, HIGH);
    digitalWrite(redLedTwo, HIGH);
    digitalWrite(greenLedOne, LOW);
    digitalWrite(greenLedTwo, LOW);
    delay(500);
    digitalWrite(redLedOne, LOW);
    digitalWrite(redLedTwo, LOW);
    digitalWrite(greenLedOne, HIGH);
    digitalWrite(greenLedTwo, HIGH);
    delay(500);
    digitalWrite(redLedOne, HIGH);
    digitalWrite(redLedTwo, HIGH);
    digitalWrite(greenLedOne, LOW);
    digitalWrite(greenLedTwo, LOW);
    delay(500);
    digitalWrite(redLedOne, LOW);
    digitalWrite(redLedTwo, LOW);
    digitalWrite(greenLedOne, HIGH);
    digitalWrite(greenLedTwo, HIGH);
}

bool movevalves(bool valve1pos, bool valve2pos, bool valve3pos){
  //Caution: untested so logic output/input states may need inverting to match hardware
  digitalWrite(valveOneMotor, valve1pos);
  digitalWrite(valveTwoMotor, valve2pos);
  digitalWrite(valveThreeMotor, valve3pos);
  if(bypassvalvefeedback){
      if(analogRead(valveOneIn) != valve1pos){return true;}   //Check if the valve is moved, if not return false
      if(analogRead(valveTwoIn) != valve2pos){return true;}
      if(analogRead(valveThreeIn) != valve3pos){return true;}
  } else if (!bypassvalvefeedback) {
      if(digitalRead(valveOneIn) != valve1pos){return false;}   //Check if the valve is moved, if not return false
      if(digitalRead(valveTwoIn) != valve2pos){return false;}
      if(digitalRead(valveThreeIn) != valve3pos){return false;}
  }
  else{return true;} 
}

void fanSpeed(int fanspeed){
  analogWrite(fanOne, fanspeed);
  analogWrite(fanTwo, fanspeed);
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

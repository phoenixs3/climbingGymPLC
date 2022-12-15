//-----Pin definitions------//

#define valveOneIn =    2;
#define valveTwoIn =    12;
#define valveThreeIn =  4;

#define greenButtonOne =  8;
#define greenButtonTwo =  18;
#define redButtonOne =    19;
#define redButtonTwo =    20;

#define valveOneMotor =   10;
#define valveTwoMotor =   9;
#define valveThreeMotor = 6;

#define fanOne = 13;
#define fanTwo = 11;

#define greenLedOne = 5;
#define greenLedTwo = 3;
#define redLedOne =   7;
#define redLedTwo =   0;

///////////////////////////////

//-----Program variables-----//

int speedOne;
int speedTwo;
int speedThree;

///////////////////////////////

void setup() {
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

}

void loop() {
greenButtonOptionOne();
greenButtonOptionTwo();
redButtonOptionOne();
cleanOption();
}

//If ether of the Green Buttons is pressed once run System Option One
void greenButtonOptionOne(){
  if(digitalRead(greenButtonOne)||digitalRead(greenButtonTwo)){runSystemOptionOne();}
}
  
//If ether of the Green Buttons is pressed & help for 5 seconds run System Option Two
void greenButtonOptionTwo(){
 if(digitalRead(greenButtonOne)||digitalRead(greenButtonTwo)){runSystemOptionTwo();}
}

//If ether of the Red Buttons is pressed once run Stop Everything
void redButtonOptionOne(){
 if(digitalRead(redButtonOne)||digitalRead(redButtonTwo)){stopEverything();}
}

//If ether of the Red Buttons is pressed once run Stop Everything
void cleanOption(){
 if(digitalRead(greenButtonOne)&&digitalRead(redButtonOne)||digitalRead(greenButtonTwo)&&digitalRead(redButtonTwo)){cleanSystem();}
}

//Valve 1 = Closed, Valve 2 = Closed, Valve 3 = Closed, Fan 1 & Fan 2 running at 20% for 4 hours off for 20 hours
void runSystemOptionOne(){
  digitalWrite(valveOneMotor,   HIGH); //Valve 1 Closed
  digitalWrite(valveTwoMotor,   HIGH); //Valve 2 Closed
  digitalWrite(valveThreeMotor, HIGH); //Valve 3 Closed

    if(digitalRead(valveOneIn)&&digitalRead(valveTwoIn)&&digitalRead(valveThreeIn)){fanSpeedTwo();}

}

//Valve 1 = Open, Valve 2 = Closed, Valve 3 = Closed, Fan 1 & Fan 2 running at 30% for 4 hours off for 20 hours
void runSystemOptionTwo(){
  digitalWrite(valveOneMotor,   LOW); //Valve 1 OPEN
  digitalWrite(valveTwoMotor,   HIGH); //Valve 2 Closed
  digitalWrite(valveThreeMotor, HIGH); //Valve 3 Closed

     if(digitalRead(!valveOneIn)&&digitalRead(valveTwoIn)&&digitalRead(valveThreeIn)){fanSpeedThree();}
     
}

//Valve 1 = Open, Valve 2 = Open, Valve 3 = Open, Fan 1 & Fan 2 running at 10% for 5 min
void stopEverything(){
  digitalWrite(valveOneMotor,   LOW); //Valve 1 Open
  digitalWrite(valveTwoMotor,   LOW); //Valve 2 Open
  digitalWrite(valveThreeMotor, LOW); //Valve 3 Open  

    if(digitalRead(!valveOneIn)&&digitalRead(!valveTwoIn)&&digitalRead(!valveThreeIn)){fanSpeedOne();}  

}

//Valve 1 = Closed, Valve 2 = Closed, Valve 3 = Closed, Fan 1 & Fan 2 running at 20% for 4 hours off for 20 hours
void cleanSystem(){
  digitalWrite(valveOneMotor,   HIGH); //Valve 1 Closed
  digitalWrite(valveTwoMotor,   HIGH); //Valve 2 Closed
  digitalWrite(valveThreeMotor, HIGH); //Valve 3 Closed

    if(digitalRead(valveOneIn)&&digitalRead(valveTwoIn)&&digitalRead(valveThreeIn)){fanSpeedTwo();}  
    
}
void fanSpeedOne(){
  for (int speed = 0; speed <= 255; speed++){
    analogWrite(fanOne, 26);
    delay(10);
  }
  for (int speed = 0; speed <= 255; speed++){
    analogWrite(fanOne, 26);
    delay(10);
  }  
}
//Fan running at 10%

void fanSpeedTwo(){
  for (int speed = 0; speed <= 255; speed++){
    analogWrite(fanOne, 51);
    delay(10);
  }
  for (int speed = 0; speed <= 255; speed++){
    analogWrite(fanOne, 51);
    delay(10);
  }   
}
//Fan running at 20%

void fanSpeedThree(){
  for (int speed = 0; speed <= 255; speed++){
    analogWrite(fanOne, 76);
    delay(10);
  }
  for (int speed = 0; speed <= 255; speed++){
    analogWrite(fanTwo, 76);
    delay(10);
  }  
}
//Fan running at 30%
//Last Change for today

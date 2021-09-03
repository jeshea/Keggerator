int KegButton1Pin = 51;
int KegButton2Pin = 52;
int KegButton3Pin = 53;
//int KegButton4Pin = 53;

bool KegButton1Press;//reads the button state
bool KegButton2Press;
bool KegButton3Press;
//bool KegButton4Press;

bool KegButton1State;//keeps track of the current state of the button after debounce
bool KegButton2State;
bool KegButton3State;
//bool KegButton4State;

long KegButton1StateChangeTime;//tracks how long since last state change
long KegButton2StateChangeTime;
long KegButton3StateChangeTime;
//long KegButton4StateChangeTime;

bool KegButton1Active;//tracks if the button is currently accepting commands
bool KegButton2Active;
bool KegButton3Active;
//bool KegButton4Active;

bool KegButton1LastPress;//tracks debounce state change
bool KegButton2LastPress;
bool KegButton3LastPress;
//bool KegButton4LastPress;

int DebounceDelay = 20;//ms for debounce
long KegButton1Debounce;//tracks how long since last debounce state change
long KegButton2Debounce;
long KegButton3Debounce;
//long KegButton4Debounce;

bool KegButton1SinglePress;//tracks single or double press
bool KegButton2SinglePress;
bool KegButton3SinglePress;
//bool KegButton4SinglePress;

bool KegButton1Running = false;//tracks if a program is already assigned to the button
bool KegButton2Running = false;
bool KegButton3Running = false;
//bool KegButton4Running = false;

int KegButton1Type; //1 for sixtel, 2 for half, 3 for cancel
int KegButton2Type;
int KegButton3Type;
//int KegButton4Type;

long LastBlink;

int Keg1LED = 48;
int Keg2LED = 49;
int Keg3LED = 50;
//int Keg4LED = 23;

const int Heater = 29;
const int Bypass = 25;
const int Keg1ValveIn = 22;//water 25
const int Keg1ValveOut = 24;//acid out 24
const int Keg2ValveIn = 17;//correct
const int Keg2ValveOut = 23;//sani in 23
const int Keg3ValveIn = 26;//sani out 27
const int Keg3ValveOut = 19;//drain 26
//const int Keg4ValveIn = 6;
//const int Keg4ValveOut = 7;
const int Keg1WarningLight = 31;
const int Keg2WarningLight = 32;
const int Keg3WarningLight = 33;
const int Keg4WarningLight = 34;
const int AcidOut = 18;//keg1 out 19
const int AcidIn = 21;//co2 22
const int SanitizerOut = 16;//kleg 3 in 16
const int SanitizerIn = 28;//keg2 out 20
const int Water = 20;//keg1 in 18
const int Drain = 27;//keg3 out 17
const int CO2 = 14; //was 22
const int Pump = 15;
const int PressureSensor = A1;


int CurrentKegIn;
int CurrentKegOut;
String CurrentKeg = "";
int CurrentKegType;
int CurrentKegWarning;
int CurrentStep;
long StepLength;
bool StepInitialize = false;
bool StepComplete = false;
int TargetPressure;
int CurrentPressure;
bool PurgePressureFlag;
int LowTargetPressure = 50;
int CO2TargetPressure = 300;
int PumpTargetPressure = 100;
//********************************************************
//0=PressureTest, 1=KegDrain ,2=PurgeDrain ,3=PurgeSanitizer ,4=AcidWash ,5=SanitizerWash ,6=WaterWash ,7=CO2Pressureize ,8=PurgeAcid , 9 = CO2Blast, 10 = ShortWaterWash, 11 = ShortPurgeDrain, 12 = LongCO2Pressurize, 100=RunComplete
int SixtelArray[]={1,2,6,2,10,11,10,2,10,2,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,4,4,4,4,4,4,4,4,4,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,4,4,4,4,4,4,4,4,4,4,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,8,6,2,10,11,10,2,10,2,5,9,5,9,5,9,5,9,5,9,5,9,5,5,5,5,9,5,9,5,9,5,9,5,3,7,11,7,11,7,11,12,100};
int HalfArray[] = {1,2,6,2,6,11,10,11,10,2,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,4,4,4,4,4,4,4,4,4,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,4,4,4,4,4,4,4,4,4,4,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,4,4,4,4,4,4,4,4,4,4,4,4,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,4,9,8,6,2,10,11,10,11,10,2,5,9,5,9,5,9,5,9,5,9,5,9,5,5,5,5,9,5,9,5,9,5,9,5,3,7,11,7,11,7,11,12,100};
//*********************************************************
int StepTracker;

const int FrontButton1 = 10;
const int FrontButton2 = 9;
const int FrontButton3 = 8;
const int FrontGreenLight = 13;//These are the large warning Lights
const int FrontYellowLight = 11;
const int FrontRedLight = 12;

//These are global variables that exist only in sub routines
int KegDrainTempPressure;
bool KegDrainWarning = false;
long StepStartTime;

void setup(){
Serial.begin(9600);
pinMode(KegButton1Pin, INPUT_PULLUP);
pinMode(KegButton2Pin, INPUT_PULLUP);
pinMode(KegButton3Pin, INPUT_PULLUP);
//pinMode(KegButton4Pin, INPUT_PULLUP);
LastBlink = millis();
pinMode(Keg1LED, OUTPUT);
pinMode(Keg2LED, OUTPUT);
pinMode(Keg3LED, OUTPUT);
//pinMode(Keg4LED, OUTPUT);
LastBlink = millis();
pinMode(Heater, OUTPUT);
pinMode(Bypass, OUTPUT);
pinMode(Keg1ValveIn, OUTPUT);
pinMode(Keg1ValveOut, OUTPUT);
pinMode(Keg2ValveIn, OUTPUT);
pinMode(Keg2ValveOut, OUTPUT);
pinMode(Keg3ValveIn, OUTPUT);
pinMode(Keg3ValveOut, OUTPUT);
//pinMode(Keg4ValveIn, OUTPUT);
//pinMode(Keg4ValveOut, OUTPUT);
pinMode(AcidOut, OUTPUT);
pinMode(AcidIn, OUTPUT);
pinMode(SanitizerOut, OUTPUT);
pinMode(SanitizerIn, OUTPUT);
pinMode(Water, OUTPUT);
pinMode(Drain, OUTPUT);
pinMode(FrontGreenLight, OUTPUT);
pinMode(FrontYellowLight, OUTPUT);
pinMode(FrontRedLight, OUTPUT);
pinMode(Keg1WarningLight, OUTPUT);
pinMode(Keg2WarningLight, OUTPUT);
pinMode(Keg3WarningLight, OUTPUT);
pinMode(Keg4WarningLight, OUTPUT);

digitalWrite(Heater, LOW);
digitalWrite(Bypass, LOW);
digitalWrite(FrontGreenLight, LOW);
digitalWrite(FrontYellowLight, LOW);
digitalWrite(FrontRedLight, LOW);
digitalWrite(Keg1ValveIn, LOW);
digitalWrite(Keg1ValveOut, LOW);
digitalWrite(Keg2ValveIn, LOW);
digitalWrite(Keg2ValveOut, LOW);
digitalWrite(Keg3ValveIn, LOW);
digitalWrite(Keg3ValveOut, LOW);
//digitalWrite(Keg4ValveIn, LOW);
//digitalWrite(Keg4ValveOut, LOW);
digitalWrite(AcidOut, LOW);
digitalWrite(AcidIn, LOW);
digitalWrite(SanitizerOut, LOW);
digitalWrite(SanitizerIn, LOW);
digitalWrite(Water, LOW);
digitalWrite(Drain, LOW);
digitalWrite(CO2, LOW);
digitalWrite(Pump, LOW);
digitalWrite(Keg1LED, LOW);
digitalWrite(Keg2LED, LOW);
digitalWrite(Keg3LED, LOW);

KegButton1Type = 3;
KegButton2Type = 3;
KegButton3Type = 3;
//KegButton4Type = 3;
}

void loop(){
  CurrentPressure = analogRead(PressureSensor);
  Serial.println((String)"Current Presssure:"+CurrentPressure);
  // Read all of the buttons one by one, if the button has changed debounce and time the press
  //Serial.println((String)"CurrentPressure:" + CurrentPressure+"CurrentKegIn:"+CurrentKegIn +"-CurrentKegOut:"+ CurrentKegOut+"-CurrentKegType:"+CurrentKegType+"-CurrentKegWarning:"+CurrentKegWarning+"-CurrentStep:"+CurrentStep+"-StepLength:"+StepLength+"-StepInitialize:"+StepInitialize+"-StepComplete:"+StepComplete+"-1Running:"+KegButton1Running+"-2Running:"+KegButton2Running+"-3Running:"+KegButton3Running+"-4Running:"+KegButton4Running);
  //Serial.println((String)"Pin:"+KegButton1Pin +"-Type:"+ KegButton1Type+"-Active:"+KegButton1Active+"-StateChangeTime:"+KegButton1StateChangeTime+"-ButtonPress:"+KegButton1Press+"-ButtonState:"+KegButton1State+"-LastPress:"+KegButton1LastPress+"-Debounce:"+KegButton1Debounce+"-SinglePress:"+KegButton1SinglePress+"-Running:"+KegButton1Running);
  //Serial.println((String)"Pin:"+KegButton2Pin +"-Type:"+ KegButton2Type+"-Active:"+KegButton2Active+"-StateChangeTime:"+KegButton2StateChangeTime+"-ButtonPress:"+KegButton2Press+"-ButtonState:"+KegButton2State+"-LastPress:"+KegButton2LastPress+"-Debounce:"+KegButton2Debounce+"-SinglePress:"+KegButton2SinglePress+"-Running:"+KegButton2Running);
  //Serial.println((String)"Pin:"+KegButton3Pin +"-Type:"+ KegButton3Type+"-Active:"+KegButton3Active+"-StateChangeTime:"+KegButton3StateChangeTime+"-ButtonPress:"+KegButton3Press+"-ButtonState:"+KegButton3State+"-LastPress:"+KegButton3LastPress+"-Debounce:"+KegButton3Debounce+"-SinglePress:"+KegButton3SinglePress+"-Running:"+KegButton3Running);
  //Serial.println((String)"Pin:"+KegButton4Pin +"-Type:"+ KegButton4Type+"-Active:"+KegButton4Active+"-StateChangeTime:"+KegButton4StateChangeTime+"-ButtonPress:"+KegButton4Press+"-ButtonState:"+KegButton4State+"-LastPress:"+KegButton4LastPress+"-Debounce:"+KegButton4Debounce+"-SinglePress:"+KegButton4SinglePress+"-Running:"+KegButton4Running);
  KegButton1Press = digitalRead(KegButton1Pin);//Reads the button
  if (KegButton1Press != KegButton1LastPress){//if the button has changed since last loop
  //debounce
    KegButton1LastPress = KegButton1Press;//record button state
    KegButton1Debounce = millis();//start tracking for debounce delay
  }
  if ((millis() - KegButton1Debounce) > DebounceDelay) {//checking if the debounce time has passed since last state change
    if (KegButton1State != KegButton1Press){ //checking if a state change has happened
      KegButton1State = KegButton1Press; //set the new state equal to the button reading
      KegButton1StateChangeTime = millis();//tracking time since last state change

      if (KegButton1State == true){//this is when the button is released
        
      }
      if (KegButton1State == false){// when the button is pressed
        KegButton1Active = true;//tracks if currently tracking a button press
        if (KegButton1SinglePress == true){//This tracks if it has already been pressed
          KegButton1Type = 2;//this is setting it as half, will change to cancel if held down
        }
        else{
          KegButton1Type = 1;//sets temporially as sixtel
          KegButton1SinglePress = true;//tracks a single press and no matter how many times you press more it will just go to half
        }
      }
    }
    else if (KegButton1Active == true){//the button is in the active state but there has been no state change this loop
      if ((millis() - KegButton1StateChangeTime) > 3000){//3 seconds has passed since last state change and button is active
        KegButton1SinglePress = false;
        if (KegButton1State == false){//this is held for 3 seconds
          KegButton1Type = 3;
        }
        if (KegButton1Type == 1){//Sixtel
          KegButton1Active = false;
        }
        if (KegButton1Type == 2){//halfKeg
          KegButton1Active = false;
        }
        if (KegButton1Type == 3){//Cancel
          KegButton1Active = false;
          KegButton1Running = false;
        }
      }
      
    }
  }
  KegButton2Press = digitalRead(KegButton2Pin);//Reads the button
  if (KegButton2Press != KegButton2LastPress){//if the button has changed since last loop
  //debounce
    KegButton2LastPress = KegButton2Press;//record button state
    KegButton2Debounce = millis();//start tracking for debounce delay
  }
  if ((millis() - KegButton2Debounce) > DebounceDelay) {//checking if the debounce time has passed since last state change
    if (KegButton2State != KegButton2Press){ //checking if a state change has happened
      KegButton2State = KegButton2Press; //set the new state equal to the button reading
      KegButton2StateChangeTime = millis();//tracking time since last state change

      if (KegButton2State == true){//this is when the button is released
        
      }
      if (KegButton2State == false){// when the button is pressed
        KegButton2Active = true;//tracks if currently tracking a button press
        if (KegButton2SinglePress == true){//This tracks if it has already been pressed
          KegButton2Type = 2;//this is setting it as half, will change to cancel if held down
        }
        else{
          KegButton2Type = 1;//sets temporially as sixtel
          KegButton2SinglePress = true;//tracks a single press and no matter how many times you press more it will just go to half
        }
      }
    }
    else if (KegButton2Active == true){//the button is in the active state but there has been no state change this loop
      if ((millis() - KegButton2StateChangeTime) > 3000){//3 seconds has passed since last state change and button is active
        KegButton2SinglePress = false;
        if (KegButton2State == false){//this is held for 3 seconds
          KegButton2Type = 3;
        }
        if (KegButton2Type == 1){//Sixtel
          KegButton2Active = false;
        }
        if (KegButton2Type == 2){//halfKeg
          KegButton2Active = false;
        }
        if (KegButton2Type == 3){//Cancel
          KegButton2Active = false;
          KegButton2Running = false;
        }
      }
      
    }
  }
  KegButton3Press = digitalRead(KegButton3Pin);//Reads the button
  if (KegButton3Press != KegButton3LastPress){//if the button has changed since last loop
  //debounce
    KegButton3LastPress = KegButton3Press;//record button state
    KegButton3Debounce = millis();//start tracking for debounce delay
  }
  if ((millis() - KegButton3Debounce) > DebounceDelay) {//checking if the debounce time has passed since last state change
    if (KegButton3State != KegButton3Press){ //checking if a state change has happened
      KegButton3State = KegButton3Press; //set the new state equal to the button reading
      KegButton3StateChangeTime = millis();//tracking time since last state change

      if (KegButton3State == true){//this is when the button is released
        
      }
      if (KegButton3State == false){// when the button is pressed
        KegButton3Active = true;//tracks if currently tracking a button press
        if (KegButton3SinglePress == true){//This tracks if it has already been pressed
          KegButton3Type = 2;//this is setting it as half, will change to cancel if held down
        }
        else{
          KegButton3Type = 1;//sets temporially as sixtel
          KegButton3SinglePress = true;//tracks a single press and no matter how many times you press more it will just go to half
        }
      }
    }
    else if (KegButton3Active == true){//the button is in the active state but there has been no state change this loop
      if ((millis() - KegButton3StateChangeTime) > 3000){//3 seconds has passed since last state change and button is active
        KegButton3SinglePress = false;
        if (KegButton3State == false){//this is held for 3 seconds
          KegButton3Type = 3;
        }
        if (KegButton3Type == 1){//Sixtel
          KegButton3Active = false;
        }
        if (KegButton3Type == 2){//halfKeg
          KegButton3Active = false;
        }
        if (KegButton3Type == 3){//Cancel
          KegButton3Active = false;
          KegButton3Running = false;
        }
      }
      
    }
  }
//  KegButton4Press = digitalRead(KegButton4Pin);//Reads the button
//  if (KegButton4Press != KegButton4LastPress){//if the button has changed since last loop
//  //debounce
//    KegButton4LastPress = KegButton4Press;//record button state
//    KegButton4Debounce = millis();//start tracking for debounce delay
//  }
//  if ((millis() - KegButton4Debounce) > DebounceDelay) {//checking if the debounce time has passed since last state change
//    if (KegButton4State != KegButton4Press){ //checking if a state change has happened
//      KegButton4State = KegButton4Press; //set the new state equal to the button reading
//      KegButton4StateChangeTime = millis();//tracking time since last state change
//
//      if (KegButton4State == true){//this is when the button is released
//        
//      }
//      if (KegButton4State == false){// when the button is pressed
//        KegButton4Active = true;//tracks if currently tracking a button press
//        if (KegButton4SinglePress == true){//This tracks if it has already been pressed
//          KegButton4Type = 2;//this is setting it as half, will change to cancel if held down
//        }
//        else{
//          KegButton4Type = 1;//sets temporially as sixtel
//          KegButton4SinglePress = true;//tracks a single press and no matter how many times you press more it will just go to half
//        }
//      }
//    }
////    else if (KegButton4Active == true){//the button is in the active state but there has been no state change this loop
//      if ((millis() - KegButton4StateChangeTime) > 3000){//3 seconds has passed since last state change and button is active
//        KegButton4SinglePress = false;
//        if (KegButton4State == false){//this is held for 3 seconds
//          KegButton4Type = 3;
//        }
//        if (KegButton4Type == 1){//Sixtel
//          KegButton4Active = false;
//        }
//        if (KegButton4Type == 2){//halfKeg
//          KegButton4Active = false;
//        }
//        if (KegButton4Type == 3){//Cancel
//          KegButton4Active = false;
//        }
//      }
//      
//    }
//  }
  //Blink the lights in unision
  if ((millis() - LastBlink) > 500){//calculate blinks every half second
    LastBlink = millis();
    if (KegButton1Type == 1){
      digitalWrite(Keg1LED, !digitalRead(Keg1LED));
      //LastBlink = millis();
    }
    else if (KegButton1Type == 2){ //Sixtel keep light steady
      digitalWrite(Keg1LED, HIGH);
    }
    else if (KegButton1Type == 3){//Cancel light off
      digitalWrite(Keg1LED, LOW);
    }
    if (KegButton2Type == 1){
      digitalWrite(Keg2LED, !digitalRead(Keg2LED));
    }
    else if (KegButton2Type == 2){ //Sixtel keep light steady
      digitalWrite(Keg2LED, HIGH);
    }
    else if (KegButton2Type == 3){//Cancel light off
      digitalWrite(Keg2LED, LOW);
    }
     if (KegButton3Type == 1){
      digitalWrite(Keg3LED, !digitalRead(Keg3LED));
    }
    else if (KegButton3Type == 2){ //Sixtel keep light steady
      digitalWrite(Keg3LED, HIGH);
    }
    else if (KegButton3Type == 3){//Cancel light off
      digitalWrite(Keg3LED, LOW);
    }
  }
//Find the next program to run
if (KegButton1Running == false && KegButton2Running == false && KegButton3Running == false){//There are no kegs running
    Cancel();
    if (KegButton1Type !=3){
      if (KegButton1Type == 1){//Run Sixtel
        KegButton1Running = true;
        CurrentKegIn = Keg1ValveIn;
        CurrentKegOut = Keg1ValveOut;
        CurrentKeg = "Keg1";
        CurrentKegType = 1;
        CurrentKegWarning = Keg1WarningLight;
        CurrentStep = 0;
        StepInitialize = true;
      }
      else if(KegButton1Type == 2){ //Rung Half
        KegButton1Running = true;
        CurrentKegIn = Keg1ValveIn;
        CurrentKegOut = Keg1ValveOut;
        CurrentKeg = "Keg1";
        CurrentKegType = 2;
        CurrentKegWarning = Keg1WarningLight;
        CurrentStep = 0;
        StepInitialize = true;
      }
      else{
        KegButton1Type = 3;
      }
    }
    else if (KegButton2Type !=3){
      if (KegButton2Type == 1){//Run Sixtel
        KegButton2Running = true;
        CurrentKegIn = Keg2ValveIn;
        CurrentKegOut = Keg2ValveOut;
        CurrentKeg = "Keg2";
        CurrentKegType = 1;
        CurrentKegWarning = Keg2WarningLight;
        CurrentStep = 0;
        StepInitialize = true;
      }
      else if(KegButton2Type == 2){ //Rung Half
        KegButton2Running = true;
        CurrentKegIn = Keg2ValveIn;
        CurrentKegOut = Keg2ValveOut;
        CurrentKeg = "Keg2";
        CurrentKegType = 2;
        CurrentKegWarning = Keg2WarningLight;
        CurrentStep = 0;
        StepInitialize = true;
      }
      else{
        KegButton2Type = 3;
      }
    }
    else if (KegButton3Type !=3){
      if (KegButton3Type == 1){//Run Sixtel
        KegButton3Running = true;
        CurrentKegIn = Keg3ValveIn;
        CurrentKegOut = Keg3ValveOut;
        CurrentKeg = "Keg3";
        CurrentKegType = 1;
        CurrentKegWarning = Keg3WarningLight;
        CurrentStep = 0;
        StepInitialize = true;
      }
      else if(KegButton3Type == 2){ //Rung Half
        KegButton3Running = true;
        CurrentKegIn = Keg3ValveIn;
        CurrentKegOut = Keg3ValveOut;
        CurrentKeg = "Keg3";
        CurrentKegType = 2;
        CurrentKegWarning = Keg3WarningLight;
        CurrentStep = 0;
        StepInitialize = true;
      }
      else{
        KegButton3Type = 3;
      }
    }
//    else if (KegButton4Type !=3){
//      if (KegButton4Type == 1){//Run Sixtel
//        KegButton4Running = true;
//        CurrentKegIn = Keg4ValveIn;
//        CurrentKegOut = Keg4ValveOut;
//        CurrentKeg = "Keg4";
//        CurrentKegType = 1;
//        CurrentKegWarning = Keg4WarningLight;
//        CurrentStep = 0;
//        StepInitialize = true;
//      }
//      else if(KegButton4Type == 2){ //Rung Half
//        KegButton4Running = true;
//        CurrentKegIn = Keg4ValveIn;
//        CurrentKegOut = Keg4ValveOut;
//        CurrentKeg = "Keg4";
//        CurrentKegType = 2;
//        CurrentKegWarning = Keg4WarningLight;
//        CurrentStep = 0;
//        StepInitialize = true;
//      }
//      else{
//        KegButton4Type = 3;
//      }
//    }
}
//Run The program 
  if (StepComplete == true){
      CurrentStep = ++CurrentStep;
      StepComplete = false;
      StepInitialize = true;
    }
  if (CurrentKegType == 1){
  //ArrayofSteps[CurrentStep]
    Serial.println("KegType1");
    StepTracker = SixtelArray[CurrentStep];
    Serial.println((String)"StepTracker:"+StepTracker);
    StepPicker();
  }
  else if(CurrentKegType == 2){
    StepTracker = HalfArray[CurrentStep];
    StepPicker();
  }
}

void StepPicker(){
  if (StepTracker == 0){
    PressureTest();
  }
  if (StepTracker == 1){
    KegDrain();
  }
  else if (StepTracker == 2){
    PurgeDrain();
  }
  else if (StepTracker == 3){
    PurgeSanitizer();
  }
  else if (StepTracker == 4){
    AcidWash();
  }
  else if (StepTracker == 5){
    SanitizerWash();
  }
  else if (StepTracker == 6){
    WaterWash();
  }
  else if (StepTracker == 7){
    CO2Pressureize();
  }
  else if (StepTracker == 8){
    PurgeAcid();
  }
  else if (StepTracker == 9){
    CO2Blast();
  }
  else if (StepTracker == 10){
    ShortWaterWash();
  }
  else if (StepTracker == 11) {
    ShortPurgeDrain();
  }
  else if (StepTracker == 12){
    LongCO2Pressureize();
  }
  else if (StepTracker == 100){
    RunComplete();
  }
}

void Cancel(){
  Serial.println("Cancel");
  digitalWrite(Keg1ValveIn, LOW);
  digitalWrite(Keg1ValveOut, LOW);
  digitalWrite(Keg2ValveIn, LOW);
  digitalWrite(Keg2ValveOut, LOW);
  digitalWrite(Keg3ValveIn, LOW);
  digitalWrite(Keg3ValveOut, LOW);
//  digitalWrite(Keg4ValveIn, LOW);
//  digitalWrite(Keg4ValveOut, LOW);
  digitalWrite(AcidOut, LOW);
  digitalWrite(AcidIn, LOW);
  digitalWrite(SanitizerOut, LOW);
  digitalWrite(SanitizerIn, LOW);
  digitalWrite(Water, LOW);
  digitalWrite(Drain, LOW);
  digitalWrite(Pump, LOW);
  digitalWrite(CO2, LOW);
  digitalWrite(CurrentKegWarning, LOW);
  //Reset to pick new keg
  
  if (CurrentKeg == "Keg1"){
    KegButton1Running = false;
    KegButton1Type = 3;
  }
  else if (CurrentKeg == "Keg2"){
    KegButton2Running = false;
    KegButton2Type = 3;
  }
  else if (CurrentKeg == "Keg3"){
    KegButton3Running = false;
    KegButton3Type = 3;
  }
//  else if (CurrentKeg == "Keg4"){
//    KegButton4Running = false;
//    KegButton4Type = 3;
//  }
  CurrentKeg = "";
  CurrentKegType = 3;
}

//Step Code 0
void PressureTest(){
Serial.println("PressureTest");
Serial.println((String)"CurrentPressure:"+CurrentPressure+"-CurrentKegIn:"+CurrentKegIn+"-StepLength:"+StepLength+"-LowTargetPressure:"+LowTargetPressure+"-CO2TargetPressure:"+CO2TargetPressure);
if (StepInitialize == true){
    StepInitialize = false;
    KegDrainTempPressure = CurrentPressure;
    digitalWrite(Keg1ValveIn, LOW);
    digitalWrite(Keg1ValveOut, LOW);
    digitalWrite(Keg2ValveIn, LOW);
    digitalWrite(Keg2ValveOut, LOW);
    digitalWrite(Keg3ValveIn, LOW);
    digitalWrite(Keg3ValveOut, LOW);
//    digitalWrite(Keg4ValveIn, LOW);
//    digitalWrite(Keg4ValveOut, LOW);
    digitalWrite(AcidOut, LOW);
    digitalWrite(AcidIn, LOW);
    digitalWrite(SanitizerOut, LOW);
    digitalWrite(SanitizerIn, LOW);
    digitalWrite(Water, LOW);
    digitalWrite(Drain, LOW);
    digitalWrite(CO2, LOW);
    digitalWrite(Pump, LOW);
    
    digitalWrite(CurrentKegIn, HIGH);
    digitalWrite(CO2, HIGH);
    if (CurrentKegType == 1){
      StepLength = millis() + 5000;
    }
    else if (CurrentKegType == 2){
      StepLength = millis() + 10000;
    }
  }
  if (CurrentPressure < LowTargetPressure && millis() > StepLength){ //If the current pressure isn't building past the low limit and the steplength has expired
    Serial.println("PurgeTest-Cancel");
    Cancel();
  }
  else{
    if (CurrentPressure >= CO2TargetPressure){
      StepComplete = true;
    }
  }  
}
//Step code 1
void KegDrain(){
  Serial.println("KegDrain");
  if (StepInitialize == true){
    StepInitialize = false;
    KegDrainTempPressure = CurrentPressure;
    digitalWrite(Keg1ValveIn, LOW);
    digitalWrite(Keg1ValveOut, LOW);
    digitalWrite(Keg2ValveIn, LOW);
    digitalWrite(Keg2ValveOut, LOW);
    digitalWrite(Keg3ValveIn, LOW);
    digitalWrite(Keg3ValveOut, LOW);
//    digitalWrite(Keg4ValveIn, LOW);
//    digitalWrite(Keg4ValveOut, LOW);
    digitalWrite(AcidOut, LOW);
    digitalWrite(AcidIn, LOW);
    digitalWrite(SanitizerOut, LOW);
    digitalWrite(SanitizerIn, LOW);
    digitalWrite(Water, LOW);
    digitalWrite(CO2, LOW);
    digitalWrite(Pump, LOW);
    
    digitalWrite(CurrentKegOut, HIGH);
    digitalWrite(Drain, HIGH);
    if (CurrentKegType == 1){
      StepLength = millis() + 15000;
    }
    else if (CurrentKegType == 2){
      StepLength = millis() + 30000;
    }
  }
  if (millis() > StepLength){
    StepComplete = true;
  }
//  if (millis() > StepLength){//has the step length expired?
//    if (CurrentPressure > KegDrainTempPressure - 100){
//      if (KegDrainWarning == false){
//        StepLength = StepLength + 5000;
//        KegDrainWarning = true;
//      }
//      else if(KegDrainWarning == true){
//        Serial.println("KegDrain-Cancel");
//        Cancel();
//      }
//    }
//    
//  }
}

//Step Code 2
void PurgeDrain(){
  Serial.println("PurgeDrain");
  //Need to initialize, test if done, and exit either with a cancel and warning flag or -StepInitialize = true, StepComplete = true
  if (StepInitialize == true){
    StepInitialize = false;
    digitalWrite(Keg1ValveIn, LOW);
    digitalWrite(Keg1ValveOut, LOW);
    digitalWrite(Keg2ValveIn, LOW);
    digitalWrite(Keg2ValveOut, LOW);
    digitalWrite(Keg3ValveIn, LOW);
    digitalWrite(Keg3ValveOut, LOW);
//    digitalWrite(Keg4ValveIn, LOW);
//    digitalWrite(Keg4ValveOut, LOW);
    digitalWrite(AcidOut, LOW);
    digitalWrite(AcidIn, LOW);
    digitalWrite(SanitizerOut, LOW);
    digitalWrite(SanitizerIn, LOW);
    digitalWrite(Water, LOW);
    digitalWrite(Pump, LOW);
    
    digitalWrite(CurrentKegIn, HIGH);
    digitalWrite(CurrentKegOut, HIGH);
    digitalWrite(Drain, HIGH);
    digitalWrite(CO2, HIGH);
    if (CurrentKegType == 1){
      StepLength = millis() + 20000;
    }
    else if (CurrentKegType == 2){
      StepLength = millis() + 40000;
    }  
  }
//  if (CurrentPressure > CO2TargetPressure && millis() > StepLength){//the pressure is higher than the target and the step time is runout
//    if (PurgePressureFlag = false){//turn off the CO2 and wait
//      digitalWrite(CO2, LOW);//turn off CO2
//      StepLength = StepLength + 5000;
//      PurgePressureFlag = true;
//    }
//    else{//already a pressure flag
//      Serial.println("PurgeDrain-Cancel");
//      Cancel();
//    }
//  }
  if (millis() > StepLength){
    StepComplete = true;
  }
}

//Step Code 3
void PurgeSanitizer(){
  Serial.println("PurgeSanitizer");
  if (StepInitialize == true){
    StepStartTime = millis();
    StepInitialize = false;
    TargetPressure = 300;
    digitalWrite(Keg1ValveIn, LOW);
    digitalWrite(Keg1ValveOut, LOW);
    digitalWrite(Keg2ValveIn, LOW);
    digitalWrite(Keg2ValveOut, LOW);
    digitalWrite(Keg3ValveIn, LOW);
    digitalWrite(Keg3ValveOut, LOW);
//    digitalWrite(Keg4ValveIn, LOW);
//    digitalWrite(Keg4ValveOut, LOW);
    digitalWrite(AcidIn, LOW);
    digitalWrite(AcidOut, LOW);
    digitalWrite(SanitizerOut, LOW);
    digitalWrite(Water, LOW);
    digitalWrite(Drain, LOW);
    digitalWrite(Pump, LOW);
    
    digitalWrite(CurrentKegIn, HIGH);
    digitalWrite(CurrentKegOut, HIGH);
    digitalWrite(SanitizerIn, HIGH);
    digitalWrite(CO2, HIGH);
    if (CurrentKegType == 1){
      StepLength = millis() + 15000;
    }
    else if (CurrentKegType == 2){
      StepLength = millis() + 35000;
    }  
  }
//  if (CurrentPressure > TargetPressure && millis() > StepLength){
//    if (PurgePressureFlag = false){
//      digitalWrite(CO2, LOW);
//      StepLength = StepLength + 5000;
//      PurgePressureFlag = true;
//    }
//    else{
//      Serial.println("PurgeSanitizer-Cancel");
//      Cancel();
//    }
//  }
  if (millis() > StepLength){
    StepComplete = true;
  }
}
//StepCode4
void AcidWash(){
  Serial.println("AcidWash");
  if (StepInitialize == true){
    StepInitialize = false;
    StepStartTime = millis();
    digitalWrite(Keg1ValveIn, LOW);
    digitalWrite(Keg1ValveOut, LOW);
    digitalWrite(Keg2ValveIn, LOW);
    digitalWrite(Keg2ValveOut, LOW);
    digitalWrite(Keg3ValveIn, LOW);
    digitalWrite(Keg3ValveOut, LOW);
//    digitalWrite(Keg4ValveIn, LOW);
//    digitalWrite(Keg4ValveOut, LOW);
    //digitalWrite(AcidOut, LOW);
    //digitalWrite(AcidIn, LOW);
    digitalWrite(SanitizerOut, LOW);
    digitalWrite(SanitizerIn, LOW);
    digitalWrite(Water, LOW);
    digitalWrite(Drain, LOW);
    //digitalWrite(Pump, LOW);
    digitalWrite(CO2, LOW);
  
    digitalWrite(CurrentKegIn, HIGH);
    digitalWrite(CurrentKegOut, HIGH);
    digitalWrite(AcidOut, HIGH);
    digitalWrite(AcidIn, HIGH);
    digitalWrite(Pump, HIGH);
    if (CurrentKegType == 1){
      StepLength = millis() + 2000;
    }
    else if (CurrentKegType == 2){
      StepLength = millis() + 2000;
    }  
  }
//  if (CurrentPressure < PumpTargetPressure && millis() > StepStartTime + 5000){
//    Serial.println("AcidWash-Cancel");
//    Cancel();
//  }
  if (millis() > StepLength){
    StepComplete = true;
  }  
}
//StepCode 5
void SanitizerWash(){
Serial.println("SanitizerWash");
  if (StepInitialize == true){
    StepInitialize = false;
    StepStartTime = millis();
    digitalWrite(Keg1ValveIn, LOW);
    digitalWrite(Keg1ValveOut, LOW);
    digitalWrite(Keg2ValveIn, LOW);
    digitalWrite(Keg2ValveOut, LOW);
    digitalWrite(Keg3ValveIn, LOW);
    digitalWrite(Keg3ValveOut, LOW);
//    digitalWrite(Keg4ValveIn, LOW);
//    digitalWrite(Keg4ValveOut, LOW);
    digitalWrite(AcidOut, LOW);
    digitalWrite(AcidIn, LOW);
    //digitalWrite(SanitizerOut, LOW);
    //digitalWrite(SanitizerIn, LOW);
    digitalWrite(Water, LOW);
    digitalWrite(Drain, LOW);
    //digitalWrite(Pump, LOW);
    digitalWrite(CO2, LOW);
  
    digitalWrite(CurrentKegIn, HIGH);
    digitalWrite(CurrentKegOut, HIGH);
    digitalWrite(SanitizerOut, HIGH);
    digitalWrite(SanitizerIn, HIGH);
    digitalWrite(Pump, HIGH);
    if (CurrentKegType == 1){
      StepLength = millis() + 5000;
    }
    else if (CurrentKegType == 2){
      StepLength = millis() + 5000;
    }  
  }
//  if (CurrentPressure < PumpTargetPressure && millis() > StepStartTime + 5000){
//    Serial.println("SanitizerWash-Cancel");
//    Cancel();
//  }
  if (millis() > StepLength){
    StepComplete = true;
  }  
}
//StepCode 6
void WaterWash(){
Serial.println("WaterWash");
Serial.println((String)"StepInitialize:"+StepInitialize);
Serial.println((String)"Time Left:"+(StepLength));
  if (StepInitialize == true){
    StepInitialize = false;
    StepStartTime = millis();
    digitalWrite(Keg1ValveIn, LOW);
    digitalWrite(Keg1ValveOut, LOW);
    digitalWrite(Keg2ValveIn, LOW);
    digitalWrite(Keg2ValveOut, LOW);
    digitalWrite(Keg3ValveIn, LOW);
    digitalWrite(Keg3ValveOut, LOW);
//    digitalWrite(Keg4ValveIn, LOW);
//    digitalWrite(Keg4ValveOut, LOW);
    digitalWrite(AcidOut, LOW);
    digitalWrite(AcidIn, LOW);
    digitalWrite(SanitizerOut, LOW);
    digitalWrite(SanitizerIn, LOW);
    //digitalWrite(Water, LOW);
    //digitalWrite(Drain, LOW);
    //digitalWrite(Pump, LOW);
    digitalWrite(CO2, LOW);
  
    digitalWrite(CurrentKegIn, HIGH);
    digitalWrite(CurrentKegOut, HIGH);
    delay(2000);
    digitalWrite(Water, HIGH);
    digitalWrite(Drain, HIGH);
    delay(2000);
    digitalWrite(Pump, HIGH);
    if (CurrentKegType == 1){
      Serial.println("Changing length");
      StepLength = millis() + 5000;
    }
    else if (CurrentKegType == 2){
      StepLength = millis() + 5000;
    }  
  }
//  if (CurrentPressure < PumpTargetPressure && millis() > StepStartTime + 5000){
//    Serial.println("WaterWash-Cancel");
//    Cancel();
//  }
  if (millis() > StepLength){
    StepComplete = true;
  } 
}
//Step Code 7
void CO2Pressureize(){
Serial.println("CO2Pressureize");
  if (StepInitialize == true){
    StepInitialize = false;
    digitalWrite(Keg1ValveIn, LOW);
    digitalWrite(Keg1ValveOut, LOW);
    digitalWrite(Keg2ValveIn, LOW);
    digitalWrite(Keg2ValveOut, LOW);
    digitalWrite(Keg3ValveIn, LOW);
    digitalWrite(Keg3ValveOut, LOW);
//    digitalWrite(Keg4ValveIn, LOW);
//    digitalWrite(Keg4ValveOut, LOW);
    digitalWrite(AcidOut, LOW);
    digitalWrite(AcidIn, LOW);
    digitalWrite(SanitizerOut, LOW);
    digitalWrite(SanitizerIn, LOW);
    digitalWrite(Water, LOW);
    digitalWrite(Drain, LOW);
    digitalWrite(Pump, LOW);
    //digitalWrite(CO2, LOW);
  
    digitalWrite(CurrentKegIn, HIGH);
    //digitalWrite(CurrentKegOut, HIGH);
    digitalWrite(CO2, HIGH);
    if (CurrentKegType == 1){
      StepLength = millis() + 11000;
    }
    else if (CurrentKegType == 2){
      StepLength = millis() + 30000;
    }  
  }
//  if (CurrentPressure < CO2TargetPressure && millis() > StepStartTime + 5000){
//    Serial.println("CO2Pressureize-Cancel");
//    Cancel();
//  }
  if (millis() > StepLength){
    StepComplete = true;
  } 
}
//StepCode 8
void PurgeAcid(){
  Serial.println("PurgeAcid");
  if (StepInitialize == true){
    StepStartTime = millis();
    StepInitialize = false;
    TargetPressure = 300;
    digitalWrite(Keg1ValveIn, LOW);
    digitalWrite(Keg1ValveOut, LOW);
    digitalWrite(Keg2ValveIn, LOW);
    digitalWrite(Keg2ValveOut, LOW);
    digitalWrite(Keg3ValveIn, LOW);
    digitalWrite(Keg3ValveOut, LOW);
    //digitalWrite(Keg4ValveIn, LOW);
    //digitalWrite(Keg4ValveOut, LOW);
    //digitalWrite(AcidIn, LOW);
    //digitalWrite(AcidOut, LOW);
    digitalWrite(SanitizerIn, LOW);
    digitalWrite(SanitizerOut, LOW);
    digitalWrite(Water, LOW);
    digitalWrite(Drain, LOW);
    digitalWrite(Pump, LOW);
    
    digitalWrite(CurrentKegIn, HIGH);
    digitalWrite(CurrentKegOut, HIGH);
    digitalWrite(AcidOut, LOW);
    digitalWrite(AcidIn, HIGH);
    digitalWrite(CO2, HIGH);
    if (CurrentKegType == 1){
      StepLength = millis() + 15000;
    }
    else if (CurrentKegType == 2){
      StepLength = millis() + 30000;
    }  
  }
//  if (CurrentPressure > TargetPressure && millis() > StepLength){
//    if (PurgePressureFlag = false){
//      digitalWrite(CO2, LOW);
//      StepLength = StepLength + 5000;
//      PurgePressureFlag = true;
//    }
//    else{
//      Serial.println("PurgeAcid-Cancel");
//      Cancel();
//    }
//  }
  if (millis() > StepLength){
    StepComplete = true;
  }
}
//step Code 9
void CO2Blast(){
  Serial.println("PurgeDrain");
  //Need to initialize, test if done, and exit either with a cancel and warning flag or -StepInitialize = true, StepComplete = true
  if (StepInitialize == true){
    StepInitialize = false;
//    digitalWrite(Keg1ValveIn, LOW);
//    digitalWrite(Keg1ValveOut, LOW);
//    digitalWrite(Keg2ValveIn, LOW);
//    digitalWrite(Keg2ValveOut, LOW);
//    digitalWrite(Keg3ValveIn, LOW);
//    digitalWrite(Keg3ValveOut, LOW);
//    digitalWrite(Keg4ValveIn, LOW);
//    digitalWrite(Keg4ValveOut, LOW);
//    digitalWrite(AcidOut, LOW);
//    digitalWrite(AcidIn, LOW);
//    digitalWrite(SanitizerOut, LOW);
//    digitalWrite(SanitizerIn, LOW);
//    digitalWrite(Water, LOW);
//    digitalWrite(Pump, LOW);
    
//    digitalWrite(CurrentKegIn, HIGH);
//    digitalWrite(CurrentKegOut, HIGH);
//    digitalWrite(Drain, HIGH);
    digitalWrite(CO2, HIGH);
    if (CurrentKegType == 1){
      StepLength = millis() + 500;
    }
    else if (CurrentKegType == 2){
      StepLength = millis() + 500;
    }  
  }
//  if (CurrentPressure > CO2TargetPressure && millis() > StepLength){//the pressure is higher than the target and the step time is runout
//    if (PurgePressureFlag = false){//turn off the CO2 and wait
//      digitalWrite(CO2, LOW);//turn off CO2
//      StepLength = StepLength + 5000;
//      PurgePressureFlag = true;
//    }
//    else{//already a pressure flag
//      Serial.println("PurgeDrain-Cancel");
//      Cancel();
//    }
//  }
  if (millis() > StepLength){
    StepComplete = true;
  }
}
//StepCode 10
void ShortWaterWash(){
Serial.println("WaterWash");
Serial.println((String)"StepInitialize:"+StepInitialize);
Serial.println((String)"Time Left:"+(StepLength));
  if (StepInitialize == true){
    StepInitialize = false;
    StepStartTime = millis();
    digitalWrite(Keg1ValveIn, LOW);
    digitalWrite(Keg1ValveOut, LOW);
    digitalWrite(Keg2ValveIn, LOW);
    digitalWrite(Keg2ValveOut, LOW);
    digitalWrite(Keg3ValveIn, LOW);
    digitalWrite(Keg3ValveOut, LOW);
//    digitalWrite(Keg4ValveIn, LOW);
//    digitalWrite(Keg4ValveOut, LOW);
    digitalWrite(AcidOut, LOW);
    digitalWrite(AcidIn, LOW);
    digitalWrite(SanitizerOut, LOW);
    digitalWrite(SanitizerIn, LOW);
    //digitalWrite(Water, LOW);
    //digitalWrite(Drain, LOW);
    //digitalWrite(Pump, LOW);
    digitalWrite(CO2, LOW);
  
    digitalWrite(CurrentKegIn, HIGH);
    digitalWrite(CurrentKegOut, HIGH);
    digitalWrite(Water, HIGH);
    delay(2000);
    digitalWrite(Drain, HIGH);
    digitalWrite(Pump, HIGH);
    if (CurrentKegType == 1){
      Serial.println("Changing length");
      StepLength = millis() + 2000;
    }
    else if (CurrentKegType == 2){
      StepLength = millis() + 1500;
    }  
  }
//  if (CurrentPressure < PumpTargetPressure && millis() > StepStartTime + 5000){
//    Serial.println("WaterWash-Cancel");
//    Cancel();
//  }
  if (millis() > StepLength){
    StepComplete = true;
  } 
}

//Step Code 11
void ShortPurgeDrain(){
  Serial.println("KegDrain");
  if (StepInitialize == true){
    StepInitialize = false;
    KegDrainTempPressure = CurrentPressure;
    digitalWrite(Keg1ValveIn, LOW);
    digitalWrite(Keg1ValveOut, LOW);
    digitalWrite(Keg2ValveIn, LOW);
    digitalWrite(Keg2ValveOut, LOW);
    digitalWrite(Keg3ValveIn, LOW);
    digitalWrite(Keg3ValveOut, LOW);
//    digitalWrite(Keg4ValveIn, LOW);
//    digitalWrite(Keg4ValveOut, LOW);
    digitalWrite(AcidOut, LOW);
    digitalWrite(AcidIn, LOW);
    digitalWrite(SanitizerOut, LOW);
    digitalWrite(SanitizerIn, LOW);
    //digitalWrite(Water, LOW);
    //digitalWrite(CO2, LOW);
    digitalWrite(Pump, LOW);
    
    digitalWrite(CurrentKegIn, HIGH);
    digitalWrite(CurrentKegOut, HIGH);
    digitalWrite(Drain, HIGH);
    //delay(1000);
    digitalWrite(CO2, HIGH);
    if (CurrentKegType == 1){
      StepLength = millis() + 6000;
    }
    else if (CurrentKegType == 2){
      StepLength = millis() + 10000;
    }
  }
  if (millis() > StepLength){
    StepComplete = true;
  }
//  if (millis() > StepLength){//has the step length expired?
//    if (CurrentPressure > KegDrainTempPressure - 100){
//      if (KegDrainWarning == false){
//        StepLength = StepLength + 5000;
//        KegDrainWarning = true;
//      }
//      else if(KegDrainWarning == true){
//        Serial.println("KegDrain-Cancel");
//        Cancel();
//      }
//    }
//    
//  }
}

//Step Code 12
void LongCO2Pressureize(){
Serial.println("CO2Pressureize");
  if (StepInitialize == true){
    StepInitialize = false;
    digitalWrite(Keg1ValveIn, LOW);
    digitalWrite(Keg1ValveOut, LOW);
    digitalWrite(Keg2ValveIn, LOW);
    digitalWrite(Keg2ValveOut, LOW);
    digitalWrite(Keg3ValveIn, LOW);
    digitalWrite(Keg3ValveOut, LOW);
//    digitalWrite(Keg4ValveIn, LOW);
//    digitalWrite(Keg4ValveOut, LOW);
    digitalWrite(AcidOut, LOW);
    digitalWrite(AcidIn, LOW);
    digitalWrite(SanitizerOut, LOW);
    digitalWrite(SanitizerIn, LOW);
    digitalWrite(Water, LOW);
    digitalWrite(Drain, LOW);
    digitalWrite(Pump, LOW);
    //digitalWrite(CO2, LOW);
  
    digitalWrite(CurrentKegIn, HIGH);
    //digitalWrite(CurrentKegOut, HIGH);
    digitalWrite(CO2, HIGH);
    if (CurrentKegType == 1){
      StepLength = millis() + 13000;
    }
    else if (CurrentKegType == 2){
      StepLength = millis() + 30000;
    }  
  }
//  if (CurrentPressure < CO2TargetPressure && millis() > StepStartTime + 5000){
//    Serial.println("CO2Pressureize-Cancel");
//    Cancel();
//  }
  if (millis() > StepLength){
    StepComplete = true;
  } 
}


//Step code 100
void RunComplete(){//a lot like cancel but no warning light
Serial.println("RunComplete");
  digitalWrite(Keg1ValveIn, LOW);
  digitalWrite(Keg1ValveOut, LOW);
  digitalWrite(Keg2ValveIn, LOW);
  digitalWrite(Keg2ValveOut, LOW);
  digitalWrite(Keg3ValveIn, LOW);
  digitalWrite(Keg3ValveOut, LOW);
  //digitalWrite(Keg4ValveIn, LOW);
  //digitalWrite(Keg4ValveOut, LOW);
  digitalWrite(AcidOut, LOW);
  digitalWrite(AcidIn, LOW);
  digitalWrite(SanitizerOut, LOW);
  digitalWrite(SanitizerIn, LOW);
  digitalWrite(Water, LOW);
  digitalWrite(Drain, LOW);
  digitalWrite(Pump, LOW);
  digitalWrite(CO2, LOW);
  //Reset to pick new keg
  
  if (CurrentKeg == "Keg1"){
    KegButton1Running = false;
    KegButton1Type = 3;
  }
  else if (CurrentKeg == "Keg2"){
    KegButton2Running = false;
    KegButton2Type = 3;
  }
  else if (CurrentKeg == "Keg3"){
    KegButton3Running = false;
    KegButton3Type = 3;
  }
//  else if (CurrentKeg == "Keg4"){
//    KegButton4Running = false;
//    KegButton4Type = 3;
//  }
  CurrentKeg = "";
  CurrentKegType = 3; 
}

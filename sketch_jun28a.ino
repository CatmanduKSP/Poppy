// Pin and variable assignments
const int BUT1 = 7;
const int BUT2 = 8;
const int BUT3 = 9;
const int BUT4 = 10;

const int LED1 = 6;
const int LED2 = 5;
const int LED3 = 4;
const int LED4 = 3;

const int RotaryCLK = 0;
const int RotaryDIR = 1;
const int RotaryPRESS = 2;

int lastStateCLK = 0; //Rotary encoder starting nmber

int masterNumber = 0; //Master value starting number

String sequence = ""; //secret code checker...
const String secretCode = "33441212567"; //Special game mode code
bool specialMode = false;

// For rotary press secret code append
bool rotaryPrevPressState = HIGH;

// --- LED show state tracking ---
unsigned long sequenceStartTime = 0; // when the LED show begins
unsigned long lastStepTime = 0;      // last step change time
int sequenceIndex = 0;               // index of current step
bool showRunning = false;            // whether special show is active

// --- STANDBY MODE VARIABLES ---
unsigned long lastInputTime = 0;  // When last input happened
const unsigned long standbyTimeout = 10000;  // 10 seconds timeout

// Independent random blink timing
unsigned long ledNextOn[4] = {0,0,0,0};
unsigned long ledNextOff[4] = {0,0,0,0};
bool ledIsOn[4] = {false,false,false,false};
const unsigned long ledMaxDelay[4] = {1000, 2000, 3000, 4000};
const int ledPins[4] = {6,5,4,3};


/////REPEAT LOGIC VARIABLES\\\\\

bool but1PrevState = HIGH;
unsigned long but1LastChangeTime = 0;
unsigned long but1LastRepeatTime = 0;

bool but2PrevState = HIGH;
unsigned long but2LastChangeTime = 0;
unsigned long but2LastRepeatTime = 0;

bool but3PrevState = HIGH;
unsigned long but3LastChangeTime = 0;
unsigned long but3LastRepeatTime = 0;

bool but4PrevState = HIGH;
unsigned long but4LastChangeTime = 0;
unsigned long but4LastRepeatTime = 0;


const unsigned long initialDelay = 300;
const unsigned long repeatInterval = 100;





void setup() {
  pinMode(BUT1, INPUT_PULLUP);
  pinMode(BUT2, INPUT_PULLUP);
  pinMode(BUT3, INPUT_PULLUP);
  pinMode(BUT4, INPUT_PULLUP);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  pinMode(RotaryCLK, INPUT_PULLUP);
  pinMode(RotaryDIR, INPUT_PULLUP);
  pinMode(RotaryPRESS, INPUT_PULLUP);

  // For rotary press secret code append
  rotaryPrevPressState = digitalRead(RotaryPRESS);

  // For standby mode timer
  lastInputTime = millis();

  playLightShow(); //Boot animation
}

void loop() {
  unsigned long currentTime = millis();


  int currentStateCLK = digitalRead(RotaryCLK);



// Increases or decreases master number with rotary encoder
  if (currentStateCLK != lastStateCLK && currentStateCLK == HIGH){
    if (digitalRead(RotaryDIR) != currentStateCLK) {
      masterNumber++;
      sequence += "5";
      lastInputTime = currentTime;
    } else {
      masterNumber--;
      sequence += "6";
      lastInputTime = currentTime;
    }
  }
  
  lastStateCLK = currentStateCLK;


// Switches increase master number by 2^(switch number), codes in key repeat with initial delay
// Adds repeat with initial delay logic



  int but1State = digitalRead(BUT1);

  if (but1PrevState == HIGH && but1State == LOW) {
    masterNumber ++;
    sequence += "1";
    lastInputTime = currentTime;
    but1LastChangeTime = currentTime;
    but1LastRepeatTime = currentTime;
  }

  if (but1State == LOW) {
    if ((currentTime - but1LastChangeTime > initialDelay) && (currentTime - but1LastRepeatTime > repeatInterval)){
      masterNumber ++;
      but1LastRepeatTime = currentTime;
      lastInputTime = currentTime;
    }
  }

  but1PrevState = but1State;


  int but2State = digitalRead(BUT2);

  if (but2PrevState == HIGH && but2State == LOW) {
    masterNumber += 2;
    sequence += "2";
    lastInputTime = currentTime;
    but2LastChangeTime = currentTime;
    but2LastRepeatTime = currentTime;
  }

  if (but2State == LOW) {
    if ((currentTime - but2LastChangeTime > initialDelay) && (currentTime - but2LastRepeatTime > repeatInterval)){
      masterNumber += 2;
      but2LastRepeatTime = currentTime;
      lastInputTime = currentTime;
    }
  }
  
  but2PrevState = but2State;


  int but3State = digitalRead(BUT3);

  if (but3PrevState == HIGH && but3State == LOW) {
    masterNumber += 4;
    sequence += "3";
    lastInputTime = currentTime;
    but3LastChangeTime = currentTime;
    but3LastRepeatTime = currentTime;
  }

  if (but3State == LOW) {
    if ((currentTime - but3LastChangeTime > initialDelay) && (currentTime - but3LastRepeatTime > repeatInterval)){
      masterNumber += 4;
      but3LastRepeatTime = currentTime;
      lastInputTime = currentTime;
    }
  }
  
  but3PrevState = but3State;


  int but4State = digitalRead(BUT4);

  if (but4PrevState == HIGH && but4State == LOW) {
    masterNumber += 8;
    sequence += "4";
    lastInputTime = currentTime;
    but4LastChangeTime = currentTime;
    but4LastRepeatTime = currentTime;
  }

  if (but4State == LOW) {
    if ((currentTime - but4LastChangeTime > initialDelay) && (currentTime - but4LastRepeatTime > repeatInterval)){
      masterNumber += 8;
      but4LastRepeatTime = currentTime;
      lastInputTime = currentTime;
    }
  }
  
  but4PrevState = but4State;


// rotary encoder click resets master number
  if (digitalRead(RotaryPRESS) == LOW && rotaryPrevPressState == HIGH) {
    masterNumber = 0;
    sequence += "7";
    lastInputTime = currentTime;
  }


  updateLEDs();

// Cut sequence to length
if (sequence.length() > 11) {
  sequence = sequence.substring(sequence.length() - 11);
  }


  // Secret code checker
  if (sequence.endsWith(secretCode)) {
    secretGameMode();
  }


  //Inactivity Check
  if (currentTime - lastInputTime > standbyTimeout) {
  standbyLoop();
  }

  
  // Update rotary press previous state for next loop
  rotaryPrevPressState = digitalRead(RotaryPRESS);


  // Keep master value 4-bit
  masterNumber = masterNumber & 0x0F;
}


// LEDs display master number in binary, wrapping every 15 numbers
void updateLEDs() {
  digitalWrite(LED1, (masterNumber >> 0) & 1);
  digitalWrite(LED2, (masterNumber >> 1) & 1);
  digitalWrite(LED3, (masterNumber >> 2) & 1);
  digitalWrite(LED4, (masterNumber >> 3) & 1);
}


//Secret game mode!
void secretGameMode() {
  playLightShow();

}


// Boot and gamemode switch light show
void playLightShow() {

  //turns off LEDs
  auto allOff = []() {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
  };

  auto step = [&](bool l1, bool l2, bool l3, bool l4, int extraDelay = 0) {
    digitalWrite(LED1, l1);
    digitalWrite(LED2, l2);
    digitalWrite(LED3, l3);
    digitalWrite(LED4, l4);
    delay(150 + extraDelay);
  };


/////LIGHT SHOW SEQUENCE\\\\\
  
  allOff();

  step(1,0,0,0);
  step(0,1,0,0);
  step(0,0,1,0);
  step(0,0,0,1);
  step(0,0,1,0);
  step(1,0,0,0);
  step(0,1,0,0);
  step(0,0,1,0);
  step(0,0,0,1);
  step(0,0,1,0);
  step(1,0,0,0);
  step(0,1,0,0);
  step(1,0,0,0);
  step(0,0,1,0);
  step(1,0,1,0);
  step(0,1,1,0);
  step(1,1,1,0);
  step(0,0,0,1);
  step(1,0,0,1);
  step(0,1,0,1);
  step(1,1,0,1);
  step(0,0,1,1);
  step(1,0,1,1);
  step(0,1,1,1);
  step(1,1,1,1);
  step(0,1,1,1);
  step(1,0,1,1);
  step(0,0,1,1);
  step(1,1,0,1);
  step(0,1,0,1);
  step(1,0,0,1);
  step(0,0,0,1);
  step(1,1,1,0);
  step(0,1,1,0);
  step(1,0,1,0);
  step(0,0,1,0);
  step(1,0,0,0);
  step(0,1,0,0);
  step(1,1,1,1, 500);
  step(1,1,1,1, 100);

  allOff();
}



/////STANDBY SEQUENCE\\\\\

void standbyLoop() {
  while (true) {  // infinite loop, exits only on input
    unsigned long currentTime = millis();

    // check for any new input
    if (digitalRead(BUT1) == LOW || digitalRead(BUT2) == LOW ||
        digitalRead(BUT3) == LOW || digitalRead(BUT4) == LOW ||
        digitalRead(RotaryCLK) == LOW || digitalRead(RotaryDIR) == LOW ||
        digitalRead(RotaryPRESS) == LOW) {
      lastInputTime = currentTime;  // reset inactivity timer
      return;  // exit standby and return to main loop
    }

    // LED blinking logic
    for (int i = 0; i < 4; i++) {
      if (!ledIsOn[i] && currentTime >= ledNextOn[i]) {
        digitalWrite(ledPins[i], HIGH);
        ledIsOn[i] = true;
        ledNextOff[i] = currentTime + random(0, ledMaxDelay[i]);
      }

      if (ledIsOn[i] && currentTime >= ledNextOff[i]) {
        digitalWrite(ledPins[i], LOW);
        ledIsOn[i] = false;
        ledNextOn[i] = currentTime + random(0, ledMaxDelay[i]);
      }
    }
  }
}
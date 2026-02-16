#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>



// SH1106 display
Adafruit_SH1106G display(128, 64, &Wire);

#define WAKE_PIN 2
#define CLK 12
#define DT 8
#define SW 9

int lastCLK;
int jokeIndex = 0;
bool showingPunchline = false;
bool isOn = true; 
bool eyesOpen = true;
unsigned long lastBlink = 0;
const unsigned long blinkInterval = 400; // ms between blinks
int eyeX1 = 40, eyeX2 = 88, eyeY = 20, eyeRadius = 20;


// --------------------------------------------
// JOKES IN PROGMEM
// --------------------------------------------


const char jokeQ32[] PROGMEM = "Why don't eggs tell\njokes?";
const char jokeQ33[] PROGMEM = "What do you call cheese\nthat isn't yours?";
const char jokeQ34[] PROGMEM = "Why did the scarecrow\nwin an award?";
const char jokeQ35[] PROGMEM = "Why did the golfer\nbring two pants?";
const char jokeQ36[] PROGMEM = "What do you call a\nsleeping bull?";
const char jokeQ37[] PROGMEM = "Why don't bananas get\nlonely?";
const char jokeQ38[] PROGMEM = "What kind of tree fits\nin your hand?";
const char jokeQ39[] PROGMEM = "Why was the broom late?";
const char jokeQ40[] PROGMEM = "Why did the frog take\nthe bus?";
const char jokeQ41[] PROGMEM = "What did the ocean say\nto the beach?";
const char jokeQ42[] PROGMEM = "Why did the stadium get\nhot?";
const char jokeQ43[] PROGMEM = "Why don’t oysters share\npearls?";
const char jokeQ44[] PROGMEM = "Why did the banana go\nto the doctor?";
const char jokeQ45[] PROGMEM = "Why can't pirates learn\nthe alphabet?";
const char jokeQ46[] PROGMEM = "Why did the chicken join\na band?";
const char jokeQ47[] PROGMEM = "What do you call a pile\nof cats?";
const char jokeQ48[] PROGMEM = "Why was the belt\narrested?";
const char jokeQ49[] PROGMEM = "Why was the phone\nwearing glasses?";
const char jokeQ50[] PROGMEM = "Why did the cookie go to\nthe doctor?";
const char jokeQ51[] PROGMEM = "What do you call a lazy\nkangaroo?";
const char jokeQ52[] PROGMEM = "Why don't seagulls fly\nover bays?";
const char jokeQ53[] PROGMEM = "What do you call a fish\nwith no eyes?";
const char jokeQ54[] PROGMEM = "Why are elevator jokes\nso good?";
const char jokeQ55[] PROGMEM = "Why was the music room\nso loud?";
const char jokeQ56[] PROGMEM = "Why did the snowman\npick a fight?";
const char jokeQ57[] PROGMEM = "What do you call a dog\nthat can do magic?";
const char jokeQ58[] PROGMEM = "Why did the grape stop\nin the middle of the road?";
const char jokeQ59[] PROGMEM = "Why did the light bulb\nget good grades?";
const char jokeQ60[] PROGMEM = "What do you call a cow\nwith no legs?";
const char jokeQ61[] PROGMEM = "Why did the coffee file\na police report?";
const char jokeQ62[] PROGMEM = "Why did the barber win\nthe race?";
const char jokeQ63[] PROGMEM = "What do you call a\nnervous javelin thrower?";
const char jokeQ64[] PROGMEM = "Why was the math class\nso cold?";
const char jokeQ65[] PROGMEM = "Why did the orange lose\nthe race?";
const char jokeQ66[] PROGMEM = "Why did the keyboard go\nto sleep?";
const char jokeQ67[] PROGMEM = "Why can’t you play hide\nand seek with mountains?";
const char jokeQ68[] PROGMEM = "What do you call a\nlaughing motorcycle?";
const char jokeQ69[] PROGMEM = "Why did the duck get a\nnew phone?";
const char jokeQ70[] PROGMEM = "Why did the shoe go to\nschool?";
const char jokeQ71[] PROGMEM = "What do you call birds\nthat stick together?";
const char jokeQ72[] PROGMEM = "Why did the potato sit\ndown early?";
const char jokeQ73[] PROGMEM = "Why did the clock get\nkicked out of class?";
const char jokeQ74[] PROGMEM = "What do you call a\nmusical whale?";
const char jokeQ75[] PROGMEM = "Why did the shovel apply\nfor a job?";
const char jokeQ76[] PROGMEM = "Why don’t robots panic?";
const char jokeQ77[] PROGMEM = "Why was the candle\nalways calm?";
const char jokeQ78[] PROGMEM = "Why did the cupcake go\nboxing?";
const char jokeQ79[] PROGMEM = "What do you call a very\nfast sheep?";
const char jokeQ80[] PROGMEM = "Why did the mushroom get\ninvited to the party?";
const char jokeQ81[] PROGMEM = "Why did the painter bring\na ladder?";
const char jokeQ82[] PROGMEM = "Why was the fish bad at\nbasketball?";
const char jokeQ83[] PROGMEM = "Why did the bee get\nmarried?";
const char jokeQ84[] PROGMEM = "What do you call a\nsinging computer?";



const char jokeA32[] PROGMEM = "They'd crack each other up!";
const char jokeA33[] PROGMEM = "Nacho cheese!";
const char jokeA34[] PROGMEM = "He was outstanding in his field!";
const char jokeA35[] PROGMEM = "In case he got a hole in one!";
const char jokeA36[] PROGMEM = "A bulldozer!";
const char jokeA37[] PROGMEM = "They hang out in bunches!";
const char jokeA38[] PROGMEM = "A palm tree!";
const char jokeA39[] PROGMEM = "It swept in late!";
const char jokeA40[] PROGMEM = "His car got toad!";
const char jokeA41[] PROGMEM = "Nothing, it just waved!";
const char jokeA42[] PROGMEM = "Because all the fans left!";
const char jokeA43[] PROGMEM = "They're shellfish!";
const char jokeA44[] PROGMEM = "It wasn't peeling well!";
const char jokeA45[] PROGMEM = "They get lost at C!";
const char jokeA46[] PROGMEM = "It had drumsticks!";
const char jokeA47[] PROGMEM = "A meow-ntain!";
const char jokeA48[] PROGMEM = "It held up a pair of pants!";
const char jokeA49[] PROGMEM = "It lost its contacts!";
const char jokeA50[] PROGMEM = "It felt crumby!";
const char jokeA51[] PROGMEM = "A pouch potato!";
const char jokeA52[] PROGMEM = "Because then they'd be bagels!";
const char jokeA53[] PROGMEM = "A fshhh!";
const char jokeA54[] PROGMEM = "They work on many levels!";
const char jokeA55[] PROGMEM = "There were too many\nnotes!";
const char jokeA56[] PROGMEM = "He wanted to melt the\ncompetition!";
const char jokeA57[] PROGMEM = "A labracadabrador!";
const char jokeA58[] PROGMEM = "It ran out of juice!";
const char jokeA59[] PROGMEM = "It was really bright!";
const char jokeA60[] PROGMEM = "Ground beef!";
const char jokeA61[] PROGMEM = "It got mugged!";
const char jokeA62[] PROGMEM = "He knew all the shortcuts!";
const char jokeA63[] PROGMEM = "Shaky spear!";
const char jokeA64[] PROGMEM = "Too many\ncold calculations!";
const char jokeA65[] PROGMEM = "It ran out of juice!";
const char jokeA66[] PROGMEM = "It lost its space bar!";
const char jokeA67[] PROGMEM = "They always peak!";
const char jokeA68[] PROGMEM = "A Yamahaha!";
const char jokeA69[] PROGMEM = "Because it kept\nquacking!";
const char jokeA70[] PROGMEM = "It wanted to be a\nsmart shoe!";
const char jokeA71[] PROGMEM = "Vel-crows!";
const char jokeA72[] PROGMEM = "It was already a little\nfried!";
const char jokeA73[] PROGMEM = "It kept talking\ntoo much!";
const char jokeA74[] PROGMEM = "An orca-stra!";
const char jokeA75[] PROGMEM = "It was digging the job!";
const char jokeA76[] PROGMEM = "Because they have\nsteel nerves!";
const char jokeA77[] PROGMEM = "It never got\nwick-ed up!";
const char jokeA78[] PROGMEM = "It wanted a better\npunchline!";
const char jokeA79[] PROGMEM = "A lamborghini!";
const char jokeA80[] PROGMEM = "He was a fungi!";
const char jokeA81[] PROGMEM = "To reach the high notes!";
const char jokeA82[] PROGMEM = "It kept getting tangled\nin the net!";
const char jokeA83[] PROGMEM = "Because he found his\nhoney!";
const char jokeA84[] PROGMEM = "A-Dell!";



const char* const questions[] PROGMEM = {
  jokeQ32, jokeQ33, jokeQ34, jokeQ35, jokeQ36, jokeQ37, jokeQ38,
  jokeQ39, jokeQ40, jokeQ41, jokeQ42, jokeQ43, jokeQ44, jokeQ45,
  jokeQ46, jokeQ47, jokeQ48, jokeQ49, jokeQ50, jokeQ51, jokeQ52,
  jokeQ53, jokeQ54, jokeQ55, jokeQ56, jokeQ57, jokeQ58, jokeQ59,
  jokeQ60, jokeQ61, jokeQ62, jokeQ63, jokeQ64, jokeQ65, jokeQ66,
  jokeQ67, jokeQ68, jokeQ69, jokeQ70, jokeQ71, jokeQ72, jokeQ73,
  jokeQ74, jokeQ75, jokeQ76, jokeQ77, jokeQ78, jokeQ79, jokeQ80,
  jokeQ81, jokeQ82, jokeQ83, jokeQ84

};

const char* const answers[] PROGMEM = {
  jokeA32, jokeA33, jokeA34, jokeA35, jokeA36, jokeA37, jokeA38,
  jokeA39, jokeA40, jokeA41, jokeA42, jokeA43, jokeA44, jokeA45,
  jokeA46, jokeA47, jokeA48, jokeA49, jokeA50, jokeA51, jokeA52,
  jokeA53, jokeA54, jokeA55, jokeA56, jokeA57, jokeA58, jokeA59,
  jokeA60, jokeA61, jokeA62, jokeA63, jokeA64, jokeA65, jokeA66,
  jokeA67, jokeA68, jokeA69, jokeA70, jokeA71, jokeA72, jokeA73,
  jokeA74, jokeA75, jokeA76, jokeA77, jokeA78, jokeA79, jokeA80,
  jokeA81, jokeA82, jokeA83, jokeA84

};

int jokeCount = sizeof(questions) / sizeof(questions[0]);
char buffer[64];

void typeWriterText(int x, int y, const char* text, int speed) {
  display.setCursor(x, y);

  for (int i = 0; text[i] != '\0'; i++) {
    display.write(text[i]);
    display.display();
    delay(speed);
  }
}


// Draw eyes: open if true, closed if false


void drawEyes() {
  display.clearDisplay();
  if (eyesOpen) {
    display.fillCircle(eyeX1, eyeY, eyeRadius, SH110X_WHITE);
    display.fillCircle(eyeX2, eyeY, eyeRadius, SH110X_WHITE);
  } else {
    display.fillRect(eyeX1 - eyeRadius, eyeY - 1, eyeRadius * 2, 2, SH110X_WHITE);
    display.fillRect(eyeX2 - eyeRadius, eyeY - 1, eyeRadius * 2, 2, SH110X_WHITE);
  }
  display.display();
}




volatile bool wakeUpFlag = false;  // wake-up flag

// interrupt function called when D2 is pressed
volatile unsigned long lastWake = 0;

void wakeUp() {
  unsigned long now = millis();
  if (now - lastWake > 200) {   // simple debounce 200ms
    isOn = !isOn;
    lastWake = now;
  }
}


// function to put MCU to deep sleep
void goToSleep() {
  attachInterrupt(digitalPinToInterrupt(WAKE_PIN), wakeUp, LOW); // trigger on D2
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_cpu();  // MCU sleeps here
  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(WAKE_PIN));
}


// --------------------------------------------
// DISPLAY FUNCTIONS
// --------------------------------------------
void showQuestion() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("[JOKEANATOR 3000] :\n");
  display.display();  
  
  strcpy_P(buffer, (char*)pgm_read_word(&questions[jokeIndex]));
  
  typeWriterText(0, 20, buffer, 35);  
}


void showAnswer() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("ANSWER:\n");
  display.display();

  strcpy_P(buffer, (char*)pgm_read_word(&answers[jokeIndex]));

  typeWriterText(0, 20, buffer, 35);  
}

// --------------------------------------------
// SETUP
// --------------------------------------------
void showOpeningScreen() {
  display.clearDisplay();

  // Big header
  display.setTextSize(1);
  typeWriterText(0, 10, "JOKEANATOR-3000", 50);  // slower speed for dramatic effect

  // Smaller subtext
  display.setTextSize(1.5);
  typeWriterText(0, 40, "\nby 0crash", 50);        

  delay(1500);  
}



void setup() {
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  pinMode(WAKE_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(WAKE_PIN), wakeUp, FALLING);

  lastCLK = digitalRead(CLK);

  display.begin(0x3C, true);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  showOpeningScreen();

  randomSeed(analogRead(A0));
  jokeIndex = random(0, jokeCount);
  
  showQuestion();
}

// --------------------------------------------
// MAIN LOOP
// --------------------------------------------
int stepCount = 0;
const int stepsPerJoke = 2;   
unsigned long lastInteraction = 0;
const unsigned long sleepDelay = 30000; // 30s inactivity

void loop() {
  if (isOn) {
    int currentCLK = digitalRead(CLK);

    // ROTATION
    if (currentCLK != lastCLK && currentCLK == 1) {
      stepCount++;
      lastInteraction = millis(); // optional if you keep inactivity timer

      if (stepCount >= stepsPerJoke) {
        stepCount = 0;
        if (digitalRead(DT) != currentCLK) jokeIndex++;
        else jokeIndex--;

        if (jokeIndex < 0) jokeIndex = jokeCount - 1;
        if (jokeIndex >= jokeCount) jokeIndex = 0;

        if (!showingPunchline) showQuestion();
      }
    }
    lastCLK = currentCLK;

    // BUTTON PRESS TOGGLE (SW)
    if (digitalRead(SW) == LOW) {
      delay(180); // simple debounce
      lastInteraction = millis();
      showingPunchline = !showingPunchline;
      if (showingPunchline) showAnswer();
      else showQuestion();
    }

 } else {
    // Splash screen loop
    while (!isOn) { // loop until D2 pressed
        unsigned long now = millis();
        if (now - lastBlink >= blinkInterval) {
            lastBlink = now;
            eyesOpen = !eyesOpen;
            drawEyes();
        }

        // Let MCU idle sleep between frames to save power
        set_sleep_mode(SLEEP_MODE_IDLE);
        sleep_enable();
        sleep_cpu();    // wakes immediately on interrupt
        sleep_disable();
    }

    // Once turned ON, resume joke machine
    lastCLK = digitalRead(CLK);   // reset encoder reading
    showingPunchline = false;     // reset punchline
    showOpeningScreen();           // splash screen once
    showQuestion();                // then show a random joke question
}

  delay(50); // small delay to debounce & reduce CPU use
}




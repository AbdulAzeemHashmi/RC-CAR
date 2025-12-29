#include <Servo.h>

const int tiltPin = 2;
const int pir1Pin = 3;
const int pir2Pin = 4;
const int ledPin = 9;
const int buzzerPin = 8;
const int servoPin = 10;

unsigned long lastMotionTime = 0;
unsigned long pirFaultTimer = 0;
bool systemTilted = false;

unsigned long prevLedTilt = 0;
unsigned long prevLedPIR = 0;
unsigned long prevFaultToggle = 0;

const unsigned long ledIntervalTilt = 200;
const unsigned long ledIntervalPIR = 600;
const unsigned long faultToggleInterval = 400;

int motionCount = 0;
int tiltCount = 0;
int faultCount = 0;

Servo myServo;

int lastTiltState = -1;
unsigned long lastTiltChangeTime = 0;
const unsigned long TILT_STUCK_MS = 30000UL;

int lastPir1 = -1;
int lastPir2 = -1;
unsigned long lastPirChangeTime = 0;
bool pirEverChanged = false;

const unsigned long DEBOUNCE_MS = 50;
unsigned long tiltBounceTime = 0;
unsigned long pir1BounceTime = 0;
unsigned long pir2BounceTime = 0;

bool motionActive = false;

void setup() {
  pinMode(tiltPin, INPUT_PULLUP); 
  pinMode(pir1Pin, INPUT_PULLUP); 
  pinMode(pir2Pin, INPUT_PULLUP); 

  pinMode(ledPin, OUTPUT); 
  pinMode(buzzerPin, OUTPUT); 

  myServo.attach(servoPin); 
  myServo.write(0); 

  Serial.begin(9600); 
  Serial.println("System Booting..."); 
  delay(1000); 
  Serial.println("System Ready."); 

  lastTiltState = digitalRead(tiltPin); 
  lastTiltChangeTime = millis(); 

  lastPir1 = digitalRead(pir1Pin); 
  lastPir2 = digitalRead(pir2Pin); 
  lastPirChangeTime = millis(); 
}

void loop() {
  unsigned long now = millis(); 

  int rawTilt = digitalRead(tiltPin); 
  int rawPir1 = digitalRead(pir1Pin); 
  int rawPir2 = digitalRead(pir2Pin); 

  if (rawTilt != lastTiltState && (now - tiltBounceTime > DEBOUNCE_MS)) {
    tiltBounceTime = now; 
    handleTiltChange(rawTilt, now); 
    lastTiltState = rawTilt; 
  }

  if (rawPir1 != lastPir1 && (now - pir1BounceTime > DEBOUNCE_MS)) {
    pir1BounceTime = now; 
    lastPir1 = rawPir1; 
    pirEverChanged = true; 
    lastPirChangeTime = now; 
  }

  if (rawPir2 != lastPir2 && (now - pir2BounceTime > DEBOUNCE_MS)) {
    pir2BounceTime = now; 
    lastPir2 = rawPir2; 
    pirEverChanged = true; 
    lastPirChangeTime = now; 
  }

  if ((now - lastTiltChangeTime) > TILT_STUCK_MS) {
    Serial.println("⚠ TILT SENSOR WARNING: MAY BE STUCK (warning only)"); 
    lastTiltChangeTime = now; 
  }

  if (systemTilted) {
    tiltAlarm(now); 
    motionActive = false; 
  } 
  else {
    bool pirTriggered = (lastPir1 == LOW || lastPir2 == LOW); 
    if (pirTriggered && !motionActive) {
      motionActive = true; 
      lastMotionTime = now; 
      motionCount++; 
      Serial.print("👀 Motion Detected! Count: "); 
      Serial.println(motionCount); 
      motionAlert(now); 
    } 
    else if (!pirTriggered) {
      motionActive = false; 
    }
  }

  const unsigned long PIR_FAULT_WINDOW = 10000UL; 
  if (pirEverChanged && (now - lastPirChangeTime > PIR_FAULT_WINDOW)) {
    faultCount++; 
    Serial.print("⚠ POSSIBLE FAULT: PIR sensors unresponsive! Count: "); 
    Serial.println(faultCount); 
    faultIndicator(now); 
    lastPirChangeTime = now; 
  }
}

void handleTiltChange(int tiltVal, unsigned long now) {
  lastTiltChangeTime = now; 
  bool isTilted = (tiltVal == LOW); 
  if (isTilted && !systemTilted) {
    systemTilted = true; 
    tiltCount++; 
    Serial.println("⚠ WARNING: Device Tilted! System Locked."); 
    myServo.write(90); 
  } 
  else if (!isTilted && systemTilted) {
    systemTilted = false; 
    Serial.println("ℹ System Stable. Resuming Sensors..."); 
    myServo.write(0); 
  }
}

void motionAlert(unsigned long now) {
  static bool ledState = LOW; 
  if (now - prevLedPIR >= ledIntervalPIR) {
    ledState = !ledState; 
    digitalWrite(ledPin, ledState); 
    prevLedPIR = now; 
    tone(buzzerPin, 2000, 100); 
  }
  myServo.write(45); 
}

void tiltAlarm(unsigned long now) {
  static bool ledState = LOW; 
  if (now - prevLedTilt >= ledIntervalTilt) {
    ledState = !ledState; 
    digitalWrite(ledPin, ledState); 
    prevLedTilt = now; 
    if (ledState) tone(buzzerPin, 1500, 200); 
  }
  myServo.write(90); 
}

void faultIndicator(unsigned long now) {
  static bool ledState = LOW; 
  if (now - prevFaultToggle >= faultToggleInterval) {
    ledState = !ledState; 
    digitalWrite(ledPin, ledState); 
    if (ledState) tone(buzzerPin, 1000, 150); 
    prevFaultToggle = now; 
  }
}

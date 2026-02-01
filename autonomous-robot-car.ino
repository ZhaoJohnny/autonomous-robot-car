#include <WiFiS3.h>
#include <WiFiUdp.h>

// ===================== Motors =====================
int enA = 9;
int in1 = 8;
int in2 = 7;

int enB = 10;
int in3 = 12;
int in4 = 13;

int speed = 200;

// ===================== Wi-Fi AP =====================
const char* ssid = "COGS300_BOT";
const char* pass = "robot1234";

WiFiUDP udp;
const unsigned int controlPort = 4210;

// ===================== Fail-safe =====================
unsigned long lastCmdMs = 0;
const unsigned long failSafeMs = 250;

// ===================== Encoders =====================
const int ENC_L_PIN = 2;
const int ENC_R_PIN = 3;

volatile long ticksL = 0;
volatile long ticksR = 0;

void isrEncL() { ticksL++; }
void isrEncR() { ticksR++; }

const int HOLES_PER_REV = 20;
const float WHEEL_RADIUS_CM = 3.0;

// ===================== Photocell Step 3 =====================
const int PHOTO_PIN = A0;   // change if needed
int threshold = 155;        // >175 tape, <=175 floor (your calibration)
const int STABLE_N = 3;

int markerState = 0;        // 0 idle, 1 clearing start tape, 2 driving to end tape, 3 done
bool markerActive = false;
int stableOn = 0;
int stableOff = 0;

// ===================== Telemetry UDP =====================
const unsigned int telemetryPort = 4211;

// Broadcast is easiest. Your laptop on the AP will receive it.
IPAddress telemetryIP(192, 168, 4, 255);

// ===================== Logging timing =====================
const unsigned long LOG_EVERY_MS = 500;
unsigned long lastLogMs = 0;

char lastCmd = 'S';

// ===================== Motor driver =====================
void drive(int a1, int a2, int b1, int b2) {
  digitalWrite(in1, a1);
  digitalWrite(in2, a2);
  analogWrite(enA, (a1 == a2) ? 0 : speed);

  digitalWrite(in3, b1);
  digitalWrite(in4, b2);
  analogWrite(enB, (b1 == b2) ? 0 : speed);
}

// ===================== Photocell helpers =====================
bool onTape(int measurement) {
  return measurement > threshold;
}

void startMarkerRun() {
  markerActive = true;
  markerState = 1;
  stableOn = 0;
  stableOff = 0;

  // Start moving immediately to clear the start marker
  drive(LOW, HIGH, LOW, HIGH);
  lastCmd = 'F';
}

void cancelMarkerRun() {
  markerActive = false;
  markerState = 0;
  stableOn = 0;
  stableOff = 0;

  drive(LOW, LOW, LOW, LOW);
  lastCmd = 'S';
}

void updateMarkerRun(int photo) {
  bool tape = onTape(photo);

  if (markerState == 1) {
    // Drive forward until OFF tape consistently
    drive(LOW, HIGH, LOW, HIGH);
    lastCmd = 'F';

    if (!tape) {
      stableOff++;
      stableOn = 0;
      if (stableOff >= STABLE_N) {
        markerState = 2;
        stableOff = 0;
        stableOn = 0;
      }
    } else {
      stableOn++;
      stableOff = 0;
    }
  }
  else if (markerState == 2) {
    // Drive forward until ON tape consistently (end marker)
    drive(LOW, HIGH, LOW, HIGH);
    lastCmd = 'F';

    if (tape) {
      stableOn++;
      stableOff = 0;
      if (stableOn >= STABLE_N) {
        markerState = 3;
        markerActive = false;
        drive(LOW, LOW, LOW, LOW);
        lastCmd = 'S';
      }
    } else {
      stableOff++;
      stableOn = 0;
    }
  }
  else if (markerState == 3) {
    drive(LOW, LOW, LOW, LOW);
    lastCmd = 'S';
  }
}

// ===================== Command handler =====================
void handleCommand(char cmd) {
  if (cmd == 'P') { startMarkerRun(); return; }
  if (cmd == 'S') { cancelMarkerRun(); return; }

  // If marker run active, ignore steering commands, allow speed
  if (markerActive) {
    if (cmd >= '0' && cmd <= '9') {
      int val = cmd - '0';
      speed = map(val, 0, 9, 0, 255);
      lastCmd = cmd;
    }
    return;
  }

  if (cmd == 'F')      { drive(LOW, HIGH, LOW, HIGH); lastCmd = 'F'; }
  else if (cmd == 'B') { drive(HIGH, LOW, HIGH, LOW); lastCmd = 'B'; }
  else if (cmd == 'L') { drive(LOW, HIGH, HIGH, LOW); lastCmd = 'L'; }
  else if (cmd == 'R') { drive(HIGH, LOW, LOW, HIGH); lastCmd = 'R'; }

  else if (cmd == 'q') { drive(LOW, HIGH, LOW, LOW);  lastCmd = 'q'; }
  else if (cmd == 'e') { drive(LOW, LOW, LOW, HIGH);  lastCmd = 'e'; }
  else if (cmd == 'z') { drive(HIGH, LOW, LOW, LOW);  lastCmd = 'z'; }
  else if (cmd == 'c') { drive(LOW, LOW, HIGH, LOW);  lastCmd = 'c'; }

  else if (cmd >= '0' && cmd <= '9') {
    int val = cmd - '0';
    speed = map(val, 0, 9, 0, 255);
    lastCmd = cmd;
  }
}

// ===================== Telemetry sender =====================
void sendTelemetryUDP(int photo) {
  long tL, tR;
  noInterrupts();
  tL = ticksL;
  tR = ticksR;
  interrupts();

  float rotL = (float)tL / (float)HOLES_PER_REV;
  float rotR = (float)tR / (float)HOLES_PER_REV;

  float distL = rotL * (TWO_PI * WHEEL_RADIUS_CM);
  float distR = rotR * (TWO_PI * WHEEL_RADIUS_CM);

  int tapeFlag = onTape(photo) ? 1 : 0;

  char buf1[120];
  char buf2[160];

  // PHOTO packet
  snprintf(buf1, sizeof(buf1), "PHOTO,%lu,%d,%d,%d",
           millis(), photo, tapeFlag, markerState);

  // DIST packet
  snprintf(buf2, sizeof(buf2), "DIST,%lu,%.2f,%.2f,%.3f,%.3f",
           millis(), distL, distR, rotL, rotR);

  // Send both
  udp.beginPacket(telemetryIP, telemetryPort);
  udp.write((const uint8_t*)buf1, strlen(buf1));
  udp.endPacket();

  udp.beginPacket(telemetryIP, telemetryPort);
  udp.write((const uint8_t*)buf2, strlen(buf2));
  udp.endPacket();

  // Also print to Serial for debugging if you ever get USB back
  Serial.println(buf1);
  Serial.println(buf2);
}

// ===================== Setup / Loop =====================
void setup() {
  pinMode(enA, OUTPUT); pinMode(in1, OUTPUT); pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT); pinMode(in3, OUTPUT); pinMode(in4, OUTPUT);

  pinMode(ENC_L_PIN, INPUT_PULLUP);
  pinMode(ENC_R_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_L_PIN), isrEncL, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_R_PIN), isrEncR, RISING);

  Serial.begin(115200);
  delay(300);

  int status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Failed to start AP");
  } else {
    Serial.print("AP started: ");
    Serial.println(ssid);
    Serial.print("Robot IP: ");
    Serial.println(WiFi.localIP());
  }

  udp.begin(controlPort);
  Serial.print("UDP control listening on port ");
  Serial.println(controlPort);

  drive(LOW, LOW, LOW, LOW);
  lastCmdMs = millis();
  lastLogMs = millis();
}

void loop() {
  int photo = analogRead(PHOTO_PIN);

  // Receive control commands
  int packetSize = udp.parsePacket();
  if (packetSize > 0) {
    char cmd = 0;
    udp.read(&cmd, 1);
    lastCmdMs = millis();
    handleCommand(cmd);
  }

  // Auto run
  if (markerActive) {
    lastCmdMs = millis();
    updateMarkerRun(photo);
  } else {
    if (millis() - lastCmdMs > failSafeMs) {
      drive(LOW, LOW, LOW, LOW);
      lastCmd = 'S';
    }
  }

  // Telemetry packets every LOG_EVERY_MS
  if (millis() - lastLogMs >= LOG_EVERY_MS) {
    lastLogMs = millis();
    sendTelemetryUDP(photo);
  }
}
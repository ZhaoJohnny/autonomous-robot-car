aspect biosystems
jia_wei_zhao
Online

aspect biosystems — Yesterday at 4:11 PM
or sunday
Lucca

 — Yesterday at 4:12 PM
Sounds good to me, I have all day free tmrw
Leon — Yesterday at 4:12 PM
Yeah wait tmr sounds good to me
😭
Lucca

 — Yesterday at 4:12 PM
Apart from an examlet at 5 but we’ll be done by then
aspect biosystems — Yesterday at 4:13 PM
I have to go to costco with a friend at 9am
so Ill pull up after that
Lucca

 — Yesterday at 4:13 PM
Sounds good
aspect biosystems — Yesterday at 4:13 PM
wait fuck
Lucca

 — Yesterday at 4:13 PM
I’ll be there 12 ish
aspect biosystems — Yesterday at 4:13 PM
FUCK
WESLEY JUST GOT HERE
IM STILL IN JAIL
Lucca

 — Yesterday at 4:13 PM
Oh fuck
aspect biosystems — Yesterday at 4:13 PM
GET ME OUT OF JAIL
Lucca

 — Yesterday at 4:13 PM
Tell him we have a new plan
aspect biosystems — Yesterday at 4:23 PM
he cant come lOL
unlcuky
also let me double check if we have access
Lucca

 — Yesterday at 4:24 PM
We do Chaavi said last time
aspect biosystems — Yesterday at 4:27 PM
what did she say last time
Lucca

 — Yesterday at 4:28 PM
We can use it on weekends
I’ll be there in 30 to help you guys
aspect biosystems — Yesterday at 4:32 PM
what time tomorrow
just so we know what the concrete plan is
Wesley — Yesterday at 4:37 PM
ok list of things to do:

calibration with white tape
distance measuring
game which shiuld be super easy
Lucca

 — Yesterday at 4:38 PM
Are you guys leaving?
aspect biosystems — Yesterday at 4:38 PM
we're still here
Wesley — Yesterday at 4:42 PM
@Lucca where the code at big dog
Lucca

 — Yesterday at 4:48 PM
Oh shoot
Wait
I’mma pull up in like 5-10 mins
Walking to you guys rn
Leon — Yesterday at 4:58 PM
Shit
If yall don’t come tmr lmk where the car is tmr
I’ll be there
Lucca

 — Yesterday at 5:11 PM
import java.net.*;
import processing.serial.*;

// ================= UDP =================
DatagramSocket socket;
InetAddress robotIP;

message.txt
5 KB
#include <WiFiS3.h>
#include <WiFiUdp.h>

// ===================== Motors =====================
// Motor A (Left)
int enA = 9;

message.txt
5 KB
Lucca

 — Yesterday at 6:55 PM
import java.net.*;

// ================= UDP (Control) =================
DatagramSocket controlSocket;
InetAddress robotIP;
int robotPort = 4210;

message.txt
9 KB
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

... (178 lines left)

message.txt
7 KB
Lucca

 — 12:03 PM
What time do you guys want to go to the lab today?
aspect biosystems — 12:03 PM
Im probably going to head there soonish
probably get there around 1?
I woke up with a massive headache today
so im just waiting for that to simmer down
Lucca

 — 12:51 PM
I’ll be there around 1 as well
aspect biosystems — 1:22 PM
Alright Im walking over now
Are you there Lucca
Lucca

 — 1:33 PM
Yes
aspect biosystems — 1:34 PM
Alright Ill be there soon
Leon — 2:23 PM
I’ll be there ~3
Just grabbing food
﻿
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
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// LCD inicializācija (Adrese 0x27 vai 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- PINI ---
const int IN1 = 5;
const int IN2 = 6;
const int IN3 = 7;
const int IN4 = 8;

const int trigPin = 12;
const int echoPin = 13;
const int lightPin = A0;
const int buttonPin = 3;

// --- GAISMU PINI ---
const int sarkanPin = 4; 
const int zalsPin = 10;   
const int zilsPin = 9;  

// Mainīgie
int gaismasSlieksnis = 300;
bool robotsAktivizets = false;
long duration;
int distance;
int gaisma;

void setup() {
  // Inicializējam LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Robots sak darbu");

  // Motoru un sensoru pini
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(sarkanPin, OUTPUT);
  pinMode(zalsPin, OUTPUT);
  pinMode(zilsPin, OUTPUT);

  // Sākumā izslēdzam gaismas
  digitalWrite(sarkanPin, LOW);
  digitalWrite(zalsPin, LOW);
  digitalWrite(zilsPin, LOW);

  Serial.begin(9600);
  delay(1000);
  lcd.clear();
}

void loop() {
  // 1. POGAS LASĪŠANA
  if (digitalRead(buttonPin) == LOW) {
    robotsAktivizets = !robotsAktivizets;
    delay(500); // Pauze, lai nereaģētu uzreiz vairākas reizes
  }

  // --- JA ROBOTS IR APTURĒTS ---
  if (!robotsAktivizets) {
    stopMotors();
    digitalWrite(sarkanPin, HIGH);
    digitalWrite(zalsPin, LOW);
    
    lcd.setCursor(0, 0);
    lcd.print("Status: STOP    ");
    lcd.setCursor(0, 1);
    lcd.print("Spied pogu!     ");
    return; 
  }

  // 2. MĒRĪJUMI (Ja robots ir palaists)
  gaisma = analogRead(lightPin);
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // 3. LCD DISPLEJA INFO (Kā 7. janvāra kodā)
  lcd.setCursor(0, 0);
  lcd.print("Gaisma: ");
  lcd.print(gaisma);
  lcd.print("    ");

  lcd.setCursor(0, 1);
  lcd.print("Att: ");
  lcd.print(distance);
  lcd.print("cm      ");

  // 4. KUSTĪBAS UN GAISMAS LOĢIKA
  if (gaisma > gaismasSlieksnis) {
    stopMotors();
    digitalWrite(sarkanPin, HIGH);
    digitalWrite(zalsPin, LOW);
  } 
  else if (distance > 0 && distance < 20) {
    griezties();
    digitalWrite(sarkanPin, LOW);
    digitalWrite(zalsPin, HIGH);
  } 
  else {
    braukt();
    digitalWrite(sarkanPin, LOW);
    digitalWrite(zalsPin, HIGH);
  }

  delay(50);
}

// --- FUNKCIJAS ---
void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void braukt() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);  digitalWrite(IN4, LOW);
}

void griezties() {
  digitalWrite(IN1, HIGH );  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);  digitalWrite(IN4, HIGH);
}
// Variables for averaging analog input
const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

// Variables for reading the PIR
int ledPin = 13;                // choose the pin for the LED
int inputPin = 2;               // choose the input pin (for PIR sensor)
int buttonPin = 8;              // choose the input pin (for button)
int redPin = 11;
int greenPin = 10;
int bluePin = 9;

int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
int buttonState = LOW;
int excuseMode = 0;
const int MODE_LAZY = 0;
const int MODE_FRUSTRATED = 1;
const int MODE_RESENTFUL = 2;
const int MODE_DEPRESSED = 3;
const int MODE_COUNT = 4;

#define COMMON_ANODE = TRUE
// Variables for analog reading
int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor

// Setup the pins and serial ports
void setup() {
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  pinMode(buttonPin, INPUT);     // declare sensor as input
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  
  Serial.begin(115200);

  setColor(255, 150, 0);

  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

String getMood() {
    switch(excuseMode) {
      case MODE_LAZY:
        return "lazy";
      case MODE_FRUSTRATED:
        return "frustrated";
      case MODE_RESENTFUL:        
        return "resentful";        
      case MODE_DEPRESSED:
        return "depressed";
    } 
}

void setMoodColor() {
   switch(excuseMode) {
      case MODE_LAZY:
        setColor(255, 150, 0);
        Serial.println("lazy: yellow");
        break;
      case MODE_FRUSTRATED:
        setColor(255, 0, 0);
        Serial.println("frustrated: red");
        break;
      case MODE_RESENTFUL:
        setColor(0, 255, 0);
        Serial.println("resentful: green");
        break;
      case MODE_DEPRESSED:
        setColor(0, 0, 255);
        Serial.println("depressed: blue");
        break;
    }
}

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

// Loops as fast as the microcontroller can go
void loop() {
  // ************* PIR Input Reading *************
  val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    if (pirState == LOW) {
      // we have just turned on
      Serial.print("saw something, mood: ");
      Serial.println(getMood());
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("reset");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }


  val = digitalRead(buttonPin);  // read input value
  if (val == HIGH && buttonState == LOW) {            // check if the input is HIGH    
    buttonState = HIGH;
    excuseMode++;
    excuseMode = excuseMode % MODE_COUNT;
    setMoodColor();
  } else if (val == LOW && buttonState == HIGH) {    
    buttonState = LOW;    
  }
  delay(100);
}

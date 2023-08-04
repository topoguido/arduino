#define STEP 4
#define DIR 5

int limite = 170;
int delayStep = 3;

void setup() {
 
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);

}

void loop() {
  
  digitalWrite(DIR, HIGH);
  for(int i = 0; i < limite; i++)
  {
    digitalWrite(STEP, HIGH);
    delay(delayStep);
    digitalWrite(STEP, LOW);
    delay(delayStep);
  }
  
  
  delay(5000);
  digitalWrite(DIR, LOW);
  for(int i = 0; i < limite; i++)
  {
    digitalWrite(STEP, HIGH);
    delay(delayStep);
    digitalWrite(STEP, LOW);
    delay(delayStep);
  }
  delay(5000);
   //exit(0);
}

int Disable = 14;
int DIR = 15;
int STEP = 13;
int start = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(Disable, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);

  digitalWrite(Disable, LOW);
  digitalWrite(DIR, LOW);
  digitalWrite(STEP, LOW);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(start == 0)
  {
    for(int i=0; i<2000;i++)
    {
      digitalWrite(STEP,HIGH);
      delay(1);
      digitalWrite(STEP,LOW);
    }
    start = 1;
    digitalWrite(Disable, HIGH);
  }
}

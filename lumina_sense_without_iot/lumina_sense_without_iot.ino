int IR1 = 5;
int IR2 = 6;
int LDR = 2;
int led1 = 10;
int led2 = 9;
int val1;
int val2;
int val3;
int val4;

void setup()
{
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(LDR, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}

void loop()
{
  val1 = digitalRead(IR1);
  val2 = digitalRead(IR2);
  val4 = analogRead(LDR);

  if (val4 == 0)
  {
    analogWrite(led1, 20);
    analogWrite(led2, 20);

    if (val1 == 0)
    {
      analogWrite(led1, 255); 

      while (val2 != 0)
      {
        delay(10);
        val2 = digitalRead(IR2);
      }

      analogWrite(led1, 20);
    }

    if (val2 == 0)
    {
      analogWrite(led2, 255); 

      while (val1 != 0)
      {
        delay(10);
        val1 = digitalRead(IR1);
      }

      analogWrite(led2, 20);
    }
    if (val1==0 && val2==0){
      analogWrite(led2, 255); 
      analogWrite(led1, 255); 

    }
  }
}
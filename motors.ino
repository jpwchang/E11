#define LPLUS 9
#define LMINUS 8
#define LEN 6
#define RPLUS 7
#define RMINUS 12
#define REN 11

void motorEn()
{
  digitalWrite(LEN, HIGH);
  digitalWrite(REN, HIGH);
}

void initMotors()
{
  pinMode(LPLUS, OUTPUT);
  pinMode(LMINUS, OUTPUT);
  pinMode(LEN, OUTPUT);
  pinMode(RPLUS, OUTPUT);
  pinMode(RMINUS, OUTPUT);
  pinMode(REN, OUTPUT);
}

void halt()
{
  motorEn();
  analogWrite(LPLUS, 0);
  analogWrite(LMINUS, 0);
  analogWrite(RPLUS, 0);
  analogWrite(RMINUS, 0);
}

void drive(int left, int right)
{
  motorEn();
  
  if(left < 0)
  {
    analogWrite(LPLUS, 0);
    analogWrite(LMINUS, abs(left));
  }
  else
  {
    analogWrite(LPLUS, abs(left));
    analogWrite(LMINUS, 0);
  }
  
  if(right < 0)
  {
    analogWrite(RPLUS, 0);
    analogWrite(RMINUS, abs(right));
  }
  else
  {
    analogWrite(RPLUS, abs(right));
    analogWrite(RMINUS, 0);
  }
}

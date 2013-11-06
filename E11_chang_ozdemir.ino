#define GCSIZE 31
#define NUMCODES 8
#define NOLIGHT 750
#define SEEDLEN 5

int values[GCSIZE];
boolean detectedCode[GCSIZE];

boolean goldCodes[8][31] = {{0,0,0,0,0,0,0,1,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1},
 {1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,0,0,0,1,1,1,1,0,0,0,1,0,1,0,0},
 {0,1,0,0,0,0,1,1,0,1,0,0,0,0,1,0,1,1,1,1,1,1,0,1,0,1,0,1,1,1,0},
 {1,0,1,0,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1,1,0,1,0,0,0,0,1,1,1,0,1},
 {0,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,1,1,1,0,1,0,0,1,1,1},
 {1,1,1,0,0,0,1,0,0,1,1,0,1,1,1,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0},
 {0,1,1,0,0,1,1,0,1,1,0,1,1,1,0,1,1,1,1,0,0,1,1,0,1,1,1,1,0,1,0},
 {1,0,0,1,0,1,1,1,0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,0,0,1,0,0,0,1,1}};

boolean test[31] = {0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,0,0,1,0,0,0,1,1,1,0,0,1,0,1,1,1};

int curDCIndex = 0; //Which index of detectedCode are we on?

int state = 0;
int nav_state = 0;

unsigned long loopStart = 0;
int average = 0;

void setup()
{
  Serial.begin(9600);
  initSensors();
  initMotors();
}

void loop()
{
  if(getTouch())
    Serial.println(digitalRead(2));
  //FSM model for navigation
  switch(nav_state)
  {
  case 0:
  {
    drive(255,255);
    if(getTouch())
      nav_state++;
    break;
  }
  case 1:
    //halt();
    break;
  }
  
  //FSM model for GC detection
  switch(state)
  {
  case 0:
  {
    loopStart = micros();
    for(int i = 0; i < GCSIZE; i++)
    {
      while(micros()-loopStart < 250) ;
      loopStart = micros();
      values[i] = analogRead(19-14);
      average += values[i];
    }
    state++;
    break;
  }
  case 1:
  {
    average /= GCSIZE;
    state++;
    break;
  }
  case 2:
  {
    if(curDCIndex < GCSIZE)
    {
      detectedCode[curDCIndex] = values[curDCIndex] > average;
      curDCIndex++;
    }
    else state++;
    break;
  }
  case 3:
  {
    for(int i = 0; i < NUMCODES; i++)
    {
      int cor = corGoldCodes(goldCodes[i], detectedCode);
      if(abs(cor) > 24)
      {
        Serial.println("Found match");
        Serial.print("ID: ");
        Serial.println(i+1);
        Serial.print("Correlation: ");
        Serial.println(cor);
        Serial.print("Color: ");
        if(cor < 0) Serial.println("White");
        else Serial.println("Green");
        break;
      }
    }
    state++;
    break;
  }
  case 4:
  {
    average = 0;
    curDCIndex = 0;
    state = 0;
  }
  default:
    break;
  }
}

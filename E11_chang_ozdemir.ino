#define GCSIZE 31
#define NUMCODES 8
#define NOLIGHT 750
#define SEEDLEN 5
#define GRAY 800
#define BLACK 915

typedef enum {
  NONE,
  WHITE,
  GREEN
} team;

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
int prev_touch = 0;

unsigned long loopStart = 0;
int average = 0;
team detectedTeam = NONE;
team myTeam = NONE;
int prevReflect = 0;
int oppSeen = 0;

boolean on = true;
int startOff = 0;

void setup()
{
  Serial.begin(9600);
  initSensors();
  initMotors();
  if(getTeam() == 0) myTeam = GREEN;
  else myTeam = WHITE;
}

void loop()
{
  //FSM model for navigation
  switch(nav_state)
  {
  case 0:
  {
    int r = getReflect();
    if(r < GRAY || prevReflect < GRAY)
      drive(255,255);
    else
    {
      drive(-255,255);
      delay(100);
      nav_state++;
    }
    prevReflect = r;
    break;
  }
  case 1:
  {
    int reading = 0;
    for(int i = 0; i < 20; i++)
      reading += getReflect();
    reading /= 20;
    if(oppSeen < 350)
    {
      if(reading < GRAY || reading > BLACK)
      {
        int rpow = 100;
        if(on)
        {
          on = false;
          startOff = millis();
        }
        else
        {
          if(millis()-startOff > 100)
            rpow = 0;
        }
        drive(255, rpow);
      }
      else
      {
        if(!on) on=true;
        drive(150,255);
      }
      
      if(detectedTeam != NONE && detectedTeam != myTeam)
        oppSeen++;
    }
    else
    {
      oppSeen = 0;
      nav_state++;
    }
    break;
  }
  case 2:
  {
    detectedTeam = NONE;
    drive(0,-150);
    delay(600);
    nav_state++;
    break;
  }
  case 3:
  {
    if(!getTouch())
      drive(255,255);
    else
      nav_state++;
    break;
  }
  case 4:
  {
    drive(-255,-255);
    delay(250);
    drive(-255,255);
    delay(250);
    nav_state = 1;
    detectedTeam = NONE;
    break;
  }
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
        if(cor < 0) detectedTeam = WHITE;
        else detectedTeam = GREEN;
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

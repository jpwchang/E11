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

int curDCIndex = 0; //Which index of detectedCode are we on?

int state = 0;

int loopStart = 0;
int average = 0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  switch(state)
  {
  case 0:
  {
    if(curDCIndex < GCSIZE)
    {
      if(loopStart == 0) loopStart = micros();
      if(micros() - loopStart >= 220)
      {
        int l = getLight();
        values[curDCIndex] = l;
        curDCIndex++;
        average += l;
        loopStart = 0;
      }
    }
    else 
    {
      curDCIndex = 0;
      state++;
    }
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
    Serial.println(corGoldCodes(goldCodes[4], detectedCode));
    Serial.println(average);
    //Serial.println(strGC(detectedCode));
    state++;
    break;
  }
  }
}

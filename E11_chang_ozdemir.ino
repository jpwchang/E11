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

unsigned long loopStart = 0;
int average = 0;

void setup()
{
  Serial.begin(9600);
  initSensors();
}

void loop()
{
  switch(state)
  {
  case 0:
  {
    /*if(curDCIndex < GCSIZE)
    {
      //if(loopStart == 0) loopStart = micros();
      long diff = micros() - loopStart;
      Serial.println(diff);
      if(diff >= 250)
      {
        loopStart = micros();
        //int l = getLight();
        //values[curDCIndex] = l;
//curDCIndex++;
        //average += l;
      }
    }
    else 
    {
      curDCIndex = 0;
      state++;
    }
    break;*/
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
      //Serial.print("Correlating with GC ");
      //Serial.println(i+1);
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
    Serial.println(strGC(detectedCode));
    //Serial.println("Done");
    state++;
    break;
  }
  case 4:
  {
    average = 0;
    curDCIndex = 0;
    //state = 0;
  }
  default:
    break;
  }
}

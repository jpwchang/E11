//#define GCSIZE 31

//boolean detectedCode[GCSIZE];

//int curDCIndex = 0; //Which index of detectedCode are we on?

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  generateGoldCodes();
  boolean b[31] = {0,0,0,0,0,0,0,1,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1};
  boolean c[31] = {1,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,0,0,0,0,0,0,0};
  int a = corGoldCodes(b,c);
  Serial.print("The correlation is:");
  Serial.println(a);


}

void loop()
{
  /*
  if(curDCIndex < GCSIZE)
  {
    if(getLight() > NOLIGHT)
    {
      detectedCode[curDCIndex] = 1;
    }
    else
    {
      detectedCode[curDCIndex] = 0;
    }
    curDCIndex++;
    delay(1);
  }
  else
    printCode(curDCIndex);
    */
}

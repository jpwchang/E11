boolean detectedCode[numSteps];

int curDCIndex = 0; //Which index of detectedCode are we on?

void setup()
{
  Serial.begin(9600);
  initSensors(); //DO NOT CHANGE OR REMOVE THIS LINE
  initMotors(); //DO NOT CHANGE OR REMOVE THIS LINE
  
  int test = getLight();
  Serial.println(test);
}

void loop()
{
  if(getLight() > NOLIGHT)
  {
    
  }
}

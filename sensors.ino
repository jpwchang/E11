/**
 * E11 Sensors Interface
 * Jonathan Chang - 29 Oct 2013
 *
 * Provides functions for accessing robot sensor values
 */

#define DISTSENSOR 14
#define REFLECT 18
#define PHOTOT 19
#define TOUCH 2

#define NOLIGHT 800 //Light threshold (may have to change)

//Set up our sensors
void initSensors()
{
  pinMode(DISTSENSOR, INPUT);
  pinMode(REFLECT, INPUT);
  pinMode(PHOTOT, INPUT);
  pinMode(TOUCH, INPUT);
}

//Get the value of the reflectance sensor
int getReflect()
{
  return analogRead(REFLECT-14);
}

//Get the value of the distance sensor
int getDist()
{
  return analogRead(DISTSENSOR-14);
}

//Get the value of the phototransistor
int getLight()
{
  return analogRead(PHOTOT-14);
}

//Get the value of the touch sensor
int getTouch()
{
  return digitalRead(TOUCH);
}

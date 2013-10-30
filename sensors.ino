#define DISTSENSOR 14
#define REFLECT 18
#define PHOTOT 19

void initSensors()
{
  pinMode(DISTSENSOR, INPUT);
  pinMode(REFLECT, INPUT);
  pinMode(PHOTOT, INPUT);
}

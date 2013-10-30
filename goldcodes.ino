#define seedSize 5
#define numSeeds 8
#define numSteps 31

boolean gcSeeds[numSeeds][seedSize] = {{0,0,0,0,1}, {0,0,0,1,0}, {0,0,0,1,1}, {0,0,1,0,0},
    {0,0,1,0,1},{0,0,1,1,0}, {0,0,1,1,1}, {0,1,0,0,0}};
boolean defaultSeed[seedSize] = {0,0,0,0,1};
int tap1[4] = {2,3,4,5};
int tap2[2] = {3,5};

//Function prototypes for functions we call in setup
//Needed here so we can actually use them in setup
//Functions are defined and commented later in file
void lsfr(boolean* seed, boolean (&result) [numSteps], int* taps, int taplength);
void joinCodes(boolean* gc1, boolean* gc2, boolean (&result) [numSteps]);
void shiftGC(boolean (&seed) [numSteps]);
void copyArray(boolean* source, boolean (&destination) [numSteps]);
void generateCode(boolean (&result), int* taps, int taplength)

//Create a gold code and store it in result
void generateCode(boolean (&result), boolean* seed, int* tap1, int* tap2, int tap1_l, int tap2_l)
{
  boolean defaultGC[numSteps];
  lsfr(defaultSeed, defaultGC, tap1, tap1_l);
  boolean otherGC[numSteps];
  lsfr(seed, otherGC, tap2, tap2_l);
  joinCodes(defaultGC, otherGC, result);
}

//XORs two booleans
boolean x_or(boolean* seed, int* taps, int taplength)
{
  int countTrue = 0;
  for(int i = 0; i < taplength; i++)
  {
    if(seed[taps[i]-1] == 1)
      countTrue++;
  }
  return countTrue%2;
}

boolean x_or(boolean b1, boolean b2)
{
  if(b1 == b2)
    return 0;
  else
    return 1;
}

//Shift the elements in seed to the right
void shift(boolean* (& seed))
{
  boolean last = seed[seedSize-1];
  for(int i = seedSize-1; i > 0; i--)
  {
    seed[i] = seed[i-1];
  }
  seed[0] = last;
}

//Shifts the gold code to the right
void shiftGC(boolean (&gc) [numSteps])
{
  boolean last = gc[numSteps-1];
  for(int i = numSteps-1; i > 0; i--)
  {
    gc[i] = gc[i-1];
  }
  gc[0] = last;
}

//Continually shifts seed and also modifies it based on the result of XORing the 3rd and 5th elements
//In each cycle, writes the rightmost digit into result
void lsfr(boolean* seed, boolean (&result) [numSteps], int* taps, int taplength)
{
  result[0] = seed[seedSize-1];
  for(int i = 1; i<numSteps; i++)
  {
    boolean first = 0;
    if(x_or(seed, taps, taplength))
      first = 1;
    else  
      first = 0;
    shift(seed);
    seed[0] = first;
    result[i] = seed[seedSize-1];
  }
}

//XOR two gold codes
void joinCodes(boolean* gc1, boolean* gc2, boolean (&result) [numSteps])
{
  for(int i = 0; i < numSteps; i++)
  {
    if(x_or(gc1[i], gc2[i]))
      result[i] = 1;
    else
      result[i] = 0;
  }
}

//Print a gold code
void printCode(boolean* gc)
{
  for(int i = 0; i < numSteps; i++)
  {
    Serial.print(gc[i]);
  }
}

//Take the dot product of two gold codes
int dotProduct(boolean gc1[numSteps], boolean gc2[numSteps])
{
  int dp = 0;
  for(int i = 0; i < numSteps; i++)
  {
    if(gc2[i] == gc1[i])
      dp++;
    else
      dp--;
  }
  return dp;
}

//Copy the elements of one array into another
void copyArray(boolean* source, boolean (&destination) [numSteps])
{
  for(int i = 0; i<numSteps; i++)
  {
    destination[i] = source[i];
  }
}

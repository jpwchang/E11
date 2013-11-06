// ps3_1
//Alex Ozdemir
//aozdemir@hmc.edu
//Generates some Gold Codes
//defines a few "Magic numbers"
#define SEEDLEN 5
#define GCLEN 31
#define NUMCODES 8
//represents the LFSR's as a boolean array representing whether or no the position is tapped
boolean LFSR1[SEEDLEN] = {0,1,1,1,1};
boolean LFSR2[SEEDLEN] = {0,0,1,0,1};
boolean GC[GCLEN];
boolean Seed1[SEEDLEN] = {0,0,0,0,1};
boolean Seeds2[NUMCODES][SEEDLEN] = {{0,0,0,0,1},
                                     {0,0,0,1,0},
                                     {0,0,0,1,1},
                                     {0,0,1,0,0},
                                     {0,0,1,0,1},
                                     {0,0,1,1,0},
                                     {0,0,1,1,1},
                                     {0,1,0,0,0}};
//Creates 2 variables, one for the current state of each LFSR
boolean regState1[SEEDLEN] = {0,0,0,0,0};
boolean regState2[SEEDLEN] = {0,0,0,0,0};
//boolean goldCodes[NUMCODES][GCLEN];

void flashGC()
{
  
}


void geGC()
{
  //Set up serial connection
  Serial.begin(9600);
  //Cycles through each code to be generated
  for (int i = 0; i < NUMCODES; i++)
  {
    //Sets he regist states to the initial values
    seedRegisters(i);
    //Goes through GCLEN times
    for (int j = 0; j < GCLEN; j++)
    {
      //adds the XOR of the last states to the GC
      GC[j] = xOr(regState1[SEEDLEN - 1], regState2[SEEDLEN - 1]);
      //Gets the next states
      getNextState();
    }
    //prints
    Serial.print("Gold Code ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(strGC(GC));
  }
}


void getNextState()
{
  //Updates each LFSR state siultaneously
  //Records the input to the first position, taking into account the current state and the LFSR layout
  boolean newStart1 = (dotProduct(regState1, LFSR1) % 2 == 1);
  boolean newStart2 = (dotProduct(regState2, LFSR2) % 2 == 1);
  //Cycles through backward, not including the first position
  for (int i = (SEEDLEN - 1); i > 0; i--)
  {
    //Takes the value from the last position
    regState1[i] = regState1[i - 1];
    regState2[i] = regState2[i - 1];
  }
  //Sets the first position equal to the previously determined input
  regState1[0] = newStart1;
  regState2[0] = newStart2;
}
  
int dotProduct(boolean *a1, boolean *a2)
{
  //Computes the actual (mathematically defined) dot product of two boolean vectors
  int sum = 0;
  for (int i = 0; i < SEEDLEN; i++)
  {
    if ((a1[i] == 1) && (a2[i] == 1))
    {
      //Increments the product whenever both positions are 1.
      sum ++;
    }
  }
  return sum;
}

boolean xOr(boolean a, boolean b)
{
  //XOR's two booleans
  return ((!(a && b)) && (a || b));
}

void seedRegisters(int seedNum)
{
  //Goes through each position in the LSFSRs (simultaneously)
  for (int i = 0; i < SEEDLEN; i++)
  {
    //Sets them equal to the value in that position of their seeds
    regState1[i] = Seed1[i];
    regState2[i] = Seeds2[seedNum][i];
  }
}

String strGC(boolean *gc)
//Turns a boolean array into a string of 1's and 0's
{
  String result;
  for (int i = 0; i < GCLEN; i++)
  {
    result += String(gc[i]);
  }
  return result;
}

void generateGoldCodes()
{
  //Cycles through each code to be generated
  for (int i = 0; i < NUMCODES; i++)
  {
    //Sets he regist states to the initial values
    seedRegisters(i);
    //Goes through GCLEN times
    for (int j = 0; j < GCLEN; j++)
    {
      //Stores the XOR of the two outputs
      goldCodes[i][j] = xOr(regState1[SEEDLEN - 1], regState2[SEEDLEN - 1]);
      //Gets the next states
      getNextState();
    }
  }
}

int correlate(boolean *gc1, boolean *gc2)
{
  //Correlation starts at 0
  int correlation = 0;
  //goes through each element of the array
  for (int i = 0; i < GCLEN; i++)
  {
    if (gc1[i] == gc2[i])//If the match it icremements
    {
      correlation ++;
    }
    else//Otherwise decrements
    {
      correlation --;
    }
  }
  return correlation;
}

//return 1 if positive correlation, -1 if negative correlation,
//0 otherwise
int corGoldCodes(boolean *gc1, boolean *gc2)
{
  // Sets the maximum cor to -31 (min possible)
  int maximum = 0;
  boolean gc3[GCLEN];
  boolean last;
  for (int i = 0; i < GCLEN; i++)
  {
    gc3[i] = gc2[i];
  }
  
  //For every shift amount
  for (int m = 0; m < GCLEN; m++)
  {
    int correlation = correlate(gc1,gc3);
    //Replace maximum if necessary
    if (abs(correlation) > abs(maximum))
    {
      maximum = correlation;
    }
    //Shifts for nxt cycle
    last = gc3[GCLEN-1];
    for (int i = GCLEN-1; i > 0; i--)
    {
      gc3[i] = gc3[i-1];
    }
    gc3[0] = last;
  }
  return maximum;
}        

void shiftGC()
{
  boolean wrap = GC[GCLEN - 1];
  for (int i = GCLEN - 1; i > 0; i--)
  {
    GC[i] = GC[i - 1];
  }
  GC[0] = wrap;
}

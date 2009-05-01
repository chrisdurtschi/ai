/*  a simple Genetic Algorithm test program

      designed by:  Dr. Ron Peterson
      for:  COMSCI 452 class
      date:  26 May 1992, revised 16 May 1995
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/* General parameters
       maximum population, characters per genetic code,
       probability per individual of:  cross-over, mutation
 */
#define MaxPop 100
#define MaxPop1 (MaxPop+1)
#define CodeSize 12
#define DefProbCross 0.10
#define DefProbMut 0.05

/* Calculational functions */
       /* random number from 0 to x, (int or float) */
float FRandom(float x)
{
  return (rand()*x/RAND_MAX);
}
int IRandom(int x)
{
  return (int)(rand()*(float)x/RAND_MAX);
}

/* Global data types */

typedef int CodeType[CodeSize];
typedef struct
{  
  CodeType Code;
  float Good;
} Indiv;
typedef Indiv PopType[MaxPop1];

/* Global data objects */

PopType Pop, NewPop;  /* lists of the population of individuals */
float TotGood;        /* the total goodness (survivability) values */
int CodeSchema[CodeSize] = {2,2,2,2,2,2,2,2,2,2,2,2};
                      /* the number of values (0 to n-1) each code position
                              can take on -- just 0,1 in this case */
int NTop = 8;         /* number of genotypes to include in output */
float ProbCross = DefProbCross;
float ProbMut = DefProbMut;

/* Global function declarations */

void InitPop(void), Repro(void), Cross(void), Mutate(void), Test(void);
void Top(int N);
int PrCode(CodeType Code);
int More(void), SetParams(void);
float WholeFit(CodeType Code);
float fitness(int arm);
void strreverse(char* begin, char* end);
void itoa(int value, char* str, int base);

/* the main program: set up the population and loop through evolutionary
       cycles (generations) as controlled by the user */
main()
{
  InitPop();  /* initialize the population */

  do    /* for each evolutionary cycle */
  {
    /* reproduce, cross-over, and mutate each cycle */
    Repro();
    Cross();
    Mutate();

    /* test each individual for survivability, update goodness variables,
       and print statistics for the top genotypes */
    Test();
    Top(NTop);
  } while (More());

  return 0;
}

/* create an initial population */
void InitPop()
{
  int i, l;

  for (i=0; i<MaxPop; i++)  /* for each individual in the population */
  {
    for (l=0; l<CodeSize; l++)  /* select random digit for each locus */
      Pop[i].Code[l] = IRandom(CodeSchema[l]);
  }
  
  Test();  /* get the goodness data */
  Pop[MaxPop].Good = -999.0; /* --dummy value to support search for best */
}

/* reproduce the next generation based on survivablility */
void Repro()
{
  int i, j;
  float Cut;  /* the point in total goodness where individual is selected */
  float Tot;  /* cumulative goodness */

  for (i=0; i<MaxPop; i++)  /* for each individual in the new population */
  {
    Cut = FRandom(TotGood);

    for (j=0, Tot=0; j<MaxPop; j++) /* search old ones to locate cut */
    {
      Tot += Pop[j].Good;
      
      if (Cut < Tot)
      {
        NewPop[i] = Pop[j];      /* select individual for new group */
        break;
      }
      
      NewPop[i] = Pop[MaxPop];  /* only if cut bigger than total ??? */
    }
  }
  
  for (i=0; i<MaxPop; i++)  /* copy each individual in the population */
    Pop[i] = NewPop[i];
}/* function to handle cross-overs in the current generation */

void Cross()
{
  int i, j, l, loc, temp;
  int NCross = 0;

  for (i=0; i<MaxPop; i++)  /* for each individual in the population */
  {
    if (FRandom(1.0) > ProbCross) continue;  /* skip unless within prob'y */

    NCross++;
    do
      j = IRandom(MaxPop);
    while (j == i);  /* select random mate (not with itself) */

    loc = IRandom(CodeSize - 2) + 1; /* cross-over location 1 to size-1 */
    for (l=loc; l<CodeSize; l++)  /* for loci at loc and beyond, exchange */
    {
      temp = Pop[i].Code[l];
      Pop[i].Code[l] = Pop[j].Code[l];
      Pop[j].Code[l] = temp;
    }
  }
  
  if (NTop)   /* print unless NTop is 0 */
    printf( "\n There were %d crossovers. \n",NCross);
}


/* perform mutations for this generation */
void Mutate()
{
  int i, l;
  int NMut = 0;

  for (i=0; i<MaxPop; i++)  /* for each individual in the population */
  {
    if (FRandom(1.0) > ProbMut) continue;  /* skip unless within prob'y */
      NMut++;

    l = IRandom(CodeSize);  /* select locus to mutate */
    Pop[i].Code[l] = (Pop[i].Code[l]
    +IRandom(CodeSchema[l]-2)+1)%CodeSchema[l]; /* different value */
  }
  if (NTop)   /* print unless NTop is 0 */
    printf( "\n There were %d mutations. \n", NMut);
}


/* test the new population against the fitness function */
void Test()
{
   int i;

   TotGood = 0.0;
   for (i=0; i<MaxPop; i++)
   {
      Pop[i].Good = WholeFit(Pop[i].Code);
      TotGood += Pop[i].Good;
   }
}

/* build the fitness for the whole code of one individual */
float WholeFit(CodeType Code)
{
  int l;
  float Tot = 0.0;

  /* 100110011001 */
  for (l=0; l<CodeSize; l++)
    //Tot += fitness(Code[l]);
    if (((l % 4 == 0 || l % 4 == 3) && Code[l] == 1.0) || ((l % 4 == 1 || l % 4 == 2) && Code[l] == 0.0))
      Tot += 1.0; 

  return Tot;
}


/* print out the stats on the top genomes (codes) */
/* start with two help functions for Code manipulation */
void CodeCopy(CodeType c1, CodeType c2)
{  
  int l;
  for (l=0; l<CodeSize; l++)
    c1[l] = c2[l];
}

int CodeMatch(CodeType c1, CodeType c2)
{  
  int l;
  for (l=0; l<CodeSize; l++)
    if (c1[l] != c2[l]) return 0;
  return 1;
}
      
void Top(int N)
{
  int t, i, j, count;
  int Best;
  CodeType BCode;
  int Avail[MaxPop];
  float Tot;

  for (i=0; i<MaxPop; i++)  /* mark all individuals as available */
    Avail[i] = 1;

  if(N)
    printf("\n # of Indivs   ----Code----   Max. Fitness   Avg. Fitness\n");
    
  for (t=0; t<N; t++)  /* for the top N codes */
  {
    for (i=0, Best=MaxPop; i<MaxPop; i++)  /* find the best code left */
      if (Pop[i].Good > Pop[Best].Good && Avail[i])
        Best = i;

    CodeCopy(BCode, Pop[Best].Code);
    count = 0;
    Tot = 0.0;
    
    for (i=0; i<MaxPop; i++)  /* identify individuals with that code */
      if (CodeMatch(Pop[i].Code,BCode) && Avail[i])
      {
        Avail[i] = 0;
        count++;
        Tot += Pop[i].Good;
      }
    
    if (count == 0) break;  /* --stop if you run out of genomes */

    /* print the results */
    printf(" %10d    ",count);
    PrCode(BCode);
    printf("  %11.2f    %11.2f \n",
    Pop[Best].Good, Tot/count);
  }
  
  if (N != 0)
    printf(" ----------------------------------------------\n");
  else
    printf(".");
}

/* print out one code */
int PrCode(CodeType Code)
{
   int l;
   char st[3];

   for (l=0; l<CodeSize; l++)
   {
     itoa(Code[l],st,10);   /* string together the individual digits */
     printf("%c", st[strlen(st)-1]);
   }
   return 0;
}


/* function to control cycling of the generation loop */
int More()
{
  static int MoreCount = 0;
  char ch;

  if (--MoreCount > 0) return 1;  /* ride out previously set count */

  printf("\nEnter: c -- continue more cycles       s -- stop\n");
  printf(  "       r -- reset parameters           n -- next cycle \n\n");

  do
  {
    printf("--Choice:\n");
    while (!isalpha(ch=getchar())) ;
    switch (ch)
    {
      case 'r':
      case 'R':   SetParams();  /* set parameters (then get cycle count) */

      case 'c':
      case 'C':   printf(" Enter number of additional cycles.\n");
      scanf("%d",&MoreCount);
      return 1;
      case 's':
      case 'S':   return 0;

      case 'n':
      case 'N':   return 1;

      default:  ch = 'x';
    }
  }
  
  while (ch == 'x');
    return 0;
}

/* change some control parameters */
int SetParams()
{
  int n;

  do
  {  
    printf(" Which parameter:  1) NTop  2) ProbCross  3) ProbMut  4) done \n");
    scanf("%d",&n);
    switch (n)
    {
      case 1: printf("Current value for NTop is %d\n",NTop);
        printf("New integer value for NTop:\n");
        scanf("%d",&NTop);
        break;
      case 2: printf("Current value for ProbCross is %5.2f\n",ProbCross);
        printf("New probability value (0.00 to 1.00) for ProbCross:\n");
        scanf("%f",&ProbCross);
        break;
      case 3: printf("Current value for ProbMut is %5.2f\n",ProbMut);
        printf("New probability value (0.00 to 1.00) for ProbMut:\n");
        scanf("%f",&ProbMut);
        break;
      case 4: break;
      default: printf("Invalid choice; re-enter a digit 1 to 4.\n");
    }
  }
  while (n != 4);

  return 0;
}
   

/* ------------------------------------------------------------
    a sample fitness function for the 2-armed bandit problem
   ------------------------------------------------------------ */
float fitness(int arm)
{
  float prob[2] = {0.25,0.75};

  if (FRandom(1.0) < prob[arm])
    return 1.0;
  else
    return 0.0;
}

void strreverse(char* begin, char* end) {
  char aux;

  while(end > begin)
   aux=*end, *end--=*begin, *begin++=aux;
}
	
void itoa(int value, char* str, int base)
{ 
  static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";

  char* wstr=str;

  int sign;

  // Validate base

  if (base<2 || base>35){ *wstr='\0'; return; }

  // Take care of sign

  if ((sign=value) < 0) value = -value;

  // Conversion. Number is reversed.

  do *wstr++ = num[value%base]; while(value/=base);

  if(sign<0) *wstr++='-';
    *wstr='\0';

  // Reverse string

  strreverse(str,wstr-1);	
}



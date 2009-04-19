/*

       This file is part of the PDP software package.
		 
       Copyright 1987 by James L. McClelland and David E. Rumelhart.
       
       Please refer to licensing information in the file license.txt,
       which is in the same directory with this source file and is
       included here by reference.
*/


/* file: bp.h

	Header file for the bp program.
	
	First version implemented by Elliot Jaffe.
	
	Date of last revision:  8-12-87/JLM.
*/

#include <math.h>

extern char *prompt;
extern  boolean System_defined;
extern int  epoch;
extern float   *netinput;
extern float   *output;
extern float   *delta;
extern float   *target;
extern float    momentum;	/* momentum constant */
extern float    tmax;		/* maximum possible target value */


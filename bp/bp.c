/*

       This file is part of the PDP software package.
		 
       Copyright 1987 by James L. McClelland and David E. Rumelhart.
       
       Please refer to licensing information in the file license.txt,
       which is in the same directory with this source file and is
       included here by reference.
*/


/* file: bp.c

	Do the actual work for the bp program.
	
	First version implemented by Elliot Jaffe
	
	Date of last revision:  8-12-87/JLM
*/

#include "general.h"
#include "bp.h"
#include "variable.h"
#include "weights.h"
#include "patterns.h"
#include "command.h"


char   *Prompt = "bp: ";
char   *Default_step_string = "epoch";
char	grain_string[20] = "pattern";
boolean System_Defined = FALSE;
boolean lflag = 1;
boolean cascade = 0;
int     epochno = 0;
int     cycleno = 0;
int     nepochs = 500;
int	ncycles = 50;
int     patno = 0;
float   tss = 0.0;
float   pss = 0.0;
float   ecrit = 0.0;
float	crate = .05;
float	drate = .95;
float	gcor = 0.0;
int	follow = 0;
float  *netinput = NULL;
float  *activation = NULL;
float  *error = NULL;
float  *target = NULL;
float  *delta = NULL;
float  **dweight = NULL;
float  **pwed = NULL;
float  *dbias = NULL;
float  *pbed = NULL;
float   tmax = 1.0;
float   momentum = 0.9;
float	mu = .5;
int	tallflag = 0;

extern int read_weights();
extern int write_weights();

init_system() {
    int     strain (), ptrain (), tall (), test_pattern (), reset_weights();
    int	    get_unames(), set_lgrain(), cycle(), newstart();
    int change_lrate(), change_crate(), set_follow_mode();

    epsilon_menu = SETCONFMENU;

    init_weights();

    install_command("strain", strain, BASEMENU,(int *) NULL);
    install_command("ptrain", ptrain, BASEMENU,(int *) NULL);
    install_command("tall", tall, BASEMENU,(int *) NULL);
    install_command("test", test_pattern, BASEMENU,(int *) NULL);
    install_command("cycle", cycle, BASEMENU,(int *) NULL);
    install_command("reset",reset_weights,BASEMENU,(int *)NULL);
    install_command("newstart",newstart,BASEMENU,(int *)NULL);
    install_command("unames", get_unames, GETMENU,(int *) NULL);
    install_command("patterns", get_pattern_pairs, 
			   			GETMENU,(int *) NULL);
    install_var("lflag", Int,(int *) & lflag, 0, 0, SETPCMENU);
    install_var("lgrain", String, (int *) grain_string,0, 0,NOMENU);
    install_command("lgrain",set_lgrain,SETMODEMENU,(int *) NULL);
    install_var("follow", Int, (int *) & follow,0, 0,NOMENU);
    install_command("follow",set_follow_mode,SETMODEMENU,(int *) NULL);
    install_var("cascade", Int,(int *) & cascade, 0, 0, SETMODEMENU);
    install_var("nepochs", Int,(int *) & nepochs, 0, 0, SETPCMENU);
    install_var("ncycles", Int,(int *) & ncycles, 0, 0, SETPCMENU);
    install_var("epochno", Int,(int *) & epochno, 0, 0, SETSVMENU);
    install_var("patno", Int,(int *) & patno, 0, 0, SETSVMENU);
    install_var("cycleno", Int,(int *) & cycleno, 0, 0, SETSVMENU);
    init_pattern_pairs();
    install_var("tss", Float,(int *) & tss, 0, 0, SETSVMENU);
    install_var("pss", Float,(int *) & pss, 0, 0, SETSVMENU);
    install_var("gcor", Float,(int *) & gcor, 0, 0, SETSVMENU);
    install_var("momentum", Float,(int *) &momentum,0,0,SETPARAMMENU);
    install_var("mu", Float,(int *) &mu,0,0,SETPARAMMENU);
    install_command("lrate", change_lrate, SETPARAMMENU, (int *) NULL);
    install_command("crate", change_crate, SETPARAMMENU, (int *) NULL);
    install_var("lrate", Float,(int *) & lrate, 0, 0, NOMENU);
    install_var("crate", Float,(int *) & crate, 0, 0, NOMENU);
    install_var("ecrit", Float,(int *) & ecrit, 0, 0, SETPCMENU);
    install_var("tmax", Float,(int *) & tmax, 0, 0, SETPARAMMENU);
}


define_system() {
    register int    i,
                    j;
    float *tmp;

    if (!nunits) {
	put_error("cannot init bp system, nunits not defined");
	return(FALSE);
    }
    else
	if (!noutputs) {
	    put_error("cannot init bp system, noutputs not defined");
	    return(FALSE);
	}
    else
	if (!ninputs) {
	    put_error("cannot init bp system, ninputs not defined");
	    return(FALSE);
	}
    else
	if (!(nunits && noutputs && ninputs)) {
	    put_error("cannot run bp system, nunits not defined");
	    return(FALSE);
	}

    netinput = (float *) emalloc((unsigned)(sizeof(float) * nunits));
    install_var("netinput", Vfloat,(int *) netinput, nunits, 0, SETSVMENU);
    for (i = 0; i < nunits; i++)
	netinput[i] = 0.0;

    activation = (float *) emalloc((unsigned)(sizeof(float) * nunits));
    install_var("activation",Vfloat,(int *)activation,nunits,0,SETSVMENU);
    for (i = 0; i < nunits; i++)
	activation[i] = 0.0;

    delta = (float *) emalloc((unsigned)(sizeof(float) * nunits));
    install_var("delta", Vfloat,(int *) delta, nunits, 0, SETSVMENU);
    for (i = 0; i < nunits; i++)
	delta[i] = 0.0;

    error = (float *) emalloc((unsigned)(sizeof(float) * nunits));
    install_var("error", Vfloat,(int *) error, nunits, 0, SETSVMENU);
    for (i = 0; i < nunits; i++)
	error[i] = 0.0;

    target = (float *) emalloc((unsigned)(sizeof(float) * noutputs));
    install_var("target", Vfloat,(int *) target, noutputs,
		0, SETSVMENU);
    for (i = 0; i < noutputs; i++)
	target[i] = 0.0;

    dweight = ((float **)
	   emalloc((unsigned)(sizeof(float *)*nunits)));
    install_var("dweight", PVweight,(int *) dweight, nunits,
					nunits, SETSVMENU);
    
    for (i = 0; i < nunits; i++) {
	dweight[i] = ((float *)
	     emalloc((unsigned)(sizeof(float)*num_weights_to[i])));
	for (j = 0; j < num_weights_to[i]; j++){
	    dweight[i][j] = 0.0;
	}
    }
    dbias = (float *) emalloc((unsigned)(sizeof(float) * nunits));
    install_var("dbias", Vfloat,(int *) dbias,
					nunits, 0, SETSVMENU);
    for (i = 0; i < nunits; i++)
	dbias[i] = 0.0;
/*  now being done in weights.c
    wed = ((float **) emalloc((unsigned)(sizeof(float *)*nunits)));
    install_var("wed", PVweight,(int *) wed, nunits,
					nunits, SETSVMENU);
    for (i = 0; i < nunits; i++) {
	wed[i] = ((float *)
	      emalloc((unsigned)(sizeof(float)*num_weights_to[i])));
	for (j = 0; j < num_weights_to[i]; j++) {
	    wed[i][j] = 0.0;
	}
    }

    bed = (float *) emalloc((unsigned)(sizeof(float) * nunits));
    install_var("bed", Vfloat,(int *) bed,
					nunits, 0, SETSVMENU);
    for (i = 0; i < nunits; i++)
	bed[i] = 0.0;
*/
    System_Defined = TRUE;
    return(TRUE);
}


float  logistic (x)
float  x;
{
    double  exp ();

#ifdef MSDOS    
/* we are conservative under msdos to avoid potential underflow
   problems that may arise from returning extremal values -- jlm */
    if (x > 11.5129)
	return(.99999);
      else
	if (x < -11.5129)
	    return(.00001);
#else
/* .99999988 is very close to the largest single precis value
   that is resolvably less than 1.0 -- jlm */
      if (x > 15.935773)
	return(.99999988);
      else
	if (x < -15.935773)
	    return(.00000012);
#endif
    else
       return(1.0 / (1.0 + (float) exp( (double) ((-1.0) * x))));
}

init_output() {
    register int i,j;
    register float *sender, *wt, *end;
    float net;
    
    /* initializes the network to asymptotic outputs given 0 input */
    cycleno = 0;
    
    for (i = ninputs; i < nunits; i++) {/* to this unit */
	net = bias[i];
	sender = &activation[first_weight_to[i]];
	wt = weight[i];
	end = sender + num_weights_to[i];
	for (j = first_weight_to[i]; j < ninputs; j++) {
		sender++; wt++; /* step over input units to 
				   initialize to all-zero input case */
	}
	for (; sender < end ; ){/* from this unit */
	    net += (*sender++) * (*wt++);
	}
	netinput[i] = net;
	activation[i] = (float) logistic(net);
    }
    if (step_size < PATTERN) {
	update_display();
	if (single_flag) {
	    if (contin_test() == BREAK) return (BREAK);
	}
    }
    if (Interrupt) {
	    Interrupt_flag = 0;
	    update_display();
	    if (contin_test() == BREAK) return (BREAK);
    }
    return(CONTINUE);
}

cycle() {
    register int i,cy;
    register float *sender,*wt,*end;
    float newinput;

    for (cy = 0; cy < ncycles; cy++) {
	cycleno++;
	for (i = ninputs; i < nunits; i++) {/* to this unit */
	    newinput = bias[i];
	    sender = &activation[first_weight_to[i]];
	    end = sender + num_weights_to[i];
	    wt = weight[i];
	    for (;sender<end;) {/* from this unit */
		newinput += (*sender++) * (*wt++);
	    }
	    netinput[i] = crate * newinput + drate * netinput[i];
	    activation[i] = (float) logistic(netinput[i]);
	}
	if (step_size == CYCLE) {
	    update_display();
	    if (single_flag) {
		if (contin_test() == BREAK) return (BREAK);
	    }
	}
	if (Interrupt) {
	    update_display();
	    Interrupt_flag = 0;
	    if (contin_test() == BREAK) return (BREAK);
	}
    }
    if (step_size == NCYCLES) {
	update_display();
    }
    return(CONTINUE);
}
	

compute_output() {
    register int    i,j;
    float *sender, *wt, *end;
    float net;

    for (i = ninputs; i < nunits; i++) {/* to this unit */
	net = bias[i];
	sender = &activation[first_weight_to[i]];
        end = sender + num_weights_to[i];
	wt = weight[i];
	for (; sender < end ;){/* from this unit */
	    net += (*sender++)*(*wt++);
	}
	netinput[i] = net;
	activation[i] = (float) logistic(net);
    }
}

compute_error() {
    register int i,j,first,num;
    float *wt, *sender, *end;
    float del;

    for (i = ninputs; i < nunits - noutputs; i++) {
	error[i] = 0.0;
    }

    for (j = 0; i < nunits; j++, i++) {
	if(target[j] >= 0)   /* We care about this one */
	     error[i] = target[j] - activation[i];
	else
	     error[i] = 0.0;
    }

    for (i = nunits - 1; i >= ninputs; i--) {
	del = delta[i] = error[i] * activation[i] * (1.0 - activation[i]);
	if (first_weight_to[i] + num_weights_to[i] < ninputs) continue;
	/* no point in propagating error back to input units */
	sender = &error[first_weight_to[i]];
	end = sender + 	num_weights_to[i];
	wt = weight[i];
	for (;sender < end;) {
	    *sender++ += del * (*wt++);
	}
    }
}

compute_wed() {
    register int   i,j;
    float *wi, *sender, *end;
    float del;

    for (i = ninputs; i < nunits; i++) {
	bed[i] += delta[i];
	sender = &activation[first_weight_to[i]];
	end = sender + num_weights_to[i];
	del = delta[i];
	wi = wed[i];
	for (;sender < end;) {
	    *wi++ += del * (*sender++);
	}
    }
}

clear_wed() {
    register int   i,j,num;
    register float *wi, *end;

    for (i = ninputs; i < nunits; i++) {
	bed[i] = 0.0;
	wi = wed[i];
	end = wi + num_weights_to[i];
	for (; wi < end;) {
	    *wi++ = 0.0;
	}
    }
}

change_weights() {
    register int    i;
    register float *wt, *dwt, *epi, *wi, *end;
    
    link_sum();
    
    for (i = ninputs; i < nunits; i++) {
	dbias[i] = bepsilon[i]*bed[i] + momentum * dbias[i];
	bias[i] += dbias[i];
	bed[i] = 0.0;
	wt = weight[i];
	dwt= dweight[i];
	wi = wed[i];
	epi = epsilon[i];
	end = wt + num_weights_to[i];
	for (; wt < end; ) {
	    *dwt = (*epi++)*(*wi) + momentum * (*dwt);
	    *wt++ += *dwt++;
	    *wi++ = 0.0;
	}
    }
    pos_neg_constraints();
}

float p_css = (float) 0.0;
float css = (float) 0.0;

change_weights_follow() {
    register int    i;
    register float *wt, *dwt, *epi, *wi, *end, *pwi;
    float tb, dp, den;

    p_css = css;
    css = 0.0;
    dp = 0.0;
    
    link_sum();

    for (i = ninputs; i < nunits; i++) {
        tb = bed[i];
	dbias[i] = tb*bepsilon[i] + momentum * dbias[i];
	bias[i] += dbias[i];
	css += ((double) tb)*((double) tb); 
	dp += ((double) tb)*((double) pbed[i]); 
	pbed[i] = tb;
	bed[i] = 0.0;
	wt = weight[i];
	dwt= dweight[i];
	wi = wed[i];
	pwi = pwed[i];
	epi = epsilon[i];
	end = wt + num_weights_to[i];
	for (; wt < end; ) {
	    *dwt = (*epi++)*(*wi) + momentum * (*dwt);
	    *wt++ += *dwt++;
	    css += ((double) (*wi))*((double) (*wi)); 
 	    dp += ((double) (*wi))*((double) (*pwi)); 
	    *pwi++ = *wi;
	    *wi++ = 0.0;
	}
    }
    
    den = p_css * css;
    if (den > 0.0) gcor = dp/(sqrt(den));
    else gcor = 0.0;

    pos_neg_constraints();
}

constrain_weights() {
    pos_neg_constraints();
    link_constraints();
}

pos_neg_constraints() {
    float **fpt;

    for (fpt = positive_constraints; fpt && *fpt; fpt++)
	if (**fpt < 0.0)
	    **fpt = 0.0;

    for (fpt = negative_constraints; fpt && *fpt; fpt++)
	if (**fpt > 0.0)
	    **fpt = 0.0;
}

link_constraints() {
    register int    i,j;
    float   t;

    for (i = 0; i < nlinks; i++) {
	t = *constraints[i].cvec[0];
	for (j = 1; j < constraints[i].num; j++) {
	    *constraints[i].cvec[j] = t;
	}
    }
}

link_sum() {
    register int    i,j;
    float   ss;

    for (i = 0; i < nlinks; i++) {
	ss = 0.0;
	for (j = 0; j < constraints[i].num; j++) {
	    ss += *constraints[i].ivec[j];
	}
	for (j = 0; j < constraints[i].num; j++) {
	    *constraints[i].ivec[j] = ss;
	}
    }
}

setinput() {
    register int    i,prev_index;
    register float  *pp;

    for (i = 0, pp = ipattern[patno]; i < ninputs; i++, pp++) {
	if ( *pp < 0.0) {
	    prev_index = ((int) (-(*pp)));
	    activation[i] = mu * activation[i] + activation[prev_index];
	    /* user must be careful that prev_index >= i */
	}
	else {
	    activation[i] = *pp;
	}
    }
    
    strcpy(cpname,pname[patno]);
}

settarget() {
    register int    i;
    register float *pp;

    for (i = 0, pp = tpattern[patno]; i < noutputs; i++, pp++) {
	target[i] = *pp;
	if (target[i] == 1.0) {
	    target[i] = tmax;
	}
	else if(target[i] == 0.0) {
	        target[i] = 1 - tmax;
	}
    }
}

setup_pattern() {
    setinput();
    settarget();
}

trial() {
    setup_pattern();
    if (cascade) {
	if (init_output() == BREAK) return (BREAK);
	if (cycle() == BREAK) return (BREAK);
    }
    else  {	
        compute_output();
        if (step_size < PATTERN) {
	      update_display();
	      if (single_flag) {
	          if (contin_test() == BREAK) return(BREAK);
	      }
	}
    }
    compute_error();
    sumstats();
    return (CONTINUE);
}

sumstats() {
    register int    i,j;
    register float t;
    pss = 0.0;
    

    for (j = 0,i = nunits - noutputs; i < nunits; i++,j++) {
      if (target[j] >= 0) {
      	t = error[i];
	pss += t*t;
      }
    }
    tss += pss;
}

ptrain() {
  return(train('p'));
}

strain() {
  return(train('s'));
}

train(c) char c; {
    int     t,i,old,npat;
    char    *str;

    if (!System_Defined)
	if (!define_system())
	    return(BREAK);

    /* in case prev epoch was terminated early we clear the weds and beds */
    if (!tallflag) clear_wed();
    cycleno = 0;
    for (t = 0; t < nepochs; t++) {
	if (!tallflag) epochno++;
	for (i = 0; i < npatterns; i++)
	    used[i] = i;
	if (c == 'p') {
	  for (i = 0; i < npatterns; i++) {
	    npat = rnd() * (npatterns - i) + i;
	    old = used[i];
	    used[i] = used[npat];
	    used[npat] = old;
	  }
	}
	tss = 0.0;
	for (i = 0; i < npatterns; i++) {
	    patno = used[i];
	    if (trial() == BREAK) return (BREAK);
	    if (lflag) {
	      compute_wed();
	      if (grain_string[0] == 'p') {
	        if (follow) change_weights_follow();
	        else change_weights();
	      }
	    }
	    if (step_size == PATTERN) {
	      update_display();
	      if (single_flag) {
	          if (contin_test() == BREAK) return(BREAK);
	      }
	    }
	    if (Interrupt) {
		Interrupt_flag = 0;
		update_display();
		if (contin_test() == BREAK) return(BREAK);
	    }
	}
	if (lflag && grain_string[0] == 'e') {
	  if (follow) change_weights_follow();
	  else change_weights();
	}
	if (step_size == EPOCH) {
	 update_display();
	 if (single_flag) {
	          if (contin_test() == BREAK) return(BREAK);
	 }
        }
	if (tss < ecrit) break;
    }
    if (step_size == NEPOCHS) {
      update_display();
    }
    return(CONTINUE);
}

tall() {
  int save_lflag;
  int save_single_flag;
  int save_nepochs;
  int save_step_size;
  
  save_lflag = lflag;  lflag = 0;
  save_single_flag = single_flag; 
  if (in_stream == stdin) single_flag = 1;
  save_step_size = step_size; 
  if (step_size > PATTERN) step_size = PATTERN;
  save_nepochs = nepochs;  nepochs = 1;
  tallflag = 1;
  train('s');
  tallflag = 0;
  lflag = save_lflag;
  nepochs = save_nepochs;
  single_flag = save_single_flag;
  step_size = save_step_size;
  return(CONTINUE);
}
  
test_pattern() {
    char   *str;
    int save_single_flag;
    int save_step_size;

    if (!System_Defined)
	if (!define_system())
	    return(BREAK);

    tss = 0.0;

    str = get_command("Test which pattern? ");
    if(str == NULL) return(CONTINUE);
    if ((patno = get_pattern_number(str)) < 0) {
       return(put_error("Invalid pattern specification."));
    }
    if (cascade) {
        save_single_flag = single_flag; single_flag = 1;
	save_step_size = step_size; step_size = CYCLE;
    }
    trial();
    update_display();
    if (cascade) {
	single_flag = save_single_flag;
	step_size = save_step_size;
    }
    return(CONTINUE);
}

newstart() {
	random_seed = rand();
	reset_weights();
}

reset_weights() {
    register int    i,j,first,num;
    char ch;
    
    epochno = 0;
    pss = tss = gcor = 0.0;
    cpname[0] = '\0';
    srand(random_seed);

    if (!System_Defined)
	if (!define_system())
	    return(BREAK);

    for (j = 0; j < nunits; j++) {
	first = first_weight_to[j];
	num = num_weights_to[j];
      for (i = 0; i < num; i++) {
	wed[j][i] = dweight[j][i] = 0.0;
	if (pwed) pwed[j][i] = 0.0;
	ch = wchar[j][i];
	if (isupper(ch)) ch = tolower(ch);
	if (ch == '.') {
	    weight[j][i] = 0.0;	    
	}
	else {
	    if (constants[ch - 'a'].random) {
		    if (constants[ch - 'a'].positive) {
			weight[j][i] = wrange * rnd();
		    }
		    else
			if (constants[ch - 'a'].negative) {
			    weight[j][i] = wrange * (rnd() - 1);
			}
		    else
			weight[j][i] = wrange * (rnd() -.5);
	    }
	    else {
		    weight[j][i] = constants[ch - 'a'].value;
	    }
	}
      }
      bed[j] = dbias[j] = 0.0;
      if (pbed) pbed[j] = 0.0;
      ch = bchar[j];
      if (isupper(ch)) ch = tolower(ch);
      if (ch == '.') {
	    bias[j] = 0;
      }
      else {
	    if (constants[ch - 'a'].random) {
		    if (constants[ch - 'a'].positive) {
			bias[j] = wrange * rnd();
		    }
		    else
			if (constants[ch - 'a'].negative) {
			    bias[j] = wrange * (rnd() - 1);
			}
		    else
			bias[j] = wrange * (rnd() -.5);
	    }
	    else {
		    bias[j] = constants[ch - 'a'].value;
	    }
      }
    }
    constrain_weights();
    for (i = 0; i < noutputs; i++) {
      target[i] = 0.0;
    }
    for (i = 0; i < nunits; i++) {
      netinput[i] = activation[i] = delta[i] = error[i] = 0.0;
    }
    update_display();
    return(CONTINUE);
}

set_lgrain() {
    char old_grain_string[STRINGLENGTH];
    struct Variable *vp, *lookup_var();
    
    strcpy(old_grain_string,grain_string);

    vp = lookup_var("lgrain");
    change_variable("lgrain",vp);
    
    if (startsame(grain_string,"epoch")) strcpy(grain_string,"epoch");
    else if (startsame(grain_string,"pattern"))
    					strcpy(grain_string,"pattern");
    else {
	strcpy(grain_string,old_grain_string);
    	return(put_error("urecognized grain -- not changed."));
    }
    return(CONTINUE);
}

set_follow_mode() {
    struct Variable *vp, *lookup_var();
    int pv, i, j;
    pv = follow;
    
    vp = lookup_var("follow");
    change_variable("follow",vp);
    
    if (follow == 0) return (CONTINUE);
    if (pwed == NULL) {
      pwed = ((float **) emalloc((unsigned)(sizeof(float *)*nunits)));
      install_var("pwed", PVweight,(int *) pwed, nunits,
					nunits, NOMENU);
      for (i = 0; i < nunits; i++) {
	pwed[i] = ((float *)
	      emalloc((unsigned)(sizeof(float)*num_weights_to[i])));
      }

      pbed = ((float *) emalloc((unsigned)(sizeof(float) * nunits)));
      install_var("pbed", Vfloat,(int *) pbed,
					nunits, 0, NOMENU);
    }
    if (pv == 0) {
      for (i = 0; i < nunits; i++) {
	for (j = 0; j < num_weights_to[i]; j++) {
	    pwed[i][j] = 0.0;
	}
      }
      for (i = 0; i < nunits; i++)
	pbed[i] = 0.0;
    }
    gcor = css = 0.0;
    return(CONTINUE);
}

change_crate() {
    struct Variable *varp;

    if ((varp = lookup_var("crate")) != NULL) {
	change_variable("crate",(int *) varp);
    }
    else {
	return(put_error("crate is not defined"));
    }
    drate = 1 - crate;
    return(CONTINUE);
}

init_weights() {
    int define_bp_network();
    install_command("network", define_bp_network,GETMENU,(int *) NULL);
    install_command("weights", read_weights, GETMENU,(int *) NULL);
    install_command("weights", write_weights, SAVEMENU,(int *) NULL);
    install_var("nunits", Int,(int *) & nunits, 0, 0, SETCONFMENU);
    install_var("ninputs", Int,(int *) & ninputs, 0, 0, SETCONFMENU);
    install_var("noutputs", Int,(int *) & noutputs, 0, 0, SETCONFMENU);
    install_var("wrange",Float,(int *) &wrange,0,0, SETPARAMMENU);
}


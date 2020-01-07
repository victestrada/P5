#include <iostream>
#include <math.h>
#include "seno.h"
#include "keyvalue.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

InstrumentSIN::InstrumentSIN(const std::string &param) 
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  /*
    You can use the class keyvalue to parse "param" and configure your instrument.
    Take a Look at keyvalue.h    
  */
  KeyValue kv(param);
  int N;
FILE * f = fopen("tblfile.log","a");
  if (!kv.to_int("N",N))
    N = 40; //default value

  //Create a tbl with one period of a sinusoidal wave
  tbl.resize(N);
  float phase = 0, step = 2 * M_PI /(float) N;
  index = 0;
  for (int i=0; i < N ; ++i) {
    tbl[i] = sin(phase);
    fprintf(f,"%f\n",tbl[i]);
    phase += step;
  }
  fclose(f);
}


void InstrumentSIN::command(long cmd, long note, long vel) {

f0 = 440*pow(2,(note - 69.)/12);

  if (cmd == 9) {		//'Key' pressed: attack begins
    bActive = true;
    adsr.start();
    index = 0;
	a = 0;
	inc = ((f0 / SamplingRate) * tbl.size());
  
	A = vel / 127.;
	a = 0;
  }
  else if (cmd == 8) {	//'Key' released: sustain ends, release begins
    adsr.stop();
  }
  else if (cmd == 0) {	//Sound extinguished without waiting for release to end
    adsr.end();
  }
}


const vector<float> & InstrumentSIN::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;
FILE * fp;
fp = fopen("xvector.log","a");

  for (unsigned int i=0; i<x.size(); ++i) {

	a = a + inc;
//Sin interpolación

	x[i] = A * tbl[round(a)];

//Con interpolación 
//x[i] =tbl[floor(a)]+(a-floor(a))*(tbl[floor(a+1)]-tbl[floor(a)])/(floor(a+1)-floor(a));
fprintf(fp,"%f\n",x[i]);
	 while(a >= tbl.size()) a = a - tbl.size();

  }
  adsr(x); //apply envelope to x and update internal status of ADSR
fclose(fp);
  return x;
}

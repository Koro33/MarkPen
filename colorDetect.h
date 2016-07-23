// colorDetect.h

#ifndef _COLORDETECT_h
#define _COLORDETECT_h
#include "arduino.h"
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define DETECT0_S2PIN 5
#define DETECT0_S3PIN 6
#define DETECT0_OUTPIN 7
#define DETECT0_LEDPIN 8

#define DETECT1_S2PIN 9
#define DETECT1_S3PIN 10
#define DETECT1_OUTPIN 11
#define DETECT1_LEDPIN 12

class colorDetect
{
public:
	colorDetect(int s2,int s3, int out, int led);
	~colorDetect();
	int begin();
	int detect(int* red, int* green, int* blue);

private:
	int s2pin;
	int s3pin;
	int outpin;
	int ledpin;

};

extern colorDetect colorDetect0;
extern colorDetect colorDetect1;



#endif


// 
// 
// 

#include "colorDetect.h"

colorDetect::colorDetect(int s2, int s3, int out, int led)
{
	s2pin = s2;
	s3pin = s3;
	outpin = out;
	ledpin = led;
}


colorDetect::~colorDetect()
{
}

int colorDetect::begin()
{
	pinMode(s2pin, OUTPUT);
	pinMode(s3pin, OUTPUT);
	pinMode(outpin, INPUT);
	pinMode(ledpin, OUTPUT);

	digitalWrite(s2pin, 1);
	digitalWrite(s3pin, 1);
	digitalWrite(ledpin, 0);
	return 0;
}

int colorDetect::detect(int* red, int* green, int* blue)
{
	//RED
	digitalWrite(s2pin, LOW);
	digitalWrite(s3pin, LOW);
	*red = pulseIn(outpin, digitalRead(outpin) == HIGH ? LOW : HIGH);
	//BLUE
	digitalWrite(s2pin, LOW);
	digitalWrite(s3pin, HIGH);
	*blue = pulseIn(outpin, digitalRead(outpin) == HIGH ? LOW : HIGH);
	//GREEN  
	digitalWrite(s3pin, HIGH);
	digitalWrite(s2pin, HIGH);
	*green = pulseIn(outpin, digitalRead(outpin) == HIGH ? LOW : HIGH);
	//close
	digitalWrite(s2pin, HIGH);
	digitalWrite(s3pin, HIGH);

	return 0;
}




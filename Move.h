// Move.h

#ifndef _MOVE_h
#define _MOVE_h
#include "arduino.h"
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define STEPPER_STEPPIN 2
#define STEPPER_DIRPIN 3
#define STEPPER_RESETPIN 4
#define STEPPER_ENABLEPIN 55

#define SUBDIVISION 16


class move
{
public:
	move();
	~move();
	int begin(void);
	int reset(void);
	int setSpeed(int setspeed);
	int moveOnePulse(int dir);
	int movePulse(int dir, int num);
	int moveOneStep(int dir);
	int moveStep(int dir, int num);
	int moveOnePosition(int dir);
	int movePosition(int dir, int num);

	int movePosFromTo(int * From, int To);



	




private:
	int speed = 450;
};

extern move move0;
#endif


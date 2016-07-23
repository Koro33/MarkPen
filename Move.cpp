// 
// 
// 

#include "Move.h"


move::move()
{
}

move::~move()
{
}

int move::begin(void)
{
	pinMode(STEPPER_STEPPIN, OUTPUT);
	pinMode(STEPPER_DIRPIN, OUTPUT);
	pinMode(STEPPER_RESETPIN, INPUT);

	digitalWrite(STEPPER_STEPPIN, 0);
	digitalWrite(STEPPER_DIRPIN, 0);

	return 0;
}

int move::reset(void)
{
	setSpeed(450);

	while (!digitalRead(STEPPER_RESETPIN))
	{
		moveOnePulse(0);
	}
	
	for (int i = 0; i < 3; i++)//½ÃÕý
	{
		moveOneStep(0);
	}

	setSpeed(800);

	return 0;
}

int move::setSpeed(int setspeed)
{
	speed = setspeed;
	return 0;
}

int move::moveOnePulse(int dir = 0)
{
	if (dir == 0)
	{
		digitalWrite(STEPPER_DIRPIN, 0);

		digitalWrite(STEPPER_STEPPIN, 1);
		delayMicroseconds(speed);
		digitalWrite(STEPPER_STEPPIN, 0);
		delayMicroseconds(speed);
	}
	else
	{
		digitalWrite(STEPPER_DIRPIN, 1);
		digitalWrite(STEPPER_STEPPIN, 1);
		delayMicroseconds(speed);
		digitalWrite(STEPPER_STEPPIN, 0);
		delayMicroseconds(speed);

	}
	return 0;
}

int move::movePulse(int dir = 0, int num = 1)
{
	for (int i = 0; i < num; i++)
	{
		moveOnePulse(dir);
	}
	return 0;
}

int move::moveOneStep(int dir = 0)
{
	for (int i = 0; i < SUBDIVISION; i++)
	{
		moveOnePulse(dir);
	}

	return 0;
}

int move::moveStep(int dir = 0, int num = 1)
{
	for (int i = 0; i < num; i++)
	{
		moveOneStep(dir);
	}
	return 0;
}

int move::moveOnePosition(int dir = 0)
{
	for (int i = 0; i < 432; i++)
	{
		moveOnePulse(dir);
	}
	return 0;
}

int move::movePosition(int dir = 0, int num = 1)
{
	for (int i = 0; i < num; i++)
	{
		moveOnePosition(dir);
	}
	return 0;
}

int move::movePosFromTo(int* From, int To)
{
	if (To >= 20)
	{
		To = To % 20;
	}
	if (*From > To)
	{
		if ((*From - To) > 10)
		{
			movePosition(0, 20 - *From + To);
		}
		else
		{
			movePosition(1, *From - To);
		}
		
	}
	else
	{
		if ((To - *From) > 10)
		{
			movePosition(1, 20 - To + *From);
		}
		else
		{
			movePosition(0, To - *From);
		}
	}

	*From = To;
	return abs(*From - To);
}






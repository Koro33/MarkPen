// 
// 
// 

#include "positionInfo.h"
#include <EEPROM.h>
#include "Move.h"
#include "colorDetect.h"

posInfo::posInfo(int a)
{
	if (a == 0)
	{
		in_OR_out = 0;
	}
	else if (a == 1)
	{
		in_OR_out = 1;
	}
}

posInfo::~posInfo()
{
}

int posInfo::posInfoWrite(int posNum)
{
	EEPROM.write(posNum * 4 + 0, isExist);
	EEPROM.write(posNum * 4 + 1, R);
	EEPROM.write(posNum * 4 + 2, G);
	EEPROM.write(posNum * 4 + 3, B);

	return 0;
}

int posInfo::posInfoRead(int posNum)
{
	this->isExist = EEPROM.read(posNum * 4 + 0);
	this->      R = EEPROM.read(posNum * 4 + 1);
	this->      G = EEPROM.read(posNum * 4 + 2);
	this->      B = EEPROM.read(posNum * 4 + 3);
	return 0;
}

int posInfo::colorDetect(void)
{
	if (in_OR_out == 0)
	{
		//move0.movePosition(1, 5);
		for (int i = 0; i < 5; i++)
		{
			colorDetect0.detect(&(this->R_tmp), &(this->G_tmp), &(this->B_tmp));
			R += R_tmp; G += G_tmp; B += B_tmp;
			if (i == 4)
			{
				R_tmp = R / 5; G_tmp = G / 5; B_tmp = B / 5;
				R_f = 100 * R_tmp / (R_tmp + G_tmp + B_tmp);
				G_f = 100 * G_tmp / (R_tmp + G_tmp + B_tmp);
				B_f = 100 * B_tmp / (R_tmp + G_tmp + B_tmp);
				R = (int)R_f;
				G = (int)G_f;
				B = (int)B_f;
			}
		}


	}
	else if (in_OR_out == 1)
	{
		//move0.movePosition(0, 4);
		for (int i = 0; i < 5; i++)
		{
			colorDetect1.detect(&(this->R_tmp), &(this->G_tmp), &(this->B_tmp));
			R += R_tmp; G += G_tmp; B += B_tmp;
			if (i == 4)
			{
				R_tmp = R / 5; G_tmp = G / 5; B_tmp = B / 5;
				R_f = 100 * R_tmp / (R_tmp + G_tmp + B_tmp);
				G_f = 100 * G_tmp / (R_tmp + G_tmp + B_tmp);
				B_f = 100 * B_tmp / (R_tmp + G_tmp + B_tmp);
				R = (int)R_f;
				G = (int)G_f;
				B = (int)B_f;
			}
		}

	}
	return 0;
}













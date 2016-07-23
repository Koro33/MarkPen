
#include <Servo.h>
#include "colorDetect.h"
#include "positionInfo.h"
#include <EEPROM.h>
#include "Move.h"
#include "positionInfo.h"
#include "math.h"

#define SERVO0_PIN 23
#define SERVO0_UP 50
#define SERVO0_DOWN 60
#define SERVO1_PIN 13
#define SERVO1_UP 160
#define SERVO1_DOWN 110

#define EXISTCHECK0_PIN 34
#define EXISTCHECK1_PIN 44

move move0;
posInfo positionInner[20] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
posInfo positionOuter[20] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
colorDetect colorDetect0(DETECT0_S2PIN, DETECT0_S3PIN, DETECT0_OUTPIN, DETECT0_LEDPIN);
colorDetect colorDetect1(DETECT1_S2PIN, DETECT1_S3PIN, DETECT1_OUTPIN, DETECT1_LEDPIN);
Servo servo0;
Servo servo1;
String comdata0 = "";//串口接收数据
String comdata1 = "";//串口接收数据

int curr_pos = 0;
int curr_states = 0;
int red;
int green;
int blue;

int red_aim;
int green_aim;
int blue_aim;

void setup()
{
	Serial.begin(115200);
	Serial1.begin(115200);
	move0.begin();
	colorDetect0.begin();
	colorDetect1.begin();
	servo0.attach(SERVO0_PIN);
	servo0.write(SERVO0_DOWN);
	servo1.attach(SERVO1_PIN);
	servo1.write(SERVO1_DOWN);
	move0.reset();
	//colorReset();
	for (int i = 0; i < 20; i++)
	{
		positionOuter[i].posInfoRead(i);
	}
	debug_showlist();
	delay(800);
}

void loop()
{	
	if (curr_states == 0)
	{
		Usart0_RevTask();
	}
	
	switch (curr_states)
	{
	case 0: {

		if (positionOuter[curr_pos].isExist == 1)
		{
			move0.movePosFromTo(&curr_pos, searchNull());
		}
		if (digitalRead(EXISTCHECK1_PIN) == 0)//笔已经放下
		{
			delay(800);
			if (digitalRead(EXISTCHECK1_PIN) == 0)
			{
				curr_states = 2;//启动放笔流程
			}
		}
		break;
	}
	case 1: {  //取笔 （1，R，G，B ff）
		move0.movePosFromTo(&curr_pos,judgeSimilar(red_aim, green_aim, blue_aim));
		delay(200);
		servo1.write(SERVO1_UP);
		delay(3000);
		servo1.write(SERVO1_DOWN);
		delay(1000);
		if (digitalRead(EXISTCHECK1_PIN) == 0) //笔没有拿走
		{
			delay(200);
			if (digitalRead(EXISTCHECK1_PIN) == 0)
			{
				curr_states = 0;
				break;
			}
		}
		//笔拿走了
		if (digitalRead(EXISTCHECK1_PIN) == 1)
		{
			delay(200);
			if (digitalRead(EXISTCHECK1_PIN) == 1)
			{
				positionOuter[curr_pos].isExist = 0;
				positionOuter[curr_pos].posInfoWrite(curr_pos);
			}

		}
		debug_showlist();
		curr_states = 0;
		break;
	}
	case 2: {  
		int pos = curr_pos;
		move0.movePosFromTo(&curr_pos, curr_pos + 4);
		positionOuter[pos].isExist = 1;
		positionOuter[pos].colorDetect();
		positionOuter[pos].posInfoWrite(pos);
		debug_showlist();
		curr_states = 0;
		break;
	}
	case 3: {  //复位
		colorReset();
		curr_states = 0;
		break;
	}
	default: {
		break;
	}
	}

	
	//Usart0_TmtTask();


}

int judgeSimilar(int ra, int ga, int ba)
{
	float err[20];
	for (int i = 0; i < 20; i++)
	{
		if (positionOuter[i].isExist == 1)
		{
			err[i] = abs(positionOuter[i].R - ra) + abs(positionOuter[i].G - ga) + abs(positionOuter[i].B - ba);

		}
		else
		{
			err[i] = 999;
		}
		
	}
	for (int i = 0; i < 20; i++)
	{
		Serial.print(String("") + err[i] + " ");
	}
	Serial.println("");
	return minNum(err, 20);
}

int minNum(float *a, int n)
{
	float min_t = a[0];
	int min_pos = 0;
	for (int i = 0; i < n; i++)
	{
		if (a[i] < min_t)
		{
			min_t = a[i];
			min_pos = i;
		}
	}
	return min_pos;
}

int searchNull(void)
{
	
	for (int i = curr_pos; i < 20; i++)
	{
		if (positionOuter[i].isExist == 0)
		{
			return i;
		}
	}
	for (int i = curr_pos; i >= 0; i--)
	{
		if (positionOuter[i].isExist == 0)
		{
			return i;
		}
	}
	return 999;
}

void colorReset(void)
{
	move0.reset();
	curr_pos = 0;
	for (int i = 0; i < 20; i++)
	{
		positionOuter[i].isExist = (1-digitalRead(EXISTCHECK1_PIN));
		positionOuter[(i<3)?i+16:i-4].colorDetect();
		move0.movePosition(0,1);
		delay(100);
	}
	for (int i = 0; i < 20; i++)
	{
		positionOuter[i].posInfoWrite(i);
	}
	debug_showlist();
	
}

//int Usart0_RevTask(void)
//{
//	String temp = "";
//	int first = 0, second = 0, third = 0;
//	int p = 0;
//
//	if (Serial.available() > 0)
//	{
//
//		while (Serial.peek() != 0x66)
//		{
//			comdata0 += char(Serial.read());
//			delay(2);
//		}
//		Serial.read();
//		delay(2);
//		Serial.read();
//		delay(2);
//
//		for (int i = 0; i < comdata0.length(); i++)
//		{
//
//			if (comdata0[i] == ',')
//			{
//				switch (p)
//				{
//				case 0: {first = i; p = 1; break; }
//				case 1: {second = i; p = 2; break; }
//				case 2: {third = i; p = 3; break; }
//				default: break;
//				}
//
//			}
//		}
//
//		Serial.println(comdata0);
//		//Serial.println(first);
//		//Serial.println(second);
//		//Serial.println(third);
//		temp = comdata0.substring(0, first);
//		curr_states = temp.toInt();
//		temp = comdata0.substring(first + 1, second);
//		//red_aim = temp.toFloat();
//		red_aim = temp.toInt();
//		temp = comdata0.substring(second + 1, third);
//		//green_aim = temp.toFloat();
//		green_aim = temp.toInt();
//		temp = comdata0.substring(third + 1, comdata0.length());
//		//blue_aim = temp.toFloat();
//		blue_aim = temp.toInt();
//		comdata0 = "";
//		//Serial.println(String("") + red_aim + " " + green_aim + " " + blue_aim + " ");
//		return 1;
//	}
//	return 0;
//}

int Usart0_RevTask(void)
{
	String temp = "";
	int first = 0, second = 0, third = 0;
	int p = 0;

	if (Serial.available() > 0)
	{
		if (char(Serial.read()) == 'n')
		{
			delay(1);
			if (char(Serial.read()) == 'b')
			{
				delay(1);
				while (char(Serial.peek()) != 'f')
				{
					comdata0 += char(Serial.read());
					delay(1);
				}
				Serial.read();
				delay(1);
				Serial.read();
				delay(1);

				for (int i = 0; i < comdata0.length(); i++)
				{

					if (comdata0[i] == ',')
					{
						switch (p)
						{
						case 0: {first = i; p = 1; break; }
						case 1: {second = i; p = 2; break; }
						case 2: {third = i; p = 3; break; }
						default: break;
						}

					}
				}

				Serial.println(comdata0);
				//Serial.println(first);
				//Serial.println(second);
				//Serial.println(third);
				temp = comdata0.substring(0, first);
				curr_states = temp.toInt();
				temp = comdata0.substring(first + 1, second);
				//red_aim = temp.toFloat();
				red_aim = temp.toInt();
				temp = comdata0.substring(second + 1, third);
				//green_aim = temp.toFloat();
				green_aim = temp.toInt();
				temp = comdata0.substring(third + 1, comdata0.length());
				//blue_aim = temp.toFloat();
				blue_aim = temp.toInt();
				comdata0 = "";
				//Serial.println(String("") + red_aim + " " + green_aim + " " + blue_aim + " ");
				return 1;
			}
			return 0;
		}
		return 0;
	}
	return 0;
}

int Usart1_RevTask(void)
{
	String temp = "";
	int first = 0, second = 0, third = 0;
	int p = 0;

	if (Serial1.available() > 0)
	{
		while (Serial1.peek() != 0x66)
		{
			comdata1 += char(Serial1.read());
			delay(2);

		}
		Serial1.read();
		delay(2);
		Serial1.read();
		delay(2);

		for (int i = 0; i < comdata1.length(); i++)
		{

			if (comdata1[i] == ',')
			{
				switch (p)
				{
				case 0: {first = i; p = 1; break; }
				case 1: {second = i; p = 2; break; }
				case 2: {third = i; p = 3; break; }
				default: break;
				}

			}
		}

		//Serial1.println(comdata1);
		//Serial1.println(first);
		//Serial1.println(second);
		//Serial1.println(third);
		temp = comdata1.substring(0, first);
		curr_states = temp.toInt();
		temp = comdata1.substring(first + 1, second);
		//red_aim = temp.toFloat();
		red_aim = temp.toInt();
		temp = comdata1.substring(second + 1, third);
		//green_aim = temp.toFloat();
		green_aim = temp.toInt();
		temp = comdata1.substring(third + 1, comdata0.length());
		//blue_aim = temp.toFloat();
		blue_aim = temp.toInt();
		comdata1 = "";
		//Serial1.println(String("") + red_aim + " " + green_aim + " " + blue_aim + " ");
		return 1;
	}
	return 0;
}
int Usart0_TmtTask(void)
{
	Serial.println(String("") + judgeSimilar(red_aim, green_aim, blue_aim));

}


void debug_showlist(void)
{
	for (int i = 0; i < 20; i++)
	{
		if (positionOuter[i].isExist == 1)
		{
			Serial.println(String("") + i + " " + positionOuter[i].isExist + " " + positionOuter[i].R + " " + positionOuter[i].G + " " + positionOuter[i].B);
		}
		else
		{
			Serial.println(String("") + i + " " + positionOuter[i].isExist);
		}
	}
}


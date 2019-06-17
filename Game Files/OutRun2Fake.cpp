#include <string>
#include "Outrun2Fake.h"
#include "SDL.h"
#include <Windows.h>
static EffectTriggers *myTriggers;
static EffectConstants *myConstants;
static Helpers *myHelpers;
static SDL_Event e;
static wchar_t *settingsFilename = TEXT(".\\FFBPlugin.ini");
static int FFBMode = GetPrivateProfileInt(TEXT("Settings"), TEXT("FFBMode"), 0, settingsFilename);
static int ShowButtonNumbersForSetup = GetPrivateProfileInt(TEXT("Settings"), TEXT("ShowButtonNumbersForSetup"), 0, settingsFilename);
static int ChangeGearsViaPlugin = GetPrivateProfileInt(TEXT("Settings"), TEXT("ChangeGearsViaPlugin"), 0, settingsFilename);
static int Gear1 = GetPrivateProfileInt(TEXT("Settings"), TEXT("Gear1"), 0, settingsFilename);
static int Gear2 = GetPrivateProfileInt(TEXT("Settings"), TEXT("Gear2"), 0, settingsFilename);
static int Gear3 = GetPrivateProfileInt(TEXT("Settings"), TEXT("Gear3"), 0, settingsFilename);
static int Gear4 = GetPrivateProfileInt(TEXT("Settings"), TEXT("Gear4"), 0, settingsFilename);
static int Gear5 = GetPrivateProfileInt(TEXT("Settings"), TEXT("Gear5"), 0, settingsFilename);
static int Gear6 = GetPrivateProfileInt(TEXT("Settings"), TEXT("Gear6"), 0, settingsFilename);
static int SpeedStrength;

static int RunningThread(void *ptr)
{
	int cnt;
	for (cnt = 0; cnt >= 0; ++cnt)
	{
		int ff = myHelpers->ReadInt32(0x0827A1A0, /* isRelativeOffset */ false);
		int ffwall = myHelpers->ReadInt32(0x08273FAC, /* isRelativeOffset */ false);
		int ff3 = myHelpers->ReadInt32(0x0827A1DA, /* isRelativeOffset */ false);
		int ff4 = myHelpers->ReadInt32(0x0827A35D, /* isRelativeOffset */ false);
		int ff5 = myHelpers->ReadInt32(0x0827A1D4, /* isRelativeOffset */ false);
		UINT8 ff6 = myHelpers->ReadByte(0x08670DC8, /* isRelativeOffset */ false); // steering
		float ff7 = myHelpers->ReadFloat32(0x08273AD4, /* isRelativeOffset */ false);
		UINT8 ff8 = myHelpers->ReadByte(0x08304ADC, /* isRelativeOffset */ false); // 1 when race
		UINT8 ff9 = myHelpers->ReadByte(0x086749CA, /* isRelativeOffset */ false); // 1 when menu
		UINT8 gear = myHelpers->ReadByte(0x0827A160, /* isRelativeOffset */ false); // gear address
		float ffspeed = myHelpers->ReadFloat32(0x08273DF0, /* isRelativeOffset */ false); //speedo
		UINT8 static oldgear = 0;
		int static oldFloat = 0.0;
		int newFloat = ff3;
		int static oldFloat1 = 0.0;
		int newFloat1 = ff4;
		float newgear = gear;
		
		if ((ffspeed >= 0.1) && (ffspeed <= 80))
		{
			SpeedStrength = 10;
		}
		else if ((ffspeed >= 80.1) && (ffspeed <= 130))
		{
			SpeedStrength = 20;
		}
		else if ((ffspeed >= 130.1) && (ffspeed <= 180))
		{
			SpeedStrength = 30;
		}
		else if ((ffspeed >= 180.1) && (ffspeed <= 220))
		{
			SpeedStrength = 40;
		}
		else if ((ffspeed >= 220.1) && (ffspeed <= 270))
		{
			SpeedStrength = 50;
		}
		else if ((ffspeed >= 270.1) && (ffspeed <= 320))
		{
			SpeedStrength = 60;
		}
		else if ((ffspeed >= 320.1) && (ffspeed <= 380))
		{
			SpeedStrength = 70;
		}
		else if ((ffspeed >= 380.1) && (ffspeed <= 430))
		{
			SpeedStrength = 80;
		}
		else if ((ffspeed >= 430.1) && (ffspeed <= 500))
		{
			SpeedStrength = 90;
		}
		else if ((ffspeed >= 500.1) && (ffspeed <= 1000))
		{
			SpeedStrength = 100;
		}
		else
		{
			SpeedStrength = 0;
		}

		if (FFBMode == 1)
		{
			if (ff8 == 1)
			{
				if ((ff6 >= 0x00) & (ff6 < 0x7F))
				{
					double percentForce = ((127 - ff6) / 127.0);
					double percentLength = 100;
					myTriggers->LeftRight(pow(percentForce, 0.5), 0, percentLength);
					myTriggers->Constant(myConstants->DIRECTION_FROM_LEFT, (pow(percentForce, 0.5)));
				}
				if ((ff6 > 0x7F) & (ff6 < 0x100))
				{
					double percentForce = ((ff6 - 127) / 128.0);
					double percentLength = 100;
					myTriggers->LeftRight(0, pow(percentForce, 0.5), percentLength);
					myTriggers->Constant(myConstants->DIRECTION_FROM_RIGHT, (pow(percentForce, 0.5)));
				}
			}
			if (ff9 == 1)
			{
				if ((ff6 >= 0x00) & (ff6 < 0x7F))
				{
					double percentForce = ((127 - ff6) / 127.0);
					double percentLength = 100;
					myTriggers->LeftRight(pow(percentForce, 0.5), 0, percentLength);
					myTriggers->Constant(myConstants->DIRECTION_FROM_LEFT, (pow(percentForce, 0.5)));
				}
				if ((ff6 > 0x7F) & (ff6 < 0x100))
				{
					double percentForce = ((ff6 - 127) / 128.0);
					double percentLength = 100;
					myTriggers->LeftRight(0, pow(percentForce, 0.5), percentLength);
					myTriggers->Constant(myConstants->DIRECTION_FROM_RIGHT, (pow(percentForce, 0.5)));
				}
			}
			if (ff5 == 2)
			{
				if (oldFloat != newFloat)
				{
					double percentForce = SpeedStrength / 100.0;
					double percentLength = 100;
					myTriggers->LeftRight(0, pow(percentForce, 0.5), percentLength);
					myTriggers->Constant(myConstants->DIRECTION_FROM_RIGHT, (pow(percentForce, 0.5)));
				}
			}
			else if (ff5 == 1)
			{
				if (oldFloat != newFloat)
				{
					double percentForce = SpeedStrength / 100.0;
					double percentLength = 100;
					myTriggers->LeftRight(0, pow(percentForce, 0.5), percentLength);
					myTriggers->Constant(myConstants->DIRECTION_FROM_RIGHT, (pow(percentForce, 0.5)));
				}
			}
		}		
		else
		{
			if (ff8 == 1)
			{
				if ((ff6 >= 0x00) & (ff6 < 0x7F))
				{
					double percentForce = ((127 - ff6) / 127.0);
					double percentLength = 100;
					myTriggers->LeftRight(percentForce, 0, percentLength);
					myTriggers->Constant(myConstants->DIRECTION_FROM_LEFT, percentForce);
				}
				if ((ff6 > 0x7F) & (ff6 < 0x100))
				{
					double percentForce = ((ff6 - 127) / 128.0);
					double percentLength = 100;
					myTriggers->LeftRight(0, percentForce, percentLength);
					myTriggers->Constant(myConstants->DIRECTION_FROM_RIGHT, percentForce);
				}
			}
			if (ff9 == 1)
			{
				if ((ff6 >= 0x00) & (ff6 < 0x7F))
				{
					double percentForce = ((127 - ff6) / 127.0);
					double percentLength = 100;
					myTriggers->LeftRight(percentForce, 0, percentLength);
					myTriggers->Constant(myConstants->DIRECTION_FROM_LEFT, percentForce);
				}
				if ((ff6 > 0x7F) & (ff6 < 0x100))
				{
					double percentForce = ((ff6 - 127) / 128.0);
					double percentLength = 100;
					myTriggers->LeftRight(0, percentForce, percentLength);
					myTriggers->Constant(myConstants->DIRECTION_FROM_RIGHT, percentForce);
				}
			}
			if (ff5 == 2)
			{
				if (oldFloat != newFloat)
				{
					double percentForce = SpeedStrength / 100.0;
					double percentLength = 100;
					myTriggers->LeftRight(0, percentForce, percentLength);
					myTriggers->Constant(myConstants->DIRECTION_FROM_RIGHT, percentForce);
				}
			}
			else if (ff5 == 1)
			{
				if (oldFloat != newFloat)
				{
					double percentForce = SpeedStrength / 100.0;
					double percentLength = 100;
					myTriggers->LeftRight(percentForce, 0, percentLength);
					myTriggers->Constant(myConstants->DIRECTION_FROM_LEFT, percentForce);
				}
			}
		}			
		if (oldFloat1 != newFloat1)
		{
			double percentForce = SpeedStrength / 100.0;
			double percentLength = 100;
			myTriggers->LeftRight(percentForce, percentForce, percentLength);
			myTriggers->Sine(200, 200, percentForce);
		}		
		else if (ff == 8)
		{
			if (SpeedStrength > 0)
			{
				double percentForce = 0.1;
				double percentLength = 100;
				myTriggers->LeftRight(percentForce, percentForce, percentLength);
				myTriggers->Sine(70, 70, percentForce);
			}			
		}
		else if (ff == 4)
		{
			if (SpeedStrength > 0)
			{
				double percentForce = 0.2;
				double percentLength = 50;
				myTriggers->LeftRight(percentForce, percentForce, percentLength);
				myTriggers->Sine(50, 50, percentForce);
			}			
		}
		else if (ff == 16)
		{
			if (SpeedStrength > 0)
			{
				double percentForce = 0.2;
				double percentLength = 50;
				myTriggers->LeftRight(percentForce, percentForce, percentLength);
				myTriggers->Sine(100, 50, percentForce);
			}			
		}
		oldFloat = newFloat;
		oldFloat1 = newFloat1;
	}
	return 0;
}
void OutRun2Fake::FFBLoop(EffectConstants *constants, Helpers *helpers, EffectTriggers* triggers) {
	SDL_Thread *thread;
	thread = SDL_CreateThread(RunningThread, "RunningThread", (void *)NULL);
	int ff = myHelpers->ReadInt32(0x0827A1A0, /* isRelativeOffset */ false);	
	helpers->log("got value: ");
	std::string ffs = std::to_string(ff);
	helpers->log((char *)ffs.c_str());

	while (SDL_WaitEvent(&e) != 0)
	{
		UINT8 transmission = helpers->ReadByte(0x082932C2, /* isRelativeOffset */ false); // Auto or Manual
		myTriggers = triggers;
		myConstants = constants;
		myHelpers = helpers;
		if (ShowButtonNumbersForSetup == 1)
		{
			if (e.type == SDL_JOYBUTTONDOWN)
			{
				if (e.jbutton.button == 0)
				{
					MessageBoxA(NULL, "Button 0 Pressed", "", 0);
				}
				else if (e.jbutton.button == 1)
				{
					MessageBoxA(NULL, "Button 1 Pressed", "", 0);
				}
				else if (e.jbutton.button == 2)
				{
					MessageBoxA(NULL, "Button 2 Pressed", "", 0);
				}
				else if (e.jbutton.button == 3)
				{
					MessageBoxA(NULL, "Button 3 Pressed", "", 0);
				}
				else if (e.jbutton.button == 4)
				{
					MessageBoxA(NULL, "Button 4 Pressed", "", 0);
				}
				else if (e.jbutton.button == 5)
				{
					MessageBoxA(NULL, "Button 5 Pressed", "", 0);
				}
				else if (e.jbutton.button == 6)
				{
					MessageBoxA(NULL, "Button 6 Pressed", "", 0);
				}
				else if (e.jbutton.button == 7)
				{
					MessageBoxA(NULL, "Button 7 Pressed", "", 0);
				}
				else if (e.jbutton.button == 8)
				{
					MessageBoxA(NULL, "Button 8 Pressed", "", 0);
				}
				else if (e.jbutton.button == 9)
				{
					MessageBoxA(NULL, "Button 9 Pressed", "", 0);
				}
				else if (e.jbutton.button == 10)
				{
					MessageBoxA(NULL, "Button 10 Pressed", "", 0);
				}
				else if (e.jbutton.button == 11)
				{
					MessageBoxA(NULL, "Button 11 Pressed", "", 0);
				}
				else if (e.jbutton.button == 12)
				{
					MessageBoxA(NULL, "Button 12 Pressed", "", 0);
				}
				else if (e.jbutton.button == 13)
				{
					MessageBoxA(NULL, "Button 13 Pressed", "", 0);
				}
				else if (e.jbutton.button == 14)
				{
					MessageBoxA(NULL, "Button 14 Pressed", "", 0);
				}
				else if (e.jbutton.button == 15)
				{
					MessageBoxA(NULL, "Button 15 Pressed", "", 0);
				}
			}
		}
		if (e.type == SDL_JOYBUTTONDOWN)
		{
			if (ChangeGearsViaPlugin == 1)
			{
				if ((ChangeGearsViaPlugin == 1) && (transmission == 1))
				{
					if (e.jbutton.button == Gear1)
					{
						helpers->WriteByte(0x0827A160, 0x01, false);
					}
					else if (e.jbutton.button == Gear2)
					{
						helpers->WriteByte(0x0827A160, 0x02, false);
					}
					else if (e.jbutton.button == Gear3)
					{
						helpers->WriteByte(0x0827A160, 0x03, false);
					}
					else if (e.jbutton.button == Gear4)
					{
						helpers->WriteByte(0x0827A160, 0x04, false);
					}
					else if (e.jbutton.button == Gear5)
					{
						helpers->WriteByte(0x0827A160, 0x05, false);
					}
					else if (e.jbutton.button == Gear6)
					{
						helpers->WriteByte(0x0827A160, 0x06, false);
					}
				}
			}
		}
	}
}
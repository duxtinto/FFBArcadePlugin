#include <string>
#include "MarioKartGPDX.h"

void MarioKartGPDX100::FFBLoop(EffectConstants *constants, Helpers *helpers, EffectTriggers* triggers) {
	helpers->log("in MKDX Ffbloop");
	
	int ff1 = helpers->ReadInt32(0x564C5F, /* isRelativeOffset */ true); //shake
	int ff2 = helpers->ReadInt32(0x559B68,/* isRelativeOffset */ true);
	int ff3 = helpers->ReadInt32(ff2 + 0x5F8, /* isRelativeOffset */ false); // terrain data
	int ff4 = helpers->ReadInt32(0x563860, /* isRelativeOffset */ true); //0-255 accl
	int ff5 = helpers->ReadInt32(ff2 + 0x628, /* isRelativeOffset */ false); //kart flying or on ground
	int ff6 = helpers->ReadInt32(0x5532C4,/* isRelativeOffset */ true);
	int ff7 = helpers->ReadInt32(ff6 + 0x1F0, /* isRelativeOffset */ false);
	int ff8 = helpers->ReadInt32(ff7 + 0x18, /* isRelativeOffset */ false);
	int ff9 = helpers->ReadInt32(ff8 + 0x7C, /* isRelativeOffset */ false);
	int ff10 = helpers->ReadInt32(ff9 + 0x164, /* isRelativeOffset */ false); // 1 during race only
	int ff11 = helpers->ReadInt32(ff2 + 0x520, /* isRelativeOffset */ false); //1065353216 when kart moves
	helpers->log("got value: ");
	std::string ffs = std::to_string(ff1);
	helpers->log((char *)ffs.c_str()); helpers->log("got value: ");


	// Large Shake when hitting walls, other karts or getting hit by items
	if ((4194308 == ff1) & (ff10 == 1))

	{
		double percentForce = 1.0;
		double percentForce1 = 3.5;
		double percentLength = (500);
		triggers->LeftRight(percentForce1, percentForce1, percentLength);
		triggers->Sine(200, 200, percentForce);
	}

	// small friction when driving on dirt while moving
	else  if ((3 == ff3) & (ff10 == 1) & (ff5 == 1) & (ff11 == 1065353216))
	{
		double percentForce = (0.3);
		double percentLength = (100);
		triggers->LeftRight(percentForce, 0, percentLength);
		triggers->Friction(percentForce);
	}
	//	Small constant when hitting bumps
	else  if ((10 == ff3) & (ff10 == 1) & (ff5 == 1) & (ff11 == 1065353216))
	{
		double percentForce = (0.2);
		double percentLength = (50);
		double percentForce1 = 3.0;
		triggers->LeftRight(percentForce1, percentForce1, percentLength);
		triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);
	}
	// Wheel rumbles while driving on grass
	else if ((4 == ff3) & (ff10 == 1) & (ff5 == 1) & (ff11 == 1065353216))

	{
		double percentForce = 0.2;
		double percentLength = (50);
		triggers->LeftRight(0, percentForce, percentLength);
		triggers->Sine(50, 50, percentForce);
	}
	//wheel hard to turn while driving through water
	else  if ((7 == ff3) & (ff10 == 1) & (ff5 == 1) & (ff11 == 1065353216))
	{
		double percentForce = (0.65);
		triggers->Friction(percentForce);
	}
	//Wheel rumbles lightly when driving over tiles
	else if ((12 == ff3) & (ff10 == 1) & (ff5 == 1) & (ff11 == 1065353216))

	{
		double percentForce = 0.1;
		double percentForce1 = 0.2;
		double percentLength = (150);
		triggers->LeftRight(percentForce1, 0, percentLength);
		triggers->Sine(70, 70, percentForce);
	}
	//Wheel rumbles lightly when driving over sand
	else if ((14 == ff3) & (ff10 == 1) & (ff5 == 1) & (ff11 == 1065353216))

	{
		double percentForce = 0.1;
		double percentForce1 = 0.2;
		double percentLength = (50);
		triggers->LeftRight(percentForce1, 0, percentLength);
		triggers->Sine(70, 70, percentForce);
	}
	//Wheel rumbles lightly when driving over rough part of track
	else if ((11 == ff3) & (ff10 == 1) & (ff5 == 1) & (ff11 == 1065353216))

	{
		double percentForce = 0.1;
		double percentForce1 = 0.2;
		double percentLength = (250);
		triggers->LeftRight(0, percentForce1, percentLength);
		triggers->Sine(40, 50, percentForce);
	}
	//Wheel rumbles moderately when driving over wooden bridges
	else if ((8 == ff3) & (ff10 == 1) & (ff5 == 1) & (ff11 == 1065353216))

	{
		double percentForce = 0.4;
		double percentLength = (100);
		triggers->LeftRight(percentForce, percentForce, percentLength);
		triggers->Sine(180, 150, percentForce);
	}
}
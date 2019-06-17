#include <string>
#include "SonicSegaAllStarsRacing.h"

void SonicSegaAllStarsRacing::FFBLoop(EffectConstants *constants, Helpers *helpers, EffectTriggers* triggers) {
	INT_PTR FFBEnable = helpers->ReadByte(0x5CD858, /* isRelativeOffset*/ true);
	INT_PTR FFB = helpers->ReadByte(0x5CD864, /* isRelativeOffset*/ true);

	{
		//Enable FFB
		helpers->WriteByte(0x5CD858, 0x03, true);
	}
	{
		//Trigger friction to stop any oscillation
		double percentForce = 0.2;
		triggers->Friction(percentForce);
	}
	if ((FFB > 0) & (FFB < 19))
	{
		helpers->log("moving wheel right");
		double percentForce = (FFB) / 18.0;
		double percentLength = 100;
		triggers->LeftRight(0, percentForce, percentLength);
		triggers->Constant(constants->DIRECTION_FROM_RIGHT, percentForce);
		lastWasStop = 0;

	}
	else if ((FFB > 237) & (FFB < 256))
	{
		helpers->log("moving wheel right");
		double percentForce = (256 - FFB) / 18.0;
		double percentLength = 100;
		triggers->LeftRight(percentForce, 0, percentLength);
		triggers->Constant(constants->DIRECTION_FROM_LEFT, percentForce);
		lastWasStop = 0;
	}
	else
	{
		if (lastWasStop == 0) {
			triggers->Constant(constants->DIRECTION_FROM_LEFT, 0); // just pass the hash of 0 strength so we update lastEffectHash & lastEffectTime
			lastWasStop = 1;
		}
	}
}
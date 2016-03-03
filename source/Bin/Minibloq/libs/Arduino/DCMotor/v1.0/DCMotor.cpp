/* 	DCMotor

	A small DC motor control class.

	Copyright (C) 2010-2013 Multiplo
	http://robotgroup.com.ar
	http://multiplo.org
	Developers: Juli�n U. da Silva Gillig
	Created: 2010.04.06 (yyyy.mm.dd)
	Last modified: 2013.09.15 (yyyy.mm.dd)

	IMPORTANT NOTICE: This library is based on code from the SNAPI library, which is under The Multiplo
	Pacifist License (MPL). This license is (or may be) not compatible with the GNU Lesser General Public
	License (LGPL), so this notice constitutes an special written exception from Multiplo, to allow deployment
	under the LGPL, in order to be compiled with the Arduino libraries and still hold compatibility	between
	both licenses.
	But this does not affects the SNAPI original license in any way, and DOES NOT AUTHORIZES ANY THIRD PARTY
	TO USE OR REDISTRIBUTE THE SNAPI LIBRARY UNDER THE LGPL. In other words: This exception ONLY AFFECTS the
	library when compiled with Arduino libraries, if and only if, this license (the MPL) results in some
	inconpatibility with the LGPL (so in this last case, then LGPL applies):

        This library is free software; you can redistribute it and/or
        modify it under the terms of the GNU Lesser General Public
        License as published by the Free Software Foundation; either
        version 2.1 of the License, or (at your option) any later version.

        This library is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
        Lesser General Public License for more details.

    Any way, we strongly believe in peace, so we would not like to see our work in any non-civil or military
    project. But, due to the possible incompatibilty with the LGPL licenses, this may be only a recommendation
    and an expression of our wishes. But, if some clever guy can determine that LGPL and MPL are compatible
    licenses, then MPL applies here. You can contact us at: info_t1@multiplo.com.ar
*/

#include "DCMotor.h"


const int maxPWMValue = 255;


void DCMotor::setPower(const float value)
{
    float tempPower = constrain(value, dcMotMinPower, dcMotMaxPower);

    prevPower = power;
    power = tempPower;

    if (!clockwise)
        tempPower = (-tempPower);

    tempPower = (tempPower/dcMotMaxPower)*maxPWMValue;
    if (tempPower > zeroPowerZone) //Margin for floating point: between -zeroPowerZone and +zeroPowerZone is read as zero.
    {
        //Forward:
        digitalWrite(d0_pin, HIGH); //First sets the direction.
        digitalWrite(d1_pin, LOW);
        analogWrite(enable_pin, (int)tempPower); //Then sets the PWM's duty cycle.
    }
    else if (tempPower < -zeroPowerZone)
    {
        //Reverse:
        digitalWrite(d0_pin, LOW);
        digitalWrite(d1_pin, HIGH);
        analogWrite(enable_pin, (int)(-tempPower));
    }
    else //(tempPower == 0)
    {
        //Free-running:
		analogWrite(enable_pin, 0);		//To avoid a possible glitch generated by the PWM timer 
										//after the next instruccion.
        digitalWrite(enable_pin, LOW);  //This line would be enough, but the following ensures
        digitalWrite(d0_pin, LOW);      //the full state of the H-bridge, so if someone writes
        digitalWrite(d1_pin, LOW);      //a HIGH to the enable_pin, the state will be "Braked".
										//And in some H-bridge chips (such as the TB6612FGN) 
										//this is necessary.
    }
    braked = false;
}


void DCMotor::brake()
{
    digitalWrite(d0_pin, HIGH);
    digitalWrite(d1_pin, HIGH);
    digitalWrite(enable_pin, HIGH); //This stops the analogWrite (PWM) for the enable_pin.
    braked = true;
}


void DCMotor::setClockwise(const bool value)
{
    clockwise = value;
    if (!braked)
        setPower(power);
}

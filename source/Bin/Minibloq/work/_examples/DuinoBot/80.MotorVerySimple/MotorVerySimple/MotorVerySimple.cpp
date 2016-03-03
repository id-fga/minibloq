#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		motor0.setPower(100);
		motor1.setPower(100);
		DigitalWrite(D15, true);
		delay(1000);
		DigitalWrite(D15, false);
		delay(1000);
	}
}

void loop()
{
}

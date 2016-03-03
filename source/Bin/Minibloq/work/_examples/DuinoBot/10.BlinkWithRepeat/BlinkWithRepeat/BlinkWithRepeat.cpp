#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		motor0.setPower(200);
		motor1.setPower(-(200));
		delay(5000);
	}
}

void loop()
{
}

#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float tecla = 0;
	float velocidad = 100;
	while(true)
	{
		delay(100);
		//motor1.setPower(100);
		if(((int)(irReceiver.getIRRemoteCode())==(int)(2)))
		{
			velocidad = 100;
		}
		else
		{
		}
		if(((int)(irReceiver.getIRRemoteCode())==(int)(8)))
		{
			velocidad = -(100);
		}
		else
		{
		}
		motor0.setPower(velocidad);
		motor1.setPower(velocidad);
	}
}

void loop()
{
}

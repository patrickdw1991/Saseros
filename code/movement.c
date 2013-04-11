void forward(int speed, int masterMotor)
{
	nSyncedTurnRatio = 100;
	motor[masterMotor] = speed;
}

void backward(int speed, int masterMotor)
{
	nSyncedTurnRatio = 100;
	motor[masterMotor] = -speed;
}

void turn_left(int speed, int masterMotor, int ms)
{
	nSyncedTurnRatio = -100;
	motor[masterMotor] = speed;
	wait1Msec(ms);
}

void turn_right(int speed, int masterMotor, int ms)
{
	nSyncedTurnRatio = -100;
	motor[masterMotor] = -speed;
	wait1Msec(ms);
}

void _stop(int masterMotor)
{
	motor[masterMotor] = 0;
}

task main()
{
	nSyncedMotors = synchAB;
	turn_right(50, motorA, 1000);
}

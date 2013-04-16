#pragma config(Sensor, S4, sonarSensor, sensorSONAR) // configurating the ultrasonic sensor

void forward(int speed, int masterMotor)
{
	nSyncedTurnRatio = 100;
	motor[masterMotor] = speed;
}

void backward(int speed, int masterMotor)
{
	//TODO: sound
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

void stop(int masterMotor)
{
	motor[masterMotor] = 0;
}

task main()
{
	nSyncedMotors = synchAB;
	nSyncedTurnRatio = 100;
}

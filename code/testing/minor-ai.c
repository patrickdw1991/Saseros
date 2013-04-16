#pragma config(Sensor, S1,     bumpFront,         sensorTouch)
#pragma config(Sensor, S2,     bumpBack,          sensorTouch)
#pragma config(Sensor, S4,		 sonarSensor,       sensorSonar)

void forward(int speed, int masterMotor){
	nSyncedTurnRatio = 100;
	motor[masterMotor] = speed;
}

void backwards(int speed, int masterMotor){
	nSyncedTurnRatio = 100;
	motor[masterMotor] = -speed;
}

void _stop(int masterMotor){
	motor[masterMotor] = 0;
}


void turn_left(int speed, int masterMotor, int ms)
{
	nSyncedTurnRatio = -100;
	motor[masterMotor] = speed;
	wait1Msec(ms);
	nSyncedTurnRatio = 100;
}

void safeState(const string sensor){
	_stop(motorA);
	while(nNxtButtonPressed == -1){
		eraseDisplay();
		nxtDisplayString(1, "%s", sensor);
		wait1Msec(50);
	}
	wait1Msec(500);
	forward(100,motorA);
}

void batLow(void){
	_stop(motorA);
	eraseDisplay();
	nxtDisplayString(1, "%s", "Battery low!");
}

void backBumperTriggered(void){
	_stop(motorA);
	eraseDisplay();
	nxtDisplayString(1, "%s", "Back bumper!");

	wait1Msec(2000);
}

void backAndTurn(void){
	_stop(motorA);
	nMotorEncoder[motorA] = 0;
	nMotorEncoderTarget[motorA] = 720;
	backwards(50,motorA);
	while(nMotorRunState[motorA] != runStateIdle){
		if(SensorValue(bumpBack)) StopAllTasks();
	}
	_stop(motorA);
	wait1Msec(1000);
	turn_left(50,motorA,(random(500)+500));
	forward(100,motorA);
}

void sonarTriggered(void)
{
	_stop(motorA);
	wait1Msec(2000);
	if (SensorValue(sonarSensor)<25) {
		BackAndTurn();
	} else {
		forward(75, motorA);
	}
}

task main()
{
	srand(nMotorEncoder[motorA]);
	nSyncedMotors = synchAB;
	eraseDisplay();
	forward(100,motorA);
	while(nNxtButtonPressed == -1){
		nxtDisplayString(1, "%d", SensorValue(bumpFront));
		nxtDisplayString(2, "%d", SensorValue(bumpBack));
  	wait1Msec(50);
  	eraseDisplay();
  	if(SensorValue(bumpFront))backAndTurn();
  	if(SensorValue(bumpBack))safeState("Back Bumper");
  	if(SensorValue(sonarSensor)<25)sonarTriggered();
	}
}

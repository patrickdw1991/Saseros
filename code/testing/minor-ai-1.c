#pragma config(Sensor, S1,     bumpFront,         sensorTouch)
#pragma config(Sensor, S2,     bumpBack,          sensorTouch)
#pragma config(Sensor, S4,		 sonarSensor,       sensorSONAR)

#define DEF_SPEED 75
#define SONAR_DISTANCE 50

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
}

void failState(const string sensor){
	_stop(motorA);
	while(nNxtButtonPressed == -1){
		eraseDisplay();
		nxtDisplayString(1, "%s", sensor);
		wait1Msec(50);
	}
	wait1Msec(500);
	forward(DEF_SPEED,motorA);
}

void batLow(void){
	_stop(motorA);
	eraseDisplay();
	nxtDisplayString(1, "%s", "Battery low!");
}

void backBumperTriggered(void){
	_stop(motorA);
	PlaySound(soundLowBuzz);
	wait1Msec(2000);
	if(SensorValue(bumpBack)) StopAllTasks();
	else forward(DEF_SPEED,motorA);
}

void backAndTurn(void){
	_stop(motorA);
	nMotorEncoder[motorA] = 0;
	nMotorEncoderTarget[motorA] = 720;
	backwards(DEF_SPEED,motorA);
	while(nMotorRunState[motorA] != runStateIdle){
		if(SensorValue(bumpBack)) StopAllTasks();
	}
	_stop(motorA);
	wait1Msec(1000);
	turn_left(50,motorA,(random(500)+500));
	forward(DEF_SPEED,motorA);
}

void sonarTriggered(void)
{
	_stop(motorA);
	PlaySound(soundBlip);
	wait1Msec(2000);
	if(SensorValue(sonarSensor)<SONAR_DISTANCE) {
		backAndTurn();
	} else {
		forward(DEF_SPEED, motorA);
	}
}

task main()
{
	srand(nMotorEncoder[motorA]);
	nSyncedMotors = synchAB;
	eraseDisplay();
	forward(DEF_SPEED,motorA);
	while(nNxtButtonPressed == -1){
		nxtDisplayString(1, "%d", SensorValue(bumpFront));
		nxtDisplayString(2, "%d", SensorValue(bumpBack));
  	wait1Msec(50);
  	eraseDisplay();
  	if(SensorValue(bumpFront)){
  		backAndTurn();
  		PlaySound(soundBeepBeep);
  	}
  	if(SensorValue(bumpBack))backBumperTriggered();
  	if(SensorValue(sonarSensor)<SONAR_DISTANCE)sonarTriggered();
	}
}

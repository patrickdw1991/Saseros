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

void turn_left(int speed, int masterMotor, int ms){
	nSyncedTurnRatio = -100;
	motor[masterMotor] = speed;
	wait1Msec(ms);
}

void failState(const string sensor){
	_stop(motorA);
	PlaySound(soundDownwardTones);
	int sound_cnt = 0;
	while(nNxtButtonPressed == -1){
		eraseDisplay();
		nxtDisplayString(1, "%s", sensor);
		wait1Msec(50);
		if(sound_cnt++ > 2000){
			sound_cnt = 0;
			PlaySound(soundDownwardTones);
		}
	}
	wait1Msec(500);
	eraseDisplay();
	forward(DEF_SPEED,motorA);
}

void batLow(void){
	_stop(motorA);
	eraseDisplay();
	nxtDisplayString(1, "%s", "Battery low!");
}

void backBumperTriggered(void){
	_stop(motorA);
	PlaySound(soundLowBuzzShort);
	wait1Msec(2000);
	if(SensorValue(bumpBack))failState("Backbumper");
	else forward(DEF_SPEED,motorA);
}

void backAndTurn(void){
	_stop(motorA);
	bool failed = false;
	nMotorEncoder[motorA] = 0;
	nMotorEncoderTarget[motorA] = 720;
	backwards(DEF_SPEED,motorA);
	while(nMotorRunState[motorA] != runStateIdle){
		if(SensorValue(bumpBack)){
			failed = true;
			break;
		}
	}
	if(failed){
		failState("Backbumper during backwards");
		failed = false;
	} else {
		_stop(motorA);
		wait1Msec(1000);
		turn_left(50,motorA,(random(500)+500));
	}
	forward(DEF_SPEED,motorA);
}

void sonarTriggered (void) {
	PlaySound(soundBlip);
	_stop(motorA);
	wait1Msec(2000);
	if(SensorValue(sonarSensor)<SONAR_DISTANCE) {
		backAndTurn();
	} else {
		forward(DEF_SPEED, motorA);
	}
}

void sensorCheck(void){

}

task main()
{
	if(nAvgBatteryLevel < 7000)failState("Battery is low");

	srand(nMotorEncoder[motorA]);
	nSyncedMotors = synchAB;

	forward(DEF_SPEED,motorA);

	while(nNxtButtonPressed == -1){
		eraseDisplay();
		nxtDisplayString(1, "%d", SensorValue(bumpFront));
		nxtDisplayString(2, "%d", SensorValue(bumpBack));
  	wait1Msec(50);

  	if(nAvgBatteryLevel < 7000)failState("Battery is low");
  	if(SensorValue(bumpFront)){
  		PlaySound(soundLowBuzzShort);
  		backAndTurn();
  	}
  	if(SensorValue(bumpBack))backBumperTriggered();
  	if(SensorValue(sonarSensor)<SONAR_DISTANCE)sonarTriggered();
	}
}

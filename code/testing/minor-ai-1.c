#pragma config(Sensor, S1,    bumpFront,     sensorTouch)
#pragma config(Sensor, S2,    bumpBack,      sensorTouch)
#pragma config(Sensor, S3,    lightSensor,   sensorLightActive)
#pragma config(Sensor, S4,    sonarSensor,   sensorSONAR)

#define DEF_SPEED 30
#define SONAR_DISTANCE 30
#define DARK_LIMIT 43
#define LOW_BATTERY 6700
#define WAIT_TIME 50

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

void turn(int speed, int masterMotor, int target){
	nSyncedTurnRatio = -100;
	//motor[masterMotor] = DEF_SPEED/8;
	nMotorEncoder[masterMotor] = 0;
	nMotorEncoderTarget[masterMotor] = target;
	int direction = (rand() % 10);
	if(direction>5)
		motor[masterMotor] = speed/4;
	else
		motor[masterMotor] = -speed/4;
	while(nMotorRunState[masterMotor] != runStateIdle){
		if(SensorValue(lightSensor)<DARK_LIMIT){
			break;
		}
	}
}

void failState(const string sensor, bool contDriving){
	_stop(motorA);
	PlaySound(soundDownwardTones);
	int sound_cnt = 0;
	while(nNxtButtonPressed == -1){
		eraseDisplay();
		nxtDisplayString(1, "%s", sensor);
		wait1Msec(50);
		if(sound_cnt++ > 100){
			sound_cnt = 0;
			PlaySound(soundDownwardTones);
		}
	}
	wait1Msec(500);
	eraseDisplay();
	if(contDriving)forward(DEF_SPEED,motorA);
	ClearTimer(T1);
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
	if(SensorValue(bumpBack))failState("Backbumper",1);
	else forward(DEF_SPEED,motorA);
}

void backLight(void){
	_stop(motorA);
	backwards(DEF_SPEED, motorA);
	while(SensorValue(lightSensor)<DARK_LIMIT){
		//intentionally empty
	}
	_stop(motorA);
}

void backAndTurn(int distanceToBackUp){
	backLight();
	bool failed = false;
	nMotorEncoder[motorA] = 0;
	nMotorEncoderTarget[motorA] = distanceToBackUp*180;
	backwards(DEF_SPEED, motorA);
	while(nMotorRunState[motorA] != runStateIdle){
		if(SensorValue(bumpBack) || SensorValue(lightSensor)<DARK_LIMIT){
			failed = true;
			break;
		}
	}
	if (failed) {
		failState("Backed up into something", 0);
		failed = false;
	} else {
		_stop(motorA);
		wait1Msec(1000);
		turn(50,motorA,(random(180) + 180));
	}
	forward(DEF_SPEED,motorA);
}

void sonarTriggered (void) {
	PlaySound(soundBlip);
	_stop(motorA);
	wait1Msec(2000);
	if(SensorValue(sonarSensor)<SONAR_DISTANCE) {
		backAndTurn(2);
	} else {
		forward(DEF_SPEED, motorA);
	}
}

void lightSensorTriggered(void) {
	PlaySound(soundBlip);
	_stop(motorA);
	wait1Msec(1000);
	backAndTurn(4);
}

void sensorCheck(void){
	eraseDisplay();
	nxtDisplayString(1, "Push the");
	nxtDisplayString(2, "front bumper");
	ClearTimer(T1);
	while(!SensorValue(bumpFront)){
		wait1Msec(50);
		if(time100[T1] > WAIT_TIME){
			failState("Frontbumper error",0);
			eraseDisplay();
			nxtDisplayString(1, "Push the");
			nxtDisplayString(2, "front bumper");
		}
	}
	PlaySound(soundBeepBeep);

	eraseDisplay();
	nxtDisplayString(1, "Push the");
	nxtDisplayString(2, "back bumper");
	ClearTimer(T1);
	while(!SensorValue(bumpBack)){
		wait1Msec(50);
		if(time100[T1] > WAIT_TIME){
			failState("Backbumper error",0);
			eraseDisplay();
			nxtDisplayString(1, "Push the");
			nxtDisplayString(2, "back bumper");
		}
	}
	PlaySound(soundBeepBeep);

	eraseDisplay();
	nxtDisplayString(1, "Place hand in");
	nxtDisplayString(2, "front of sonar");
	ClearTimer(T1);
	while(SensorValue(sonarSensor)>SONAR_DISTANCE){
		wait1Msec(50);
		if(time100[T1] > WAIT_TIME){
			failState("Sonar error",0);
			eraseDisplay();
			nxtDisplayString(1, "Place hand in");
			nxtDisplayString(2, "front of sonar");
		}
	}
	PlaySound(soundBeepBeep);

	eraseDisplay();
	nxtDisplayString(1, "Light Sensor:");
	if (SensorValue(lightSensor) > 0) {
		nxtDisplayString(2, "OK");
	} else {
		failState("Lightsensor error",0);
	}
	wait1Msec(2500);

	eraseDisplay();
	nxtDisplayString(1, "Starting shortly...");
	wait1Msec(3000);
}

void motorCheck(void) {
	//Reset motorA and motorB
	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorB] = 0;

	//Set target for motorA and motorB
	nMotorEncoderTarget[motorA] = 10;
	nMotorEncoderTarget[motorB] = 10;

	eraseDisplay();
	nxtDisplayTextLine(0, "Testing motor A");
	wait1Msec(1000);
	ClearTimer(T1);
	motor[motorA] = DEF_SPEED;
	while(nMotorRunState[motorA] != runStateIdle){
		if(time100[T1] > 20) failState("Motor A failed",0);
	}
	eraseDisplay();
	nxtDisplayTextLine(0, "Testing motor B");
	ClearTimer(T1);
	motor[motorB] = DEF_SPEED;
	while(nMotorRunState[motorB] != runStateIdle){
		if(time100[T1] > 20) failState("Motor B failed",0);
	}
}


task main()
{
	if (nAvgBatteryLevel < LOW_BATTERY) failState("Battery is low",1);
	motorCheck();
	sensorCheck();
	srand(nSysTime);
	nSyncedMotors = synchAB;

	forward(DEF_SPEED,motorA);

	while(nNxtButtonPressed == -1){
		eraseDisplay();
		nxtDisplayString(1, "Front: %d", SensorValue(bumpFront));
		nxtDisplayString(2, "Back: %d", SensorValue(bumpBack));
		nxtDisplayString(3, "Sonar: %d", SensorValue(sonarSensor));
		nxtDisplayString(4, "Light %d", SensorValue(lightSensor));
  	wait1Msec(50);

  	if(nAvgBatteryLevel < LOW_BATTERY) failState("Battery is low",1);
  	if(SensorValue(bumpFront)){
  		PlaySound(soundLowBuzzShort);
  		backAndTurn(3);
  	}
  	if(SensorValue(bumpBack)) backBumperTriggered();
  	if(SensorValue(sonarSensor)<SONAR_DISTANCE) sonarTriggered();
  	if(SensorValue(lightSensor)<DARK_LIMIT) lightSensorTriggered();
  	alive();
	}
}

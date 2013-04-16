#pragma config(Sensor, S1,     bumpFront,         sensorTouch)
#pragma config(Sensor, S2,     bumpBack,         sensorTouch)

void forward(int speed, int masterMotor){
	nSyncedTurnRatio = 100;
	motor[masterMotor] = speed;
}

void backward(int speed, int masterMotor){
	nSyncedTurnRatio = 100;
	motor[masterMotor] = -speed;
}

void turn_left(int speed, int masterMotor, int ms)
{
	nSyncedTurnRatio = -100;
	motor[masterMotor] = speed;
	wait1Msec(ms);
}

void stop(int masterMotor){
	motor[masterMotor] = 0;
}

void safeState(const string sensor){
	stop(motorA);
	while(nNxtButtonPressed == -1){
		eraseDisplay();
		nxtDisplayString(1, "%s", sensor);
		wait1Msec(50);
	}
	wait1Msec(500);
	forward(100,motorA);
}

void failState(const string fail){
	stop(motorA);
	while(nNxtButtonPressed == -1){
		eraseDisplay();
		nxtDisplayString(1, "%s", fail);
		wait1Msec(50);
	}
}

void batLow(void){
	stop(motorA);
	eraseDisplay();
	nxtDisplayString(1, "%s", "Battery low!");
}

void backBumper(void){
	stop(motorA);
	eraseDisplay();
	nxtDisplayString(1, "%s", "Back bumper!");
	wait1Msec(2000);
}

void frontBumper(void){
	stop(motorA);
	eraseDisplay();
	nxtDisplayString(1, "%s", "Back bumper!");
	nMotorEncoder[motorA] = 0;
	nMotorEncoderTarget[motorA] = 720;
	backward(40,motorA);
	while(nMotorRunState[motorB] != runStateIdle){
		if(SensorValue(bumpBack)) failState("I think i'm stuck");
	}
	wait1Msec(1000);
	turn_left(50,motorA,(random(180)+60));
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
  	if(SensorValue(bumpFront))frontBumper();
  	if(SensorValue(bumpBack))safeState("Back Bumper");
 // 	if(nAvgBatteryLevel < 6100)batLow();
	}
}

#pragma config(Sensor, S1,     bumpFront,         sensorTouch)
#pragma config(Sensor, S2,     bumpBack,         sensorTouch)



task main()
{
	eraseDisplay();
  while(1){
  	forward()
  	nxtDisplayString(1, "%d", SensorValue(bumpFront));
  	nxtDisplayString(2, "%d", SensorValue(bumpBack));
  	wait1Msec(50);
  	eraseDisplay();
	}
}

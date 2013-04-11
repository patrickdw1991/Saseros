#pragma config(Sensor, S4,sonarSensor,sensorSONAR) // configurating the ultrasonic sensor

task main()
{
	eraseDisplay();
  while(1){
  	nxtDisplayString(1, "%d", SensorValue[sonarSensor]);
  	wait1Msec(50);
  	eraseDisplay();
	}
}

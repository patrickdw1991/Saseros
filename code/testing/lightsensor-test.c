#pragma config(Sensor, S3,     lightSensor,         sensorLightActive)

task main()
{
	eraseDisplay();
  while(1){
  	nxtDisplayString(1, "%d", SensorValue[lightSensor]);
  	wait1Msec(50);
  	eraseDisplay();
	}
}

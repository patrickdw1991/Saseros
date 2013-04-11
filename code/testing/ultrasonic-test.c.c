// Testing program to get values from the ultrasonic sensor

// Config the type of sensor on the specific port
#pragma config(Sensor, S4,sonarSensor,sensorSONAR)

task main(){
	eraseDisplay();
	while(1){
		// Display the values from the ultrasonic sensor on the display
		nxtDisplayString(1, "%d", SensorValue[sonarSensor]);
  	wait1Msec(50);
  	eraseDisplay();
	}
}

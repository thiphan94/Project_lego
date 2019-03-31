
#include "hitechnic-angle.h"

task main () {

#ifdef NXT
  nNxtButtonTask  = -2;
#endif

  displayCenteredTextLine(0, "HiTechnic");
  displayCenteredBigTextLine(1, "Angle");
  displayCenteredTextLine(3, "Test 1");
  displayCenteredTextLine(5, "Connect sensor");
  displayCenteredTextLine(6, "to S1");
  sleep(2000);

  eraseDisplay();
  displayCenteredTextLine(0, "HiTechnic Angle");
  displayTextLine(1, "-------------------");
  displayTextLine(5, "-------------------");

  // Create struct to hold sensor data
  tHTANG angleSensor;

  // Initialise and configure struct and port
  initSensor(&angleSensor, S1);

  while (true) {

    // Reset all the values in the sensor
    if (getXbuttonValue(xButtonEnter)) {
      resetSensor(&angleSensor);
      while (!getXbuttonValue(xButtonAny)) sleep(1);
    }

    // If the left button is pressed, set the 0 point to the current angle
    else if (getXbuttonValue(xButtonLeft)) {
      resetAngle(&angleSensor);
      while (!getXbuttonValue(xButtonAny)) sleep(1);

    // Reset the total accumulated angle to 0
    } else if (getXbuttonValue(xButtonRight)) {
       resetAccmulatedAngle(&angleSensor);
      while (!getXbuttonValue(xButtonAny)) sleep(1);
    }

    // Read the data from the sensor
    readSensor(&angleSensor);
    displayTextLine(2, "Ang: %7d deg", angleSensor.angle);


    displayTextLine(3, "Tot: %7d deg", angleSensor.accumlatedAngle);


    displayTextLine(4, "RPM: %7d", angleSensor.rpm);
    displayTextLine(6, " < Set angle");
    displayTextLine(7, "   Reset total >");
    sleep(50);
  }
}

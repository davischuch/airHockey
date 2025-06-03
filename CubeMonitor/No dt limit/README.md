In this example, the sensor was simply moved do the right (positive X axis displacement).

Note that Ax resolution is not stable. Some really sharp peaks are measurements with 1ms resolution, but then follows a big plateau, in which the acceleration data stays constant for up to 100ms.

This is likely because the data reading is being done too frequently, and the registrers or I2C communication is messed up.

It is no surprise that this causes extremely inaccurate velocity values.

For the next test there is going to be a delay between measurements.


ps: the dips to 0 of any variable aren't real; their measurement was just made between the shifting of the array and the next measurement/calculation.
Now Ax resolution is constant - 50ms.

We see that the velocity is more accurate to its actual values. The problem here is that it couldn't go back to a near zero value, because the negative acceleration (break) was too sudden.

This way, the 50ms resolution wasn't enough to calculate the actual velocity, which should become zero right after the first acceleration wave (positive X axis displacement).

The second wave is a negative X axis displacement. We see the acceleretion wave start negative, then become positive as expected. Velocity decreased, but didn't return to its original value as it should. Again, probably because losing speed is quicker than gaining speed, so the low-resolution measurements can't detect that the backwards acceleration were enough to completely stop the sensor.

This can be solved both by increasing resolution and with the square velocity filter.

Next, a timer will be used to measure the data at a constant rate, and Tx will be arbitrary instead of measured.
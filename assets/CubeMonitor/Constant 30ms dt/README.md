TIM7 was set to 30ms. Now measurement of MPU is made only after each TIM7 interrupt.

By seeting dt as a constant value, we see that Tx increments correctly 30ms each 30ms.

Ax now has a constant 30ms sample rate too. We see that there's no more peaks and plateaus - just constant 30ms period of measurement.

During the negative X axis displacement displayed, we see that Vx correctly become negative and then came back to similar levels as it were before.

From now, more resulution (<30ms) will be studied and considered. When the best resolution possible be found, the sigmoid and square filters will be applied again.

After this, supposedly we'll at least have a Px that follows the direction of movement of the sensor.
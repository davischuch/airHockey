Now we see a good resolution of acceleration readings, with constant 1ms between them.

When the sensor accelerates then stops, the speed went back to ~0.032m/s instead of 0m/s. This is expectable, and actually it is pretty good, even better than expected.

Position grew to ~47cm, which is about what it actualy moved. On the other hand, when it was pulled back, Px decreases to only ~15cm instead of 0cm. Maybe this was because of the speed offset. Sigmoid and square filters will be applied.

ps: don't mind the spikes to zero - they're just there because CubeMonitor reads data between array shifting and the next measurement.
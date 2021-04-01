修改源代码，主要库在lib目录下。

注意: 如果在Arduino IDE环境下，注释掉arduino.ino第一行。

对于每个电机，都有个对应的任务队列，所以例如给底盘旋转分配任务，rotateRotationBaseTo，会在当前这个点击任务队伍清空后，才执行。

stepperMotor顶层函数: delayRotationBase, rotateRotationBaseTo, delayLiftBase, liftBaseTo, delayMoveBase, moveBaseTo, 详细见lib/stepperMotor注释
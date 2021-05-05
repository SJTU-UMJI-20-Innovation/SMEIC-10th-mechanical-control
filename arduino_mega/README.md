按照以下步骤实现电脑网页端直接控制主体机械部分:

1.打开路由器，树莓派，8V，24V电源，确保路由器与树莓派网线连接，树莓派通电，树莓派与arduino MEGA接线正常

2. 电脑连接到Lavosier wifi

3. 打开命令行，ssh 登录树莓派，指令为: "ssh pi@192.168.1.3"（引号内为指令），密码为raspberry

4."cd server", "node websocket-to-spi.js"（可以按下tab），保持这个状态（退出按下control D）

5.打开html文件夹下index.html

6.在输入框内输入指令，send（支持多行指令）。对于输入指令的反馈会打在下方的文本框内

具体指令规范在raspberry---arduino文件内，不要输入目前未实现的指令，可能导致错误。若有错误发生，重新烧程序到arduino MEGA中

假如期望得到arduino MEGA反馈信息，可以连接上arduino MEGA，串口监视器会显示更多信息
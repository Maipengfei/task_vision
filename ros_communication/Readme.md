#基于话题实现两个节点间的通讯

#前提：已经在本地配置好ros

#实现流程：
    1）自定义话题消息类型person.msg 存储路径：./catkin_ros/src/node_communication/msg
    2)编写发布者（output_pub.cpp）和订阅者代码（cal_scr.cpp），储存在功能包node_communication下的src中
    3）执行步骤：i）打开终端， 输入 ‘roscore’ ，打开ros master
		 ii）再打开两个新终端，分别执行发布者和订阅者代码，即可实现如图效果
    4）具体代码功能实现流程在发布者（output_pub.cpp）和订阅者代码（cal_scr.cpp）皆有注释。
    
#打开三个终端分别执行代码：
   ##1. 首先配置环境变量
      (i)cd ~
      (ii)gedit .bashrc #打开系统环境，并将一下内容粘贴在文件的末尾

		# Set ROS melodic
		source /opt/ros/melodic/setup.bash
		source ~/catkin_ws/devel/setup.bash
		source ~/catkin_ros/devel/setup.bash

		# Set ROS Network
		#ifconfig查看你的电脑ip地址，记得修改ip地址，否则无法打开rosmaster
		export ROS_HOSTNAME=IP地址
		export ROS_MASTER_URI=http://${ROS_HOSTNAME}:11311
		 
		# Set ROS alias command 快捷指令
		alias cw='cd ~/catkin_ws'
		alias cs='cd ~/catkin_ws/src'
		alias cm='cd ~/catkin_ws && catkin_make'
		alias cr='cd ~/catkin_ros'
		
   ##2. 接着关闭终端重新打开，运行命令 roscore
      打开ros master（负责管理ros下各节点）
      
   ##3. 打开新终端，cd到根目录catkin_ros后执行 catkin_make
      生成两个可执行文件output_pub cal_scr
      
   ##4. 在该终端下执行发布者代码，命令：./output_pub
      打开新终端执行订阅者代,命令：./cal_scr
      tips:二者执行顺序不影响结果



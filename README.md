# Smart_car
电磁循迹小车
## PID
### 位置pid
位置式 PID 算法的特点：由于全量输出，所以每次输出均与过去状态有关， 
计算时要对 ek 进行累加，工作量大；并且，因为计算机输出的 uk 对应的是执 
行机构的实际位置，如果计算机出现故障，输出的 uk 将大幅度变化，会引起执 
行机构的大幅度变化，有可能因此造成严重的生产事故，这在实际生产中是不允 
许的。 
### 增量pid
所谓增量式 PID 是指数字控制器的输出只是控制量的增量∆uk。当执行机 
构。需要的控制量是增量，而不是位置量的绝对数值时，可以使用增量式 PID 控 
制算法进行控制。

### 模糊pid

由于智能车系统具有时变性、滞后性和非线性等特点, 在智能车行驶过程中 
建立能够实时调节控制系统且精确的数学模型很困难, 故而根据系统工作需求 
特点,采用模糊算法实现对智能车的有效控制，具体实现如下三个步骤。 
（1）控制系统的建立 
为了实现对小车的精确控制，故进行以下控制系统的设计： 
（2）对模糊 PID 控制器的设计 
在保留原有PID控制器的基础上，根据模糊表规则完成对变量的调节控制， 
从而满足在不同赛道、车身姿态的输出需求，保持系统稳定性及动态响应效果。 
（3）对输入输出量进行模糊化 
将赛道曲率模糊化分为零、极小、小、大、非常大。（其中如果曲率为零， 
表示曲率半径无穷大，该赛道为直道）。模糊子集就定义为 Curve ={零，极小， 
小，大，非常大}。再将曲率量化为 7 个等级，分别为 0，1，2，3，4，5，6。 
那么曲率的论域就为 Curve={0，1，2，3，4，5，6}。调试小车在弯道上的路径 
即可得出 Curve 量的隶属度表。相同的，也可以得出 kp、ki、kd 的隶属度表。 
最后根据模糊控制规则,经由系统模糊合成可以得到关于 kp、ki、kd 的各个修正 参数，再加到传统 pid 控制上。
舵机模糊位置PD控制算法
在经过反复的传统 PID 参数调试后，我们发现单一 PD 变量在车速较慢时 
可以较好的控制小车，在转弯时打角正常，车身姿态也较好。但车速上升后，小 
车在曲率不同的弯道时路径不能同时到达一个很好的效果。这是由于赛道是不 
可确定的，车速也是随时在变的。于是我们采用了模糊 PD 对舵机进行控制，我 
们用车速和摄像头得到的偏差共同控制舵机的 P。但在小车行驶过程中，小车需 
要较快的响应以消除偏差，我们最终决定采用静态 D 值

模糊控制器的设计主要包括输入模糊化、模糊规则的建立、模糊推理、逆模 
糊化四个部分。




### 整定参数 
速度环 增量pid 
i从小到大 响应加快 出现抖动时
调p 消抖



速度环 模糊pid

参数 后排左右横电感的差值的绝对值以及他的变化率 

对模糊表进行标定  

确定差值的范围 可通过代码来测得最大和最小 
确定变化率的范围  也可通过代码来测的最大和最小
PID代码



舵机pid算法


int16 pid_duoji_fork(float dig)//差比和  dig 0~1
{
	static float error = 0;
	static float last_error = 0;
	static int16 servoDuty = 0;
	error = dig;
	//duojikp = Fuzzy_Update(&Duoji_P,error,last_error-error);
	servoDuty =  duojikp_fork* error  +duojikd_fork* (last_error-error);
	last_error = error;
	return servoDuty;
}

电机pid
int Incremental_PI_L (int16 Encoder,int16 Target)
{ 	
	 static int16 Bias=0,Last_bias=0,Last_last_bias=0;
	 static int16 Pwm = 0;
	 Bias = Target-Encoder; //Calculate the deviation //计算偏差
	 if(Bias>=60)
	 {
		 Pwm = 7200;
	 }
	 else
	 {
		 Pwm += Velocity_KP_L * (Bias-Last_bias) + Velocity_KI_L* Bias + Velocity_KD_L*(Bias-2* Last_bias + Last_last_bias); 
		 if(Pwm>7200)Pwm= 7200;
		 if(Pwm<-7200)Pwm= -7200;
	 }
	 Last_last_bias= Last_bias;
	 Last_bias=Bias; //Save the last deviation //保存上一次偏差
	 return Pwm;    
}

外加棒棒算法
当偏差值大于一定阈值，以最大pwm输出。
if(Bias>=60)
{
	 Pwm = 7200;
}




# 关于STC
51内核
30Mhz
资源太少
5个定时器
2个用去编码器
1个5ms速度环
1个蓝牙模块
1个超声波模块

工作频率太低
想要快速处理速度环
在运行期间需要将屏幕等其他与跑赛道无关的挂起

在主函数的while中执行运行函数
先是进行ADC采集的平均滤波，再对状态判断来识别元素
再执行相应的状态机的处理函数
完成赛道循迹，在进行对舵机pwm赋值，
现在改进方案就是将复杂算法改进成简单耗时少
对逻辑进行优化，去掉不必要的条件判断
再者减少ADC采集的次数 目前是10次


当电量不足时，在运行过程中，会导致元素误判。

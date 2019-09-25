#ifndef CAR_H_
#define CAR_H_
#include<iostream>
#include<vector>
#include<queue>
#include "read_txt.h"
#include "path.h"
using namespace std;

// version-4 编译通过
// 4. 增加数据 real_go_time 指示汽车实际出发时间

class car
{
public:
	// 直接读取的汽车向量信息
	int id;	 			// car ID
	int start;			// 始发路口ID
	int target;			// 目的地路口ID
	int max_vel;		// 汽车最大速度
	int go_time;		// 汽车最早出发时间

	bool state;			// 汽车在当前调度时间片的调度状态  1.等待调度-true  2.终止调度-false
	int cur_roadID;		// 汽车当前道路ID
	int next_roadID;	// 汽车下一目标道路ID
	int facing_cross;	// 汽车面对的路口
	int vel;			// 汽车在当前调度时间片的实际移动速度 min(max_vel, road_vel, s/t)
	bool is_drive;		// 汽车是否已经上路
	bool is_reach;		// 汽车是否到达终点
	bool is_wait;		// 汽车是否在起点
	int wait_car;
	
	vector<int> cross_path;  // 汽车要经过的路口路径
	vector<int> road_path;	// 汽车要经过的道路路径

	int real_go_time;	// 汽车实际出发时间
	vector<int> path;	// 汽车走过的路径
public:

	car();
	car(vector<int> c);
	~car();
	
	void back_start();
	void end_run();
	void start_run();
	void update_stateOfCar();
	

};

#endif










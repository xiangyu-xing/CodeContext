#ifndef CROSS_H
#define CROSS_H

//version-3 编译完成
#include <vector>
#include <map>
#include "read_txt.h"
#include "car.h"
#include "road.h"
#include <algorithm>
//#include "short_path.h"
using namespace std;
extern map<const int, vector<int>>map_path;

extern int carInRoad;

class cross
{
public:
	cross();
	cross(vector<int>&cross_data); //初始化一个路口对象
	~cross();

	int left(int cur_road);		//得到当前道路左转所到达的道路
	int right(int cur_road);	//得到当前道路右转所到达的道路
	int straight(int cur_road);	//得到当前道路直行所到达的道路
	int getid();	//得到id值
	vector<int> get_cross();	//得到id路口的所有道路信息
	bool is_left(int from, int to);		//判断从道路（from）去往目的道路（to）是否是左转
	bool is_right(int from, int to);	//判断从道路（from）去往目的道路（to）是否是右转
	bool is_straight(int from, int to);	//判断从道路（from）去往目的道路（to）是否是直行
	int direction(int from, int to);	//判断从道路（from）去往目的道路（to）是按什么方向前进（-1不存在、0右转、1左转、2直行）


	bool run_cross();//负责当前时间所连接道路上可通过路口车辆的行驶，决定其行驶顺序


	bool is_in(int road_id); //判断从当前路口能否进入指定道路
	bool is_in(road road_name);


	int cross_road[4];		//记录四条道路
	int cross_id;		//记录路口id
private:
	int getTHeWaitCar(int carId); //返回>0的数，可以通过；返回0,只能在当前道路；返回<0的数，等待该车辆
	void back_car(int carId);//当车辆无路可走时用来回到原点
};
#endif











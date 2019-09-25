//#include "stdafx.h"
#include"car.h"
#include "read_txt.h"
#include <iostream>
using namespace std;

// version-3 编译通过

car::car()
{
}

car::car(vector<int> c)
{
	id = c[0];
	start = c[1];
	target = c[2];
	max_vel = c[3];
	go_time = c[4];
	is_drive = false;
	is_reach = false;
	is_wait = true;
	state = false;			// 汽车在当前调度时间片的调度状态  1.等待调度-true  2.终止调度-false
	cur_roadID = 0;			// 汽车当前道路ID
	next_roadID = 0;	    // 汽车下一目标道路ID
	facing_cross = start;		// 汽车面对的路口
	vel = 0;				// 汽车在当前调度时间片的实际移动速度 min(max_vel, road_vel, s/t)
}

car::~car()
{
}


// 更新汽车走过的路径

void car::end_run()
{
	is_drive = false;
	is_reach = true;
	is_wait = false;

}

void car::start_run()
{
	is_wait = false;
	is_drive = true;
	is_reach = false;
}
void car::update_stateOfCar()
{
	if (road_path.front() != -1)
	{
		 /*更新地图的权值asd*/
		if (cur_roadID != 0)
		{
			int fromId = map_road.at(cur_roadID).fromCrossID;
			int toId = map_road.at(cur_roadID).toCrossID;
			if (fromId == facing_cross)
			{
				reduce_Graph(toId, fromId);
			}
			else if (toId == facing_cross)
			{
				reduce_Graph(fromId, toId);
			}
		}
		/*更新车辆本身的属性asd*/
		cur_roadID = road_path.front();
		path.push_back(road_path.front());
		road_path.erase(road_path.begin());
		next_roadID = road_path.front();
		cross_path.erase(cross_path.begin());
		facing_cross = cross_path.front();
	}
}

void car::back_start()
{
	cur_roadID = 0;
	is_drive = false;
	is_reach = false;
	is_wait = true;
	if (!cross_path.empty())//如果不是空的，需要在删除路径前，将道路使用次数减去
	{
		vector<int>&tmp = cross_path;
		for (int i = 0; i < tmp.size() - 1; i++)
		{
			reduce_Graph(tmp[i], tmp[i + 1]);
		}
	}
	road_path.clear();
	cross_path.clear();
	path.clear();
	wait_car = 0;
	facing_cross = start;
}


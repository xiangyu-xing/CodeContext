//#include "stdafx.h"
#include "cross.h"
//version-3 编译完成
map<const int, vector<int>>map_path;

int carInRoad;
cross::cross()
{
	cross_id = -1;
	cross_road[0] = -1;
	cross_road[1] = -1;
	cross_road[2] = -1;
	cross_road[3] = -1;
}

cross::cross(vector<int>&cross_data)
{
	cross_id = cross_data[0];
	cross_road[0] = cross_data[1];
	cross_road[1] = cross_data[2];
	cross_road[2] = cross_data[3];
	cross_road[3] = cross_data[4];
}


cross::~cross()
{
}
int cross::getid() 
{
	return cross_id;
}
	
int cross::left(int cur_road)
{
	if (cur_road == cross_road[0])return cross_road[1];
	else if (cur_road == cross_road[1])return cross_road[2];
	else if (cur_road == cross_road[2])return cross_road[3];
	else if (cur_road == cross_road[3])return cross_road[0];
	return -1;
}
int cross::right(int cur_road) 
{
	if (cur_road == cross_road[0])return cross_road[3];
	else if (cur_road == cross_road[1])return cross_road[0];
	else if (cur_road == cross_road[2])return cross_road[1];
	else if (cur_road == cross_road[3])return cross_road[2];
	return -1;
}
int cross::straight(int cur_road)
{
	if (cur_road == cross_road[0])return cross_road[2];
	else if (cur_road == cross_road[1])return cross_road[3];
	else if (cur_road == cross_road[2])return cross_road[0];
	else if (cur_road == cross_road[3])return cross_road[1];
	return -1;
}
bool cross::is_left(int from, int to)
{
	if (left(from) == to)return true;
	else return false;
}
bool cross::is_right(int from, int to)
{
	if (right(from) == to)return true;
	else return false;
}
bool cross::is_straight(int from, int to)
{
	if (straight(from) == to)return true;
	return false;
}
int cross::direction(int from, int to)
{
	if (is_right(from, to))return 0;
	if (is_left(from, to))return 1;
	if (is_straight(from, to))return 2;
	return -1;
}
//bool cross::run_cross()
//{
//	int have_run = 0;//用于记录本次成功过路的车的数量 ，如果本次没有车辆移动，则返回false
//					 //将当前路口所连道路上的优先级最高的车保存在carid中，接下来通过交规来判断这几辆车的行进优先级
//	vector<int>carid;
//	for (int i = 0; i < 4; i++)
//	{
//		if (cross_road[i] != -1)	 //判断是否有这条路
//			carid.push_back(map_road.at(cross_road[i]).findFirstCar());
//	}
//	map<int, int>map_run;//key为目标道路编号，value为当前可进入的车辆编号
//
//	for (auto id : carid)
//	{
//		lastcar.clear();
//		if (id == -1)continue;//如果车辆不能移动或者没有车辆要走，车辆编号为-1，看下一辆车
//							  //p.Dijikstra(map_car.at(id)); //重新计算一次车辆的下一道路，以免之前的车辆干扰
//							  //////////////////////////////////////////////////////////////////////////
//		lastcar.push_back(id);
//
//		if (map_car.at(id).next_roadID == -1)
//		{
//			have_run++;//如果有到达终点的，已走的车辆数加1
//
//			map_car.at(id).end_run();
//			vector<int>path{ map_car.at(id).real_go_time };
//			path.insert(path.end(), map_car.at(id).path.begin(), map_car.at(id).path.end());
//			map_path[id] = path;//将车辆路径存入
//			map_road.at(map_car.at(id).cur_roadID).removeCar(id);//从道路上删除车辆
//			map_car.erase(id);//从车辆map里删除已到达终点的车辆
//			carInRoad--;
//			continue;
//		}//如果下一道路-1，说明已到达终点 ，可以看下一辆车了
//		 //////////////////////////////////////////////////////////////////////////
//		int next_id = getTHeWaitCar(id);																											   //next_id如果为负数，则为下一道路上第一优先级车编号的负数
//		if (next_id == 0)
//		{
//			map_road.at(map_car.at(id).cur_roadID).moveA2B(id, map_car.at(id).next_roadID);
//			have_run++;
//			continue;
//		}//判断这辆车能否到达下一条路，如果不能到达，则不计入map_run,但是需要让其行驶
//
//		if (next_id < 0)		 //如果前放有等待车辆堵路
//		{
//			map<int, int>loop_car;
//			loop_car[id]++;
//			while (next_id < 0)
//			{
//				next_id = -next_id;
//				loop_car[next_id]++;																						   //if (next_id == id)
//				if (loop_car.at(next_id) > 1)
//				{
//					p.Dijikstra(map_car.at(next_id), true);//如果出现闭环，为当前车辆重新规划路线
//					break;//之所以不进行新的闭环检测，是为了避免一开始的大量运算，可以在下一轮检测，认为重新规划后没有闭环
//				}
//				else
//				{
//					if (map_car.at(next_id).next_roadID == -1)
//						break;//如果下一车辆要到达终点，则说明没有闭环
//					else
//						next_id = getTHeWaitCar(next_id);
//				}
//			}
//		}  //如果返回负数，说明前方车辆为等待状态,检查是不是形成了闭环，如果形成了闭环，换一个方向
//		   //////////////////////////////////////////////////////////////////////////
//		if (map_run.find(map_car.at(id).next_roadID) != map_run.end()) //判断是否有汽车nextroad相互重复，若有要判断优先级
//			if (direction(map_car.at(id).cur_roadID, map_car.at(id).next_roadID) > direction(map_car.at(map_run[map_car.at(id).next_roadID]).cur_roadID, map_car.at(id).next_roadID))
//				map_run[map_car.at(id).next_roadID] = id;	  //如果优先级高，则用优先级高的车辆来代替原有车辆进入该道路
//			else continue;
//		else map_run[map_car.at(id).next_roadID] = id;//当前车辆的nextroad没有车辆要进入，直接存储
//	}
//	//接下来移动车辆
//
//
//	auto beg = map_run.begin();
//	while (beg != map_run.end())
//	{
//		int id = beg->second;
//		int next_id = map_road.at(map_car.at(id).cur_roadID).getlengthNextRoad(id, map_car.at(id).next_roadID);//记录前方等待车辆的id
//		if (next_id < 0)
//		{
//			beg++;
//			continue;
//		}
//		//这里调用road的函数
//		auto road_A = map_car.at(beg->second).cur_roadID;
//		if (map_road.at(road_A).moveA2B(beg->second, beg->first))
//			have_run++;//有车辆成功过路，则已过路车辆数加1
//		beg++;
//	}
//	if (have_run == 0)return false;	 //说明没有车要走，结束
//	return true;	//执行成功，返回true
//}


bool cross::is_in(int road_id)
{
	if(map_road.at(road_id).isDuplex==false&&map_road.at(road_id).fromCrossID!=cross_id)
		return false;
	return true;
}

bool cross::is_in(road road_name)
{
	is_in(road_name.id);
	return false;
}

int cross::getTHeWaitCar(int carId)
{

	int cur_cross = map_car.at(carId).facing_cross;//得到当前车辆面对的路口
	setCrossID(cur_cross);//改变当前的路口
	int next_id = map_road.at(map_car.at(carId).cur_roadID).getlengthNextRoad(carId, map_car.at(carId).next_roadID);//记录前方等待车辆的id
	if (next_id == 0)
	{
		setCrossID(cross_id);//把路口数值返回正常值
		return 0;
	}
	else//如果车辆拥有过路口的能力
	{
		int cur_carId = carId;//当前的第一优先级的车辆
		int cur_car_curRoad = map_car.at(cur_carId).cur_roadID;
		int cur_car_nextRoad = map_car.at(cur_carId).next_roadID;
		//优先判断路口的等待，该等待优先级高于所要前往路的等待
		for (int i = 0; i < 4; i++)
		{
			int roadId = map_cross.at(cur_cross).cross_road[i];
			if (roadId != -1)	 //判断是否有这条路
			{
				int next_carId = map_road.at(roadId).findFirstCar();
				if (next_carId == -1)continue;
				int next_car_curRoad = map_car.at(next_carId).cur_roadID;
				int next_car_nextRoad = map_car.at(next_carId).next_roadID;
				if (cur_car_nextRoad==next_car_nextRoad)
				{
					if (map_cross.at(cur_cross).direction(next_car_curRoad, next_car_nextRoad)
		> map_cross.at(cur_cross).direction(cur_car_curRoad, cur_car_nextRoad))
					{
						cur_carId = next_carId;
						cur_car_curRoad = next_car_curRoad;
				   }
				}
			}	
		} //决断出路口等待的车辆
		if (cur_carId != carId)next_id = -cur_carId;
		setCrossID(cross_id);
		return next_id;
	}
	return -1;
}

void cross::back_car(int carId)
{
	setCrossID(map_car.at(carId).facing_cross);
	int roadId = map_car.at(carId).cur_roadID;
	map_road.at(roadId).removeCar(carId);//从道路上移出车辆
	map_car.at(carId).back_start();	//恢复车辆状态
	int s_time = map_car.at(carId).go_time;
	car_wait.at(s_time).insert(*map_car.find(carId));  //在等待车辆中添加该车辆
	carInRoad--;
	setCrossID(cross_id);
}


vector<int> cross::get_cross()
{
	vector<int>vec{ cross_road[0],cross_road[1],cross_road[2],cross_road[3] };

	return vec;
}

bool cross::run_cross()
{
	int have_run = 0;//用于记录本次成功过路的车的数量 ，如果本次没有车辆移动，则返回false
					 //将当前路口所连道路上的优先级最高的车保存在carid中，接下来通过交规来判断这几辆车的行进优先级
	for (int i = 0; i < 4; i++) {
		int cur_roadId = cross_road[i];
		int num = 0;   //负责记录当前道路通过车辆的数目
		if (cur_roadId == -1)continue;
		else {
			while (1) {
				int id = map_road.at(cur_roadId).findFirstCar();
				if (id == -1)break;
				else {
					if (map_car.at(id).next_roadID == -1) {
						num++;
						map_car.at(id).end_run();
						vector<int>path{ map_car.at(id).real_go_time };
						path.insert(path.end(), map_car.at(id).path.begin(), map_car.at(id).path.end());
						map_path[id] = path;//将车辆路径存入
						map_road.at(map_car.at(id).cur_roadID).removeCar(id);//从道路上删除车辆
						map_car.erase(id);//从车辆map里删除已到达终点的车辆
						carInRoad--;
					}
					else {
						int next_id = getTHeWaitCar(id);
						if (next_id > 0) {
							map_road.at(map_car.at(id).cur_roadID).moveA2B(id, map_car.at(id).next_roadID);
							num++;
						}
						if (next_id == 0) {
							map_road.at(map_car.at(id).cur_roadID).moveA2B(id, map_car.at(id).next_roadID);
							num++;
						}

						else if (next_id < 0)		 //如果前放有等待车辆堵路
						{
							map<int, int>loop_car;
							loop_car[id]++;
							while (next_id < 0)
							{
								next_id = -next_id;
								loop_car[next_id]++;
								if (map_car.at(next_id).next_roadID == -1)
									break;//如果下一车辆要到达终点，则说明没有闭环
								if (loop_car.at(next_id) > 1)
								{
									auto pos = loop_car.find(next_id);
									while (!update_carPath(pos->first, true))
									{
										pos++;
										if (pos == loop_car.end())
										{
											back_car(next_id);
											break;
										}
									};
									/*for (auto c : have_d)
									{
										if (c == next_id)
										{
											back_car(next_id);
											break;
										}
									}*/
								//update_carPath(next_id, true);//如果出现闭环，为当前车辆重新规划路线
								//have_d.push_back(next_id);
								/*	if (map_car.at(next_id).next_roadID == -1)
									{
										back_car(next_id);
										break;
									}
									if (getTHeWaitCar(next_id) <= 0)
									{
										back_car(next_id);
									}*/
									
									break;//之所以不进行新的闭环检测，是为了避免一开始的大量运算，可以在下一轮检测，认为重新规划后没有闭环
								}
								else
								{
									if (map_car.at(next_id).next_roadID == -1)
										break;//如果下一车辆要到达终点，则说明没有闭环
									else
										next_id = getTHeWaitCar(next_id);
								}
							}
							 break;
						}  //如果返回负数，说明前方车辆为等待状态,检查是不是形成了闭环，如果形成了闭环，换一个方向

					}
				}
			}
			//if (num ==0) {
			//	int cost_of_add = 0;
			//	if (crossIDProcessNow_glable == map_road.at(cur_roadId).fromCrossID) {
			//		cost_of_add = map_road.at(cur_roadId).saturation.first;
			//		if (cost_of_add > 0.7)
			//			p.g.edgs_matrix[map_road.at(cur_roadId).toCrossID][crossIDProcessNow_glable] *= 3;
			//	}

			//	else {
			//		cost_of_add = map_road.at(cur_roadId).saturation.second;
			//		if (cost_of_add > 0.7)
			//			p.g.edgs_matrix[map_road.at(cur_roadId).fromCrossID][crossIDProcessNow_glable] *= 3;
			//	}
			//}
		}
	}
	return false;
}


// ConsoleApplication5.cpp : 定义控制台应用程序的入口点。

//#include "stdafx.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "cross.h"
#include "answer.h"
#include "road.h"
#include "car.h"
#include "read_txt.h"
#include "short_path.h"

using namespace std;



int main()
{

	string car_path = "D:\\桌面\\比赛\\code\\第九版新\\测试\\car.txt";
	string road_path = "D:\\桌面\\比赛\\code\\第九版新\\测试\\road.txt";
	string cross_path = "D:\\桌面\\比赛\\code\\第九版新\\测试\\cross.txt";
	string answer_Path = "D:\\桌面\\比赛\\code\\第九版新\\测试\\answer.txt";

	read_txt(car_path, map_car);
	read_txt(road_path, map_road);
	read_txt(cross_path, map_cross);

	
	shortpath p;  //short_path的对象
	
	// cout << p << endl;
	
	for (auto &c : map_car)	//车辆上路的程序
	{
		c.second.state = true;  //重置每辆车的状态为可以移动
		
		p.Dijikstra(c.second);

		// c.second.update_stateOfCar();
		cout << c.second << endl;

		/***************************************** 分割线 *********************************************/
		
		cout << c.second;
		cout << "cross_path: " << endl;
		while (!c.second.cross_path.empty())
		{
			int temp = c.second.cross_path.front();
			cout << temp << "  ";
			c.second.cross_path.pop();

		}
		cout << endl;

		cout << "road_path: " << endl;
		while (!c.second.road_path.empty())
		{
			int temp = c.second.road_path.front();
			cout << temp << "  ";
			c.second.road_path.pop();
		}

		cout << endl << endl;
		
	}

	
	 car & c = map_car.at(13142);

	
	p.Dijikstra(c, true);
	
	//cout << c;
	//c.update_stateOfCar();
	//cout << c << endl;
	cout << "cross_path: " << endl;
	while (!c.cross_path.empty())
	{
		int temp = c.cross_path.front();
		cout << temp << "  ";
		c.cross_path.pop();

	}
	cout << endl;

	cout << "road_path: " << endl;
	while (!c.road_path.empty())
	{
		int temp = c.road_path.front();
		cout << temp << "  ";
		c.road_path.pop();
	}

	cout << endl << endl;

	

	return 0;

}
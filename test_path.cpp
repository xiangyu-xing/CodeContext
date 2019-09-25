// ConsoleApplication5.cpp : �������̨Ӧ�ó������ڵ㡣

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

	string car_path = "D:\\����\\����\\code\\�ھŰ���\\����\\car.txt";
	string road_path = "D:\\����\\����\\code\\�ھŰ���\\����\\road.txt";
	string cross_path = "D:\\����\\����\\code\\�ھŰ���\\����\\cross.txt";
	string answer_Path = "D:\\����\\����\\code\\�ھŰ���\\����\\answer.txt";

	read_txt(car_path, map_car);
	read_txt(road_path, map_road);
	read_txt(cross_path, map_cross);

	
	shortpath p;  //short_path�Ķ���
	
	// cout << p << endl;
	
	for (auto &c : map_car)	//������·�ĳ���
	{
		c.second.state = true;  //����ÿ������״̬Ϊ�����ƶ�
		
		p.Dijikstra(c.second);

		// c.second.update_stateOfCar();
		cout << c.second << endl;

		/***************************************** �ָ��� *********************************************/
		
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
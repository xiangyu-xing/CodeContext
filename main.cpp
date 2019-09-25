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
#include "path.h"

using namespace std;
void ceshi(int carid)
{
	int roadid = map_car.at(carid).cur_roadID;
	road temp_road = map_road.at(roadid);
	cout << "��·����" << temp_road.speed << "     " << "��·id��" << temp_road.id << endl;
	for (int i = 0; i < temp_road.matrixFrom.size(); i++)
	{
		for (int j = 0; j < temp_road.matrixFrom[0].size(); j++)
			cout << temp_road.matrixFrom[i][j] << " ";
		cout << endl;
	}

	for (int i = 0; i < temp_road.matrixTo.size(); i++)
	{
		for (int j = 0; j < temp_road.matrixTo[0].size(); j++)
			cout << temp_road.matrixTo[i][j] << " ";
		cout << endl;
	}

	cout << endl << endl;
	roadid = map_car.at(carid).next_roadID;
	temp_road = map_road.at(roadid);
	cout << "��·����" << temp_road.speed << "     " << "��·id��" << temp_road.id << endl;
	for (int i = 0; i < temp_road.matrixFrom.size(); i++)
	{
		for (int j = 0; j < temp_road.matrixFrom[0].size(); j++)
			cout << temp_road.matrixFrom[i][j] << " ";
		cout << endl;
	}

	for (int i = 0; i < temp_road.matrixTo.size(); i++)
	{
		for (int j = 0; j < temp_road.matrixTo[0].size(); j++)
			cout << temp_road.matrixTo[i][j] << " ";
		cout << endl;
	}

	cout << endl << endl << endl << endl;


}

void ceshi_road(int roadid)
{
	road temp_road = map_road.at(roadid);
	cout << "��·����" << temp_road.speed << "     " << "��·id��" << temp_road.id << endl;
	for (int i = 0; i < temp_road.matrixFrom.size(); i++)
	{
		for (int j = 0; j < temp_road.matrixFrom[0].size(); j++)
			cout << temp_road.matrixFrom[i][j] << " ";
		cout << endl;
	}

	for (int i = 0; i < temp_road.matrixTo.size(); i++)
	{
		for (int j = 0; j < temp_road.matrixTo[0].size(); j++)
			cout << temp_road.matrixTo[i][j] << " ";
		cout << endl;
	}

	cout << endl << endl << endl << endl;


}
void ceshi_cross(int crossid)
{
	for (int i = 0; i < 4; i++)
	{
		int roadid = map_cross.at(crossid).cross_road[i];
		if (roadid != -1)
		{
			road temp_road = map_road.at(roadid);
			cout << "��·����" << temp_road.speed << "     " << "��·id��" << temp_road.id << endl;
			for (int i = 0; i < temp_road.matrixFrom.size(); i++)
			{
				for (int j = 0; j < temp_road.matrixFrom[0].size(); j++)
					cout << temp_road.matrixFrom[i][j] << " ";
				cout << endl;
			}

			for (int i = 0; i < temp_road.matrixTo.size(); i++)
			{
				for (int j = 0; j < temp_road.matrixTo[0].size(); j++)
					cout << temp_road.matrixTo[i][j] << " ";
				cout << endl;
			}
			cout << endl << endl;
		}
	}
	cout << endl << endl << endl << endl;
}

int main()
{
	int wendang = 10;
	string car_path = "E:\\vs2015_workspace\\ConsoleApplication5\\SDK_C++\\config_" + to_string(wendang) + "\\car.txt";
	string road_path = "E:\\vs2015_workspace\\ConsoleApplication5\\SDK_C++\\config_" + to_string(wendang) + "\\road.txt";
	string cross_path = "E:\\vs2015_workspace\\ConsoleApplication5\\SDK_C++\\config_" + to_string(wendang) + "\\cross.txt";
	string answer_Path = "E:\\vs2015_workspace\\ConsoleApplication5\\SDK_C++\\config_" + to_string(wendang) + "\\answer.txt";
	//string car_path = "E:\\vs2015_workspace\\ConsoleApplication5\\SDK_C++\\1-map-training-" + to_string(wendang) + "\\car.txt";
	//string road_path = "E:\\vs2015_workspace\\ConsoleApplication5\\SDK_C++\\1-map-training-" + to_string(wendang) + "\\road.txt";
	//string cross_path = "E:\\vs2015_workspace\\ConsoleApplication5\\SDK_C++\\1-map-training-" + to_string(wendang) + "\\cross.txt";
	//string answer_Path = "E:\\vs2015_workspace\\ConsoleApplication5\\SDK_C++\\1-map-training-" + to_string(wendang) + "\\answer.txt";

	read_txt(car_path, map_car);
	read_txt(road_path, map_road);
	read_txt(cross_path, map_cross);
	car_num = map_car.size();
	
	
	update_Graph();//������ͼ
	for (auto &c : map_car)
	{
		int time = c.second.go_time;
		int Id = c.first;
		car_wait[time].insert({ Id,c.second });
	}	 //������������ʱ����� ��������������·

	
	
	carInRoad = 0;
	int time = 0;//ʱ��Ƭ
	while (map_car.size() != 0)	//���г�û�е����յ�,ÿ����һ���յ㣬run_cross��ɾ���ﵽ�յ�ĳ���
	{
		// ��·�ϵĳ�����ʼ��

		for (auto &c : map_car)
		{
			c.second.state = true;  //����ÿ������״̬Ϊ�����ƶ�
		}
		car_size_true = carInRoad; //��ʼ����·�Ͽ��ƶ���������Ŀ
		for (auto &c : map_road)   //ʹÿ����·�ϵĳ����������ܵ���
		{
			c.second.moveCarFirst();
		}
		while (car_size_true != 0)//��֤���г��ڵ�ǰʱ��Ƭ����ʻ��һ��
		{
			//have_d.clear();
			for (auto &c : map_cross) //��������·�ڣ���ÿ����·�������ܵ���ʻ����
			{
				setCrossID(c.first);
				while (c.second.run_cross());	 //��ʻһ��·���������ĵ�·�ϵĳ�����û��һ�������ߣ����������Ӧ��·�ϵĳ���״̬
			}
		}
		
		/************************************************************************/
		/* ����Ϊ����·�ϵĳ��ĳ�������Ϊδ��·�ĳ�����·����                      */
		/************************************************************************/
	
		++time;		
		auto waitBeg = car_wait.begin();
		while (waitBeg != car_wait.end())
		{
			if (waitBeg->first > time)break;//�����ʱ�����������ֹѭ��
			auto beg = waitBeg->second.begin();
			while (beg != waitBeg->second.end())  //����ӳ���������������ֹѭ��
			{
				if(map_car.at(beg->first).cross_path.empty())
					update_carPath(beg->first); //Ϊ������·�ĳ����滮·��
				int voidplace_of_add = 0;
				float cost_of_add = 1;
				auto &road_next = map_road.at(map_car.at(beg->first).next_roadID);	//�õ�Ԥ�ƽ����·������
				if (beg->second.start == road_next.fromCrossID)
				{
					cost_of_add = road_next.saturation.second;
					voidplace_of_add = road_next.voidPlace.second;
				}
				else
				{
					cost_of_add = road_next.saturation.first;
					voidplace_of_add = road_next.voidPlace.first;
				}
				if (voidplace_of_add)
				{
					if (addNewCar(beg->first) == true)	  //�ӳ��ɹ�����Ҫ��car_waitɾ����Ӧ�ĳ���
					{
						map_car.at(beg->first).real_go_time = time;
						carInRoad++;
						waitBeg->second.erase(beg++->first);
						continue;
					}
				}  //������ܼӳ�
				beg++;
			}
			waitBeg++;
		}
		/*************************************** �����ô��� *********************************************/
		if (time % 2 == 0)
			cout << map_car.size() << endl;

		/***************************************** �ָ��� *********************************************/

	}
	cout << time << endl;
	write_txt(answer_Path);//�������ļ�
	
	return 0;

}



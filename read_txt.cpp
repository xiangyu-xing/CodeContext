//#include "stdafx.h"
#include "read_txt.h"
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include "road.h"
#include "car.h"
#include "cross.h"

using  namespace std;
//version-4 ������ɣ�������ɣ��߼���ȷ�Բ������
// �޸Ķ�ȡ cross �� ��-1�� ���
// ����txt �ļ����пո��е����


map<const int, class road> map_road;
map<const int, class car> map_car;
map<const int, class  cross> map_cross;
map<int, int> map_use_time;
map<int, int> map_road_use_time;
map<int, map<const int, car>>car_wait;
int car_num;

vector<int>lastcar;
vector<int>have_d;


void read_txt(string file, map<const int,class road> & mp)
{
	ifstream infile;
	infile.open(file.data());
	//assert(infile.is_open());
	if (!infile.is_open())
	{
		cout << "open file: " << file << "has failed!" << endl;
		return;
	}

	string s;
	road temproad;
	while (getline(infile, s))
	{
		if (s[0] == '#')
			continue;
		if (s.size() == 0 || s[0] == ' ' )
			continue;
		vector<int> temp;
		int index = 0;
		int len = s.size();
		string num_str = "";
		while (index < len)
		{
			if (s[index] == '(')
			{
				++index;
				continue;
			}
			else if (s[index] >= '0' && s[index] <= '9')
				num_str += s[index];
			else if (s[index] == ',' || s[index] == ')')
			{
				temp.push_back(atoi(num_str.c_str()));
				num_str.clear();
			}
			++index;
		}
		temproad = road(temp);
		mp[temproad.id] = temproad;
	}

}
void read_txt(string file, map<const int, class car> & mp)
{
	ifstream infile;
	infile.open(file.data());
	//assert(infile.is_open());
	if (!infile.is_open())
	{
		cout << "open file: " << file << "has failed!" << endl;
		return;
	}

	string s;
	car tempcar;
	while (getline(infile, s))
	{
		if (s[0] == '#')
			continue;
		if (s.size() == 0 || s[0] == ' ' )
			continue;
		vector<int> temp;
		int index = 0;
		int len = s.size();
		string num_str = "";
		while (index < len)
		{

			if (s[index] == '(')
			{
				++index;
				continue;
			}
			else if (s[index] >= '0' && s[index] <= '9')
				num_str += s[index];
			else if (s[index] == ',' || s[index] == ')')
			{
				temp.push_back(atoi(num_str.c_str()));
				num_str.clear();
			}
			++index;
		}
		tempcar = car(temp);
		mp[tempcar.id] = tempcar;
	}

}

// int car_num = map_car.size();


void read_txt(string file, map<const int, class cross> & mp)
{
	ifstream infile;
	infile.open(file.data());
	//assert(infile.is_open());
	if (!infile.is_open())
	{
		cout << "open file: " << file << "has failed!" << endl;
		return;
	}

	string s;
	cross tempcross;
	while (getline(infile, s))
	{
		if (s[0] == '#')
			continue;
		if (s.size() == 0 || s[0] == ' ' )
			continue;
		vector<int> temp;
		int index = 0;
		int len = s.size();
		string num_str = "";
		int flag = 1;
		while (index < len)
		{

			if (s[index] == '(')
			{
				++index;
				continue;
			}
			else if(s[index] == '-')
				flag = -1;
			else if (s[index] >= '0' && s[index] <= '9')
				num_str += s[index];
			else if (s[index] == ',' || s[index] == ')')
			{
				temp.push_back(flag * atoi(num_str.c_str()));
				num_str.clear();
				flag = 1;
			}
			++index;
		}
		tempcross = cross(temp);
		mp[tempcross.getid()] = tempcross;
	}

}




#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include "road.h"
#include "car.h"
#include "cross.h"

//version-3 ������ɣ�������ɣ��߼���ȷ�Բ������
// �޸Ķ�ȡ cross �� ��-1�� ���

using namespace std;
void read_txt(string file, map<const int, class road> & mp);
void read_txt(string file, map<const int, class  car> & mp);
void read_txt(string file, map<const int, class cross> & mp);

extern map<const int,class road> map_road;
extern map<const int,class car> map_car;
extern map<const int,class cross> map_cross;
extern map<int, int> map_use_time;
extern map<int, int> map_road_use_time;
extern int car_num;
extern vector<int>lastcar;
extern map<int, map<const int, car>>car_wait;	  //���ճ���ʱ�������ֳ���
extern vector<int>have_d;



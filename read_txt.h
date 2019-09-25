#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include "road.h"
#include "car.h"
#include "cross.h"

//version-3 功能完成，编译完成，逻辑正确性测试完成
// 修改读取 cross 的 （-1） 情况

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
extern map<int, map<const int, car>>car_wait;	  //按照出发时间来划分车辆
extern vector<int>have_d;



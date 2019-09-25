#ifndef ROAD_H
#define ROAD_H
//version-7 功能完成，编译完成，逻辑正确性测试完成
//以下信息需要统一改写
#include <string>
#include <utility>		
#include <map>
#include "car.h"
#include "cross.h"
#include "read_txt.h"
using namespace std;

//传递路口信息，就能够获取采用的哪个矩阵了,这个road都需要这个信息

extern int crossIDProcessNow_glable;
extern int car_size_true;
void setCar_size_true();
void setCrossID(int crossIDProcessNow);
bool addNewCar(int carID);//添加新车，成功返回true，失败返回false
bool addNewCar(car & tempcar, road & temproad, vector<vector<int>> & matrixtemp, int facingcrossID);
void changeCarState_add(int carID, int roadID, int facingcrossID);//更新加车时的车
class road
{
private:
	void moveToTermination(vector<vector<int>> &matrixtemp);
	void moveToTermination(vector<vector<int>> &matrixtemp, int carID);
	//void countWaitCar(int type);
	int moveOut(int carID);
	void changeCarState_road(int carID);//更新路内移动车辆状态
	void changeCarState_cross(int carID, int roadIDB);//更新过路的车辆
	int findFirstCar(vector<vector<int>> & matrixtemp, vector<int> & sign);
	bool moveIn(int carID, int roadIDB, int lengthGone);	
	int	getMoveState(int carID, int roadIDB, int lengthGone);
	pair<vector<int>, vector<int>> findSign;
	void removeCar(int carID, vector<vector<int>> & matrixtemp);
	bool moveIn(car & tempcar, road & temproad, vector<vector<int>> & matrixtemp, int lenghtGone);
	int getLengthGone(int carID, vector<vector<int>> & matrixtemp);//计算矩阵中出车的已经走过的距离
public:
	//描述路面信息的数据
	int id;
	int roadLength;
	int speed;
	int channel;
	int fromCrossID;//==crossIDProcessNow 处理matrixFrom
	int toCrossID;
	bool isDuplex;//==true表示双向,==false表示单项道路，如果是双向没有入口和出口概念

	int use_times;	// 道路使用次数


	//出口为正方向
	vector<vector<int>> matrixFrom;//以fromcrossID为出口的矩阵，to为入口
	vector<vector<int>> matrixTo;//以tocrossID为出口的矩阵，from为入口
	//行为channel，列为length
	//描述代价用的数据,每个数据都是成对出现的，因为存在双向车道的问题
	//前面是fromcrossID为出口的数据，后面是tocrossID为出口的数据，如果
	pair<int, int> voidPlace;//可以进入车的数量==-1表示这个位置不存在
	pair<float, float> saturation;//饱和度
	pair<int, int>  timeBeforCar;//前面车走完需要的时间

	//内部计算的函数
	road();
	~road();
	road(vector<int> c);
	void countPlace();//计算剩余的可供进入的车的数量，路尾端空余的位置
	void countSaturation();//计算饱和度
	void countTimeBeforCar();//计算前面车辆走光需要的时间
	void initMatrix();
	//需要交互的函数
	//作为出口使用
	int findFirstCar();//返回一条路上优先级最高的车的id
	void removeCar(int carID);//移除到达终点的车
	//运行顺序为，movecarbefor->movecarA2B or removecar->movecarafter
	//void moveCarSecond();//第二次移动车辆，所有车都移动到不能移动为止
	void moveCarFirst();//第一次移动车辆，只移动能够移动最大距离的车辆
	bool moveA2B(int carID, int roadIDB);//从A路中把车挪到B路中
	void print_test_only(ofstream & txt);
	void print_test_only();
	int getlengthNextRoad(int carID, int roadIDB);
	void updateCost();

};

#endif

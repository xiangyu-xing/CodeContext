//version-0325-01:
/////////////log/////////////////
//0320:针对linux进行了优化
//0321:调整了movesecond
//0321:修正了movefirst修正
//修正移动车辆car—size—true，移动车辆就需要减一
//修正moveA2B，的移动逻辑，完成
//修正getlength，完成
//移除movesecond,放在这就行
//修正findfirstcar的逻辑错误,完成
//0323:对函数进行了重构
//修正了getlengthnextroad对下一条路的处理异常
//0324:调整了getmovestate的逻辑，现在这个逻辑可能会有问题
//0325：修正了判断逻辑的错误
	//出口逻辑调整为
		//crossIDProcessNow_glable == toCrossID, use matrixTo
		//crossIDProcessNow_glabel == fromCrossID && isDuplex == true, use matrixFrom
		//crossIDProcessNow_glabel == fromCrossID && isDuplex == false, error process
	//入口逻辑调整为
		//crossIDProcessNow_glable == fromCrossID, use matrixTo
		//crossIDProcessNow_glabel == toCrossID && isDuplex == true, use matrixFrom
		//crossIDProcessNow_glabel == toCrossID && isDuplex == false, error process
//#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include "road.h"
#include "car.h"
#include "cross.h"
#include "read_txt.h"
//////////////////////////////////////////////////////////////////////////
//需要更新处理的内容
//1：只要移动车辆，就需要处理代价的变更，updateCost
//2：移动后车的几个状态需要更新
//2.1：state，描述车的终止和等待状态,移动，addnewcar更新
//2.2：update_stateOfCar()，更新车的路口信息，road只负责调用,a2b时更新，addnewcar更新,removecar更新
//2.3：updateCost(),更新代价，移动，addnewcar更新
//2.4：cur_roadID, a2b时更新，addnewcar更新
//2.5：facing_cross，a2b时更新，addnewcar更新
//2.6：start_run()，addnewcar更新
//3:car size true,移动的操作都要进行更新
/*
过路口计算为一个距离统一处理在进入的函数中，出的函数不负责处理这种情况

matrixtemp的函数，只能内部调用，用于简化代码结构
getlenthgone  返回车移除这个道路需要消耗的移动能力lengthgone
    返回-1，表示出不去
    返回正数，表示车移动到路口第一个位置消耗的距离
getMoveState  返回车辆进入下一个路口的移动可能性
    返回0，表示不能进入下一个路口
    返回正数，表示能够进入下一个路口，并表示距离
    返回负数，表示进入下一个路口并存在等待情况，表示负的carid

具有矩阵使用逻辑的函数，执行逻辑处理功能
moveout 返回车车辆移除需要消耗的能力，具有cross逻辑
    返回-1，表示不能被移出该路
    返回正数，表示车移动到路口第一个位置消耗的距离
getlengthNextRoad 返回能否进入下一条的路的情况
    返回0，表示不能进入下一个路口
    返回正数，表示能够进入下一个路口，并表示距离
    返回负数，表示进入下一个路口并存在等待情况，表示负的carid
moveIn 在moveA2B判断出能够进入下一条路的时候才能调用moveIn
    返回true，表示加车成功
    返回false，表示加车失败
moveA2B  处理移动过程中的逻辑
*/
//////////////////////////////////////////////////////////////////////////
using namespace std;
//////////////////////////////////////////////////////////////////////////
//全局函数
//////////////////////////////////////////////////////////////////////////

int crossIDProcessNow_glable;
int car_size_true;
void setCar_size_true()//check
{
	car_size_true -= 1;
}
void setCrossID(int crossIDProcessNow)//check
{
	crossIDProcessNow_glable = crossIDProcessNow;
}
void changeCarState_add(int carID, int roadID, int facingcrossID)//check
{
	auto & tempcar = map_car.at(carID);
	tempcar.start_run();
	tempcar.state = false;
	tempcar.wait_car = 0;
	tempcar.cur_roadID = roadID;
	tempcar.facing_cross = facingcrossID;
	tempcar.update_stateOfCar();
}
//////////////////////////////////////////////////////////////////////////
//简化过程用的函数
//////////////////////////////////////////////////////////////////////////
void road::initMatrix()//check
{
	if (isDuplex)//==true 双向车道
	{
		vector<int> temp(roadLength, 0);
		for (int i = 0; i < channel; i++)
			matrixFrom.push_back(temp);
		matrixTo = matrixFrom;
	}
	else//==false 单项车道，出口为to，入口为from
	{
		vector<int> temp(roadLength, 0);
		for (int i = 0; i < channel; i++)
			matrixTo.push_back(temp);
	}
}
void road::print_test_only(ofstream & txt)//check
{
	txt << id << "; " << roadLength << "; " << speed << "; " << channel << "; " << fromCrossID << "; " << toCrossID << "; " << isDuplex << "; " << endl;

	if (isDuplex)
	{
		txt << "matrix from is:" << endl;
		for (int i = 0; i < channel; i++)
		{
			for (int j = 0; j < roadLength; j++)
			{
				txt << matrixFrom[i][j] << "; ";
			}
			txt << endl;
		}
		txt << "data about cost:" << endl;
		txt << "void place is:" << voidPlace.first << endl;
		txt << "saturation is:" << saturation.first << endl;

		txt << "matrix to is:" << endl;
		for (int i = 0; i < channel; i++)
		{
			for (int j = 0; j < roadLength; j++)
			{
				txt << matrixTo[i][j] << "; ";
			}
			txt << endl;
		}
		txt << "data about cost:" << endl;
		txt << "void place is:" << voidPlace.second << endl;
		txt << "saturation is" << saturation.second << endl;
	}
	else
	{
		txt << "matrix to is:" << endl;
		for (int i = 0; i < channel; i++)
		{
			for (int j = 0; j < roadLength; j++)
			{
				txt << matrixTo[i][j] << "; ";
			}
			txt << endl;
		}
		txt << "data about cost:" << endl;
		txt << "void place is:" << voidPlace.second << endl;
		txt << "saturation is" << saturation.second << endl;
	}
}
road::road(){}//check
road::~road(){}//check
road::road(vector<int> c)//check
{
	//初始化其他数据
	id = c[0];
	roadLength = c[1];
	speed = c[2];
	channel = c[3];
	fromCrossID = c[4];
	toCrossID = c[5];
	isDuplex = c[6];

	use_times = 0;

	//初始化matrix的过程
	initMatrix();
	//初始化需要计算的数据
	countPlace();
	countSaturation();
	countTimeBeforCar();
	if (isDuplex)
	{
		vector<int> temp(channel, 1);
		findSign.first = temp;
		findSign.second = temp;
	}
	else
	{
		vector<int> temp(channel, 1);
		findSign.second = temp;
	}
	
}
void road::countPlace()//check
{
	if (isDuplex)
	{
		voidPlace.first = 0;
		voidPlace.second = 0;
		for (int i = channel - 1; i >= 0; i--)
		{
			for (int j = roadLength - 1; j >= 0; j--)
			{
				if (matrixFrom[i][j] == 0)
				{
					voidPlace.first += 1;
				}
				else
				{
					break;
				}

			}
		}
		for (int i = channel - 1; i >= 0; i--)
		{
			for (int j = roadLength - 1; j >= 0; j--)
			{
				if (matrixTo[i][j] == 0)
				{
					voidPlace.second += 1;
				}
				else
				{
					break;//一旦遇到不等于0的地方，这个车道就不用查了
				}

			}
		}
	}
	else
	{
		voidPlace.first = -1;
		voidPlace.second = 0;
		for (int i = channel - 1; i >= 0; i--)
		{
			for (int j = roadLength - 1; j >= 0; j--)
			{
				if (matrixTo[i][j] == 0)
				{
					voidPlace.second += 1;
				}
				else
				{
					break;
				}

			}
		}
	}

}
void road::countSaturation()//check
{
	float voidfrom = 0, voidto = 0;
	int	allPlace = roadLength * channel;
	if (isDuplex)//==true 双向车道
	{
		for (int i = 0; i < channel; i++)
		{
			for (int j = 0; j < roadLength; j++)
			{
				if (matrixFrom[i][j] == 0)
					voidfrom += 1;
				if (matrixTo[i][j] == 0)
					voidto += 1;
			}
		}
		saturation.first = voidfrom / allPlace;
		saturation.second = voidto / allPlace;
	}
	else//==false 单项车道，出口为to，入口为from
	{
		for (int i = 0; i < channel; i++)
		{
			for (int j = 0; j < roadLength; j++)
			{
				if (matrixTo[i][j] == 0)
					voidto += 1;
			}
		}
		saturation.first = -1;
		saturation.second = voidto / allPlace;
	}
}
void road::countTimeBeforCar()//check
{

	//还不知道该怎么描述呢
	if (isDuplex)//==true 双向车道
	{

	}
	else//==false 单项车道，出口为to，入口为from
	{

	}
	timeBeforCar = { 0 ,0 };
}
void road::updateCost()//check
{
	countPlace();
	countSaturation();
	countTimeBeforCar();
}
void road::print_test_only()//check
{
	cout << id << "; " << roadLength << "; " << speed << "; " << channel << "; " << fromCrossID << "; " << toCrossID << "; " << isDuplex << "; " << endl;

	if (isDuplex)
	{
		cout << "matrix from is:" << endl;
		for (int i = 0; i < channel; i++)
		{
			for (int j = 0; j < roadLength; j++)
			{
				cout << matrixFrom[i][j] << "; ";
			}
			cout << endl;
		}
		cout << "data about cost:" << endl;
		cout << "void place is:" << voidPlace.first << endl;
		cout << "saturation is:" << saturation.first << endl;

		cout << "matrix to is:" << endl;
		for (int i = 0; i < channel; i++)
		{
			for (int j = 0; j < roadLength; j++)
			{
				cout << matrixTo[i][j] << "; ";
			}
			cout << endl;
		}
		cout << "data about cost:" << endl;
		cout << "void place is:" << voidPlace.second << endl;
		cout << "saturation is" << saturation.second << endl;
	}
	else
	{
		cout << "matrix to is:" << endl;
		for (int i = 0; i < channel; i++)
		{
			for (int j = 0; j < roadLength; j++)
			{
				cout << matrixTo[i][j] << "; ";
			}
			cout << endl;
		}
		cout << "data about cost:" << endl;
		cout << "void place is:" << voidPlace.second << endl;
		cout << "saturation is" << saturation.second << endl;
	}
}
void road::changeCarState_road(int carID)//check
{
	auto & tempcar = map_car.at(carID);
	tempcar.state = false;
	tempcar.wait_car = 0;
}
void road::changeCarState_cross(int carID, int roadIDB)//check
{
	auto & tempcar = map_car.at(carID);
	tempcar.state = false;
	tempcar.wait_car = 0;
	tempcar.cur_roadID = roadIDB;
	tempcar.facing_cross = crossIDProcessNow_glable == map_road.at(roadIDB).fromCrossID ? 
		map_road.at(roadIDB).toCrossID : map_road.at(roadIDB).fromCrossID;
	tempcar.update_stateOfCar();
}
//////////////////////////////////////////////////////////////////////////
//不需要处理矩阵逻辑的函数
//////////////////////////////////////////////////////////////////////////
int road::findFirstCar(vector<vector<int>> & matrixtemp, vector<int> & sign)
{
	int sum = 0;
	for (auto tt : sign)
		sum += tt;
	if (sum == 0)//每个通道的出车状态都是假，就所有车道都不能出车
		return -1;

	auto maxsite = speed < roadLength ? speed : roadLength;
	for (int j = 0; j < maxsite; j++)
	{
		for (int i = 0; i < channel; i++)
		//这个位置控制如果以道路限速为速度的正好能走到第一个位置
		{
			if (sign[i] == 1)//说明这个channel还能出车
			{
				auto caridtemp = matrixtemp[i][j];
				if (caridtemp == 0)
				{
					continue;
				}
				else
				{
 					auto maxlength = map_car.at(caridtemp).max_vel < speed ?
						map_car.at(caridtemp).max_vel : speed;
					if (j >= speed)//符合这个条件的一定出不去
					{
						sign[i] = 0;
						continue;
					}
					else//速度大于j只要，前面没有车就能出去,第一个车出不去后面就都出不去
					{
						if (j < maxlength && map_car.at(caridtemp).state == true)//changed from > to <
						{
							return caridtemp;
						}
						else
						{
							sign[i] = 0;
							continue;
						}
					}
				}
			}
			else//说明这个channel不能出车,计算下一个channel
			{
				continue;
			}
			
		}
		sum = 0;//遍历了所有的车道之后，判断channel是不都是不能出的状态，如果是，不用在遍历车位了
		for (auto tt : sign)
			sum += tt;
		if (sum == 0)//每个通道的出车状态都是假，就所有车道都不能出车
			return -1;
	}
	return -1;//说明没有车道能够出车，返回-1
}
int road::getLengthGone(int carID, vector<vector<int>> & matrixtemp)//check
{
	//返回一个车carid移出该路需要消耗的移动能力lengthgone
	auto &tempcar = map_car.at(carID);
	int maxlength = tempcar.max_vel < speed ? tempcar.max_vel : speed;
	int maxsite = speed < roadLength ? speed : roadLength;
	maxsite = maxsite < maxlength ? maxsite : maxlength;
	//超过这个位置的车一定出不去，所以直接设置最大距离，减少循环次数
	for (int i = 0; i < channel; i++)
	{
		for (int j = 0; j < maxsite; j++)
		{
			if (matrixtemp[i][j] == carID)
				return j;
		}
	}
	return -1;//如果没有在最大搜索距离内没有找到，说明出不去 
}
void road::removeCar(int carID, vector<vector<int>> & matrixtemp)//check
{
	for(int i = 0; i < channel; i++)
	{
		for (int j = 0; j < roadLength; j++)
		{
			if (matrixtemp[i][j] == carID)
				matrixtemp[i][j] = 0;
			else
				continue;	
		}
	}
}
void road::moveToTermination(vector<vector<int>> &matrixtemp, int carID)//check
{
	//carid必须是该车道第一优先级的车才可以
	for (int i = 0; i < channel; i ++)
	{
		for (int j = 0; j < roadLength; j++)
		{
			if (matrixtemp[i][j] == carID)
			{
				if (j == 0)
				{
					changeCarState_road(carID);
					setCar_size_true();
					return;
				}
				else
				{
					matrixtemp[i][0] = carID;
					matrixtemp[i][j] = 0;
					changeCarState_road(carID);
					setCar_size_true(); 
					return;
				}
			}
			else
				continue;
		}
	}
}
void road::moveToTermination(vector<vector<int>> & matrixtemp)//check
{
	//使car产生两类终止状态移动
		//1，能移动到最大距离
		//2，前面有终止车辆,后面会因为这个终止状态而前进到终止状态
	int carID;
	int maxlength;
	for (int i = 0; i < channel; i++)
	{
		for (int j = 0; j < roadLength; j++)
		{
			if  (matrixtemp[i][j] == 0)
				continue;
			else
			{
				carID = matrixtemp[i][j];
				if (map_car.at(carID).state == false)//车已经移动过，则不能再移动了
					continue;//遍历下一个元素
				else//如果车没移动过，则看是否能够移动
				{
					maxlength = map_car.at(carID).max_vel < speed ? map_car.at(carID).max_vel : speed;//计算最大移动距离
					if (j == 0)//如果是第一个位置，直接处于等待状态
						map_car.at(carID).wait_car = -1;//不修改state状态
						//车不动
					else//如果，不是第一个位置，就要判断能不能继续移动
					{
						for (int k = j -  1; k >= 0; k--)//访问这个车前面的路
						{
							if  (matrixtemp[i][k] == 0)//前面是0的时候
							{
								if (k == j - maxlength)
								{
									matrixtemp[i][k] = carID;//移动到最大距离,移动情况1
								    matrixtemp[i][j] = 0;
									changeCarState_road(carID);
									setCar_size_true();
									break;
								}
								else
								{
									if (k == 0)
									{
										map_car.at(carID).wait_car = -1;//这个车是这个路口优先级最高的车
										break;
									}
									else
									{
										continue;
									}
								}									
							}
							else//前面有车阻挡
							{
								int carIDTemp = matrixtemp[i][k];
								if (map_car.at(carIDTemp).state == false)//车是终止状态，移动情况2
								{
									if (k == j -1)//紧跟着的就是终止车辆，不移动只改变状态
									{
										changeCarState_road(carID);
										setCar_size_true();
										break;
									}
									else
									{
										matrixtemp[i][k+1] = carID;
										matrixtemp[i][j] = 0;
										changeCarState_road(carID);
										setCar_size_true();
										break;
									}							   
								}
								else//如果前车是等待状态，那么该车也是等待状态
								{
									map_car.at(carID).wait_car = carIDTemp;//等待前面的车
									break;
								}
								
							}							
						}
					}					
				}
			}
		}
	}
}
bool addNewCar(car & tempcar, road & temproad, vector<vector<int>> & matrixtemp, int facingcrossID)//check
{
	int maxlength = 0;
	int caridtemp;
	for (int i = 0; i < temproad.channel; i++)
	{
		for (int j = temproad.roadLength - 1; j >= 0; j--)
		{
			maxlength = temproad.speed < tempcar.max_vel ? 
				temproad.speed : tempcar.max_vel;
			maxlength -= 1;//TODO:看看这个路口应不应该处理一个1
			caridtemp = matrixtemp[i][j];
			if (caridtemp == 0)
			{
				if (j == temproad.roadLength - 1 - maxlength)
				{
					matrixtemp[i][j] = tempcar.id;
					changeCarState_add(tempcar.id, temproad.id, facingcrossID);
					return true;
				}
				else
					continue;
			}
			else
			{
				if (temproad.roadLength - 1 == j)
					break;
				else
				{
					matrixtemp[i][j + 1] = tempcar.id;
					changeCarState_add(tempcar.id, temproad.id, facingcrossID);
					return true;
				}
			}
			
		}
	}
	return false;
}
bool road::moveIn(car & tempcar, road & temproad, vector<vector<int>> & matrixtemp, int lenghtGone)//check
{
	int maxlength = tempcar.max_vel < temproad.speed ? tempcar.max_vel : temproad.speed;
	maxlength = maxlength - lenghtGone - 1;//处理过路口消耗的1个距离
	for (int i = 0; i < temproad.channel; i++)
	{
		for (int j = temproad.roadLength - 1; j >= 0; j--)
		{
			int caridtemp = matrixtemp[i][j];
			if (caridtemp == 0)
			{
				if (j == temproad.roadLength - 1 - maxlength)
				{
					matrixtemp[i][j] = tempcar.id;
					return true;
				}
				else
					continue;
			}
			else
			{
				if (j == temproad.roadLength - 1)
					break;
				else
				{
					matrixtemp[i][j + 1] = tempcar.id;
					return true;
				}
				
			}
			
		}
	}
	return false;
}
/////////////////////////////////////////////////////////////////////
//处理矩阵使用逻辑的函数
/////////////////////////////////////////////////////////////////////
int road::moveOut(int carID)//check
{
	//返回车移动需要消耗的移动能力，具有cross判断逻辑
		//返回-2， 表示单向路时访问matrixfrom矩阵
		//返回-1，说明车出不去，但是访问了正确的矩阵
		//返回正数，表示访问成功，表示消耗的移动能力
	int lengthGone = 0;
	if (isDuplex && (crossIDProcessNow_glable == fromCrossID))//双向且fromID为出口方向时
		lengthGone = getLengthGone(carID, matrixFrom);
	else if (crossIDProcessNow_glable == toCrossID)
		lengthGone = getLengthGone(carID, matrixTo);
	else
		return -1;//如果访问不存在的这个位置，就返回-1，表示没有这个车
	return lengthGone;
}
int road::getMoveState(int carID, int roadIDB, int lengthGone)//TODO: 这个位置的逻辑可能又问题
{
	auto &tempcar = map_car.at(carID);
	auto &temproad = map_road.at(roadIDB);
	int maxlength = temproad.speed < tempcar.max_vel ? temproad.speed : tempcar.max_vel;
	maxlength = maxlength - 1 - lengthGone;//过路口计算为一个距离
	//这个maxlength=1表示正好能够进入下一个路的最后一个位置
	if (maxlength < 0 || maxlength <= lengthGone )//剩余移动能力不足，返回不能进入,至少要等于0，到新路的最后一个位置
		return 0; 
	//需要自己处理使用的是那个矩阵
	vector<vector<int>> matrixtemp;//只使用查询功能，不用引用
	int voidplace_temp;
	if (temproad.fromCrossID == crossIDProcessNow_glable)
	{
		matrixtemp = temproad.matrixTo;
		voidplace_temp = temproad.voidPlace.second;
	}
	else if (temproad.toCrossID == crossIDProcessNow_glable && temproad.isDuplex == true)
	{
		matrixtemp = temproad.matrixFrom;
		voidplace_temp = temproad.voidPlace.first;
	}
	else
	{
		return 0;
	}

	if (voidplace_temp == 0)
	{
		for (int i  = 0; i < temproad.channel; i++)
		{
			if (map_car.at(matrixtemp[i][temproad.roadLength - 1]).state == true)
				return -matrixtemp[i][temproad.roadLength - 1];
			else
				continue;
		}
		return 0;
	}	
	int caridtemp = 0;
	
	for (int i = 0; i < temproad.channel; i++)
	{
		for (int j = temproad.roadLength - 1; j >=0; j--)
		{
			caridtemp = matrixtemp[i][j];
			if (caridtemp == 0)
			{
				if (j == temproad.roadLength - 1 - maxlength)//到最大距离了
					return maxlength + 1;//把过路口消耗的距离处理回来
				else
					continue;			
			}
			else//前面有车阻挡
			{
				if (j == temproad.roadLength - 1)//如果路口第一个就是等待
				{
					if (map_car.at(caridtemp).state == true)
						return -caridtemp;
					else
						break;//只有第一车道的末尾车是终止状态，才能判断下一个车道
				}
				else
				{
					if (map_car.at(caridtemp).state == false)
						return temproad.roadLength - 1 - j;//返回能够行进的距离
					else
						return -caridtemp;//如果前车是等待状态，直接返回该车编号
					
				}
				
			}
		}
	}
	return 0;//每个车道都进不去，表示失败了
}
bool road::moveIn(int carID, int roadIDB, int lengthGone)//check
{
	auto &tempcar = map_car.at(carID);
	auto &temproad = map_road.at(roadIDB);
	bool result = false;
	if (temproad.fromCrossID == crossIDProcessNow_glable)
		result = moveIn(tempcar, temproad, temproad.matrixTo, lengthGone);
	else
		result = moveIn(tempcar, temproad, temproad.matrixFrom, lengthGone);
	return result;
}
void road::removeCar(int carID)//check
{
	if (crossIDProcessNow_glable == fromCrossID && isDuplex == true)//双向且fromID为出口方向时
	{
		removeCar(carID, matrixFrom);
		moveToTermination(matrixFrom);//判断路的两类终止移动
	}
	else if (crossIDProcessNow_glable == toCrossID)
	{
		removeCar(carID, matrixTo);
		moveToTermination(matrixTo);//判断路的两类终止移动
	}
	else//应该不会进入这里
	{
		return;
	}
	//map_car.at(carID).update_stateOfCar();
	setCar_size_true();
	updateCost();
	return;
}
void road::moveCarFirst()//check
{
	//移动的有两种情况
		//1，能移动到最大距离
		//2，前面有终止车辆,后面会因为这个终止状态而前进到终止状态
	//如果处于等待状态，判断等的是哪个车
	if (isDuplex)
	{
		for(auto &t:findSign.first)
			t = 1;
		for(auto &t:findSign.second)
			t = 1;
	}
	else
	{
		for(auto &t:findSign.second)
			t = 1;
	}

	if (isDuplex)
	{
		moveToTermination(matrixFrom);//先移动
		moveToTermination(matrixTo);
	}
	else
	{
		moveToTermination(matrixTo);//先移动
	}
	updateCost();
}
bool road::moveA2B(int carID, int roadIDB)//check
{
	//从A路中把车挪到B路中,这个方法应该有roadIDA对象调用，我要访问roadIDB元素
	//转moveToTermination处理的移动，更新工作有其完成
	if (map_car.at(carID).state == false)//表示这个车移动过了
		return false;
	int lengthGone = 0;
	bool result;
	//判断移出，并检查合法性
	lengthGone = moveOut(carID);
	if (lengthGone == -1 || getlengthNextRoad(carID, roadIDB) == 0)//不能进入下一条道路，移动失败，进行一次强制移动
	{
		result = false;
	}
	else if (lengthGone == -2)//访问非法数据，访问了单向道的matrixfrom矩阵
	{
		return false;//检查移出部分的数据合法性，后面的负责移入部分的合法性
	}
	else
	{
		//判断移入，并检查合法性
		if (crossIDProcessNow_glable == map_road.at(roadIDB).toCrossID && map_road.at(roadIDB).isDuplex == false)
			return false;//访问单向路的matrixfrom矩阵，为非法操作
		else
			result = moveIn(carID, roadIDB, lengthGone);//数据合法性的判断有前面完成，子函数不附带次功能
	}
	//如果访问的都是合法单元
	if (result == true)//如果过路口成功
	{
		if (isDuplex == true && crossIDProcessNow_glable == fromCrossID)
		{
			removeCar(carID, matrixFrom);//删除原来的车
			moveToTermination(matrixFrom);//判断路的两类终止移动
		}
		else if (crossIDProcessNow_glable == toCrossID)
		{
			removeCar(carID, matrixTo);//删除原来的车
			moveToTermination(matrixTo);//判断路的两类终止移动
		}
		else
		{
			return false;//访问非法数据单元
		}
		setCar_size_true();//减少这个计数器
		changeCarState_cross(carID, roadIDB);//更新车的状态
		updateCost();//更新原来的路的损失
		map_road.at(roadIDB).updateCost();//更新进入的路损失
		return true;
	}
	else
	{
		if (isDuplex == true && crossIDProcessNow_glable == fromCrossID)
		{
			moveToTermination(matrixFrom, carID);//产生一次强制移动某个车
			moveToTermination(matrixFrom);
		}
		else if (crossIDProcessNow_glable == toCrossID)
		{
			moveToTermination(matrixTo, carID);
			moveToTermination(matrixTo);
		}
		else
		{
			return false;
		}
		updateCost();
		return false;
	}
	
}
bool addNewCar(int carID)//check
{
	car &tempcar = map_car.at(carID);
	road &temproad = map_road.at(tempcar.next_roadID);
	bool result;

	if (tempcar.start == temproad.fromCrossID)//
	{
		result = addNewCar(tempcar, temproad, temproad.matrixTo, temproad.toCrossID);
	}
	else if (tempcar.start == temproad.toCrossID && temproad.isDuplex == true)
	{
		result = addNewCar(tempcar, temproad, temproad.matrixFrom, temproad.fromCrossID);
	}
	else//处理异常
	{
		return false;
	}

	if (result)
	{
		temproad.updateCost();
		setCar_size_true();
		return true;
	}
	else
		return false;
}
int road::findFirstCar()//check
{
	if (isDuplex && (crossIDProcessNow_glable == fromCrossID))
		return findFirstCar(matrixFrom, findSign.first);
	else if (crossIDProcessNow_glable == toCrossID)
		return findFirstCar(matrixTo, findSign.second);
	else
		return -1;//访问非法数据，返回没有车为第一优先级
}
int road::getlengthNextRoad(int carID, int roadIDB)//check
{
	//返回值说明
		//返回0，表示前方没有可以移动的距离了，或者有非法操作
		//返回正值，下条路能走的距离
		//返回负值，负值车号，表示roadidb的第一优先级的车
	if (crossIDProcessNow_glable == map_road.at(roadIDB).toCrossID && map_road.at(roadIDB).isDuplex == false)
		return 0;//访问非法数据单元，始终返回0，先检验完数据的合法性了，后面应该不会出现非法字段了

	int lengthGone = moveOut(carID);//有可能返回-1，-2
	if (lengthGone == -2)//访问非法数据
		return 0;
	if (lengthGone == -1)//车挪不出来
		return 0;

	int result, result2;
	int maxlengthnextroad = map_car.at(carID).max_vel < map_road.at(roadIDB).speed ? map_car.at(carID).max_vel : map_road.at(roadIDB).speed;
	if (maxlengthnextroad > lengthGone)//下一条路的限速小于等于当前道路的已经走过的路程，不能进入下一条路
	{
		result = getMoveState(carID, roadIDB, lengthGone);//有可能进入下一个路
		if (result == 0)//可以直接返回，不用处理
			return 0;
		else if (result > 0)
		{
			return result;
		}
		else
		{
			//todo: need to test this part is still right
			//以下为新增内容
			int crossidtemp = crossIDProcessNow_glable;
			if (map_road.at(roadIDB).fromCrossID == crossIDProcessNow_glable)
			{
				setCrossID(map_road.at(roadIDB).toCrossID);
			}
			else
			{
				setCrossID(map_road.at(roadIDB).fromCrossID);
			}
			//以上为新增内容
			result2 = map_road.at(roadIDB).findFirstCar();
			setCrossID(crossidtemp);//新增内容
			if (result2 == -1 || result2 == -2)//返回-1表示没有车可以出路口
				return 0;
			else
				return -result2;//此时返回的车号的负数

		}

	}
	else//这种就是一定不能进入下一条路的情况
		return 0;
}
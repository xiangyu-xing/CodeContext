#include "iostream"
#include <string>
#include <vector>
#include <map>
#include "cross.h"
#include "answer.h"
#include "road.h"
#include "car.h"
#include "read_txt.h"
#include "short_path.h"




int main(int argc, char *argv[])
{
    std::cout << "Begin" << std::endl;
	
	if(argc < 5){
		std::cout << "please input args: carPath, roadPath, crossPath, answerPath" << std::endl;
		exit(1);
	}
	
	std::string carPath(argv[1]);
	std::string roadPath(argv[2]);
	std::string crossPath(argv[3]);
	std::string answerPath(argv[4]);
	
	std::cout << "carPath is " << carPath << std::endl;
	std::cout << "roadPath is " << roadPath << std::endl;
	std::cout << "crossPath is " << crossPath << std::endl;
	std::cout << "answerPath is " << answerPath << std::endl;
	
	// TODO:read input filebuf
	// TODO:process
	// TODO:write output file
	
	
	/****************************************** 分割线 ******************************************/
	
	read_txt(carPath, map_car);
	read_txt(roadPath, map_road);
	read_txt(crossPath, map_cross);
	car_num = map_car.size();
	map<int, map<int, car>>car_wait;	  //按照出发时间来划分车辆

	for (auto &c : map_car)
	{
		int time = c.second.go_time;
		int Id = c.first;
		car_wait[time].insert({ Id,c.second });
	}	 //将汽车按出发时间分类 ，将用在汽车上路


	shortpath p;  //short_path的对象
	for (auto &c : map_car)
	{
		p.Dijikstra(c.second);
	}
	carInRoad = 0;
	int time = 0;//时间片
	while (map_car.size() != 0)	//还有车没有到达终点,每到达一个终点，run_cross会删掉达到终点的车辆
	{
		// 道路上的车辆初始化

		for (auto &c : map_car)
		{
			c.second.state = true;  //重置每辆车的状态为可以移动
		}
		car_size_true = carInRoad; //初始化在路上可移动车辆的数目
		for (auto &c : map_road)   //使每条道路上的车辆都尽可能的走
		{
			c.second.moveCarFirst();
		}
		while (car_size_true != 0)//保证所有车在当前时间片都行驶了一次
		{
			//if (map_car.at(10382).is_wait == false)
			//	ceshi(10382);
			for (auto &c : map_cross) //遍历所有路口，让每条道路都尽可能的行驶车辆
			{
				setCrossID(c.first);
				while (c.second.run_cross(p));	 //行驶一个路口所关联的道路上的车辆，没有一辆车行走，都会更新相应道路上的车辆状态
			}
		}

		++time;
		auto waitBeg = car_wait.begin();
		while (waitBeg != car_wait.end())
		{
			if (waitBeg->first > time)break;//负责从时间层面上来终止循环
			auto beg = waitBeg->second.begin();
			while (beg != waitBeg->second.end())  //负责从车辆数量方面来终止循环
			{
				p.Dijikstra(beg->second); //为即将上路的车辆规划路径
				int voidplace_of_add = 0;
				float cost_of_add = 1;
				auto &road_next = map_road.at(beg->second.next_roadID);	//得到预计进入道路的引用
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
					if (addNewCar(beg->first) == true)	  //加车成功后需要把car_wait删除对应的车辆
					{
						beg->second.real_go_time = time;
						carInRoad++;
						waitBeg->second.erase(beg++->first);
						continue;
					}
				}  //如果不能加车
				beg++;
			}
			waitBeg++;
		}

	}
	write_txt(answerPath);	//输出结果文件

	return 0;
}

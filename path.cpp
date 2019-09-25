#include "path.h"
vector<vector<int>>use_of_Graph;
vector <vector<double >> Graph;
void update_carPath(int carId, bool deadlock)
{
	vector<vector<double>>mapofcar = Graph;	//��ǰ�������õ�ͼ
	int startId = map_car.at(carId).facing_cross;//��ʼ��
	int endId = map_car.at(carId).target;//Ŀ���
	int min = maxweigts;
	int k = 0;//��¼��̾���ڵ���
	vector<bool>flag(mapofcar.size(),false);//ÿ�����Ƿ��ֵ����·��
	vector<int>dist(mapofcar.size()); //��ʼ�ڵ�ǰ�������ڵ��·��
	vector<int>prev(mapofcar.size(), 0);//ÿ���ڵ��ǰ�ýڵ�
	if (deadlock == true)	 //�����������Ҫ��·
	{
		int roadId = map_car.at(carId).next_roadID;
		int toId = 0;
		if (startId == map_road.at(roadId).fromCrossID)toId = map_road.at(roadId).toCrossID;
		else toId = map_road.at(roadId).fromCrossID;
		mapofcar[startId][toId] = maxweigts;
		int cur_road = map_car.at(carId).cur_roadID;
		int next_id = 0;
		if (map_road.at(cur_road).fromCrossID == startId) next_id = map_road.at(cur_road).toCrossID;
		else next_id = map_road.at(cur_road).fromCrossID;
		mapofcar[startId][next_id] = maxweigts;//��ֹ������ͷ��ʹ
		if (!map_car.at(carId).cross_path.empty())//������ǿյģ���Ҫ��ɾ��·��ǰ������·ʹ�ô�����ȥ
		{
			vector<int>&tmp = map_car.at(carId).cross_path;
			for (int i = 0; i < tmp.size() - 1; i++)
			{
				reduce_Graph(tmp[i], tmp[i + 1]);
			}
		}
	}
	for (int i = 0; i < mapofcar.size(); i++)	//��ʼ������ǰ������ľ���
	{
		dist[i] = mapofcar[startId][i];
	}
	dist[startId] = 0;
	flag[startId] = true;
	while (flag[endId] == false)	//�����ҵ�ȫ�������·����ֻҪ�ҵ�����Ŀ�������·��
	{
		min = maxweigts;
		for (int i = 0; i < mapofcar.size(); i++) //�ҵ���̾���
		{
			if (dist[i] < min&&flag[i] == false)
			{
				min = dist[i];
				k = i;
			}
		}
		flag[k] = true;	//ǰ��һ���ڵ�����·���Ѿ��ҵ�
		for (int i = 0; i < mapofcar.size(); i++)	//��ʼ�������·��
		{
			int tmp = (mapofcar[k][i] == maxweigts ? maxweigts : (min + mapofcar[k][i]));
			if (flag[i] == false && tmp < dist[i])
			{
				dist[i] = tmp;
				prev[i] = k;
			}
		}
	}
	/*�Ѿ��ҵ���ǰ��Ŀ�������·��*/
	map_car.at(carId).cross_path.clear();//��ն���
	map_car.at(carId).road_path.clear();
	vector<int>tmp;
	while (endId != 0)
	{
		tmp.insert(tmp.begin(), endId);
		endId = prev[endId];
	}
	tmp.insert(tmp.begin(),startId);
	map_car.at(carId).cross_path = tmp;	//������·�ڶ����Ѿ����
	for (int i = 0; i < tmp.size()-1; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			int roadId = map_cross.at(tmp[i]).cross_road[j];
			if (roadId != -1)
			{
				if (map_road.at(roadId).fromCrossID == tmp[i] && map_road.at(roadId).toCrossID == tmp[i + 1])
				{
					map_car.at(carId).road_path.push_back(roadId);	 
					add_Graph(tmp[i], tmp[i + 1]);
					break;
				}
				else if (map_road.at(roadId).toCrossID == tmp[i] && map_road.at(roadId).fromCrossID==tmp[i+1])
				{
					map_car.at(carId).road_path.push_back(roadId);	
					add_Graph(tmp[i], tmp[i + 1]);
					break;
				}
			}
		}
	}
	map_car.at(carId).road_path.push_back(-1);
	map_car.at(carId).next_roadID = map_car.at(carId).road_path.front();
	/*����˶Գ���·���Ĺ滮*/
}
void update_Graph()
{
	int crossNUmber = map_cross.rbegin()->first; //�õ�ͼ�Ĵ�С
	Graph.resize(crossNUmber+1);
	use_of_Graph.resize(crossNUmber + 1);
	for (int i = 0; i < crossNUmber + 1; i++)
	{
		Graph[i].resize(crossNUmber+1);
		use_of_Graph[i].resize(crossNUmber + 1);
		for (int j = 0; j < crossNUmber + 1; j++)
		{
			Graph[i][j] = weigts(i, j);
		}
	}
	
}
double weigts(int fromId, int toId)
{
	if (map_cross.find(fromId) == map_cross.end() || map_cross.find(toId) == map_cross.end())
		return maxweigts;
	for (int i = 0; i < 4; i++)
	{
		int roadId = map_cross.at(fromId).cross_road[i];
		if(roadId==-1)continue;
		else if (map_road.at(roadId).fromCrossID == fromId&&map_road.at(roadId).toCrossID == toId)
		{
			return (double)map_road.at(roadId).roadLength / (double)map_road.at(roadId).speed;
		}
		else if (map_road.at(roadId).fromCrossID == toId&&map_road.at(roadId).toCrossID == fromId)
		{
			if(map_road.at(roadId).isDuplex)return (double)map_road.at(roadId).roadLength /(double) map_road.at(roadId).speed;
			else return maxweigts;
		}
	}
	return maxweigts;
}
void add_Graph(int fromId, int toId)
{
	int pay = 0;
	
	use_of_Graph[fromId][toId]++;
	Graph[fromId][toId] = weigts(fromId, toId);//�ָ�Ȩֵ
	int roadId = 0;
	for (int i = 0; i < 4; i++)
	{
		roadId = map_cross.at(fromId).cross_road[i];
		if(roadId==-1)continue;
		else if (map_road.at(roadId).fromCrossID == fromId&&map_road.at(roadId).toCrossID == toId)
			break;
		else if (map_road.at(roadId).toCrossID == fromId&&map_road.at(roadId).fromCrossID == toId)
			 break;
		roadId = -1;
	}
	double maxOfRoad = k0*(map_road.at(roadId).roadLength*map_road.at(roadId).channel + map_road.at(roadId).speed);
	if (use_of_Graph[fromId][toId] <= maxOfRoad)
	{
		pay = 0;
	}
	else
		pay = Graph[fromId][toId] * ((double)use_of_Graph[fromId][toId] / maxOfRoad - 1);
	Graph[fromId][toId] += pay;
}
void reduce_Graph(int fromId, int toId)
{
	int pay = 0;

	use_of_Graph[fromId][toId]--;
	Graph[fromId][toId] = weigts(fromId, toId);//�ָ�Ȩֵ
	int roadId = 0;
	for (int i = 0; i < 4; i++)
	{
		roadId = map_cross.at(fromId).cross_road[i];
		if (roadId == -1)continue;
		else if (map_road.at(roadId).fromCrossID == fromId&&map_road.at(roadId).toCrossID == toId)
			break;
		else if (map_road.at(roadId).toCrossID == fromId&&map_road.at(roadId).fromCrossID == toId)
			break;
		roadId = -1;
	}
	double maxOfRoad = k0*(map_road.at(roadId).roadLength*map_road.at(roadId).channel + map_road.at(roadId).speed);
	if (use_of_Graph[fromId][toId] <= maxOfRoad)
	{
		pay = 0;
	}
	else
		pay = Graph[fromId][toId] * ((double)use_of_Graph[fromId][toId] / maxOfRoad - 1);
	Graph[fromId][toId] += pay;
}
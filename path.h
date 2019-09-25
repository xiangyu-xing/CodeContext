#ifndef PATH_H
#define PATH_H
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <cmath>
#include "read_txt.h"
using namespace std;
#define maxweigts 9999999
#define k0 1
extern vector <vector<double >> Graph; //���ݵ�·��·�����ݽ����ĵ�ͼ
extern vector<vector<int>>use_of_Graph;//ÿ����·��ǰ��Ԥ��ʹ�ô���
void update_Graph(); //	 ����ͼ��ֻ����������ȡ���ݺ�ʹ��һ��
double weigts(int fromId, int toId);//�Ӷ���fromǰ������to��Ȩ��
void add_Graph(int fromId, int toId);//����ͼ��Ȩֵ
void reduce_Graph(int fromId, int toId);//����ͼ��Ȩֵ
void update_carPath(int carId,bool deadlock=false);

#endif



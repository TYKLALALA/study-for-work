#ifndef __HAIKANG__
#define __HAIKANG__
#include <stdlib.h>
#include <stdio.h>
#include "OSSocket.h"
#include "JsonParse.h"
#include "CmdParse.h"
#include"astar.h"
#include"blockallocator.h"
#include<map>
#include<haikang.h>
#include<vector>
using namespace std;

struct Point
{
	int x;
	int y;
	Point():x(0),y(0)
	{}
	Point(int a, int b) :x(a), y(b){}
	~Point()
	{}

};
struct Point3d
{
	int x;
	int y;
	int z;
	Point3d() :x(0), y(0), z(0)
	{}
	Point3d(int a, int b,int c) :x(a), y(b),z(c){}
	~Point3d()
	{}
};
struct uav2good
{
	int uavno;
	int goodno;
	uav2good() :uavno(0), goodno(0){}
	uav2good(int a, int b) :uavno(a), goodno(b){}
	~uav2good(){}

};
int **CrearMap(MAP_INFO *pstMap, int high);
void freemap(int **map, int X, int Y);
void AlgorithmCalculationFun(MAP_INFO *pstMap, MATCH_STATUS * pstMatch, FLAY_PLANE *pstFlayPlane, vector<uav2good>Uav2Good, map<int, Point3d>&initialpos,int &goodno);



//map<int, Point3d>initialpos;//存储初始化的位置
#endif
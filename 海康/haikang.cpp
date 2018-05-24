//产生高度为high时的二维平面图
#include <stdlib.h>
#include <stdio.h>
#include "OSSocket.h"
#include "JsonParse.h"
#include "CmdParse.h"
#include"astar.h"
#include"blockallocator.h"
#include"haikang.h"
#include<vector>
#include<time.h>
#include<map>
#include<unordered_map>  
using namespace std;
int **CrearMap(MAP_INFO *pstMap, int high)
{
	int X = pstMap->nMapX;
	int Y = pstMap->nMapY;
	int buildx; int buildy;
	int buildl, buildw, buildh;
	int buildnum = pstMap->nBuildingNum;
	//申请空间并且全部置零
	int **map = (int**)malloc(sizeof(int*)*Y);
	for (int i = 0; i < Y; i++)
	{
		map[i] = (int*)malloc(sizeof(int)*X);
		for (int j = 0; j < X; j++)
			map[i][j] = 0;
	}
	for (int i = 0; i <buildnum; i++)
	{
		if ((pstMap->astBuilding)[i].nH < high)
			continue;
		buildx = (pstMap->astBuilding)[i].nX;
		buildy = (pstMap->astBuilding)[i].nY;
		buildl = (pstMap->astBuilding)[i].nL;
		buildw = (pstMap->astBuilding)[i].nW;
		buildh = (pstMap->astBuilding)[i].nH;
		for (int j = 0; j < buildw; j++)
			for (int k = 0; k < buildl; k++)
				map[buildy + j][buildx + k] = 1;
	}
	return map;
}
void freemap(int **map, int X, int Y)
{
	for (int i = 0; i < Y; i++)
		free(map[i]);
	free(map);
}
//二维平面规划路径
bool findpath(int **map, MAP_INFO *pstMap,vector<Point>&path,Point start,Point end)
{
	AStar::Params param;
	param.width = pstMap->nMapX;
	param.height = pstMap->nMapY;
	param.corner = true;
	param.start = AStar::Vec2(start.x, start.y);
	param.end = AStar::Vec2(end.x, end.y);
	param.can_pass = [&](const AStar::Vec2 &pos)->bool
	{
		return map[pos.y][pos.x] == 0;
	};

	// 执行搜索
	BlockAllocator allocator;
	AStar algorithm(&allocator);
	auto path1 = algorithm.find(param);
	if (path1.size() == 0)
		return false;
	else 
	{
		for (int i = 0; i < path1.size(); i++)
			path.push_back(Point(path1[i].x, path1[i].y));
		return true;
	}
}

//产生初始化无人机位置的坐标
void initialposfunc(MAP_INFO *pstMap,map<int,Point3d>&initialpos)
{
	int posX, posY,posZ;
	int uavnum = pstMap->nUavNum;
	int divide = ceil((double)uavnum / (pstMap->nHHigh - pstMap->nHLow));
	srand((unsigned)time(NULL));
	for (int i = 0; i < uavnum; i++)
	{
		posZ = pstMap->nHLow + i / divide;
		int **map = CrearMap(pstMap, posZ);
		while (1)
		{
			int a = (int)pstMap->nMapX / 4;
			int b = (int)pstMap->nMapY / 4;
			posX = rand() % (2*a)+a;
			posY = rand() % (2*b)+b;
			if (!map[posY][posX])
			{
				freemap(map, pstMap->nMapX, pstMap->nMapY);
				break;
			}
		}		
		initialpos.insert(pair<int, Point3d>(pstMap->astUav[i].nNO, Point3d(posX, posY, posZ)));

	}


}
//分配无人机任务
int UAV2GOOG(GOODS *good, UAV uav, int goodnum, MAP_INFO *pstMap)
{
	int **map = CrearMap(pstMap, uav.nZ);
	Point start(uav.nX, uav.nY);
	int flag = 0;
	for (int i = 0; i < goodnum; i++)
	{
		if (good[i].nWeight < uav.nLoadWeight)
		{
			Point end(good[i].nStartX, good[i].nStartY);
			vector<Point>path;
			if (findpath(map, pstMap, path, start, end))
			{
				if (path.size() + uav.nZ <= good[i].nLeftTime)
				{
					flag = good[i].nNO;
					break;
				}
			}
		}
	}
	return flag;
}
// 获取相应货物编号对应的货物在数组中的位置
int getgoodno(int goodno, GOODS *Good, int goodnum)
{
	int i;
	int flag = -1;
	for (i = 0; i < goodnum; i++)
	{
		if (Good[i].nNO == goodno)
		{
			flag=i;
			break;
		}
	}
	return flag;
}
void moveuav(UAV uav, GOODS *Goods, int goodnum, unordered_map<int, UAVGoodStatue>UavandGood)
{
	int uavno = uav.nNO;
	if (UavandGood[uavno].statue == 0)
	{
		//分配任务
	}
}

//void ini_uav(MAP_INFO *pstMap)
/** @fn     void AlgorithmCalculationFun()
*  @brief	学生的算法计算， 参数什么的都自己写，
*	@return void
*/
void AlgorithmCalculationFun(MAP_INFO *pstMap, MATCH_STATUS * pstMatch, FLAY_PLANE *pstFlayPlane, vector<uav2good>Uav2Good, map<int, Point3d>&initialpos,int &goodno)
{

	//设置标志来查看初始化是否完成
	static bool IniFLAG = 0;
	if (IniFLAG == 0&&pstMatch->nTime>4)
	{
		int flag = 0;
		for (int i = 0; i < pstMatch->nUavWeNum; i++)
		{
			if (pstMatch->astWeUav[i].nStatus != 1 && (pstMatch->astWeUav[i].nX != initialpos[pstMatch->astWeUav[i].nNO].x||pstMatch->astWeUav[i].nY != initialpos[pstMatch->astWeUav[i].nNO].y||pstMatch->astWeUav[i].nZ != initialpos[pstMatch->astWeUav[i].nNO].z))
			{
				flag = 1;
				break;
			}
		}
		if (flag ==0)
			IniFLAG = 1;
	}
	int uavnum;
	//进行初始化
	if (pstMatch->nTime < pstMap->nHLow)
	{
		if (pstMatch->nTime == 1)
		{
			initialposfunc(pstMap, initialpos);//计算初始化坐标
			uavnum = pstMatch->nUavWeNum;
			pstFlayPlane->nUavNum = uavnum;
			for (int i = 0; i < uavnum; i++)
				pstFlayPlane->astUav[i] = pstMap->astUav[i];
			pstFlayPlane->astUav[0].nZ += 1;
		}
		else{
			uavnum = pstMatch->nUavWeNum;
			for (int i = 0; i < (pstMatch->nTime < uavnum ? pstMatch->nTime : uavnum); i++)
			{
				if (pstMap->astUav[i].nStatus != 1)
				{
					pstFlayPlane->astUav[i] = pstMatch->astWeUav[i];
					pstFlayPlane->astUav[i].nZ = pstMatch->astWeUav[i].nZ + 1;
				}
				else
				{
					uavnum--;
				}
			}
			pstFlayPlane->nUavNum = uavnum;
		}
	}
	else if (IniFLAG==0)
	{
		uavnum = pstMatch->nUavWeNum;
		srand((unsigned)time(NULL));
		for (int i = 0; i < uavnum; i++)
		{
			if (pstMatch->astWeUav[i].nStatus != 1)
			{
				if (pstMatch->astWeUav[i].nZ < initialpos[pstMatch->astWeUav[i].nNO].z)
				{
					pstFlayPlane->astUav[i].nX = pstMatch->astWeUav[i].nX;
					pstFlayPlane->astUav[i].nY = pstMatch->astWeUav[i].nY;
					pstFlayPlane->astUav[i].nZ = pstMatch->astWeUav[i].nZ + 1;
				}
				else if (pstMatch->astWeUav[i].nZ == initialpos[pstMatch->astWeUav[i].nNO].z)
				{
					int **map = CrearMap(pstMap, pstMatch->astWeUav[i].nZ);
					vector<Point> pathi;
					Point start(pstMatch->astWeUav[i].nX, pstMatch->astWeUav[i].nY);
					Point end(initialpos[pstMatch->astWeUav[i].nNO].x, initialpos[pstMatch->astWeUav[i].nNO].y);
					if (findpath(map, pstMap, pathi, start, end))

					{
						pstFlayPlane->astUav[i].nX = pathi[0].x;
						pstFlayPlane->astUav[i].nY = pathi[0].y;
						pstFlayPlane->astUav[i].nZ = pstMatch->astWeUav[i].nZ;
					}
					else if (start.x != end.x || start.y != end.y)
					{
						while (!findpath(map, pstMap, pathi, start, end) || map[end.x][end.y])
						{
							end.x = rand() % pstMap->nMapX;
							end.y = rand() % pstMap->nMapY;

						}
						initialpos[pstMatch->astWeUav[i].nNO].x = end.x;
						initialpos[pstMatch->astWeUav[i].nNO].y = end.y;

					}
					freemap(map, pstMap->nMapX, pstMap->nMapY);
				}
			}
			else
			{
				uavnum--;
			}
		}
		pstFlayPlane->nUavNum = uavnum;
	}
	//初始化完成，准备去拾取货物
	else if (IniFLAG==1)
	{
		int goodinshuzu;
		if (goodno <= 0)
		{
			goodno = UAV2GOOG(pstMatch->astGoods, pstMatch->astWeUav[0], pstMatch->nGoodsNum, pstMap);

		}
		for (int i = 0; i < pstMatch->nGoodsNum; i++)
		{
			if (pstMatch->astGoods[i].nNO == goodno)
			{
				goodinshuzu = i;
				break;
			}
		}
		//if (pstMatch->astGoods[goodinshuzu].nState!=0)
			//goodno = UAV2GOOG(pstMatch->astGoods, pstMatch->astWeUav[0], pstMatch->nGoodsNum, pstMap);
		if (goodno > 0 && pstMatch->astWeUav[0].nGoodsNo == -1)
		{
			if (0<pstMatch->astWeUav[0].nZ&&pstMatch->astWeUav[0].nZ < pstMap->nHLow)
			{
				if (pstMatch->astWeUav[0].nZ == 1)
				{
					for (int i = 0; i < pstMatch->nUavWeNum; i++)
						pstFlayPlane->astUav[i] = pstMatch->astWeUav[i];
					pstFlayPlane->nUavNum = pstMatch->nUavWeNum;
					pstFlayPlane->astUav[0].nZ = 0;
					pstFlayPlane->astUav[0].nGoodsNo = goodno;
				}
				else
				{
					for (int i = 0; i < pstMatch->nUavWeNum; i++)
						pstFlayPlane->astUav[i] = pstMatch->astWeUav[i];
					pstFlayPlane->nUavNum = pstMatch->nUavWeNum;
					pstFlayPlane->astUav[0].nZ = pstMatch->astWeUav[0].nZ - 1;
				}
			}
			else
			{
				Point good;
				Point start(pstMatch->astWeUav[0].nX, pstMatch->astWeUav[0].nY);
				for (int i = 0; i < pstMatch->nGoodsNum; i++)
				{
					if (pstMatch->astGoods[i].nNO == goodno)
					{
						good.x = pstMatch->astGoods[i].nStartX;
						good.y = pstMatch->astGoods[i].nStartY;
					}
				}
				if (pstMatch->astWeUav[0].nX == good.x&&pstMatch->astWeUav[0].nY == good.y)
				{
					for (int i = 0; i < pstMatch->nUavWeNum; i++)
						pstFlayPlane->astUav[i] = pstMatch->astWeUav[i];
					pstFlayPlane->nUavNum = pstMatch->nUavWeNum;
					pstFlayPlane->astUav[0].nZ = pstMatch->astWeUav[0].nZ - 1;
				}
				else
				{
					vector<Point> pathgood;
					int**map = CrearMap(pstMap, pstMatch->astWeUav[0].nZ);
					if (findpath(map, pstMap, pathgood, start, good))
					{
						for (int i = 0; i < pstMatch->nUavWeNum; i++)
							pstFlayPlane->astUav[i] = pstMatch->astWeUav[i];
						pstFlayPlane->nUavNum = pstMatch->nUavWeNum;
						pstFlayPlane->astUav[0].nX = pathgood[0].x;
						pstFlayPlane->astUav[0].nY = pathgood[0].y;

					}
				}

			}
		}

		//if (pstMatch->astWeUav[0].nStatus!=1&&pstMatch->astWeUav[0].nGoodsNo!=-1)

	}
	//{
	//	uavnum = pstMatch->nUavWeNum;

	//	int **map = CrearMap(pstMap, pstMap->nHLow + 1);
	//	for (int i = 0; i < 20; i++)
	//		for (int j = 0; j < 20; j++)
	//			printf("%d  ", map[i][j]);
	//	vector<Point> path0;
	//	Point start(pstMatch->astWeUav[0].nX, pstMatch->astWeUav[0].nY);
	//	if (pstMatch->nGoodsNum != 0)
	//	{
	//		//Point end(pstMatch->astGoods[0].nStartX, pstMatch->astGoods[0].nStartY);
	//		Point end(17,8);
	//		if (findpath(map, pstMap, path0, start, end))

	//		{
	//			pstFlayPlane->astUav[0].nX = path0[0].x;
	//			pstFlayPlane->astUav[0].nY = path0[0].y;
	//			pstFlayPlane->astUav[0].nZ = pstMatch->astWeUav[0].nZ;
	//			for (int i = 1; i < uavnum; i++)
	//			{
	//				pstFlayPlane->astUav[i] = pstMatch->astWeUav[i];
	//			}
	//		}
	//		else
	//		{
	//			for (int i = 0; i < uavnum; i++)
	//			{
	//				pstFlayPlane->astUav[i] = pstMatch->astWeUav[i];
	//			}
	//		}
	//	}

	//}
}

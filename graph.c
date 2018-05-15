#include<stdio.h>
#include<stdlib.h>
typedef struct Gnode* PtNode;
#define MaxV 1000
struct Gnode{
	int Nv;
	int Ne;
	int G[MaxV][MaxV];
	
};
typedef PtNode MGraph;
typedef struct Enode *PTEnode;
struct Enode{
	int V1;
	int V2;//有向边<v1,v2>
	int Weight;

};
typedef PTEnode Edge;
//初始化有V个顶点但是没有边 的图
MGraph CreatGraph(int Vertexnum)
{
	int V, W;
	MGraph graph;
	graph = (MGraph)malloc(sizeof(struct Gnode));
	graph->Ne = 0;
	graph->Nv = Vertexnum;
	for (V = 0; V < graph->Nv; V++)
		for (W = 0; W < graph->Nv; W++)
			graph->G[V][W] = 0;
}
void insertedge(MGraph graph,  Edge E)
{
	graph->G[E->V1][E->V2] = E->Weight;
	//若是无向图
	graph->G[E->V2][E->V1] = E->Weight;
}
//
MGraph buildgraph()
{
	int Nv,i;
	MGraph graph;
	Edge E;

	scanf("%d", &graph->Nv);
	graph = CreatGraph(Nv);
	scanf("%d", &(graph->Ne));
	if (graph->Ne != 0)
	{
		E = (Edge)malloc(sizeof(struct Enode));
		for (i = 0; i < graph->Ne; i++)
		{
			scanf("%d %d %d", &E->V1, &E->V1, &E->Weight);
			insertedge(graph, E);
		}
	}
}
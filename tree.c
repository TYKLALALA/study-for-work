#include<stdio.h>
#include<stdlib.h>
typedef struct Tnode* Tree;
struct Tnode
{
	Tree Right;
	Tree Left;
	int v;
	int flag;

};
Tree creatnode(int v)
{
	Tree tree = (Tree)malloc(sizeof(struct Tnode));
	tree->Left = NULL;
	tree->Right = NULL;
	tree->v = v;
	tree->flag = 0;
	return tree;
}
Tree insert(Tree tree, int v)
{
	if (!tree)
	{
		tree = creatnode(v);
	}
	else
	{
		if (v > tree->v)
			tree->Right=insert(tree->Right, v);
		else
		{
			tree->Left= insert(tree->Left, v);
		}
	}
	return tree;
}
Tree maketree(int N)
{
	int v;
	scanf("%d", &v);
	Tree tree = creatnode(v);
	for (int i = 1; i < N; i++)
	{
		scanf("%d", &v);
		insert(tree, v);
	}
	return tree;
}
//是返回0，否返回1；
int check(Tree tree, int v)
{
	if (!tree->flag&&v == tree->v)
	{
		tree->flag = 1;
		return 0;
	}
	else if (!tree->flag&&v != tree->v)
	{
		return 1;
	}
	else if (tree->flag&&v != tree->v)
	{
		if (v > tree->v)
			return check(tree->Right, v);
		else
		return	check(tree->Left, v);

	}
	else
	{
		return 1;
	}
	
}
//是为0，否为1；
int judge(Tree tree,int N)
{
	int v;
	int flag = 0;
	for (int i = 0; i < N; i++)
	{
		scanf("%d", &v);
		if (!flag&&check(tree, v))
			flag = 1;
	}
	if (flag)return 1;
	else
	{
		return 0;
	}
}
Tree resetTree(Tree tree)
{
	if (tree)
	{ 
		tree->flag = 0;
		resetTree(tree->Left);
		resetTree(tree->Right);
	}
	return tree;
}
void freeTree(Tree tree)
{
	if (tree)
	{
		free(tree);
		freeTree(tree->Left);
		freeTree(tree->Right);
	}
}
void main()
{
	int N;
	scanf("%d", &N);
	Tree tree = maketree(N);
	if (judge(tree, N))
		printf("no\n");
	else
	{
		printf("yes");
	}
	tree = resetTree(tree);
	free(tree);
	
}
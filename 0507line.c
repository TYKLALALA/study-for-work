#include<data_struct.h>
#include<stdio.h>
#include<stdlib.h>
typedef struct
{
	int data[MAXSIZE];
	int last;

}student;
//构造空线性表
student * listempty()
{
	student *ptr;
	ptr = (student*)malloc(sizeof(student));
	//ptr->data = (int *)malloc(sizeof(int)*MAXSIZE);
	ptr->last = -1;
	return ptr;
}
//查找元素
int find(int a, student *list)
{
	int i = 0;
	while (i<=list->last&&list->data[i]!=a)
	{
		i++;
	}
	if (i > list->last)
		return -1;
	else
	{
		return i;
	}
}
//插入元素
void insert(int a,int b, student *list)
{
	if (list->last == MAXSIZE - 1)
		return;
	if (a<1 || a>list->last + 2)
		return;
	int i = list->last+1;
	while (i>a-1)
	{
		list->data[i] = list->data[i - 1];
		i--;
	}
	list->data[i] = b;
	list->last++;
}
//删除元素
void delete_a(int a, student *list)
{
	if (a<1 || a>list->last + 1)
		return;
	int i = a-1;
	while (i<list->last-1)
	{
		list->data[i] = list->data[i + 1];
		i++;
	}
	list->last--;
}
//链式操作实现堆栈
typedef struct snode*stack;
struct snode
{
	int data;
	struct snode *next;
};
stack createmptylist()
{
	stack node;
	node = (stack)malloc(sizeof(struct snode));
	node->next = NULL;
	return node;
}
int isempty(stack s)
{
	if (s->next == NULL)
		return 1;
	else
	{
		return 0;
	}
}
void push(int value,stack S)
{
	stack temp = (stack)malloc(sizeof(struct snode));
	temp->data = value;
	temp->next = S->next;
	S->next = temp;

}
int pop(stack S)
{
	int value;
	if (isempty(S))
	{
		printf("空栈\n");
		return;
	}
	stack temp = S->next;
	value = temp->data;
	S->next = temp->next;
	free(temp);
	return value;
}
void main()
{
	student *list = listempty();
	for (int i = 0; i < 10; i++)
	{
		list->data[i] = 100;
	}
	list->last = 9;
	for (int i = 0; i < 10; i++)
	{
		insert(1, i, list);
	}
	delete_a(3, list);
	stack list1 = createmptylist();
	for (int i = 0; i < 10; i++)
		push(i, list1);
	for (int i = 0; i < 5; i++)
		printf("%d　　", pop(list1));
}

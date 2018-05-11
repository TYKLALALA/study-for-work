#include<stdio.h>
#include<stdlib.h>
typedef struct HeapStruct*Heap;
struct HeapStruct
{
	int *data;
	int size;
	int capicity;
}; 

Heap creat(int maxsize)
{
	Heap heap = (Heap)malloc(sizeof(struct  HeapStruct));
	heap->data = (int*)malloc(sizeof(int)*(maxsize + 1));//从第一个开始放哨兵
	//heap->data[0] = 1000;// 创建最大堆】
	heap->data[0] = -1000;
	heap->size = 0;
	heap->capicity = maxsize;
	return heap;
}

int deletemax(Heap heap)
{
	int parent, child;
	int maxdata, temp;
	if (heap->size == 0)
		return;
	maxdata = heap->data[1];
	temp = heap->data[heap->size--];
	for (parent = 1; parent * 2 <= heap->size; parent = child)
	{
		child = parent * 2;
		if ((child != heap->size) && (heap->data[child] < heap->data[child + 1]))
			child++;
		if (temp > heap->data[child])
			break;
		else
		{
			heap->data[parent] = heap->data[child];
		}

	}
	heap->data[parent] = temp;
	return maxdata;

}
//堆中插入元素
void insert(Heap heap, int v)
{
	int i;
	if (heap->size == heap->capicity)
	{
		printf("yiman\n");
		return;
	}
	heap->size++;
	for (i = heap->size; heap->data[i/2]<v; i = i / 2)
	{
		heap->data[i] = heap->data[i / 2];
	}
	heap->data[i] = v;
	
}
//创建最大堆
void creatmaxheap(Heap heap)
{
	int j = heap->size;
	for (; j >= 2; j--)
	{
		for (int i = j / 2; i >= 1; i = i / 2)
		{
			if (heap->data[j] > heap->data[i])
			{
				int temp = heap->data[i];
				heap->data[i ] = heap->data[j];
				heap->data[j] = temp;
			}
		}
	}
}

void insertminheap(Heap heap, int v)
{
	int i;
	for (i = ++heap->size; heap->data[i / 2] > v; i = i / 2)
		heap->data[i] = heap->data[i / 2];
	heap->data[i] = v;
}
void display(Heap heap,int m)
{
	int i = m;
	printf("%d\n", heap->data[i]);
	while (i>1)
	{
		i /= 2;
		printf("%d\n", heap->data[i]);
	   
	}

}

void main()
{
	Heap heap = creat(30);
	insertminheap(heap, 46);
	insertminheap(heap, 23);
	insertminheap(heap, 26);
	insertminheap(heap, 24);
	insertminheap(heap, 10);
	heap->size = 5;
	for (int i = 0; i < 6; i++)
	{
		printf("%d\n", heap->data[i]);
	}
	display(heap, 4);
	//int b = deletemax(heap);
	/*creatmaxheap(heap);
	for (int i = 0; i < 6; i++)
	{
		printf("%d\n", heap->data[i]);
	}*/
	

}


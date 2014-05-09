#include <iostream>
using namespace std;


int array[4][4] = {
	{0 , 0 , 1, 0},
	{1,  0 , 0, 1},
	{1,  1,  0, 0},
	{1,  1,  1, 0}
};

typedef  int  VertexType;

struct EdgeNode
{
	int adjvex;
	struct EdgeNode*next;
};

struct VertexNode
{
	VertexType data;
	int index;
	EdgeNode*firstedge;
};

struct QueueNode
{
	struct VertexNode QueueData;
	struct QueueNode*next;
};

struct Queue
{
	struct QueueNode* front, *rear;
};

class Maze{
private:
	struct Queue Q;
	struct VertexNode*Nodes;
	int numberofzero;
public:
	void CreateGraph(int array [][4]);
	bool InitQueue();
	bool EnQueue(int i); //add the ith element of Nodes to the Queue
	bool DeQueue(int*i); //Delete the first element of the Queue and add the number of this element to *i
	bool QueueEmpty();
	bool Adjacency(int a, int b);
	bool IsMazeSoluted(int array [][4]);
	
	~Maze();
};

Maze::~Maze()
{
	for(int i = 0; i < numberofzero; ++i)
	{
		EdgeNode*p = Nodes[i].firstedge;
		while(p)
		{
			EdgeNode*q = p->next;
			delete p;
			p = q;
		}
	}
	delete []Nodes;
}

bool Maze::Adjacency(VertexType a, VertexType b)
{
	if(a - b == 1|| a -b ==-1|| a-b ==4|| a - b == -4)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void Maze::CreateGraph(int array [][4])
{
	int numberofzero = 0;
	//判断节点中是0的点有几个												
	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			if( 0 == array[i][j])
			{
				++numberofzero;
			}
		}
	}
	this->numberofzero = numberofzero;
	
	//用来存储节点在array中位置的动态数组
	int*VertexType = new int[numberofzero];			
	int m = 0;
	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			if(0 == array[i][j])
			{
				VertexType[m] = j + i*4;
				m++;
			}
		}
	}
	
	//初始化邻节表中的各个节点
	Nodes = new VertexNode[numberofzero];
	for(int i = 0; i < numberofzero; ++i)
	{
		Nodes[i].data = VertexType[i];
		Nodes[i].index = i;
		Nodes[i].firstedge = NULL;
	}

	
	bool**Link = new bool*[numberofzero];
	for(int i = 0; i < numberofzero; ++ i)
	{
		Link[i] = new bool[numberofzero];
	}
	for(int i = 0; i < numberofzero; ++i)
	{
		for(int j = 0; j < numberofzero; ++ j)
			Link[i][j] = false;
	}

	//构造邻边表
	for(int i =0 ; i < numberofzero; ++i)
	{
		for(int j = 0; j < i ;++ j)
		{
			if(Link[i][j] == true)
			{
				EdgeNode* p  = new EdgeNode();
				p->adjvex = j;
				p->next = Nodes[i].firstedge;
				Nodes[i].firstedge = p;
			}
			else if(Adjacency(Nodes[i].data, Nodes[j].data))
			{
				EdgeNode* p  = new EdgeNode();
				p->adjvex = j;
				p->next = Nodes[i].firstedge;
				Nodes[i].firstedge = p;
				Link[i][j] = true;
				Link[j][i] = true;
			}
		}
		
		for(int j = i+1; j < numberofzero; ++j)
		{
			if(Link[i][j] == true)
			{
				EdgeNode* p  = new EdgeNode();
				p->adjvex = j;
				p->next = Nodes[i].firstedge;
				Nodes[i].firstedge = p;
			}
			else if(Adjacency(Nodes[i].data, Nodes[j].data))
			{
				EdgeNode* p  = new EdgeNode();
				p->adjvex = j;
				p->next = Nodes[i].firstedge;
				Nodes[i].firstedge = p;
				Link[i][j] = true;
				Link[j][i] = true;
			}
		}
	}
	for(int i = 0 ; i < numberofzero; ++i)
	{
		delete []Link[i];
	}
	delete []Link;
}

bool Maze::InitQueue()
{
	struct QueueNode*p = new QueueNode();
	if( p == NULL)
		return false;
	p->QueueData.data = '*';
	p->QueueData.firstedge = NULL;
	p->QueueData.index = -1;
	p->next = NULL;
	Q.front = Q.rear = p;
	return true;
}

bool Maze::EnQueue(int i)
{
	struct QueueNode*p = new QueueNode();
	if(NULL == p)
		return false;
	p->QueueData.data = (Nodes+i)->data;
	p->QueueData.firstedge = (Nodes+i)->firstedge;
	p->QueueData.index = (Nodes+i)->index;
	p->next = NULL;
	Q.rear ->next = p;
	Q.rear = p;
	return true;
}

bool Maze::DeQueue(int*i)
{
	if(Q.front == Q.rear)
		return false;
	struct QueueNode*p = Q.front->next;
	Q.front->next = p->next;
	*i = p->QueueData.index;
	if(p == Q.rear)
	{
		Q.rear = Q.front;
	}
	delete p;
	return true;
}

bool Maze::QueueEmpty()
{
	if(Q.front == Q.rear)
		return true;
	else
		return false;
}

bool Maze::IsMazeSoluted(int array [][4])
{
	CreateGraph(array);

	int i;
	InitQueue();
	bool*visited = new bool[numberofzero];
	for(int i = 0; i < numberofzero; ++i)
	{
		visited[i] = false;
	}
	
	EnQueue(0);
	while(!QueueEmpty())
	{
		
		DeQueue(&i);
		if(visited[i] ==false)
		{
			visited[i] = true;
 			if(Nodes[i].data == 15)
				return true;
			
			struct EdgeNode*p = Nodes[i].firstedge;
			while(p)
			{
				EnQueue(p->adjvex);
				p = p->next;
			}
		}

	}
	return false;
}

int main()
{   Maze *a = new Maze();
	
	cout << a->IsMazeSoluted(array);
	return 0;
}

#include <bits/stdc++.h>
#include <fstream>
#include <map>

using namespace std;

struct FrameMem
{
	FrameMem *prev;
	FrameMem *next;
	string pageid;
};

class Frame
{
private:
	FrameMem * start, *end;
	int size, max;
	map<string, FrameMem*> mapframe;
public:
	void insert(string pageid)
	{
		if (size == max)
		{
			FrameMem *f = end;
			end = end->prev;
			end->next = NULL;
			string id = f->pageid;
			delete f;
			mapframe.erase(id);
			size--;
		}
		FrameMem *a = new FrameMem;
		a->pageid = pageid;
		a->prev = NULL;
		a->next = start;
		if (start) {
			start->prev = a;
		}
		else end = a;
		start = a;
		mapframe[a->pageid] = a;
		size++;
	}

	void adjust(string pageid)
	{
		FrameMem *pos = mapframe[pageid];
		if (pos != start)
		{
			FrameMem *p = pos->prev, *n = pos->next;
			if (n)
			{
				p->next = n;
				n->prev = p;
			}
			else
			{
				p->next = NULL;
				end = p;
			}
					
			pos->prev = NULL;
			pos->next = start;
			start->prev = pos;
			start = pos;
		}
	}

	bool isinmap(string pageid){return (mapframe.find(pageid) != mapframe.end())? true : false;}

	Frame(int a)
	{
		size = 0;
		max = a;
		start = NULL;
		end = NULL;
	}
};

int main() {
	ifstream trace;
	//FIFO
	printf("FIFO---\n");
	printf("%-8s%-8s%-15s%s\n", "size", "miss", "hit", "page fault ratio");
	for (int size = 64; size <= 512; size*=2)
	{
		int miss = 0, hit = 0;
		string id;
		Frame frame(size);
		trace.open("trace.txt");
		while (trace >> id)
		{
			string pageid = id.substr(0, 5);
			if (frame.isinmap(pageid))
			{
				hit++;
			}
			else
			{
				miss++;
				frame.insert(pageid);
			}
		}
		printf("%-8d%-8d%-15d%.9f\n", size, miss, hit, (double)miss / (miss + hit));
		trace.close();
	}
	
	//LRU
	printf("\nLRU---\n");
	printf("%-8s%-8s%-15s%s\n", "size", "miss", "hit", "page fault ratio");
	for (int size = 64; size <= 512; size *= 2)
	{
		int miss = 0, hit = 0;
		string id;
		Frame frame(size);
		trace.open("trace.txt");
		while (trace >> id)
		{
			string pageid = id.substr(0, 5);
			if (frame.isinmap(pageid))
			{
				hit++;
				frame.adjust(pageid);
			}
			else
			{
				miss++;
				frame.insert(pageid);
			}
		}
		printf("%-8d%-8d%-15d%.9f\n", size, miss, hit, (double)miss / (miss + hit));
		trace.close();
	}
}

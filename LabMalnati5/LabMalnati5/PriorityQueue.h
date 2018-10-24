#pragma once
#include <chrono>
#include <mutex>
#include <queue>
#include <future>
#include <iostream>

using namespace std;

struct packaged_packaged_task {
	chrono::milliseconds curr_time;
	shared_ptr<packaged_task<void()>> task;
	bool operator<(const packaged_packaged_task &ppt) const{
		//we want inverse order
		return curr_time > ppt.curr_time;
	}
};

class PriorityQueue
{
public:
	priority_queue<packaged_packaged_task> pq;
	mutex m;
	condition_variable cv;
	bool valid;

	//to add
	//chrono::milliseconds nexttime;


	void push(shared_ptr<packaged_task<void()>> &task);
	void push_delayed(shared_ptr<packaged_task<void()>> &task,chrono::milliseconds delay);
	bool pop(shared_ptr<packaged_task<void()>> &pt);
	bool empty();
	void invalidate();

	PriorityQueue();
	~PriorityQueue();
};

	
#include "stdafx.h"
#include "PriorityQueue.h"

using Clock = std::chrono::high_resolution_clock;
using Ms = std::chrono::milliseconds;

template<class Duration>
using TimePoint = std::chrono::time_point<Clock, Duration>;

PriorityQueue::PriorityQueue()
{
	valid = true;
	//nexttime = chrono::milliseconds::max();
}


PriorityQueue::~PriorityQueue()
{
	if(valid==true)
		invalidate();
}

void PriorityQueue::push(shared_ptr<packaged_task<void()>> &task) {
	lock_guard<mutex> l(m);
	struct packaged_packaged_task ppt;
	chrono::milliseconds curr_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
	ppt.curr_time = curr_time;
	ppt.task = task;
	pq.push(ppt);
	
	//added
	//nexttime = curr_time;

	cv.notify_one();
}
void PriorityQueue::push_delayed(shared_ptr<packaged_task<void()>> &task, chrono::milliseconds delay) {
	lock_guard<mutex> l(m);
	struct packaged_packaged_task ppt;
	
	chrono::milliseconds task_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()) + delay;

	ppt.curr_time = task_time;
	ppt.task = task;
	pq.push(ppt);

	/*
	 if (nexttime > task_time)
	{
		nexttime = task_time;
		cv.notify_all();
	}
	*/ 
	cv.notify_one();
}
bool PriorityQueue::pop(shared_ptr<packaged_task<void()>> &pt) {
	unique_lock<mutex> l(m);
/*

cout << "time before: " << chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count() << endl;
cv.wait_until(l, TimePoint<Ms> (this->nexttime), [this]() {return !(valid //&& this->pq.empty()
// &&chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()) > this->nexttime))
);});
cout << "time after: " << chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count() << endl;
*/

	cv.wait(l, [this]() {return !(this->pq.empty() && valid ) ;});

	//cv.wait_until(l, chrono::time_point<chrono::milliseconds>(this->nexttime), [this]() {return !(valid && this->pq.empty()); });


	if (!valid) {
		return false;
	}
	if (this->pq.empty()) {
		cout << "something does not work\n";
		return false;
	}
	packaged_packaged_task ppt = pq.top();
	pq.pop();

/*
if (pq.empty()) {
this->nexttime = chrono::milliseconds::max();
//cv.notify_all();
}
else {
this->nexttime = pq.top().curr_time;
//cv.notify_all();
}
*/

	l.unlock();
	//cv.notify_all();


	pt = ppt.task;
	return true;

}

bool PriorityQueue::empty() {
	lock_guard<mutex> l(m);
	return pq.empty();
}

void PriorityQueue::invalidate() {
	lock_guard<mutex> l(m);
	valid = false;
	cv.notify_all();
}

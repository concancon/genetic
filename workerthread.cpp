#include "workerthread.h"

WorkerThread::WorkerThread() :isRunning(false)
{
	thread.reset(new std::thread([this]  // makes a new thread
	{    //this is the stuff that runs in the thread
		isRunning = true;
		startThread(); // here we set a mutex
	}));
}

WorkerThread::~WorkerThread()
{
	stop();	
}

void WorkerThread::startThread()
{
	std::unique_lock<std::mutex> l(mutex);

    //running thread accepts new tasks while isRunning==true
	do
	{
		while (isRunning && tasks.empty())
			itemInQueue.wait(l); //itemInQueue is a condition variable. waiting is used to allow other threads to start their tasks and to make this thread idle (waiting for more tasks)

		while (!tasks.empty())
		{
			/*
			auto size = tasks.size();
			printf("Number of pending task are %lu\n", size);
			 */
			const std::function<void()> t = tasks.front();
			tasks.pop_front();
			l.unlock();
			t(); //processing the task itself
			l.lock();
		}
		itemInQueue.notify_all();


	} while (isRunning);
	itemInQueue.notify_all();
}

void WorkerThread::doAsync(const std::function<void()>& t) //add new tasks. a new task is passed a function reference
{
	std::lock_guard<std::mutex> _(mutex);
	tasks.push_back(t);
	itemInQueue.notify_one(); //when this is called then wait returns. that means this is what stops wait from waiting. It tells the do while loop above that theres stuff to do

}

void WorkerThread::doSync(const std::function<void()>& t)
{
	std::condition_variable event;
	bool finished = false;

	std::unique_lock<std::mutex> l(mutex);
	auto lambda = [this, &t, &finished, &event]
	{
		t();
		std::lock_guard<std::mutex> l(mutex);

		finished = true;
		event.notify_one();
	};
	tasks.push_back(lambda);
	itemInQueue.notify_one();

	while (!finished)
		event.wait(l);

}

void WorkerThread::wait()  //wait until there are no more tasks to complete. this allows us to for example wait until all the parallel threads are done so we can collect the result
{
	std::unique_lock<std::mutex> l(mutex);
	while (!tasks.empty())
		itemInQueue.wait(l);
}

void WorkerThread::stop()  //this stops the thread function. it gets called when the destructor is called.
{
	{
		std::lock_guard<std::mutex> l(mutex);
		isRunning = false;
		itemInQueue.notify_one();
	}
	thread->join();
}

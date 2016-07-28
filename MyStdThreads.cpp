#include<iostream>
#include<thread>
using namespace std;

namespace task {
class thread {
 public:
 virtual ~thread() {}
 virtual void run(void *(*routine) (void *), void * arg) = 0;
 virtual bool joinable() = 0;
 virtual void join() = 0;
 };
}

class MyThread:public task::thread
{
public:
	MyThread()
	{
		cout << "MyThread constructor\n";
	}
	~MyThread()
	{
		if(myThread.joinable())
		{
			cout << "MyThread joinable\n";
        	myThread.join();
		}
		cout << "MyThread destructor\n";
	}
	void run(void *(*routine) (void *), void * arg)
	{
		myThread = std::thread(routine,arg);
	}
	
	bool joinable()
	{
		if(myThread.joinable())
		{
			cout<<"Joinable\n";
			return true;
		}
		else
		{
			cout<<"Not Joinable\n";
			return false;
		}
	}
	
	void join()
	{
		std::cout << "Thread join" << std::endl;
	}
	

	private:
		std::thread myThread;	
};

void *call_from_thread(void* tid) {
    std::cout << "Launched by thread " << *(int*)(tid) << std::endl;
}
int main()
{
	MyThread theThread;
	
	int threadArg=1;
	
	theThread.run(&call_from_thread,(void*)&threadArg);
	
	if(theThread.joinable())
		theThread.join();
		
	return 0;
}

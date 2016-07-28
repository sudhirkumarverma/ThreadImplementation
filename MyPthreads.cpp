#include<iostream>
#include<pthread.h>
#include<stdlib.h>
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

using namespace task;

void *threadRoutine1(void *t);
void *threadRoutine2(void *t);

enum ThreadType{THREAD_JOINABLE=0,THREAD_DETACHABLE};

class MyThread:public thread
{
public:
	
	MyThread()
	{
		pthread_attr_init(&myAttr); // always set attribute of thread as Joinable
		pthread_attr_setdetachstate(&myAttr, PTHREAD_CREATE_JOINABLE);
	}

	MyThread(ThreadType threadType)
	{
		
		pthread_attr_init(&myAttr); // always set attribute of thread as Joinable
		if(threadType==THREAD_JOINABLE)
			pthread_attr_setdetachstate(&myAttr, PTHREAD_CREATE_JOINABLE);
		else if(threadType==THREAD_DETACHABLE)
			pthread_attr_setdetachstate(&myAttr, PTHREAD_CREATE_DETACHED);
	}

	virtual ~MyThread() {
	//pthread_attr_destroy(&myAttr);
	}

	 virtual void run(void *(*routine) (void *), void * arg) 
	 {
	 	cout<<"Inside function void run()"<<endl;
	 	int rc = pthread_create( &myThreadId, &myAttr, routine, arg);
	 	if (rc) {
          cout<<"ERROR; return code from pthread_create() is ";
          exit(-1);
          }
	 }
	 
	 virtual bool joinable() {
	 	cout<<"Inside function bool joinable()-  Checking if thread joinable"<<endl;
	 	pthread_attr_t Attr;
	 	int detachstate;
	 	if(pthread_attr_getdetachstate (&Attr, &detachstate)!=0){
		 
	 		cout<<"\n Error : Couldnot get thread detach state ";
	 	}
		 else
	 	{
	 		if(detachstate == PTHREAD_CREATE_JOINABLE)
	 		{
				cout<<" JOINABLE"<<endl;
	 			return true;
	 		}
	 		else if(detachstate == PTHREAD_CREATE_DETACHED)
	 		{
				cout<<" DETAVCHABLE"<<endl;
	 			return false;
	 		}
		}
	 	
	 	
	 }
	 virtual void join() {
	 	cout<<"Inside function void join() "<<endl;
	 	pthread_join(myThreadId,NULL);
	 }
	 void destroyAttr()
	 {
	 	pthread_attr_destroy(&myAttr);
	 }
 private:
 	pthread_t  myThreadId;
	pthread_attr_t myAttr;
	int m_running;
};

void *threadRoutine1(void *t)
{
	cout<<"In threadRoutine1"<<endl;
}

void *threadRoutine2(void *t)
{
	cout<<"In threadRoutine2"<<endl;
}

int main(int argc, char* argv[])
{
	MyThread thread(THREAD_JOINABLE);  // Type of thread to be created
	thread.run(threadRoutine1,NULL); // Create thread for threadRoutine1 and pass NULL as argument to threadRoutine1
	thread.destroyAttr();
	if(thread.joinable())
	thread.join();	
	
	MyThread thread1(THREAD_DETACHABLE);
	thread1.run(threadRoutine2,NULL); // Create thread for threadRoutine1 and pass NULL as argument to threadRoutine1
	thread1.destroyAttr();
	if(thread1.joinable())
	thread1.join();	
	return 0;
}

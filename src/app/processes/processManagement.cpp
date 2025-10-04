#include "processManagement.hpp"
#include "Cryption.hpp"
#include<sys/mman.h>
#include<sys/fcntl.h>
#include<semaphore.h>


ProcessManagement::ProcessManagement(){
    item_semaphore = sem_open("/items_semaphore",O_CREAT,0666,0);
    empty_semaphore = sem_open("/empty_semaphore",O_CREAT,0666,1000);
    shmfd = shm_open(SHM_NAME,O_CREAT | O_RDWR,0666);
    ftruncate(shmfd,sizeof(SharedMemory));
    sharedMem = static_cast<SharedMemory*>(mmap(nullptr,sizeof(SharedMemory),PROT_READ | PROT_WRITE,MAP_SHARED,shmfd,0));
    sharedMem->front = 0;
    sharedMem->rear=0;
    sharedMem->size.store(0);
}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task>task){
    sem_wait(empty_semaphore);
    std::unique_lock<std::mutex>lock(queue_lock);
    if(sharedMem->size.load()>=1000) return false;

    strcpy(sharedMem->tasks[sharedMem->rear],task->toString().c_str());
    sharedMem->rear = (sharedMem->rear+1)%1000;
    sharedMem->size.fetch_add(1);
    lock.unlock();
    sem_post(item_semaphore);

    // creating child process for multiprocessing which is slow ....

    // int pid = fork();
    // if(pid < 0){
    //     return false;
    // }
    // else if(pid > 0){
    //     std::cout<<"Entering the parent process"<<std::endl;
    // }else {
    //     std::cout<<"Entering the child process"<<std::endl;
    //     executeTasks();
    //     std::cout<<"Exiting the child process"<<std::endl;
    //     exit(0);
    // }

    

    // multithreading is faster than multiprocessing in this case
    
    std::thread t(&ProcessManagement::executeTasks,this);
    t.detach();
    return true;
}

void ProcessManagement::executeTasks(){

    sem_wait(item_semaphore);
    std::unique_lock<std::mutex>lock(queue_lock);

    char task[256];
    strcpy(task,sharedMem->tasks[sharedMem->front]);
    sharedMem->front = (sharedMem->front+1)%1000;
    sharedMem->size.fetch_sub(1);
    lock.unlock();
    sem_post(empty_semaphore);

    std::cout<<"Executing task: "<<task<<std::endl;
    executeCryption(task);
}

#ifndef PROCESS_MANAGEMENT_HPP
#define PROCESS_MANAGEMENT_HPP

#include<bits/stdc++.h>
#include "Task.hpp"
#include<semaphore.h>

class ProcessManagement {
    sem_t *item_semaphore;
    sem_t *empty_semaphore;
    public:
        ProcessManagement();
        bool submitToQueue(std::unique_ptr<Task>task);
        void executeTasks();

    private:
        struct SharedMemory{
            std::atomic<int>size;
            char tasks[1000][256];
            int front;
            int rear;

            void printSharedMemory(){
                std::cout<<"Size : "<<size<<std::endl;
            }
        };

        SharedMemory *sharedMem;
        int shmfd;
        const char* SHM_NAME = "/my_queue";
        std::mutex queue_lock;
};

#endif
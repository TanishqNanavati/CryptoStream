#include "processManagement.hpp"
#include "Cryption.hpp"


ProcessManagement::ProcessManagement(){}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task>task){
    taskQueue.push(std::move(task));
    return true;
}

void ProcessManagement::executeTasks(){
    while(!taskQueue.empty()){
        std::unique_ptr<Task> taskToExecute = std::move(taskQueue.front());
        taskQueue.pop();

        std::cout<<taskToExecute->toString()<<std::endl;
        executeCryption(taskToExecute->toString());
    }
}
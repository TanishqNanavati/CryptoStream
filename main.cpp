#include<bits/stdc++.h>
#include "processManagement.hpp"
#include "Task.hpp"

namespace fs = std::filesystem;

int main(int argc,char *argv[]){

    std::string directory;
    std::string action;

    std::cout<<"Enter the directory path : "<<std::endl;
    std::getline(std::cin,directory);

    std::cout<<"Enter the action : "<<std::endl;
    std::getline(std::cin,action);

    try{
        if(fs::exists(directory) && fs::is_directory(directory)){
            ProcessManagement processManagement;
            for(const auto &entry : fs::directory_iterator(directory)){
                if(entry.is_regular_file()){
                    std::string filePath = entry.path().string();
                    IO io(filePath);
                    std::fstream f_stream = std::move(io.getFileStream());
                    if(f_stream.is_open()){
                        Action task_action = (action =="ENCRYPT") ? (Action::ENCRYPT) : (Action::DECRYPT);
                        auto task = std::make_unique<Task>(std::move(f_stream),task_action,filePath);
                        processManagement.submitToQueue(std::move(task));
                    }else{
                        throw std::runtime_error("Unable to open the file : "+filePath);
                    }
                }
            }

            processManagement.executeTasks();
        }else{
            throw std::runtime_error("The provided path is not a valid directory.");
        }
    }catch(fs::filesystem_error &e){
        std::cerr<<"Error : "<<e.what()<<std::endl;
        return 1;
    }
}
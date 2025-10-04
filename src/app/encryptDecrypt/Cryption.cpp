#include "Cryption.hpp"
#include "Task.hpp"
#include "../filehandling/ReadEnv.cpp"
#include<bits/stdc++.h>

int executeCryption(const std::string &taskData){
    Task task = Task::fromString(taskData);

    ReadEnv env;

    std::string envKey = env.getEnv();

    int key = std::stoi(envKey);

    if(task.action == Action::ENCRYPT){
        char ch;
        while(task.f_stream.get(ch)){
            ch = (ch+key)%256;
            task.f_stream.seekp(-1,std::ios::cur);
            task.f_stream.put(ch);
        }
        task.f_stream.close();
        std::cout << "File encrypted successfully: " << task.filePath << std::endl;
    }else{
        char ch;
        while(task.f_stream.get(ch)){
            ch = (ch-key+256)%256;
            task.f_stream.seekp(-1,std::ios::cur);
            task.f_stream.put(ch);
        }
        task.f_stream.close();
        std::cout << "File decrypted successfully: " << task.filePath << std::endl;

    }

    std::time_t t = std::time(nullptr);
    std::tm * now  = std::localtime(&t);
    std::cout<<"Exiting the Encryption/Decryption : "<<std::put_time(now,"%Y-%m-%d %H:%M:%S")<<std::endl;

    return 0;

}
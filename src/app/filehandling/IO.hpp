#ifndef IO_HPP
#define IO_HPP

#include<bits/stdc++.h>

class IO{
    public:
        IO(const std::string &file_path);
        ~IO();
        std::fstream getFileStream();

    private:
        std::fstream file_stream;
};

#endif
#include<bits/stdc++.h>
#include "processManagement.hpp"
#include "Task.hpp"

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    std::string directory;
    std::string action;

    std::cout << "Enter the directory path : " << std::endl;
    std::getline(std::cin, directory);

    std::cout << "Enter the action : " << std::endl;
    std::getline(std::cin, action);

    try {
        if (fs::exists(directory) && fs::is_directory(directory)) {
            ProcessManagement processManagement;

            // ⏱️ Start time
            auto start = std::chrono::high_resolution_clock::now();

            for (const auto &entry : fs::directory_iterator(directory)) {
                if (entry.is_regular_file()) {
                    std::string filePath = entry.path().string();
                    IO io(filePath);
                    std::fstream f_stream = std::move(io.getFileStream());
                    if (f_stream.is_open()) {
                        Action task_action = (action == "ENCRYPT") ? Action::ENCRYPT : Action::DECRYPT;
                        auto task = std::make_unique<Task>(std::move(f_stream), task_action, filePath);
                        processManagement.submitToQueue(std::move(task));
                    } else {
                        throw std::runtime_error("Unable to open the file : " + filePath);
                    }
                }
            }

            // ⚠️ Important: Wait a bit to let all detached threads finish
            // (since you're detaching, you must block main until they're done)
            std::cout << "Waiting for all threads to finish..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5)); // adjust if needed

            // ⏹️ End time
            auto end = std::chrono::high_resolution_clock::now();

            // 🧮 Calculate duration
            std::chrono::duration<double> duration = end - start;
            std::cout << "\nTotal Execution Time: " << duration.count() << " seconds\n";
        } else {
            throw std::runtime_error("The provided path is not a valid directory.");
        }
    } catch (fs::filesystem_error &e) {
        std::cerr << "Error : " << e.what() << std::endl;
        return 1;
    }
}

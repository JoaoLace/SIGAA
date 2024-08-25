#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

struct Task {
    std::string days;
    std::string name;
    std::string desc;
    std::string date;
};

enum {
    DAYS = 0,
    NAME,
    DESC,
    DATE
};

int lines(const std::string& file_name) {
    std::ifstream file(file_name);
    int count = 0;
    std::string line;
    
    while (std::getline(file, line)) {
        if (!line.empty()) {
            count++;
        }
    }
    
    return count;
}

void into_task(std::vector<Task>& tasks, const std::string& file_name) {
    std::ifstream file(file_name);
    
    if (!file) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }
    
    Task task;
    size_t line = 0;
    
    std::string str;
    while (std::getline(file, str)) {
        if (str.empty()) continue;
        
        switch (line) {
            case DAYS:
                task.days = str;
                line++;
                break;
            case NAME:
                task.name = str;
                line++;
                break;
            case DESC:
                task.desc = str;
                line++;
                break;
            case DATE:
                task.date = str;
                tasks.push_back(task);
                task = Task();  // Reset task
                line = 0;  // Reset line count
                break;
        }
    }
    
    if (tasks.empty()) {
        std::cerr << "No tasks found in file" << std::endl;
    }
}

void showTasks(const std::vector<Task>& tasks) {
    for (size_t i = 0; i < tasks.size(); ++i) {
        std::cout << "Task " << i + 1 << ":\n";
        std::cout << "Days: " << tasks[i].days << "\n";
        std::cout << "Name: " << tasks[i].name << "\n";
        std::cout << "Description: " << tasks[i].desc << "\n";
        std::cout << "Date: " << tasks[i].date << "\n";
        std::cout << "\n";
    }
}

void errorNoCommands() {
    std::cout << "Nenhum comando fornecido.\n";
    std::cout << "Lista de comandos: \n- Init\n- Show\n- Done\n\n";
}

void markTaskDone(std::vector<Task>& tasks, int task_index) {
    if (task_index >= 0 && task_index < tasks.size()) {
        std::cout << "Marking task " << task_index + 1 << " as done.\n";
        // Implement logic to mark the task as done
    } else {
        std::cerr << "Invalid task index" << std::endl;
    }
}

void updateFileWithDoneTasks(const std::string& file_name, const std::vector<Task>& tasks) {
    std::ofstream file(file_name, std::ios::trunc);
    
    if (!file) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }
    
    for (const auto& task : tasks) {
        file << task.days << "\n" << task.name << "\n" << task.desc << "\n" << task.date << "\n\n";
    }
}

int main(int argc, char *argv[]) {
    if (argc >= 2) {
        const std::string file_name = "tasks.txt";
        std::string command = argv[1];
        std::transform(command.begin(), command.end(), command.begin(), ::toupper);

        int total_lines = lines(file_name);
        
        if (total_lines % 4 != 0) {
            std::cerr << "File does not have a proper number of lines for tasks" << std::endl;
            return 1;
        }

        int number_of_tasks = total_lines / 4;
        std::vector<Task> tasks;

        into_task(tasks, file_name);

        if (command == "SHOW") {
            showTasks(tasks);
        } 
        
        else if (command == "DONE") {
            if (argc == 3) {
                int index = std::stoi(argv[2]) - 1;
                markTaskDone(tasks, index);
                updateFileWithDoneTasks(file_name, tasks);
            } else {
                std::cerr << "Usage: ./program DONE <task_index>\n";
            }
        } 
        
        else if (command == "INIT"){
            // Implement the INIT command functionality
        }

        else {
            errorNoCommands();
        }
    } else {
        errorNoCommands();
        return 1;
    }

    return 0;
}

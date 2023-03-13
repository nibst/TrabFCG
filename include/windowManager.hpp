#pragma once
#include <string>
#include <GLFW/glfw3.h>  // Criação de janelas do sistema operacional
class WindowManager{
    public:
        WindowManager();
        GLFWwindow* createWindow();
        void updateWindow();
        void closeWindow();
        bool shouldCloseWindow();
        GLFWwindow* getWindow();
    private:
        //Class cant be instatiated (static class)
        GLFWwindow* window;
        const int WIDHT = 800;
        const int HEIGHT = 600;
        const std::string TITLE = "my title";
};
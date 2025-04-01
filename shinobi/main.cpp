#include <GLFW/glfw3.h>
#include "gui/Toolbar.h"
#include "utils/Notification.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "gui/CameraWindow.h"
#include "gui/MyWindow.h"
//#include "imgim_impl_opengl3.h"

int main() 
{
    if (!glfwInit()) return 1;

    GLFWwindow* window = glfwCreateWindow(1280, 720, "My App", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // Enable docking (MUST be done before first ImGui::NewFrame())
    // ImGuiIO& io = ImGui::GetIO(); 
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // <-- Add this line
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
     
        
    MyWindow app;
    app.initialize();


    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        app.render();
        Notification::Render();

        // Rendering
        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }


    // while (!glfwWindowShouldClose(window)) {
    //     glfwPollEvents();

    //     ImGui_ImplOpenGL3_NewFrame();
    //     ImGui_ImplGlfw_NewFrame();
    //     ImGui::NewFrame();

    //     toolbar.Draw();
    //     optionsScreen.Draw(&showOptions);

    //     ImGui::Render();

       

    //     int display_w, display_h;
    //     glfwGetFramebufferSize(window, &display_w, &display_h);
    //     glViewport(0, 0, display_w, display_h);
    //     glClear(GL_COLOR_BUFFER_BIT);
    //     ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    //     glfwSwapBuffers(window);
    
    // }

    // CleanUp
    app.shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
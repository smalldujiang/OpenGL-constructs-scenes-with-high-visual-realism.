#include <iostream>
#include "glframework/core.h"

#include "wrapper/checkError.h"
#include "application/Application.h"

#include "application/camera/perspectiveCamera.h"
#include "application/camera/trackBallCameraControl.h"
#include "application/assimpLoader.h"

#include "glframework/geometry.h"
#include "glframework/framebuffer/framebuffer.h"
#include "glframework/material/PhongMaterial.h"
#include "glframework/material/WhiteMaterial.h"
#include "glframework/material/DepthMaterial.h"
#include "glframework/material/ScreenMaterial.h"

#include "glframework/mesh.h"
#include "glframework/renderer/renderer.h"
#include "glframework/light/pointLight.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glframework/scene.h"

Framebuffer* framebuffer = nullptr;

Texture* colorAttachment = nullptr;

int WIDTH = 1600;
int HEIGHT = 1200;

Geometry* geometry = nullptr;
Geometry* geometry1 = nullptr;
PerpectiveCamera* camera = nullptr;
TrackBallCameraControl* cameraControl = nullptr;
Renderer* renderer = nullptr;
Scene* scene = nullptr;
Scene* scene2 = nullptr;

AmbientLight* ambLight = nullptr;
SpotLight* spotLight = nullptr;
DirectionLight* dirLight = nullptr;
std::vector<PointLight*> pointLights{};

glm::vec3 clearColor{};

void OnResize(int width, int height)
{
    GL_CALL(glViewport(0, 0, width, height));
}

void OnKey(int key, int action, int mods)
{
    std::cout << key << std::endl;
    cameraControl->onKey(key, action, mods);
}

void OnMouse(int button, int action, int mods)
{
    /*std::cout << "鼠标点击" << button<< std::endl;*/
    double x, y;
    glApp->getCursorPosition(&x, &y);
    cameraControl->onMouse(button, action, x, y);

}

void OnCursor(double xpos, double ypos)
{
    cameraControl->onCursor(xpos, ypos);
}

void lightTransform()
{
    float xPos = glm::sin(glfwGetTime()) + 8.0f;
    //meshWhite->setPosition(glm::vec3(xPos, 0.0f, 0.0f));
    //pointLight->setPosition(glm::vec3(xPos, 0.0f, 0.0f));
}

void Camerapreparation()
{
    camera = new PerpectiveCamera(
        60.0f,
        (float)glApp->getWidth() / (float)glApp->getHeight(),
        0.01f,
        100.0f,
        glm::vec3(0.0f, 15.0f, 60.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f)
    );

    cameraControl = new TrackBallCameraControl;
    cameraControl->setCamera(camera);

}

void prepare()  
{
    renderer = new Renderer();
    scene = new Scene();

    framebuffer = new Framebuffer(WIDTH, HEIGHT);

    auto geometry = Geometry::createSphere(10.0f);

    auto materialA = new PhongMaterial();
    materialA->mDiffuse = new Texture("assets/photos/floor.jpg", 0);
    auto meshA = new Mesh(geometry, materialA);
    meshA->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    scene->addChild(meshA);

    dirLight = new DirectionLight();
    dirLight->mDirection = glm::vec3(-1.0f, -1.0f, -1.0f);
    dirLight->mColor = glm::vec3(0.0f, 0.0f, 0.0f);
    
    auto pointLight1 = new PointLight();
    pointLight1->setPosition(glm::vec3(0.0f, 0.0f, 40.0f));
    pointLight1->mColor = glm::vec3(1.0f, 1.0f, 1.0f);
    pointLight1->mK2 = 0.032f;
    pointLight1->mK1 = 0.09f;
    pointLight1->mKc = 1.0f;
    pointLights.push_back(pointLight1);

    ambLight = new AmbientLight();
    ambLight->mColor = glm::vec3(0.1f);
}

void initIMGUI() 
{
    ImGui::CreateContext();   //创建imgui上下文
    ImGui::StyleColorsDark();

    //设置ImGui与GLFW和OpenGL的绑定
    ImGui_ImplGlfw_InitForOpenGL(glApp->getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void renderIMGUI()
{
    //1.开启当前的IMGUI渲染
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //2.决定当前的GUI上面有哪些控件，从上到下
    ImGui::Begin("setting");
    ImGui::Text("Hello,world");
    ImGui::Button("Test Button", ImVec2(40, 20));
    ImGui::ColorEdit3("clear color", (float*)&clearColor);

    ImGui::End();

    //3.执行UI渲染
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(glApp->getWindow(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

int main()
{

    if (!glApp->init(WIDTH, HEIGHT))
    {
        return -1;
    }
   
    glApp->setResizeCallback(OnResize);
    glApp->setKeyBoardCallback(OnKey);
    glApp->setMouseCallback(OnMouse);
    glApp->setCursorCallback(OnCursor);

    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    Camerapreparation();
    prepare();
    initIMGUI();

    while (glApp->update())
    {
        cameraControl->update();
        renderer->setClearColor(clearColor);

        renderer->render(scene, camera, dirLight, pointLights, ambLight);
        renderIMGUI();
    }

    glApp->destroy();

}


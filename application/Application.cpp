#include "Application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Application* Application::mInstance = nullptr;

Application* Application::getInstance()
{
	if (mInstance == nullptr) 
	{
		mInstance = new Application();
	}

	return mInstance;

}

Application::Application()
{

}

Application::~Application()
{

}

bool Application::init(const int& width , const int& height )
{
    mWidth = width;
    mHeight = height;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", NULL, NULL);
    if (mWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(mWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glfwSetFramebufferSizeCallback(mWindow, frameBufferSizeCallback);

    //this就是当前全局唯一的Application对象
    glfwSetWindowUserPointer(mWindow, this);
    glfwSetKeyCallback(mWindow, keyCallback);
    glfwSetMouseButtonCallback(mWindow, mouseCallback);
    glfwSetCursorPosCallback(mWindow, cursorCallback);

    return true;
}

bool Application::update()
{
    if (glfwWindowShouldClose(mWindow))
    {
        return false;
    }

    glfwPollEvents();

    glfwSwapBuffers(mWindow);

    return true;
}

void Application::destroy()
{
    glfwTerminate();
}

void Application::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    Application* self = (Application*)glfwGetWindowUserPointer(window);
    if (self->mResizeCallback != nullptr)
    {
        self->mResizeCallback(width, height);
    }
}

void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Application* self = (Application*)glfwGetWindowUserPointer(window);
    if (self->mKeyBoardCallback != nullptr)
    {
        self->mKeyBoardCallback(key, action, mods);
    }
}

void Application::mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    Application* self = (Application*)glfwGetWindowUserPointer(window);
    if (self->mMouseCallback != nullptr)
    {
        self->mMouseCallback(button, action, mods);
    }
}
void Application::cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    Application* self = (Application*)glfwGetWindowUserPointer(window);
    if (self->mCursorCallback != nullptr)
    {
        self->mCursorCallback(xpos, ypos);
    }

}

void Application::getCursorPosition(double* x, double* y)
{
    glfwGetCursorPos(mWindow, x, y);
}

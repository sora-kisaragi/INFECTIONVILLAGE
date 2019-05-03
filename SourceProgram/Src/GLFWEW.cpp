/**
* @file GLFWE.cpp
*/
#include "GLFWEW.h"
#include <iostream>

/// GLFW��GLEW�����b�v���邽�߂̖��O���.
namespace GLFWEW {

	void APIENTRY OutputGLDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, GLvoid *userParam)
	{
		std::cerr << message << "\n";
	}

	/**
	* GLFW����̃G���[�񍐂���������.
	*
	* @param error �G���[�ԍ�.
	* @param desc  �G���[�̓��e.
	*/
	void ErrorCallback(int error, const char* desc)
	{
		std::cerr << "ERROR: " << desc << std::endl;
	}


	/**
	* �V���O���g���C���X�^���X���擾����.
	*
	* @return Window�̃V���O���g���C���X�^���X.
	*/
	Window& Window::Instance()
	{
		static Window instance;
		return instance;
	}

	/**
	* �R���X�g���N�^.
	*/
	Window::Window()
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	Window::~Window()
	{
		if (isGLFWInitialized) {
			glfwTerminate();
		}
	}

	/**
	* GLFW/GLEW�̏�����.
	*
	* @param w �E�B���h�E�̕`��͈͂̕�(�s�N�Z��).
	* @param h �E�B���h�E�̕`��͈͂̍���(�s�N�Z��).
	* @param title �E�B���h�E�^�C�g��(UTF-8��0�I�[������).
	*
	* @retval true ����������.
	* @retval false ���������s.
	*/
	bool Window::Init(int w, int h, const char* title)
	{
		if (isInitialized) {
			std::cerr << "ERROR: GLFWEW�͊��ɏ���������Ă��܂�." << std::endl;
			return false;
		}
		if (!isGLFWInitialized) {
			glfwSetErrorCallback(ErrorCallback);
			if (glfwInit() != GL_TRUE) {
				return false;
			}
			isGLFWInitialized = true;
		}

		if (!window) {
			//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
			window = glfwCreateWindow(w, h, title, nullptr, nullptr);
			if (!window) {
				return false;
			}
			glfwMakeContextCurrent(window);
		}

		windowSizeX = w;
		windowSizeY = h;


		if (glewInit() != GLEW_OK) {
			std::cerr << "ERROR: GLEW�̏������Ɏ��s���܂���." << std::endl;
			return false;
		}

		glDebugMessageCallback(OutputGLDebugMessage, nullptr);

		const GLubyte* renderer = glGetString(GL_RENDERER);
		std::cout << "Renderer: " << renderer << std::endl;
		const GLubyte* version = glGetString(GL_VERSION);
		std::cout << "Version: " << version << std::endl;

		isInitialized = true;
		return true;
	}

	/**
	* �E�B���h�E�����ׂ������ׂ�.
	*
	* @retval true ����.
	* @retval false ���Ȃ�.
	*/
	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(window) != 0;
	}

	/**
	* �t�����g�o�b�t�@�ƃo�b�N�o�b�t�@��؂�ւ���.
	*/
	void Window::SwapBuffers() const
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	/**
	* �L�[��������Ă��邩���ׂ�.
	*
	* @param key ���ׂ�L�[��ID(GLFW_KEY_A�Ȃ�).
	*
	* @retval true  �L�[��������Ă���.
	* @retval false �L�[��������Ă��Ȃ�.
	*/
	bool Window::IsKeyPressed(int key) const
	{
		if (key < 0 || key >= GLFW_KEY_LAST + 1) {
			return false;
		}
		return keyState[key] != KeyState::release;
	}

	/**
	* �L�[�������ꂽ�u�Ԃ����ׂ�.
	*
	* @param key ���ׂ�L�[��ID(GLFW_KEY_A�Ȃ�).
	*
	* @retval true  �L�[�������ꂽ�u��.
	* @retval false �L�[�������ꂽ�u�Ԃł͂Ȃ�.
	*/
	bool Window::IsKeyDown(int key) const
	{
		if (key < 0 || key >= GLFW_KEY_LAST + 1) {
			return false;
		}
		return keyState[key] == KeyState::press1st;
	}

	/**
	* �^�C�}�[������������.
	*
	* @sa UpdateTimer, GetDeltaTime
	*/
	void Window::InitTimer()
	{
		glfwSetTime(0.0);
		previousTime = 0.0;
		deltaTime = 0.0;
	}

	/**
	* �^�C�}�[���X�V����.
	*
	* @sa InitTimer, GetDeltaTime
	*/
	void Window::UpdateTimer()
	{
		// �o�ߎ��Ԃ��v��.
		const double currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		// �o�ߎ��Ԃ������Ȃ肷���Ȃ��悤�ɒ���.
		const float upperLimit = 0.25f; // �o�ߎ��ԂƂ��ċ��e�������.
		if (deltaTime > upperLimit) {
			deltaTime = 0.1f;
		}
	}

	/**
	* �o�ߎ��Ԃ��擾����.
	*
	* @return ���O��2���UpdateTimer()�Ăяo���̊ԂɌo�߂�������.
	*
	* @sa InitTimer, UpdateTimer
	*/
	double Window::DeltaTime() const
	{
		return deltaTime;
	}

	/**
	* ��Ԃ��X�V����.
	*/
	void Window::Update()
	{
		UpdateTimer();
		for (size_t i = 0; i < GLFW_KEY_LAST + 1; ++i) {
			const bool pressed = glfwGetKey(window, i) == GLFW_PRESS;
			if (pressed) {
				if (keyState[i] == KeyState::release) {
					keyState[i] = KeyState::press1st;
				}
				else if (keyState[i] == KeyState::press1st) {
					keyState[i] = KeyState::press;
				}
			}
			else if (keyState[i] != KeyState::release) {
				keyState[i] = KeyState::release;
			}
		}

		//�E�B���h�E�T�C�Y�ύX
		int width;
		int height;
		//���݂̃E�B���h�E�T�C�Y�擾
		glfwGetWindowSize(window, &width, &height);
		//�T�C�Y���������
		if ((windowSizeX != width) || (windowSizeY != height))
		{
			//�ύX��������
			glViewport(0, 0, width, height);
			windowSizeX = width;
			windowSizeY = height;

		}

	}

	/**
	*
	*/
	glm::vec2 Window::GetMousePosition() const
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return glm::vec2(x, y);
	}



	/**
	*
	*/
	int Window::GetMouseButton(int button) const
	{
		return glfwGetMouseButton(window, button);
	}

} // namespace GLFWEW
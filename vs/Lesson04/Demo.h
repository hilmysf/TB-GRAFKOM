#pragma once
#include "RenderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SOIL/SOIL.h>

class Demo :
	public RenderEngine
{
public:
	Demo();
	~Demo();
private:
	GLuint shaderProgram, VBOkasur1, VAOkasur1, EBOkasur1, textureKasur1, VBOlantai, VAOlantai, EBOlantai, textureLantai, VBOkasur2, VAOkasur2, EBOkasur2, textureKasur2, VBOlemariKecil, VAOlemariKecil, EBOlemariKecil, textureLemariKecil, VBOtembok1, VAOtembok1, EBOtembok1, textureTembok, VBOtembok2, VAOtembok2, EBOtembok2, VBOLemariBesar, VAOLemariBesar, EBOLemariBesar, textureLemariBesar, VBOKaca, VAOKaca, EBOKaca, textureKaca, VBOPintu, VAOPintu, EBOPintu, texturepintu;
	GLuint shadowmapShader, cubeVBO, cubeVAO, cubeEBO, cube_texture, planeVBO, planeVAO, planeEBO, plane_texture, stexture, stexture2;
	GLuint activeProgram;
	float viewCamX, viewCamY, viewCamZ, upCamX, upCamY, upCamZ, posCamX, posCamY, posCamZ, CAMERA_SPEED, fovy;
	float angle = 0;
	virtual void Init();
	virtual void DeInit();
	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void ProcessInput(GLFWwindow *window);
	void BuildPintu();
	void BuildKasur1();
	void BuildKasur2();
	void BuildLemariKecil();
	void BuildLemariBesar();
	void BuildPintu2();

	void BuildPlaneLantai();
	void BuildPlaneTembok1();
	void BuildPlaneTembok2();

	void DrawPintu();
	void DrawKasur1();
	void DrawKasur2();
	void DrawLemariKecil();
	void DrawLemariBesar();
	void DrawPintu2();

	void DrawPlaneLantai();
	void DrawPlaneTembok1();
	void DrawPlaneTembok2();
	void DrawPlaneTembok3();
	void DrawPlaneTembok4();

	void MoveCamera(float speed);
	void StrafeCamera(float speed);
	void RotateCamera(float speed);
	void InitCamera();
};


#pragma once
#include <string.h>

#include "ScatteringScene.h"
#include "Particle.h"

class MyGLWidget
{
public:
	MyGLWidget(const std::string &filename);
	~MyGLWidget();

	ScatteringScene *scene;

public:
	void display();
	void initializeGL();
	void resize(int w, int h);
	void draw();

private:
	Particle *m_particle;
};

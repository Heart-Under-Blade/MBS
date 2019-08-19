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
	void rotate(int button, int state, int x, int y);
	void motion(int x, int y);

private:
	Particle *m_particle;

	double m_theta;
	double m_phi;
	double m_depth;
	int m_downX;
	int m_downY;

	bool isLmb;
	bool isMmb;
};

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "Linear_Algebra.h"
#include "RayCastRender.h"
#include "Camera.h"
#include "Object.h"
#include "Light.h"
#include "Sphere.h"
#include "AmbientLight.h"
#include "PointLight.h"
#include "Cylinder.h"

/* ================================== INITIALIZE ================================== */

namespace RenderAPI
{
	typedef GLuint VertexBuffer;

	static bool renderEnable = true;
	//pixel buffer
	VertexBuffer vbo;
	//list of objects
	std::vector<Object> objs;
	//list of lights
	std::vector<Light *> lights;

	//screen size
	int width = 512;
	int height = 512;

	//main camera
	Camera cam = Camera(
		Vector4Df{ 0.0f, 0.0f, 10.0f, 1.0f },//position
		Vector4Df{ 0.0f, 0.0f, 0.0f, 1.0f },//lookat
		Vector4Df{ 0.0f, 1.0f, 0.0f, 0.0f },//up
		float2{ width*1.0f, height*1.0f }, //resolution
		float2{ 4.0f, 4.0f },			//dimensions
		4.0f							//near
	);

	//raycast class for renderization 
	RayCast rc(cam);

	void init(void)
	{
		lights.push_back(new AmbientLight(Vector4Df{ 10.0f, 10.0f, 10.0f,0 }));
		//lights.push_back(new PointLight(Vector4Df{ 1.0f, 1.0f, 1.0f, 0 }, cam.eye));
	}

	/* ================================== VERTEX BUFFER ================================== */

	inline void CreateVBO(VertexBuffer *_vbo, int width, int height)
	{
		if (!_vbo)return;
		//Create vertex buffer object
		glGenBuffers(1, _vbo);
		glBindBuffer(GL_ARRAY_BUFFER, *_vbo);

		//Initialize VBO
		unsigned int size = width * height * sizeof(float) * 5;
		glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	inline void DeleteVBO(VertexBuffer* _vbo)
	{
		glDeleteBuffers(1, _vbo);
	}

	inline void BufferBind(const VertexBuffer &vbo) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
	}

	inline void MapBuffer(Vector4Df *color_buffer, int width, int height)
	{
		//RayPathRender(triangles, cam, accumulatebuffer);

		float *buffer = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (!buffer)return;
		Vector4Df *c_buff = color_buffer;

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++, c_buff++) {
				buffer[(y*width + x) * 5] = (float)x;
				buffer[(y*width + x) * 5 + 1] = (float)y;
				memcpy(&buffer[(y*width + x) * 5 + 2], c_buff, sizeof(float) * 3);
			}
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);

		glVertexPointer(2, GL_FLOAT, 20, 0);
		glColorPointer(4, GL_FLOAT, 20, (GLvoid*)8);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glDrawArrays(GL_POINTS, 0, width * height);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	/* ================================== OPENGL API ================================== */

	// display function called by MainLoop(), gets executed every frame 
	void disp(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Render using RayCast
		BufferBind(vbo);
		if (renderEnable == true)
		{
			Vector4Df* colorBuffer = rc.Render(cam, objs, lights);
			MapBuffer(colorBuffer, width, height);
			renderEnable = false;

			glutSwapBuffers();
		}
		glutPostRedisplay();
	}
	
	// resize
	void reshape(int w, int h)
	{
		CreateVBO(&vbo, width, height);
	}

	// Main.
	void Start_Window(int argc, char **argv)
	{
		// Create GLUT window
		// init glut:
		glutInit(&argc, argv);
		// specify the display mode to be RGB and single buffering:
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
		// specify the initial window position:
		glutInitWindowPosition(0, 0);
		// specify the initial window size:
		glutInitWindowSize(width, height);
		// create the window and set title:
		glutCreateWindow("Render");

		//glEnable(GL_DEPTH_TEST);
		//glDisable(GL_CULL_FACE);

		glClearColor(0.0, 1.0, 0.0, 0.0);
		glMatrixMode(GL_PROJECTION);
		gluOrtho2D(0.0, width, 0.0, height);

		glewInit();
		if (!glewIsSupported("GL_VERSION_2_0 ")) {
			exit(0);
		}

		init();

		glutDisplayFunc(disp);

		//create the pixel buffer
		CreateVBO(&vbo, width, height);

		//start render loop
		//glutMainLoop();

		//delete the pixel buffer
		//DeleteVBO(&vbo);
	}

} //namespace RenderAPI


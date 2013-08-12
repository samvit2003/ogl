#include "framework.hpp"

//Globals for the app
GLuint g_program = 0;
GLuint g_positionBufferObject = 0;
GLuint g_vao;

void reshape (int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}


void keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
		case 27: //Escape
			glutLeaveMainLoop();
			return;
	}
}

void init(const float* vertex_pos, size_t nelems) {

	//Get Shaders...
	std::vector<GLuint> shader_list;
	shader_list.push_back(simple_ogl::create_shader(GL_VERTEX_SHADER,   "first.vs"));
	shader_list.push_back(simple_ogl::create_shader(GL_FRAGMENT_SHADER, "first.ps"));

	//Initialize Program
	g_program = simple_ogl::create_program(shader_list);

	//Initialize Vertex Buffer...
	glGenBuffers(1, &g_positionBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, g_positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, nelems, vertex_pos, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1,&g_vao);
	glBindVertexArray(g_vao);
}

void display() {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(g_program);
	glBindBuffer(GL_ARRAY_BUFFER, g_positionBufferObject);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0,4,GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1,4,GL_FLOAT, GL_FALSE, 0, (void*)48);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glUseProgram(0);

	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char** argv)
{

	std::string wname{"OGL is FUN"};
	int width        = 540;
	int height       = 540;
	int x            = 300;
	int y            = 200;
	int display_mode = GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL;
	int major        = 3;
	int minor        = 3;
	int profile      = GLUT_CORE_PROFILE;


	simple_ogl::create_window(argc, argv, wname, width, height, x, y, display_mode, major, minor, profile);

	const float vertexPositions[] = {
		0.75f, 0.75f, 0.0f, 1.0f,
		0.75f, -0.75f, 0.0f, 1.0f,
		-0.75f, -0.75f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};


	init(vertexPositions, sizeof(vertexPositions)); //FUNC 
	glutDisplayFunc(display); //FUNC 
	glutReshapeFunc(reshape); //FUNC
	glutKeyboardFunc(keyboard); //FUNC
	glutMainLoop();

	return 0;
}


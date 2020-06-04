#include <math.h>
#include <vector>
#include <GL/glut.h>
#include <bevgrafmath2017.h>

constexpr int winWidth = 800, winHeight = 600;
GLint dragged = -1;
std::vector<vec2> points;
bool canDrawCurve = false;

void init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//kontrollpoligon
	glLineWidth(1.0);
	glColor3f(0.8, 0.8, 0.8);
	glBegin(GL_LINE_STRIP);
	glLineWidth(0.0);
	for (int i = 0; i < points.size(); i++)
	{
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();

	//kontrollpontok
	glPointSize(5.0);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < points.size(); i++)
	{
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();

	glPointSize(2.0);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < points.size(); i++)
	{
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();
	
	if (canDrawCurve)
	{
		//görbe
		const int n = points.size();
		mat24 G;
		vec2 p_1 = 2 * points[0] - points[1];
		vec2 p_n = 2 * points[n - 1] - points[n - 2];
		mat4 M = {	
				{ -1.0f ,  3.0f , -3.0f , 1.0f },
				{  3.0f , -6.0f ,  0.0f , 4.0f },
				{ -3.0f ,  3.0f ,  3.0f , 1.0f },
				{  1.0f ,  0.0f ,  0.0f , 0.0f } 
		};
		M = (1.0f / 6.0f) * M;

		//elsõ 4 pont görbe
		glLineWidth(2.0);
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINE_STRIP);
		G = { p_1, points[0], points[1], points[2]};
		for (float t = 0.0; t < 1.0; t += 0.01)
		{
			vec4 T = { t * t * t, t * t, t, 1 };
			vec2 Q = G * M * T;
			glVertex2f(Q.x, Q.y);
		}
		glEnd();

		//elsõ joint pont
		glPointSize(5.0);
		glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_POINTS);
		G = { p_1, points[0], points[1], points[2]};
		vec4 T = { 1, 1, 1, 1 };
		vec2 Q = G * M * T;
		glVertex2f(Q.x, Q.y);
		glEnd();

		//utolsó 4 pont görbe
		glLineWidth(2.0);
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINE_STRIP);
		G = { points[n - 3], points[n - 2], points[n - 1], p_n};
		for (float t = 0.0; t < 1.0; t += 0.01)
		{
			vec4 T = { t * t * t, t * t, t, 1 };
			vec2 Q = G * M * T;
			glVertex2f(Q.x, Q.y);
		}
		glEnd();

		//közbensõ pontokra esõ görbe
		for (int i = 0; i < n - 3; i++)
		{
			glLineWidth(2.0);
			glColor3f(0.0, 0.0, 0.0);
			glBegin(GL_LINE_STRIP);
			G = { points[i] , points[i + 1], points[i + 2] , points[i + 3] };
			for (float t = 0.0; t < 1.0; t += 0.01)
			{
				vec4 T = { t * t * t, t * t, t, 1 };
				vec2 Q = G * M * T;
				glVertex2f(Q.x, Q.y);
			}
			glEnd();

			//joint pontok
			glPointSize(5.0);
			glColor3f(0.0, 0.0, 1.0);
			glBegin(GL_POINTS);
			vec4 T = { 1, 1, 1, 1 };
			vec2 Q = G * M * T;
			glVertex2f(Q.x, Q.y);
			glEnd();
		}
	}
	glutSwapBuffers();
}

GLint getActivePoint1(const std::vector<vec2> p, const GLint size, const GLint sens, const GLint x, const GLint y)
{
	GLint s = sens * sens;
	vec2 P = { (float)x, (float)y };
	for (int i = 0; i < size; i++) 
	{
		if (dist2(p[i], P) < s) 
		{
			return i;
		}
	}
	return -1;
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	GLint i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
	{	
		if ((i = getActivePoint1(points, points.size(), 10, xMouse, winHeight - yMouse)) != -1) 
		{
			dragged = i;
		} 
		else 
		{
			points.push_back(vec2(xMouse, winHeight - yMouse));
		}
	}
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN && points.size() > 3 )
	{
		canDrawCurve = true;
	}
	
	//drag & drop 
	//TODO = törlés
	if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN && points.size() > 4) 
	{
		if ((i = getActivePoint1(points, points.size(), 10, xMouse, winHeight - yMouse)) != -1) 
		{
			points.erase(points.begin() + i);
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN && points.size() <= 4)
	{
		if ((i = getActivePoint1(points, points.size(), 10, xMouse, winHeight - yMouse)) != -1) 
		{
			points.erase(points.begin() + i);
			canDrawCurve = false;
		}
	}
	
	if (button == GLUT_LEFT_BUTTON && action == GLUT_UP) 
	{
		dragged = -1;
	}
		
	
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
	GLint i;
	if (dragged >= 0) 
	{
		points[dragged].x = xMouse;
		points[dragged].y = winHeight - yMouse;
		glutPostRedisplay();
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Bspline - Abai Alex");
	init();
	glutDisplayFunc(display);
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutMainLoop();
	return 0;
}
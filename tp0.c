#include <GL/glew.h>
#include<stdio.h>
#include <GL/freeglut.h>
int x=0, y=0, d=0;

void square(int k, int l){

	glBegin(GL_POLYGON);
        glVertex3f(k, l, 0);
        glVertex3f(k+10, l, 0);
        glVertex3f(k+10, l+10, 0);
        glVertex3f(k, l+10, 0);
        glEnd();
}



void desenhaCena(){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 1);
	square(x,y);
	glutSwapBuffers();
}



// Inicia algumas variáveis de estado
void inicializa(void)
{
    // cor para limpar a tela
    glClearColor(1, 1, 1, 0);      // branco
}

// Callback de redimensionamento
void redimensiona(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, 100, 0, 100, -1, 1);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Callback de evento de teclado
void teclado(unsigned char key)
{
   switch(key)
   {
      // Tecla ESC
      case 27:
         exit(0);
         break;
      case 'w':
	y++;
	glutPostRedisplay();
	break;
      case 's':
	y--;
	glutPostRedisplay();
	break;
      case 'd':
	while(d<10){
	x++;
	glutPostRedisplay();
	d++;
	}
	d=0;
	break;
      case 'a':
	x--;
	glutPostRedisplay();
	break;
      default:
         break;
   }
}


// Rotina principal
int main(int argc, char **argv)
{
    // Acordando o GLUT
    glutInit(&argc, argv);

    // Definindo a versão do OpenGL que vamos usar
    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    // Configuração inicial da janela do GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);

    // Abre a janela
    glutCreateWindow("Bruno Henrique Amaral Fonseca");

    // Registra callbacks para alguns eventos
    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    inicializa();

    // Entra em loop e nunca sai
    glutMainLoop();
    return 0;
}

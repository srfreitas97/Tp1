#include <GL/glew.h>
#include "characterMoves.h"
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

GLfloat orientacaoEmGraus = 0;
GLfloat velocidadeAngular = 0.05;
GLfloat x = 0, y = 0;
float velocidadeTangencial = 0.5;
int trueMouseX, trueMouseY;
#define LARGURA_DO_MUNDO 300
#define ALTURA_DO_MUNDO 230
int globalWidth=LARGURA_DO_MUNDO, globalHeight= ALTURA_DO_MUNDO;
#define radianoParaGraus(radianos) (radianos * (180.0 / M_PI))
#define grausParaRadianos(graus) ((graus * M_PI) / 180.0)

float axisx=0, axisy=0;
int up=0,down=0,left=0,right=0, r,enemieX,enemieY,enemieCount=0,level=5;
struct ponto {
   int x, y;
};
struct Enemie{
	int x;
	int y;
};
struct Enemie enemieVector[100];
struct ponto posicaoMouse;
void randomPosition(int* x,int* y){
	r = random()%90;
	*x = r;
	r = random()%90;
	*y = r;
}

void movimentoMouse(int x, int y) {     //callback do mouse
    posicaoMouse.x = x;
    posicaoMouse.y = y;
}
void createEnemie(){
	r = random()%globalHeight;
	enemieVector[enemieCount].x = r;
	r = random()%globalWidth;
	enemieVector[enemieCount].y = r;
	enemieCount++;
}
void drawEnemie(struct Enemie enemie){
	glColor3f(0, 1, 0);
	glBegin(GL_POLYGON);
        	glVertex3f(enemie.x,enemie.y, 0);
					glColor3f(1,0,1);
        	glVertex3f(enemie.x+30,enemie.y, 0);
        	glVertex3f(enemie.x,enemie.y+30, 0);
        	glVertex3f(enemie.x+30,enemie.y+30, 0);
  glEnd();
}


void desenhaCena(void){
    glClear(GL_COLOR_BUFFER_BIT);
    // Começa a usar a cor azul
    corrigeMouse(posicaoMouse.x,posicaoMouse.y,&trueMouseX,&trueMouseY,globalHeight);
    glColor3f(.5, .5, 1);
	  movimenta(up,down,left,right,&axisx,&axisy);
    glPushMatrix();
        // Move o sistema de coordenadas para a posição onde deseja-se desenhar
        glTranslatef(x, y, 0);
        // Rotaciona o sistema de coordenadas para o ângulo de orientação,
        // no eixo Z (como estamos em 2D, só faz sentido rotacionar em 'z')
        // O ângulo esperado pela glRotate deve estar em graus
        // Os argumentos "0, 0, 1" indicam que a rotação é no eixo Z
        glRotatef(atan2(trueMouseY-axisy,trueMouseX-axisx)*180/M_PI, 0, 0, 1);

        // Desenha o personagem principal
        characterShape();
    glPopMatrix();
    if(enemieCount < level){
        randomPosition(&enemieX,&enemieY);
        createEnemie();
    }
    for(int i=0;i<enemieCount;i++){
      drawEnemie(enemieVector[i]);
    }

    // Diz ao OpenGL para colocar o que desenhamos na tela
    glutSwapBuffers();
}

// Inicia algumas variáveis de estado
void inicializa(void){
    // cor para limpar a tela
    glClearColor(0, 0, 0, 0);      // preto

    // imprime instruções
    printf("Instrucoes:\n");
    printf("\t+: gira no sentido horario\n");
    printf("\t-: gira no sentido anti-horario\n\n");
}

// Callback de redimensionamento
void redimensiona(int w, int h){

  globalWidth = w;
  globalHeight = h;
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, 0, h, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

/*void redimensiona(int width, int height) {            //COM ASPECTRATIO IGUAL DO EXEMPLO

    globalWidth = width;
    globalHeight = height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,LARGURA_DO_MUNDO, 0, ALTURA_DO_MUNDO, -1, 1);

    float razaoAspectoJanela = ((float)width)/height;
    float razaoAspectoMundo = ((float)LARGURA_DO_MUNDO)/ ALTURA_DO_MUNDO;
    // se a janela está menos larga do que o mundo (16:9)...
    if (razaoAspectoJanela < razaoAspectoMundo) {
        // vamos colocar barras verticais (acima e abaixo)
        float hViewport = width / razaoAspectoMundo;
        float yViewport = (height - hViewport)/2;
        glViewport(0, yViewport, width, hViewport);
    }
    // se a janela está mais larga (achatada) do que o mundo (16:9)...
    else if (razaoAspectoJanela > razaoAspectoMundo) {
        // vamos colocar barras horizontais (esquerda e direita)
        float wViewport = ((float)height) * razaoAspectoMundo;
        float xViewport = (width - wViewport)/2;
        glViewport(xViewport, 0, wViewport, height);
    } else {
        glViewport(0, 0, width, height);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}*/

void atualiza(int idx) {
    // O ângulo esperado pelas funções "cos" e "sin" da math.h devem
    // estar em radianos
    GLfloat orientacaoEmRadianos = grausParaRadianos(orientacaoEmGraus);
    x = axisx+ cos(orientacaoEmRadianos);
    y = axisy+ sin(orientacaoEmRadianos);


/*    if(posicaoMouse.x<axisx || posicaoMouse.y>axisy){
      velocidadeAngular-=0.5;
    }
    if(posicaoMouse.x>axisx || posicaoMouse.y<axisy){
      velocidadeAngular+=0.5;
    }*/

    orientacaoEmGraus += velocidadeAngular;

    glutPostRedisplay();
    glutTimerFunc(17, atualiza, 0);
}

// Callback de evento de teclado
void teclado(unsigned char key, int x, int y)
{
    switch(key)
    {
        // Tecla ESC
        case 27:
            exit(0);
            break;
  			case 'w':
  				resetaMovimento(&up,&down,&left,&right);
  				up=1;
  				break;
  			case 's':
  				resetaMovimento(&up,&down,&left,&right);
  				down=1;
  				break;
  			case 'd':
  				resetaMovimento(&up,&down,&left,&right);
  				right=1;
  				break;
  			case 'a':
  				resetaMovimento(&up,&down,&left,&right);
  				left=1;
  				break;
  			case '+':
            velocidadeTangencial+=5;
            break;
        case '=':
            velocidadeTangencial += 0.5;
            break;
        case '-':
              velocidadeTangencial -= 5;
              break;
        case '_':
            velocidadeAngular -= 0.5;
            break;
        default:
            break;
    }
}

// Rotina principal
int main(int argc, char **argv)
{
    // Configuração inicial da janela do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(LARGURA_DO_MUNDO, ALTURA_DO_MUNDO);
    glutInitWindowPosition(100, 100);

    // Abre a janela
    glutCreateWindow("Triângulo em movimento e orientado");

    // Registra callbacks para alguns eventos
    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    glutPassiveMotionFunc(movimentoMouse);
    glutTimerFunc(0, atualiza, 0);
    inicializa();

    // Entra em loop e nunca sai
    glutMainLoop();
    return 0;
}

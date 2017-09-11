#include <GL/glew.h>
#include "characterMoves.h"
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <SOIL/SOIL.h>

GLuint texturaBackground;
GLuint texturaCharizard;
GLfloat orientacaoEmGraus = 0;
GLfloat velocidadeAngular = 0.05;
GLfloat x = 0, y = 0;
float velocidadeTangencial = 0.5;
int trueMouseX, trueMouseY;
#define LARGURA_DO_MUNDO 800
#define ALTURA_DO_MUNDO 800
int colidiu =0;
int enemieColidiu=0;
int globalWidth=LARGURA_DO_MUNDO, globalHeight= ALTURA_DO_MUNDO;
#define radianoParaGraus(radianos) (radianos * (180.0 / M_PI))
#define grausParaRadianos(graus) ((graus * M_PI) / 180.0)
const int raioEnemy =10;
const int raioPlayer=10;


float axisx=0, axisy=0;
int up=0,down=0,left=0,right=0,r,enemieX,enemieY,enemieCount=0,level=12;
int clicou =0,parametroJ;

void initCharizard() {
  texturaCharizard = SOIL_load_OGL_texture("charizard.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaCharizard == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}
void init() {
  texturaBackground = SOIL_load_OGL_texture("ice.jpg",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaBackground == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}
void backGround(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturaBackground);
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(0, 0); glVertex3f(0, 0,0);
		glTexCoord2f(1, 0); glVertex3f( globalWidth, 0,0);
		glTexCoord2f(1, 1); glVertex3f( globalWidth,  globalHeight,0);
		glTexCoord2f(0, 1); glVertex3f(0,  globalHeight,0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

struct ponto {
   int x, y;
};

struct Enemie{
	int x;
	int y;
};

struct Enemie enemieVector[100];
struct ponto posicaoMouse;
struct ponto cliqueMouse;
struct ponto tiro;

void clickMouse(int button, int estado, int m, int n){

  if (button==GLUT_LEFT_BUTTON){
    cliqueMouse.x=m;
    cliqueMouse.y=globalHeight-n;
    clicou=1;
  }
}

void drawShot(){
  glColor3f(1,0,0);
      glVertex2f(0,-3);
      glVertex2f(-3,0);
      glVertex2f(0,3);
      glVertex2f(3,0);
  glEnd();
}

void movimentoMouse(int x, int y) {     //callback do mouse       ELIMINAR FUNCAO CORRIGE MOUSE
    posicaoMouse.x = x;
    posicaoMouse.y = y;
}
void createEnemie(){
	r = globalWidth+(rand()%3300-2600);
	enemieVector[enemieCount].x = r;
	r = globalHeight+(rand()%3300-2600);
	enemieVector[enemieCount].y = r;
}

int colisao (int i) {
  int distancia = sqrt(((axisx/2 - enemieVector[i].x/2) * (axisx/2 - enemieVector[i].x/2)) + ((axisy/2 - enemieVector[i].y/2) * (axisy/2 - enemieVector[i].y/2)));
  if (axisx>=globalWidth){ //FAZER RETORNAR 1 PARA PODER TIRAR VIDA DO PLAYER
    axisx-=30;
  }
  if(axisy>=globalHeight){
    axisy-=30;
  }
  if (axisx<=0){
    axisx+=30;
  }
  if(axisy<=0){
    axisy+=30;
  }
  if (distancia <= raioPlayer + raioEnemy){
        if(axisy>enemieVector[i].y){
            axisy+=30;
        }
        else
          axisy-=30;

      if(axisx<enemieVector[i].x){
        axisx-=30;
    }
    else
      axisx+=30;

      return 1;
  }
  else
      return 0;
}

int colisaoEnemie (struct Enemie enemie, struct Enemie enemy) {
  int distancia = sqrt(((enemie.x/2 - enemy.x/2) * (enemie.y/2 - enemy.x/2)) + ((enemie.y/2 - enemy.y/2) * (enemie.y/2 - enemy.y/2)));
  if (distancia <= raioPlayer){
    return 1;
  }
  else
    return 0;
}

void drawEnemie(struct Enemie enemie){

/*  if(enemieColidiu==1){
    enemieColidiu=0;
          if(enemieVector[j].y>enemieVector[i].y){
              enemie.y+=50;
          }
          else
              enemie.y-=50;

        if(enemieVector[j].y<enemieVector[i].x){
              enemie.x+=50;
      }
      else
              enemie.x-=50;
  }*/
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texturaCharizard);
  glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, 0); glVertex3f(enemie.x-15, enemie.y+15,0);
    glTexCoord2f(1, 0); glVertex3f( enemie.x-15,enemie.y-15,0);
    glTexCoord2f(1, 1); glVertex3f( enemie.x+15,  enemie.y-15,0);
    glTexCoord2f(0, 1); glVertex3f(enemie.x+15,  enemie.y+15,0);
  glEnd();
  glDisable(GL_TEXTURE_2D);

}

void enemyFollows(int i){

  float vetorx = axisx - enemieVector[i].x;
  float vetory = axisy - enemieVector[i].y;
  float vetorx2 = enemieVector[parametroJ].x - enemieVector[i].x;
  float vetory2 = enemieVector[parametroJ].y - enemieVector[i].y;
  int  norma = sqrt(vetorx*vetorx + vetory*vetory);
  int  norma2 = sqrt(vetorx2*vetorx2 + vetory2*vetory2);
  vetorx /= norma;
  vetory /= norma;
  vetorx2 /= norma2;
  vetory2 /= norma2;

  enemieVector[i].x += (vetorx)*(1.89f)-(vetorx2);
  enemieVector[i].y += (vetory)*(1.89f)-(vetory2);
}

void desenhaCena(void){
    glClear(GL_COLOR_BUFFER_BIT);
    backGround();
    // Começa a usar a cor azul
    corrigeMouse(posicaoMouse.x,posicaoMouse.y,&trueMouseX,&trueMouseY,globalHeight);
	  movimenta(up,down,left,right,&axisx,&axisy);
    glPushMatrix();
        // Faz o objeto se orientar pelo mouse
        glTranslatef(x, y, 0);
        glRotatef(atan2(trueMouseY-axisy,trueMouseX-axisx)*180/M_PI, 0, 0, 1);

        // Desenha o personagem principal
        characterShape(&colidiu);
        glPopMatrix();

        if(enemieCount < level){
            randomPosition(&enemieX,&enemieY);

            createEnemie();
            enemieCount++;
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

void atualiza(int idx) {
  GLfloat orientacaoEmRadianos = grausParaRadianos(orientacaoEmGraus);
  if(clicou==1){
      printf("\n\nAs coordenadas do clique sao: x=%d, y=%d",cliqueMouse.x,cliqueMouse.y);
      clicou=0;
  }
  for(int i=0;i<enemieCount;i++){ //LOOP PARA CONFIGURAR OS INIMIGOS
    if(colisao(i)==1){
      colidiu=1;
    }
    for(parametroJ=0;parametroJ<enemieCount;parametroJ++){
      if(parametroJ!=i && colisaoEnemie(enemieVector[parametroJ],enemieVector[i])==1){
        enemieColidiu=1;
        //enemyFollows(i);
      }
    }
    enemyFollows(i);

  }
    x = axisx+ cos(orientacaoEmRadianos);
    y = axisy+ sin(orientacaoEmRadianos);

    orientacaoEmGraus += velocidadeAngular;
    glutPostRedisplay();
    glutTimerFunc(33, atualiza, 0);
}
// Callback de evento de teclado
void teclado(unsigned char key, int x, int y){
  switch(key){
    case 'W':
      key='w';
      break;
    case 'S':
      key='s';
      break;
    case 'A':
      key='a';
      break;
    case 'D':
      key='d';
      break;
  }
    switch(key){
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
int main(int argc, char **argv){
    // Configuração inicial da janela do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(LARGURA_DO_MUNDO, ALTURA_DO_MUNDO);
    glutInitWindowPosition(100, 100);

    // Abre a janela
    glutCreateWindow("KILL THE WALKERS!");
    init();
    initCharizard();
    initPikachu();

    // Registra callbacks para alguns eventos
    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    glutPassiveMotionFunc(movimentoMouse);
    glutMouseFunc(clickMouse);
    glutTimerFunc(0, atualiza, 0);
    inicializa();

    // Entra em loop e nunca sai
    glutMainLoop();
    return 0;
}

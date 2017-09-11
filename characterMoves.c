#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include "characterMoves.h"

GLuint texturaPikachu;

void initPikachu() {
  texturaPikachu = SOIL_load_OGL_texture("charizard.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

  if (texturaPikachu == 0) {
    printf("Erro carregando textura: '%s'\n", SOIL_last_result());
  }
}
void movimenta(int cima, int baixo, int esquerda, int direita, float *x, float *y){

	if(cima==1){
	*y+=8;
	}
	if(baixo==1){
	 *y-=8;
	}
	if(esquerda==1){
	 *x-=8;
	}
 if(direita==1){
	 *x+=8;
 }
}

void resetaMovimento(int *up, int *down, int *left, int *right){
	*up=0;
	*down=0;
	*left=0;
	*right=0;
}
void characterShape(int* colidou){
	if(*colidou==1){
		*colidou = 0;
	}
	else
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturaPikachu);
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(0, 0); glVertex3f(0,-20,0);
		glTexCoord2f(1, 0); glVertex3f(-20,0,0);
		glTexCoord2f(1, 1); glVertex3f(0,20,0);
		glTexCoord2f(0, 1); glVertex3f(20,0,0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	}

	void randomPosition(int* x, int* y){
		int r = (rand()%21-10);
		*x = r;
		r = (rand()%21-10);
		*y = r;
	}

void corrigeMouse(int x, int y,int* tMouseX, int* tMouseY, int gloHght){
/*trueMouseX=x*((float)LARGURA_DO_MUNDO/globalWidth);
  trueMouseY=(globalHeight-y)*((float)ALTURA_DO_MUNDO/globalHeight);*/
  *tMouseX = x;
  *tMouseY = (gloHght-y);
}

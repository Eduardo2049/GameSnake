#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*  CONSTANTES  */
#define COLUNAS 20              
#define LINHAS  20
#define TAM_BLOCO 25         
#define MAX_COBRA (COLUNAS * LINHAS)

/* ESTADO DO JOGO */
int cobraX[MAX_COBRA], cobraY[MAX_COBRA];
int tamanho = 3;                
int dirX = 1, dirY = 0;         
int pecaX, pecaY;               
int pontos = 0;
int velocidade = 150;           
int gameOver = 0;
int novaPecaX = 1, novaPecaY = 0;
int caudaX = 0, caudaY = 0;


/*  OBJETOS  */

/* desenha um bloco na posicao (x, y) do cenario */
void desenhaQuadrado(int x, int y) {
    glBegin(GL_POLYGON);
        glVertex2i(x, y);
        glVertex2i(x + 1, y);
        glVertex2i(x + 1, y + 1);
        glVertex2i(x, y + 1);
    glEnd();
}

void desenhaBordas(void) {
    glColor3f(0.0f, 0.08f, 0.25f);
    for (int i = 0; i < COLUNAS; i++) {
        desenhaQuadrado(i, 0);
        desenhaQuadrado(i, LINHAS - 1);
    }
    for (int i = 0; i < LINHAS; i++) {
        desenhaQuadrado(0, i);
        desenhaQuadrado(COLUNAS - 1, i);
    }
}

void desenhaCobra(void) {
    glColor3f(0.0f, 0.6f, 0.0f); // CORPO DA COBRA
    for (int i = 0; i < tamanho; i++) {
        desenhaQuadrado(cobraX[i], cobraY[i]);
    }

    glColor3f(0.4f, 1.0f, 0.4f); // CABECA DA COBRA
    desenhaQuadrado(cobraX[0], cobraY[0]);
}

void desenhaPeca(void) {
    glColor3f(1.0f, 0.0f, 0.0f);
    desenhaQuadrado(pecaX, pecaY);
}

void desenhaTexto(float x, float y, const char *texto) {
    glRasterPos2f(x, y);
    for (; *texto; texto++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *texto);
}

void desenhaGameOver(void) {
    char msg[40];

    glColor3f(1.0f, 0.2f, 0.2f);
    desenhaTexto(COLUNAS / 2.0f - 2.0f, LINHAS / 2.0f + 3, "GAME OVER");

    glColor3f(1.0f, 1.0f, 1.0f);
    sprintf(msg, "Pontuacao total: %d", pontos);
    desenhaTexto(COLUNAS / 2.0f - 3.0f, LINHAS / 2.0f + 1, msg);

    desenhaTexto(COLUNAS / 2.0f - 5.0f, LINHAS / 2.0f - 2, "R - Jogar novamente");
    desenhaTexto(COLUNAS / 2.0f - 3.5f, LINHAS / 2.0f - 3, "ESC - Sair");
}

/*  LOGICA  */

void inicializaCobra(void) {
    for (int i = 0; i < tamanho ; i++) {
        cobraX[i] = COLUNAS / 2 - i;
        cobraY[i] = LINHAS / 2;
    }
}

void geraPeca(void) {
    int livre;
    do {
        livre = 1;
        pecaX = 1 + rand() % (COLUNAS - 2);
        pecaY = 1 + rand() % (LINHAS - 2);

        for (int i = 0; i < tamanho; i++) {
            if (cobraX[i] == pecaX && cobraY[i] == pecaY) {
                livre = 0;
            }
        }
    } while (!livre);
}

void reiniciaJogo(void) {
    tamanho = 3;
    pontos = 0;
    velocidade = 150;
    dirX = 1;
    dirY = 0;
    novaPecaX = 1; novaPecaY = 0;
    gameOver = 0;

    inicializaCobra();   // recoloca os 3 blocos
    geraPeca();          // sorteia depois, com a cobra já no lugar
}

void moveCobra(void) {

    // guarda a posição da cauda antes de mover
    caudaX = cobraX[tamanho - 1];
    caudaY = cobraY[tamanho - 1];

    for (int i = tamanho - 1; i > 0; i--) { // move o corpo da cobra
        cobraX[i] = cobraX[i - 1];
        cobraY[i] = cobraY[i - 1];
    }

   // Cabeça anda 1 bloco 
    cobraX[0] += dirX;
    cobraY[0] += dirY;
}

int verificaColisao(void) {
    
    if (cobraX[0] <= 0 || cobraX[0] >= COLUNAS - 1 || cobraY[0] <= 0 || cobraY[0] >= LINHAS - 1) {
        return 1; // colisão com a parede
    }

    for (int i = 1; i < tamanho; i++) {
        if (cobraX[0] == cobraX[i] && cobraY[0] == cobraY[i]) {
            return 1; // colisão com o próprio corpo
        }
    }
    return 0;
}

void comePeca(void) {
    if (cobraX[0] == pecaX && cobraY[0] == pecaY) {
        cobraX[tamanho] = caudaX;
        cobraY[tamanho] = caudaY;
        tamanho++;
        pontos++;
    

    if (pontos % 5 == 0 && velocidade > 50) 
        velocidade -= 20; // aumenta a velocidade a cada 5 pontos

    geraPeca();
    }
}

/*  CALLBACKS  */

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    if (gameOver) {
        desenhaGameOver();
    } else {
        desenhaBordas();
        desenhaPeca();
        desenhaCobra();
    }

    glutSwapBuffers();
}

void timer(int valor) {
    dirX = novaPecaX;
    dirY = novaPecaY;
    moveCobra();

    if (verificaColisao()) {
        gameOver = 1;
    } else {
        comePeca();
    }

    glutPostRedisplay();
    if (!gameOver)
    glutTimerFunc(velocidade, timer, 0);
}

void teclado(int tecla, int x, int y) {

    if (tecla == GLUT_KEY_UP && dirY != -1) {
        novaPecaX = 0; novaPecaY = 1;
    } else if (tecla == GLUT_KEY_DOWN && dirY != 1) {
        novaPecaX = 0; novaPecaY = -1;
    } else if (tecla == GLUT_KEY_LEFT && dirX != 1) {
        novaPecaX = -1; novaPecaY = 0;
    } else if (tecla == GLUT_KEY_RIGHT && dirX != -1) {
        novaPecaX = 1; novaPecaY = 0;
    }
}

void tecladoLetras(unsigned char tecla, int x, int y) {

    if (gameOver) {
        if (tecla == 'r' || tecla == 'R') { // reinicia o jogo
            reiniciaJogo();
            glutTimerFunc(velocidade, timer, 0);
            glutPostRedisplay();
        }
        if (tecla == 27) { // ESC
            exit(0);    
        }
    }
    
    if (tecla == 'w' || tecla == 'W') teclado(GLUT_KEY_UP,    x, y);
    if (tecla == 's' || tecla == 'S') teclado(GLUT_KEY_DOWN,  x, y);
    if (tecla == 'a' || tecla == 'A') teclado(GLUT_KEY_LEFT,  x, y);
    if (tecla == 'd' || tecla == 'D') teclado(GLUT_KEY_RIGHT, x, y);
}

/*  PRINCIPAL  */

int main(int argc, char **argv) {
    srand(time(NULL));

    // Abrir uma janela grafica 
    glutInit(&argc, argv);
    glutInitWindowSize(COLUNAS * TAM_BLOCO, LINHAS * TAM_BLOCO);

    //Preparar o OpenGL para desenhar (double buffering, RGB) 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Snake");

    // Sistema de coordenadas e estado inicial 
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, COLUNAS, 0, LINHAS);       
    inicializaCobra();
    geraPeca();

    glutDisplayFunc(display);
    glutSpecialFunc(teclado);
    glutKeyboardFunc(tecladoLetras);
    glutTimerFunc(velocidade, timer, 0);
    glutMainLoop();

    return 0;
}
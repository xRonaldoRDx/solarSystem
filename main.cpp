#include <GL/glut.h>

float tamanhoSol = 5.0;

void configuraLuz() {
    // Luz pontual no centro (o Sol)
    GLfloat luzBranca[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat posicao[] = {0.0f, 0.0f, 0.0f, 1.0f}; // W=1.0 torna a luz pontual

    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzBranca);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao);
}

void desenhaSol(float tamanho) {
    glDisable(GL_LIGHTING); // Sol não recebe luz, ele emite
    glColor3f(1.0f, 1.0f, 0.0f); // Amarelo
    glutSolidSphere(tamanho, 30, 30);
    glEnable(GL_LIGHTING);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 10.0, 30.0, // Câmera afastada para observar o sistema
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);

    configuraLuz();
    
    desenhaSol(tamanhoSol);

    glutSwapBuffers();
}

void inicializa() {
    glClearColor(0.05, 0.05, 0.1, 1.0); // Fundo azul escuro
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    
    // Configura o material para reagir à luz
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h); // Define o tamanho da tela
    glMatrixMode(GL_PROJECTION); // Muda para o modo de projeção (lente)
    glLoadIdentity();
    
    // Define o ângulo de visão, proporção da tela e distância de corte (perto e longe)
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    
    glMatrixMode(GL_MODELVIEW); // Volta para o modo de desenho
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("Sistema Solar - Open GL");

    inicializa();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
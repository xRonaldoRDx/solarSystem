#include <GL/glut.h>

// Angulo para animaçoes
float anguloGlobal = 0.0f;

// Tamanhos
float tamanhoSol = 5.0;
float tamanhoLuaTerra = 0.3;
float tamanhoMercurio = 0.6;
float tamanhoVenus = 1.1;
float tamanhoTerra = 1.2;
float tamanhoMarte = 0.8;
float tamanhoJupiter = 3.0;
float tamanhoSaturno = 2.5;
float tamanhoUrano = 1.8;
float tamanhoNetuno = 1.7;

// Distâncias
float distanciaLua = 1.8; //Distancia da Lua a Terra
float distanciaMercurio = 8.0;
float distanciaVenus = 11.0;
float distanciaTerra = 15.0;
float distanciaMarte = 19.0;
float distanciaJupiter = 25.0;
float distanciaSaturno = 32.0;
float distanciaUrano = 38.0;
float distanciaNetuno = 44.0;

// Velocidades de Translação
float velocidadeLua = 13.0f;
float velocidadeMercurio = 4.15f;
float velocidadeVenus = 1.62f;
float velocidadeTerra = 1.0f;
float velocidadeMarte = 0.53f;
float velocidadeJupiter = 0.20f;
float velocidadeSaturno = 0.15f;
float velocidadeUrano = 0.10f;
float velocidadeNetuno = 0.05f;

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

void desenhaPlaneta(float distancia, float tamanho, float velocidade, float r, float g, float b) {
glPushMatrix();
        // Movimento de translação
        glRotatef(anguloGlobal * velocidade, 0.0f, 1.0f, 0.0f);
        glTranslatef(distancia, 0.0f, 0.0f);

        // Movimento de rotação
        glRotatef(anguloGlobal * 2.0f, 0.0f, 1.0f, 0.0f); 

        glColor3f(r, g, b);
        glutSolidSphere(tamanho, 20, 20);
    glPopMatrix();
}

void idle() {
    anguloGlobal += 0.1f; // Incrementa o ângulo continuamente
    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 10.0, 80.0, // Câmera afastada para observar o sistema
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);

    configuraLuz();
    
    desenhaSol(tamanhoSol);
    
    desenhaPlaneta(distanciaMercurio, tamanhoMercurio, velocidadeMercurio, 0.6, 0.6, 0.6); // Mercurio
    desenhaPlaneta(distanciaVenus, tamanhoVenus, velocidadeVenus, 0.9, 0.8, 0.5); // Venus
    desenhaPlaneta(distanciaTerra, tamanhoTerra, velocidadeTerra, 0.2, 0.5, 1.0); // Terra
    desenhaPlaneta(distanciaMarte, tamanhoMarte, velocidadeMarte, 0.8, 0.2, 0.1); // Marte
    desenhaPlaneta(distanciaJupiter, tamanhoJupiter, velocidadeJupiter, 0.8, 0.6, 0.4); // Jupiter
    desenhaPlaneta(distanciaSaturno, tamanhoSaturno, velocidadeSaturno, 0.9, 0.8, 0.6); // Saturno
    desenhaPlaneta(distanciaUrano, tamanhoUrano, velocidadeUrano, 0.6, 0.9, 0.9); // Urano
    desenhaPlaneta(distanciaNetuno, tamanhoNetuno, velocidadeNetuno, 0.2, 0.3, 0.8); // Netuno

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
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
    
    glMatrixMode(GL_MODELVIEW); // Volta para o modo de desenho
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // --- DETECTA A RESOLUÇÃO DO MONITOR ---
    int larguraScren = glutGet(GLUT_SCREEN_WIDTH);
    int alturaScreen = glutGet(GLUT_SCREEN_HEIGHT);

    // Define o tamanho da janela para o tamanho detectado
    glutInitWindowSize(larguraScren, alturaScreen); 
    glutCreateWindow("Sistema Solar - Open GL");

    inicializa();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
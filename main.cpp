#include <GL/glut.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLUquadric* quadric;

// Variaveis de controle da camera
bool girandoCamera = false;
int ultimoX = 0, ultimoY = 0;
float anguloCameraX = 0.0f;
float anguloCameraY = 45.0f; // Inicia inclinada acima
float distanciaCamera = 110.0f;

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

// IDs das texturas dos astros
GLuint texturaSol;
GLuint texturaMercurio;
GLuint texturaVenus;
GLuint texturaTerra;
GLuint texturaLua;
GLuint texturaMarte;
GLuint texturaJupiter;
GLuint texturaSaturno;
GLuint texturaAnelSaturno;
GLuint texturaUrano;
GLuint texturaNetuno;

GLuint carregaTextura(const char* caminho) {
    GLuint texturaID;
    glGenTextures(1, &texturaID);
    glBindTexture(GL_TEXTURE_2D, texturaID);

    // Configura o comportamento da textura (repetição e filtro de suavização)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Carrega a imagem usando stb_image
    int largura, altura, canaisCores;
    stbi_set_flip_vertically_on_load(true); // Inverte a imagem (OpenGL lê de baixo para cima)
    unsigned char* dados = stbi_load(caminho, &largura, &altura, &canaisCores, 0);

    if (dados) {
        // Verifica se a imagem é JPG (3 canais RGB) ou PNG (4 canais RGBA)
        GLenum formato = (canaisCores == 4) ? GL_RGBA : GL_RGB;
        
        // Envia os pixels para a placa de vídeo
        glTexImage2D(GL_TEXTURE_2D, 0, formato, largura, altura, 0, formato, GL_UNSIGNED_BYTE, dados);
    } else {
        printf("Falha ao carregar a textura: %s\n", caminho);
    }
    
    // Limpa a imagem da memória RAM
    stbi_image_free(dados);
    
    return texturaID;
}

void configuraLuz() {
    // Luz pontual no centro (o Sol)
    GLfloat luzBranca[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat posicao[] = {0.0f, 0.0f, 0.0f, 1.0f}; // W=1.0 torna a luz pontual

    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzBranca);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao);
}

void desenhaSol(float tamanho) {
    glDisable(GL_LIGHTING); // Sol não recebe luz, ele emite
    glColor3f(1.0f, 1.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, texturaSol);

    glPushMatrix();
        // Movimento de rotação
        glRotatef(anguloGlobal * 2.0f, 0.0f, 1.0f, 0.0f); 

        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        gluSphere(quadric, tamanho, 30, 30);
    glPopMatrix();
    
    glEnable(GL_LIGHTING);
}

void desenhaOrbita(float raio) {
    glDisable(GL_LIGHTING);   // Desliga a luz
    glDisable(GL_TEXTURE_2D); // DESLIGA A TEXTURA PARA A LINHA

    glColor3f(0.3f, 0.3f, 0.3f); // Cor da órbita (cinza escuro)
    
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++) {
        float angulo = i * 2.0f * 3.14159f / 100.0f; 
        glVertex3f(raio * cos(angulo), 0.0f, raio * sin(angulo));
    }
    glEnd();
    
    glEnable(GL_TEXTURE_2D); // RELIGA A TEXTURA PARA OS PLANETAS
    glEnable(GL_LIGHTING);   // Religa a luz
}

void desenhaPlaneta(float distancia, float tamanho, float velocidade, GLuint texturaID) {
    desenhaOrbita(distancia);
    
    glPushMatrix();
        // Movimento de translação
        glRotatef(anguloGlobal * velocidade, 0.0f, 1.0f, 0.0f);
        glTranslatef(distancia, 0.0f, 0.0f);

        // Movimento de rotação
        glRotatef(anguloGlobal * 2.0f, 0.0f, 1.0f, 0.0f); 

        glColor3f(1.0f, 1.0f, 1.0f);

        glBindTexture(GL_TEXTURE_2D, texturaID);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        gluSphere(quadric, tamanho, 30, 30);
    glPopMatrix();
}

void atualizaTemporizador(int valor) {
    anguloGlobal += 0.5f;
    glutPostRedisplay();
    
    // Chama novamente após 16 milissegundos (~60 FPS)
    glutTimerFunc(16, atualizaTemporizador, 0); 
}

void desenhaSistemaTerra(float distanciaTerra, float tamanhoTerra, float velocidadeTerra, float distanciaLua, float tamanhoLua, float velocidadeLua){
    desenhaOrbita(distanciaTerra);
    
    glPushMatrix();
        // Translação da Terra ao redor do Sol
        glRotatef(anguloGlobal * velocidadeTerra, 0.0f, 1.0f, 0.0f);
        glTranslatef(distanciaTerra, 0.0f, 0.0f);

        // --- BLOCO TERRA ---
        glPushMatrix();
            // Rotação da Terra
            glRotatef(anguloGlobal * 2.0f, 0.0f, 1.0f, 0.0f); 

            glColor3f(1.0, 1.0, 1.0);
            glBindTexture(GL_TEXTURE_2D, texturaTerra);
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
            gluSphere(quadric, tamanhoTerra, 30, 30);
        glPopMatrix();

        // --- BLOCO LUA ---
        glPushMatrix();
            
            // Inclinação orbital da Lua em relação à Terra (20 graus no eixo Z)
            glRotatef(20.0f, 0.0f, 0.0f, 1.0f); 

            // Orbita Lua
            desenhaOrbita(distanciaLua);
            
            // Movimento de translação da Lua ao redor da Terra
            glRotatef(anguloGlobal * velocidadeLua, 0.0f, 1.0f, 0.0f);
            glTranslatef(distanciaLua, 0.0f, 0.0f);

            // Rotação da Lua
            glRotatef(anguloGlobal * 2.0f, 0.0f, 1.0f, 0.0f); 

            glColor3f(1.0, 1.0, 1.0);
            glBindTexture(GL_TEXTURE_2D, texturaLua);
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
            gluSphere(quadric, tamanhoLua, 30, 30);
        glPopMatrix();
    glPopMatrix();
}

void desenhaSistemaSaturno(float distanciaSaturno, float tamanhoSaturno, float velocidadeSaturno) {
    desenhaOrbita(distanciaSaturno);

    glPushMatrix();
        // Translação de Saturno ao redor do Sol
        glRotatef(anguloGlobal * velocidadeSaturno, 0.0f, 1.0f, 0.0f);
        glTranslatef(distanciaSaturno, 0.0f, 0.0f);

        // --- BLOCO SATURNO ---
        glPushMatrix();
            // Rotação de Saturno
            glRotatef(anguloGlobal * 2.0f, 0.0f, 1.0f, 0.0f); 

            glColor3f(1.0, 1.0, 1.0);
            glBindTexture(GL_TEXTURE_2D, texturaSaturno);
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
            gluSphere(quadric, tamanhoSaturno, 30, 30);
        glPopMatrix();

        // --- BLOCO ANEIS ---
        glPushMatrix();
            // Inclinação orbital dos aneis em relação a saturno
            glRotatef(-20.0f, 0.0f, 0.0f, 1.0f); 

            // Rotação dos Anéis
            glRotatef(anguloGlobal * 2.0f, 0.0f, 1.0f, 0.0f);

            glColor3f(1.0, 1.0, 1.0);
            glBindTexture(GL_TEXTURE_2D, texturaAnelSaturno);
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
            gluDisk(quadric, tamanhoSaturno + 0.2f, tamanhoSaturno + 1.5f, 30, 1);
        glPopMatrix();
    glPopMatrix();
}

void desenhaTexto(const char* texto, int x, int y) {
    glRasterPos2i(x, y);
    for (const char* c = texto; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void desenhaInstrucoes() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    int largura = glutGet(GLUT_WINDOW_WIDTH);
    int altura = glutGet(GLUT_WINDOW_HEIGHT);
    gluOrtho2D(0, largura, 0, altura);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D); 

    glColor3f(1.0f, 1.0f, 1.0f); // Cor do texto
    desenhaTexto("CONTROLES:", 20, altura - 40);
    desenhaTexto("- Botao Direito + Arrastar: Girar Camera", 20, altura - 65);
    desenhaTexto("- Scroll: Zoom In/Out", 20, altura - 90);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glPopMatrix(); 
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); 
    glMatrixMode(GL_MODELVIEW); 
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // --- CÂMERA ---
    // Converte os ângulos de graus para radianos
    float radX = anguloCameraX * 3.14159f / 180.0f;
    float radY = anguloCameraY * 3.14159f / 180.0f;

    // Calcula a posição x, y, z usando coordenadas esféricas
    float camX = distanciaCamera * cos(radY) * sin(radX);
    float camY = distanciaCamera * sin(radY);
    float camZ = distanciaCamera * cos(radY) * cos(radX);

    // Aplica a nova câmera
    gluLookAt(camX, camY, camZ, // Posição calculada
              0.0, 0.0, 0.0,    // Ponto para onde olha (Sol)
              0.0, 1.0, 0.0);   // Vetor "Cima"

    configuraLuz();
    
    desenhaSol(tamanhoSol);
    
    desenhaPlaneta(distanciaMercurio, tamanhoMercurio, velocidadeMercurio, texturaMercurio); // Mercurio
    desenhaPlaneta(distanciaVenus, tamanhoVenus, velocidadeVenus, texturaVenus); // Venus

    desenhaSistemaTerra(distanciaTerra, tamanhoTerra, velocidadeTerra, distanciaLua, tamanhoLuaTerra, velocidadeLua); // Terra

    desenhaPlaneta(distanciaMarte, tamanhoMarte, velocidadeMarte, texturaMarte); // Marte
    desenhaPlaneta(distanciaJupiter, tamanhoJupiter, velocidadeJupiter, texturaJupiter); // Jupiter

    desenhaSistemaSaturno(distanciaSaturno, tamanhoSaturno, velocidadeSaturno); // Saturno e Anéis

    desenhaPlaneta(distanciaUrano, tamanhoUrano, velocidadeUrano, texturaUrano); // Urano
    desenhaPlaneta(distanciaNetuno, tamanhoNetuno, velocidadeNetuno, texturaNetuno); // Netuno

    desenhaInstrucoes();

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

    // Configurações para textura
    glEnable(GL_TEXTURE_2D); // Ativa o uso de texturas no OpenGL
    
    quadric = gluNewQuadric(); // Inicializa o desenhador de esferas
    gluQuadricTexture(quadric, GL_TRUE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    texturaSol = carregaTextura("textures/sun.jpg");
    texturaMercurio = carregaTextura("textures/mercury.jpg");
    texturaVenus = carregaTextura("textures/venus.jpg");
    texturaTerra = carregaTextura("textures/earth.jpg");
    texturaLua = carregaTextura("textures/moon.jpg");
    texturaMarte = carregaTextura("textures/mars.jpg");
    texturaJupiter = carregaTextura("textures/jupiter.jpg");
    texturaSaturno = carregaTextura("textures/saturn.jpg");
    texturaAnelSaturno = carregaTextura("textures/saturnRing.png");
    texturaUrano = carregaTextura("textures/uranus.jpg");
    texturaNetuno = carregaTextura("textures/neptune.jpg");
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h); // Define o tamanho da tela
    glMatrixMode(GL_PROJECTION); // Muda para o modo de projeção (lente)
    glLoadIdentity();
    
    // Define o ângulo de visão, proporção da tela e distância de corte (perto e longe)
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
    
    glMatrixMode(GL_MODELVIEW); // Volta para o modo de desenho
}

void mouseClick(int button, int state, int x, int y) {
    // --- Rotação da Câmera ---
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            girandoCamera = true;
            ultimoX = x;
            ultimoY = y;
        } else {
            girandoCamera = false;
        }
    }

    // --- Sistema de Zoom ---
    if (state == GLUT_DOWN) {
        if (button == 3) { 
            distanciaCamera -= 5.0f; 
            
            if (distanciaCamera < 10.0f) {
                distanciaCamera = 10.0f;
            }
        } 
        else if (button == 4) { 
            distanciaCamera += 5.0f;
            
            if (distanciaCamera > 150.0f) {
                distanciaCamera = 150.0f;
            }
        }
    }
}

void mouseMove(int x, int y) {
    if (girandoCamera) {
        int deltaX = x - ultimoX;
        int deltaY = y - ultimoY;

        // Sensibilidade do movimento
        anguloCameraX += deltaX * 0.5f; 
        anguloCameraY += deltaY * 0.5f; 

        // Limita o eixo Y
        if (anguloCameraY > 89.0f) anguloCameraY = 89.0f;
        if (anguloCameraY < -89.0f) anguloCameraY = -89.0f;

        ultimoX = x;
        ultimoY = y;
        glutPostRedisplay();
    }
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

    // --- Movimento Câmera ---
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMove);

    inicializa();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutTimerFunc(16, atualizaTemporizador, 0);

    glutMainLoop();
    return 0;
}
//The song for buzzer was taken from this web https://openwebinars.net/blog/tutorial-de-arduino-sonidos-con-arduino/


#include <LCD5110_Graph.h>
LCD5110 myGLCD(7, 6, 5, 3, 4); //arduino pins RST-3 CE-4 DC-5 DIN-6 CLK-7 
#define joyX 2//topes son 0izq 1023der
#define joyY 3
#define botonrojo 4
#define botonazul 5
#define der 0
#define izq 2
#define arriba 3
#define abajo 1
extern uint8_t SmallFont[];
extern uint8_t TinyFont[];
extern uint8_t snake[]; //dibujo inicial
char cadena[16];
//serpiente
uint8_t tamserpiente = 2;
uint8_t serpienteposX[30];//con estos dos arrays el cuerpo de la serpiente
uint8_t serpienteposY[30];
uint8_t cabezaX = 42;
uint8_t cabezaY = 23;
uint8_t cabezaXant;
uint8_t cabezaYant;
uint8_t colaX;
uint8_t colaY;
uint8_t dir, dir2;
uint8_t direccion = 0;
uint8_t comidaX, comidaY;
unsigned long tiempoact, tiempoant;
unsigned long tiempoact2, tiempoant2;

uint8_t inversovelocidad = 1000;
uint8_t puntos = 0;
uint8_t i, diranterior;

int spk = 2;                                         // altavoz a GND y pin 13
int c[5] = {131, 262, 523, 1046, 2093}; // frecuencias 4 octavas de Do
int cs[5] =  {139, 277, 554, 1108, 2217}; // Do#
int d[5] = {147, 294, 587, 1175, 2349}; // Re
int ds[5] = {156, 311, 622, 1244, 2489}; // Re#
int e[5] = {165, 330, 659, 1319, 2637}; // Mi
int f[5] = {175, 349, 698, 1397, 2794}; // Fa
int fs[5] = {185, 370, 740, 1480, 2960}; // Fa#
int g[5] = {196, 392, 784, 1568, 3136}; // Sol
int gs[5] = {208, 415, 831, 1661, 3322}; // Sol#
int a[5] = {220, 440, 880, 1760, 3520}; // La
int as[5] = {233, 466, 932, 1866, 3729}; // La#
int b[5] = {247, 494, 988, 1976, 3951}; // Si



void nota(int frec, int t)
{
  tone(spk, frec);     // suena la nota frec recibida
  delay(t);                // para despues de un tiempo t
}
void gameover() { //comprueba si te has chocado con la pared y pierdes el juego
  if (cabezaX >= 83 || cabezaX <= 0 || cabezaY >= 47 || cabezaY <= 0) { //RUTINA GAMEOVER
    myGLCD.clrScr(); // Limpiamos la pantalla
    myGLCD.print("Puntuacion:", CENTER, 0);
    sprintf(cadena, "%d", puntos);
    myGLCD.print(cadena, CENTER , 15);
    myGLCD.print("Bien jugado", CENTER, 30);
    myGLCD.update(); // Refresca la pantalla
    nota(a[1], 400); noTone(spk); delay(400);
    nota(e[1], 400); noTone(spk); delay(400);
    nota(a[1], 400); noTone(spk); delay(200);
    nota(e[1], 200); noTone(spk); delay(200);
    nota(a[1], 200); noTone(spk); delay(200);
    nota(as[1], 100); noTone(spk); delay(100);
    nota(b[1], 400); noTone(spk); delay(400);
    nota(fs[1], 400); noTone(spk); delay(400);
    nota(b[1], 400); noTone(spk); delay(200);
    nota(fs[1], 200); noTone(spk); delay(200);
    nota(b[1], 200); noTone(spk); delay(200);
    nota(as[1], 100); noTone(spk); delay(100);
    nota(a[1], 400); noTone(spk); delay(400);
    nota(e[1], 400); noTone(spk); delay(400);
    nota(a[1], 400); noTone(spk); delay(400);
    while (1);
  }
  int j;
  for (j = 0; j < tamserpiente - 1; j++) {
    if ((cabezaX == serpienteposX[j]) && (cabezaY == serpienteposY[j])) { //se come a si misma
      myGLCD.clrScr(); // Limpiamos la pantalla
      myGLCD.print("Puntuacion:", CENTER, 0);
      sprintf(cadena, "%d", puntos);
      myGLCD.print(cadena, CENTER , 15);
      myGLCD.print("Bien jugado", CENTER, 30);
      myGLCD.update(); // Refresca la pantalla
      nota(a[1], 400); noTone(spk); delay(400);
      nota(e[1], 400); noTone(spk); delay(400);
      nota(a[1], 400); noTone(spk); delay(200);
      nota(e[1], 200); noTone(spk); delay(200);
      nota(a[1], 200); noTone(spk); delay(200);
      nota(as[1], 100); noTone(spk); delay(100);
      nota(b[1], 400); noTone(spk); delay(400);
      nota(fs[1], 400); noTone(spk); delay(400);
      nota(b[1], 400); noTone(spk); delay(200);
      nota(fs[1], 200); noTone(spk); delay(200);
      nota(b[1], 200); noTone(spk); delay(200);
      nota(as[1], 100); noTone(spk); delay(100);
      nota(a[1], 400); noTone(spk); delay(400);
      nota(e[1], 400); noTone(spk); delay(400);
      nota(a[1], 400); noTone(spk); delay(400);
      while (1);
    }

  }
}

void iniciar() {//inicio y dibujo pantalla de carga, marco y cabeza serpiente
  randomSeed(analogRead(A0));
  pinMode(A1, INPUT_PULLUP);  // set pull-up on analog pin 1 
  pinMode(2, OUTPUT); //sound
  tiempoact = 0;
  tiempoant = 0;
  tiempoact2 = 0;
  tiempoant2 = 0;
  int i;
  pinMode(16,INPUT);//ARRIBA
  pinMode(17,INPUT);//IZQ
  pinMode(18,INPUT);//ABAJO
  pinMode(19,INPUT);//DER
comidaX = random(0, 83);
  comidaY = random(0, 47);
  colaX = cabezaX;
  colaY = cabezaY;
  pinMode(8, INPUT);
  for (i = 0; i < 29; i++) {
    serpienteposX[i] = 90;
    serpienteposY[i] = 90; //asi nunca se borraran si aun no estaban
  }
  myGLCD.InitLCD();//inicializo
  myGLCD.clrScr(); // Limpiamos la pantalla
  myGLCD.setFont(SmallFont); // Decimos el tipo de fuente "SmallFont"
  // Escribimos un texto que será colocado en el Centro en X y en Y su posición sera 10
  myGLCD.print("STARTING", CENTER, 10);
  // Escribimos un texto que será colocado en el CENTRO en X y en Y su posición sera 30
  myGLCD.print("GAME:", CENTER, 30);
  myGLCD.update(); // Refresca la pantalla para que se muestren los datos introducidos
  delay(2000); // Retardo general
  myGLCD.drawBitmap(0, 0, snake, 84, 48);
  myGLCD.print("SNAKE", CENTER, 5);
  myGLCD.update();
  delay(2000); // Retardo general
  myGLCD.clrScr();
  myGLCD.drawLine(0, 0, 83, 0);
  myGLCD.drawLine(83, 0, 83, 47);
  myGLCD.drawLine(0, 47, 83, 47);
  myGLCD.drawLine(0, 0, 0, 47);
  myGLCD.setPixel(cabezaX, cabezaY);
  myGLCD.drawRect(comidaX, comidaY, comidaX + 1, comidaY + 1);
  myGLCD.update();
}

void obtenerdireccion() { //obtengo direccion y la actualizo

  if(digitalRead(19)==HIGH){
    direccion=der;
    }else if(digitalRead(18)){
      direccion=abajo;
    }else if(digitalRead(17)){
          direccion=izq;
    }else if(digitalRead(16)){
          direccion=arriba;
}

}


void mover() { //voy moviendo la serpiente en la direccion que es
  myGLCD.clrScr();
  inversovelocidad = 1000 - (puntos * (10)); //cambio velocidad
  Serial.println(inversovelocidad);
  myGLCD.drawLine(0, 0, 83, 0);
  myGLCD.drawLine(83, 0, 83, 47);
  myGLCD.drawLine(0, 47, 83, 47);
  myGLCD.drawLine(0, 0, 0, 47);
  myGLCD.drawRect(comidaX, comidaY, comidaX + 1, comidaY + 1);
  i = tamserpiente - 2;

  for (i = tamserpiente - 2; i > 0; i--) {
    serpienteposX[i] = serpienteposX[i - 1];
    serpienteposY[i] = serpienteposY[i - 1];
    myGLCD.drawCircle(serpienteposX[i], serpienteposY[i], 1);
  }
  if (tamserpiente > 1) {
    serpienteposX[0] = cabezaXant;
    serpienteposY[0] = cabezaYant;
    myGLCD.drawCircle(serpienteposX[0], serpienteposY[0], 1);
  }
  switch (direccion) {
    case der:
      cabezaXant = cabezaX;
      cabezaYant = cabezaY;
      cabezaX = cabezaX + 2;
      myGLCD.drawCircle(cabezaX, cabezaY, 1);
      break;
    case izq:
      cabezaXant = cabezaX;
      cabezaYant = cabezaY;
      cabezaX = cabezaX - 2;
      myGLCD.drawCircle(cabezaX, cabezaY, 1);
      break;
    case abajo:
      cabezaXant = cabezaX;
      cabezaYant = cabezaY;
      cabezaY = cabezaY + 2;
      myGLCD.drawCircle(cabezaX, cabezaY, 1);
      break;
    case arriba:
      cabezaXant = cabezaX;
      cabezaYant = cabezaY;
      cabezaY = cabezaY - 2;
      myGLCD.drawCircle(cabezaX, cabezaY, 1);
      break;
  }
  myGLCD.update();
}

void comprobarcomida() {
  if (((cabezaX >= comidaX) && (cabezaX <= comidaX + 1)) && ((cabezaY >= comidaY) && (cabezaY <= comidaY + 1))) {
    //tamserpiente++;
    myGLCD.clrRect(comidaX, comidaY, comidaX + 1, comidaY + 1);
    comidaX = random(0, 80);
    comidaY = random(0, 45);
    tamserpiente++;
    tone(2, 1000);
    delay(100);
    noTone(2);
    delay(100);
    tone(2, 1000);

    puntos++;
  }


}
void setup()
{
  Serial.begin(9600);
  iniciar();//inicio y dibujo pantalla de carga, marco y cabeza serpiente
}

void loop()
{

  // obtenerdireccion();
  tiempoact2 = millis();
  if (tiempoact2 - tiempoant2 > 100) {
    obtenerdireccion();
    tiempoant2 = tiempoact2;
  }
  tiempoact = millis();
  if (tiempoact - tiempoant > inversovelocidad) {
    mover();
    tiempoant = tiempoact;
    noTone(2);
  }
  comprobarcomida();
  gameover();//comprueba si te has chocado con la pared y pierdes el juego
}

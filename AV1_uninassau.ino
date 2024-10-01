#include <Servo.h>               // Importa a biblioteca para controle do servo motor

// Definição dos pinos que vamos usar
const int trigPin = 9;   // Pino de Trigger do sensor ultrassônico
const int echoPin = 10;  // Pino de Echo do sensor ultrassônico
const int ledGreen = 5;  // LED verde que indica nível seguro
const int ledRed = 13;   // LED vermelho que indica nível crítico
const int servoPin = 3;  // Pino para o servo motor que controla a comporta
const int buzzerPin = 4; // Pino para o buzzer piezo que emite sons

// Distância segura para o nível da água
const int safeDistance = 50;   // Distância segura em centímetros (50 cm)

// Instâncias globais
Servo servo;                  // Criamos uma instância do servo motor

// Variáveis globais para armazenar informações
long duration;                 // Variável para armazenar a duração do pulso ultrassônico
int distance;                  // Variável para armazenar a distância calculada

void setup() {
  // Configurando os pinos
  pinMode(trigPin, OUTPUT);    // O pino Trigger é saída
  pinMode(echoPin, INPUT);     // O pino Echo é entrada
  pinMode(ledGreen, OUTPUT);   // LED verde como saída
  pinMode(ledRed, OUTPUT);     // LED vermelho como saída
  pinMode(buzzerPin, OUTPUT);  // Buzzer piezo como saída
  
  // Conectamos o servo ao pino e o inicializamos em 0° (comporta fechada)
  servo.attach(servoPin);
  servo.write(0);

  // Começando a comunicação com o monitor serial
  Serial.begin(9600);
  Serial.println("Sistema de monitoramento iniciado.");
}

void loop() {
  // Enviando um pulso ultrassônico
  digitalWrite(trigPin, LOW);  // Começamos com o Trigger em nível baixo
  delayMicroseconds(2);        // Esperamos 2 microssegundos
  digitalWrite(trigPin, HIGH); // Enviamos o pulso por 10 microssegundos
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  // Retornamos o Trigger para nível baixo
  
  // Capturamos o pulso refletido e calculamos a distância
  duration = pulseIn(echoPin, HIGH);  // Medimos o tempo que o pulso levou para voltar
  distance = duration * 0.034 / 2;    // Convertendo o tempo em distância (cm)

  // Mostrando a distância no monitor serial para verificação
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Verificando o nível da água
  if (distance >= safeDistance) {
    // Nível de água está seguro
    digitalWrite(ledGreen, HIGH);   // Acende o LED verde
    digitalWrite(ledRed, LOW);      // Desliga o LED vermelho
    servo.write(0);                 // Mantemos a comporta fechada (0 graus)
    noTone(buzzerPin);              // Desligamos o buzzer
    Serial.println("Nivel seguro. Comporta fechada.");
  } else {
    // Nível de água está crítico
    digitalWrite(ledGreen, LOW);    // Desliga o LED verde
    digitalWrite(ledRed, HIGH);     // Acende o LED vermelho
    servo.write(180);               // Abrimos a comporta (180 graus)
    tone(buzzerPin, 1000);          // O buzzer emite um som a 1000 Hz
    Serial.println("Nivel critico! Comporta aberta. Alarme ativado.");
  }
  
  // Pausa para evitar leituras muito rápidas
  delay(1000);
}

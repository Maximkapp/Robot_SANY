#include <IRremote.h> // библиотека для ИК датчика
#define PIN_A1 18
#define PIN_A2 19
#define PIN_B1 16
#define PIN_B2 17
#define echoPin 25 // Echo Pin
#define trigPin 26 // Trigger Pin
IRrecv irrecv(12); // ИК датчик



decode_results results; // хз
const int MaxDistanceMeter = 10; // максимально допустимое число для дальномера в СМ (сколько до предмета)
int MinimumRange = 0; // Минимальное допустимое значение в СМ
long duration, distance; // Длительность, используемая для вычисления расстояния, растояние 
const int TimeToTurn = 1000; // время для поворота на 90
unsigned long last_time; // для millis() для дальномера
const long interval = 100; // С каким интервалом дальномер будет чекать расстояние миллисекундах
byte speed = 128;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(PIN_A1, OUTPUT); 
  pinMode(PIN_A2, OUTPUT);
  pinMode(PIN_B1, OUTPUT);
  pinMode(PIN_B2, OUTPUT);
  irrecv.enableIRIn(); // вкл ИК приемник 
  Serial.begin(115200);
}


void loop() 
{
  if(millis() - last_time > interval){     //Прерывание для дальномера
    last_time = millis();
    calculate_distance();
  }
  if ( irrecv.decode( &results )) {  // возвращает код кнопки с пульта 
    Serial.println(results.value, HEX);

    if (results.value==0xFF02FD && speed < 255){ // тут надо кнопку + прописать, увеличение скорости при нажатии кнопки
      speed += 30;
    }
    if (results.value==0xFF02FD && speed > 0){ // тут надо кнопку - прописать, уменьшение скорости при нажатии кнопки
      speed -= 30;
    }

    if (results.value==0xFF629D){ // вперед на пульте
      forward();
      Serial.print("jopa");
    }
    if (results.value==0xFFA857){ // назад
      backward();
    }   

    if (results.value==0xFF22DD){ // влево
      left();
    }
    if (results.value==0xFFC23D){ // вправо
      right();
    }
    if (results.value==0xFF02FD){ // стоп
      stop();
    }
    
    irrecv.resume();  
  }
}


void forward() // Движение вперед
{
  analogWrite(PIN_A1, speed);
  analogWrite(PIN_A2, 0);
  analogWrite(PIN_B1, speed);
  analogWrite(PIN_B2, 0);
}

void backward() // Движение назад
{
  analogWrite(PIN_A1, 0);
  analogWrite(PIN_A2, speed);
  analogWrite(PIN_B1, 0);
  analogWrite(PIN_B2, speed);
}

void right() // Движение вправо
{
  analogWrite(PIN_A1, speed);
  analogWrite(PIN_A2, 0);
  analogWrite(PIN_B1, 0);
  analogWrite(PIN_B2, speed);
}

void left() // Движение влево
{
  analogWrite(PIN_A1, 0);
  analogWrite(PIN_A2, speed);
  analogWrite(PIN_B1, speed);
  analogWrite(PIN_B2, 0);
}

void stop() // Остановка движения
{
  analogWrite(PIN_A1, 0);
  analogWrite(PIN_A2, 0);
  analogWrite(PIN_B1, 0);
  analogWrite(PIN_B2, 0);
}

int calculate_distance()      //Следующая функция возвращает расстояние и останавливает если есть препятствие
{ 
// trigPin/echoPin используется для определения 
//расстояние до ближайшего объекта, отражаясь от него звуковыми волнами 
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 
 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 distance = duration/58.2;  //расстояние (в см) исходя из скорости звука.
if(distance >= MaxDistanceMeter || distance <= MinimumRange){
  Serial.println(distance); // далеко 
}
else{
  Serial.println(distance); // близко
  stop();
  Serial.print("jopa");
}
return distance;
}
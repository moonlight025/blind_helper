#define trigPin 9 // 超聲波傳感器 trig pin
#define echoPin 10 // 超聲波傳感器 echo pin
#define BeeperPin 13 // LED 腳位
#define Do 523
#define Re 587
#define Mi 659
#define Fa 698
#define So 784
#define La 880
#define Si 988


int melody[7] = {Do, Re, Mi, Fa, So, La, Si};

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(BeeperPin, OUTPUT);
  Serial.begin(9600); // 建立 Serial 連接，可以觀察超聲波傳感器返回的距離數據
}

void loop() {
  digitalWrite(trigPin, LOW); // 先發送一個 LOW 訊號
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH); // 發送一個 10 毫秒的高訊號
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH); // 讀取回傳的脈衝寬度
  int distance = duration * 0.034 / 2; // 計算距離，公式為距離 = 脈衝寬度 * 速度 / 2，其中速度為聲速（約為 34 cm/ms）

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < 20) { // 當距離小於 10 cm 時亮起 LED
    for (int i = 0;i < 8; i++){
      tone(BeeperPin, melody[i]);
      delay(250);
    }
  } else { // 否則熄滅 LED
    noTone(BeeperPin);
    delay(2000);
  }

  delay(500); // 延遲 500 毫秒，避免短時間內大量讀取超聲波傳感器數據
}

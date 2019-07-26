#define SRCLK   (5)
#define RCLK    (6)
#define SER     (7)
#define N_OE    (8)
#define Button_Power (3)
#define Button_Switch (2)
int n_p = 0;
int val1_p = 0;
int old_val1_p = 0;
int n_s = 0;

void setup() {
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK,  OUTPUT);
  pinMode(SER,   OUTPUT);
  pinMode(N_OE,  OUTPUT);
  pinMode(Button_Power, INPUT);
  pinMode(Button_Switch, INPUT);
}

// 点灯パターン(1=点灯, 0=消灯)
const long PATTERNS[] = {
  0b0000000000000001,
  0b0000000000000010,
  0b0000000000000100,
  0b0000000000001000,
  0b0000000000010000,
  0b0000000000100000,
  0b0000000001000000,
  0b0000000010000000,
  0b0000000100000000,
  0b0000001000000000,
  0b0000010000000000,
  0b0000100000000000,
  0b0001000000000000,
  0b0010000000000000,
  0b0100000000000000,
  0b1000000000000000,
};

//Button_Powerによってon,off切り替え
void power()
{
  val1_p = digitalRead(Button_Power);
  if((val1_p == 1)&&(old_val1_p == 0)){
    n_p = 1 - n_p;
    delay(100);
  }
 
  old_val1_p = val1_p;
  
  if(n_p == 1){
    digitalWrite(N_OE,HIGH);
  }else{
    digitalWrite(N_OE,LOW);
  }
}

void myShiftOut(int dataPin, int clockPin, long data, int dataLength) 
{
  for (int i = 0; i < dataLength; i++ ) {
    digitalWrite(dataPin, !!(data & 1L << i));
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
  }
}

void data_switch()
{
  //Button_Switchによって出力するデータを切り替え
  if(digitalRead(Button_Switch) == LOW)
  {
    int max_pattern = sizeof(PATTERNS)/sizeof(PATTERNS[0]);
    int sft = n_s % max_pattern; 
    myShiftOut(SER, SRCLK, PATTERNS[sft], 16);
    n_s++;
  }
}

void loop() {
  power();

    digitalWrite(RCLK,  LOW);

    data_switch();
  
    digitalWrite(RCLK,  HIGH);

    delay(200);
}

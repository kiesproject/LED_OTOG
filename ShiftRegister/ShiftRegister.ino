#define SRCLK   (5)
#define RCLK    (6)
#define SER     (7)
#define N_OE    (8)
#define Button_Power (3)
#define Button_Switch (2)
#define Button_Data (4)
int n_p = 0;
int val_p = 0;
int old_val_p = 0;
int n_s = 0;
int val_d = 0;
int old_val_d = 0;
int n_d = 1;
int n_dt = 0;
//データの長さ
int data_len;
//パターンの数
int pattern_num;
//LEDの数
int LED_num = 16;


void setup() {
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK,  OUTPUT);
  pinMode(SER,   OUTPUT);
  pinMode(N_OE,  OUTPUT);
  pinMode(Button_Power, INPUT);
  pinMode(Button_Switch, INPUT);
  pinMode(Button_Data, INPUT);
}

// 点灯パターン（2進数にしたとき、1の場所が点灯） 
const long PATTERNS[2][16] = {
  {
    0x0001,
    0x0002,
    0x0004,
    0x0008,
    0x0010,
    0x0020,
    0x0040,
    0x0080,
    0x0100,
    0x0200,
    0x0400,
    0x0800,
    0x1000,
    0x2000,
    0x4000,
    0x8000
  },
  {
    0x8000,
    0x4000,
    0x2000,
    0x1000
  }
};

//Button_Powerによってon,off切り替え
void power()
{
  val_p = digitalRead(Button_Power);
  if((val_p == 1)&&(old_val_p == 0)){
    n_p = 1 - n_p;
    delay(100);
  }
 
  old_val_p = val_p;
  
  if(n_p == 1){
    digitalWrite(N_OE,HIGH);
  }else{
    digitalWrite(N_OE,LOW);
  }
}

//Button_Dataによって出力パターン切り替え
void pattern_switch()
{
  val_d = digitalRead(Button_Data);
  if((val_d == 1)&&(old_val_d == 0)){
    pattern_num = sizeof(PATTERNS)/sizeof(PATTERNS[n_d]);
    n_dt++;
    n_d = n_dt % pattern_num;
    delay(100);
  }
 
  old_val_d = val_d;
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
    data_len = sizeof(PATTERNS[n_d]) / sizeof(PATTERNS[n_d][1]);
    int sft = n_s % data_len; 
    myShiftOut(SER, SRCLK, PATTERNS[n_d][sft], data_len);
    n_s++;
  }
}

void loop() 
{
  power();

  pattern_switch();

  digitalWrite(RCLK,  LOW);

  data_switch();
  
  digitalWrite(RCLK,  HIGH);
  
  delay(200);
}

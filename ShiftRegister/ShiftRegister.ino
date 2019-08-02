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
const long PATTERNS[2][27] = {
  {
    0x0083,   //0000000010000011
    0x0085,
    0x0086,
    0x0103,
    0x0105,
    0x0106,
    0x0203,
    0x0205,
    0x0206,
    0x0403,
    0x0405,
    0x0406,
    0x0803,
    0x0805,
    0x0806,
    0x1003,
    0x1005,
    0x1006,
    0x2003,
    0x2005,
    0x2006,
    0x4003,
    0x4005,
    0x4006,
    0x8003,
    0x8005,
    0x8006
  },
  {
    0xFF83,
    0xFF85,
    0xFF86,
    0xFF83,
    0xFF85,
    0xFF86,
    0xFF83,
    0xFF85,
    0xFF86,
    0xFF83,
    0xFF85,
    0xFF86,
    0xFF83,
    0xFF85,
    0xFF86,
    0xFF83,
    0xFF85,
    0xFF86,
    0xFF83,
    0xFF85,
    0xFF86,
    0xFF83,
    0xFF85,
    0xFF86,
    0xFF83,
    0xFF85,
    0xFF86,
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

void data_out()
{
  data_len = sizeof(PATTERNS[n_d]) / sizeof(PATTERNS[n_d][1]);
  int sft = n_s % data_len; 
  myShiftOut(SER, SRCLK, PATTERNS[n_d][sft], 16);
  n_s++;
}

void loop() 
{
  power();

  pattern_switch();

  digitalWrite(RCLK,  LOW);

  data_out();
  
  digitalWrite(RCLK,  HIGH);
  
  delay(200);
}

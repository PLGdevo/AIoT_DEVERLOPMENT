#include <Arduino.h>
//  ================================================================  //
//  Khai báo các chân kết nối ngoại vi có trên TZ_KIT  //
#define Statusled 2
#define Buzzer 14

#define SENSOR1 36 // Dùng cho ác cảm biến 3 dây trả về tín hiệu điện áp từ 0 - 3.3
#define SENSOR2 39 // Dùng cho ác cảm biến 3 dây trả về tín hiệu điện áp từ 0 - 3.3
#define SENSOR3 34 // Dùng cho các cảm biến cần đấu thêm trở Fixed 10KOhm
#define SENSOR4 35 // Dùng cho các cảm biến cần đấu thêm trở Fixed 10KOhm
int SENSOR[4] = {SENSOR1, SENSOR2, SENSOR3, SENSOR4};

#define OUT1 32
#define OUT2 33
#define OUT3 25
#define OUT4 26
int OUT[4] = {OUT1, OUT2, OUT3, OUT4};

#define BUTTON1 23 // NUT UP
#define BUTTON2 5  // NUT ENTER/BACK/...
#define BUTTON3 13 // NUT DOWN
int BUTTON[3] = {BUTTON1, BUTTON2, BUTTON3};

// Khai báo cổng Serial1 với chân TXD1 và RXD1 lần lượt là chân 19 18
#define TXD1 19
#define RXD1 18
// Khai báo cổng Serial2 phục vụ cho MODBUS với chân TXD1 và RXD1 lần lượt là chân 17 16
#define TXD2 17
#define RXD2 16
// Khai báo cổng giao tiếp I2C với chân SDA và SCL lần lượt là 21 22
#define SDA_ 21
#define SCL_ 22
//  ================================================================  //

// Các biến phục vụ cho việc hiển thị LCD trên board TZ_KIT //
int functionScreen = 0;
bool trans_Screen = false;
unsigned long lasttime_Sceen = 0;
unsigned long time_Sceen = 0;
// Biến phục vụ cho việc thực thi theo chu kì các tác vụ như đọc nút nhấn, hiển thị màn hình
unsigned long lasttimeprocess = 0;
unsigned long timeprocess = 0;
//  ================================================================  //
// Các biến phục vụ cho việc xử lý cảm biến
const unsigned long SampleTime4 = 10;
unsigned long timer4 = 0;
int Sum_adc4 = 0;
int count4 = 0;

//  ================================================================  //
// Các biến phục vụ cho việc xử lý nút nhấn
int ENTER = 0;
int BACK = 0;
//  ================================================================  //
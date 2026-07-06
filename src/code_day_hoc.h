#include <Arduino.h>
#define BUTTON1 23 // NUT UP
#define BUTTON2 5  // NUT ENTER/BACK/...
#define BUTTON3 13 // NUT DOWN
int BUTTON[3] = {BUTTON1, BUTTON2, BUTTON3};
#define ledstatus 2 // Đèn led báo trạng thái

void setup()
{
    Serial.begin(115200);
    for (int i = 0; i < 3; i++)
    {
        pinMode(BUTTON[i], INPUT); // Đoạn này cấu hình nút nhấn dùng for
    }
    pinMode(ledstatus, OUTPUT);   // Đoạn này cấu hình nút nhấn là OUTPUT
    digitalWrite(ledstatus, LOW); // Tắt ngõ ra khi vừa khởi động đảm bảo việc an toàn cho hệ thống
}

void nhapnhayled()
{
    static unsigned long time_ = 0;   // Khai báo biến time_ khi lần đầu được gọi là 0
    static int a = 0;                 // Khai báo biến a khi lần đầu được gọi là 0
    const unsigned long chu_ky = 500; // Khai báo chu kỳ chớp tắt đèn led
    if (millis() - time_ >= chu_ky)   // So sánh nếu đù với thời gian chu kỳ được truyền vào
    {
        time_ = millis(); // biến time_ bằng với thời gian mà ESP32 đã chạy
        a = !a;           // Đảo trạng thái biến a
    }
    digitalWrite(ledstatus, a); // Sau khi hoàn tất xử lý, xuất tín hiệu đèn led theo biến a
}

void tanggiam_dosang()
{
    static int dosang = 0;                           // Khai báo độ sáng của đèn khi được cấp nguồn là 0
    int doctrangthai_nuttang = digitalRead(BUTTON1); // Đọc trạng thái nút nhấn tăng (ở đây là nút 1)
    int doctrangthai_nutgiam = digitalRead(BUTTON3); // Đọc trạng thái nút nhấn giảm (ở đây là nút 3)
    if (doctrangthai_nuttang == HIGH)                // Kiểm tra nút tăng có đang được ấn
    {
        dosang++; // Nếu đúng được ấn thì tăng độ sáng
        if (dosang > 255)
        {
            dosang = 255; // Giới hạn giá trị độ sáng
        }
    }
    if (doctrangthai_nutgiam == HIGH) // Kiểm tra nút giảm có đang được ấn
    {
        dosang--; // Nếu đúng được ấn thì giảm độ sáng
        if (dosang < 0)
        {
            dosang = 0; // Giới hạn giá trị độ sáng
        }
    }
    analogWrite(ledstatus, dosang); // Sau khi hoàn tất quá trình kiểm tra biến và xử lý độ sáng thì xuất ra giá trị PWM theo giá trị độ sáng để điều chỉnh ánh sáng led
}

void xulychonchedo()
{
    static byte mode = 0;
    static byte lastbuttonState = 0;
    byte buttonState = digitalRead(BUTTON2);
    if (buttonState != lastbuttonState)
    {
        lastbuttonState = buttonState;
        if (buttonState == HIGH)
        {
            mode++;
            if (mode > 2)
                mode = 0;
        }
    }
    switch (mode)
    {
    case 0:
        digitalWrite(ledstatus, LOW);
        break;
    case 1:
        nhapnhayled();
        break;
    case 2:
        tanggiam_dosang();
        break;
    }
}

void loop()
{
    xulychonchedo();
}
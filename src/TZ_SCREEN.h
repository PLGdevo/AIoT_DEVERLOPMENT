#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
// SCREEN STARTUP
void Screen_STARTUP()
{
    lcd.clear();
    lcd.setCursor(2, 1);
    lcd.print("RUNNING SYSTEM...");
}
void Screen_CONNECT()
{
    lcd.clear();
    lcd.backlight();
    if (WiFi.status() == WL_CONNECTED)
    {
        lcd.setCursor(0, 0);
        lcd.print("CONNECTED WIFI!!");
        lcd.setCursor(0, 1);
        lcd.print("WIFI: ");
        lcd.print(WiFi.SSID());
        lcd.setCursor(0, 2);
        lcd.print("ESP IP: ");
        lcd.print(WiFi.localIP().toString());
    }
    if (WiFi.status() != WL_CONNECTED)
    {
        lcd.setCursor(0, 0);
        lcd.print("FAILD CONNECT WIFI!!");
    }
}

// SCREEN MENU
void Screen_DASHBOARD() // 1
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("<> DASHBOARD");
    lcd.setCursor(0, 1);
    lcd.print("   CONTROL");
    lcd.setCursor(0, 2);
    lcd.print("   SENSOR");
    lcd.setCursor(0, 3);
    lcd.print("   OPERATE");
}
void Screen_CONTROL() // 2
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("   DASHBOARD");
    lcd.setCursor(0, 1);
    lcd.print("<> CONTROL");
    lcd.setCursor(0, 2);
    lcd.print("   SENSOR");
    lcd.setCursor(0, 3);
    lcd.print("   OPERATE");
}
void Screen_SENSOR() // 3
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("   DASHBOARD");
    lcd.setCursor(0, 1);
    lcd.print("   CONTROL");
    lcd.setCursor(0, 2);
    lcd.print("<> SENSOR");
    lcd.setCursor(0, 3);
    lcd.print("   OPERATE");
}
void Screen_RUN() // 4
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("   DASHBOARD");
    lcd.setCursor(0, 1);
    lcd.print("   CONTROL");
    lcd.setCursor(0, 2);
    lcd.print("   SENSOR");
    lcd.setCursor(0, 3);
    lcd.print("<> OPERATE");
}
void Screen_SYSTEM() // 5
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("   CONTROL");
    lcd.setCursor(0, 1);
    lcd.print("   SENSOR");
    lcd.setCursor(0, 2);
    lcd.print("   OPERATE");
    lcd.setCursor(0, 3);
    lcd.print("<> SYSTEM");
}
void Screen_ABOUT() // 6
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("   SENSOR");
    lcd.setCursor(0, 1);
    lcd.print("   OPERATE");
    lcd.setCursor(0, 2);
    lcd.print("   SYSTEM");
    lcd.setCursor(0, 3);
    lcd.print("<> ABOUT");
}

// SCREEN TASK CONTROL
void Screen_CONTROL_1() // 21
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("<> OUTPUT 1");
    lcd.setCursor(0, 1);
    lcd.print("   OUTPUT 2");
    lcd.setCursor(0, 2);
    lcd.print("   OUTPUT 3");
    lcd.setCursor(0, 3);
    lcd.print("   OUTPUT 4");
}
void Screen_CONTROL_2() // 22
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("   OUTPUT 1");
    lcd.setCursor(0, 1);
    lcd.print("<> OUTPUT 2");
    lcd.setCursor(0, 2);
    lcd.print("   OUTPUT 3");
    lcd.setCursor(0, 3);
    lcd.print("   OUTPUT 4");
}
void Screen_CONTROL_3() // 23
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("   OUTPUT 1");
    lcd.setCursor(0, 1);
    lcd.print("   OUTPUT 2");
    lcd.setCursor(0, 2);
    lcd.print("<> OUTPUT 3");
    lcd.setCursor(0, 3);
    lcd.print("   OUTPUT 4");
}
void Screen_CONTROL_4() // 24
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("   OUTPUT 1");
    lcd.setCursor(0, 1);
    lcd.print("   OUTPUT 2");
    lcd.setCursor(0, 2);
    lcd.print("   OUTPUT 3");
    lcd.setCursor(0, 3);
    lcd.print("<> OUTPUT 4");
}

// SCREEN TASK SENSOR
void Screen_SENSOR_STATUS() // 31
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("<> STATE SENSOR");
    lcd.setCursor(0, 1);
    lcd.print("   DASH SENSOR");
}
void Screen_SENSOR_DASH() // 32
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("   STATE SENSOR");
    lcd.setCursor(0, 1);
    lcd.print("<> DASH SENSOR");
}

// SCREEN TASK RUN
void Screen_RUN_AUTO() // 41
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("<> TURN AUTO");
    lcd.setCursor(0, 1);
    lcd.print("   TURN MANUAL");
    lcd.setCursor(0, 2);
    lcd.print("   SETTING PID");
    lcd.setCursor(0, 3);
    lcd.print("   SETTING THRESHOLD");
}
void Screen_RUN_MANUAL() // 42
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("   TURN AUTO");
    lcd.setCursor(0, 1);
    lcd.print("<> TURN MANUAL");
    lcd.setCursor(0, 2);
    lcd.print("   SETTING PID");
    lcd.setCursor(0, 3);
    lcd.print("   SETTING THRESHOLD");
}
void Screen_RUN_SETTING_PID() // 43
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("   TURN AUTO");
    lcd.setCursor(0, 1);
    lcd.print("   TURN MANUAL");
    lcd.setCursor(0, 2);
    lcd.print("<> SETTING PID");
    lcd.setCursor(0, 3);
    lcd.print("   SETTING THRESHOLD");
}
void Screen_RUN_SETTING_THRESHOLD() // 44
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("   TURN AUTO");
    lcd.setCursor(0, 1);
    lcd.print("   TURN MANUAL");
    lcd.setCursor(0, 2);
    lcd.print("   SETTING PID");
    lcd.setCursor(0, 3);
    lcd.print("<> SETTING THRESHOLD");
}

void Screen_AUTO_PID() // 411
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("<> ENABLE PID SYSTEM");
    lcd.setCursor(0, 1);
    lcd.print("   ENABLE ON/OFF SYSTEM");
}
void Screen_AUTO_ON_OFF() // 412
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("   ENABLE PID SYSTEM");
    lcd.setCursor(0, 1);
    lcd.print("<> ENABLE ON/OFF SYSTEM");
}

void Screen_PID() // 411-
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("SETPOINT: ");
    lcd.setCursor(0, 1);
    lcd.print("PRESENT VALUE: ");
    lcd.setCursor(0, 2);
    lcd.print("CAPACITY: ");
    lcd.setCursor(0, 3);
    lcd.print("STATE: ");
}
void Screen_ON_OFF() // 412-
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("THRESHOLD UP: ");
    lcd.setCursor(0, 1);
    lcd.print("THRESHOLD DOWN: ");
    lcd.setCursor(0, 2);
    lcd.print("PRESENT VALUE: ");
    lcd.setCursor(0, 3);
    lcd.print("STATE: ");
}

void Screen_MANUAL_CONTROL() // 421
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("PRESENT VALUE: ");
}

void Screen_SETTING_PID() // 431
{
}
void Screen_SETTING_THRESHOLD() // 441
{
}

// SCREEN TASK SYSTEM
void Screen_SYSTEM_NETWORK() // 51
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("<> NETWORK");
    lcd.setCursor(0, 1);
    lcd.print("   DEVICE INFO");
    lcd.setCursor(0, 2);
    lcd.print("   STORAGE");
    lcd.setCursor(0, 3);
    lcd.print("   RESTART");
}
void Screen_SYSTEM_DEVICE_INFOR() // 52
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("   NETWORK");
    lcd.setCursor(0, 1);
    lcd.print("<> DEVICE INFO");
    lcd.setCursor(0, 2);
    lcd.print("   STORAGE");
    lcd.setCursor(0, 3);
    lcd.print("   RESTART");
}
void Screen_SYSTEM_STORAGE() // 53
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("   NETWORK");
    lcd.setCursor(0, 1);
    lcd.print("   DEVICE INFO");
    lcd.setCursor(0, 2);
    lcd.print("<> STORAGE");
    lcd.setCursor(0, 3);
    lcd.print("   RESTART");
}
void Screen_SYSTEM_RESTART() // 54
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("   NETWORK");
    lcd.setCursor(0, 1);
    lcd.print("   DEVICE INFO");
    lcd.setCursor(0, 2);
    lcd.print("   STORAGE");
    lcd.setCursor(0, 3);
    lcd.print("<> RESTART");
}

void Screen_NETWORK_CONNECT() // 511
{
}
void Screen_NETWORK_WIFI() // 512
{
}
void Screen_NETWORK_MQTT() // 513
{
}
void Screen_NETWORK_STATE() // 514
{
}

void programScreenMenu()
{
    static int locationScreenMeunu = 1;
    static int lastlocationScreenMeunu = 0;
    if (trans_Screen)
    {
        trans_Screen = false;
        lastlocationScreenMeunu = 0;
    }
    int KEY_U_D = programButton_UP_DOWN();
    if (KEY_U_D == 3)
    {
        locationScreenMeunu++;
        if (locationScreenMeunu > 6)
        {
            locationScreenMeunu = 1;
        }
    }
    if (KEY_U_D == 4)
    {
        locationScreenMeunu--;
        if (locationScreenMeunu < 1)
        {
            locationScreenMeunu = 6;
        }
    }
    if (locationScreenMeunu != lastlocationScreenMeunu)
    {
        lastlocationScreenMeunu = locationScreenMeunu;
        switch (locationScreenMeunu)
        {
        case 1:
            Screen_DASHBOARD(); // 1
            break;
        case 2:
            Screen_CONTROL(); // 2
            break;
        case 3:
            Screen_SENSOR(); // 3
            break;
        case 4:
            Screen_RUN(); // 4
            break;
        case 5:
            Screen_SYSTEM(); // 5
            break;
        case 6:
            Screen_ABOUT(); // 6
            break;
        default:
            break;
        }
    }
    int KEY_E_B = programButton_ENTER_BACK();
    if (KEY_E_B == 1)
    {
        functionScreen = locationScreenMeunu;
        lcd.clear();
        trans_Screen = true;
    }
}
void programScreenControl()
{
    static int locationScreenControl = 1;
    static int lastlocationScreenControl = 0;
    if (trans_Screen)
    {
        trans_Screen = false;
        lastlocationScreenControl = 0;
    }
    int KEY_U_D = programButton_UP_DOWN();
    if (KEY_U_D == 3)
    {
        locationScreenControl++;
        if (locationScreenControl > 4)
        {
            locationScreenControl = 1;
        }
    }
    if (KEY_U_D == 4)
    {
        locationScreenControl--;
        if (locationScreenControl < 1)
        {
            locationScreenControl = 4;
        }
    }
    if (locationScreenControl != lastlocationScreenControl)
    {
        lastlocationScreenControl = locationScreenControl;
        switch (locationScreenControl)
        {
        case 1:
            Screen_CONTROL_1(); // 21
            break;
        case 2:
            Screen_CONTROL_2(); // 22
            break;
        case 3:
            Screen_CONTROL_3(); // 23
            break;
        case 4:
            Screen_CONTROL_4(); // 24
            break;
        default:
            break;
        }
    }
    int KEY_E_B = programButton_ENTER_BACK();
    if (KEY_E_B == 1)
    {
        if (locationScreenControl == 1)
        {
            functionScreen = 21;
        }
        if (locationScreenControl == 2)
        {
            functionScreen = 22;
        }
        if (locationScreenControl == 3)
        {
            functionScreen = 23;
        }
        if (locationScreenControl == 4)
        {
            functionScreen = 24;
        }
        lcd.clear();
        trans_Screen = true;
    }
    else if (KEY_E_B == 2)
    {
        functionScreen = 0;
        lcd.clear();
        trans_Screen = true;
    }
}
void programScreenSensor()
{
    static int locationScreenSensor = 1;
    static int lastlocationScreenSensor = 0;
    if (trans_Screen)
    {
        trans_Screen = false;
        lastlocationScreenSensor = 0;
    }
    int KEY_U_D = programButton_UP_DOWN();
    if (KEY_U_D == 3)
    {
        locationScreenSensor++;
        if (locationScreenSensor > 2)
        {
            locationScreenSensor = 1;
        }
    }
    if (KEY_U_D == 4)
    {
        locationScreenSensor--;
        if (locationScreenSensor < 1)
        {
            locationScreenSensor = 2;
        }
    }
    if (locationScreenSensor != lastlocationScreenSensor)
    {
        lastlocationScreenSensor = locationScreenSensor;
        switch (locationScreenSensor)
        {
        case 1:
            Screen_SENSOR_STATUS();
            break;
        case 2:
            Screen_SENSOR_DASH();
            break;
        default:
            break;
        }
    }
    int KEY_E_B = programButton_ENTER_BACK();
    if (KEY_E_B == 1)
    {
        if (locationScreenSensor == 1)
        {
            functionScreen == 31;
        }
        if (locationScreenSensor == 2)
        {
            functionScreen == 32;
        }
        lcd.clear();
        trans_Screen = true;
    }
    else if (KEY_E_B == 2)
    {
        functionScreen = 0;
        lcd.clear();
        trans_Screen = true;
    }
}
void programScreenOperate()
{
    static int locationScreenOperate = 1;
    static int lastlocationScreenOperate = 0;
    if (trans_Screen)
    {
        trans_Screen = false;
        lastlocationScreenOperate = 0;
    }
    int KEY_U_D = programButton_UP_DOWN();
    if (KEY_U_D == 3)
    {
        locationScreenOperate++;
        if (locationScreenOperate > 4)
        {
            locationScreenOperate = 1;
        }
    }
    if (KEY_U_D == 4)
    {
        locationScreenOperate--;
        if (locationScreenOperate < 1)
        {
            locationScreenOperate = 4;
        }
    }
    if (locationScreenOperate != lastlocationScreenOperate)
    {
        lastlocationScreenOperate = locationScreenOperate;
        switch (locationScreenOperate)
        {
        case 1:
            Screen_RUN_AUTO();
            break;
        case 2:
            Screen_RUN_MANUAL();
            break;
        case 3:
            Screen_RUN_SETTING_PID();
            break;
        case 4:
            Screen_RUN_SETTING_THRESHOLD();
            break;
        default:
            break;
        }
    }
    int KEY_E_B = programButton_ENTER_BACK();
    if (KEY_E_B == 1)
    {
        if (locationScreenOperate == 1)
        {
            functionScreen = 41;
        }
        if (locationScreenOperate == 2)
        {
            functionScreen = 42;
        }
        if (locationScreenOperate == 3)
        {
            functionScreen = 43;
        }
        if (locationScreenOperate == 4)
        {
            functionScreen = 44;
        }
        lcd.clear();
        trans_Screen = true;
    }
    else if (KEY_E_B == 2)
    {
        functionScreen = 0;
        lcd.clear();
        trans_Screen = true;
    }
}
void programScreenSystem()
{
    static int locationScreenSystem = 1;
    static int lastlocationScreenSystem = 0;
    if (trans_Screen)
    {
        trans_Screen = false;
        lastlocationScreenSystem = 0;
    }
    int KEY_U_D = programButton_UP_DOWN();
    if (KEY_U_D == 3)
    {
        locationScreenSystem++;
        if (locationScreenSystem > 4)
        {
            locationScreenSystem = 1;
        }
    }
    if (KEY_U_D == 4)
    {
        locationScreenSystem--;
        if (locationScreenSystem < 1)
        {
            locationScreenSystem = 4;
        }
    }
    if (locationScreenSystem != lastlocationScreenSystem)
    {
        lastlocationScreenSystem = locationScreenSystem;
        switch (locationScreenSystem)
        {
        case 1:
            Screen_SYSTEM_NETWORK();
            break;
        case 2:
            Screen_SYSTEM_DEVICE_INFOR();
            break;
        case 3:
            Screen_SYSTEM_STORAGE();
            break;
        case 4:
            Screen_SYSTEM_RESTART();
            break;
        default:
            break;
        }
    }
    int KEY_E_B = programButton_ENTER_BACK();
    if (KEY_E_B == 1)
    {
        if (locationScreenSystem == 1)
        {
            functionScreen = 51;
        }
        if (locationScreenSystem == 2)
        {
            functionScreen = 52;
        }
        if (locationScreenSystem == 3)
        {
            functionScreen = 53;
        }
        if (locationScreenSystem == 4)
        {
            functionScreen = 54;
        }
        lcd.clear();
        trans_Screen = true;
    }
    else if (KEY_E_B == 2)
    {
        functionScreen = 0;
        lcd.clear();
        trans_Screen = true;
    }
}
void programScreenAbout()
{
    static int locationScreenAbout = 1;
    static int lastlocationScreenAbout = 0;
    if (trans_Screen)
    {
        trans_Screen = false;
        lastlocationScreenAbout = 0;
    }
    if (locationScreenAbout != lastlocationScreenAbout)
    {
        lastlocationScreenAbout = locationScreenAbout;
        lcd.clear();
        lcd.backlight();
        lcd.setCursor(2, 0);
        lcd.print("TZIoT CONTROLLER");
        lcd.setCursor(0, 1);
        lcd.print("Model: TZ-DEV-ESP32");
        lcd.setCursor(0, 2);
        lcd.print("Firmware: 0.21.6");
        lcd.setCursor(3, 3);
        lcd.print("(C) 2026 TZIoT");
    }
    int KEY_E_B = programButton_ENTER_BACK();
    if (KEY_E_B == 2)
    {
        functionScreen = 0;
        lcd.clear();
        trans_Screen = true;
    }
}

void SCREEN()
{
    switch (functionScreen)
    {
    case 0:
        programScreenMenu();
        break;
    case 2:
        programScreenControl();
        break;
    case 3:
        programScreenSensor();
        break;
    case 4:
        programScreenOperate();
        break;
    case 5:
        programScreenSystem();
        break;
    case 6:
        programScreenAbout();
        break;
    case 21:
        break;
    case 22:
        break;
    case 23:
        break;
    case 24:
        break;
    case 31:
        break;
    case 32:
        break;
    case 41:
        break;
    case 42:
        break;
    case 43:
        break;
    case 44:
        break;
    default:
        break;
    }
}
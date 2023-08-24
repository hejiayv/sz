#include <Arduino.h>
#include <U8g2lib.h>
#include <TM1637.h>
#include <WiFi.h>
#include <Wire.h>
#include <RTClib.h>
#include <WebServer.h>
#include <EEPROM.h>


// 定义PCF8574T的I2C地址
#define PCF8574T_ADDRESS 0x20

// 定义PCF8574T的引脚
#define PIN_LED 4
#define NOTE_B4 4110            // BB机的频率
#define NTP1 "ntp1.aliyun.com"  // 定义 NTP 服务器地址
#define NTP2 "ntp2.aliyun.com"
#define NTP3 "ntp3.aliyun.com"
const char* assid = "esp32c3时钟";    //热点名称
const char* apassword = "123456789";  //热点密码
String ssid;
String password;

String username;
String passwords;
int x = 0;
int y = 48;
const int SDA_PIN = 5;  //引脚连接
const int SDC_PIN = 4;
const uint8_t TM1637_CLK = 2;
const uint8_t TM1637_DIO = 3;
const int speakerPin = 18;  // 喇叭连接到引脚18
// 定义闹钟声音的音调和持续时间
const int alarmMelody[] = {
  NOTE_B4, NOTE_B4
};

const int noteDurations[] = {
  110, 110
};
bool lb = false;
bool ll = true;
DateTime dateTime;
int Connection_timing = 0;   //wifi连接后关闭WiFi连接的计时器
int Connection_timings = 0;  //闹钟被关闭后的计时器
int chun;
bool pw = true;        //配网第一次运行
int LiangDu = 0;       //时钟亮度
int ShouDianTong = 0;  //手电筒
char ymbx;             //页面位置保存
bool JiShi = false;    //确认时间是否被第一次获取
char xh;               //二级菜单序号
char ymr = 0;
int page = 3;        //控制初始化完成后的默认页面
int m = 128 * page;  //动画
char yml = 0;

int Third_level_page = 0;   //三级菜单序号
int Fourth_level_page = 0;  //四级菜单序号
bool hj = false;
bool Enter_page = false;
int firstRun = false;  //控制Enter_page第一次不运行

int i = 0;  //123
int h = 0;

int networkCount;  //wifi列表数量

int gpioy = 1;
int gpior = 0;
int gpios = 12;  //定义io口
int gpiof = 13;

int pin0;
int pin1;  //io口的存储
int pin18;
int pin13;


//闹钟变量
bool alarm_clock_switch = true;                   //闹钟开关
int clock1[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  //闹钟1
int clock2[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  //闹钟2
int clock3[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  //闹钟3
int clock4[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  //闹钟4
int clock5[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  //闹钟5





int sec = 0;     // 定义一个 int 变量来存储秒钟
int hour = 0;    // 定义一个 int 变量来存储小时
int minute = 0;  // 定义一个 int 变量来存储分钟
int year = 0;    // 定义一个 int 变量来存储年份
int month = 0;   // 定义一个 int 变量来存储月份
int day = 0;     // 定义一个 int 变量来存储日期
int weekt = 0;


int secl = 0;     // 定义一个 int 变量来存储秒钟
int hourl = 0;    // 定义一个 int 变量来存储小时
int minutel = 0;  // 定义一个 int 变量来存储分钟
int yearl = 0;    // 定义一个 int 变量来存储年份
int monthl = 0;   // 定义一个 int 变量来存储月份
int dayl = 0;     // 定义一个 int 变量来存储日期


/******************************************************************************/  //定义网页显示内容


String HTML = R"=====(
<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>esp32c3配网界面</title>
<style>
* {
    margin: 0;
    padding: 0;
}
h1{color: #C4C4C4;
    text-align: center;}
.dx {
    width: auto;
    height: 1200px;
    background: #2994E1
}
.bx {
    width: 600px;
    height: 1200px;
    background: #128FE9;
    margin: 0 auto;
}
.srk {
    height: 220px;
    width: 450px;
    border-radius: 12px;
    margin: 0 auto;
    background: #FFB600;
    color: #FFFFFF;
    font-size: 30px;
}
.srk label {
    margin-left: 10px;
}
.srk input {
    margin: 30px 10px 30px 0px;
}
.tj {
    height: 50px;
    width: 200px;
    border-radius: 5px;
    margin: 40px auto;
}
.tj input {
    background: #FF7E00;
    height: 50px;
    width: 200px;
    color: #FFFFFF;
    border: none;
    border-radius: 5px;
    font-size: 25px;
}
.tjhz {
    height: 50px;
    width: 200px;
}
.sr {
    border-radius: 5px;
    height: 40px;
    width: 250px;
    border: none;
}
</style>
</head>
<body>
<div class="dx">
  <div class="bx">
      <h1>输入网络名称和密码</h1>
    <form action= "/submit">
      <div class="srk">
        <label for="username"> wifi名称： </label>
        <input class="sr" type="text" id="username" name="username">
        <label for= "password"> wifi密码： </label>
        <input class="sr" type="password" id="password" name="password">
      </div>
      <div class="tj">
        <input  type="submit" value="提交 ">
      </div>
    </form>
  </div>
</div>
</body>
</html>
)=====";


String HTMLR = R"=====(

<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>esp32c3配网界面</title>
<style>
	h1{color: #C4C4C4;
	text-align: center;}
.dx{width: auto;height: 1200px;background: #2994E1}
.bx{width: 600px;height: 1200px;background: #128FE9;margin: 0 auto;}
.bx p{color: #FFFFFF;line-height: 50px;margin-left: 40px;font-size: 30px;}
</style>
</head>
<body>
<div class="dx">
<div class="bx">
<h1>提交完成</h1>
</div>
</div>
</body>
</html>
)=====";


/***********************************************************************************************************************/  // 初始化


IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

//char* sj[] ={0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,-,}


//U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, U8X8_PIN_NONE, SDC_PIN, SDA_PIN);//定义单屏幕
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2);  //屏幕时钟芯片
RTC_DS1307 RTC;
//#define OLED_I2C_ADDRESS 0x3C
TM1637 tm1637(TM1637_CLK, TM1637_DIO);

/********************************************************************************************************************/  // 显示的图片


// 设置图标
const unsigned char settingsIcon[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xC0, 0x03, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0xE3, 0xC7, 0x00, 0x80, 0xE7, 0xE7, 0x01,
  0xC0, 0xFF, 0xFF, 0x03, 0xC0, 0xFF, 0xFF, 0x03, 0x80, 0xFF, 0xFF, 0x01, 0x00, 0xFF, 0xFF, 0x00,
  0x00, 0x3F, 0xFC, 0x00, 0xE0, 0x1F, 0xF8, 0x07, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F,
  0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xE0, 0x1F, 0xF8, 0x07, 0x00, 0x3F, 0xFC, 0x00,
  0x00, 0xFF, 0xFF, 0x00, 0x80, 0xFF, 0xFF, 0x01, 0xC0, 0xFF, 0xFF, 0x03, 0xC0, 0xFF, 0xFF, 0x03,
  0x80, 0xE7, 0xE7, 0x01, 0x00, 0xE3, 0xC7, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0xC0, 0x03, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*"d:\Users\hejia\Desktop\123.bmp",0*/


};

const unsigned char settingsIconl[] PROGMEM = {

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xF8, 0x1F, 0x00, 0x00, 0xF0, 0x0F, 0x00, 0x00, 0xF0, 0x0F, 0x00, 0x00, 0xE0, 0x07, 0x00,
  0x00, 0xE0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x02, 0x00,
  0x00, 0x40, 0x02, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x40, 0x02, 0x00,
  0x00, 0x40, 0x02, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00,
  0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*"d:\Users\hejia\Desktop\112.bmp",0*/

};
const unsigned char settingsIcone[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x3F, 0x00, 0x80, 0xFF, 0xFF, 0x01,
  0xC0, 0xFF, 0xFF, 0x07, 0xF0, 0x07, 0xE0, 0x0F, 0xF8, 0x01, 0x80, 0x1F, 0x7C, 0xF0, 0x0F, 0x3E,
  0x1C, 0xFE, 0x7F, 0x38, 0x08, 0xFF, 0xFF, 0x10, 0xC0, 0x1F, 0xF8, 0x03, 0xC0, 0x07, 0xE0, 0x03,
  0xC0, 0xE1, 0x87, 0x03, 0x80, 0xF8, 0x1F, 0x01, 0x00, 0xFC, 0x3F, 0x00, 0x00, 0x7E, 0x7E, 0x00,
  0x00, 0x1C, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xE0, 0x07, 0x00,
  0x00, 0xE0, 0x07, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*"d:\Users\hejia\Desktop\544454.bmp",0*/
};
/****************************************************************/
// 函数：向PCF8574T芯片写入数据
void writePCF8574T(uint8_t pin, uint8_t value) {
  Wire.beginTransmission(PCF8574T_ADDRESS);      // 开始 I2C 传输，设置通信设备地址
  Wire.write(value ? (1 << pin) : ~(1 << pin));  // 通过位操作符设置引脚状态
  Wire.endTransmission();                        // 结束 I2C 传输，发送数据
}
/***********************************************************************************************************************/  //喇叭
void playAlarm() {
  for (int i = 0; i < sizeof(alarmMelody) / sizeof(alarmMelody[0]); i++) {
    int toneDuration = noteDurations[i];

    if (alarmMelody[i] == 0) {
      noTone(speakerPin);  // 停止发声
    } else {
      tone(speakerPin, alarmMelody[i]);  // 播放音调
    }

    delay(toneDuration);  // 持续时间
    noTone(speakerPin);   // 停止发声
    delay(50);            // 短暂的间隔
  }
}
void laba() {
  for (int i; i < 8; i++) {
    writePCF8574T(PIN_LED, LOW);
    playAlarm();
    writePCF8574T(PIN_LED, HIGH);
    delay(60);
    writePCF8574T(PIN_LED, LOW);
    playAlarm();
    writePCF8574T(PIN_LED, HIGH);
    delay(500);
  }


  delay(1000);
}
/**********************************************************************************************************************************************/  //网页
void handle_root() {
  server.send(200, "text/html", HTML);
}

void handle_submit() {
  username = server.arg("username");  //读取网页信息
  passwords = server.arg("password");
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 10);
    u8g2.print("用户名: " + username);
    u8g2.setCursor(0, 30);
    u8g2.print("密码: " + passwords);
  } while (u8g2.nextPage());
  // server.send(200, "text/plain", "数据已接收");
  int address2 = sizeof(username) + 4;  // 下一个变量的地址是上一个变量所占空间的大小
  // 将变量写入EEPROM
  EEPROM.put(4, username);
  EEPROM.put(address2, passwords);

  // 将修改保存到EEPROM
  EEPROM.commit();

  wifilj();  //连接网络
  server.send(200, "text/html", HTMLR);
}
void web() {
  WiFi.softAP(assid, apassword);                 // 创建一个软件接入点（SoftAP）
  WiFi.softAPConfig(local_ip, gateway, subnet);  // 配置接入点的 IP 地址、网关和子网掩码
  server.on("/", handle_root);                   // 处理根URL请求的回调函数为 handle_root
  server.on("/submit", handle_submit);           // 处理表单提交请求的回调函数为 handle_submit
  server.begin();                                // 启动HTTP服务器
  delay(100);
}

/*********************************************************************************************************************************************/
//WiFi获取时间
void setClock() {
  struct tm timeinfo;  // 定义一个 tm 结构体来存储时间信息
  if (WiFi.status() == WL_CONNECTED) {

    configTime(8 * 3600, 0, NTP1, NTP2, NTP3);  // 配置时间服务器并获取时间
  }
  if (getLocalTime(&timeinfo)) {
    // 打印当前时间
    //u8g2.print(timeinfo.tm_sec);
  } else {
    //u8g2.print("xxx");
  }

  year = timeinfo.tm_year + 1900;  // 获取年份并存储到 year 变量中
  month = timeinfo.tm_mon + 1;     // 获取月份并存储到 month 变量中
  day = timeinfo.tm_mday;          // 获取日期并存储到 day 变量中
  sec = timeinfo.tm_sec;           // 获取秒时并存储到 sec 变量中
  hour = timeinfo.tm_hour;         // 获取小时并存储到 hour 变量中
  minute = timeinfo.tm_min;        // 获取分钟并存储到 minute 变量中
  weekt = timeinfo.tm_wday;



  secl = dateTime.second(), DEC;
  hourl = dateTime.hour(), DEC;
  minutel = dateTime.minute(), DEC;
  yearl = dateTime.year(), DEC;
  monthl = dateTime.month(), DEC;
  dayl = dateTime.day(), DEC;

  if (WiFi.status() == WL_CONNECTED) {
    if (year != yearl || monthl != month || day != dayl || hour != hourl || minute != minutel || JiShi != true) {
      RTC.adjust(DateTime(year, month, day, hour, minute, sec));  //设置初始时间
    }
  }

  JiShi = true;  //获取到时间后记录已获取
}
/******************************************

//获取附近2.4GWiFi列表
void wifihuoqv() {

  WiFi.mode(WIFI_MODE_NULL);
  networkCount = WiFi.scanNetworks();

  for (int ib = 0; ib < networkCount; ++ib) {

    //u8g2.setCursor(0, 5);

    // 打印当前网络的编号
    u8g2.setCursor(0, (ib + 1) * 16);
    u8g2.print(ib + 1);
    u8g2.print(WiFi.SSID(ib));  // 打印当前网络的名称

    delay(10);  // 延迟10ms，防止单次串口数据量过大导致丢数据
  }
}
***********************************************/
//连接WiFi

void wifilj() {
  int js = 5;

  EEPROM.get(4, ssid);
  EEPROM.get(sizeof(ssid) + 4, password);
  u8g2.firstPage();
  u8g2.sendBuffer();
  u8g2.setCursor(0, 16);
  u8g2.print("wifi: ");
  u8g2.setCursor(0, 33);
  u8g2.print(ssid);
  u8g2.setCursor(0, 50);
  u8g2.print("密码: ");
  u8g2.setCursor(0, 64);
  u8g2.print(password);
  u8g2.sendBuffer();
  delay(1500);


  u8g2.firstPage();
  u8g2.sendBuffer();
  u8g2.setCursor(0, 20);
  u8g2.print("连接中...");
  u8g2.sendBuffer();  // 在串口上打印连接WiFi的信息
  delay(500);
  u8g2.firstPage();
  u8g2.sendBuffer();
  WiFi.begin(ssid, password);  // 连接WiFi网络

  while (WiFi.status() != WL_CONNECTED && js > 0) {  // 等待WiFi连接状态

    u8g2.setCursor(0, 18);
    u8g2.print("连接中...");
    u8g2.setCursor(0, 32);
    u8g2.print(js);
    u8g2.sendBuffer();
    delay(400);
    u8g2.firstPage();


    js--;
  }

  u8g2.setCursor(3, 16);

  u8g2.print("WIFI名称:");
  u8g2.print(ssid);  // 在串口上打印成功连接WiFi的信息
  u8g2.setCursor(3, 32);
  u8g2.print("密码:");
  u8g2.print(password);  // 在串口上打印IP地址信息
  u8g2.setCursor(3, 48);
  u8g2.print("IP地址:");

  if (WiFi.status() == WL_CONNECTED) {
    u8g2.setCursor(3, 64);
    u8g2.print(WiFi.localIP());  // 获取并输出ESP32-C3的IP地址信息
    WiFi.mode(WIFI_STA);
    server.stop();   // 停止接受新请求
    server.close();  // 断开与客户端的连接
  } else {
    u8g2.print("连接失败");
  }

  u8g2.sendBuffer();
  u8g2.firstPage();

  delay(500);
}
/**************************************************************************************************************************************/  //按键检测
void anjian() {
  pin0 = digitalRead(gpioy);
  pin1 = digitalRead(gpior);
  pin18 = digitalRead(gpios);
  pin13 = digitalRead(gpiof);
  if (yml >= 0) {

    if (pin0 == LOW) {
      page++;
      delay(200);  //上
    }
    if (pin1 == LOW) {
      page--;
      delay(200);  //下
    }
    if (pin18 == LOW) {
      yml++;
      delay(200);  //中
    }
  }
  if (pin13 == LOW) {
    if (yml > 0 && !Enter_page) {
      yml--;
    } else if (yml <= -1) {  //返回键
      yml = 0;
    }

    delay(200);
  }

  if (page < 0 || page > 3) {
    if (page < 0) {
      page = 3;
    } else if (page > 3) {
      page = 0;
    }
  }
}
/**************************************************************************************************************************************/  //一级页面
void xs() {
  u8g2.firstPage();

  u8g2.setFont(u8g2_font_7_Seg_33x19_mn);
  u8g2.setFont(u8g2_font_shizhong_32);
  u8g2.setCursor(-128 + 10 - m, 30);
  u8g2.print(year);
  u8g2.print("年");
  u8g2.setCursor(-128 + 5 - m, 60);
  u8g2.print(month);
  u8g2.print("月");
  u8g2.print(day);  //界面3
  u8g2.print("日");
  u8g2.setFont(u8g2_font_wqy16_t_gb2312);





  u8g2.setCursor(48 - m, 60);
  u8g2.print("设置");
  u8g2.drawXBMP(48 - m, 8, 32, 32, settingsIcon);


  u8g2.setCursor(168 - m, 60);
  u8g2.print("手电筒");  //界面1
  u8g2.drawXBMP(176 - m, 8, 32, 32, settingsIconl);



  u8g2.setCursor(304 - m, 60);
  u8g2.print("WIFI");  //界面2
  u8g2.drawXBMP(304 - m, 8, 32, 32, settingsIcone);


  u8g2.setFont(u8g2_font_7_Seg_33x19_mn);
  u8g2.setFont(u8g2_font_shizhong_32);
  u8g2.setCursor(394 - m, 30);
  u8g2.print(year);
  u8g2.print("年");
  u8g2.setCursor(389 - m, 60);
  u8g2.print(month);
  u8g2.print("月");
  u8g2.print(day);  //界面3
  u8g2.print("日");
  u8g2.setFont(u8g2_font_wqy16_t_gb2312);





  u8g2.setCursor(512 + 48 - m, 60);
  u8g2.print("设置");
  u8g2.drawXBMP(512 + 48 - m, 8, 32, 32, settingsIcon);

  u8g2.sendBuffer();
}
void First_level_page() {
  if (page == 0) {
    x = 0;
  } else if (page == 1) {
    x = 128;
  } else if (page == 2) {
    x = 256;
  } else if (page == 3) {
    x = 384;
  }
  for (; m != x;) {
    if (m < x) {
      if (x - m > 128) {
        for (; m != -128;) {
          m = m - 16;
          xs();
        }
        m = 384;
        break;
      } else {
        m = m + 16;
      }
    } else if (m > x) {

      if (m - x > 128) {
        for (; m != 512;) {
          m = m + 16;
          xs();
        }
        m = 0;
        break;
      } else {
        m = m - 16;
      }
    }

    xs();
  }
  if (m == x) {
    if (page == 0) {
      u8g2.setCursor(48, 60);
      u8g2.print("设置");

      u8g2.drawXBMP(48, 8, 32, 32, settingsIcon);
    } else if (page == 1) {
      u8g2.setCursor(40, 60);
      u8g2.print("手电筒");  //界面1
      u8g2.drawXBMP(48, 8, 32, 32, settingsIconl);
    } else if (page == 2) {
      u8g2.setCursor(48, 60);
      u8g2.print("WIFI");  //界面2
      u8g2.drawXBMP(48, 8, 32, 32, settingsIcone);
    } else if (page == 3) {
      u8g2.setFont(u8g2_font_7_Seg_33x19_mn);
      u8g2.setFont(u8g2_font_shizhong_32);
      u8g2.setCursor(10, 30);

      u8g2.print(year);
      u8g2.print("年");
      u8g2.setCursor(5, 60);
      u8g2.print(month);
      u8g2.print("月");

      u8g2.print(day);  //界面3
      u8g2.print("日");
    }
  }
}
/********************************************************************************************************************************************/  //读取写入闹钟数据
void alarm_clock_put() {

  EEPROM.put(512, clock1);
  EEPROM.put(552, clock2);
  EEPROM.put(592, clock3);
  EEPROM.put(632, clock4);
  EEPROM.put(672, clock5);
  // 将修改保存到EEPROM
  EEPROM.commit();
}
void alarm_clock_get() {



  EEPROM.get(512, clock1);
  EEPROM.get(552, clock2);
  EEPROM.get(592, clock3);
  EEPROM.get(632, clock4);
  EEPROM.get(672, clock5);
}


/*****************************************/
//闹钟列表显示
void Alarm_display() {
  int* currentClock;
  switch (Third_level_page) {
    case 0:
      currentClock = clock1;
      break;
    case 1:
      currentClock = clock2;
      break;
    case 2:
      currentClock = clock3;
      break;
    case 3:
      currentClock = clock4;
      break;
    case 4:
      currentClock = clock5;
      break;
    default:
      // 处理默认情况，如果需要的话
      return;
  }

  u8g2.setCursor(45, 14);
  u8g2.print("闹钟");

  u8g2.print(Third_level_page + 1);
  u8g2.setCursor(20, 14);
  if (currentClock[0] == 1) {
    u8g2.print("off");
  } else {
    u8g2.print("no");
  }
  // 闹钟界面
  u8g2.setCursor(5, 32);
  if (currentClock[1] / 10 < 1) {
    u8g2.print("0");
  }

  u8g2.print(currentClock[1]);
  u8g2.setCursor(22, 32);
  u8g2.print("时");
  u8g2.setCursor(52, 32);
  if (currentClock[2] / 10 < 1) {
    u8g2.print("0");
  }
  u8g2.print(currentClock[2]);
  u8g2.setCursor(70, 32);
  u8g2.print("分");
  
u8g2.setCursor(90, 37);
 u8g2.print("星期:");
  for (int i = 3; i <= 9; i++) {
    u8g2.setCursor((i - 3) * 15 + 7, 60);
    if (currentClock[i] == 1) {
      u8g2.print(i - 3);
    } else {
      u8g2.drawFrame((i - 3) * 15 + 6, 47, 11, 15);
      u8g2.print(i - 3);
    }
  }
}
/*******************************************************/


//闹钟列表调整
void Alarm_list_adjustment() {

  if (Fourth_level_page == 0) {
    u8g2.drawFrame(15, 0, 30, 18);
  }
  if (Fourth_level_page == 1) {
    u8g2.drawFrame(1, 16, 40, 20);
  }
  if (Fourth_level_page == 2) {
    u8g2.drawFrame(48, 16, 40, 20);
  }
  if (Fourth_level_page == 3) {
    u8g2.drawFrame(4, 45, 15, 19);
  }
  if (Fourth_level_page == 4) {
    u8g2.drawFrame(19, 45, 15, 19);
  }
  if (Fourth_level_page == 5) {
    u8g2.drawFrame(34, 45, 15, 19);
  }
  if (Fourth_level_page == 6) {
    u8g2.drawFrame(49, 45, 15, 19);
  }
  if (Fourth_level_page == 7) {
    u8g2.drawFrame(64, 45, 15, 19);
  }
  if (Fourth_level_page == 8) {
    u8g2.drawFrame(79, 45, 15, 19);
  }
  if (Fourth_level_page == 9) {
    u8g2.drawFrame(94, 45, 15, 19);
  }
}
/*******************************************************************************************************************************************/
void Alarm_reminder() {
  if (!alarm_clock_switch) {
    if (Connection_timings >= 1550) {

      alarm_clock_switch = true;

      Connection_timings = 0;
    } else {
      Connection_timings++;
    }
  }


  int* clocks[] = { clock1, clock2, clock3, clock4, clock5 };  // 将数组声明修改为包含正确的初始化项
  int numClocks = sizeof(clocks) / sizeof(clocks[0]);
  ll = true;
  for (int i = 0; i < numClocks; i++) {





    if (clocks[i][1] == hour && clocks[i][2] == minute && clocks[i][0] == 0 && clocks[i][weekt + 3] == 0) {



      ll = false;
      if (pin0 == LOW || pin1 == LOW || pin18 == LOW || pin13 == LOW) {
        alarm_clock_switch = false;
        lb = false;
        // 在条件满足时跳出循环
      }

      if (alarm_clock_switch) {
        tm1637.display(1, 8);  //第一位
        tm1637.display(2, 8);  //第二位
        tm1637.display(0, 8);  //第三位
        tm1637.display(3, 8);  //第四位
        tm1637.point(1);       //分号

        // 执行相应操作
        lb = true;
        break;
      }
    }
  }
  if (ll) {


    lb = false;
    alarm_clock_switch = true;
    Connection_timings = 0;
  }

  // 将alarm_clock_switch的值设置为true移至for循环之外
  // 遍历完所有时钟后，如果没有满足条件的，则将其设置为true
}
/*******************************************************************************************************************************************/

void setup() {
  Wire.begin(SDA_PIN, SDC_PIN);  //初始化iic

  RTC.begin();  // 初始化时钟

  u8g2.begin();  // 初始化显示屏

  Serial.begin(115200);  //初始化串口

  WiFi.mode(WIFI_STA);          // 设置 ESP32 工作模式为无线终端模式
  pinMode(speakerPin, OUTPUT);  // 将引脚设为输出模式
  pinMode(gpioy, INPUT_PULLUP);
  pinMode(gpior, INPUT_PULLUP);  // 配置输入引脚并设置为上拉
  pinMode(gpios, INPUT_PULLUP);


  writePCF8574T(PIN_LED, HIGH);  //控制扩展io芯片4脚上拉以保证喇叭处于关闭状态

  tm1637.init();
  tm1637.set(0.2);  //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;亮度设置

  // 设定字体
  u8g2.enableUTF8Print();  // 启用UTF-8编码
  //configTime(8 * 3600, 0, NTP1, NTP2, NTP3);  // 配置时间服务器并获取时间
  //setClock();  // 调用 setClock 函数来设置时间
  EEPROM.begin(1024);  //长久存储初始化
  EEPROM.get(0, chun);
  if (chun != 1) {
    chun = 1;
    username = "--------";
    passwords = "--------";
    EEPROM.put(0, chun);
    EEPROM.put(4, username);
    EEPROM.put(sizeof(username) + 4, passwords);
    EEPROM.put(512, clock1);
    EEPROM.put(552, clock2);
    EEPROM.put(592, clock3);
    EEPROM.put(632, clock4);
    EEPROM.put(672, clock5);
    // 将修改保存到EEPROM
    EEPROM.commit();
  }
  // 设置中文字体
  u8g2.setFont(u8g2_font_wqy16_t_gb2312);

  //u8g2.setFont(u8g2_font_shizhong);

  // 清空显示屏并准备显示下一帧内容

  wifilj();  //连接网络
  alarm_clock_get();
}


/**************************************************************************************************************************************/



void loop() {
  dateTime = RTC.now();
  // 读取GPIO引脚状态


  // 设置中文字体
  u8g2.setFont(u8g2_font_wqy16_t_gb2312);

  //u8g2.setFont(u8g2_font_shizhong);
  u8g2.firstPage();  // 清空显示屏并准备显示下一帧内容




  /***************************************************/  //按键
  anjian();

  /**************************************************/
  // 绘制图形、文字等
  ShouDianTong = 0;  //判断手电筒是否开启

  do {
    // 绘制图片
    //u8g2.setDisplayRotation(U8G2_R2);




    if (yml == 0) {
      /*******************************************************************************/
      if (i == 1) {

        page = ymbx;
        i = 0;
      }
      First_level_page();  //显示一级页面

      u8g2.setFont(u8g2_font_wqy16_t_gb2312);


      if (sec == 59) {
        sec = 0;
      } else {  //降低误差
        sec++;
      }
      /*
      u8g2.setCursor(95, 16);
      u8g2.print(Connection_timings);
      u8g2.setCursor(110, 32);*/
      u8g2.setCursor(110, 16);
      u8g2.print(sec);  //显示秒数
      ymr = page;


      /*****************************************************************************************/

    } else if (yml >= 1) {

      if (i == 0) {

        page = 0;
        i = 1;
      }
      ymbx = ymr;
      /********************************************************************************/
      if (ymr == 0) {
        if (yml == 2 && xh == 0) {

          if (pin1 == LOW) {
            LiangDu++;
            //上
          } else if (pin0 == LOW) {
            LiangDu--;
            //下
          }


          if (LiangDu < 0 || LiangDu > 7) {
            if (LiangDu < 0) {
              LiangDu = 7;
            } else {
              LiangDu = 0;
            }
          }
          u8g2.drawFrame(0, 0, 128, 18);
          u8g2.drawBox(0, 0, 18.3 * LiangDu, 18);  //亮度设置百分比进度条
          u8g2.setCursor(60, 55);
          u8g2.print(LiangDu);
          tm1637.set(LiangDu);
          page = 0;

        } else if (yml == 2 && xh == 1) {

          u8g2.setCursor(0, 16);
          u8g2.print("请进入网页配置网络");
          u8g2.sendBuffer();
          WiFi.disconnect(true);  //重新开启配网模式（断开wifi连接）

          web();
          delay(1000);
          yml = 1;

        } else if (yml == 2 && xh == 2) {  //闹钟页面
          if (pin13 == LOW) {
            if (firstRun) {
              firstRun = false;
            } else {
              Enter_page = false;
              alarm_clock_put();
            }
            hj = false;
          }



          if (Enter_page && !firstRun) {
            if (pin0 == LOW) {
              Fourth_level_page++;
              //上
            } else if (pin1 == LOW) {
              Fourth_level_page--;
              //下
            }


            if (Fourth_level_page < 0 || Fourth_level_page > 9) {
              if (Fourth_level_page < 0) {
                Fourth_level_page = 9;
              } else {
                Fourth_level_page = 0;
              }
            }



            Alarm_list_adjustment();
          } else if (Enter_page && firstRun) {
            Alarm_list_adjustment();
            int* currentClocksl;
            switch (Third_level_page) {
              case 0:
                currentClocksl = clock1;
                break;
              case 1:
                currentClocksl = clock2;
                break;
              case 2:
                currentClocksl = clock3;
                break;
              case 3:
                currentClocksl = clock4;
                break;
              case 4:
                currentClocksl = clock5;
                break;
              default:
                // 处理默认情况，如果需要的话
                return;
            }
            if (Fourth_level_page == 1) {
              if (pin0 == LOW) {
                currentClocksl[1]++;
                if (currentClocksl[1] > 23) {
                  currentClocksl[1] = 0;
                }
                //上
              } else if (pin1 == LOW) {
                currentClocksl[1]--;
                if (currentClocksl[1] < 0) {
                  currentClocksl[1] = 23;
                }
                //下
              }
            }
            if (Fourth_level_page == 2) {
              if (pin0 == LOW) {
                currentClocksl[2]++;
                if (currentClocksl[2] > 59) {
                  currentClocksl[2] = 0;
                }
                //上
              } else if (pin1 == LOW) {
                currentClocksl[2]--;
                if (currentClocksl[2] < 0) {
                  currentClocksl[2] = 59;
                }
                //下
              }
            }
            if (Fourth_level_page == 1) {
              u8g2.drawFrame(0, 15, 42, 22);
            }
            if (Fourth_level_page == 2) {
              u8g2.drawFrame(47, 15, 42, 22);
            }


          } else {
            Fourth_level_page = 0;




            if (pin0 == LOW) {
              Third_level_page++;
              //上
            } else if (pin1 == LOW) {
              Third_level_page--;
              //下
            }


            if (Third_level_page < 0 || Third_level_page > 4) {
              if (Third_level_page < 0) {
                Third_level_page = 4;
              } else {
                Third_level_page = 0;
              }
            }
          }

          Alarm_display();  //闹钟显示内容
                            //第一次进入不修改内容




          page = 2;

        } else if (yml > 2) {
          if (xh == 2) {
            int* currentClocks;
            switch (Third_level_page) {
              case 0:
                currentClocks = clock1;
                break;
              case 1:
                currentClocks = clock2;
                break;
              case 2:
                currentClocks = clock3;
                break;
              case 3:
                currentClocks = clock4;
                break;
              case 4:
                currentClocks = clock5;
                break;
              default:
                // 处理默认情况，如果需要的话
                return;
            }
            if (hj) {
              for (int i = 0; i <= 9; i++) {
                if (Fourth_level_page == i && i != 1 && i != 2) {


                  if (currentClocks[i] == 1) {
                    currentClocks[i] = 0;
                  } else {
                    currentClocks[i] = 1;
                  }
                } /*else if (Fourth_level_page == i&&i==1){
                if (currentClocks[i] == 1) {
                  currentClocks[i]++;
                } else {
                  currentClocks[i] ++;
                }
              }*/
              }
            }
            hj = true;
          }
          yml = 2;
          if (!Enter_page && (Fourth_level_page != 1 || Fourth_level_page != 2)) {
            Enter_page = true;
          } else if (Fourth_level_page == 1 || Fourth_level_page == 2) {
            firstRun = true;
          }

        }

        else {
          u8g2.setCursor(5, 16);
          u8g2.print("亮度");  //亮度设置
          u8g2.setCursor(5, 37);
          u8g2.print("重新配置网络");  //重新选择网络
          u8g2.setCursor(5, 57);
          u8g2.print("闹钟");  //闹钟
          if (page > 2 && pin1 == LOW) {
            page = 2;
          } else if (page > 2) {
            page = 0;
          }
          if (page == 0) {


            u8g2.drawFrame(0, 0, 128, 20);
            xh = 0;
          } else if (page == 1) {

            u8g2.drawFrame(0, 21, 128, 20);
            xh = 1;
          } else if (page == 2) {

            u8g2.drawFrame(0, 41, 128, 20);
            xh = 2;
          }
        }


      }
      /*************************************************************************************/  //设置界面
      else if (ymr == 1) {


        if (yml == 1) {
          u8g2.drawXBMP(48, 0, 32, 32, settingsIconl);
          u8g2.drawXBMP(48, 32, 32, 32, settingsIconl);
          tm1637.set(7);
          tm1637.display(1, 8);  //第一位
          tm1637.display(2, 8);  //第二位
          tm1637.display(0, 8);  //第三位
          tm1637.display(3, 8);  //第四位
          tm1637.point(1);       //分号
          ShouDianTong = 1;


        } else if (yml > 1) {
          yml = 1;
        }





      }
      /*************************************************************************************/  //手电筒界面
      else if (ymr == 2) {
        if (yml == 2 && xh == 0) {
          u8g2.setCursor(3, 16);

          u8g2.print("WIFI名称:");
          u8g2.print(ssid);  // 在串口上打印成功连接WiFi的信息
          u8g2.setCursor(3, 34);
          u8g2.print("密码:");
          u8g2.print(password);  // 在串口上打印IP地址信息
          u8g2.setCursor(3, 52);
          u8g2.print("IP地址:");


          if (WiFi.status() == WL_CONNECTED) {
            u8g2.setCursor(3, 64);
            u8g2.print(WiFi.localIP());  // 获取并输出ESP32-C3的IP地址信息
          } else {
            u8g2.print("连接失败");
          }

          page = 0;

        } else if (yml == 2 && xh == 1) {
          wifilj();  //重新连接网络
          yml = 1;

        } else if (yml > 2) {
          yml = 2;
        }

        else {

          WiFi.mode(WIFI_STA);
          u8g2.setCursor(3, 16);

          u8g2.print("WIFI");





          if (WiFi.status() == WL_CONNECTED) {

          } else {
            u8g2.print("连接失败");
            u8g2.setCursor(3, 36);
            u8g2.print("重新连接:");
          }


          if (page == 0) {
            xh = 0;

            u8g2.drawFrame(0, 0, 128, 20);
          } else if (page == 1 || WiFi.status() != WL_CONNECTED) {
            xh = 1;

            u8g2.drawFrame(0, 20, 128, 20);
          } else if (page == 1 || WiFi.status() == WL_CONNECTED) {

            page = 0;
          }
        }



      }
      /***************************************************************************************/  //WiFi界面
      else if (ymr == 3) {
        if (yml >= 2) {

          yml = 1;
        }
        u8g2.setCursor(3, 16);

        u8g2.print(year);
        u8g2.print("年");
        u8g2.setCursor(3, 32);
        u8g2.print(month);
        u8g2.print("月");
        u8g2.setCursor(3, 48);
        u8g2.print(day);
        u8g2.print("日");

        u8g2.print(weekt);
      }
      /*******************************************************************************************/  //日期界面
    }
    /***************************************************************************/




  } while (u8g2.nextPage());
  /******************************************************************************************************/  //数码管显示

  if (ShouDianTong == 0) {  //判断手电筒开启

    tm1637.set(LiangDu);  //设置时钟亮度
    if (WiFi.status() == WL_CONNECTED || JiShi == true) {
      setClock();  //确定网络是否连接是则使用网络获取时间数据
    } else {       //利用芯片获取时间数据
      sec = dateTime.second(), DEC;
      hour = dateTime.hour(), DEC;
      minute = dateTime.minute(), DEC;
      year = dateTime.year(), DEC;
      month = dateTime.month(), DEC;
      day = dateTime.day(), DEC;
      weekt = dateTime.dayOfTheWeek(), DEC;
    }

    if (sec % 2 == 0) {
      tm1637.point(1);

    } else {  //控制分号闪烁
      tm1637.point(0);
    }

    if (h > 8) {
      h = 0;
    }
    tm1637.display(0, hour / 10);    //第一位
    tm1637.display(1, hour % 10);    //第二位
    tm1637.display(2, minute / 10);  //第三位
    tm1637.display(3, minute % 10);  //第四位
  }
  if (WiFi.status() != WL_CONNECTED) {
    if (pw) {
      web();
      pw = false;
    }
    server.handleClient();  // 处理HTTP客户端的请求
  } else {
    if (Connection_timing >= 1500) {

      WiFi.disconnect(true);

      Connection_timing = 0;
    } else {
      Connection_timing++;
    }
  }



  Alarm_reminder();
  if (lb) {
    for (int i; i < 2; i++) {
      writePCF8574T(PIN_LED, LOW);
      playAlarm();
      writePCF8574T(PIN_LED, HIGH);
      delay(60);
      writePCF8574T(PIN_LED, LOW);
      playAlarm();
      writePCF8574T(PIN_LED, HIGH);
      delay(500);
    }


    delay(400);
  }
}
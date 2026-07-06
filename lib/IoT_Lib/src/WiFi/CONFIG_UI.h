#ifndef WEBUI_HPP
#define WEBUI_HPP
#include <Arduino.h>
class WebUI
{
public:
    static String MQTTConfigPage(
        const String &wifi,
        const String &user,
        const String &pass)
    {
        String s;
        s.reserve(1800);

        s =
            F("<!doctype html><html>"
              "<head>"
              "<meta charset=utf-8>"
              "<meta name=viewport content='width=device-width'>"
              "<title>MQTT</title>"
              "<style>"
              "body{font:14px sans-serif;background:#f4f6f8;margin:0}"
              ".c{max-width:360px;margin:30px auto}"
              ".b{background:#fff;padding:20px;border-radius:12px;"
              "box-shadow:0 3px 12px #0001}"
              "h3{text-align:center;margin:0 0 15px}"
              ".i{background:#eef8f3;padding:10px;"
              "border-radius:8px;margin-bottom:15px}"
              "input{width:100%;padding:10px;"
              "margin:6px 0 12px;border:1px solid #ddd;"
              "border-radius:8px;box-sizing:border-box}"
              ".p{position:relative}"
              ".e{position:absolute;right:10px;"
              "top:12px;cursor:pointer}"
              "button{width:100%;padding:11px;"
              "background:#43b581;color:#fff;"
              "border:0;border-radius:8px}"
              "</style>"
              "</head>"
              "<body><div class=c><div class=b>"
              "<h3>MQTT CONFIG</h3>"
              "<div class=i>WiFi:<br><b>");
        s += wifi;
        s += F(
            "</b></div>"
            "<form method=POST action=/save>"
            "User"
            "<input name=mqtt_user value='");
        s += user;
        s += F(
            "'>"
            "Password"
            "<div class=p>"
            "<input id=p type=password "
            "name=mqtt_pass value='");
        s += pass;
        s += F(
            "'>"
            "<span class=e onclick='t()'>👁</span>"
            "</div>"
            "<button>RECONNECT</button>"
            "</form>"
            "</div></div>"
            "<script>"
            "function t(){"
            "p.type="
            "p.type=='password'?"
            "'text':'password'"
            "}"
            "</script>"
            "</body></html>");
        return s;
    }
};

#endif

#ifndef WEBUI_HPP
#define WEBUI_HPP

#include <Arduino.h>

class WebUI
{
public:
    static const char WebConfigHEAD[];
    static const char WebConfigFOOT[];
    static const char WebConfigCONNECT[];
    static String buildWiFiOption(const String &ssid, int rssi);
    static int getSignalLevel(int rssi);

    static String MQTTConfigPage(const String &wifi, const String &user, const String &pass);
};

String WebUI::MQTTConfigPage(const String &wifi, const String &user, const String &pass)
{
    String html = R"rawliteral(
<!DOCTYPE html>
<html lang="vi">

<head>

    <meta charset="UTF-8">

    <meta name="viewport" content="width=device-width, initial-scale=1.0">

    <title>MQTT CONFIG</title>

    <style>
        :root {
            --primary: #4CAF93;
            --bg: #f5f7f9;
            --card: #ffffff;
            --text: #2c3e50;
            --border: #e0e6ed;
        }

        body {
            font-family: "Segoe UI";
            background: var(--bg);
            padding: 20px;
            margin: 0;
        }

        .container {
            max-width: 420px;
            margin: auto;
        }

        .card {
            background: white;
            padding: 25px;
            border-radius: 14px;

            box-shadow:
                0 6px 20px rgba(0, 0, 0, .05);
        }

        h2 {
            text-align: center;
        }

        .info {

            background: #f1f8f5;

            padding: 12px;

            border-radius: 10px;

            margin-bottom: 20px;

        }

        label {
            display: block;

            margin-top: 12px;
        }

        input {

            width: 100%;

            padding: 12px;

            border-radius: 10px;

            border:
                1px solid var(--border);

            box-sizing: border-box;

            margin-top: 8px;

        }

        .password-box {

            position: relative;

        }

        .password-box input {

            padding-right: 45px;

        }

        .toggle-eye {

            position: absolute;

            right: 14px;

            top: 50%;

            transform: translateY(-50%);

            cursor: pointer;

        }

        button {

            width: 100%;

            padding: 12px;

            margin-top: 20px;

            border: none;

            border-radius: 10px;

            background: var(--primary);

            color: white;

            font-size: 15px;

        }
    </style>

</head>

<body>

    <div class='container'>

        <div class='card'>

            <h2>MQTT RECOVERY</h2>

            <div class='info'>

                Connected WiFi:
                <br>

                <strong>
)rawliteral";

    html += wifi;

    html += R"rawliteral(
</strong>

</div>

<form action="/save" method="POST">

    <label>

        MQTT USER

    </label>

    <input type="text" name="mqtt_user" value="
)rawliteral";

    html += user;

    html += R"rawliteral(
">
<label>

    MQTT PASSWORD

</label>

<div class="password-box">

    <input id="mqttPass" type="password" name="mqtt_pass" value="

)rawliteral";

    html += pass;

    html += R"rawliteral(
">
<span class="toggle-eye" onclick="togglePassword()">

    👁

</span>

</div>

<button>

    RECONNECT

</button>

</form>

</div>

</div>

<script>

    function togglePassword() {

        let x =
            document.getElementById(
                "mqttPass");

        if (
            x.type
            ===
            "password"
        )
            x.type = "text";

        else
            x.type = "password";

    }

</script>

</body>

</html>

)rawliteral";
    return html;
}

#endif
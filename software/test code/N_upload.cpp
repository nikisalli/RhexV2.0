#include "N_upload.h"
#include "N_defines.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

WebServer server(80);

const char* host = OTA_WIFI_HOST;
const char* ssid = OTA_WIFI_SSID;
const char* password = OTA_WIFI_PWD;
long t_start;
long t_stop;
/* Style */
String style =
  "<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
  "input{background:#f1f1f1;border:0;padding:0 15px}body{background:#555;font-family:sans-serif;font-size:14px;color:#777}"
  "#file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
  "#bar,#prgbar{background-color:#ccc;border-radius:10px}#bar{background-color:#555;width:0%;height:10px}"
  "form{background:#fff;max-width:258px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
  ".btn{background:#555;color:#fff;cursor:pointer}</style>";

/* Login page */
String loginIndex =
  "<form name=loginForm>"
  "<h1>Nightmare V2.0</h1>"
  "<input name=userid placeholder='User ID'> "
  "<input name=pwd placeholder=Password type=Password> "
  "<input type=submit onclick=check(this.form) class=btn value=Login></form>"
  "<script>"
  "function check(form) {"
  "if(form.userid.value=='nik' && form.pwd.value=='motorino11')"
  "{window.open('/serverIndex')}"
  "else"
  "{alert('Error Password or Username')}"
  "}"
  "</script>" + style;

/* Server Index Page */
String serverIndex =
  "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
  "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
  "<input type='file' name='update' id='file' onchange='sub(this)' style=display:none>"
  "<label id='file-input' for='file'>   Choose file...</label>"
  "<input type='submit' class=btn value='Update'>"
  "<br><br>"
  "<div id='prg'></div>"
  "<br><div id='prgbar'><div id='bar'></div></div><br></form>"
  "<script>"
  "function sub(obj){"
  "var fileName = obj.value.split('\\\\');"
  "document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
  "};"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  "$.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "$('#bar').css('width',Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!') "
  "},"
  "error: function (a, b, c) {"
  "}"
  "});"
  "});"
  "</script>" + style;

/*
  setup function
*/

TaskHandle_t Task1;

double calcSpeed(unsigned long ms, size_t len) {
  return (double)(len * 125) / (double)(ms * 16);
}

void Task1code( void * parameter) {
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = 100;
  xLastWakeTime = xTaskGetTickCount();
  
  // Connect to WiFi network
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(1);
  }

  digitalWrite(4, HIGH);

  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://esp32.local
    while (1) {
      vTaskDelay(100);
    }
  }
  /*return index page which is stored in serverIndex */
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Update.begin(UPDATE_SIZE_UNKNOWN); //start with max available size
      t_start = millis();
      Serial.printf("Upload: %s\n", upload.filename.c_str());
    }
    else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      Update.write(upload.buf, upload.currentSize);
    }
    else if (upload.status == UPLOAD_FILE_END) {
      t_stop = millis();
      Serial.print("Time UPLOAD: "); Serial.print((t_stop - t_start) / 1000.0); Serial.println(" sec.");
      Serial.print("Speed UPLOAD: "); Serial.print(calcSpeed(t_stop - t_start, upload.totalSize)); Serial.println(" Kbit/s");
      Serial.printf("Upload Success: %u bytes\n", upload.totalSize);
      Update.end(true);//true to set the size to the current progress
    }
  });
  server.begin();
  for (;;) {
    server.handleClient();
    vTaskDelayUntil( &xLastWakeTime, xFrequency );
  }
}

void esp32server_setup() {
  xTaskCreatePinnedToCore(Task1code, "Task1", 50000, NULL, 1, &Task1, 0);
}

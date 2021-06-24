#include<ESP8266WiFi.h>
#include<WiFiManager.h>
#include<ESP8266WebServer.h>
#include<FS.h>

ESP8266WebServer server(80);
WiFiManager wifimanager;

void returnOK() {
  server.send(200, "text/plain", "");
}

void handleOtherFiles(){
  if(checkInFS(server.uri())){
    return;
  }
  server.send(404,"text.plain", "please restart the module");
  
}

void returnFail(String msg) {
  server.send(500, "text/plain", msg + "\r\n");
}

 bool checkInFS(String path){
  String dataType = "text/plain";
  if(path.endsWith("/") || path.endsWith(".html")){
    dataType = "text/html";
    path = "/index.html";
  }else if(path.endsWith(".css")){
    dataType = "text/css";
    path = "/styles.css";
  }else if(path.endsWith(".js")){
    dataType = "application/js";
    path = "/script.js";
  }else{
    dataType = "image/"+path.substring(path.lastIndexOf("."));
  }
  
  File file = SPIFFS.open(path, "r");
  if(!file)
    return false;
  server.streamFile(file, dataType);
  file.close();
  return true;
}


File uploadFile;

void handleFileUpload() {
  if (server.args() == 0) {
    return returnFail("BAD ARGS");
  }
  String upName;
  
  HTTPUpload& upload = server.upload();

  if (upload.status == UPLOAD_FILE_START) {
    upName = upload.filename;
    String extension = upName.substring(upName.lastIndexOf('.'));
    upName = upName.substring(0,upName.lastIndexOf('.'));
    if(upName.length() > 8) upName = (upName.substring(0,8) + extension); else upName =  upload.filename;
    String fileName = "/"+upName;
    if (SPIFFS.exists(fileName) ){
      SPIFFS.remove(fileName);
    }
    Serial.println("FileName: "+upName);
    uploadFile = SPIFFS.open(fileName, "w");
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (uploadFile) {
      uploadFile.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    upName = upload.filename;
    if (uploadFile) {
      uploadFile.close();
    }
    Serial.println("FileName: "+upName);
  }
}

void setup() {
  Serial.begin(115200);
  SPIFFS.begin();
  wifimanager.autoConnect("imageViewer");
  
  server.on("/img",HTTP_GET,[](){
    Dir root = SPIFFS.openDir("/");
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/json", "");
    server.sendContent("[");
    while(root.next()){
      String i = root.fileName();
      Serial.println(i);
      if(!(i.endsWith(".html") || i.endsWith(".css") || i.endsWith(".js") ||i.endsWith(".htm"))){
        server.sendContent("\"");
        server.sendContent(i);
        server.sendContent("\",");
      }
    }
    server.sendContent("]");
    server.sendContent("");

  });

  server.on("/edit",HTTP_POST, [](){returnOK();}, handleFileUpload);
  server.onNotFound(handleOtherFiles);
  server.begin();
}

void loop() {
  server.handleClient();
}

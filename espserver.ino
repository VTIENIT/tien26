
//  ESP8266 đóng vai trò là một mini server, sau đó ta dùng
//  trình duyệt để truy cập vào nó điều khiển tín hiệu đèn led 
//  trên esp bằng chính đèn led trong esp.

#include<ESP8266WiFi.h>

const char* ssid      ="Wi - IT"  ;
const char* password  = "67896789"  ;


int ledpin =16 ;

WiFiServer server(80) ;

void setup()
{
  Serial.begin(115200) ;
  delay(10) ;
  pinMode(ledpin,OUTPUT);
  digitalWrite(ledpin,LOW) ;

// kết nối WiFi 

Serial.print(" ket noi den wifi: ") ;
Serial.print(ssid);


WiFi.begin(ssid,password) ; // hàm kết nối WiFi

while(WiFi.status() != WL_CONNECTED) 
{
  delay(1000) ;
  Serial.println(".") ;
  
}

Serial.println(" ") ;
Serial.println("WiFI connection") ;


server.begin(); // hàm khởi chạy server trong esp
Serial.println("SERVER START" ) ;


//in địa chỉ ip do esp ( đóng vai trò server) cấp phát

 Serial.print(" use this URL  connect: ") ;
 Serial.print("http://") ;
 Serial.print(WiFi.localIP()) ;
 Serial.println("/");
   
}


void loop()
{
  WiFiClient client = server.available() ; // khởi động client cho trình duyệt
  if(! client ) return ;

 Serial.println("new client") ;
 while(!client.available()) 
 {
  delay(1) ;
 }

  String request= client.readStringUntil('\r') ;
  Serial.println(request) ;
  client.flush() ; // bỏ qua các phần chưa đọc, dư thừa

  int value = LOW;
  if(request.indexOf("/LED=ON") != -1)
  {
    digitalWrite(ledpin,HIGH) ;
    value= HIGH ;
  }
  if(request.indexOf("/LED=OFF") != -1 )
  {
    digitalWrite(ledpin,LOW ) ;
    value =LOW ;
  }

// response của trình duyệt trả lại server 

client.println("HTTP/1.1 200 OK ") ;
client.println(" content-Type: text/html" ) ;
client.println("") ;
client.println("<!DOCTYPE HTML>") ;
client.println("<html>") ;


client.println("led pin is now ") ;

if(value==HIGH) 
{
  client.println("ON") ;
}
else
{
  client.println("OFF") ;
}

  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");  
  client.println("</html>");


  delay(1) ;
  Serial.println("client disconnected") ;
  Serial.println("") ;
}



#include <SPI.h>
#include <Ethernet2.h>
#include <SD.h>



int temperature=0;
int humidite=0;
int pression=0;
int altitude=0;
int luminosite=0;
int directionVent=0;
int vitesseVent=0;
int pluie=0;
int pluieJour=0;
int rafraichir=0;
int action =0;
String password="alpine";





String ethernet_buffer;

byte mac[] = {0xA8, 0x61, 0x0A, 0xAE, 0x17, 0xDD};
IPAddress ip(172, 25, 12, 6); 
EthernetServer server(80);
File webFile;


void setup()
{
 
          
  Serial.begin(9600);
  delay(3000);
   
     Serial.println("Initializing SD card...");
    if (!SD.begin(4)) {
        Serial.println("ERROR - SD card initialization failed!");
        return;    // init failed
    }
    Serial.println("SUCCESS - SD card initialized.");
    // check for index.htm file
    if (!SD.exists("index1.htm") && !SD.exists("index2.htm")) {
        Serial.println("ERROR - Can't find file!");
        return;  // can't find index file
    }
    else Serial.println("Your files has been found !");
    
    
    ethernet_buffer ="";
  
    
    Ethernet.begin(mac, ip);  
    server.begin();          
}



void loop()
{
EthernetClient client = server.available();

    if (client) {
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                ethernet_buffer += c;
                Serial.write(c);

                if (c == '\n' && currentLineIsBlank) {

                     action =0;

         
                     
                     if(ethernet_buffer.indexOf("totality")>0) action=1;    // rafraichissement des données
                    // if(ethernet_buffer.indexOf("analog_val")>0) action=2; 
                    // if(ethernet_buffer.indexOf("transmettre")>0) action=4;
                    

                     if(action !=0)
                     {
                        print_message("HTTP/1.1 200 OK",true,client);
                        print_message("Content-Type: text/xml",true,client);
                        print_message("Connection: keep-alive",true,client);
                        client.println();
                        XML_response(client, action);                      // générer le fichier XML
                    }
                    
                    else {  
                        
                        if(ethernet_buffer.indexOf("password.js")>0)
                          {
                          print_message("HTTP/1.1 200 OK",true,client);
                          print_message("Content-Type: text/javascript",true,client);
                          print_message("Connection: keep-alive",true,client);
                          client.println();
                        
                          load_file("password.js",client);                    // chargement password.js
                            
                          }
                          else if(ethernet_buffer.indexOf("parallax.js")>0)
                            {
                        print_message("HTTP/1.1 200 OK",true,client);
                        print_message("Content-Type: text/javascript",true,client);
                        print_message("Connection: keep-alive",true,client);
                        client.println();
                        
                        load_file("parallax/parallax.js", client);      // chargement parallax.js 
                              
                            }

                 

                          else
                          {
                            print_message("HTTP/1.1 200 OK",true,client);
                            print_message("Content-Type: text/html",true,client);
                            print_message("Connection: keep-alive",true,client);
                            client.println();
                            
                            if(ethernet_buffer.indexOf("index2.htm")>0){
                              load_file("pages/index2.htm",client);      
                            }
                                 else {
                                  load_file("index1.htm",client);                        // chargement de la page web 
                                 }
                          }
                        
                        
                        
                        

                                            
                        
                        if (webFile) {
                            while(webFile.available()) {
                                                        client.write(webFile.read()); // lecture sur carte SD
                                                        }
                            webFile.close();
                            }
                         }
                    

                    ethernet_buffer="";
                    break;
                }
                if (c == '\n') {currentLineIsBlank = true;} 
                else if (c != '\r') {currentLineIsBlank = false;}
            } 
        } 
        delay(1);   
        client.stop(); 
    } 
  
}





//**************************************************
// Fichier XML
void XML_response(EthernetClient cl, int ordre)
{

temperature = random(13, 25);
humidite = 12;

cl.print("<?xml version = \"1.0\" ?>");
cl.print("<inputs>");

cl.print("<temperature>");
cl.print(temperature);
cl.print("</temperature>");

cl.print("<humidite>");
cl.print(humidite);
cl.print("</humidite>");

cl.print("<password>");
cl.print(password);
cl.print("</password>");
     
cl.print("</inputs>");

}







//**************************************************




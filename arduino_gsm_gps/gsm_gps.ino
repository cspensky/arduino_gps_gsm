// Product name: GPS/GPRS/GSM Module V3.0
// # Product SKU : TEL0051
// # Version     : 0.1
 
// # Description:
// # The sketch for driving the gsm mode via the Arduino board
 
// # Steps:
// #        1. Turn the S1 switch to the Prog(right side)
// #        2. Turn the S2 switch to the Arduino side(left side)
// #        3. Set the UART select switch to middle one.
// #        4. Upload the sketch to the Arduino board
// #        5. Turn the S1 switch to the comm(left side) 
// #        6. RST the board 
 
// #        wiki link- http://www.dfrobot.com/wiki/index.php/GPS/GPRS/GSM_Module_V3.0_(SKU:TEL0051)
 
int UNIT_ID = 1;
 
 
byte gsmDriverPin[3] = {
  3,4,5};//The default digital driver pins for the GSM and GPS mode
//If you want to change the digital driver pins
//or you have a conflict with D3~D5 on Arduino board,
//you can remove the J10~J12 jumpers to reconnect other driver pins for the module!

// Strings for specific GPS info
char GPRMC[]={'$','G','P','R','M','C',',','\0'};//match the gps protocol

void gps_get_info(char * buffer, char * tag) 
{
  // Switch to GPS mode
  digitalWrite(4,LOW);//Enable GPS mode
  digitalWrite(3,HIGH);//Disable GSM mode
  delay(2000);
    
  // Index to keep track of where we are storing the char in our buffer
  int i = 0; 
    
  // Read until we find the proper tag, e.g. $GPRMC
  while(1)
  {
    if(Serial.available())
    {
      buffer[i] = Serial.read();//get the data from the serial interface
      if(buffer[i]==tag[i]) //Match the protocol
      {    
        i++;
        if(i==strlen(tag))
        {
          i = 0;
          break;
        }
      }
      else
        i=0;
    }
  } 
  
  // Read the rest of the data into our buffer (break at newline \n)
  while(1)
  {
    if(Serial.available())
    {
      buffer[i] = Serial.read();
      
      // Replace special characters
      if (buffer[i] == ',')
      {
        buffer[i] = '_';
      } else if (buffer[i] == '*') 
      {
        buffer[i] = '\0';
        break;
      }
      i++;
    } // end serial available
  } // end while
  
//  // Disable both modes
//  digitalWrite(3,HIGH);//Disable GSM mode
//  digitalWrite(4,HIGH);//Disable GPS mode
//  delay(2000);
}

void gprs_init()
{
  // Enable GSM
  digitalWrite(3,LOW);//Enable the GSM mode
  digitalWrite(4,HIGH);//Disable the GPS mod
  
  Serial.println("AT");
  delay(2000);
  
    // SIM Connected?
//  Serial.println("AT+CPIN?");
  
  
  // Registered with cellular network?
//  Serial.println("AT+CGREG?");
  
  // What network are we on?
//  Serial.println"AT+COPS?");
  
  // Signal Quality
//  Serial.println("AT+CSQ");

  // Attach to packet domain
  Serial.println("AT+CGATT=1");
  delay(5000);

  // Setup our Packet Data Protocol (PDP) context
  // E.g. Access Point Names (APNs)
  Serial.println("AT+CGDCONT=1,\"IP\",\"epc.tmobile.com\""); // T-Mobile
  delay(500);
  Serial.println("AT+CGDCONT=2,\"IP\",\"isp.cingular\""); // Cingular
  delay(500);

  // Confirm our context settings
//  Serial.println("AT+CGDCONT?");
  
  // Activate our PDP context
  Serial.println("AT+CGACT=1");
  delay(5000);
  
  // Check Bearer status (Still not 100% what these are)
//  Serial.println("AT+SAPBR=4,1");
//  Serial.println("AT+SAPBR=4,2");
//  Serial.println("AT+SAPBR=4,3");
  
  // Select first bearer?
  Serial.println("AT+SAPBR=1,1");
  delay(5000);
  
  // We should have an IP now.
  Serial.println("AT+SAPBR=2,1");
  delay(1000);
  
  // Turn UART off
  digitalWrite(3,HIGH);//Disable the GSM mode
  digitalWrite(4,HIGH);//Disable the GPS mode
}

void gprs_close()
{
  // Enable GSM
  digitalWrite(3,LOW);//Enable the GSM mode
  digitalWrite(4,HIGH);//Disable the GPS mod
  
  // Close our connections up
  Serial.println("AT+CGATT=0"); // Close Packet Data Protocol
  delay(1000);
  
  // Turn UART off
  digitalWrite(3,HIGH);//Disable the GSM mode
  digitalWrite(4,HIGH);//Disable the GPS mode
}

void httpPost(char * parameter, char * value)
{
  // Enable GSM
  digitalWrite(3,LOW);//Enable the GSM mode
  digitalWrite(4,HIGH);//Disable the GPS mod

  ///
  // Now actually do a HTTP POST
  ///
  
  // First initialize our HTTP protocol
  Serial.println("AT+HTTPINIT");
  delay(1000);
  
  
  // Set CID to 1 (Probably don't need this, but every example has it.)
  Serial.println("AT+HTTPPARA=\"CID\",1");
  delay(1000);

  // Set our parameters
  Serial.print("AT+HTTPPARA=\"URL\",\"http://18.181.2.31/harley/s.php?");
  Serial.print("id=");
  Serial.print(UNIT_ID);  // Appen our id to the query
  Serial.print("&");
  Serial.print(parameter);
  Serial.print("=");
  Serial.print(value);
  Serial.println("\"");
  delay(1000);

  // Set our action (0 - GET, 1 - POST, 2 - HEAD)
  Serial.println("AT+HTTPACTION=1");
  delay(1000);

  // Read response
  Serial.println("AT+HTTPREAD");
  delay(1000);

  // Terminate HTTP session
  Serial.println("AT+HTTPTERM");
  delay(1000);
  
  // Turn UART off
  digitalWrite(3,HIGH);//Disable the GSM mode
  digitalWrite(4,HIGH);//Disable the GPS mode

}



void setup()
{    
  //Init the driver pins for GSM function
  for(int i = 0 ; i < 3; i++){
    pinMode(gsmDriverPin[i],OUTPUT);
  }
  
  digitalWrite(5,HIGH);//Output GSM Timing 
  delay(1500);
  digitalWrite(5,LOW);  
  
  
  digitalWrite(3,LOW);//Enable the GSM mode
  digitalWrite(4,HIGH);//Disable the GPS mode
  
  delay(2000);
  Serial.begin(9600); //set the baud rate
  delay(5000);//call ready
  
  // Enable GPS
  Serial.println("AT");   
  delay(2000);
  //turn on GPS power supply
  Serial.println("AT+CGPSPWR=1");
  delay(1000);
  //reset GPS in autonomy mode
  Serial.println("AT+CGPSRST=1");
  delay(1000);
  
  // Just wait a bit...
  delay(5000);
  delay(5000);
}
 
void loop()
{  
  
  // Buffer for our GPS input
  char buffer[150] = "test\0";
  
  // Init GPS
  gprs_init();
  
  // Loop forever
  while (1) {
    
   gps_get_info(buffer,GPRMC);
        
     
//    while (Serial.available())
//  {
//    char v = Serial.read();
//  }
    // Send this GPS String to our web interface.
    httpPost("gprmc",buffer);
   
   delay(30000); 
  }
  
  gprs_close();



  
//  //Send message
//  Serial.println("AT+CMGF=1");
//  delay(1000);
//  Serial.println("AT+CMGS=\"12524520759\"");//Change the receiver phone number
//  delay(1000);
//  Serial.print("I am computer.");//the message you want to send
//  delay(1000);
//  Serial.write(26);
  while(1);
}

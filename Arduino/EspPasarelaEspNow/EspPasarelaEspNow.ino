/*
 
  Recibe mensajes ESP-NOW y los reenvía por el puerto serie añadiendole antes la MAC del dispositivo que envío el mensaje
  Está escuchando en la MAC 3E:33:33:33:33:33
 
 */
 
//Librerias
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <SoftwareSerial.h>

#define BAUD_RATE 115200

// GPIOs
#define LEDCheck 16  

//canal que se va a utilizar para el ESP-NOW
#define WIFI_CHANNEL 6

uint8_t kok[16]= {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
uint8_t key[16]= {0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44};

//Configuramos el protocolo serie.
SoftwareSerial swSer(14, 12, false);

//char para construir el mensaje que se va a enviar por ESP-NOW
char JSON_serie[256];
//MAC de la ESP remota que recibe los mensajes de esta ESP mediante ESP-NOW.
uint8_t macPasarela[] = {0x3E, 0x33, 0x33, 0x33, 0x33, 0x33};
//MAC de la ESP que está en la pasarela y se encarga del ESP-NOW
uint8_t macDHT11[] = {0x3E, 0x33, 0x33, 0x33, 0x33, 0x34};
//Variable para controlar el tiempo.
volatile unsigned long ultimo_mensaje=0;
//Variable para controlar el aviso de "Esperando mensajes ESP-NOW"
unsigned long heartBeat=0;

//Método que se encarga de recoger lo que llega del protocolo serie 
//y se envía por ESP-NOW al ESP.
inline void readSerial() {

	//Obtenemos la longitud del JSON que vamos a recibir.
	while (swSer.available() < 2) { delay(1); }
	int len1 = (swSer.read() - 48)*10;
	int len2 = (swSer.read() - 48);
	int len = len1+len2+1;

	//Obtenemos el JSON
	while (swSer.available() < len) { delay(1); }
	swSer.readBytes((char*)&JSON_serie, len);
	JSON_serie[len]='\0'; //fin de cadena, por si acaso

	//Enviamos el JSON por ESP_NOW
	esp_now_send(macDHT11, (uint8_t *) JSON_serie, strlen(JSON_serie)+1);
  
}

void setup() {
	
	Serial.begin(115200); 
	Serial.println();

	//Inicializamos el LED.
	pinMode(LEDCheck, OUTPUT);    
	digitalWrite(LEDCheck, LOW);

	//Ponemos el WiFi en modo AP.
	WiFi.mode(WIFI_AP);
	WiFi.disconnect();
	//Establecemos la MAC para esta ESP
	wifi_set_macaddr(SOFTAP_IF, &macPasarela[0]);
	Serial.print("MAC: "); Serial.println(WiFi.softAPmacAddress());
  
	//Iniciamos ESP-NOW
	if (esp_now_init()!=0) {
		Serial.println("*** Fallo al iniciar el ESP-NOW");
		ESP.restart();
	}

	//Especificamos las claves para enviar la información de manera encriptada.
	esp_now_set_kok(kok, 16);

	//Especificamos el rol del ESP-NOW (0=OCIOSO, 1=MAESTRO, 2=ESCLAVO y 3=MAESTRO+ESCLAVO)
	esp_now_set_self_role(3);
	//Emparejamos con el otro ESP
	esp_now_add_peer(macDHT11, 3, WIFI_CHANNEL, key, 0);
	//Especificamos las claves para enviar la información de manera encriptada.
	esp_now_set_peer_key(macPasarela, key, 16);

	//Configuramos protoloco serie
	swSer.begin(BAUD_RATE);

	//Si llega un mensaje por ESP-NOW a esta ESP, recogemos la información y la enviamos por serial.
	esp_now_register_recv_cb([](uint8_t *mac, uint8_t *data, uint8_t len) {

		//Encendemos el LED
		digitalWrite(LEDCheck, HIGH);

		//Preparamos el mensaje para enviarlo por serial. Añadimos la dirección 
		//MAC del dispositivo que envía la información.
		//Si el tamaño del mensaje es mayor que 30, es un mensaje con información del sensor, 
		//no enviamos con el inicio de $$, si es el mensaje de que está vivo, lo enviamos con %% 
		//para diferenciarlos.
		if(len > 31){ Serial.write("$$"); }
		else{ Serial.write("%%"); }
		
		Serial.write(mac, 6);
		Serial.write(len);
		Serial.write(data, len);
		
		ultimo_mensaje=millis();
    
  });
}

void loop() {
	//Obtenemos el momento actual.
	unsigned long ahora = millis();

	//Si la diferencia desde el último heartBeat hasta ahora es de más de 30 segundos, 
	//mostramos un mensaje para hacer referencia que estamos esperando un mensaje de ESP-NOW.
	if (ahora-heartBeat > 30000) {
		Serial.println("\nEsperando mensajes ESP-NOW...");
		heartBeat = ahora;
	}

	//Si el LED está encendido porque se ha enviado un mensaje por el 
	//protocolo serie y hace más de 0,2s que está encedido, se apaga.
	if (digitalRead(LEDCheck)==HIGH && ahora-ultimo_mensaje>=200){ digitalWrite(LEDCheck, LOW); }
 	
	//Si tenemos algún mensaje por serie, entramos en el bucle.
	while (swSer.available()){ 
		//Comprobamos si viene primero el icono '$'
		if ( swSer.read() == '$' ) {
			//En caso de venir ese icono, comprobamos que el segundo sera también '$'. 
			//En caso de ser, es un JSON con información del sensor y vamos a proceder a leerlo.
			while ( ! swSer.available() )  delay(1); 
			
			if ( swSer.read() == '$' ) { 
				//Leemos los datos.
				readSerial();
				ultimo_mensaje=ahora;
			}
		}
	}
}
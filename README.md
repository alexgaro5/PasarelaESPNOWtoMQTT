# Pasarela MQTT para dispositivos IoT con protocolo de comunicación ESP-NOW

<p>Trabajo final de máster de Ingeniería Informática de la Univerisdad de Málaga realizado por:</p>

<ul>
  <li>Nombre del estudiante: Rafael Alejandro García Rodríguez.</li>
  <li>Nombre del tutor: Andrés Rodríguez Moreno.</li>
</ul>

<h3>Introducción y objetivo</h3>

<p>La Inteligencia Ambiental ha sido descrita desde distintas perspectivas. Desde un punto de vista tecnológico, la Inteligencia Ambiental puede definirse como “una inteligencia omnipresente y transparente en un entorno vigilado que soporta las actividades e interacciones de los usuarios”. La visión principal de la Inteligencia Ambiental presenta al usuario rodeado de interfaces inteligentes e intuitivas, integradas en los objetos cotidianos de su entorno de forma transparente. Estas interfaces poseen capacidad para reconocer la presencia de diferentes usuarios, y modificar su comportamiento en función de la identidad de dicho usuario, sus necesidades y las características del contexto o entorno donde se encuentren. Especial atención merecen aspectos como la facilidad de uso, el soporte eficiente de los servicios y la posibilidad de mantener interacciones naturales con las personas. Su gran relevancia reside en los importantes cambios que, a no muy largo plazo, implicarán sus resultados en la vida diaria de las personas.</P>
  
<p>La visión de la Inteligencia Ambiental consiste en la creación de espacios donde los usuarios interaccionen de forma natural y sin esfuerzo con los diferentes sistemas, gracias a que las tecnologías de computación y comunicación se convierten, en estos entornos, en invisibles para el usuario, al estar siempre presentes e integradas en los objetos cotidianos del mismo. De esta forma, es la propia tecnología la que se adapta a los individuos y a su contexto, actuando de forma autónoma, y facilitándoles la realización de sus tareas diarias y la comunicación entre ellos y con el entorno.</P>
  
<p>En este Trabajo Fin de Máster nos vamos a centrar en estudiar un nuevo protocolo de comunicación llamado “ESP-NOW” frente a otros protocolos ya existentes, y realizar un ejemplo de como esto puede llevarse a la Inteligencia Ambiental con un ejemplo usando este protocolo y conectándolo a la nube.</p>

<p>El objetivo principal de este TFM es investigar acerca del protocolo de comunicación “ESP-NOW” y hacer un pequeño ejemplo práctico en el que se muestra como esto podemos llevarlo a un entorno inteligente conectándolo a la nube. Para ello necesitaremos hacer uso de hardware especifico y la creación de código para aplicar el protocolo de comunicación nombrado.</p>

<h3>Esquema de carpetas</h3>

<p>En el proyecto podemos encontrarnos dos carpetas:</p>

<ul>
  <li><b>VM:</b> En ella se guarda el certificado que usaremos para conectarlos al broker MQTT desde Node-Red y desde la ESP. También tenemos el flujo de Node-Red, y un enlace para descargar la máquina virtual completa con el broker MQTT y Node-Red.</li>
  <li><b>Arduino:</b> Contiene el código que hay que cargar en la ESP que juega el rol de ESP con el sensor DHT11 (EspDHT11), el código de la ESP que está en la pasarela y recibe los mensajes de ESP-NOW (EspPasarelaEspNow) y el código de la ESP que está en la pasarela y envía los datos por MQTT a Node-Red (EspPasarelaMQTT).</li>
</ul>

<h3>Esquema de conexionado</h3>

<P>El conexionado de las tres ESPs es la siguiente:</P>

<h6>ESP que contiene el sensor DHT11</h6>

![alt text](https://i.ibb.co/1zhG9KQ/DHT11.png)

<h6>ESPs que completan la pasarela</h6>

![alt text](https://i.ibb.co/bKSPNXR/Pasarela.jpg)

<h3>Despliegue del proyecto</h3>

<p>Para ejecutar el Node-Red y el broker MQTT simplemente debemos de importar la máquina virtual, encenderla y ejecutar el script que está en el escritorio llamado "config.sh".</p>
<p>El código de la ESP-32 simplemente descargarlo, compilarlo y cargarlo en las correspondientes ESPs.</p>

<p>Finalmente recordar que todo debe de ejecutar en una red la cual debe de asignar IPs del estilo "192.168.1.XXX", ya que la máquina virtual y el código arduino tiene IPs estáticas que apuntan a direcciones con esta estructura.</P>

# Smart Transport

Smart Tansporte es un prototipo para realizar el conteo de pasajeros,  su estado ocupacional y ubicación en tiempo real con alta confiabilidad, alta precisión y bajo costo.

* ### Definición del proyecto

En los sistemas de transporte público de pasajeros hay una gran cantidad de aspectos relevantes cuando de sostenibilidad y/o auto-sostenibilidad se trata. Uno de ellos es desarrollar estrategias para mantener, y si es posible, incrementar la demanda del servicio, ya que este es uno de los ítems más relevantes entre los inputs del modelo financiero; modelo a partir del cual se calcula la tarifa que finalmente pagan los usuarios. Otro de estos aspectos importantes es el control y fiscalización sobre la cantidad de personas que hacen uso del sistema de transporte, ya que de nada sirve desplegar las estrategias para mantener o incrementar la demanda, si al final no es posible percibir y custodiar la totalidad de los ingresos. En sistemas de transporte, tales como el SETP (Sistema Estratégico de Transporte Público de Pasajeros) de Popayán, estos dos aspectos descritos son los que principalmente conforman la base que soporta los costos de operación del servicio.
Debido a esto se están desarrollando y se comercializan herramientas tecnológicas que permiten registrar el comportamiento de la demanda del transporte público, y a la vez controlar y fiscalizar los ingresos. Algunas de estas herramientas son los sistemas de conteo de pasajeros, entre los cuales existe una gran variedad de opciones, desde torniquetes (indeseados por disminuir la accesibilidad al medio físico e incrementar los tiempos de acceso al vehículo) hasta desarrollos basados en procesamiento digital de imágenes, pasando por pulsadores en forma de tapetes y por barreras infrarrojas.
Si se tiene en cuenta que por cada vehículo que hace parte del sistema de transporte es necesario instalar uno de estos contadores de pasajeros, se hace evidente la necesidad de encontrar una tecnología de bajo costo y que al mismo tiempo sea de alta confiabilidad para optimizar costos de Operación & Mantenimiento, todo esto balanceado de tal forma que se asegure una alta precisión en el conteo, su estado ocupacional y ubicación en tiempo real.

* ### Especificaciones

El sistema  lleva el control y fiscalización sobre la cantidad de personas que hacen uso del sistema de transporte, teniendo en cuenta la capacidad máxima (60%) permitida por protocolos de bioseguridad, mediante un pulsador para cada acción (subir y descender) que ejecuta el conductor del vehículo.
El sistema muestra en dos pantallas LCD tanto al conductor como a los pasajeros la cantidad de asientos disponibles.
Cada que un usuario hace uso del transporte, el sistema envía a la empresa, su ubicación en tiempo real mediante GPS, el total de pasajeros transportados hasta el momento y el total de ingresos.

* ###Arquitectura

![](https://i.imgur.com/waM1OoS.jpg)


* ### Diagrama de flujo

![](https://i.imgur.com/W2P9t7K.jpg)

Pantalla 1  / 2 : Muestra el estado de ocupación al conductor, 
mostrarPantallas: Recibe información del sistema de operaciones lógicas para indicar a las pantallas lo que debe mostrar.
operacionesLogicas: se encarga de recibir información de los sensores para hacer su respectiva operación (suma o resta).
sensor1: estará ubicado en la parte delantera del vehículo, será el encargado de contar cuántas personas han subido y enviará la información al operador lógico.
sensor2 : está ubicado en la parte trasera del vehículo, será el encargado de contar cuantas personas bajan del autobús y enviará la información al operador lógico.
conecionEmpresa. Se encargará de enviar toda la información a la empresa. Su ubicación, cuántos pasajeros lleva.
revisarEstado: Permite a los usuarios revisar el estado de los autobuses.  Su posición y si hay asientos disponibles.



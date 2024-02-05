## Kran Otomatis: ESP8266/ESP32 dengan Firebase Realtime Database

**Functionality:**

* Connects to WiFi using provided SSID and password.
* Authenticates with Firebase using user email and password.
* Reads humidity using a DHT sensor and sends data to Firebase.
* Reads water level using an ultrasonic sensor and sends data to Firebase.
* Listens for infrared remote commands and acts accordingly.
* Controls a servo motor to open/close the faucet based on commands or Firebase data.
* Reads and updates the faucet status ("on" or "off") from Firebase.

**Hardware:**

* ESP8266/ESP32 microcontroller
* DHT sensor (D4)
* Ultrasonic sensor (D8, D7)
* Servo motor
* Infrared receiver (D5)

**Software:**

* Arduino IDE
* Firebase libraries: FirebaseESP8266/FirebaseESP32, TokenHelper, RTDBHelper
* DHTesp library (for DHT sensor)
* IrSignal library (for infrared receiver)
* HCSR04 library (for ultrasonic sensor)

**Code Structure:**

* **Setup:**
    * Connects to WiFi.
    * Initializes Firebase with API key, user credentials, and database URL.
    * Sets up DHT sensor and infrared receiver.
* **Loop:**
    * Reads humidity and sends data to Firebase.
    * Reads water level and sends data to Firebase.
    * Listens for infrared commands and updates servo position accordingly.
    * Reads faucet status from Firebase and updates servo position if needed.

**Explanation of key sections:**

* **Firebase configuration:** Set your WiFi credentials, API key, database URL, user email, and password in the defined constants.
* **Sensor data reading:** Use `dht.getHumidity()` for humidity and `distanceSensor.dist()` for water level. Send data to Firebase with `Firebase.setInt()`.
* **Infrared commands:** Use `receiver->receive()` to listen for commands. Decode the received data and take appropriate actions (e.g., update servo position).
* **Servo control:** Set the `sudut_servo` variable based on desired position (e.g., 90 for closed, 180 for open). Update the servo with `myservo.write(sudut_servo)`.
* **Firebase data reading/writing:** Use `Firebase.getString()` to read values and `Firebase.setInt()` or `Firebase.setString()` to update values in the database.

**Additional notes:**

* This is a basic explanation. Modify the code according to your specific needs and hardware setup.
* Refer to the documentation of the used libraries for more details on their functionalities.

I hope this README helps you understand the code and set up your own automated faucet system!



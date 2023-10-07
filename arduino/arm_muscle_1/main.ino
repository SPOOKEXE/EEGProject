
const int DATA_PIN = 1;

void setup() {
	Serial.begin(115200);

	pinMode(DATA_PIN, INPUT);
}

void loop() {

	int value = analogRead( DATA_PIN );
	Serial.println(value);
}

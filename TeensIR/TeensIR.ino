#include <limits.h>
#include <IRremote.h>

#define STATUS_LED 6

#define DEBUG 1

IRsend Remote;

void setup()
{
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, LOW);

  Serial.begin(9600);
  Serial.setTimeout(1000);
}

#define LED_ON digitalWrite(STATUS_LED, HIGH)
#define LED_OFF digitalWrite(STATUS_LED, LOW)

#ifdef DEBUG
#define DBG(fmt, args...) Serial.printf(fmt "\r\n", ##args)
#else
#define DBG(fmt, ...) 0
#endif

void emit(char *command_str) {
    unsigned long command = strtoul(command_str, NULL, 16);
    int n_bits = strlen(command_str) * 4;
    if (n_bits > sizeof(command) * CHAR_BIT) {
        Serial.printf("%d bits don't fit into an unsigned long here\r\n",
            n_bits);
        return;
    }

    DBG("sending command 0x%lx (%d bits)", command, n_bits);
    LED_ON;
    Remote.sendSAMSUNG(command, n_bits);
    LED_OFF;
    DBG("done");
}

void parse_command(char *buf) {
    char *start = buf, *p = buf;

    while (*p) {
        if (*p == '\r' || *p == '\n') {
            *p = '\0';
            break;
        }

        if (isspace(*p)) {
            *p = '\0';
            if (start < p)
                emit(start);

            DBG("delaying 100ms");

            delay(100);
            start = p+1;
        } else if (!isxdigit(*p)) {
            Serial.printf("Invalid character 0x%02x at input pos %d\r\n",
                *p, p-buf);
            return;
        }

        p++;
    }

    if (start < p)
        emit(start);
}

void loop() {
    char buf[256];
    int c = Serial.readBytesUntil('\r', buf, sizeof(buf));
    buf[c] = '\0';

    if (c > 0) {
        parse_command(buf);
    }
}

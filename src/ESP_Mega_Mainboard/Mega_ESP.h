#ifndef MEGA_ESP
#define MEGA_ESP

void listen_esp()
{
    int i = 0;
    while (Serial2.available() > 0)
    {
        char a = Serial2.read();
        if (a == '\0')
            continue;
        if (a != '\r' && a != '\n' && (a < 32))
            continue;
        if (i < max_length) msg[i] = a;

        Serial.print(a);
        i++;
    }
    if (i < max_length) msg[i] = '\0';
    else  msg[max_length - 1] = '\0';
}

void talk_esp()
{
    while (Serial.available() > 0)
    {
        char a = Serial.read();
        Serial.write(a);
        Serial2.write(a);
    }
}

#endif
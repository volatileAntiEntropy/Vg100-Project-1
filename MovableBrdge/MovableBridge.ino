#include <Servo.h>
#include <L298N.h>
using namespace L298N;

typedef enum
{
    Retracted,
    Deployed
} BridgeStatus;

BridgeStatus Status = Retracted;
Motor MotorA;
Servo Rotator;

void setup()
{
    Serial.begin(115200);
    Rotator.attach(9);
    Rotator.write(170);
    MotorA.attach(4, 5);
    MotorA.free();
    delay(100);
    Serial.println(F("Ready"));
}

void loop()
{
    String command = "";
    if (Serial.available())
    {
        command = Serial.readString();
    }
    if (command.equals(F("Query")))
    {
        Serial.println(Status);
    }
    else
    {
        switch (Status)
        {
        case Deployed:
            if (command.equals(F("Retract")))
            {
                Retract();
            }
            break;
        case Retracted:
            if (command.equals(F("Deploy")))
            {
                Deploy();
            }
            else if (command.equals(F("End")))
            {
                End();
            }
            break;
        default:
            break;
        }
    }
}

void Retract()
{
    for (uint8_t i = 11; i <= 170; i += 2)
    {
        Rotator.write(i);
        delay(40);
    }
    MotorA.runAt(255, Forward);
    delay(7000);
    Status = Retracted;
}

void Deploy()
{
    MotorA.runAt(255, Backward);
    delay(6500);
    MotorA.free();
    for (uint8_t i = 170; i >= 11; i -= 2)
    {
        Rotator.write(i);
        delay(40);
    }
    Status = Deployed;
}

void End()
{
    for (uint8_t i = 170; i >= 11; i -= 2)
    {
        Rotator.write(i);
        delay(40);
    }
    Status = Deployed;
}
// see:
// http://www.bogotobogo.com/DesignPatterns/command.php
//
// Command Pattern - simple example 2
//
// For example, LightOnCommand object has the right method (execute(), and
// actually, doing light->On()) of the receiver (Light object). This is
// possible, the command object has the pointer to the Light receiver as its
// member. So, when asked to perform an action on a receiver, we just feeding
// the command object to the invoker.
//
// The invoker which is named as control in the code make a request of a
// Command object by calling that object's execute() method. Then, since the
// method knows what the receiver is, it invokes the action on that receiver.
//
// Here is an example code:
//
// The simple example of the previous section has been extended so that we can
// handle several receivers. The commands bounded for each receivers are
// stored in vector form:
//

#include <iostream>
#include <vector>
using namespace std;

const int MaxCommand = 5;

enum Receiver
{
    LIGHT = 0,
    FAN,
    DOOR,
    OVEN,
    NONE
};

// Command Interface
class Command
{
public:
    virtual ~Command(){};
    virtual void execute() = 0;
};

// Receiver Class
class Light
{
public:
    void on() { cout << "The light is on\n"; }
    void off() { cout << "The light is off\n"; }
};

// Receiver Class
class Fan
{
public:
    void on() { cout << "The fan is on\n"; }
    void off() { cout << "The fan is off\n"; }
};

// Command for turning on the light
class NullCommand : public Command
{
public:
    void execute() { cout << "Null command: does nothing\n"; }
};

// Command for turning on the light
class LightOnCommand : public Command
{
public:
    LightOnCommand(Light *light) : mLight(light) {}
    void execute() { mLight->on(); }

private:
    Light *mLight;
};

// Command for turning off the light
class LightOffCommand : public Command
{
public:
    LightOffCommand(Light *light) : mLight(light) {}
    void execute() { mLight->off(); }

private:
    Light *mLight;
};

// Command for turning on the fan
class FanOnCommand : public Command
{
public:
    FanOnCommand(Fan *fan) : mFan(fan) {}
    void execute() { mFan->on(); }

private:
    Fan *mFan;
};

// Command for turning off the fan
class FanOffCommand : public Command
{
public:
    FanOffCommand(Fan *fan) : mFan(fan) {}
    void execute() { mFan->off(); }

private:
    Fan *mFan;
};

// Invoker
// Stores the ConcreteCommand objects
class RemoteControl
{
public:
    RemoteControl() : mOnCommand(MaxCommand), mOffCommand(MaxCommand)
    {
        Command *nullCmd = new NullCommand; // FIXME: memory leak! CK
        for (int i = 0; i < MaxCommand; i++) {
            mOffCommand[i] = nullCmd;
            mOnCommand[i] = nullCmd;
        }
    }

    void setCommand(Receiver id, Command *onCmd, Command *offCmd)
    {
        mOnCommand[id] = onCmd;
        mOffCommand[id] = offCmd;
    }

    void onButtonPressed(Receiver id) { mOnCommand[id]->execute(); }

    void offButtonPressed(Receiver id) { mOffCommand[id]->execute(); }

private:
    vector<Command *> mOnCommand, mOffCommand;
};

// The client
int main()
{
    // Receiver
    Light *light = new Light;
    Fan *fan = new Fan;

    // concrete Command objects
    LightOnCommand *lightOn = new LightOnCommand(light);
    LightOffCommand *lightOff = new LightOffCommand(light);
    FanOnCommand *fanOn = new FanOnCommand(fan);
    FanOffCommand *fanOff = new FanOffCommand(fan);
    NullCommand *nullOn = new NullCommand();
    NullCommand *nullOff = new NullCommand();

    // invoker objects
    RemoteControl *control = new RemoteControl;

    // execute
    control->setCommand(LIGHT, lightOn, lightOff);
    control->onButtonPressed(LIGHT);
    control->offButtonPressed(LIGHT);

    // execute
    control->setCommand(FAN, fanOn, fanOff);
    control->onButtonPressed(FAN);
    control->offButtonPressed(FAN);

    // execute
    // XXX control->setCommand(OVEN, nullOn, nullOff);
    control->onButtonPressed(NONE);
    control->offButtonPressed(DOOR);

    delete light;
    delete lightOn;
    delete lightOff;

    delete fan;
    delete fanOn;
    delete fanOff;

    delete nullOn;
    delete nullOff;

    delete control;

    return 0;
}

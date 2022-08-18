
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
using namespace omnetpp;

class Station_A : public cSimpleModule
{
  private:
    int temp1;
    int i;
    simtime_t timeout;  // timeout
    cMessage *timeoutEvent;  // holds pointer to the timeout self-message
    int seq = 0;
    cMessage *message;  // message that has to be re-sent on timeout

 public:
    Station_A();
    virtual ~Station_A();

  protected:
    virtual void generateNewMessage();
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Station_A);

Station_A::Station_A()
{
    timeoutEvent = message = nullptr;
}
Station_A::~Station_A()
{
    cancelAndDelete(timeoutEvent);
    delete message;
}

void Station_A::initialize()
{
    timeout = 0.5;
    timeoutEvent = new cMessage("timeoutEvent");
    EV << "Sending initial message form Station_A \n";
    WATCH(seq);
    generateNewMessage();
    scheduleAt(simTime()+timeout, timeoutEvent);
}

void Station_A::handleMessage(cMessage *msg)
{
   if (msg == timeoutEvent)
        {
           EV << "Timeout expired, resending message and restarting timer\n";
           seq = seq - temp1;
           generateNewMessage();
           scheduleAt(simTime()+timeout, timeoutEvent);
        }
   else
       {
           temp1 = msg->par("WINSIZE").longValue();

           EV << "Received by handle message of Station_A : " << msg -> getName() << endl;
           delete msg;
           EV << "Timer cancelled.\n";
           cancelEvent(timeoutEvent);
           generateNewMessage();
           scheduleAt(simTime()+timeout, timeoutEvent);
       }
}

void Station_A::generateNewMessage()
{
    if(seq == 0)
    {
        int tempSeq = seq;
        char msgname[20];
        sprintf(msgname, "msg-%d\n", tempSeq);
        cMessage *msg = new cMessage(msgname);
        msg->addPar("SEQ");
        msg->par("SEQ").setLongValue(tempSeq);
        EV <<"'Inside if loop of generate New Message Station_A side, SEQ value = " << msg->par("SEQ").longValue() << "\n";
        seq = seq + 1;
        send(msg,"out");
    }
    else
    {
        for(i=0; i< temp1; i++)
        {
            char msgname[20];
            sprintf(msgname, "msg-%d\n", seq);
            cMessage *msg = new cMessage(msgname);
            msg->addPar("SEQ");
            msg->par("SEQ").setLongValue(seq);
            send(msg,"out");
            seq = seq + 1;
        }
    }
}

class Station_B : public cSimpleModule
{

  private:
    int windowsize;
    int temp;
    int counter = 0;
    cMessage *message;

  protected:
    virtual void handleMessage(cMessage *msg) override;
    virtual void initialize() override;
    virtual void generateNewMessage();
};

Define_Module(Station_B);

void Station_B::initialize()
{
    windowsize = par("window");
}

void Station_B::handleMessage(cMessage *msg)
{
   if (uniform(0, 1) < 0.1)
      {
            EV << "\"Losing\" message " << msg << endl;
            bubble("message lost");
            counter=0;
            delete msg;
      }

}
void Station_B::generateNewMessage()
{
if(temp == 0)
    {
        char msgname[20];
        sprintf(msgname,"Station_B window size is - %d\n",windowsize);
        cMessage *msg = new cMessage(msgname);
        msg->addPar("WINSIZE");
        msg->par("WINSIZE").setLongValue(windowsize);
        EV << "'Window size in Station_B is = " << msg->par("WINSIZE").longValue() << endl;
        send(msg, "out");
    }









//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Node.h"
#include "MyMessage_m.h"
#include <iostream>
#include <fstream>

Define_Module(Node);
#define MAX_SEQ 9999 //to make debugging easier

void Node::initialize()
{
    // TODO - Generated method body
    //intializations
    sender = -1;
    buffersUsed = 0;
    //frameNumber = 0; //Assumption: frame number of 1st sent frame is 1
    frameExpected = 0;
    WR = par("WR").intValue();
    WS = par("WS").intValue();
    //WR = 5;
    tooFar = WR; //Note that last frame I'll send according to this value is "WR - 1" cuz of how between function works
    ackExpected = 0;
    nextFrameToSend = 0;
    processingTime = par("PT").doubleValue();
    //processingTime = 5;
    noNak = true;
    for (int i = 0; i < WR;i++)
    {
        std::string dummyTimeout = "dummy timeout";
        cMessage *dummyTimeoutMsg = new cMessage(dummyTimeout.c_str());
        arrived.push_back(false);
        buffers.push_back("empty"); //to initialize the vector with (WR) elements. Otherwise trying to access buffers[3] later on would return segmentation error
        timeouts.push_back(dummyTimeoutMsg); //to initialize the vector with (WR) elements. Otherwise trying to access timeouts [3] later on would return segmentation error
    }
    TO = par("TO").doubleValue();
    //TO = 5;
}

//checking if  a<=b<c (NOT a<=b<=c)
bool between(int a,int b,int c)
{
    if (c > a)
    {
        if (a<=b && b < c)
        {
            return true;
        }
    }
    else if ( c < a)
    {
        if (a <= b)
        {
            return true;
        }
        if (b < c)
        {
            return true;
        }
    }
    return false;
}
void sendData(int frameKind, int frameNumber, int frameExpected, cMessage *msg )
{
    //0 for data, 1 for ACK, 2 for NAK, 3 for Coordinator initialize, think of this as part 2 of header
    //TODO: implement sending in this function. Ana bas 5ayf a send a cMessage as a parameter fe function, dont know if haye7sal ay moshkla

}

//comparing given value with MAX_SEQ to ensure frameExpected, tooFar, ackExpected, nextFrameToSend are between 0,(MAX_SEQ-1)
int circularIncrement(int x)
{
    x++;
    if (x == MAX_SEQ)
    {
        x = 0;
    }
    return x;
}

void Node::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    MyMessage_Base *mmsg = check_and_cast<MyMessage_Base *>(msg);
        EV<<"received message with sequence number ...   ";
        EV << mmsg->getSeq_Num();
        EV<<"  and payload of ... ";
        EV<< mmsg->getM_Payload();
        EV<<"   and check bits of ...";
        EV<< mmsg->getMycheckbits().to_string();

        if(msg->isSelfMessage())
        {
            //this must mean sender is sending itself timeout signal because it didnt receive ACK
            if(sender != 1)
            {
                //For debugging bas
                //if sender variable is not set correctly hena yeb2a akeed fe 7aga 8alat
                std::cout << "Fe 7aga 8alat, either receiver is sending timeout which should never happen or sender variable not set correctly"<<endl;
            }
            else
            {
                //Now I'm sure sender==1
                //I should resend the lower edge of the sender window
                std::string payload = buffers[ackExpected%WR]; //retrieving frame at lower edge of sender window
                MyMessage_Base * msgSend = new MyMessage_Base(payload.c_str());
                msgSend->setM_Type(0); //sender is sending data
                msgSend->setM_Payload(payload.c_str());
                msgSend->setSeq_Num(ackExpected); //setting sequence number to be sequence number of frame at lower edge of sender window
                sendDelayed(msgSend,processingTime,"out"); //For now ignore this TODO, initially sendDelayed wasnt working with our custom MyMessage_Base class bas el7 it started working lewa7daha TODO: exchange every sendDelayed with scheduleAt
            }
        }
        if (sender == -1) //kda I still don't know if I'm sender or receiver
        {
            //0 for data, 1 for ACK, 2 for NAK, 3 for Coordinator initialize, think of this as part 2 of header
            if (mmsg->getM_Type() == 3)
            {
                //then I'm the sender
                sender = 1;
                // Compare your name with node0
                if (strcmp(getName(), "node0") == 0)
                {
                    // Open node0.txt
                    nodeNumber = 0;
                    //std::ifstream file("node0.txt");
                    //fileVar = file;
                    file.open("node0.txt");
                }
                else if(strcmp(getName(),"node1") == 0)
                {
                    // Open node1.txt
                    nodeNumber = 1;
                    //std::ifstream file("node1.txt");
                    //fileVar = file;
                    file.open("node1.txt");

                }
                else
                {
                    std::cout << "Fe 7aga 8alat,node name not set correctly" << endl;
                }
            }
            else
            {
                sender = 0;
            }

        }

        if (sender == 1)
        {
 //sender code

            //ignore the counter variable, I was experimenting with a different approach
            //int counter = 1; //this counter will be used in while loop to calculate processing time for each frame

            //while number of buffers used less than window size (so I still have empty buffers) and I havent reached end of file yet, read a new line/message
                while (buffersUsed < par("WS").intValue() && !(file.eof()))
                {
                    std::string errorCode;
                    std::string payload;
                    std::string line;
                    // Read the error code and payload
                    std::getline(file, line);
                    errorCode = line.substr(0, 3);
                    payload = line.substr(5, line.length() - 5);
                    //store string I should send in string vector in case I need to send it tany cuz of timeout
                    //since sender is "storing" frame from file
                    buffers[nextFrameToSend%WR] = payload;
                    buffersUsed++;
                    // Set the error code and payload
                    MyMessage_Base * msgSend = new MyMessage_Base(payload.c_str());
                    //0 for data, 1 for ACK, 2 for NAK, 3 for Coordinator initialize, think of this as part 2 of header
                    msgSend->setM_Type(0); //sender is sending data
                    msgSend->setM_Payload(payload.c_str());
                    msgSend->setSeq_Num(nextFrameToSend);
                    //TODO:calculate checksum
                    //msgSend->setMycheckbits(mycheckbits)
                    //TODO:byte stuffing

                    sendDelayed(msgSend,processingTime,"out"); //For now ignore this TODO, initially sendDelayed wasnt working with our custom MyMessage_Base class bas el7 it started working lewa7daha TODO: exchange every sendDelayed with scheduleAt
                    //TODO: can I cascade sendDelayed function? (el moshkla ely shara7taha fel voice note)

                    //Ignore 2 lines below, I was experimenting
                   // msgSend->setName("Frame to send");
                    //scheduleAt(simTime()+ processingTime, msgSend);


                    //scheduling timer
                    //TODO: set frame number in frame timeout or check if i dont need to
                    std::string timer = "frame timeout";
                    cMessage *timermsg = new cMessage(timer.c_str());
                    scheduleAt(simTime()+TO, timermsg);
                    timeouts[nextFrameToSend%WR] = timermsg;
                    nextFrameToSend = circularIncrement(nextFrameToSend); //sender received frame from "Network Layer" so should advance upper edge of window
                }

                //0 for data, 1 for ACK, 2 for NAK, 3 for Coordinator initialize, think of this as part 2 of header
                //received ACK
                if(mmsg->getM_Type() == 1)
                {
                    //TODO:turn this to ACK for each frame
                    //while loop to keep acknowledging if I received cumulative ACK
                     while(between(ackExpected,mmsg->getAck_Num(),nextFrameToSend))
                     {
                         buffersUsed--;
                         buffers[ackExpected%WR] = "no longer using this buffer";
                         cancelEvent(timeouts[ackExpected%WR]); //Canceling timer since I received frame
                         ackExpected = circularIncrement(ackExpected); //sender received in order ack so should advance lower edge of window
                     }
                     //reached end of file and acknowledged kolo
                     if(ackExpected == nextFrameToSend && (file.eof()))
                     {
                         endSimulation();
                     }
                }
                //0 for data, 1 for ACK, 2 for NAK, 3 for Coordinator initialize, think of this as part 2 of header
                //received NAK
                if(mmsg->getM_Type() == 2)
                {
                    //checking NAK is within sender window
                    if (between(ackExpected,mmsg->getAck_Num(),nextFrameToSend))
                    {
                        //I should resend the frame the receiver is requesting
                        std::string payload = buffers[(mmsg->getAck_Num())%WR]; //retrieving frame receiver is requesting from buffer
                        MyMessage_Base * msgSend = new MyMessage_Base(payload.c_str());
                        msgSend->setM_Type(0); //sender is sending data
                        msgSend->setM_Payload(payload.c_str());
                        msgSend->setSeq_Num(mmsg->getAck_Num()); //setting sequence number to sequence number receiver is requesting
                        sendDelayed(msgSend,processingTime,"out"); //For now ignore this TODO, initially sendDelayed wasnt working with our custom MyMessage_Base class bas el7 it started working lewa7daha TODO: exchange every sendDelayed with scheduleAt

                    }
                }

        }
        else if (sender == 0)
        {
            //receiver code
                //0 for data, 1 for ACK, 2 for NAK, 3 for Coordinator initialize, think of this as part 2 of header
                if (mmsg->getM_Type() == 0)
                {
                    //checking if received message is out of order and noNak is true (meaning we still havent sent a NAK)
                    //if (mmsg->getSeq_Num() != frameExpected && noNak)\
                    //TODO: Ask TAs if we should use noNak boolean variable, wala I can send multiple NAKs wara b3d 3ady
                    if (mmsg->getSeq_Num() != frameExpected)
                    {
                        noNak = false;
                        std::string dummy = "this frame is NAK";
                        MyMessage_Base * msgSendFromReceiver = new MyMessage_Base(dummy.c_str());
                        msgSendFromReceiver->setM_Type(2); //2 cuz we're sending NAK
                        msgSendFromReceiver->setM_Payload(dummy.c_str());
                        msgSendFromReceiver->setSeq_Num(0); //sequence number for NAK is irrelevant
                        msgSendFromReceiver->setAck_Num(frameExpected); //setting NAK number to be frameExpeced cuz that is the frame receiver should request sender to resend
                        send(msgSendFromReceiver,"out"); //No delay for NAK
                    }
                    else
                    {
               //Nevermind, ignore these comments no need for Ack timeout
                        //check if I need ack number hena
                        //No need to set ack number just resend ack
                        //scheduleAt(simTime()+TO, "Ack timeout");
                    }
                    //checking if received frame is within receiver window
                    //AND checking if it has arrived previously or not
                    //Note: msg sequence number will be between 0,MAX_SEQ. same for frameExpected,tooFar,
                    if (between(frameExpected,mmsg->getSeq_Num(),tooFar) && !(arrived[mmsg->getSeq_Num()%WR]))
                    {
                        bool modified = false;
                        //TODO: check if modified error occured if so then send NAK for errored frame
                        if(!modified)
                        {
                            arrived[mmsg->getSeq_Num()%WR] = true;
                            buffers[mmsg->getSeq_Num()%WR] = mmsg->getM_Payload(); //storing message we've received
                            //if lower edge of receiver window has arrived then I will advance receiver buffer
                            //TODO: trace en the while condition tamam
                            while (arrived[frameExpected%WR])
                            {
                                //Received in order frame so reset Nak boolean
                                noNak = true;
                                arrived[mmsg->getSeq_Num()%WR] = false; //to use it for next batch
                                buffers[mmsg->getSeq_Num()%WR] = "receiver buffer no longer in use"; //storing message we've received
                                frameExpected = circularIncrement(frameExpected);
                                tooFar = circularIncrement(tooFar); //advancing window of receiver since we received in order frame
                                std::string dummy = "this frame is ACK";
                                int ackNumber = frameExpected; //since if we were expecting frame 2 and frame 2 was received then I should send ack for frame 3. Note that ackNumber is set after frameExpected is incremented to achieve this
                                MyMessage_Base * msgSendFromReceiver = new MyMessage_Base(dummy.c_str());
                                msgSendFromReceiver->setM_Type(1); //1 cuz we're sending ACK
                                msgSendFromReceiver->setM_Payload(dummy.c_str());
                                msgSendFromReceiver->setSeq_Num(0); //sequence number for ACK is irrelevant (since receiver doesnt send back data)
                                msgSendFromReceiver->setAck_Num(ackNumber);
                                send(msgSendFromReceiver,"out"); //No delay for ACK
                            //Nevermind no need for Ack timeout
                                //schedule ack timer
                            }

                        }
                        else
                        {
                            //modified error occured so should send NAK
                            std::string dummy = "this frame is NAK";
                            MyMessage_Base * msgSendFromReceiver = new MyMessage_Base(dummy.c_str());
                            msgSendFromReceiver->setM_Type(2); //2 cuz we're sending NAK
                            msgSendFromReceiver->setM_Payload(dummy.c_str());
                            msgSendFromReceiver->setSeq_Num(0); //sequence number for NAK is irrelevant
                            msgSendFromReceiver->setAck_Num(mmsg->getSeq_Num()); //setting NAK number to be frame number of the received frame that had modified error
                            send(msgSendFromReceiver,"out"); //No delay for NAK
                        }


                    }

                }
                else
                {
                    std::cout <<"Fe 7aga 8alat, receiver should only receive data not ACK or NAK" << endl;
                }
        }
        else
        {
            std::cout << "Fe 7aga 8alat,sender variable not set correctly" << endl;
        }


}

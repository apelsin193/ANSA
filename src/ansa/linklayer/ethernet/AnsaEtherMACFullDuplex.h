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

#ifndef ANSAETHERMACFULLDUPLEX_H_
#define ANSAETHERMACFULLDUPLEX_H_

#include "AnsaEtherMACBaseVF.h"
#include "EtherMACBase.h"

class INET_API AnsaEtherMACFullDuplex : public AnsaEtherMACBaseVF
{
  public:
    AnsaEtherMACFullDuplex();

  protected:
    virtual void initialize();
    virtual void initializeStatistics();
    virtual void initializeFlags();
    virtual void handleMessage(cMessage *msg);

    // finish
    virtual void finish();

    // event handlers
    virtual void handleEndIFGPeriod();
    virtual void handleEndTxPeriod();
    virtual void handleEndPausePeriod();
    virtual void handleSelfMessage(cMessage *msg);

    // helpers
    virtual void startFrameTransmission();
    virtual void processFrameFromUpperLayer(EtherFrame *frame);
    virtual void processMsgFromNetwork(EtherTraffic *msg);
    virtual void processReceivedDataFrame(EtherFrame *frame);
    virtual void processPauseCommand(int pauseUnits);
    virtual void scheduleEndIFGPeriod();
    virtual void scheduleEndPausePeriod(int pauseUnits);
    virtual void beginSendFrames();


    // statistics
    simtime_t totalSuccessfulRxTime; // total duration of successful transmissions on channel
};

#endif /* ANSAETHERMACFULLDUPLEX_H_ */

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

#ifndef __INET_EIGRPRTP_H_
#define __INET_EIGRPRTP_H_

#include <omnetpp.h>

#include "EigrpMsgReq.h"
#include "EigrpInterfaceTable.h"
#include "EigrpIpv4NeighborTable.h"

class EigrpRtp;

/**
 * Queue for storing requests for message sending.
 */
class EigrpRequestQueue: public cObject
{
  private:
    typedef std::list<EigrpMsgReq *> MessageQueue;

    MessageQueue reqQueue;          /**< Queue with requests waiting for sending (rel/unrel) */

    // Only for debugging
    //friend class EigrpRtp;

  public:
    virtual ~EigrpRequestQueue();

    void pushReq(EigrpMsgReq *req);
    EigrpMsgReq *findReqByIf(int ifaceId, bool sent = true);
    EigrpMsgReq *findReqByNeighbor(int neighId, bool sent = true);
    EigrpMsgReq *findUnrelReqByIf(int ifaceId);
    EigrpMsgReq *findReqBySeq(uint32_t seqNumber);
    EigrpMsgReq *removeReq(EigrpMsgReq *msgReq);
    void removeAllMsgsToIf(int ifaceId);
    void removeAllMsgsToNeigh(int ifaceId);
    int getNumReq() const { return reqQueue.size(); }

    void printInfo() const;
};

/**
 * Class represents Reliable Transport Protocol for reliable transmission of EIGRP messages.
 */
class EigrpRtp : public cSimpleModule
{
    struct NeighborInfo
    {
        int neighborId;
        int neighborIfaceId;
        uint32_t lastSeqNum;
        int numOfAck;
    };
    typedef std::list<EigrpMsgReq *> MessageQueue;

    const char *RTP_OUTPUT_GW;

    uint32_t seqNumber;             /**< Sequence number for reliable transport of messages */

    EigrpRequestQueue *requestQ;

    EigrpInterfaceTable *eigrpIft;
    EigrpIpv4NeighborTable *eigrpNt;

    /**
     * Process request for message sending.
     */
    void processRequest(cMessage *msg);
    /**
     * Process message header for ensuring reliable transmission.
     */
    void processHeader(cMessage *msg);

    /**
     * Schedule sending next reliable/unreliable message in transmission queue.
     */
    void scheduleNextMsg(int ifaceId);
    /**
     * Schedule new request for sending message.
     */
    void scheduleNewMsg(EigrpMsgReq *msgReq);
    /**
     * Send reliable/unreliable message
     */
    void sendMsg(EigrpMsgReq *msgReq);
    /**
     * Send reliable message.
     */
    void sendRelMsg(EigrpMsgReq *msgReq);
    /**
     * Send unreliable message.
     */
    void sendUnrelMsg(EigrpMsgReq *msgReq);
    /**
     * Delete request.
     */
    void discardMsg(EigrpMsgReq *msgReq);

    /**
     * Return informations about neighbor.
     */
    EigrpNeighbor<IPv4Address> *getNeighborId(EigrpMessage *msg);
    /**
     * Send message with specified acknowledge number to neighbor.
     */
    void acknowledgeMsg(int neighId, int ifaceId, uint32_t ackNum);

  protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual int numInitStages() const { return 4; }

  public:
    EigrpRtp();
    virtual ~EigrpRtp();

};

#endif

// Copyright (c) 2018 The Luxcore developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef LUXGATE_H
#define LUXGATE_H

#include <blockchainclient.h>
#include <main.h>
#include <serialize.h>
#include <uint256.h>
#include <util.h>

class COrder;

typedef uint256 OrderId; 
typedef std::pair<OrderId, std::shared_ptr<COrder>> OrderEntry;
typedef std::map<OrderId, std::shared_ptr<COrder>> OrderMap;

extern OrderMap orderbook;
extern OrderMap activeOrders;

void ProcessMessageLuxgate(CNode* pfrom, const std::string& strCommand, CDataStream& vRecv, bool& isLuxgate);

/**
 * @brief Check for LuxGate service bit
 * @param pfrom Node to check for LuxGate support
 * @return LuxGate support flag
 */
bool IsLuxGateServiceSupported(const CNode* pfrom);

class COrder 
{

public:
    COrder() {}
    COrder(Ticker base, Ticker rel, CAmount baseAmount, CAmount relAmount) : 
                    base(base), rel(rel), baseAmount(baseAmount), relAmount(relAmount) {}
    COrder(const COrder&) = default;

    OrderId ComputeId() const;
    CAddress Sender() const { return sender; }
    void SetSender(CAddress addr) { sender = addr; }
    bool SenderIsValid() const;
    Ticker Base() const { return base; }
    Ticker Rel() const { return rel; }
    CAmount BaseAmount() const { return baseAmount; }
    CAmount RelAmount() const { return relAmount; }

    bool Matches(const COrder& order) const;

    std::string ToString() const;

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion) {
        READWRITE(base);
        READWRITE(rel);
        READWRITE(baseAmount);
        READWRITE(relAmount);
        READWRITE(sender);
    }

    bool operator==(const COrder &o) const 
    {
        return base == o.base && rel == o.rel && baseAmount == o.baseAmount && relAmount == o.relAmount;
    }

protected:

    Ticker base;
    Ticker rel;
    CAmount baseAmount;
    CAmount relAmount;
    CAddress sender;
};


#endif
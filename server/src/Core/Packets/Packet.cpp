#include "Packet.h"

std::string GetPacketMessage(DataPacket &packet)
{
     return std::string(packet.data.begin(), packet.data.end());
 }

std::string GetWholePacketInfo(DataPacket &packet)
{
    std::string ownerId = std::to_string(packet.ownerId);
    std::string dataType = std::to_string(packet.dataType);
    std::string timeStamp = std::to_string(packet.timeStamp);
    return "[" + timeStamp + "] [" + ownerId + "][" + dataType + "]" + GetPacketMessage(packet);
}
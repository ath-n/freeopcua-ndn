/// @author Athreya H N
/// @email indiathreya92@gmail.com
/// @author Dr. Mohit P. Tahiliani
/// @email tahiliani@nitk.edu.in
/// @brief OPC binary NDN API.
/// @license GNU LGPL
///
/// Distributed under the GNU LGPL License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/lgpl.html)

#ifndef FREEOPCUA_NDN_CHANNEL_H
#define FREEOPCUA_NDN_CHANNEL_H

#include <opc/ua/protocol/channel.h>
#include <ndn-cxx/face.hpp>

namespace OpcUa
{

class NdnChannel : public OpcUa::IOChannel
{
public:
  NdnChannel(const ndn::Name _namespace, ndn::Face face);

  virtual std::size_t Receive(char * data, std::size_t size);
  virtual void Send(const char* message, std::size_t size);

private:
  ndn::Name m_namespace;
  ndn::Face& m_face;
  ndn::KeyChain m_keyChain;
  uint64_t m_counter;
  uint64_t m_recv_counter;
  void onData(const ndn::Data& data);
  void onNack(const ndn::Interest& interest);
  void onTimeout(const ndn::Interest& interest);
};

}
#endif //FREEOPCUA_NDN_CHANNEL_H

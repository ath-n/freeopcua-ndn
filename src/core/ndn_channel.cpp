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

#include <opc/ua/ndn_channel.h>
#include <opc/ua/ndn_receiver.h>
#include <opc/ua/errors.h>

#include <iostream>
#include <stdexcept>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/data.hpp>

OpcUa::NdnChannel::NdnChannel (const std::string &_namespace, ndn::Face &face)
  : m_namespace(_namespace)
  , m_face(face)
{
}

std::size_t OpcUa::NdnChannel::Receive(const ndn::Data &data, std::size_t size)
{
  OpcUa::NdnReceiver receiver(m_face, m_namespace);
  std::string str_data = receiver.RequestData ();

  return 0;
}

void OpcUa::NdnChannel::Send(const char *message, std::size_t size)
{
  std::shared_ptr<ndn::Data> data = std::make_shared<ndn::Data>(message);
  std::ostringstream os;
  os << "C++ LINE #" << (m_counter++) << std::endl;
  std::string content = os.str();
  data->setContent(reinterpret_cast<const uint8_t*>(content.c_str()), content.size());
  data->setFreshnessPeriod(ndn::time::seconds(10));
  m_keyChain.sign(*data);
  m_face.put(*data);
}
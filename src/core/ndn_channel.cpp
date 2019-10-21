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
#include <opc/ua/errors.h>
#include <iostream>
#include <stdexcept>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/data.hpp>
#include <python3.6m/Python-ast.h>

OpcUa::NdnChannel::NdnChannel (const ndn::Name _namespace, ndn::Face face)
  : m_face(face)
  , m_namespace(_namespace)
{
}
std::size_t OpcUa::NdnChannel::Receive(char * data, std::size_t size)
{
  ndn::Name nextName = m_namespace.appendSequenceNumber (m_recv_counter);
  std::cerr << ">> C++ " << nextName << std::endl;

  m_face.expressInterest(ndn::Interest(nextName).setMustBeFresh(true),
                         std::bind(&OpcUa::NdnChannel::onData, this, _2),
                         std::bind(&OpcUa::NdnChannel::onNack, this, _1),
                         std::bind(&OpcUa::NdnChannel::onTimeout, this, _1));
  ++m_recv_counter;
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

void
OpcUa::NdnChannel::onData(const ndn::Data& data)
{
  std::cerr << "<< C++ "
    << std::string(reinterpret_cast<const char*>(data.getContent().value()),
                   data.getContent().value_size())
    << std::endl;

  if (data.getName().get(-1).toSequenceNumber() >= 10) {
      return;
    }

  Receive(std::string(reinterpret_cast<const char*>(data.getContent().value()), data.getContent().value_size()));
}

void
OpcUa::NdnChannel::onNack(const ndn::Interest& interest)
{
  std::cerr << "<< got NACK for " << interest << std::endl;
}

void
OpcUa::NdnChannel::onTimeout(const ndn::Interest& interest)
{
  // re-express interest
  std::cerr << "<< C++ timeout for " << interest << std::endl;
   m_face.expressInterest(ndn::Interest(interest.getName()),
                         std::bind(&OpcUa::NdnChannel::onData, this, _2),
                         std::bind(&OpcUa::NdnChannel::onNack, this, _1),
                         std::bind(&OpcUa::NdnChannel::onTimeout, this, _1));
}
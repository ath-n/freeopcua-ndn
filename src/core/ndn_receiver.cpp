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

OpcUa::NdnReceiver::NdnReceiver (ndn::Face &face, const std::string &_namespace)
  : m_face (face), m_baseName (_namespace), m_currentSeqNo (0), m_data ("")
{
  std::cerr << "Base name: " << m_baseName << std::endl;
}

const char * OpcUa::NdnReceiver::RequestData() {
  requestNext ();
  return m_data.c_str ();
}

void
OpcUa::NdnReceiver::requestNext ()
{
  ndn::Name nextName = ndn::Name (m_baseName).appendSequenceNumber (m_currentSeqNo);

  m_face.expressInterest (ndn::Interest (nextName).setMustBeFresh (true),
                          std::bind (&NdnReceiver::onData, this, _2),
                          std::bind (&NdnReceiver::onNack, this, _1),
                          std::bind (&NdnReceiver::onTimeout, this, _1));
  ++m_currentSeqNo;

}

void
OpcUa::NdnReceiver::onData (const ndn::Data &data)
{

  m_data = m_data + std::string (reinterpret_cast<const char *>(data.getContent ().value ()),
                                 data.getContent ().value_size ());

  if (data.getName ().get (-1).toSequenceNumber () >= 10)
    {
      return;
    }

  requestNext ();
}

void
OpcUa::NdnReceiver::onNack (const ndn::Interest &interest)
{
  std::cerr << "<< got NACK for " << interest << std::endl;
}

void
OpcUa::NdnReceiver::onTimeout (const ndn::Interest &interest)
{
  // re-express interest
  std::cerr << "<< C++ timeout for " << interest << std::endl;
  m_face.expressInterest (ndn::Interest (interest.getName ()),
                          std::bind (&NdnReceiver::onData, this, _2),
                          std::bind (&NdnReceiver::onNack, this, _1),
                          std::bind (&NdnReceiver::onTimeout, this, _1));
}
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

#ifndef FREEOPCUA_NDN_NDN_RECEIVER_H
#define FREEOPCUA_NDN_NDN_RECEIVER_H

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/data.hpp>

#include <iostream>
#include <string>
namespace OpcUa {

class NdnReceiver
{
public:
  NdnReceiver (ndn::Face &face, const std::string &_namespace);
  std::string RequestData();

private:
  void requestNext ();
  void onData (const ndn::Data &data);
  void onNack (const ndn::Interest &interest);
  void onTimeout (const ndn::Interest &interest);

  ndn::Face &m_face;
  ndn::Name m_baseName;
  uint64_t m_currentSeqNo;
  std::string m_data;
};
}
#endif //FREEOPCUA_NDN_NDN_RECEIVER_H

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_PACKET_WRITER_H_
#define NET_QUIC_QUIC_PACKET_WRITER_H_

#include "net/base/ip_endpoint.h"

namespace net {

struct WriteResult;

// An interface between writers and the entity managing the
// socket (in our case the QuicDispatcher).  This allows the Dispatcher to
// control writes, and manage any writers who end up write blocked.
class NET_EXPORT_PRIVATE QuicPacketWriter {  //数据真正的发送处理在类QuicSocketUtils
//QuicConnection中包含该类成员writer_，QuicConnection.writer_赋值为 QuicPacketWriter* writer = new QuicDefaultPacketWriter(fd_);
 public:
  virtual ~QuicPacketWriter() {}

  // Sends the packet out to the peer.  If the write succeeded, the result's
  // status is WRITE_STATUS_OK and bytes_written is populated. If the write
  // failed, the result's status is WRITE_STATUS_BLOCKED or WRITE_STATUS_ERROR
  // and error_code is populated.
  //通过udp套接字发送数据  QuicSocketUtils::WritePacket，真正的数据发送在这里
  virtual WriteResult WritePacket(  //WritePacketInner  SendVersionNegotiationPacket中调用
      const char* buffer, size_t buf_len,
      const IPAddressNumber& self_address,
      const IPEndPoint& peer_address) = 0;

  // Returns true if the writer buffers and subsequently rewrites data
  // when an attempt to write results in the underlying socket becoming
  // write blocked.
  virtual bool IsWriteBlockedDataBuffered() const = 0;

  // Returns true if the network socket is not writable.
  virtual bool IsWriteBlocked() const = 0;

  // Records that the socket has become writable, for example when an EPOLLOUT
  // is received or an asynchronous write completes.
  virtual void SetWritable() = 0;
};

}  // namespace net

#endif  // NET_QUIC_QUIC_PACKET_WRITER_H_

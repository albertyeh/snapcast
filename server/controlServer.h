/***
    This file is part of snapcast
    Copyright (C) 2014-2019  Johannes Pohl

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef CONTROL_SERVER_H
#define CONTROL_SERVER_H

#include <asio.hpp>
#include <memory>
#include <mutex>
#include <set>
#include <sstream>
#include <thread>
#include <vector>

#include "common/queue.h"
#include "common/sampleFormat.h"
#include "controlSession.h"
#include "message/codecHeader.h"
#include "message/message.h"
#include "message/serverSettings.h"

using asio::ip::tcp;

/// Telnet like remote control
/**
 * Telnet like remote control
 */
class ControlServer : public ControlMessageReceiver
{
public:
    ControlServer(asio::io_context* io_context, size_t port, ControlMessageReceiver* controlMessageReceiver = nullptr);
    virtual ~ControlServer();

    void start();
    void stop();

    /// Send a message to all connceted clients
    void send(const std::string& message, const ControlSession* excludeSession = nullptr);

    /// Clients call this when they receive a message. Implementation of MessageReceiver::onMessageReceived
    void onMessageReceived(ControlSession* connection, const std::string& message) override;

private:
    void startAccept();
    void handleAccept(tcp::socket socket);
    void cleanup();

    mutable std::recursive_mutex session_mutex_;
    std::vector<std::weak_ptr<ControlSession>> sessions_;
    std::shared_ptr<tcp::acceptor> acceptor_v4_;
    std::shared_ptr<tcp::acceptor> acceptor_v6_;

    asio::io_context* io_context_;
    size_t port_;
    ControlMessageReceiver* controlMessageReceiver_;
};



#endif

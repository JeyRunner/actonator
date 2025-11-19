/* 
 * Author: Joshua Johannson
 *
  */
#pragma once


#include <iostream>
#include <chrono>
#include <ctime>
#include <client_ws.hpp>

using namespace std;
using WsClient = SimpleWeb::SocketClient<SimpleWeb::WS>;

/*
 * StreamWebsocket class
 */
class StreamWebsocket
{
  private:
    WsClient client;
    shared_ptr<WsClient::Connection> connection = nullptr;
    shared_ptr<thread> client_thread = nullptr;
    chrono::system_clock::time_point time_start;

  public:
    StreamWebsocket(): client("localhost:9871") {
    }

    void start() {
      time_start = chrono::system_clock::now();

      client.on_open = [&](shared_ptr<WsClient::Connection> connection) {
        cout << "Websocket streaming: Opened connection" << endl;
        this->connection = connection;
      };

      client.on_close = [&](shared_ptr<WsClient::Connection> /*connection*/, int status, const string & /*reason*/) {
        cout << "Websocket streaming: Closed connection with status code " << status << endl;
        this->connection = nullptr;
      };

      client.on_message = [](shared_ptr<WsClient::Connection> connection, shared_ptr<WsClient::InMessage> in_message) {
        cout << "Client: Message received: \"" << in_message->string() << "\"" << endl;
      };


      // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
      client.on_error = [](shared_ptr<WsClient::Connection> /*connection*/, const SimpleWeb::error_code &ec) {
        cout << "Client: Error: " << ec << ", error message: " << ec.message() << endl;
      };

      client_thread = make_shared<thread>([&]() {
        cout << "will connect to websocket server at 'localhost:9871'" << endl;
        client.start();
      });
    }


    template<class T>
    void sendValue(string name, T value) {
      if (!connection) {
        return;
      }
      chrono::duration<double> timeSeconds= chrono::system_clock::now() - time_start;
      //to_string(timeSeconds.count())

      //auto time = chrono::system_clock::now().time_since_epoch();
      //auto secs = chrono::duration_cast<chrono::microseconds>(time);
      //cout << "t: " << secs.count() << endl;

      string data;
      data += "{";
      data += "\"timestamp\": " + to_string(timeSeconds.count()) + ", ";
      data += "\"t\": " + to_string(timeSeconds.count()) + ", ";
      data += "\"t\": " + to_string(timeSeconds.count()) + ", ";
      data += "\"data\": {";
      data += "  \"" + name + "\": " + to_string(value) + " ";
      data += "}}";
      //cout << "send: " << data << endl;
      connection->send(data);
    }
};

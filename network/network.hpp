#ifndef NETWORK_HPP
#define NETWORK_HPP
#include <string>
/* Abstraction layer for networking modules
 * For now I am going to implement the bare minimum (UDP only)*/
/* udpConnection notes
 * -------------------
 * You must both bind and connect before you can send or receive*/
class udpConnection
{
	private:

	public:
		//This connection will listen to the requested port; this is also needed to send
		bool bind(unsigned int);
		//Messages from this connection will be sent to the requested IP address & port
		void connect(const std::string&, unsigned int);

		//Sends data to the connected address through the bound port
		bool send(const std::string&);
		//Returns a string from the bound port; note that this will not
		//ignore messages from other connections
		std::string receive();
};
#endif

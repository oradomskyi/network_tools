#include "../../include/networks/SocketGLIBC.h"

SocketGLIBC::SocketGLIBC()
    : Network()
	, address("")
	, port(0)
    , m_socket(network::SocketState::SOCKET_ERROR)
{}

SocketGLIBC::SocketGLIBC(const string& address, const uint16_t& port)
    : Network()
	, address(address)
	, port(port)
    , m_socket(network::SocketState::SOCKET_ERROR)
{
        cout << "SocketGLIBC ctor" << endl;
}

SocketGLIBC::~SocketGLIBC()
{
    this->Shutdown();
    cout << "SocketGLIBC dtor" << endl;
}

network::Result SocketGLIBC::Create()
{
	// taken from here
	// https://www.gnu.org/software/libc/manual/html_node/Byte-Stream-Example.html

    cout << "SocketGLIBC create" << endl;

	// TCP SOCK_STREAM
	// UDP SOCK_DGRAM

	// make socket
    int _socket = socket(PF_INET, SOCK_STREAM, 0); // todo move to constants
	
   	cout << "socket is " << _socket << endl;

    if(network::SocketState::SOCKET_ERROR >= _socket)
    {
        this->state = network::State::ERROR;
   		cout << "err socket is " << _socket << endl;

        return network::Result::RESULT_ERROR;
    }
    this->m_socket = _socket;
  	this->m_servername.sin_family = AF_INET;
  	this->m_servername.sin_port = htons (this->port);
    	
    this->state = network::State::CREATED;
    cout<<"create state " << this->state << endl;

    return network::Result::RESULT_OK;
}

network::Result SocketGLIBC::Resolve()
{
	struct hostent* hostinfo = gethostbyname (this->address.c_str());
	cout<<"host addr " << this->address << ", h_name " << hostinfo->h_name<< endl;
  	if (hostinfo == nullptr)
    {
		this->setState(network::State::ERROR);
 		cout<<"Unknown host " << this->address << endl;
		return network::Result::RESULT_ERROR;
    }

	this->m_servername.sin_addr = *(struct in_addr *) hostinfo->h_addr;

	this->state = network::State::HOST_RESOLVED;
    return network::Result::RESULT_OK;
}

network::Result SocketGLIBC::Connect()
{
    cout << "SocketGLIBC conn" << endl;
	
	if(network::State::HOST_RESOLVED == this->state)
	{
		cout << "SocketGLIBC conn network::State::READY" << endl;
		cout << this->m_servername.sin_family <<" "<<this->m_servername.sin_port << " "<< endl;///this->m_servername.sin_addr<<" " << endl;
		int err = connect(this->m_socket
			, (struct sockaddr *) &this->m_servername
			, sizeof (this->m_servername));

		cout << err << endl;
    	if (network::SocketState::SOCKET_ERROR >= err)
    	{
			this->state = network::State::ERROR;
		  	cout << "SocketGLIBC conn failed :( " << endl;
			return network::Result::RESULT_ERROR;
    	}
	}
    this->state = network::State::CONNECTED;
	cout<<"conn state " << this->state << endl;
    return network::Result::RESULT_OK;
}

network::Result SocketGLIBC::Write(const string& data)
{ 
    write(this->m_socket, data.c_str(), data.size());
    cout<<">>>>>>>>>> BANG !!! >>>>>>>>>>>>>>>>>>>>>>>>>>>> " << this->state << endl;
    
    //reset the state??
    this->state = network::State::CREATED;

    cout<<"Write state " << this->state << endl;
    return network::Result::RESULT_OK;
}

network::Result SocketGLIBC::Disconnect()
{
	cout << "SocketGLIBC disconnect" << endl;
    return network::Result::RESULT_OK;
}

network::Result SocketGLIBC::Shutdown()
{
	cout << "SocketGLIBC shutdown" << endl;

    int err = shutdown(this->m_socket, this->shutdown_mode); // todo move to constants

    this->m_socket = network::SocketState::SOCKET_ERROR; 

    if(network::SocketState::SOCKET_ERROR >= err)
    {
	   	cout << "err close socket is " << err << endl;
        return network::Result::RESULT_ERROR;
    }
    return network::Result::RESULT_OK;
}

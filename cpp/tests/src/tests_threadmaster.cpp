#include "../include/tests_threadmaster.h"

static const string ip1 = "markets.businessinsider.com";
static const uint16_t port1 = 443;

void test_threadmaster_run()
{
    cout << "test_threadmaster_run" << endl;

    vector<Flood*> v = { 
          new GET(ip1, port1)
    };
       
    Serial s(&v);
    

    cout << "Serial.getState() = " << s.getState() << endl;
    cout << "Serial.getType() = " << s.getType() << endl;
    
    s.start();
    s.stop();
    
    s.start();
    s.stop();

    cout << "ok"<<endl;
}

void test_threadmaster_long_run()
{
    cout << "test_threadmaster_long_run" << endl;

    vector<Flood*> v = { 
          new GET(ip1, port1)
    };

    for(int i = 0; i < 1000000; i++)
    {
    
    Serial s(&v);
    s.start();
    s.stop();
    
    }
}

void test_threadmaster_init_with_floods()
{
    cout << "test_threadmaster_init_with_floods" << endl;

    vector<Flood*> v = { 
          new GET(string(ip1), port1)
    };
       
    Serial s(&v);
    

    cout << "Serial.getState() = " << s.getState() << endl;
    cout << "Serial.getType() = " << s.getType() << endl;
    
    cout << "ok"<<endl;
}

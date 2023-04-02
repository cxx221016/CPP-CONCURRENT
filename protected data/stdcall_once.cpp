#include<list>
#include<mutex>
#include<algorithm>
#include<iostream>
#include<thread>

class X
{
private:
    connection_info connection_details;
    connection_handle connection;
    std::once_flag connection_init_flag;

    void open_connection()
    {
        connection=connnection_manager.open(connection_details);
    }
public:
    X(connection_info const& connection_datails_):connection_datails(connection_datails_){}
    void send_data(data_packet const& data)
    {
        std::call_once(connection_init_flag,&X::open_connection,this);
        connection.send_data(data);
    }
    data_packet receive_data()
    {
        std::call_once(connection_init_flag,&X::open_connection,this);
        return connection.receive_data();
    }
};
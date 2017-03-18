#include "rpc.h"

int main()
{
    RPC* p_rpc = new RPC("10.0.0.2","10.0.0.1");
    p_rpc->call("hello");
    return 0;
}

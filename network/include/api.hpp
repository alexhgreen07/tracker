#ifndef __API_HPP__
#define __API_HPP__

#include <json-rpc.h>

using namespace jsonrpc;

namespace Tracker
{
namespace Network
{
	
class Api
{
public:
	virtual JsonMethods & getProcedures() = 0;
	virtual JsonNotifications & getNotifications() = 0;
};
	
}
}

#endif
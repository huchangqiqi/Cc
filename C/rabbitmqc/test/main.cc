#include "test.pb.h"
#include <iostream>

using namespace google::protobuf;
using namespace std;

int main()
{
	Arena arena;
	SearchRequest *sr = Arena::CreateMessage<SearchRequest>(&arena);

	return 0;
}

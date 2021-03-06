#include "./rapidjson/include/rapidjson/document.h"
#include "./rapidjson/include/rapidjson/writer.h"
#include "./rapidjson/include/rapidjson/stringbuffer.h"

#include <iostream>

using namespace rapidjson;
int main(){
    const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    Document d;
    d.Parse(json);

    Value& s = d["stars"];
    s.SetInt(s.GetInt()+1);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    std::cout << buffer.GetString() << std::endl;
    return 0;
}

#ifndef _CONFIG_LOADER_H_
#define _CONFIG_LOADER_H_

#include "util.h"
#include "exception_type.h"
#include <rapidjson/document.h>
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include <rapidjson/writer.h>
#include <memory>
#include <fstream>

#define CONFIG_FILE_PATH "./platform_conf.json"

class Config_loader
{
public:
    Config_loader(const std::string& file_path, const std::string& config_item);
    ~Config_loader();
	std::string get_config(const std::string module_name, const std::string& item);
	void set_config(const std::string& item, const std::string& value);
private:
	std::shared_ptr<rapidjson::Document> m_config_val;
	std::string m_file_path;
	std::string m_config_item;
};

inline Config_loader::Config_loader(const std::string & file_path, const std::string & config_item)
	:m_file_path(file_path),m_config_item(config_item)
{
	rapidjson::Document dom(rapidjson::kObjectType);
	std::ifstream fp(file_path);
	if (!fp.is_open())
	{
		throw fatal_exception("open config file fail", FILE_NOT_EXIST);
	}
	try
	{
		std::string json_str;
		char buf[1024];
		while (!fp.eof())
		{
			fp.getline(buf, 1023);
			json_str += trim(delete_rn(std::string(buf)));
		}
		m_config_val = shared_ptr<rapidjson::Document>(new rapidjson::Document(rapidjson::kObjectType));
		m_config_val->Parse(json_str.c_str());
		fp.close();
	}
	catch (exception* e)
	{
		throw fatal_exception("alloc memory fail", ALLOCATE_FAIL);
	}
}

inline Config_loader::~Config_loader()
{
}

inline std::string Config_loader::get_config(const std::string module_name, const std::string& item)
{
	if (m_config_val->HasMember(module_name.c_str()))
	{
		if ((*m_config_val)[module_name.c_str()].IsObject())
		{
			if ((*m_config_val)[module_name.c_str()].HasMember(item.c_str()))
			{
				return std::string((*m_config_val)[module_name.c_str()][item.c_str()].GetString());
			}
		}
	}
	return std::string("");
}

/************************************************************************/
/* only allow set config of this module                                */
/************************************************************************/
inline void Config_loader::set_config(const std::string & item, const std::string & value)
{
	rapidjson::Document::AllocatorType& allocator = m_config_val->GetAllocator();
	if (m_config_val->HasMember(m_config_item.c_str()))
	{
		if ((*m_config_val)[m_config_item.c_str()].IsObject())
		{
			if ((*m_config_val)[m_config_item.c_str()].HasMember(item.c_str()))
			{
				(*m_config_val)[m_config_item.c_str()][item.c_str()] = rapidjson::Value(value.c_str(), allocator);
			}
			else
			{
				(*m_config_val)[m_config_item.c_str()].AddMember(rapidjson::GenericValue<rapidjson::UTF8<>, rapidjson::Document::AllocatorType>(item.c_str(), allocator),
					rapidjson::Value(value.c_str(), allocator), allocator);
			}
		}
	}
	try
	{
#ifdef WIN32
		FILE* fp = fopen(m_file_path.c_str(), "wb");
#else
		FILE* fp = fopen(m_file_path.c_str(), "w");
#endif
		char writeBuffer[65536];
		rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
		rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
		m_config_val->Accept(writer);
		fclose(fp);
	}
	catch (const std::exception&)
	{
		throw error_exception("write config to file fail", FILE_NOT_EXIST);
	}
}

#endif //_CONFIG_LOADER_H_


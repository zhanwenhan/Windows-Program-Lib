#ifndef __SYSTEM_CONFIG_MANAGER__
#define __SYSTEM_CONFIG_MANAGER__

#include "component/Xml_Wrapper.h"

class System_config_manager
{
public:
	System_config_manager();
	~System_config_manager();

public:
	int initial(std::string xml_file);
	void show_config();

private:
	Xml_Wrapper m_xml_wrapper;
};

#endif
#include <iostream>
#include <map>
#include <string.h>
#include <stdlib.h>

#include "common/handle_error.h"

#include "System_config_manager.h"

System_config_manager::System_config_manager()
{

}

System_config_manager::~System_config_manager()
{

}

int System_config_manager::initial(std::string xml_file)
{
	if(!this->m_xml_wrapper.load_from_file(xml_file.c_str()))
	{
		handle_error("System_config_manager::initial - m_xml_wrapper.load_from_file");
		return -1;
	}
	
	return 0;
}

void System_config_manager::show_config()
{

}

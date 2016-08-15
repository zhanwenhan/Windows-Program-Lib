#include <iostream>

#include "common/handle_error.h"

#include "Xml_Wrapper.h"

Xml_Wrapper::Xml_Wrapper()
{
	this->m_doc = NULL;
}

Xml_Wrapper::~Xml_Wrapper()
{
	if(this->m_doc)
		xmlFreeDoc(this->m_doc);
}

xmlNode *Xml_Wrapper::create_xml(const char *root_name)
{
	this->m_doc = xmlNewDoc(BAD_CAST "1.0");
	xmlNode *root_node = xmlNewNode(NULL, BAD_CAST root_name);
	xmlDocSetRootElement(this->m_doc, root_node);//return the old root element if any was found, NULL if root was NULL
	return root_node;
}

xmlNode *Xml_Wrapper::load_from_buffer(const char *xml_buf, unsigned int size)
{
	if(this->m_doc)
		xmlFreeDoc(this->m_doc);

	this->m_doc = xmlReadMemory(xml_buf, size, NULL, "UTF-8", int(XML_PARSE_RECOVER));
	if(!this->m_doc)
	{
		handle_error("Xml_Wrapper::load_from_buffer - xmlReadMemory");
		return NULL;
	}
	xmlNodePtr root_node = xmlDocGetRootElement(this->m_doc);
	if(!root_node)
	{
		handle_error("Xml_Wrapper::load_from_buffer - xmlDocGetRootElement");
		return NULL;
	}
	return root_node;
}

xmlNode *Xml_Wrapper::load_from_file(const char *xml_file)
{
	if(this->m_doc)
		xmlFreeDoc(this->m_doc);

	this->m_doc = xmlReadFile(xml_file, "UTF-8", int(XML_PARSE_RECOVER));
	if(!this->m_doc)
	{
		handle_error("Xml_Wrapper::load_from_file - xmlReadFile");
		return NULL;
	}
	xmlNodePtr root_node = xmlDocGetRootElement(this->m_doc);
	if(!root_node)
	{
		handle_error("Xml_Wrapper::load_from_file - xmlDocGetRootElement");
		return NULL;
	}
	return root_node;
}

int Xml_Wrapper::save_in_buffer(xmlNode *cur_node, std::string &xml_buf)
{
	xmlBufferPtr pxml_buf = xmlBufferCreate();
	if(!pxml_buf)
	{
		handle_error("Xml_Wrapper::save_in_buffer - xmlBufferCreate");
		return -1;
	}

	int bytes = xmlNodeDump(pxml_buf, this->m_doc, cur_node, 0, 1);
	if(bytes < 0)
	{
		handle_error("Xml_Wrapper::save_in_buffer - xmlNodeDump");
		return -1;
	}

	std::string temp_buf((char*)pxml_buf->content, bytes);
	xml_buf = temp_buf;
	xmlBufferFree(pxml_buf);
	return 0;
}

int Xml_Wrapper::save_in_file(const char *xml_file)
{
	int nwrite = xmlSaveFormatFileEnc(xml_file, this->m_doc, "UTF-8", 1);
	if(nwrite == -1)
		handle_error("Xml_Wrapper::save_in_file - xmlSaveFormatFileEnc");
	return nwrite;
}

xmlNode *Xml_Wrapper::get_root_node()
{
	return xmlDocGetRootElement(this->m_doc);
}

xmlNode *Xml_Wrapper::get_parent_node(xmlNode *cur_node)
{
	if(!cur_node)
	{
		handle_error("Xml_Wrapper::get_parent_node - para is null");
		return NULL;
	}
	return cur_node->parent;
}

xmlNode *Xml_Wrapper::get_first_child_node(xmlNode *cur_node)
{
	std::vector<xmlNode *> children_nodes_vector;
	if(this->get_all_children_nodes(cur_node, children_nodes_vector) < 0)
	{
		handle_error("Xml_Wrapper::get_first_child_node - get_all_children_nodes");
		return NULL;
	}

	xmlNode *first_child_node = NULL;
	if(children_nodes_vector.size() > 0)
		first_child_node = *(children_nodes_vector.begin());
	else
		handle_error("Xml_Wrapper::get_first_child_node - no child");
	return first_child_node;
}

int Xml_Wrapper::get_all_children_nodes(xmlNode *cur_node, std::vector<xmlNode *> &children_nodes_vector)
{
	if(!cur_node)
	{
		handle_error("Xml_Wrapper::get_all_children_nodes - para is null");
		return -1;
	}

	xmlNodePtr child_node = cur_node->xmlChildrenNode;
	while(child_node)
	{
		children_nodes_vector.push_back(child_node);
		child_node = child_node->next;
	}
	return 0;
}

xmlNode *Xml_Wrapper::get_specific_child_node(xmlNode *cur_node, const char* node_name)
{
	std::vector<xmlNode *> children_nodes_vector;
	if(this->get_all_children_nodes(cur_node, children_nodes_vector) < 0)
	{
		handle_error("Xml_Wrapper::get_specific_child_node - get_all_children_nodes");
		return NULL;
	}

	std::vector<xmlNode *>::iterator vit;
	for(vit = children_nodes_vector.begin(); vit != children_nodes_vector.end(); vit++)
		if(!xmlStrcmp((*vit)->name, BAD_CAST node_name))
			return *vit;
	handle_error("Xml_Wrapper::get_specific_child_node - not find");
	return NULL;
}

int Xml_Wrapper::get_specific_children_nodes(xmlNode *cur_node, const char* node_name, std::vector<xmlNode *> &specific_children_nodes_vector)
{
	std::vector<xmlNode *> children_nodes_vector;
	if(this->get_all_children_nodes(cur_node, children_nodes_vector) < 0)
	{
		handle_error("Xml_Wrapper::get_specific_children_nodes - get_all_children_nodes");
		return -1;
	}

	std::vector<xmlNode *>::iterator vit;
	for(vit = children_nodes_vector.begin(); vit != children_nodes_vector.end(); vit++)
		if(!xmlStrcmp((*vit)->name, BAD_CAST node_name))
			specific_children_nodes_vector.push_back(*vit);
	return 0;
}

xmlNode *Xml_Wrapper::add_simple_child_node(xmlNode *parent_node, const char *node_name, const char *node_value)
{
	return xmlNewTextChild(parent_node, NULL, BAD_CAST node_name, BAD_CAST node_value);
}

xmlNode *Xml_Wrapper::add_child_node_by_name(xmlNode *parent_node, const char *node_name)
{
	xmlNodePtr child_node = xmlNewNode (NULL, BAD_CAST node_name);
	return xmlAddChild(parent_node, child_node);
}

xmlNode *Xml_Wrapper::add_child_node_by_node(xmlNode *parent_node, xmlNode *cur_node)//recursively copy cur_node, and add to the parent
{
	xmlNodePtr new_node = xmlCopyNode(cur_node, 1);//if 1 do a recursive copy , if 2 copy properties and namespaces
	if(!new_node)
	{
		handle_error("Xml_Wrapper::add_child_node_by_node - xmlCopyNode");
		return NULL;
	}
	return xmlAddChild(parent_node, new_node);//return the child or NULL in case of error
}

int Xml_Wrapper::get_node_name(xmlNode *cur_node, std::string &name)
{
	if(!cur_node)
	{
		handle_error("Xml_Wrapper::get_node_name - para is null");
		return -1;
	}
	name = (char *)cur_node->name;
	return 0;
}

int Xml_Wrapper::get_node_value(xmlNode *cur_node, std::string &value)
{
	if(!cur_node)
	{
		handle_error("Xml_Wrapper::get_node_value - para is null");
		return -1;
	}
	xmlChar *pvalue = xmlNodeGetContent(cur_node);
	if(!pvalue)
	{
		handle_error("Xml_Wrapper::get_node_value - xmlNodeGetContent");
		return -1;
	}
	else
	{
		value = (char*)pvalue;
		xmlFree(pvalue);
		return 0;
	}
}

int Xml_Wrapper::set_node_value(xmlNode *cur_node, const char* value)
{
	if(!cur_node)
	{
		handle_error("Xml_Wrapper::set_node_value - para is null");
		return -1;
	}
	xmlNodeSetContent(cur_node, BAD_CAST value);
	return 0;
}

int Xml_Wrapper::get_node_property(xmlNode *cur_node, const char *property_name, std::string &property_value)
{
	if(xmlHasProp(cur_node, BAD_CAST property_name))
	{
		xmlChar *prop = xmlGetProp(cur_node, BAD_CAST property_name);
		property_value = (const char*)prop;
		xmlFree(prop);
		return 0;
	}
	else
	{
		handle_error("Xml_Wrapper::get_node_property - xmlHasProp");
		return -1;
	}
}

int Xml_Wrapper::set_node_property(xmlNode *cur_node, const char *property_name, const char *property_value)
{
	if(xmlHasProp(cur_node, BAD_CAST property_name))
	{
		if(!xmlSetProp(cur_node, BAD_CAST property_name, BAD_CAST property_value))
		{
			handle_error("Xml_Wrapper::set_node_property - xmlSetProp");
			return -1;
		}
		return 0;
	}
	else
	{
		if(!xmlNewProp(cur_node, BAD_CAST property_name, BAD_CAST property_value))
		{
			handle_error("Xml_Wrapper::set_node_property - xmlNewProp");
			return -1;
		}
		return 0;
	}
}

int Xml_Wrapper::del_node_property(xmlNode *cur_node, const char *property_name)
{
	xmlAttrPtr prop = NULL;
	if(prop = xmlHasProp(cur_node, BAD_CAST property_name))
		return xmlRemoveProp(prop);
	else
	{
		handle_error("Xml_Wrapper::del_node_property - xmlHasProp");
		return -1;
	}
}

int Xml_Wrapper::get_nodes_by_xpath(const char *xpath, std::vector<xmlNode *> &node_vector)
{
	xmlXPathContextPtr context = xmlXPathNewContext(this->m_doc);
	if(!context)
	{
		handle_error("Xml_Wrapper::get_nodes_by_xpath - xmlXPathNewContext");
		return -1;
	}

	xmlXPathObjectPtr result = xmlXPathEvalExpression(BAD_CAST xpath, context);
	xmlXPathFreeContext(context);
	if(!result)
	{
		handle_error("Xml_Wrapper::get_nodes_by_xpath - xmlXPathEvalExpression");
		xmlXPathFreeObject(result);
		return -1;
	}

	xmlNodeSetPtr pnode_set = result->nodesetval;
	for(int i = 0; i != pnode_set->nodeNr; i++)
		node_vector.push_back(pnode_set->nodeTab[i]);
	xmlXPathFreeObject(result);
	return 0;
}

xmlNode *Xml_Wrapper::get_first_node_by_xpath(const char *xpath)
{
	xmlXPathContextPtr context = xmlXPathNewContext(this->m_doc);
	if(!context)
	{
		handle_error("Xml_Wrapper::get_first_node_by_xpath - xmlXPathNewContext");
		return NULL;
	}

	xmlXPathObjectPtr result = xmlXPathEvalExpression(BAD_CAST xpath, context);
	xmlXPathFreeContext(context);
	if(!result)
	{
		handle_error("Xml_Wrapper::get_first_node_by_xpath - xmlXPathEvalExpression");
		xmlXPathFreeObject(result);
		return NULL;
	}

	xmlNodePtr temp_node = NULL;
	xmlNodeSetPtr pnode_set = result->nodesetval;
	if(pnode_set->nodeNr >= 1)
		temp_node = pnode_set->nodeTab[0];
	else
		handle_error("Xml_Wrapper::get_first_node_by_xpath - no such path");
	xmlXPathFreeObject(result);
	return temp_node;
}

int Xml_Wrapper::get_first_node_value_by_xpath(const char *xpath, std::string &node_value)
{
	xmlNode *node = this->get_first_node_by_xpath(xpath);
	if(!node)
	{
		handle_error("Xml_Wrapper::get_first_node_value_by_xpath - get_first_node_by_xpath");
		return -1;
	}
	return this->get_node_value(node, node_value);
}

#ifndef __XML_WRAPPER_H__
#define __XML_WRAPPER_H__

#include <string>
#include <vector>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>

//wrap the management of xml

class Xml_Wrapper
{
public:
	Xml_Wrapper();
	~Xml_Wrapper();

	xmlNode *create_xml(const char *root_name);
	xmlNode *load_from_buffer(const char *xml_buf, unsigned int size);
	xmlNode *load_from_file(const char *xml_file);
	int save_in_buffer(xmlNode *cur_node, std::string &xml_buf);
	int save_in_file(const char *xml_file);

	xmlNode *get_root_node();
	xmlNode *get_parent_node(xmlNode *cur_node);
	xmlNode *get_first_child_node(xmlNode *cur_node);
	int get_all_children_nodes(xmlNode *cur_node, std::vector<xmlNode *> &children_nodes_vector);
	xmlNode *get_specific_child_node(xmlNode *cur_node, const char* node_name);
	int get_specific_children_nodes(xmlNode *cur_node, const char* node_name, std::vector<xmlNode *> &specific_children_nodes_vector);

	xmlNode *add_simple_child_node(xmlNode *parent_node, const char *node_name, const char *node_value);
	xmlNode *add_child_node_by_name(xmlNode *parent_node, const char *node_name);
	xmlNode *add_child_node_by_node(xmlNode *parent_node, xmlNode *cur_node);//recursively copy cur_node, and add to the parent

	int get_node_name(xmlNode *cur_node, std::string &name);
	int get_node_value(xmlNode *cur_node, std::string &value);
	int set_node_value(xmlNode *cur_node, const char* value);

	int get_node_property(xmlNode *cur_node, const char *property_name, std::string &property_value);
	int set_node_property(xmlNode *cur_node, const char *property_name, const char *property_value);
	int del_node_property(xmlNode *cur_node, const char *property_name);

	int get_nodes_by_xpath(const char *xpath, std::vector<xmlNode *> &node_vector);
	xmlNode *get_first_node_by_xpath(const char *xpath);
	int get_first_node_value_by_xpath(const char *xpath, std::string &node_value);

private:
	xmlDocPtr m_doc;
};

#endif

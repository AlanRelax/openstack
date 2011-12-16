#include "mylib.h"


static xmlNode *propNode;

void print_element_names(xmlNode *node) {

    xmlNode *cur_node = NULL;

    for (cur_node = node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            printf("node type:Element,name:%s\n", cur_node->name);
        }
    }
}

int get_element_node(xmlNodePtr cur,xmlChar *node) {

    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
//printf("%s\n", cur->name);
        if (!xmlStrcmp(cur->name,node)) {
//            printf("node: %s\n", cur->name);
            propNode = cur;
            return TRUE;
        }
        cur = cur->next;
    }
    return FALSE;
}

int get_element_key(xmlDocPtr doc, xmlNodePtr cur,xmlChar *key) {

    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
        if (!xmlStrcmp(cur->name,key)) {
            key = xmlNodeListGetString(doc,cur->xmlChildrenNode,1);
            printf("keyword: %s\n", key);
            xmlFree(key);
            propNode = cur;
            return TRUE;
        }
        cur = cur->next;
    }
    return FALSE;
}

void get_element_prop(xmlNodePtr propNode, xmlChar *prop, char **vnet) {

    if (xmlHasProp(propNode, prop)) {
        xmlAttrPtr attrPtr = propNode->properties;
        while (attrPtr != NULL) {
            if (!xmlStrcmp(attrPtr->name, prop)) {
                xmlChar *szAttr = xmlGetProp(propNode,prop);
//                printf("get attribute: %s\n", szAttr);
                strcpy(*vnet, (char *)szAttr);
                xmlFree(szAttr);
            }  
            attrPtr = attrPtr->next;
        }    
    }
}


int get_vnet(const char *filename, char **vnet) {

    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    doc = xmlReadFile(filename, NULL, 0);

    if (doc == NULL) {
        printf("error: cloud not parse file %s\n", filename);
        xmlFreeDoc(doc);
        return -2;
    }

    root_element = xmlDocGetRootElement(doc);

//    print_element_names(root_element);
    
    xmlNode *cur;
    cur = root_element;
//    cur = cur->xmlChildrenNode;
    propNode = cur;
    while (cur != NULL) {
        if (get_element_node(propNode, (xmlChar *)"devices") == 0) {
            if (get_element_node(propNode, (xmlChar *)"interface") == 0) {
                if (get_element_node(propNode, (xmlChar *)"target") == 0) {
                    get_element_prop(propNode, (xmlChar *)"dev", vnet);
                }
            }
        }
        cur = cur->next;
    }
    
    xmlFreeDoc(doc);

    xmlCleanupParser();

    return TRUE;

}

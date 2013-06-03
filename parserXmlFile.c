#include<libxml/parser.h>
#include<stdio.h>
int parserXmlFile(char *rootName);
/*int main()
{
    char *str = "root.xml";
    parserXmlFile(str);
    return 0;
}*/
int parserXmlFile(char *rootName)
{
    xmlDocPtr doc; // djf定义解析文档指针
    xmlNodePtr curNode;//定义节点指针（在各个节点之间移动）
    xmlChar *szKey;//定义临时字符串变量

    char *szDocName;
    szDocName = (char *)rootName;
    doc = xmlReadFile(szDocName,"GB2312",XML_PARSE_RECOVER);
    if (doc == NULL)
    {
        printf("document not parsed successfully");
        return -1;
    }

    //确定文档根元素
    curNode = xmlDocGetRootElement(doc);
    if (curNode == NULL)
    {
        printf("empty document");
        xmlFreeDoc(doc);
        return -1;
    }

    curNode=curNode->xmlChildrenNode;
    xmlNodePtr propNodePtr = curNode;

    while (curNode != NULL)
    {
        printf("name:%s\t",curNode->name);
       // xmlNodePtr obj = curNode->xml

        xmlFree(szKey);
        curNode = curNode->next;
    }
    xmlFreeDoc(doc);
}

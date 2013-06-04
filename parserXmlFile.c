/*
* gcc -I/usr/local/include/libxml2 -L/usr/local/lib -lxml2 -lz -lm -ltokyocabinet -ltokyotyrant -o parserXmlFile md5.c parserXmlFile.c ttnet.c
*/
#include<libxml/parser.h>
#include<stdio.h>
#include<string.h>
#include "md5.h"
#include"global.h"
#include"md_type.h"
int parserXmlFile(char *rootName);
int listBucket(char *clientName);
int listObjectInBucket(char *rootName,char *BucketName);
int main()
{
    char *xmlFileName = "client1.xml";
    char *bucketName = "buc2";
    PutInCacheFile("cacheFile.txt",xmlFileName);
    //listObjectInBucket(xmlFileName,bucketName);
    //listBucket(xmlFileName);
    //parserXmlFile(str);
    return 0;
}
//cache file
int PutInCacheFile(char *fileName,char *xmlFileName)
{
    FILE *fp;
    if ((fp=fopen(fileName,"w"))==NULL)
    {
        printf("cannot open file\n");
        exit(0);
    }
    xmlDocPtr doc; // djf定义解析文档指针
    xmlNodePtr rootNode,bucNode,objNode;//定义节点指针（在各个节点之间移动）

    char *szDocName;
    szDocName = (char *)xmlFileName;
    doc = xmlReadFile(szDocName,"GB2312",XML_PARSE_RECOVER);
    if (doc == NULL)
    {
        printf("document not parsed successfully");
        return -1;
    }

    //确定文档根元素
    rootNode = xmlDocGetRootElement(doc);
    if (rootNode == NULL)
    {
        printf("empty document");
        xmlFreeDoc(doc);
        return -1;
    }

    //curNode=curNode->xmlChildrenNode;
    bucNode = rootNode->children;
    printf("--------------------------------\n");
    while (bucNode != NULL) //打印第一层节点
    {
        objNode = bucNode->children;
        while (objNode!=NULL)//打印第二层节点
        {
            unsigned char temp[50];
            strcpy(temp,rootNode->name);
            strcat(temp,"/");
            strcat(temp,bucNode->name);
            strcat(temp,"/");
            strcat(temp,objNode->name);
            int len = strlen(temp);

            //生成md5值
            unsigned char md5sum[16]={0};
            unsigned char md5_str[33]={0};
            md5(temp,len,md5sum);
            md5_2_str(md5sum,md5_str);

                //生成value值
            Meta_Data *metaData = (Meta_Data *)malloc(sizeof(Meta_Data));
            Meta_Data *meta1 = (Meta_Data *)malloc(sizeof(Meta_Data));
            strcpy(metaData->replica[0].rep_ip,"192.168.0.18");

            md_put(md5_str,metaData);
                GetValue(md5_str,meta1);
            printf("the value  is %s\n",meta1->replica[0].rep_ip);
            //printf("%s\t%s\n",temp,md5_str);
            fprintf(fp,"%s\t%s\t%s\n",temp,md5_str,meta1->replica[0].rep_ip);
            // PutValue(temp,md5_str);

            objNode=objNode->next;
        }

        bucNode = bucNode->next;
    }
    xmlFreeDoc(doc);
    fclose(fp);


}

//list object in bucket
int listObjectInBucket(char *xmlFileName,char *BucketName)
{
    xmlDocPtr doc; // djf定义解析文档指针
    xmlNodePtr curNode;//定义节点指针（在各个节点之间移动）
    xmlChar *szKey;//定义临时字符串变量

    char *szDocName;
    szDocName = (char *)xmlFileName;
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

    //curNode=curNode->xmlChildrenNode;
    curNode = curNode->children;
    xmlNodePtr propNodePtr = curNode;

    printf("--------------------------------\n");
    while (curNode != NULL) //打印第一层节点
    {
        if ((xmlStrcmp(curNode->name,(xmlChar *)BucketName))==0)
        {
            printf("object in %s:%s\n",BucketName,curNode->name);
            xmlNodePtr obj = curNode->children;
            while (obj!=NULL)//打印第二层节点
            {
                printf("%s\n",obj->name);
                obj=obj->next;
            }
        }
        curNode = curNode->next;
    }
    xmlFreeDoc(doc);

}

//list bucket
int listBucket(char *xmlFileName)
{
    xmlDocPtr doc; // djf定义解析文档指针
    xmlNodePtr curNode;//定义节点指针（在各个节点之间移动）
    xmlChar *szKey;//定义临时字符串变量

    char *szDocName;
    szDocName = (char *)xmlFileName;
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

    //curNode=curNode->xmlChildrenNode;
    curNode = curNode->children;
    xmlNodePtr propNodePtr = curNode;

    printf("--------------------------------\n");
    printf("buckets in %s:\n",xmlFileName);
    while (curNode != NULL) //打印第一层节点
    {

        printf("%s\n",curNode->name);
        curNode = curNode->next;
    }
    xmlFreeDoc(doc);
}

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

    //curNode=curNode->xmlChildrenNode;
    curNode = curNode->children;
    xmlNodePtr propNodePtr = curNode;

    while (curNode != NULL) //打印第一层节点
    {
        printf("--------------------------------\n");
        printf("bucket:%s\t\n",curNode->name);
        xmlNodePtr obj = curNode->children;
        while (obj!=NULL)//打印第二层节点
        {
            printf("object in bucket %s :\n%s/%s\n",curNode->name,curNode->name,obj->name);
            obj=obj->next;
        }


        curNode = curNode->next;
    }
    xmlFreeDoc(doc);
}


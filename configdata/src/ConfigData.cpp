#include <iostream>
#include "tinyxml.h"
#include "ConfigData.h"
using namespace std;

ConfigData& ConfigData::Instance()
{
    static ConfigData instance;
    return instance;
}

bool ConfigData::ReadPaniniConfig(PaniniConfigInfo *psPaniniConfig)
{
    if (!xmlDoc.LoadFile("../configfile/PaniniConfig.xml"))
	{
        cout << "load PaniniConfig.xml file failed!" << endl;
		return false;
	}
	int iLength = sizeof(PaniniConfigInfo) / sizeof(float);
	float piData[iLength];
	TiXmlElement *xmlRootElement = xmlDoc.RootElement();
	TiXmlElement *xmlSubElementRoot = xmlRootElement->FirstChildElement("PaniniConfig");
	if(xmlSubElementRoot == NULL)
	{
		cout << "ReadPaniniConfigFromXML xmlSubElementRoot is NULL,default value PaniniConfig" << endl;
		return false;
	}

	TiXmlElement *xmlSubElementPre = xmlSubElementRoot->FirstChildElement("RearConfig");
	if(xmlSubElementPre == NULL)
	{
		cout << "ReadPaniniConfigFromXML xmlSubElementPre is NULL,default value RearConfig" << endl;
		return false;
	}

	TiXmlElement *xmlSubElement = xmlSubElementPre->FirstChildElement();
	for (int i = 0; i < iLength; i++)
	{
		const char *pchElement = xmlSubElement->GetText();
		piData[i] = atof(pchElement);
		xmlSubElement = xmlSubElement->NextSiblingElement();
	}

	psPaniniConfig->iWidth = piData[0];
	psPaniniConfig->iHeight = piData[1];
	psPaniniConfig->fD = piData[2];
	psPaniniConfig->fTop = piData[3];
	psPaniniConfig->fBottom = piData[4];
	psPaniniConfig->fAzimuth = piData[5];
	psPaniniConfig->fTheta = piData[6];
	psPaniniConfig->fRotDegX = piData[7];
	psPaniniConfig->fRotDegY = piData[8];
	psPaniniConfig->fRotDegZ = piData[9];
	psPaniniConfig->iTransY = piData[10];
	psPaniniConfig->iTransZ = piData[11];
	psPaniniConfig->iMirrorInputImg = piData[12];
	psPaniniConfig->iMirrorPoint = piData[13];
	psPaniniConfig->iOffsetX = piData[14];
	psPaniniConfig->iOffsetY = piData[15];

    return true;
}

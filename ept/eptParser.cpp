#ifndef EPT_PARSER_CPP
#define EPT_PARSER_CPP
#include <iostream>
#include <fstream>
#include <cstring> //Used for strstr()
#include <cstdlib> //atoi()
#include <sstream> //Used for int||floatToAttr() functions

#include "eptParser.hpp"

const int SUPPORTED_VERSION=1;
////////////////////////////////////////////////
std::string eptGroup::getAttribute(const std::string& attrName)
{
	std::map<std::string, std::string>::iterator myIt=attributes.find(attrName);
	if (myIt==attributes.end()) return "";
	else return myIt->second;
}
void eptGroup::setAttribute(std::string name, std::string value)
{
	attributes[name]=value;
}
std::string eptGroup::getAttributeFromIndex(int i, std::string& attributeName)
{
	std::map<std::string, std::string>::iterator it=attributes.begin();
	for (int n=0; n<i; ++n)
	{
		++it;
	}
	if (it !=attributes.end())
	{
		attributeName=it->first;
		return it->second;
	}
	else
	{
		attributeName="";
	}
	return "";
}
int eptGroup::getTotalAttributes()
{
	return attributes.size();
}
////////////////////////////////////////////////
eptFile::eptFile()
{
	version=99;
}
eptFile::~eptFile()
{
	//clearAllData();
}
eptGroup* eptFile::getGroup(const std::string& groupName)
{
	eptGroup* returnValue=NULL;
	std::map<std::string, eptGroup*>::iterator myIt=groups.find(groupName);
	if (myIt!=groups.end()) returnValue=myIt->second;
	return returnValue;
}
void eptFile::addGroup(const std::string& groupName, eptGroup* newGroup)
{
	groups[groupName]=newGroup;
}
int eptFile::getVersion()
{
	return version;
}
///???
void eptFile::clearAllData()
{
	for (std::map<std::string, eptGroup*>::iterator groupIt=groups.begin(); groupIt !=groups.end(); ++groupIt)
	{
		delete groupIt->second;
		//groupIt=groups.erase(groupIt);
	}
}
void eptFile::setVersion(int newVersion)
{
	version=newVersion;
}
//Shows how easy it is to dump EPT stuff
bool eptFile::dumpToFile(std::string fileName, std::string collectionName)
{
	std::ofstream out;
	out.open(fileName.c_str());
	if (!out.is_open()) return false;
	//Header information
	out << "<" << collectionName << "." << version << ">\n\n";
	//Groups
	for (std::map<std::string, eptGroup*>::iterator myIt=groups.begin(); myIt!=groups.end(); ++myIt)
	{
		//Group name
		out << myIt->first << ":\n";
		//Attributes
		for (std::map<std::string, std::string>::iterator attrIt=myIt->second->attributes.begin();
		attrIt !=myIt->second->attributes.end(); ++attrIt)
		{
			out << "\t"<< attrIt->first << "=" << attrIt->second << ";\n";
		}
	}
	out.close();
	return true;
}
eptGroup* eptFile::getGroupFromIndex(int i, std::string& groupName)
{
	std::map<std::string, eptGroup*>::iterator it=groups.begin();
	for (int n=0; n<i; ++n)
	{
		++it;
	}
	if (it !=groups.end())
	{
		groupName=it->first;
		return it->second;
	}
	else
	{
		groupName="";
	}
	return NULL;
}
std::string eptFile::getAttribute(const std::string& location)
{
    unsigned int strPos = location.find(".");
    eptGroup* group = getGroup(location.substr(0, strPos));
    if (group)
    {
        return group->getAttribute(location.substr(strPos + 1, location.length() - 1));
    }
    return "";
}
////////////////////////////////////////////////
eptParser::~eptParser()
{
	clearAllData();
}
eptFile* eptParser::getFile(const std::string& fileName)
{
	eptFile* returnValue=NULL;
	std::map<std::string, eptFile*>::iterator finder=files.find(fileName);
	if (finder!=files.end()) returnValue=finder->second;
	return returnValue;
}
//Ugh, file I/O...
//Now that I see this is so huge, the E part of EPT might be a bit of a misnomer...
bool eptParser::load(const char* fileName)
{
	std::ifstream in;
	in.open(fileName);
	if (in.is_open())
	{
		//Get a new file to work with
		eptFile* newFile=new eptFile;
		std::string line;
		
		//First line is file header
		getline(in, line);
		//Grab the collection/file name
		unsigned int strPos=line.find(".");
		if (strPos>line.length())
		{
			std::cout << "Incorrect header format; file failed to load (couldn't find '.')\n";
			in.close();
			return false; 
		}
        
        std::string fileInternalName = line.substr(1, strPos-1);
        //Check if the file already exists
        std::map<std::string, eptFile*>::iterator search = files.find(fileInternalName);
        if (search!=files.end())
        {
            std::cout << "WARNING: File '"<< fileName <<"'already loaded\n";
            return true;
        }
        else
        {
            //Insert file into map
            files[fileInternalName]=newFile;
        }
		//newFile=files.find(line.substr(1, strPos-1))->second;
		//Set the file's version
		unsigned int strPosB=line.find(">");
		if (strPosB>line.length())
		{
			std::cout << "Incorrect header format; file failed to load (couldn't find '>')\n";
			in.close();
			return false;
		}
		int newVersion=atoi(line.substr(strPos+1, strPosB-1).c_str());
		if (newVersion !=SUPPORTED_VERSION)
		{
			std::cout << "Version not supported (supports " << SUPPORTED_VERSION << "); file failed to load\n";
			in.close();
			return false;
		}
		newFile->setVersion(newVersion);

		eptGroup* currentGroup=NULL;
		//Go line by line and parse each line individually
		while (in.good())
		{
			getline(in, line);
			//If '//' is found, the line is a comment and should be ignored
			if (line.length()==0 || strstr(line.c_str(), "//"))
			{
				if (!in.eof()) continue;
				else break;
			}
			
			//Look for a new group definition
			strPos=line.find(":");
			if (strPos < line.length())
			{
				for (strPosB=0; strPosB!=line.length(); ++strPosB)
				{
					if (line[strPosB] !=' ' && line[strPosB] !='\t') break;
				}
				newFile->addGroup(line.substr(strPosB, strPos-strPosB), new eptGroup);
				currentGroup=newFile->getGroup(line.substr(strPosB, strPos-strPosB));
				continue;
			}
			//Look for an attribute definition (if there is no =, there is none)
			strPos=line.find("=");
			if (strPos>line.length()) continue;
			
			//Trim any spaces etc.
			for (strPosB=0; strPosB!=line.length(); ++strPosB)
			{
				if (line[strPosB] !=' ' && line[strPosB] !='\t') break;
			}
			std::string attrName=line.substr(strPosB, strPos-strPosB);
			//Find the semicolon
			strPosB=line.find(";");
			while (strPosB>line.length())
			{
				//std::cout << "Attribute value does not terminate properly (forget the ';'?). Attr Ignored...\n";
				//continue;
				line+='\n';
				std::string temp;
				temp=line;
				getline(in, line);
				temp+=line;
				line=temp;
				strPosB=line.find(";");
				if (!in.good())
				{
					std::cout << "		EPT: Forget semicolon!\n";
					return false;
				}
			}
			currentGroup->setAttribute(attrName,line.substr(strPos + 1, strPosB - (strPos + 1)));
		}
		in.close();
		return true;
	}
	else return false;
}
void eptParser::clearAllData()
{
	
}
void eptParser::addFile(std::string collectionName, eptFile* newFile)
{
	files[collectionName]=newFile;
}
eptGroup* eptParser::getGroup(const std::string& location)
{
	eptGroup* group=NULL;
	unsigned int strPos=location.find(".");
	eptFile* file=getFile(location.substr(0, strPos));
	if (file)
	{
		std::string newLocation=location.substr(strPos+1, location.length()-1);
		strPos=newLocation.find(".");
		group=file->getGroup(newLocation.substr(0, strPos));
	}
	return group;
}
std::string eptParser::getAttribute(const std::string& location)
{
	std::string returnString="";
	unsigned int strPos=location.find(".");
	eptFile* file=getFile(location.substr(0, strPos));
	if (file)
	{
		std::string newLocation=location.substr(strPos+1, location.length()-1);
		strPos=newLocation.find(".");
		eptGroup* group=file->getGroup(newLocation.substr(0, strPos));
		if (group)
		{
			newLocation=newLocation.substr(strPos+1, newLocation.length()-1);
			returnString=group->getAttribute(newLocation);
		} 
	}
	return returnString;
}

///////////////////////////
/////Utility Functions/////
///////////////////////////
float attrToFloat(const std::string& attr)
{
	if (attr.empty()) return 0;
	return atof(attr.c_str());
}
int attrToInt(const std::string& attr)
{
	if (attr.empty()) return 0;
	return atoi(attr.c_str());
}
bool attrToBool(const std::string& attr)
{
	if (attr.empty()) return false;
	if (attr=="true") return true;
	return false;
}

std::string getArrayValue(const std::string& attr, int index)
{
    unsigned int openPos = attr.find("{");
    unsigned int closePos = attr.find("}");
    std::string inArray = attr.substr(openPos, closePos - openPos);
    //getArrayValue won't do bounds checking or see if it's a malformed
    //array because it needs to be fast. It will trust that getArrayLength was
    //checked first
    if (index == 0)
    {
        //Trim for first element
        unsigned int startPos = 0;
        unsigned int endPos = inArray.length();
        for (unsigned int i = 0; i < inArray.length(); ++i)
        {
            if (inArray[i] != ' ' && startPos==0) startPos = i;
            if (inArray[i] == '}' || (inArray[i] == ',' && inArray[i-1] != '\\'))
            {
                endPos = i;
                break;
            }
        }
        return inArray.substr(startPos, endPos - startPos);
    }
    int counter = 0;
    for (unsigned int i = 0; i < inArray.length(); ++i)
    {
        if (inArray[i] == ',' && inArray[i - 1] != '\\') //Found nondelimited comma
        {
            counter++;
            if (counter == index) //Found the desired element
            {
                unsigned int startPos = i + 1;
                unsigned int endPos = inArray.length();
                for (unsigned int n = i + 1; n < inArray.length(); n++)
                {
                    if (startPos == i + 1 && inArray[n] != ' ') startPos = n;
                    if (inArray[n]=='}' || (inArray[n] == ',' && inArray[n - 1] != '\\'))
                    {
                        endPos = n;
                        break;
                    }
                }
                return inArray.substr(startPos, endPos - i - 2);
            }
        }
    }
    
    //Item doesn't exist (out of bounds, malformed array, etc.)
    return "";
}
float attrToArrayFloat(const std::string& attr, int index)
{
    return attrToFloat(getArrayValue(attr, index));
}
int attrToArrayInt(const std::string& attr, int index)
{
    return attrToInt(getArrayValue(attr, index));
}
bool attrToArrayBool(const std::string& attr, int index)
{
    return attrToBool(getArrayValue(attr, index));
}
std::string attrToArrayStr(const std::string& attr, int index)
{
    std::string val = getArrayValue(attr, index);
    //Parse out comma delimiters
    for (unsigned int i = 0; i < val.length(); ++i)
    {
        if (val[i]=='\\' && val[i + 1]==',') val.erase(i, 1);
    }
    return val;
}
//Returns 0 if the attribute is not an array
int attrToArrayLength(const std::string& attr)
{
    unsigned int openPos = attr.find("{");
    unsigned int closePos = attr.find("}");
    //Not an array if no curly braces or nothing between the curly braces ( {} )
    if (openPos >= attr.length() || closePos >= attr.length() || closePos - openPos == 1)
    {
        //Malformed or not an array; return 0
        return 0;
    }
    //Length is determined by counting the number of commas
    unsigned int length = 1;
    for (unsigned int i = openPos + 1; i < closePos; i++)
    {
        if (attr[i]==',' && attr[i - 1] != '\\') length++;
    }
    return length;
}

/*int main()
{
    std::string arr = "{2, 50}";
    std::cout << "Length: " << attrToArrayLength(arr) << "\n";
    for (int i = 0; i < attrToArrayLength(arr); ++i)
    {
        std::cout << attrToArrayInt(arr, i) << "\n";
    }
}*/
std::string floatToAttr(float in)
{
	std::ostringstream s;
	s << in;
	return s.str();
}
std::string intToAttr(int in)
{
	std::ostringstream s;
	s << in;
	return s.str();
}

#endif

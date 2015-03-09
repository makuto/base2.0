#ifndef EPT_PARSER_HPP
#define EPT_PARSER_HPP
#include <string>
#include <map>
#include <vector>

//Should this be pointers? What would be fastest? (references?)
class eptFile;
class eptGroup
{
	friend class eptFile;
	protected:
		std::map<std::string, std::string> attributes;
	public:
		std::string getAttribute(const std::string&);
		void setAttribute(std::string, std::string);
		//Allows looping through the map (ignores textual indices); Break with ""
		//Second param is name
		std::string getAttributeFromIndex(int, std::string&);
		int getTotalAttributes();
};
class eptFile
{
	private:
		std::map<std::string, eptGroup*> groups;
		int version;
	public:
		eptFile();
		~eptFile();
		eptGroup* getGroup(const std::string&);
		//Pointer because groups can be large
		void addGroup(const std::string&, eptGroup*);
		int getVersion();
		void setVersion(int);
		void clearAllData();
		//File name, then this file/collection's name
		bool dumpToFile(std::string, std::string);
		//Loop through all groups via an indice (second param is name)
		//Break on NULL pointer
		eptGroup* getGroupFromIndex(int, std::string&);
        //Get an attribute via dot notation (eg. group.attribute)
        std::string getAttribute(const std::string&);
};
class eptParser
{
	private:
		std::map<std::string, eptFile*> files;
	public:
		~eptParser();
		eptFile* getFile(const std::string&);
		eptGroup* getGroup(const std::string&);
		void addFile(std::string, eptFile*);
		bool load(const char*);
		void clearAllData();
		//Use dotted notation to get an attribute (safer)
        //eg. files.ept loaded; "files.images.defaultSprite" will open
        //eptFile files, find group images, and return attr defaultSprite
		//PASS POINTER INSTEAD FOR EDITING? HOW?
		std::string getAttribute(const std::string&);
};
//Some basic utility functions to make life easier
//Returns 0 if failure
float attrToFloat(const std::string&);
int attrToInt(const std::string&);
bool attrToBool(const std::string&);
std::string floatToAttr(float);
std::string intToAttr(int);
#endif

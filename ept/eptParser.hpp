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
//Array utility functions
//Arrays have the following notation:
//myArray={1, -2.34, false, This is a string\, charles!};
//You MUST put a space after the commas! {arr, arr}, NOT {arr,arr}
//No, arrays cannot be nested. Could they be in the future? yes (TODO)
//They do not have to be homogenous, but you need to call the right functions
//for each respective value. If the value is not an array, attrToArrayLength
//will return a length of 0. All other functions will return default null values
float attrToArrayFloat(const std::string&, int index);
int attrToArrayInt(const std::string&, int index);
bool attrToArrayBool(const std::string&, int index);
std::string attrToArrayStr(const std::string&, int index);
//Returns 0 if the attribute is not an array (or the array is empty; note
//that an empty array MUST be {} - it cannot have spaces etc.
int attrToArrayLength(const std::string&);

//Convert values into attributes
std::string floatToAttr(float);
std::string intToAttr(int);
#endif

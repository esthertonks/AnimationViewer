#include <string>
#include <map>

template <typename ENUM>
class StringToEnumConvertor
{
public:
	StringToEnumConvertor() {};
	virtual ~StringToEnumConvertor() {};

	ENUM GetEnumValue(
		std::string enumValueAsString
	)
	{
		boost::algorithm::to_lower(enumValueAsString);
		std::map<std::string, ENUM>::const_iterator enumValueIterator = enumToString.find(enumValueAsString);
		if (enumValueIterator == enumToString.end()) {
			return eNotValid;
		}

		return enumValueIterator->second;
	}

private:
	std::map<std::string, ENUM> enumToString;
};
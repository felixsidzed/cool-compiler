#include <string>

#include "../gen/Global.h"

namespace cc {
	string Global::dump() {
		std::string result;
		switch (linkage) {
		case PublicLinkage:
			result += "public ";
			break;
		case InternalLinkage:
			result += "internal ";
			break;
		case PrivateLinkage:
			result += "private ";
			break;
		default:
			result += "unknown ";
			break;
		}

		result += type->dump();
		result += " ";
		result += name;
		result += " = ";
		if (initializer)
			result += initializer->dump();
		else
			result += "null";

		return result.c_str();
	}
}

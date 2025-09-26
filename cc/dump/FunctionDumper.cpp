#include "../gen/Function.h"

static const char* attrNames[] = {
	"noreturn",
	"inline"
};

namespace cc {
	string Function::dump() {
		std::string result;

		if (!attrs.empty()) {
			for (uint32_t i = 0; i < attrs.size; i++) {
				if (i > 0) result += ", ";
				result.append(1, '@');
				result.append(attrNames[attrs[i]]);
			}
		}
		result.append(1, '\n');

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

		result += "func ";
		result.append(name);
		result.append(1, '(');

		FunctionType* ftype = (FunctionType*)type;

		if (!args.empty()) {
			getArg(0);
			for (uint32_t i = 0; i < args.size; i++) {
				if (i > 0) result += ", ";
				result += ftype->args[i]->dump();
				result += " ";
				result += args[i].dump();
			}
		}

		if (ftype->vararg) {
			if (args.size > 0)
				result += ", ";
			result += "...";
		}

		result += ") -> ";
		result += ftype->returnType->dump();
		result.append(1, '\n');

		for (auto& block : blocks) {
			result.append(block.name);
			result += ":\n";

			std::string blockResult = block.dump().data;
			size_t pos = 0;
			while (pos < blockResult.size()) {
				blockResult.insert(pos, "    ");
				pos = blockResult.find('\n', pos);
				if (pos == std::string::npos)
					break;
				pos += 1;
			}
			result += blockResult;
			result.erase(result.find_last_not_of('\n') + 1);
		}

		result += "\nend";
		return result.c_str();
	}
}

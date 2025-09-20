#pragma once

#include <string>

#include "../map.h"
#include "../string.h"
#include "../vector.h"

namespace cc {
	class Scope {
	public:
		Scope() {
			used.emplace("");
		}

		bool isUsed(const char* name) const {
			return used.find(name) != nullptr;
		}

		const char* add(const char* name, bool dedup = true) {
			if (dedup) {
				if (!isUsed(name))
					return used.emplace(name)->data;

				string base = name;
				string candidate = base;

				int counter = 1;
				do {
					candidate = base + "." + std::to_string(counter++).c_str();
				} while (isUsed(candidate));

				return used.emplace(candidate)->data;
			} else {
				if (isUsed(name))
					return nullptr;
				return used.emplace(name)->data;
			}
		}

	private:
		vector<string> used;
	};
}

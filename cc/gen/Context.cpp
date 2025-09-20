#include "Context.h"

#include "Values.h"

namespace cc {
	Context& getGlobalContext() {
		static Context ctx;
		return ctx;
	}
}

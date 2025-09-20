#include <iostream>

#include "cc/gen/Module.h"
#include "cc/gen/Builder.h"
#include "cc/gen/Constant.h"

int main() {
	cc::Module module("[module]");

	cc::Function* fmain = module.addFunction(
		"main",
		cc::FunctionType::get(cc::Type::getInteger(32), nullptr, 0, false)
	);

	cc::Builder b;
	b.insertInto(fmain->appendBlock("entry"));

	b.ret(cc::ConstantInteger::get<int>(0));

	std::cout << module.dump() << std::endl;

	return 0;
}

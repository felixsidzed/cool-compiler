#include <fstream>
#include <iostream>

#include "cc/gen/Module.h"
#include "cc/gen/Builder.h"
#include "cc/gen/Constant.h"

#include "cc/emit/x86/X86Target.h"

int main() {
	cc::Module module("[module]");

	cc::Function* fmain = module.addFunction(
		"main",
		cc::FunctionType::get(cc::Type::getInteger(32), nullptr, 0, false)
	);

	cc::Builder b;
	b.insertInto(fmain->appendBlock("entry"));

	b.ret(cc::ConstantInteger::get(67));

	cc::x86Target target(std::endian::native, true);
	auto coff = target.emitObject(&module);

	std::cout << module.dump() << std::endl;

	std::ofstream file("test.o");
	file.write(coff.first.get(), coff.second);
	file.close();

	return 0;
}

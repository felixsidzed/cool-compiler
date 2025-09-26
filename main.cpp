#include <fstream>
#include <iostream>

#include "cc/gen/Module.h"
#include "cc/gen/Builder.h"
#include "cc/gen/Constant.h"

#include "cc/emit/x86/X86Target.h"

int main() {
	auto module = std::make_unique<cc::Module>("[module]");

	cc::Function* fmain = module->addFunction(
		"main",
		cc::FunctionType::get(cc::Type::getPointer(cc::Type::getInteger(32)), nullptr, 0, false)
	);

	cc::Builder b;
	b.insertInto(fmain->appendBlock("entry"));

	auto al = b.alloca_(cc::Type::getInteger(32));
	b.store(al, cc::ConstantInteger::get(67));
	b.ret(al);

	std::cout << module->dump() << std::endl;

	cc::x86Target target(std::endian::native, true);
	auto coff = target.emitObject(module.get());

	std::ofstream file("test.o");
	file.write(coff.first.get(), coff.second);
	file.close();

	return 0;
}

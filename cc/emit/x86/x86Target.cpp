#include "x86Target.h"

#include <ctime>
#include <vector>
#include <string.h>
#include <Windows.h>

#include "x86BlockEmitter.h"
#include "../../gen/Module.h"
#include "x86EmissionContext.h"

namespace cc {
	std::pair<std::unique_ptr<char[]>, uint32_t> x86Target::emitObject(Module* module) {
		IMAGE_FILE_HEADER fh;
		fh.Machine = arch == x86Arch ? IMAGE_FILE_MACHINE_I386 : IMAGE_FILE_MACHINE_AMD64;
		fh.NumberOfSections = 0;
		fh.TimeDateStamp = (DWORD)time(NULL);
		fh.PointerToSymbolTable = 0;
		fh.NumberOfSymbols = 0;
		fh.SizeOfOptionalHeader = 0;
		fh.Characteristics = IMAGE_FILE_EXECUTABLE_IMAGE;
		if (arch == x86Arch)
			fh.Characteristics |= IMAGE_FILE_32BIT_MACHINE;

		std::vector<char> buffer;

		buffer.resize(sizeof(IMAGE_FILE_HEADER));
		memset(buffer.data(), 0, sizeof(IMAGE_FILE_HEADER));

		if (!module->functions.empty()) {
			IMAGE_SECTION_HEADER sh;
			memset(&sh, 0, sizeof(IMAGE_SECTION_HEADER));
			fh.NumberOfSections++;

			strcpy_s((char*)sh.Name, IMAGE_SIZEOF_SHORT_NAME, ".text");
			sh.Characteristics = IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ;

			uint32_t shOffset = (uint32_t)buffer.size();
			buffer.resize(buffer.size() + sizeof(IMAGE_SECTION_HEADER));
			memset(buffer.data() + shOffset, 0, sizeof(IMAGE_SECTION_HEADER));

			uint32_t codeOffset = (uint32_t)buffer.size();
			sh.PointerToRawData = codeOffset;

			x86BlockEmitter be(this);
			x86EmissionContext ctx((std::vector<uint8_t>&)buffer, this, &sh);

			for (auto& func : module->functions)
				for (auto& block : func.blocks)
					be.emit(&ctx, &block);

			sh.SizeOfRawData = (uint32_t)buffer.size() - codeOffset;

			memcpy_s(buffer.data() + shOffset, sizeof(IMAGE_SECTION_HEADER), &sh, sizeof(IMAGE_SECTION_HEADER));
		}

		memcpy_s(buffer.data(), sizeof(IMAGE_FILE_HEADER), &fh, sizeof(IMAGE_FILE_HEADER));

		uint32_t size = (uint32_t)buffer.size();
		auto outBuffer = std::make_unique<char[]>(size);
		memcpy_s(outBuffer.get(), size, buffer.data(), size);
		return { std::move(outBuffer), size };
	}
}

#include "x86Target.h"

#include <ctime>
#include <vector>
#include <string.h>
#include <Windows.h>

#include "x86BlockEmitter.h"
#include "x86GlobalEmitter.h"
#include "../../gen/Module.h"
#include "x86EmissionContext.h"

namespace cc {
	std::pair<std::unique_ptr<char[]>, uint32_t> x86Target::emitObject(Module* module) {
		IMAGE_FILE_HEADER fh = {};
		fh.Machine = arch.is64 ? IMAGE_FILE_MACHINE_AMD64 : IMAGE_FILE_MACHINE_I386;
		fh.NumberOfSections = 0;
		fh.TimeDateStamp = (DWORD)time(NULL);
		fh.PointerToSymbolTable = 0;
		fh.NumberOfSymbols = 0;
		fh.SizeOfOptionalHeader = 0;
		fh.Characteristics = IMAGE_FILE_EXECUTABLE_IMAGE;
		if (!arch.is64)
			fh.Characteristics |= IMAGE_FILE_32BIT_MACHINE;

		std::vector<uint8_t> buffer;
		buffer.resize(sizeof(IMAGE_FILE_HEADER), 0);

		std::vector<IMAGE_SECTION_HEADER> headers;
		if (!module->functions.empty()) {
			IMAGE_SECTION_HEADER sh = {};
			fh.NumberOfSections++;
			strcpy_s((char*)sh.Name, IMAGE_SIZEOF_SHORT_NAME, ".text");
			sh.Characteristics = IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ;
			headers.push_back(sh);
		}

		if (!module->globals.empty()) {
			IMAGE_SECTION_HEADER sh = {};
			fh.NumberOfSections++;
			strcpy_s((char*)sh.Name, IMAGE_SIZEOF_SHORT_NAME, ".data");
			sh.Characteristics = IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE;
			headers.push_back(sh);
		}

		size_t headersOffset = buffer.size();
		buffer.resize(buffer.size() + headers.size() * sizeof(IMAGE_SECTION_HEADER), 0);

		std::vector<IMAGE_SYMBOL> symbols;
		std::vector<IMAGE_BASE_RELOCATION> relocs;

		x86EmissionContext ctx(buffer, this, symbols, relocs);

		uint8_t sectionIdx = 1;
		for (auto& sh : headers) {
			uint32_t dataOffset = (uint32_t)buffer.size();
			sh.PointerToRawData = dataOffset;

			ctx.sectionIdx = sectionIdx++;

			if (strcmp((char*)sh.Name, ".text") == 0) {
				x86BlockEmitter be;
				ctx.blockEmitter = &be;
				for (auto& func : module->functions)
					for (auto& block : func.blocks)
						be.emit(&ctx, &block);
			} else if (strcmp((char*)sh.Name, ".data") == 0) {
					x86GlobalEmitter ge;
					for (auto& global : module->globals)
						ge.emit(&ctx, &global);
			}

			sh.SizeOfRawData = (uint32_t)buffer.size() - dataOffset;
		}

		if (!symbols.empty()) {
			size_t symOffset = buffer.size();
			buffer.resize(buffer.size() + symbols.size() * sizeof(IMAGE_SYMBOL), 0);
			memcpy_s(buffer.data() + symOffset, symbols.size() * sizeof(IMAGE_SYMBOL), symbols.data(), symbols.size() * sizeof(IMAGE_SYMBOL));

			fh.PointerToSymbolTable = (DWORD)symOffset;
			fh.NumberOfSymbols = (DWORD)symbols.size();
		}

		uint32_t sizeofSt = 4;
		uint32_t stOffset = (uint32_t)buffer.size();
		buffer.resize(buffer.size() + sizeofSt, 0);
		memcpy_s(buffer.data() + stOffset, sizeofSt, &sizeofSt, sizeofSt);

		memcpy_s(buffer.data(), sizeof(IMAGE_FILE_HEADER), &fh, sizeof(IMAGE_FILE_HEADER));
		memcpy_s(buffer.data() + headersOffset, headers.size() * sizeof(IMAGE_SECTION_HEADER), headers.data(), headers.size() * sizeof(IMAGE_SECTION_HEADER));

		uint32_t size = (uint32_t)buffer.size();
		auto outBuffer = std::make_unique<char[]>(size);
		memcpy_s(outBuffer.get(), size, buffer.data(), size);
		return { std::move(outBuffer), size };
	}
}

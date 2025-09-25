#include "x86GlobalEmitter.h"

#include <cassert>
#include <Windows.h>

#include "x86EmissionContext.h"
#include "../../gen/Constant.h"

namespace cc {
	void x86GlobalEmitter::emit(EmissionContext* ctx_, Global* global) {
		x86EmissionContext* ctx = (x86EmissionContext*)ctx_;

		if (global->initializer->type->typeId == FunctionTypeID)
			return;

		uint32_t offset = (uint32_t)ctx->buffer.size();
		if (global->initializer) {
			assert(global->initializer->vk == ConstantValueKind);
			assert(global->initializer->type->typeId != VoidTypeID);
			switch (global->initializer->type->typeId) {
				case FloatTypeID: {
					union {
						float f;
						uint32_t n;
					} conv;
					conv.f = (float)((ConstantFP*)global->initializer)->val;
					ctx->target->encode(ctx, conv.n);
				} break;
				case DoubleTypeID: {
					union {
						double f;
						uint64_t n;
					} conv;
					conv.f = ((ConstantFP*)global->initializer)->val;
					ctx->target->encode(ctx, conv.n);
				} break;
				case IntegerTypeID: {
					ConstantInteger* c = (ConstantInteger*)global->initializer;
					ctx->target->encode(ctx, c->val, ((IntegerType*)c->type)->width);
				} break;
				default:
					ctx->buffer.push_back(0);
					break;
			}
		} else
			ctx->buffer.push_back(0);

		if (global->linkage != PrivateLinkage) {
			IMAGE_SYMBOL sym = {};
			size_t len = strlen(global->name);
			if (len <= IMAGE_SIZEOF_SHORT_NAME) {
				memcpy(sym.N.ShortName, global->name, len);
			} else {
				sym.N.Name.Short = 0;
				sym.N.Name.Long = 0; // TODO
			}
			sym.Value = offset;
			sym.SectionNumber = ctx->sectionIdx;
			sym.Type = IMAGE_SYM_TYPE_NULL;
			sym.StorageClass = global->linkage == PublicLinkage ? IMAGE_SYM_CLASS_EXTERNAL : IMAGE_SYM_CLASS_STATIC;
			sym.NumberOfAuxSymbols = 0;
		
			ctx->symbols.push_back(sym);
		}
	}
}

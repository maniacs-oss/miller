#ifndef TYPE_DECL_H
#define TYPE_DECL_H

#include "../containers/mlrval.h"

// ----------------------------------------------------------------
// These use the MT defines from mlrval.h, except that map-types (mlhmmv.h)
// are outside of mlrval.h.
#define TYPE_MASK_ERROR    (1 << MT_ERROR)
#define TYPE_MASK_ABSENT   (1 << MT_ABSENT)
#define TYPE_MASK_EMPTY    (1 << MT_EMPTY)
#define TYPE_MASK_STRING  ((1 << MT_STRING) | (1 << MT_EMPTY))
#define TYPE_MASK_INT      (1 << MT_INT)
#define TYPE_MASK_FLOAT    (1 << MT_FLOAT)
#define TYPE_MASK_NUMERIC (TYPE_MASK_INT | TYPE_MASK_FLOAT)
#define TYPE_MASK_BOOLEAN  (1 << MT_BOOLEAN)
// Initial declaration is an empty map.
// xxx maybe change that? For oosvars there's no empty map but for local mapvars there could be?
// Or, simply use the convention that empty map <=> absent?
#define TYPE_MASK_MAP     ((1 << MT_DIM) | (1 << MT_ABSENT))
#define TYPE_MASK_ANY     (~0)

// ----------------------------------------------------------------
char* type_mask_to_desc(int type_mask);
static inline int type_mask_from_mv(mv_t* pmv) {
	return 1 << pmv->type;
}

#endif // TYPE_DECL_H

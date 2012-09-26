#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "fixml-attr.h"
#include "fixml-attr-rev.c"

#include "fixml_to_fixc_prep.c"

#define countof(x)	(sizeof(x) / sizeof(*x))

static int
fixml50sp2_p(fixc_attr_t tag)
{
	switch (tag) {
	case FIXC_ATTR_CRSS_PCT:
	case FIXC_ATTR_EXP_TYPE:
	case FIXC_ATTR_IOIID:
	case FIXC_ATTR_LIST_ID:
	case FIXC_ATTR_RT_PRTY_ID:
	case FIXC_ATTR_RT_PTY_ID_SRC:
	case FIXC_ATTR_RT_PTY_SUB_ID:
	case FIXC_ATTR_RT_PTY_SUB_ID_TYP:
	case FIXC_ATTR_LIST_SEQ_NO:
	case FIXC_ATTR_LT_TYP:
	case FIXC_ATTR_NO_ALLOCS:
	case FIXC_ATTR_NO_ALT_MD_SRC:
	case FIXC_ATTR_NO_BID_COMPONENTS:
	case FIXC_ATTR_NO_BID_DESCPTRS:
	case FIXC_ATTR_NO_CAPACITIES:
	case FIXC_ATTR_NO_CLRNG_INSTRCTNS:
	case FIXC_ATTR_NO_CNTRA_BRKRS:
	case FIXC_ATTR_NO_COLL_INQ_QUAL:
	case FIXC_ATTR_NO_COMP_IDS:
	case FIXC_ATTR_NO_CONT_AMTS:
	case FIXC_ATTR_NO_DISTRIB_INSTS:
	case FIXC_ATTR_NO_DLVY_INST:
	case FIXC_ATTR_NO_EVENTS:
	case FIXC_ATTR_NO_EXECS:
	case FIXC_ATTR_NO_HOPS:
	case FIXC_ATTR_NO_INSTR_ATTRIB:
	case FIXC_ATTR_NO_IOI_QUALS:
	case FIXC_ATTR_NO_LEGS:
	case FIXC_ATTR_NO_LEG_ALLOCS:
	case FIXC_ATTR_NO_LEG_STIPS:
	case FIXC_ATTR_NO_LINES_OF_TEXT:
	case FIXC_ATTR_NO_MD_ENTRIES:
	case FIXC_ATTR_NO_MD_ENTRY_TYPS:
	case FIXC_ATTR_NO_MISC_FEES:
	case FIXC_ATTR_NO_NST2_PTY_IDS:
	case FIXC_ATTR_NO_NST2_PTY_SUB_IDS:
	case FIXC_ATTR_NO_NST3_PTY_IDS:
	case FIXC_ATTR_NO_NST3_PTY_SUB_IDS:
	case FIXC_ATTR_NO_NST_PTY_IDS:
	case FIXC_ATTR_NO_NST_PTY_SUB_IDS:
	case FIXC_ATTR_NO_ORDS:
	case FIXC_ATTR_NO_POSS:
	case FIXC_ATTR_NO_POS_AMT:
	case FIXC_ATTR_NO_PTY_IDS:
	case FIXC_ATTR_NO_PTY_SUB_IDS:
	case FIXC_ATTR_NO_QUOT_ENTRIES:
	case FIXC_ATTR_NO_QUOT_QUALS:
	case FIXC_ATTR_NO_QUOT_SETS:
	case FIXC_ATTR_NO_REGIST_DTLS:
	case FIXC_ATTR_NO_RELTD_SYM:
	case FIXC_ATTR_NO_RPTS:
	case FIXC_ATTR_NO_RTG_IDS:
	case FIXC_ATTR_NO_SEC_ALT_ID:
	case FIXC_ATTR_NO_SEC_TYPS:
	case FIXC_ATTR_NO_SETTL_INST:
	case FIXC_ATTR_NO_SETTL_PTY_IDS:
	case FIXC_ATTR_NO_SETTL_PTY_SUB_IDS:
	case FIXC_ATTR_NO_SIDES:
	case FIXC_ATTR_NO_STIPS:
	case FIXC_ATTR_NO_STRKS:
	case FIXC_ATTR_NO_TRDG_SESSS:
	case FIXC_ATTR_NO_TRDS:
	case FIXC_ATTR_NO_TRD_REG_TMSTAMPS:
	case FIXC_ATTR_NO_UNDS:
	case FIXC_ATTR_NO_UND_SEC_ALT_ID:
	case FIXC_ATTR_NO_UND_STIPS:
	case FIXC_ATTR_CRSS_PRIORTSTN:
	case FIXC_ATTR_FUT_VAL_METH:
	case FIXC_ATTR_STRK:
	case FIXC_ATTR_HIGH_LMT_PX:
	case FIXC_ATTR_RPT_REJ_RSN:
	case FIXC_ATTR_RSP:
	case FIXC_ATTR_RT_PTY_RL:
	case FIXC_ATTR_TRD_MTCH_ID:
	case FIXC_ATTR_TRD_RPT_ID2:
	case FIXC_ATTR_SET_SUB_ID:
		return 0;
	default:
		return 1;
	}
}

int
main(void)
{
	int res = 0;

	for (size_t i = 0; i < countof(tags); i++) {
		const fixc_attr_t tag = (fixc_attr_t)tags[i];
		const char *attr;

		if (!fixml50sp2_p(tag)) {
			continue;
		}

		attr = __aid_fixmlify(tag);
		if (strcmp(attrs[i], attr)) {
			/* cater for some ucase/lcase fuck ups */
			switch (tag) {
			case FIXC_ATTR_FX_RATE:
			case FIXC_ATTR_FX_RATE_CALC:
			case FIXC_ATTR_MLEG_RPT_TYP:
				break;
			default:
				fprintf(stderr, "\
is \"%s\"\t\
should_be \"%s\"\n",
					attr, attrs[i]);
				res = 1;
			}
		}
	}
	return res;
}

/* fixc_to_fixml_hash_01.c ends here */

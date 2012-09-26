/*** fixml-comp.h -- fixml attributes
 *
 * Copyright (C) 2010-2012 Sebastian Freundt
 *
 * Author:  Sebastian Freundt <freundt@ga-group.nl>
 *
 * This file is part of libfixc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the author nor the names of any contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **/
#if !defined INCLUDED_fixml_comp_h_
#define INCLUDED_fixml_comp_h_

typedef enum {
	/* these ones are our own */
	FIXC_COMP_UNK,
	/* fixml's root element */
	FIXC_COMP_FIXML,
	/* auto-gen'd by
	   cat spec/fixml_comp.all | \
	   cut -f2 | sort -u | \
	   sed -f spec/uncamel.sed -e 's/^/FIXC_COMP_/ | \
	   tr 'a-z' 'A-Z'
	   
	 * The first entry in this list is used to denote failure to map */
	FIXC_COMP_AID,
	FIXC_COMP_AFFECT_ORD,
	FIXC_COMP_ALL_EXC,
	FIXC_COMP_ALLOC,
	FIXC_COMP_ALLOC_ACK,
	FIXC_COMP_AMT,
	FIXC_COMP_APPL_ID_REQ_ACK_GRP,
	FIXC_COMP_APPL_ID_REQ_GRP,
	FIXC_COMP_APPL_ID_RPT_GRP,
	FIXC_COMP_APPL_SEQ_CTRL,
	FIXC_COMP_ATTRB,
	FIXC_COMP_BASE_TRDG_RULES,
	FIXC_COMP_BNCHMK_CURVE,
	FIXC_COMP_CID_REQ,
	FIXC_COMP_CID_STAT,
	FIXC_COMP_CLR_INST,
	FIXC_COMP_CMPLX_EVNT,
	FIXC_COMP_COLL_EXC,
	FIXC_COMP_COMM,
	FIXC_COMP_COMP_REQ,
	FIXC_COMP_COMP_RSP,
	FIXC_COMP_CONT_AMT,
	FIXC_COMP_CONTRA,
	FIXC_COMP_CPCTY,
	FIXC_COMP_DERIV_INSTRMT,
	FIXC_COMP_DERIV_SEC_DEF,
	FIXC_COMP_DESC_REQ,
	FIXC_COMP_DISC_INSTR,
	FIXC_COMP_DLV_INST,
	FIXC_COMP_DSPLY_INSTR,
	FIXC_COMP_EVNT,
	FIXC_COMP_EVNT_DTS,
	FIXC_COMP_EVNT_TMS,
	FIXC_COMP_EXEC,
	FIXC_COMP_EXEC_INST_RULES,
	FIXC_COMP_FILLS_GRP,
	FIXC_COMP_FIN_DETLS,
	FIXC_COMP_FULL,
	FIXC_COMP_HOP,
	FIXC_COMP_IOI,
	FIXC_COMP_INC,
	FIXC_COMP_INST,
	FIXC_COMP_INST_REQ,
	FIXC_COMP_INSTRMT,
	FIXC_COMP_INSTRMT_EXT,
	FIXC_COMP_INSTRMTS,
	FIXC_COMP_LEG,
	FIXC_COMP_LEG_AID,
	FIXC_COMP_LIST_STAT,
	FIXC_COMP_LOT_TYPE_RULES,
	FIXC_COMP_MD_FEED_TYPS,
	FIXC_COMP_MAT_RULES,
	FIXC_COMP_MISC_FEES,
	FIXC_COMP_MKT_SEG_GRP,
	FIXC_COMP_MSG_TYPE_GRP,
	FIXC_COMP_MTCH_RULES,
	FIXC_COMP_NOT_AFFECTED_ORDERS_GRP,
	FIXC_COMP_ORD,
	FIXC_COMP_ORD_ALLOC,
	FIXC_COMP_ORD_QTY,
	FIXC_COMP_ORD_TYP_RULES,
	FIXC_COMP_PEG_INSTR,
	FIXC_COMP_POS_UND,
	FIXC_COMP_PRE_ALL,
	FIXC_COMP_PRE_ALLOC_MLEG,
	FIXC_COMP_PTY,
	FIXC_COMP_PX_LMTS,
	FIXC_COMP_PX_LMTS2,
	FIXC_COMP_QTY,
	FIXC_COMP_QUAL,
	FIXC_COMP_QUOT,
	FIXC_COMP_QUOT_CXL_ENTRY,
	FIXC_COMP_QUOT_ENTRY,
	FIXC_COMP_QUOT_ENTRY_ACK,
	FIXC_COMP_QUOT_QUAL,
	FIXC_COMP_QUOT_REQ,
	FIXC_COMP_QUOT_REQ_REJ,
	FIXC_COMP_QUOT_SET,
	FIXC_COMP_QUOT_SET_ACK,
	FIXC_COMP_QUOTE_STAT,
	FIXC_COMP_RFQ_REQ,
	FIXC_COMP_REFS,
	FIXC_COMP_REL_SYM,
	FIXC_COMP_REQ,
	FIXC_COMP_REQS,
	FIXC_COMP_RG_DTL,
	FIXC_COMP_RG_DTL_INST,
	FIXC_COMP_RJCT,
	FIXC_COMP_RPT_SIDE,
	FIXC_COMP_RPTS,
	FIXC_COMP_RT_PRTYS,
	FIXC_COMP_RT_SRC,
	FIXC_COMP_RT_SUB_PRTYS,
	FIXC_COMP_RTG,
	FIXC_COMP_SEC_L,
	FIXC_COMP_SEC_LST_UPD_REL_SYMS_LEG_GRP,
	FIXC_COMP_SEC_SIZES_GRP,
	FIXC_COMP_SEC_T,
	FIXC_COMP_SEC_TRDG_RULES,
	FIXC_COMP_SEC_XML,
	FIXC_COMP_SET_INST,
	FIXC_COMP_SET_INSTR,
	FIXC_COMP_SETTL_DETAILS,
	FIXC_COMP_SETTL_OBLIG_INST,
	FIXC_COMP_SIDE_CROSS_CXL,
	FIXC_COMP_SIDE_CROSS_MOD,
	FIXC_COMP_SPRD_BNCHMK_CURVE,
	FIXC_COMP_STATS_IND_GRP,
	FIXC_COMP_STIP,
	FIXC_COMP_STRK_PX,
	FIXC_COMP_STRK_RULES,
	FIXC_COMP_STRP_PRM_GRP,
	FIXC_COMP_STRT_PRM_GRP,
	FIXC_COMP_SUB,
	FIXC_COMP_TGT_PTY,
	FIXC_COMP_TICK_RULES,
	FIXC_COMP_TM_IN_FORCE_RULES,
	FIXC_COMP_TRADE_CAP_LEG_UNDLY_GRP,
	FIXC_COMP_TRD_CAP_DT,
	FIXC_COMP_TRD_COLL,
	FIXC_COMP_TRD_LEG,
	FIXC_COMP_TRD_REG_TS,
	FIXC_COMP_TRD_REP_INDICATORS_GRP,
	FIXC_COMP_TRD_RPT_ORD_DETL,
	FIXC_COMP_TRD_SES,
	FIXC_COMP_TRD_SESS_LST_GRP,
	FIXC_COMP_TRGR_INSTR,
	FIXC_COMP_TXT_LN,
	FIXC_COMP_UND_AID,
	FIXC_COMP_UND_COLL,
	FIXC_COMP_UND_DLV_AMT,
	FIXC_COMP_UND_STRK_PX,
	FIXC_COMP_UNDLY,
	FIXC_COMP_USER_GRP,
	FIXC_COMP_YIELD,
} fixc_comp_t;

#endif	/* INCLUDED_fixml_comp_h_ */

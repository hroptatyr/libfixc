#if !defined INCLUDED_fixml_to_fixc_prep_c_
#define INCLUDED_fixml_to_fixc_prep_c_

#include "fixml-attr.h"

static __attribute__((unused)) const char attrs[][24] = {
	"xmlns",
	"v",
	"AOSCurr",
	"AOSPct",
	"AOSValu",
	"Acct",
	"AcctIDSrc",
	"AcctTyp",
	"AcrdIntAmt",
	"AcrdIntRt",
	"ActIDSrc",
	"ActTyp",
	"Actn",
	"AdjQty",
	"AdjTyp",
	"Adjmt",
	"AdvId",
	"AdvRefID",
	"AdvSide",
	"AdvTransTyp",
	"AffctdOrdID",
	"AffctdScndOrdID",
	"AffirmStat",
	"AggBook",
	"AgmtCcy",
	"AgmtDesc",
	"AgmtDt",
	"AgmtID",
	"AgrsrInd",
	"AllocAcct",
	"AllocAcctIDSrc",
	"AllocID",
	"AllocID2",
	"AllocInd",
	"AllocPct",
	"AllocPosEfct",
	"AllocQty",
	"AllocSettlCcy",
	"AllocSettlCurrAmt",
	"AllocType",
	"AltID",
	"AltIDSrc",
	"AltMDSrcID",
	"Amt",
	"ApplBegSeqNum",
	"ApplEndSeq",
	"ApplID",
	"ApplLastSeqNum",
	"ApplNewSeqNum",
	"ApplQuActn",
	"ApplQuDepth",
	"ApplQuMax",
	"ApplQuResolution",
	"ApplReqID",
	"ApplReqTyp",
	"ApplResendFlag",
	"ApplRespErr",
	"ApplRespID",
	"ApplRespTyp",
	"ApplRptID",
	"ApplRptTyp",
	"ApplSeqNum",
	"ApplTotMsgCnt",
	"ApplTyp",
	"ApplVerID",
	"AsOfInd",
	"AsgnMeth",
	"AsgnReqTyp",
	"AsgnRsn",
	"AsgnTyp",
	"AttchPnt",
	"AutoAcceptInd",
	"AvgParPx",
	"AvgPx",
	"AvgPxInd",
	"AvgPxPrcsn",
	"Basis",
	"BasisFeatureDt",
	"BasisFeaturePx",
	"BasisPxTyp",
	"BidDescptr",
	"BidDescptrTyp",
	"BidFwdPnts",
	"BidFwdPnts2",
	"BidID",
	"BidPx",
	"BidReqTransTyp",
	"BidSpotRt",
	"BidSwapPnts",
	"BidSz",
	"BidTrdTyp",
	"BidTyp",
	"BidYld",
	"BizDt",
	"BizRejRefID",
	"BizRejRsn",
	"BkngQty",
	"BkngRefID",
	"BkngTyp",
	"BkngUnit",
	"BuyVol",
	"Buyer",
	"CFI",
	"CPPgm",
	"CPRegT",
	"CPRegTyp",
	"CSetMo",
	"CalcCcyLastQty",
	"CalcDt",
	"CapPx",
	"CapValu",
	"CardExpDt",
	"CardHolderName",
	"CardIssNum",
	"CardNum",
	"CardStartDt",
	"Cash",
	"CashAmt",
	"CashTyp",
	"Ccy",
	"CcyAmt",
	"ClBidID",
	"ClrFeeInd",
	"ClrngInstrctn",
	"ClsTm",
	"Cmult",
	"CnfmID",
	"CnfmRefID",
	"CnfmRejRsn",
	"CnfmReqID",
	"CnfmStat",
	"CnfmTransTyp",
	"CnfmTyp",
	"CntraBrkr",
	"ColAmt",
	"Comm",
	"CommTyp",
	"ComplianceID",
	"Concession",
	"Cond",
	"ContAmtCurr",
	"ContAmtTyp",
	"ContAmtValu",
	"ContingencyType",
	"CopyMsgInd",
	"CorpActn",
	"Cover",
	"Covered",
	"Cpcty",
	"CpctyQty",
	"CpnPmt",
	"CpnRt",
	"CrdRtg",
	"CrssPct",
	"CrssPriortstn",
	"CrssTyp",
	"CshDistribAgentAcctName",
	"CshDistribAgentAcctNum",
	"CshDistribAgentCode",
	"CshDistribAgentName",
	"CshDistribCurr",
	"CshDistribPayRef",
	"CshMgn",
	"CshOutstanding",
	"Ctry",
	"CumQty",
	"CurVal",
	"Curr",
	"CurrencyRatio",
	"CustCpcty",
	"CustDrctdOrd",
	"CustOrdHdlInst",
	"CxlQty",
	"CxlRejRsn",
	"CxlRejRspTo",
	"CxlRplcRsn",
	"CxlTyp",
	"CxllationRights",
	"CycleNo",
	"D2ID",
	"D2Loc",
	"D2Sub",
	"Dated",
	"DayAvgPx",
	"DayBkngInst",
	"DayCumQty",
	"DayOrdQty",
	"DealingCpcty",
	"DefApplVerID",
	"DefBidSz",
	"DefOfrSz",
	"DelRsn",
	"Desc",
	"Designation",
	"DeskID",
	"DetchPnt",
	"DfltApplExtID",
	"DfltVerInd",
	"DirtPx",
	"DisplayHighQty",
	"DisplayQty",
	"DistribPctage",
	"DistribPmtMethod",
	"DividendYield",
	"DkRsn",
	"DlvDt",
	"DlvryForm",
	"DlvryTyp",
	"DsctnInst",
	"DsctnPx",
	"DskOrdHndlInst",
	"DskTyp",
	"DskTypSrc",
	"DspMinIncr",
	"DspMthd",
	"DspWhn",
	"DsplLwQty",
	"Dt",
	"DtOfBirth",
	"Dtls",
	"DueToReltd",
	"EFP",
	"EFPTrkngErr",
	"EfctvTm",
	"Email",
	"EmailThreadID",
	"EmailTyp",
	"EncAllocText",
	"EncAllocTextLen",
	"EncHeadline",
	"EncHeadlineLen",
	"EncIssr",
	"EncIssrLen",
	"EncLegIssr",
	"EncLegIssrLen",
	"EncLegSecDesc",
	"EncLegSecDescLen",
	"EncListExecInst",
	"EncListExecInstLen",
	"EncListStatText",
	"EncListStatTextLen",
	"EncNewPwd",
	"EncPwd",
	"EncPwdMethod",
	"EncSecDesc",
	"EncSecDescLen",
	"EncSubject",
	"EncSubjectLen",
	"EncTxt",
	"EncTxtLen",
	"EncUndIssr",
	"EncUndIssrLen",
	"EncUndSecDesc",
	"EncUndSecDescLen",
	"EncodedMktSegmDesc",
	"EncodedMktSegmDescLen",
	"EndAcrdIntAmt",
	"EndCsh",
	"EndDt",
	"EndMMY",
	"EndPx",
	"EndStrkPxRng",
	"EndTickPxRng",
	"EndTm",
	"EndVal",
	"EntryID",
	"EntryRejRsn",
	"EventTyp",
	"ExDest",
	"ExDestIDSrc",
	"ExDt",
	"Exch",
	"ExchRule",
	"ExchSpeclInstr",
	"ExecAckStat",
	"ExecID",
	"ExecID2",
	"ExecInst",
	"ExecInstValu",
	"ExecPxAdjment",
	"ExecPxTyp",
	"ExecRefID",
	"ExecRstmtRsn",
	"ExecTyp",
	"ExecValuationPoint",
	"ExerStyle",
	"ExpQty",
	"ExpTyp",
	"ExpType",
	"ExpirationCycle",
	"ExpireDt",
	"ExpireTm",
	"ExrMethod",
	"FXRate",
	"FXRateCalc",
	"FairValu",
	"Fctr",
	"FeeMult",
	"FillExecID",
	"FillPx",
	"FillQty",
	"FillStationCd",
	"FinclStat",
	"FirmTrdID",
	"FirmTrdID2",
	"FirstPx",
	"FlexInd",
	"FlexProdElig",
	"FlowSchedTyp",
	"FlrPx",
	"ForexReq",
	"FundRenewWaiv",
	"FutValMeth",
	"FxRate",
	"FxRateCalc",
	"GTBkngInst",
	"GrossTrdAmt",
	"HaltRsn",
	"HandlInst",
	"Headline",
	"HiLmtPx",
	"HighLmtPx",
	"HighPx",
	"HndInst",
	"HstCxID",
	"ID",
	"ID2",
	"IOIID",
	"IOIQty",
	"ImReqTyp",
	"ImplctDel",
	"ImpldMktInd",
	"InViewOfCmn",
	"IncTaxInd",
	"IndAllocID",
	"IndAllocID2",
	"IndAllocRejCode",
	"InptDev",
	"InptSrc",
	"InstSrc",
	"InstTyp",
	"InstrctnInd",
	"IntAcrl",
	"IntAtMat",
	"InvestorCtryOfResidence",
	"IssDt",
	"Issr",
	"IssuCtry",
	"Issued",
	"LangCd",
	"LastCpcty",
	"LastFragment",
	"LastFwdPnts",
	"LastFwdPnts2",
	"LastLqdtyInd",
	"LastMkt",
	"LastNtwkRspID",
	"LastParPx",
	"LastPx",
	"LastQty",
	"LastRptReqed",
	"LastSpotRt",
	"LastSwapPnts",
	"LastUpdateTm",
	"LateInd",
	"Lcl",
	"LctnID",
	"LeavesQty",
	"LegAllocID",
	"LegBidFwdPnts",
	"LegCalcCcyLastQty",
	"LegCurrencyRatio",
	"LegDividendYield",
	"LegExecInst",
	"LegGrossTrdAmt",
	"LegLastFwdPnts",
	"LegNo",
	"LegOfrFwdPnts",
	"LegOptionRatio",
	"LegRefID",
	"LegVolatility",
	"LegalCnfm",
	"LimitTyp",
	"LinkID",
	"LinkTyp",
	"ListDesc",
	"ListExecInst",
	"ListExecInstTyp",
	"ListID",
	"ListMeth",
	"ListName",
	"ListOrdStat",
	"ListRefID",
	"ListRejectReason",
	"ListReqTyp",
	"ListSeqNo",
	"ListStatText",
	"ListStatTyp",
	"ListTyp",
	"ListUpdActn",
	"LmtTyp",
	"LnkID",
	"LocReqd",
	"Long",
	"LotTyp",
	"LowLmtPx",
	"LowPx",
	"LqdtyInd",
	"LqdtyIndTyp",
	"LqdtyNumSecurities",
	"LqdtyPctHigh",
	"LqdtyPctLow",
	"LqdtyValu",
	"LstTypSrc",
	"LtTyp",
	"MDBkTyp",
	"MDEntryFwdPnts",
	"MDEntrySpotRt",
	"MDFeedTyp",
	"MDOrigTyp",
	"MDPxLvl",
	"MDQteTyp",
	"MDSecSize",
	"MDSecSizeType",
	"MDStrmID",
	"MDSubBkTyp",
	"MLEGRptTyp",
	"MLEGRptTypReq",
	"MLegRptTyp",
	"MMY",
	"MMYFmt",
	"MMYIncr",
	"MMYIncrUnits",
	"MailingDtls",
	"MailingInst",
	"ManOrdInd",
	"MarketSegmentDesc",
	"MassActionRejectReason",
	"MassActionReportID",
	"MassActionResponse",
	"MassActionScope",
	"MassActionType",
	"MassCxlRejRsn",
	"MassCxlRsp",
	"Mat",
	"MatDt",
	"MatNetMny",
	"MatRuleID",
	"MatTm",
	"MaxFloor",
	"MaxShow",
	"MaxTrdVol",
	"Meth",
	"Method",
	"MgnExcess",
	"MgnRatio",
	"MidPx",
	"MidYld",
	"MinBidSz",
	"MinLotSz",
	"MinOfrSz",
	"MinPxIncr",
	"MinPxIncrAmt",
	"MinQty",
	"MinTrdVol",
	"Mkt",
	"MktBidPx",
	"MktDepth",
	"MktID",
	"MktOfrPx",
	"MktReqID",
	"MktRptID",
	"MktSegID",
	"MktUpdtActn",
	"MlegModel",
	"MlegPxMeth",
	"MnyLaunderingStat",
	"Mode",
	"ModelTyp",
	"MoveTyp",
	"MsgEncd",
	"MsgEvtSrc",
	"MsgTyp",
	"MtchAlgo",
	"MtchID",
	"MtchInc",
	"MtchStat",
	"MtchTyp",
	"Mult",
	"MultTyp",
	"MxPxLvls",
	"MxPxVar",
	"NTPosLmt",
	"Name",
	"NatFlag",
	"NetChgPrevDay",
	"NetGrossInd",
	"NetMny",
	"NewPassword",
	"NewsCatgy",
	"NoAffctdOrds",
	"NoAllocs",
	"NoAltMDSrc",
	"NoBidComponents",
	"NoBidDescptrs",
	"NoCapacities",
	"NoClrngInstrctns",
	"NoCntraBrkrs",
	"NoCollInqQual",
	"NoCompIDs",
	"NoContAmts",
	"NoDistribInsts",
	"NoDlvyInst",
	"NoDts",
	"NoEvents",
	"NoExecs",
	"NoHops",
	"NoIOIQuals",
	"NoInstrAttrib",
	"NoLegAllocs",
	"NoLegSecAltID",
	"NoLegStips",
	"NoLegs",
	"NoLinesOfText",
	"NoMDEntries",
	"NoMDEntryTyps",
	"NoMiscFees",
	"NoNested4PartyIDs",
	"NoNested4PartySubIDs",
	"NoNst2PtyIDs",
	"NoNst2PtySubIDs",
	"NoNst3PtyIDs",
	"NoNst3PtySubIDs",
	"NoNstPtyIDs",
	"NoNstPtySubIDs",
	"NoOrds",
	"NoOrdsTyp",
	"NoPosAmt",
	"NoPoss",
	"NoPtyIDs",
	"NoPtySubIDs",
	"NoQuotEntries",
	"NoQuotQuals",
	"NoQuotSets",
	"NoRegistDtls",
	"NoReltdSym",
	"NoRpts",
	"NoRtgIDs",
	"NoSecAltID",
	"NoSecTyps",
	"NoSettlInst",
	"NoSettlPtyIDs",
	"NoSettlPtySubIDs",
	"NoSides",
	"NoStips",
	"NoStrks",
	"NoTrdRegTmstamps",
	"NoTrdgSesss",
	"NoTrds",
	"NoUndSecAltID",
	"NoUndStips",
	"NoUnds",
	"NotAffOrigClOrdID",
	"NotAffectedOrderID",
	"NotifyBrkrOfCredit",
	"NotlPctOut",
	"NtwkReqID",
	"NtwkReqTyp",
	"NtwkRspID",
	"NtwkStatRspTyp",
	"NumBidders",
	"NumDaysInt",
	"NumOfOrds",
	"NumTkts",
	"OBID",
	"OBLoc",
	"OBSub",
	"OddLot",
	"OfrFwdPnts",
	"OfrFwdPnts2",
	"OfrPx",
	"OfrSpotRt",
	"OfrSwapPnts",
	"OfrSz",
	"OfrYld",
	"OfstTyp",
	"OfstVal",
	"OfstValu",
	"OpenCloseSettlFlag",
	"OpenClsSettlFlag",
	"OpenInt",
	"OpenTm",
	"OptA",
	"OptAt",
	"OptPayAmt",
	"OptPayoutTyp",
	"OrdCat",
	"OrdDelay",
	"OrdDelayUnit",
	"OrdHndlInstSrc",
	"OrdID",
	"OrdID2",
	"OrdInptDev",
	"OrdQty",
	"Orig",
	"OrigCrssID",
	"OrigCustOrdCpcty",
	"OrigID",
	"OrigNotlPctOut",
	"OrigOrdModTm",
	"OrigReqRefID",
	"OrigSnt",
	"OrigTm",
	"OrigTrdDt",
	"OrigTrdHandlInst",
	"OrigTrdID",
	"OrignDt",
	"OrignTrdID2",
	"OutMainCntryUNdx",
	"OutsideNdxPct",
	"OwnerTyp",
	"OwnershipTyp",
	"ParentMktSegmID",
	"ParticipationRt",
	"Password",
	"PayAmt",
	"Pct",
	"PctAtRisk",
	"PegPxTyp",
	"PegSecDesc",
	"PegSecID",
	"PegSecurityIDSource",
	"PeggedPx",
	"PgSymbl",
	"PggdRefPx",
	"PmtDt",
	"PmtMethod",
	"PmtRef",
	"PmtRemtrID",
	"Point",
	"Pool",
	"PosDup",
	"PosEfct",
	"PosLmt",
	"PosNo",
	"PosRsnd",
	"PrTrdAnon",
	"PreClsTm",
	"PreallocMeth",
	"PrevClsPx",
	"PrevlyRpted",
	"PriInd",
	"PriSetPx",
	"PriorSpreadInd",
	"Priorty",
	"ProcCode",
	"Prod",
	"ProdCmplx",
	"ProgPeriodIntvl",
	"ProgRptReqs",
	"PrvtQt",
	"PtyRole",
	"PubTrdInd",
	"PutCall",
	"Px",
	"Px2",
	"PxBndryMeth",
	"PxBndryPrcsn",
	"PxDelta",
	"PxDtrmnMeth",
	"PxImprvmnt",
	"PxLmtTyp",
	"PxPrtScp",
	"PxQteMeth",
	"PxTmTyp",
	"PxTyp",
	"PxUOM",
	"PxUOMQty",
	"QCond",
	"QID",
	"QltyInd",
	"QtEntSts",
	"QtMsgID",
	"Qty",
	"Qty2",
	"QtyDt",
	"QtyTyp",
	"Qual",
	"QuotPxTyp",
	"R",
	"RFQReqID",
	"RFR",
	"RatioQty",
	"RawData",
	"RawDataLength",
	"RcvdDptID",
	"RedDt",
	"RedPx",
	"RedPxTyp",
	"Redeem",
	"Ref",
	"RefApplExtID",
	"RefApplID",
	"RefApplLastSeqNum",
	"RefApplVerID",
	"RefCompID",
	"RefCstmApplVerID",
	"RefID",
	"RefInd",
	"RefMsgTyp",
	"RefOrdID",
	"RefOrdIDRsn",
	"RefOrdIDSrc",
	"RefPg",
	"RefSeqNum",
	"RefSubID",
	"RefTagID",
	"RefTyp",
	"RegStat",
	"RejCode",
	"RejRsn",
	"RejRsnCd",
	"RejRsnTxt",
	"RejTxt",
	"RepoCollSecTyp",
	"RepoRt",
	"RepoTrm",
	"ReportedPxDiff",
	"ReqID",
	"ReqRejResn",
	"ReqRejRsn",
	"ReqRslt",
	"ReqStat",
	"ReqTyp",
	"RespID",
	"RespTyp",
	"RestrctTyp",
	"ReversalInd",
	"RfrshQty",
	"Rgstry",
	"RndDir",
	"RndLot",
	"RndMod",
	"RndPx",
	"RptID",
	"RptID2",
	"RptRefID",
	"RptRejRsn",
	"RptSeq",
	"RptSys",
	"RptToExch",
	"RptTyp",
	"RptedPx",
	"Rslt",
	"RsnCD",
	"Rsp",
	"RspDest",
	"RspID",
	"RspLvl",
	"RspTransportTyp",
	"RspTyp",
	"RspdntTyp",
	"Rstctions",
	"RtPrtyID",
	"RtPtyIDSrc",
	"RtPtyRl",
	"RtPtySubID",
	"RtPtySubIDTyp",
	"RtSrc",
	"RtSrcTyp",
	"RtgID",
	"RtgTyp",
	"SID",
	"SLoc",
	"SSub",
	"Schema",
	"Scope",
	"SecAltID",
	"SecAltIDSrc",
	"SecDspQty",
	"SecGrp",
	"SecID",
	"SecIDSrc",
	"SecSubTyp",
	"SecTrdEvnt",
	"SecTyp",
	"SecType",
	"SellVol",
	"Seller",
	"SellerDays",
	"SeqNo",
	"SeqNum",
	"SesID",
	"SesSub",
	"SessStat",
	"SetID",
	"SetMeth",
	"SetPx",
	"SetPxTyp",
	"SetSesID",
	"SetSesSub",
	"SetStat",
	"SetSubID",
	"SettlCcy",
	"SettlCurrAmt",
	"SettlCurrBidFxRt",
	"SettlCurrFxRt",
	"SettlCurrFxRtCalc",
	"SettlCurrOfrFxRt",
	"SettlDt",
	"SettlDt2",
	"SettlID",
	"SettlInstID",
	"SettlInstMode",
	"SettlInstMsgID",
	"SettlInstRefID",
	"SettlInstReqID",
	"SettlInstReqRejCode",
	"SettlInstTransTyp",
	"SettlInstTyp",
	"SettlMeth",
	"SettlMode",
	"SettlMsgID",
	"SettlOnOpenFlag",
	"SettlRefID",
	"SettlSrc",
	"SettlTransTyp",
	"SettlTyp",
	"Sfx",
	"SharedComm",
	"Short",
	"ShrtSaleRsn",
	"Side",
	"SideComplianceID",
	"SideExecID",
	"SideGrossTradeAmt",
	"SideQty",
	"SideTmFrc",
	"SideValu1",
	"SideValu2",
	"SideValuInd",
	"Snrty",
	"Snt",
	"SolFlag",
	"Spread",
	"Src",
	"StOrProvnc",
	"StPrv",
	"StandInstDbID",
	"StandInstDbName",
	"StandInstDbTyp",
	"StartCsh",
	"StartDt",
	"StartMMY",
	"StartStrkPxRng",
	"StartTickPxRng",
	"StartTm",
	"StartVal",
	"Stat",
	"StatRejRsn",
	"StatReqID",
	"StatText",
	"StatValu",
	"StatsTyp",
	"Status",
	"StipTyp",
	"StipVal",
	"StlDt",
	"StopPx",
	"Strk",
	"StrkCcy",
	"StrkExrStyle",
	"StrkIncr",
	"StrkMult",
	"StrkPx",
	"StrkPxBndryMeth",
	"StrkPxBndryPrcsn",
	"StrkPxDtrmnMeth",
	"StrkRule",
	"StrkTm",
	"StrkValu",
	"StrtPrmNme",
	"StrtPrmTyp",
	"StrtPrmVal",
	"SubReqTyp",
	"SubTyp",
	"SubType",
	"Subject",
	"SwapPnts",
	"SwapTyp",
	"Sym",
	"Sz",
	"TID",
	"TLoc",
	"TS",
	"TSub",
	"TZTransactTime",
	"TaxAdvantageTyp",
	"TgtStrategy",
	"TgtStrategyParameters",
	"TgtStrategyPerformance",
	"ThresholdAmt",
	"TickDirctn",
	"TickIncr",
	"TickRuleTyp",
	"TierCD",
	"Tm",
	"TmBkt",
	"TmInForce",
	"TmToExp",
	"TmUnit",
	"TotAcrdIntAmt",
	"TotAffctdOrds",
	"TotNetValu",
	"TotNoAccQts",
	"TotNoAllocs",
	"TotNoCxldQts",
	"TotNoFills",
	"TotNoOrds",
	"TotNoQuotEntries",
	"TotNoRejQts",
	"TotNoReltdSym",
	"TotNoSecTyps",
	"TotNoStrks",
	"TotNumAsgnRpts",
	"TotNumRpts",
	"TotNumTrdRpts",
	"TotRpts",
	"TotTakedown",
	"TotVolTrdd",
	"TradSesEvent",
	"TradSesUpdtActn",
	"TradingSessionDesc",
	"TransBkdTm",
	"TransTyp",
	"TrdCcy",
	"TrdCond",
	"TrdDt",
	"TrdHandlInst",
	"TrdID",
	"TrdID2",
	"TrdLegRefID",
	"TrdMtchID",
	"TrdPubInd",
	"TrdQty",
	"TrdRepInd",
	"TrdRptID2",
	"TrdRptRefID2",
	"TrdRptStat",
	"TrdSubTyp",
	"TrdTm",
	"TrdTyp",
	"TrdTyp2",
	"TrdVol",
	"TrddFlatSwitch",
	"TrdgRefPx",
	"TrdgStat",
	"Trdr",
	"TrgrActn",
	"TrgrNewPx",
	"TrgrNewQty",
	"TrgrOrdTyp",
	"TrgrPx",
	"TrgrPxDir",
	"TrgrPxTyp",
	"TrgrPxTypScp",
	"TrgrSecDesc",
	"TrgrSecID",
	"TrgrSecIDSrc",
	"TrgrSym",
	"TrgrTrdSessID",
	"TrgrTrdSessSubID",
	"TrgrTyp",
	"TrmTyp",
	"TrnsfrRsn",
	"TxnTm",
	"TxnTyp",
	"Txt",
	"Typ",
	"UOM",
	"UOMQty",
	"URL",
	"UndLastPx",
	"UndLastQty",
	"UndSesID",
	"UndSesSub",
	"UndSetPx",
	"UndSetPxTyp",
	"UndlyDlvAmt",
	"Unit",
	"Unsol",
	"UpdActn",
	"UpdtAct",
	"UpdtTyp",
	"Urgency",
	"UserReqID",
	"UserReqTyp",
	"UserStat",
	"UserStatText",
	"Username",
	"Val",
	"ValMeth",
	"ValidTil",
	"ValidUntilTm",
	"ValuOfFuts",
	"VenuTyp",
	"Vol",
	"WorkingInd",
	"WtAvgLqdty",
	"Yld",
};

static __attribute__((unused)) const fixc_attr_t tags[] = {
	FIXC_ATTR_XMLNS,
	FIXC_ATTR_V,
	FIXC_ATTR_AOS_CURR,
	FIXC_ATTR_AOS_PCT,
	FIXC_ATTR_AOS_VALU,
	FIXC_ATTR_ACCT,
	FIXC_ATTR_ACCT_ID_SRC,
	FIXC_ATTR_ACCT_TYP,
	FIXC_ATTR_ACRD_INT_AMT,
	FIXC_ATTR_ACRD_INT_RT,
	FIXC_ATTR_ACT_ID_SRC,
	FIXC_ATTR_ACT_TYP,
	FIXC_ATTR_ACTN,
	FIXC_ATTR_ADJ_QTY,
	FIXC_ATTR_ADJ_TYP,
	FIXC_ATTR_ADJMT,
	FIXC_ATTR_ADV_ID,
	FIXC_ATTR_ADV_REF_ID,
	FIXC_ATTR_ADV_SIDE,
	FIXC_ATTR_ADV_TRANS_TYP,
	FIXC_ATTR_AFFCTD_ORD_ID,
	FIXC_ATTR_AFFCTD_SCND_ORD_ID,
	FIXC_ATTR_AFFIRM_STAT,
	FIXC_ATTR_AGG_BOOK,
	FIXC_ATTR_AGMT_CCY,
	FIXC_ATTR_AGMT_DESC,
	FIXC_ATTR_AGMT_DT,
	FIXC_ATTR_AGMT_ID,
	FIXC_ATTR_AGRSR_IND,
	FIXC_ATTR_ALLOC_ACCT,
	FIXC_ATTR_ALLOC_ACCT_ID_SRC,
	FIXC_ATTR_ALLOC_ID,
	FIXC_ATTR_ALLOC_ID2,
	FIXC_ATTR_ALLOC_IND,
	FIXC_ATTR_ALLOC_PCT,
	FIXC_ATTR_ALLOC_POS_EFCT,
	FIXC_ATTR_ALLOC_QTY,
	FIXC_ATTR_ALLOC_SETTL_CCY,
	FIXC_ATTR_ALLOC_SETTL_CURR_AMT,
	FIXC_ATTR_ALLOC_TYPE,
	FIXC_ATTR_ALT_ID,
	FIXC_ATTR_ALT_ID_SRC,
	FIXC_ATTR_ALT_MD_SRC_ID,
	FIXC_ATTR_AMT,
	FIXC_ATTR_APPL_BEG_SEQ_NUM,
	FIXC_ATTR_APPL_END_SEQ,
	FIXC_ATTR_APPL_ID,
	FIXC_ATTR_APPL_LAST_SEQ_NUM,
	FIXC_ATTR_APPL_NEW_SEQ_NUM,
	FIXC_ATTR_APPL_QU_ACTN,
	FIXC_ATTR_APPL_QU_DEPTH,
	FIXC_ATTR_APPL_QU_MAX,
	FIXC_ATTR_APPL_QU_RESOLUTION,
	FIXC_ATTR_APPL_REQ_ID,
	FIXC_ATTR_APPL_REQ_TYP,
	FIXC_ATTR_APPL_RESEND_FLAG,
	FIXC_ATTR_APPL_RESP_ERR,
	FIXC_ATTR_APPL_RESP_ID,
	FIXC_ATTR_APPL_RESP_TYP,
	FIXC_ATTR_APPL_RPT_ID,
	FIXC_ATTR_APPL_RPT_TYP,
	FIXC_ATTR_APPL_SEQ_NUM,
	FIXC_ATTR_APPL_TOT_MSG_CNT,
	FIXC_ATTR_APPL_TYP,
	FIXC_ATTR_APPL_VER_ID,
	FIXC_ATTR_AS_OF_IND,
	FIXC_ATTR_ASGN_METH,
	FIXC_ATTR_ASGN_REQ_TYP,
	FIXC_ATTR_ASGN_RSN,
	FIXC_ATTR_ASGN_TYP,
	FIXC_ATTR_ATTCH_PNT,
	FIXC_ATTR_AUTO_ACCEPT_IND,
	FIXC_ATTR_AVG_PAR_PX,
	FIXC_ATTR_AVG_PX,
	FIXC_ATTR_AVG_PX_IND,
	FIXC_ATTR_AVG_PX_PRCSN,
	FIXC_ATTR_BASIS,
	FIXC_ATTR_BASIS_FEATURE_DT,
	FIXC_ATTR_BASIS_FEATURE_PX,
	FIXC_ATTR_BASIS_PX_TYP,
	FIXC_ATTR_BID_DESCPTR,
	FIXC_ATTR_BID_DESCPTR_TYP,
	FIXC_ATTR_BID_FWD_PNTS,
	FIXC_ATTR_BID_FWD_PNTS2,
	FIXC_ATTR_BID_ID,
	FIXC_ATTR_BID_PX,
	FIXC_ATTR_BID_REQ_TRANS_TYP,
	FIXC_ATTR_BID_SPOT_RT,
	FIXC_ATTR_BID_SWAP_PNTS,
	FIXC_ATTR_BID_SZ,
	FIXC_ATTR_BID_TRD_TYP,
	FIXC_ATTR_BID_TYP,
	FIXC_ATTR_BID_YLD,
	FIXC_ATTR_BIZ_DT,
	FIXC_ATTR_BIZ_REJ_REF_ID,
	FIXC_ATTR_BIZ_REJ_RSN,
	FIXC_ATTR_BKNG_QTY,
	FIXC_ATTR_BKNG_REF_ID,
	FIXC_ATTR_BKNG_TYP,
	FIXC_ATTR_BKNG_UNIT,
	FIXC_ATTR_BUY_VOL,
	FIXC_ATTR_BUYER,
	FIXC_ATTR_CFI,
	FIXC_ATTR_CP_PGM,
	FIXC_ATTR_CP_REG_T,
	FIXC_ATTR_CP_REG_TYP,
	FIXC_ATTR_CSET_MO,
	FIXC_ATTR_CALC_CCY_LAST_QTY,
	FIXC_ATTR_CALC_DT,
	FIXC_ATTR_CAP_PX,
	FIXC_ATTR_CAP_VALU,
	FIXC_ATTR_CARD_EXP_DT,
	FIXC_ATTR_CARD_HOLDER_NAME,
	FIXC_ATTR_CARD_ISS_NUM,
	FIXC_ATTR_CARD_NUM,
	FIXC_ATTR_CARD_START_DT,
	FIXC_ATTR_CASH,
	FIXC_ATTR_CASH_AMT,
	FIXC_ATTR_CASH_TYP,
	FIXC_ATTR_CCY,
	FIXC_ATTR_CCY_AMT,
	FIXC_ATTR_CL_BID_ID,
	FIXC_ATTR_CLR_FEE_IND,
	FIXC_ATTR_CLRNG_INSTRCTN,
	FIXC_ATTR_CLS_TM,
	FIXC_ATTR_CMULT,
	FIXC_ATTR_CNFM_ID,
	FIXC_ATTR_CNFM_REF_ID,
	FIXC_ATTR_CNFM_REJ_RSN,
	FIXC_ATTR_CNFM_REQ_ID,
	FIXC_ATTR_CNFM_STAT,
	FIXC_ATTR_CNFM_TRANS_TYP,
	FIXC_ATTR_CNFM_TYP,
	FIXC_ATTR_CNTRA_BRKR,
	FIXC_ATTR_COL_AMT,
	FIXC_ATTR_COMM,
	FIXC_ATTR_COMM_TYP,
	FIXC_ATTR_COMPLIANCE_ID,
	FIXC_ATTR_CONCESSION,
	FIXC_ATTR_COND,
	FIXC_ATTR_CONT_AMT_CURR,
	FIXC_ATTR_CONT_AMT_TYP,
	FIXC_ATTR_CONT_AMT_VALU,
	FIXC_ATTR_CONTINGENCY_TYPE,
	FIXC_ATTR_COPY_MSG_IND,
	FIXC_ATTR_CORP_ACTN,
	FIXC_ATTR_COVER,
	FIXC_ATTR_COVERED,
	FIXC_ATTR_CPCTY,
	FIXC_ATTR_CPCTY_QTY,
	FIXC_ATTR_CPN_PMT,
	FIXC_ATTR_CPN_RT,
	FIXC_ATTR_CRD_RTG,
	FIXC_ATTR_CRSS_PCT,
	FIXC_ATTR_CRSS_PRIORTSTN,
	FIXC_ATTR_CRSS_TYP,
	FIXC_ATTR_CSH_DISTRIB_AGENT_ACCT_NAME,
	FIXC_ATTR_CSH_DISTRIB_AGENT_ACCT_NUM,
	FIXC_ATTR_CSH_DISTRIB_AGENT_CODE,
	FIXC_ATTR_CSH_DISTRIB_AGENT_NAME,
	FIXC_ATTR_CSH_DISTRIB_CURR,
	FIXC_ATTR_CSH_DISTRIB_PAY_REF,
	FIXC_ATTR_CSH_MGN,
	FIXC_ATTR_CSH_OUTSTANDING,
	FIXC_ATTR_CTRY,
	FIXC_ATTR_CUM_QTY,
	FIXC_ATTR_CUR_VAL,
	FIXC_ATTR_CURR,
	FIXC_ATTR_CURRENCY_RATIO,
	FIXC_ATTR_CUST_CPCTY,
	FIXC_ATTR_CUST_DRCTD_ORD,
	FIXC_ATTR_CUST_ORD_HDL_INST,
	FIXC_ATTR_CXL_QTY,
	FIXC_ATTR_CXL_REJ_RSN,
	FIXC_ATTR_CXL_REJ_RSP_TO,
	FIXC_ATTR_CXL_RPLC_RSN,
	FIXC_ATTR_CXL_TYP,
	FIXC_ATTR_CXLLATION_RIGHTS,
	FIXC_ATTR_CYCLE_NO,
	FIXC_ATTR_D2_ID,
	FIXC_ATTR_D2_LOC,
	FIXC_ATTR_D2_SUB,
	FIXC_ATTR_DATED,
	FIXC_ATTR_DAY_AVG_PX,
	FIXC_ATTR_DAY_BKNG_INST,
	FIXC_ATTR_DAY_CUM_QTY,
	FIXC_ATTR_DAY_ORD_QTY,
	FIXC_ATTR_DEALING_CPCTY,
	FIXC_ATTR_DEF_APPL_VER_ID,
	FIXC_ATTR_DEF_BID_SZ,
	FIXC_ATTR_DEF_OFR_SZ,
	FIXC_ATTR_DEL_RSN,
	FIXC_ATTR_DESC,
	FIXC_ATTR_DESIGNATION,
	FIXC_ATTR_DESK_ID,
	FIXC_ATTR_DETCH_PNT,
	FIXC_ATTR_DFLT_APPL_EXT_ID,
	FIXC_ATTR_DFLT_VER_IND,
	FIXC_ATTR_DIRT_PX,
	FIXC_ATTR_DISPLAY_HIGH_QTY,
	FIXC_ATTR_DISPLAY_QTY,
	FIXC_ATTR_DISTRIB_PCTAGE,
	FIXC_ATTR_DISTRIB_PMT_METHOD,
	FIXC_ATTR_DIVIDEND_YIELD,
	FIXC_ATTR_DK_RSN,
	FIXC_ATTR_DLV_DT,
	FIXC_ATTR_DLVRY_FORM,
	FIXC_ATTR_DLVRY_TYP,
	FIXC_ATTR_DSCTN_INST,
	FIXC_ATTR_DSCTN_PX,
	FIXC_ATTR_DSK_ORD_HNDL_INST,
	FIXC_ATTR_DSK_TYP,
	FIXC_ATTR_DSK_TYP_SRC,
	FIXC_ATTR_DSP_MIN_INCR,
	FIXC_ATTR_DSP_MTHD,
	FIXC_ATTR_DSP_WHN,
	FIXC_ATTR_DSPL_LW_QTY,
	FIXC_ATTR_DT,
	FIXC_ATTR_DT_OF_BIRTH,
	FIXC_ATTR_DTLS,
	FIXC_ATTR_DUE_TO_RELTD,
	FIXC_ATTR_EFP,
	FIXC_ATTR_EFP_TRKNG_ERR,
	FIXC_ATTR_EFCTV_TM,
	FIXC_ATTR_EMAIL,
	FIXC_ATTR_EMAIL_THREAD_ID,
	FIXC_ATTR_EMAIL_TYP,
	FIXC_ATTR_ENC_ALLOC_TEXT,
	FIXC_ATTR_ENC_ALLOC_TEXT_LEN,
	FIXC_ATTR_ENC_HEADLINE,
	FIXC_ATTR_ENC_HEADLINE_LEN,
	FIXC_ATTR_ENC_ISSR,
	FIXC_ATTR_ENC_ISSR_LEN,
	FIXC_ATTR_ENC_LEG_ISSR,
	FIXC_ATTR_ENC_LEG_ISSR_LEN,
	FIXC_ATTR_ENC_LEG_SEC_DESC,
	FIXC_ATTR_ENC_LEG_SEC_DESC_LEN,
	FIXC_ATTR_ENC_LIST_EXEC_INST,
	FIXC_ATTR_ENC_LIST_EXEC_INST_LEN,
	FIXC_ATTR_ENC_LIST_STAT_TEXT,
	FIXC_ATTR_ENC_LIST_STAT_TEXT_LEN,
	FIXC_ATTR_ENC_NEW_PWD,
	FIXC_ATTR_ENC_PWD,
	FIXC_ATTR_ENC_PWD_METHOD,
	FIXC_ATTR_ENC_SEC_DESC,
	FIXC_ATTR_ENC_SEC_DESC_LEN,
	FIXC_ATTR_ENC_SUBJECT,
	FIXC_ATTR_ENC_SUBJECT_LEN,
	FIXC_ATTR_ENC_TXT,
	FIXC_ATTR_ENC_TXT_LEN,
	FIXC_ATTR_ENC_UND_ISSR,
	FIXC_ATTR_ENC_UND_ISSR_LEN,
	FIXC_ATTR_ENC_UND_SEC_DESC,
	FIXC_ATTR_ENC_UND_SEC_DESC_LEN,
	FIXC_ATTR_ENCODED_MKT_SEGM_DESC,
	FIXC_ATTR_ENCODED_MKT_SEGM_DESC_LEN,
	FIXC_ATTR_END_ACRD_INT_AMT,
	FIXC_ATTR_END_CSH,
	FIXC_ATTR_END_DT,
	FIXC_ATTR_END_MMY,
	FIXC_ATTR_END_PX,
	FIXC_ATTR_END_STRK_PX_RNG,
	FIXC_ATTR_END_TICK_PX_RNG,
	FIXC_ATTR_END_TM,
	FIXC_ATTR_END_VAL,
	FIXC_ATTR_ENTRY_ID,
	FIXC_ATTR_ENTRY_REJ_RSN,
	FIXC_ATTR_EVENT_TYP,
	FIXC_ATTR_EX_DEST,
	FIXC_ATTR_EX_DEST_ID_SRC,
	FIXC_ATTR_EX_DT,
	FIXC_ATTR_EXCH,
	FIXC_ATTR_EXCH_RULE,
	FIXC_ATTR_EXCH_SPECL_INSTR,
	FIXC_ATTR_EXEC_ACK_STAT,
	FIXC_ATTR_EXEC_ID,
	FIXC_ATTR_EXEC_ID2,
	FIXC_ATTR_EXEC_INST,
	FIXC_ATTR_EXEC_INST_VALU,
	FIXC_ATTR_EXEC_PX_ADJMENT,
	FIXC_ATTR_EXEC_PX_TYP,
	FIXC_ATTR_EXEC_REF_ID,
	FIXC_ATTR_EXEC_RSTMT_RSN,
	FIXC_ATTR_EXEC_TYP,
	FIXC_ATTR_EXEC_VALUATION_POINT,
	FIXC_ATTR_EXER_STYLE,
	FIXC_ATTR_EXP_QTY,
	FIXC_ATTR_EXP_TYP,
	FIXC_ATTR_EXP_TYPE,
	FIXC_ATTR_EXPIRATION_CYCLE,
	FIXC_ATTR_EXPIRE_DT,
	FIXC_ATTR_EXPIRE_TM,
	FIXC_ATTR_EXR_METHOD,
	FIXC_ATTR_FX_RATE,
	FIXC_ATTR_FX_RATE_CALC,
	FIXC_ATTR_FAIR_VALU,
	FIXC_ATTR_FCTR,
	FIXC_ATTR_FEE_MULT,
	FIXC_ATTR_FILL_EXEC_ID,
	FIXC_ATTR_FILL_PX,
	FIXC_ATTR_FILL_QTY,
	FIXC_ATTR_FILL_STATION_CD,
	FIXC_ATTR_FINCL_STAT,
	FIXC_ATTR_FIRM_TRD_ID,
	FIXC_ATTR_FIRM_TRD_ID2,
	FIXC_ATTR_FIRST_PX,
	FIXC_ATTR_FLEX_IND,
	FIXC_ATTR_FLEX_PROD_ELIG,
	FIXC_ATTR_FLOW_SCHED_TYP,
	FIXC_ATTR_FLR_PX,
	FIXC_ATTR_FOREX_REQ,
	FIXC_ATTR_FUND_RENEW_WAIV,
	FIXC_ATTR_FUT_VAL_METH,
	FIXC_ATTR_FX_RATE,
	FIXC_ATTR_FX_RATE_CALC,
	FIXC_ATTR_GT_BKNG_INST,
	FIXC_ATTR_GROSS_TRD_AMT,
	FIXC_ATTR_HALT_RSN,
	FIXC_ATTR_HANDL_INST,
	FIXC_ATTR_HEADLINE,
	FIXC_ATTR_HI_LMT_PX,
	FIXC_ATTR_HIGH_LMT_PX,
	FIXC_ATTR_HIGH_PX,
	FIXC_ATTR_HND_INST,
	FIXC_ATTR_HST_CX_ID,
	FIXC_ATTR_ID,
	FIXC_ATTR_ID2,
	FIXC_ATTR_IOIID,
	FIXC_ATTR_IOI_QTY,
	FIXC_ATTR_IM_REQ_TYP,
	FIXC_ATTR_IMPLCT_DEL,
	FIXC_ATTR_IMPLD_MKT_IND,
	FIXC_ATTR_IN_VIEW_OF_CMN,
	FIXC_ATTR_INC_TAX_IND,
	FIXC_ATTR_IND_ALLOC_ID,
	FIXC_ATTR_IND_ALLOC_ID2,
	FIXC_ATTR_IND_ALLOC_REJ_CODE,
	FIXC_ATTR_INPT_DEV,
	FIXC_ATTR_INPT_SRC,
	FIXC_ATTR_INST_SRC,
	FIXC_ATTR_INST_TYP,
	FIXC_ATTR_INSTRCTN_IND,
	FIXC_ATTR_INT_ACRL,
	FIXC_ATTR_INT_AT_MAT,
	FIXC_ATTR_INVESTOR_CTRY_OF_RESIDENCE,
	FIXC_ATTR_ISS_DT,
	FIXC_ATTR_ISSR,
	FIXC_ATTR_ISSU_CTRY,
	FIXC_ATTR_ISSUED,
	FIXC_ATTR_LANG_CD,
	FIXC_ATTR_LAST_CPCTY,
	FIXC_ATTR_LAST_FRAGMENT,
	FIXC_ATTR_LAST_FWD_PNTS,
	FIXC_ATTR_LAST_FWD_PNTS2,
	FIXC_ATTR_LAST_LQDTY_IND,
	FIXC_ATTR_LAST_MKT,
	FIXC_ATTR_LAST_NTWK_RSP_ID,
	FIXC_ATTR_LAST_PAR_PX,
	FIXC_ATTR_LAST_PX,
	FIXC_ATTR_LAST_QTY,
	FIXC_ATTR_LAST_RPT_REQED,
	FIXC_ATTR_LAST_SPOT_RT,
	FIXC_ATTR_LAST_SWAP_PNTS,
	FIXC_ATTR_LAST_UPDATE_TM,
	FIXC_ATTR_LATE_IND,
	FIXC_ATTR_LCL,
	FIXC_ATTR_LCTN_ID,
	FIXC_ATTR_LEAVES_QTY,
	FIXC_ATTR_LEG_ALLOC_ID,
	FIXC_ATTR_LEG_BID_FWD_PNTS,
	FIXC_ATTR_LEG_CALC_CCY_LAST_QTY,
	FIXC_ATTR_LEG_CURRENCY_RATIO,
	FIXC_ATTR_LEG_DIVIDEND_YIELD,
	FIXC_ATTR_LEG_EXEC_INST,
	FIXC_ATTR_LEG_GROSS_TRD_AMT,
	FIXC_ATTR_LEG_LAST_FWD_PNTS,
	FIXC_ATTR_LEG_NO,
	FIXC_ATTR_LEG_OFR_FWD_PNTS,
	FIXC_ATTR_LEG_OPTION_RATIO,
	FIXC_ATTR_LEG_REF_ID,
	FIXC_ATTR_LEG_VOLATILITY,
	FIXC_ATTR_LEGAL_CNFM,
	FIXC_ATTR_LIMIT_TYP,
	FIXC_ATTR_LINK_ID,
	FIXC_ATTR_LINK_TYP,
	FIXC_ATTR_LIST_DESC,
	FIXC_ATTR_LIST_EXEC_INST,
	FIXC_ATTR_LIST_EXEC_INST_TYP,
	FIXC_ATTR_LIST_ID,
	FIXC_ATTR_LIST_METH,
	FIXC_ATTR_LIST_NAME,
	FIXC_ATTR_LIST_ORD_STAT,
	FIXC_ATTR_LIST_REF_ID,
	FIXC_ATTR_LIST_REJECT_REASON,
	FIXC_ATTR_LIST_REQ_TYP,
	FIXC_ATTR_LIST_SEQ_NO,
	FIXC_ATTR_LIST_STAT_TEXT,
	FIXC_ATTR_LIST_STAT_TYP,
	FIXC_ATTR_LIST_TYP,
	FIXC_ATTR_LIST_UPD_ACTN,
	FIXC_ATTR_LMT_TYP,
	FIXC_ATTR_LNK_ID,
	FIXC_ATTR_LOC_REQD,
	FIXC_ATTR_LONG,
	FIXC_ATTR_LOT_TYP,
	FIXC_ATTR_LOW_LMT_PX,
	FIXC_ATTR_LOW_PX,
	FIXC_ATTR_LQDTY_IND,
	FIXC_ATTR_LQDTY_IND_TYP,
	FIXC_ATTR_LQDTY_NUM_SECURITIES,
	FIXC_ATTR_LQDTY_PCT_HIGH,
	FIXC_ATTR_LQDTY_PCT_LOW,
	FIXC_ATTR_LQDTY_VALU,
	FIXC_ATTR_LST_TYP_SRC,
	FIXC_ATTR_LT_TYP,
	FIXC_ATTR_MD_BK_TYP,
	FIXC_ATTR_MD_ENTRY_FWD_PNTS,
	FIXC_ATTR_MD_ENTRY_SPOT_RT,
	FIXC_ATTR_MD_FEED_TYP,
	FIXC_ATTR_MD_ORIG_TYP,
	FIXC_ATTR_MD_PX_LVL,
	FIXC_ATTR_MD_QTE_TYP,
	FIXC_ATTR_MD_SEC_SIZE,
	FIXC_ATTR_MD_SEC_SIZE_TYPE,
	FIXC_ATTR_MD_STRM_ID,
	FIXC_ATTR_MD_SUB_BK_TYP,
	FIXC_ATTR_MLEG_RPT_TYP,
	FIXC_ATTR_MLEG_RPT_TYP_REQ,
	FIXC_ATTR_MLEG_RPT_TYP,
	FIXC_ATTR_MMY,
	FIXC_ATTR_MMY_FMT,
	FIXC_ATTR_MMY_INCR,
	FIXC_ATTR_MMY_INCR_UNITS,
	FIXC_ATTR_MAILING_DTLS,
	FIXC_ATTR_MAILING_INST,
	FIXC_ATTR_MAN_ORD_IND,
	FIXC_ATTR_MARKET_SEGMENT_DESC,
	FIXC_ATTR_MASS_ACTION_REJECT_REASON,
	FIXC_ATTR_MASS_ACTION_REPORT_ID,
	FIXC_ATTR_MASS_ACTION_RESPONSE,
	FIXC_ATTR_MASS_ACTION_SCOPE,
	FIXC_ATTR_MASS_ACTION_TYPE,
	FIXC_ATTR_MASS_CXL_REJ_RSN,
	FIXC_ATTR_MASS_CXL_RSP,
	FIXC_ATTR_MAT,
	FIXC_ATTR_MAT_DT,
	FIXC_ATTR_MAT_NET_MNY,
	FIXC_ATTR_MAT_RULE_ID,
	FIXC_ATTR_MAT_TM,
	FIXC_ATTR_MAX_FLOOR,
	FIXC_ATTR_MAX_SHOW,
	FIXC_ATTR_MAX_TRD_VOL,
	FIXC_ATTR_METH,
	FIXC_ATTR_METHOD,
	FIXC_ATTR_MGN_EXCESS,
	FIXC_ATTR_MGN_RATIO,
	FIXC_ATTR_MID_PX,
	FIXC_ATTR_MID_YLD,
	FIXC_ATTR_MIN_BID_SZ,
	FIXC_ATTR_MIN_LOT_SZ,
	FIXC_ATTR_MIN_OFR_SZ,
	FIXC_ATTR_MIN_PX_INCR,
	FIXC_ATTR_MIN_PX_INCR_AMT,
	FIXC_ATTR_MIN_QTY,
	FIXC_ATTR_MIN_TRD_VOL,
	FIXC_ATTR_MKT,
	FIXC_ATTR_MKT_BID_PX,
	FIXC_ATTR_MKT_DEPTH,
	FIXC_ATTR_MKT_ID,
	FIXC_ATTR_MKT_OFR_PX,
	FIXC_ATTR_MKT_REQ_ID,
	FIXC_ATTR_MKT_RPT_ID,
	FIXC_ATTR_MKT_SEG_ID,
	FIXC_ATTR_MKT_UPDT_ACTN,
	FIXC_ATTR_MLEG_MODEL,
	FIXC_ATTR_MLEG_PX_METH,
	FIXC_ATTR_MNY_LAUNDERING_STAT,
	FIXC_ATTR_MODE,
	FIXC_ATTR_MODEL_TYP,
	FIXC_ATTR_MOVE_TYP,
	FIXC_ATTR_MSG_ENCD,
	FIXC_ATTR_MSG_EVT_SRC,
	FIXC_ATTR_MSG_TYP,
	FIXC_ATTR_MTCH_ALGO,
	FIXC_ATTR_MTCH_ID,
	FIXC_ATTR_MTCH_INC,
	FIXC_ATTR_MTCH_STAT,
	FIXC_ATTR_MTCH_TYP,
	FIXC_ATTR_MULT,
	FIXC_ATTR_MULT_TYP,
	FIXC_ATTR_MX_PX_LVLS,
	FIXC_ATTR_MX_PX_VAR,
	FIXC_ATTR_NT_POS_LMT,
	FIXC_ATTR_NAME,
	FIXC_ATTR_NAT_FLAG,
	FIXC_ATTR_NET_CHG_PREV_DAY,
	FIXC_ATTR_NET_GROSS_IND,
	FIXC_ATTR_NET_MNY,
	FIXC_ATTR_NEW_PASSWORD,
	FIXC_ATTR_NEWS_CATGY,
	FIXC_ATTR_NO_AFFCTD_ORDS,
	FIXC_ATTR_NO_ALLOCS,
	FIXC_ATTR_NO_ALT_MD_SRC,
	FIXC_ATTR_NO_BID_COMPONENTS,
	FIXC_ATTR_NO_BID_DESCPTRS,
	FIXC_ATTR_NO_CAPACITIES,
	FIXC_ATTR_NO_CLRNG_INSTRCTNS,
	FIXC_ATTR_NO_CNTRA_BRKRS,
	FIXC_ATTR_NO_COLL_INQ_QUAL,
	FIXC_ATTR_NO_COMP_IDS,
	FIXC_ATTR_NO_CONT_AMTS,
	FIXC_ATTR_NO_DISTRIB_INSTS,
	FIXC_ATTR_NO_DLVY_INST,
	FIXC_ATTR_NO_DTS,
	FIXC_ATTR_NO_EVENTS,
	FIXC_ATTR_NO_EXECS,
	FIXC_ATTR_NO_HOPS,
	FIXC_ATTR_NO_IOI_QUALS,
	FIXC_ATTR_NO_INSTR_ATTRIB,
	FIXC_ATTR_NO_LEG_ALLOCS,
	FIXC_ATTR_NO_LEG_SEC_ALT_ID,
	FIXC_ATTR_NO_LEG_STIPS,
	FIXC_ATTR_NO_LEGS,
	FIXC_ATTR_NO_LINES_OF_TEXT,
	FIXC_ATTR_NO_MD_ENTRIES,
	FIXC_ATTR_NO_MD_ENTRY_TYPS,
	FIXC_ATTR_NO_MISC_FEES,
	FIXC_ATTR_NO_NESTED4_PARTY_IDS,
	FIXC_ATTR_NO_NESTED4_PARTY_SUB_IDS,
	FIXC_ATTR_NO_NST2_PTY_IDS,
	FIXC_ATTR_NO_NST2_PTY_SUB_IDS,
	FIXC_ATTR_NO_NST3_PTY_IDS,
	FIXC_ATTR_NO_NST3_PTY_SUB_IDS,
	FIXC_ATTR_NO_NST_PTY_IDS,
	FIXC_ATTR_NO_NST_PTY_SUB_IDS,
	FIXC_ATTR_NO_ORDS,
	FIXC_ATTR_NO_ORDS_TYP,
	FIXC_ATTR_NO_POS_AMT,
	FIXC_ATTR_NO_POSS,
	FIXC_ATTR_NO_PTY_IDS,
	FIXC_ATTR_NO_PTY_SUB_IDS,
	FIXC_ATTR_NO_QUOT_ENTRIES,
	FIXC_ATTR_NO_QUOT_QUALS,
	FIXC_ATTR_NO_QUOT_SETS,
	FIXC_ATTR_NO_REGIST_DTLS,
	FIXC_ATTR_NO_RELTD_SYM,
	FIXC_ATTR_NO_RPTS,
	FIXC_ATTR_NO_RTG_IDS,
	FIXC_ATTR_NO_SEC_ALT_ID,
	FIXC_ATTR_NO_SEC_TYPS,
	FIXC_ATTR_NO_SETTL_INST,
	FIXC_ATTR_NO_SETTL_PTY_IDS,
	FIXC_ATTR_NO_SETTL_PTY_SUB_IDS,
	FIXC_ATTR_NO_SIDES,
	FIXC_ATTR_NO_STIPS,
	FIXC_ATTR_NO_STRKS,
	FIXC_ATTR_NO_TRD_REG_TMSTAMPS,
	FIXC_ATTR_NO_TRDG_SESSS,
	FIXC_ATTR_NO_TRDS,
	FIXC_ATTR_NO_UND_SEC_ALT_ID,
	FIXC_ATTR_NO_UND_STIPS,
	FIXC_ATTR_NO_UNDS,
	FIXC_ATTR_NOT_AFF_ORIG_CL_ORD_ID,
	FIXC_ATTR_NOT_AFFECTED_ORDER_ID,
	FIXC_ATTR_NOTIFY_BRKR_OF_CREDIT,
	FIXC_ATTR_NOTL_PCT_OUT,
	FIXC_ATTR_NTWK_REQ_ID,
	FIXC_ATTR_NTWK_REQ_TYP,
	FIXC_ATTR_NTWK_RSP_ID,
	FIXC_ATTR_NTWK_STAT_RSP_TYP,
	FIXC_ATTR_NUM_BIDDERS,
	FIXC_ATTR_NUM_DAYS_INT,
	FIXC_ATTR_NUM_OF_ORDS,
	FIXC_ATTR_NUM_TKTS,
	FIXC_ATTR_OBID,
	FIXC_ATTR_OB_LOC,
	FIXC_ATTR_OB_SUB,
	FIXC_ATTR_ODD_LOT,
	FIXC_ATTR_OFR_FWD_PNTS,
	FIXC_ATTR_OFR_FWD_PNTS2,
	FIXC_ATTR_OFR_PX,
	FIXC_ATTR_OFR_SPOT_RT,
	FIXC_ATTR_OFR_SWAP_PNTS,
	FIXC_ATTR_OFR_SZ,
	FIXC_ATTR_OFR_YLD,
	FIXC_ATTR_OFST_TYP,
	FIXC_ATTR_OFST_VAL,
	FIXC_ATTR_OFST_VALU,
	FIXC_ATTR_OPEN_CLOSE_SETTL_FLAG,
	FIXC_ATTR_OPEN_CLS_SETTL_FLAG,
	FIXC_ATTR_OPEN_INT,
	FIXC_ATTR_OPEN_TM,
	FIXC_ATTR_OPT_A,
	FIXC_ATTR_OPT_AT,
	FIXC_ATTR_OPT_PAY_AMT,
	FIXC_ATTR_OPT_PAYOUT_TYP,
	FIXC_ATTR_ORD_CAT,
	FIXC_ATTR_ORD_DELAY,
	FIXC_ATTR_ORD_DELAY_UNIT,
	FIXC_ATTR_ORD_HNDL_INST_SRC,
	FIXC_ATTR_ORD_ID,
	FIXC_ATTR_ORD_ID2,
	FIXC_ATTR_ORD_INPT_DEV,
	FIXC_ATTR_ORD_QTY,
	FIXC_ATTR_ORIG,
	FIXC_ATTR_ORIG_CRSS_ID,
	FIXC_ATTR_ORIG_CUST_ORD_CPCTY,
	FIXC_ATTR_ORIG_ID,
	FIXC_ATTR_ORIG_NOTL_PCT_OUT,
	FIXC_ATTR_ORIG_ORD_MOD_TM,
	FIXC_ATTR_ORIG_REQ_REF_ID,
	FIXC_ATTR_ORIG_SNT,
	FIXC_ATTR_ORIG_TM,
	FIXC_ATTR_ORIG_TRD_DT,
	FIXC_ATTR_ORIG_TRD_HANDL_INST,
	FIXC_ATTR_ORIG_TRD_ID,
	FIXC_ATTR_ORIGN_DT,
	FIXC_ATTR_ORIGN_TRD_ID2,
	FIXC_ATTR_OUT_MAIN_CNTRY_UNDX,
	FIXC_ATTR_OUTSIDE_NDX_PCT,
	FIXC_ATTR_OWNER_TYP,
	FIXC_ATTR_OWNERSHIP_TYP,
	FIXC_ATTR_PARENT_MKT_SEGM_ID,
	FIXC_ATTR_PARTICIPATION_RT,
	FIXC_ATTR_PASSWORD,
	FIXC_ATTR_PAY_AMT,
	FIXC_ATTR_PCT,
	FIXC_ATTR_PCT_AT_RISK,
	FIXC_ATTR_PEG_PX_TYP,
	FIXC_ATTR_PEG_SEC_DESC,
	FIXC_ATTR_PEG_SEC_ID,
	FIXC_ATTR_PEG_SECURITY_ID_SOURCE,
	FIXC_ATTR_PEGGED_PX,
	FIXC_ATTR_PG_SYMBL,
	FIXC_ATTR_PGGD_REF_PX,
	FIXC_ATTR_PMT_DT,
	FIXC_ATTR_PMT_METHOD,
	FIXC_ATTR_PMT_REF,
	FIXC_ATTR_PMT_REMTR_ID,
	FIXC_ATTR_POINT,
	FIXC_ATTR_POOL,
	FIXC_ATTR_POS_DUP,
	FIXC_ATTR_POS_EFCT,
	FIXC_ATTR_POS_LMT,
	FIXC_ATTR_POS_NO,
	FIXC_ATTR_POS_RSND,
	FIXC_ATTR_PR_TRD_ANON,
	FIXC_ATTR_PRE_CLS_TM,
	FIXC_ATTR_PREALLOC_METH,
	FIXC_ATTR_PREV_CLS_PX,
	FIXC_ATTR_PREVLY_RPTED,
	FIXC_ATTR_PRI_IND,
	FIXC_ATTR_PRI_SET_PX,
	FIXC_ATTR_PRIOR_SPREAD_IND,
	FIXC_ATTR_PRIORTY,
	FIXC_ATTR_PROC_CODE,
	FIXC_ATTR_PROD,
	FIXC_ATTR_PROD_CMPLX,
	FIXC_ATTR_PROG_PERIOD_INTVL,
	FIXC_ATTR_PROG_RPT_REQS,
	FIXC_ATTR_PRVT_QT,
	FIXC_ATTR_PTY_ROLE,
	FIXC_ATTR_PUB_TRD_IND,
	FIXC_ATTR_PUT_CALL,
	FIXC_ATTR_PX,
	FIXC_ATTR_PX2,
	FIXC_ATTR_PX_BNDRY_METH,
	FIXC_ATTR_PX_BNDRY_PRCSN,
	FIXC_ATTR_PX_DELTA,
	FIXC_ATTR_PX_DTRMN_METH,
	FIXC_ATTR_PX_IMPRVMNT,
	FIXC_ATTR_PX_LMT_TYP,
	FIXC_ATTR_PX_PRT_SCP,
	FIXC_ATTR_PX_QTE_METH,
	FIXC_ATTR_PX_TM_TYP,
	FIXC_ATTR_PX_TYP,
	FIXC_ATTR_PX_UOM,
	FIXC_ATTR_PX_UOM_QTY,
	FIXC_ATTR_QCOND,
	FIXC_ATTR_QID,
	FIXC_ATTR_QLTY_IND,
	FIXC_ATTR_QT_ENT_STS,
	FIXC_ATTR_QT_MSG_ID,
	FIXC_ATTR_QTY,
	FIXC_ATTR_QTY2,
	FIXC_ATTR_QTY_DT,
	FIXC_ATTR_QTY_TYP,
	FIXC_ATTR_QUAL,
	FIXC_ATTR_QUOT_PX_TYP,
	FIXC_ATTR_R,
	FIXC_ATTR_RFQ_REQ_ID,
	FIXC_ATTR_RFR,
	FIXC_ATTR_RATIO_QTY,
	FIXC_ATTR_RAW_DATA,
	FIXC_ATTR_RAW_DATA_LENGTH,
	FIXC_ATTR_RCVD_DPT_ID,
	FIXC_ATTR_RED_DT,
	FIXC_ATTR_RED_PX,
	FIXC_ATTR_RED_PX_TYP,
	FIXC_ATTR_REDEEM,
	FIXC_ATTR_REF,
	FIXC_ATTR_REF_APPL_EXT_ID,
	FIXC_ATTR_REF_APPL_ID,
	FIXC_ATTR_REF_APPL_LAST_SEQ_NUM,
	FIXC_ATTR_REF_APPL_VER_ID,
	FIXC_ATTR_REF_COMP_ID,
	FIXC_ATTR_REF_CSTM_APPL_VER_ID,
	FIXC_ATTR_REF_ID,
	FIXC_ATTR_REF_IND,
	FIXC_ATTR_REF_MSG_TYP,
	FIXC_ATTR_REF_ORD_ID,
	FIXC_ATTR_REF_ORD_ID_RSN,
	FIXC_ATTR_REF_ORD_ID_SRC,
	FIXC_ATTR_REF_PG,
	FIXC_ATTR_REF_SEQ_NUM,
	FIXC_ATTR_REF_SUB_ID,
	FIXC_ATTR_REF_TAG_ID,
	FIXC_ATTR_REF_TYP,
	FIXC_ATTR_REG_STAT,
	FIXC_ATTR_REJ_CODE,
	FIXC_ATTR_REJ_RSN,
	FIXC_ATTR_REJ_RSN_CD,
	FIXC_ATTR_REJ_RSN_TXT,
	FIXC_ATTR_REJ_TXT,
	FIXC_ATTR_REPO_COLL_SEC_TYP,
	FIXC_ATTR_REPO_RT,
	FIXC_ATTR_REPO_TRM,
	FIXC_ATTR_REPORTED_PX_DIFF,
	FIXC_ATTR_REQ_ID,
	FIXC_ATTR_REQ_REJ_RESN,
	FIXC_ATTR_REQ_REJ_RSN,
	FIXC_ATTR_REQ_RSLT,
	FIXC_ATTR_REQ_STAT,
	FIXC_ATTR_REQ_TYP,
	FIXC_ATTR_RESP_ID,
	FIXC_ATTR_RESP_TYP,
	FIXC_ATTR_RESTRCT_TYP,
	FIXC_ATTR_REVERSAL_IND,
	FIXC_ATTR_RFRSH_QTY,
	FIXC_ATTR_RGSTRY,
	FIXC_ATTR_RND_DIR,
	FIXC_ATTR_RND_LOT,
	FIXC_ATTR_RND_MOD,
	FIXC_ATTR_RND_PX,
	FIXC_ATTR_RPT_ID,
	FIXC_ATTR_RPT_ID2,
	FIXC_ATTR_RPT_REF_ID,
	FIXC_ATTR_RPT_REJ_RSN,
	FIXC_ATTR_RPT_SEQ,
	FIXC_ATTR_RPT_SYS,
	FIXC_ATTR_RPT_TO_EXCH,
	FIXC_ATTR_RPT_TYP,
	FIXC_ATTR_RPTED_PX,
	FIXC_ATTR_RSLT,
	FIXC_ATTR_RSN_CD,
	FIXC_ATTR_RSP,
	FIXC_ATTR_RSP_DEST,
	FIXC_ATTR_RSP_ID,
	FIXC_ATTR_RSP_LVL,
	FIXC_ATTR_RSP_TRANSPORT_TYP,
	FIXC_ATTR_RSP_TYP,
	FIXC_ATTR_RSPDNT_TYP,
	FIXC_ATTR_RSTCTIONS,
	FIXC_ATTR_RT_PRTY_ID,
	FIXC_ATTR_RT_PTY_ID_SRC,
	FIXC_ATTR_RT_PTY_RL,
	FIXC_ATTR_RT_PTY_SUB_ID,
	FIXC_ATTR_RT_PTY_SUB_ID_TYP,
	FIXC_ATTR_RT_SRC,
	FIXC_ATTR_RT_SRC_TYP,
	FIXC_ATTR_RTG_ID,
	FIXC_ATTR_RTG_TYP,
	FIXC_ATTR_SID,
	FIXC_ATTR_SLOC,
	FIXC_ATTR_SSUB,
	FIXC_ATTR_SCHEMA,
	FIXC_ATTR_SCOPE,
	FIXC_ATTR_SEC_ALT_ID,
	FIXC_ATTR_SEC_ALT_ID_SRC,
	FIXC_ATTR_SEC_DSP_QTY,
	FIXC_ATTR_SEC_GRP,
	FIXC_ATTR_SEC_ID,
	FIXC_ATTR_SEC_ID_SRC,
	FIXC_ATTR_SEC_SUB_TYP,
	FIXC_ATTR_SEC_TRD_EVNT,
	FIXC_ATTR_SEC_TYP,
	FIXC_ATTR_SEC_TYPE,
	FIXC_ATTR_SELL_VOL,
	FIXC_ATTR_SELLER,
	FIXC_ATTR_SELLER_DAYS,
	FIXC_ATTR_SEQ_NO,
	FIXC_ATTR_SEQ_NUM,
	FIXC_ATTR_SES_ID,
	FIXC_ATTR_SES_SUB,
	FIXC_ATTR_SESS_STAT,
	FIXC_ATTR_SET_ID,
	FIXC_ATTR_SET_METH,
	FIXC_ATTR_SET_PX,
	FIXC_ATTR_SET_PX_TYP,
	FIXC_ATTR_SET_SES_ID,
	FIXC_ATTR_SET_SES_SUB,
	FIXC_ATTR_SET_STAT,
	FIXC_ATTR_SET_SUB_ID,
	FIXC_ATTR_SETTL_CCY,
	FIXC_ATTR_SETTL_CURR_AMT,
	FIXC_ATTR_SETTL_CURR_BID_FX_RT,
	FIXC_ATTR_SETTL_CURR_FX_RT,
	FIXC_ATTR_SETTL_CURR_FX_RT_CALC,
	FIXC_ATTR_SETTL_CURR_OFR_FX_RT,
	FIXC_ATTR_SETTL_DT,
	FIXC_ATTR_SETTL_DT2,
	FIXC_ATTR_SETTL_ID,
	FIXC_ATTR_SETTL_INST_ID,
	FIXC_ATTR_SETTL_INST_MODE,
	FIXC_ATTR_SETTL_INST_MSG_ID,
	FIXC_ATTR_SETTL_INST_REF_ID,
	FIXC_ATTR_SETTL_INST_REQ_ID,
	FIXC_ATTR_SETTL_INST_REQ_REJ_CODE,
	FIXC_ATTR_SETTL_INST_TRANS_TYP,
	FIXC_ATTR_SETTL_INST_TYP,
	FIXC_ATTR_SETTL_METH,
	FIXC_ATTR_SETTL_MODE,
	FIXC_ATTR_SETTL_MSG_ID,
	FIXC_ATTR_SETTL_ON_OPEN_FLAG,
	FIXC_ATTR_SETTL_REF_ID,
	FIXC_ATTR_SETTL_SRC,
	FIXC_ATTR_SETTL_TRANS_TYP,
	FIXC_ATTR_SETTL_TYP,
	FIXC_ATTR_SFX,
	FIXC_ATTR_SHARED_COMM,
	FIXC_ATTR_SHORT,
	FIXC_ATTR_SHRT_SALE_RSN,
	FIXC_ATTR_SIDE,
	FIXC_ATTR_SIDE_COMPLIANCE_ID,
	FIXC_ATTR_SIDE_EXEC_ID,
	FIXC_ATTR_SIDE_GROSS_TRADE_AMT,
	FIXC_ATTR_SIDE_QTY,
	FIXC_ATTR_SIDE_TM_FRC,
	FIXC_ATTR_SIDE_VALU1,
	FIXC_ATTR_SIDE_VALU2,
	FIXC_ATTR_SIDE_VALU_IND,
	FIXC_ATTR_SNRTY,
	FIXC_ATTR_SNT,
	FIXC_ATTR_SOL_FLAG,
	FIXC_ATTR_SPREAD,
	FIXC_ATTR_SRC,
	FIXC_ATTR_ST_OR_PROVNC,
	FIXC_ATTR_ST_PRV,
	FIXC_ATTR_STAND_INST_DB_ID,
	FIXC_ATTR_STAND_INST_DB_NAME,
	FIXC_ATTR_STAND_INST_DB_TYP,
	FIXC_ATTR_START_CSH,
	FIXC_ATTR_START_DT,
	FIXC_ATTR_START_MMY,
	FIXC_ATTR_START_STRK_PX_RNG,
	FIXC_ATTR_START_TICK_PX_RNG,
	FIXC_ATTR_START_TM,
	FIXC_ATTR_START_VAL,
	FIXC_ATTR_STAT,
	FIXC_ATTR_STAT_REJ_RSN,
	FIXC_ATTR_STAT_REQ_ID,
	FIXC_ATTR_STAT_TEXT,
	FIXC_ATTR_STAT_VALU,
	FIXC_ATTR_STATS_TYP,
	FIXC_ATTR_STATUS,
	FIXC_ATTR_STIP_TYP,
	FIXC_ATTR_STIP_VAL,
	FIXC_ATTR_STL_DT,
	FIXC_ATTR_STOP_PX,
	FIXC_ATTR_STRK,
	FIXC_ATTR_STRK_CCY,
	FIXC_ATTR_STRK_EXR_STYLE,
	FIXC_ATTR_STRK_INCR,
	FIXC_ATTR_STRK_MULT,
	FIXC_ATTR_STRK_PX,
	FIXC_ATTR_STRK_PX_BNDRY_METH,
	FIXC_ATTR_STRK_PX_BNDRY_PRCSN,
	FIXC_ATTR_STRK_PX_DTRMN_METH,
	FIXC_ATTR_STRK_RULE,
	FIXC_ATTR_STRK_TM,
	FIXC_ATTR_STRK_VALU,
	FIXC_ATTR_STRT_PRM_NME,
	FIXC_ATTR_STRT_PRM_TYP,
	FIXC_ATTR_STRT_PRM_VAL,
	FIXC_ATTR_SUB_REQ_TYP,
	FIXC_ATTR_SUB_TYP,
	FIXC_ATTR_SUB_TYPE,
	FIXC_ATTR_SUBJECT,
	FIXC_ATTR_SWAP_PNTS,
	FIXC_ATTR_SWAP_TYP,
	FIXC_ATTR_SYM,
	FIXC_ATTR_SZ,
	FIXC_ATTR_TID,
	FIXC_ATTR_TLOC,
	FIXC_ATTR_TS,
	FIXC_ATTR_TSUB,
	FIXC_ATTR_TZ_TRANSACT_TIME,
	FIXC_ATTR_TAX_ADVANTAGE_TYP,
	FIXC_ATTR_TGT_STRATEGY,
	FIXC_ATTR_TGT_STRATEGY_PARAMETERS,
	FIXC_ATTR_TGT_STRATEGY_PERFORMANCE,
	FIXC_ATTR_THRESHOLD_AMT,
	FIXC_ATTR_TICK_DIRCTN,
	FIXC_ATTR_TICK_INCR,
	FIXC_ATTR_TICK_RULE_TYP,
	FIXC_ATTR_TIER_CD,
	FIXC_ATTR_TM,
	FIXC_ATTR_TM_BKT,
	FIXC_ATTR_TM_IN_FORCE,
	FIXC_ATTR_TM_TO_EXP,
	FIXC_ATTR_TM_UNIT,
	FIXC_ATTR_TOT_ACRD_INT_AMT,
	FIXC_ATTR_TOT_AFFCTD_ORDS,
	FIXC_ATTR_TOT_NET_VALU,
	FIXC_ATTR_TOT_NO_ACC_QTS,
	FIXC_ATTR_TOT_NO_ALLOCS,
	FIXC_ATTR_TOT_NO_CXLD_QTS,
	FIXC_ATTR_TOT_NO_FILLS,
	FIXC_ATTR_TOT_NO_ORDS,
	FIXC_ATTR_TOT_NO_QUOT_ENTRIES,
	FIXC_ATTR_TOT_NO_REJ_QTS,
	FIXC_ATTR_TOT_NO_RELTD_SYM,
	FIXC_ATTR_TOT_NO_SEC_TYPS,
	FIXC_ATTR_TOT_NO_STRKS,
	FIXC_ATTR_TOT_NUM_ASGN_RPTS,
	FIXC_ATTR_TOT_NUM_RPTS,
	FIXC_ATTR_TOT_NUM_TRD_RPTS,
	FIXC_ATTR_TOT_RPTS,
	FIXC_ATTR_TOT_TAKEDOWN,
	FIXC_ATTR_TOT_VOL_TRDD,
	FIXC_ATTR_TRAD_SES_EVENT,
	FIXC_ATTR_TRAD_SES_UPDT_ACTN,
	FIXC_ATTR_TRADING_SESSION_DESC,
	FIXC_ATTR_TRANS_BKD_TM,
	FIXC_ATTR_TRANS_TYP,
	FIXC_ATTR_TRD_CCY,
	FIXC_ATTR_TRD_COND,
	FIXC_ATTR_TRD_DT,
	FIXC_ATTR_TRD_HANDL_INST,
	FIXC_ATTR_TRD_ID,
	FIXC_ATTR_TRD_ID2,
	FIXC_ATTR_TRD_LEG_REF_ID,
	FIXC_ATTR_TRD_MTCH_ID,
	FIXC_ATTR_TRD_PUB_IND,
	FIXC_ATTR_TRD_QTY,
	FIXC_ATTR_TRD_REP_IND,
	FIXC_ATTR_TRD_RPT_ID2,
	FIXC_ATTR_TRD_RPT_REF_ID2,
	FIXC_ATTR_TRD_RPT_STAT,
	FIXC_ATTR_TRD_SUB_TYP,
	FIXC_ATTR_TRD_TM,
	FIXC_ATTR_TRD_TYP,
	FIXC_ATTR_TRD_TYP2,
	FIXC_ATTR_TRD_VOL,
	FIXC_ATTR_TRDD_FLAT_SWITCH,
	FIXC_ATTR_TRDG_REF_PX,
	FIXC_ATTR_TRDG_STAT,
	FIXC_ATTR_TRDR,
	FIXC_ATTR_TRGR_ACTN,
	FIXC_ATTR_TRGR_NEW_PX,
	FIXC_ATTR_TRGR_NEW_QTY,
	FIXC_ATTR_TRGR_ORD_TYP,
	FIXC_ATTR_TRGR_PX,
	FIXC_ATTR_TRGR_PX_DIR,
	FIXC_ATTR_TRGR_PX_TYP,
	FIXC_ATTR_TRGR_PX_TYP_SCP,
	FIXC_ATTR_TRGR_SEC_DESC,
	FIXC_ATTR_TRGR_SEC_ID,
	FIXC_ATTR_TRGR_SEC_ID_SRC,
	FIXC_ATTR_TRGR_SYM,
	FIXC_ATTR_TRGR_TRD_SESS_ID,
	FIXC_ATTR_TRGR_TRD_SESS_SUB_ID,
	FIXC_ATTR_TRGR_TYP,
	FIXC_ATTR_TRM_TYP,
	FIXC_ATTR_TRNSFR_RSN,
	FIXC_ATTR_TXN_TM,
	FIXC_ATTR_TXN_TYP,
	FIXC_ATTR_TXT,
	FIXC_ATTR_TYP,
	FIXC_ATTR_UOM,
	FIXC_ATTR_UOM_QTY,
	FIXC_ATTR_URL,
	FIXC_ATTR_UND_LAST_PX,
	FIXC_ATTR_UND_LAST_QTY,
	FIXC_ATTR_UND_SES_ID,
	FIXC_ATTR_UND_SES_SUB,
	FIXC_ATTR_UND_SET_PX,
	FIXC_ATTR_UND_SET_PX_TYP,
	FIXC_ATTR_UNDLY_DLV_AMT,
	FIXC_ATTR_UNIT,
	FIXC_ATTR_UNSOL,
	FIXC_ATTR_UPD_ACTN,
	FIXC_ATTR_UPDT_ACT,
	FIXC_ATTR_UPDT_TYP,
	FIXC_ATTR_URGENCY,
	FIXC_ATTR_USER_REQ_ID,
	FIXC_ATTR_USER_REQ_TYP,
	FIXC_ATTR_USER_STAT,
	FIXC_ATTR_USER_STAT_TEXT,
	FIXC_ATTR_USERNAME,
	FIXC_ATTR_VAL,
	FIXC_ATTR_VAL_METH,
	FIXC_ATTR_VALID_TIL,
	FIXC_ATTR_VALID_UNTIL_TM,
	FIXC_ATTR_VALU_OF_FUTS,
	FIXC_ATTR_VENU_TYP,
	FIXC_ATTR_VOL,
	FIXC_ATTR_WORKING_IND,
	FIXC_ATTR_WT_AVG_LQDTY,
	FIXC_ATTR_YLD,
};

#endif	/* INCLUDED_fixml_to_fixc_prep_c_ */

/* hand-crafted reverse nsuri lookup (FIXC_VER_* -> fixml uri) */

static __attribute__((unused)) const char *__ver_fixmlify(fixc_ver_t vid)
{
	switch (vid) {
	default:
	case FIXC_VER_UNK:
		return "";
	case FIXC_VER_44:
		return "http://www.fixprotocol.org/FIXML-4-4";
	case FIXC_VER_50:
		return "http://www.fixprotocol.org/FIXML-5-0";
	case FIXC_VER_50_SP1:
		return "http://www.fixprotocol.org/FIXML-5-0-SP1";
	case FIXC_VER_50_SP2:
		return "http://www.fixprotocol.org/FIXML-5-0-SP2";
	}
}

static __attribute__((unused)) const char *__ver_fixify(fixc_ver_t vid)
{
	switch (vid) {
	default:
	case FIXC_VER_UNK:
		return "";
	case FIXC_VER_40:
		return "FIX.4.0";
	case FIXC_VER_41:
		return "FIX.4.1";
	case FIXC_VER_42:
		return "FIX.4.2";
	case FIXC_VER_43:
		return "FIX.4.3";
	case FIXC_VER_44:
		return "FIX.4.4";
	case FIXC_VER_50:
		return "FIX.5.0";
	case FIXC_VER_50_SP1:
		return "FIX.5.0.SP1";
	case FIXC_VER_50_SP2:
		return "FIX.5.0.SP2";
	case FIXC_VER_T11:
		return "FIXT.1.1";
	}
}

static __attribute__((unused)) const char *__ver_fixml_v_ify(fixc_ver_t vid)
{
	switch (vid) {
	default:
	case FIXC_VER_UNK:
		return "";
	case FIXC_VER_44:
		return "4.4";
	case FIXC_VER_50:
		return "5.0";
	case FIXC_VER_50_SP1:
		return "5.0 SP1";
	case FIXC_VER_50_SP2:
		return "5.0 SP2";
	}
}

/* fixml-nsuri-rev.c ends here */

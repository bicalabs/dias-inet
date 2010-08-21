#include "StdAfx.h"
#include "SaxHandlers.h"

/////////// Class CSaxContent

ulong CSaxContent::references = 0;

CSaxContent::CSaxContent () { }

CSaxContent::CSaxContent (CTreeCtrl* tree, kscmd* cmdsys, ksmtx_factors* fctsys, ksmtx_order* ordsys) :
	m_pTree (tree), cmds (cmdsys), factors (fctsys), orders (ordsys), cls (-1) { }

CSaxContent::~CSaxContent () { }

long __stdcall CSaxContent::raw_putDocumentLocator (struct ISAXLocator *pLocator) {
	long hr = S_OK;
	return hr;
}

long __stdcall CSaxContent::raw_startDocument () {
	long hr = S_OK;
	m_tviCurr = TVI_ROOT;
	memset (cmds, 0, sizeof(kscmd));
//	MessageBox (NULL, "SAX parsing XML document started", "SAX Parser Interface Wrapper", MB_ICONINFORMATION | MB_OK);
	return hr;
}

long __stdcall CSaxContent::raw_endDocument () {
	long hr = S_OK;
//	MessageBox (NULL, "SAX parsing XML document ended", "SAX Parser Interface Wrapper", MB_ICONINFORMATION | MB_OK);
	return hr;
}

long __stdcall CSaxContent::raw_startPrefixMapping (ushort *wsPrefix, int nPrefix,
						    ushort *wsUri, int nUri) {
	long hr = S_OK;
	return hr;
}
                                                
long __stdcall CSaxContent::raw_endPrefixMapping (ushort *wsPrefix, int nPrefix) {
	long hr = S_OK;
	return hr;
}

long __stdcall CSaxContent::raw_startElement (ushort *wsNsUri, int nNsUri, ushort *wsName, int nName,
					      ushort *wsQName, int nQName, struct ISAXAttributes *pAttr) {
	long hr = S_OK;
	int i, j, len, lenx1, lenx2, arg_cnt, multipl;
	ushort *wname, *wvalue, *wx1, *wx2, *p;

	if (!wcscmp (wsName, L"cmdsys")) cls = 0;
	else if (!wcscmp (wsName, L"subsys")) cls = 1;
	else if (!wcscmp (wsName, L"class")) cls = 2;
	else if (!wcscmp (wsName, L"div")) cls = 3;
	else if (!wcscmp (wsName, L"cmd")) cls = 4;
	else if (!wcscmp (wsName, L"arg")) cls = 5;
	else if (!wcscmp (wsName, L"ret")) cls = 6;
	else if (!wcscmp (wsName, L"enum")) cls = 7;
	else if (!wcscmp (wsName, L"entry")) cls = 8;
	else if (!wcscmp (wsName, L"factors")) cls = 0x100;
	else if (!wcscmp (wsName, L"order")) cls = 0x101;
	else cls = -1;

	if (cls <= 4 && cls >= 0) {
		pAttr->getValueFromName (wsNsUri, nNsUri, L"name", wcslen(L"name"), &wname, &len);
		wcstombs (buff, wname, 0x100);
		buff[len] = 0;
		m_tviCurr = m_pTree->InsertItem (buff, m_tviCurr);
		m_pTree->SetItemImage (m_tviCurr, cls, cls);
	}
	if (cls < 4 && cls >= 0) {
		kscmd *cmd = new kscmd;
		memset (cmd, 0, sizeof (kscmd));

		if (pAttr->raw_getValueFromName (wsNsUri, nNsUri, L"codebase", wcslen(L"codebase"), &wname, &len) != S_OK) {
			if (pAttr->raw_getValueFromName (wsNsUri, nNsUri, L"codeoff", wcslen(L"codeoff"), &wname, &len) == S_OK)
				cmd->code = wcstoul (wname, &wx1, 16) + ((kscmd*)(m_pTree->GetItemData (m_pTree->GetParentItem (m_tviCurr))))->code;
			else
				cmd->code = ((kscmd*)(m_pTree->GetItemData (m_pTree->GetParentItem (m_tviCurr))))->code;
		} else
			cmd->code = wcstoul (wname, &wx1, 16);
		m_pTree->SetItemData (m_tviCurr, (DWORD_PTR)cmd);
	}
	if (cls == 4) {
		kscmd* cmd = new kscmd;
		memset (cmd, 0, sizeof (kscmd));
		ksarg* arg = new ksarg;
		memset (arg, 0, sizeof (ksarg));
		cmd->args = arg;
		ksarg* ret = new ksarg;
		memset (ret, 0, sizeof (ksarg));
		cmd->rets = ret;
		bdlist_add (cmds, cmd);
		cmd->name = new char[strlen(buff)];
		strcpy (cmd->name, buff);

		pAttr->getValueFromName (wsNsUri, nNsUri, L"title", wcslen(L"title"), &wname, &len);
		wcstombs (buff, wname, 0x100);
		buff[len] = 0;
		cmd->title = new char[strlen(buff)];
		strcpy (cmd->title, buff);

		pAttr->getValueFromName (wsNsUri, nNsUri, L"state", wcslen(L"state"), &wname, &len);
		wcstombs (buff, wname, 0x100);
		buff[len] = 0;

		if (pAttr->raw_getValueFromName (wsNsUri, nNsUri, L"codebase", wcslen(L"codebase"), &wname, &len) != S_OK) {
			pAttr->raw_getValueFromName (wsNsUri, nNsUri, L"codeoff", wcslen(L"codeoff"), &wname, &len);
			cmd->code = wcstoul (wname, &wx1, 16) + ((kscmd*)(m_pTree->GetItemData (m_pTree->GetParentItem (m_tviCurr))))->code;
		} else
			cmd->code = wcstoul (wname, &wx1, 16);

		if (strcmp (buff, "prototype") == 0) {
			cmd->state = false;
			m_pTree->SetItemImage (m_tviCurr, 6, 6);
			m_pTree->SetItemData (m_tviCurr, (DWORD_PTR)cmd);
		} else {
			cmd->state = true;
			m_pTree->SetItemImage (m_tviCurr, 4, 4);
			m_pTree->SetItemData (m_tviCurr, (DWORD_PTR)cmd);
		}
		cmds = cmd;
	}
	if (cls == 5 || cls == 6) {
		j = 0;
		multipl = 1;
		do {
			ksarg* arg = new ksarg;
	//		memset (arg, 0, sizeof (ksarg));
			arg->gentype = 0;
			arg->subtype = 0;
			arg->str = "0";
			for (i = 0, arg_cnt = pAttr->getLength (); i < arg_cnt; ++i) {
				pAttr->getName (i, &wx1, &lenx1, &wname, &len, &wx2, &lenx2);
				pAttr->getValue (i, &wvalue, &len);
				p = wcschr (wname, L'=');
				if (p) *p = NULL;
				wcstombs (buff, wvalue, 0x100);
				buff[len] = 0;
				if (!wcscmp (wname, L"title")) {
					arg->title = new char[len];
					strcpy (arg->title, buff);
				} else if (!wcscmp (wname, L"name")) {
					arg->name = new char[len];
					strcpy (arg->name, buff);
				} else if (!wcscmp (wname, L"gentype")) {
					if (!strcmp (buff, "int")) arg->gentype = KSARGT_INT;
					else if (!strcmp (buff, "string")) arg->gentype = KSARGT_STRING;
					else if (!strcmp (buff, "real")) arg->gentype = KSARGT_REAL;
				} else if (!wcscmp (wname, L"subtype")) {
					if (!strcmp (buff, "bool")) arg->subtype = DARGST_BOOL;
					else if (!strcmp (buff, "enum")) arg->subtype = DARGST_ENUM;
					else if (!strcmp (buff, "greyval")) arg->subtype = DARGST_GREYVAL;
					else if (!strcmp (buff, "filename")) arg->subtype = DARGST_FILENAME;
					else if (!strcmp (buff, "graph")) arg->subtype = DARGST_GRAPH;
					else if (!strcmp (buff, "plot")) arg->subtype = DARGST_PLOT;
					else if (strstr (buff, "object")) {
						if (strstr (buff, "handle")) arg->subtype = DARGST_HOBJ;
						else if (strstr (buff, "content")) arg->subtype = DARGST_OBJECT;
						else if (strstr (buff, "filename")) arg->subtype = DARGST_OBJFILE;

						if (strstr (buff, "storage")) arg->subtype |= (DOBJT_STORAGE << 16);
						else if (strstr (buff, "image")) arg->subtype |= (DOBJT_IMAGE << 16);
						else if (strstr (buff, "mask")) arg->subtype |= (DOBJT_MASK << 16);
						else if (strstr (buff, "mark")) arg->subtype |= (DOBJT_MARK << 16);
						else if (strstr (buff, "label")) arg->subtype |= (DOBJT_LABEL << 16);
						else if (strstr (buff, "metric")) arg->subtype |= (DOBJT_METRIC << 16);
						else if (strstr (buff, "survey")) arg->subtype |= (DOBJT_SURVEY << 16);
					} else if (strstr (buff, "matrix")) {
						if (strstr (buff, "offsets")) arg->subtype = DARGST_MTX_OFFS;
						else if (strstr (buff, "factors")) arg->subtype = DARGST_MTX_FACTORS;
					} else if (strstr (buff, "coord")) {
						if (strstr (buff, "top")) arg->subtype = DARGST_COORD_TOP;
						else if (strstr (buff, "lect")) arg->subtype = DARGST_COORD_LEFT;
						else if (strstr (buff, "right")) arg->subtype = DARGST_COORD_RIGHT;
						else if (strstr (buff, "bottom")) arg->subtype = DARGST_COORD_BOTTOM;
					}
				} else if (!wcscmp (wname, L"default")) {
					arg->str = buff;
				} else if (!wcscmp (wname, L"multipl")) {
					if (j == 0) {
						multipl = atoi (buff);
						j = 1;
					}
				}
			}
			if (arg->gentype == KSARGT_STRING)
				arg->deflt.str = arg->val.str = arg->str;
			else if (arg->gentype == KSARGT_REAL)
				arg->deflt.r = arg->val.r = atof (arg->str);
			else if (arg->gentype == KSARGT_INT)
				arg->deflt.n = arg->val.n = atol (arg->str);
			bdlist_add ((cls == 6) ? (cmds->rets) : (cmds->args), arg);
		} while (--multipl);

	}
	if (cls == 0x100) {
		ksmtx_factors* fc = new ksmtx_factors;
//		memset (fc, 0, sizeof (ksmtx_factors));
		chars="";
		for (i = 0, arg_cnt = pAttr->getLength (); i < arg_cnt; ++i) {
			pAttr->getName (i, &wx1, &lenx1, &wname, &len, &wx2, &lenx2);
			pAttr->getValue (i, &wvalue, &len);
			p = wcschr (wname, L'=');
			if (p) *p = NULL;
			wcstombs (buff, wvalue, 0x100);
			buff[len] = 0;
			if (!wcscmp (wname, L"name"))
				fc->name = buff;
			else if (!wcscmp (wname, L"dimx"))
				fc->dimx = atoi (buff);
			else if (!wcscmp (wname, L"dimy"))
				fc->dimy = atoi (buff);
		}
		fc->factors = new rtbyte[fc->dimx * fc->dimy];
		bdlist_add (factors, fc);
	}
	if (cls == 0x101) {
		ksmtx_order* fc = new ksmtx_order;
//		memset (fc, 0, sizeof (ksmtx_order));
		chars="";
		for (i = 0, arg_cnt = pAttr->getLength (); i < arg_cnt; ++i) {
			pAttr->getName (i, &wx1, &lenx1, &wname, &len, &wx2, &lenx2);
			pAttr->getValue (i, &wvalue, &len);
			p = wcschr (wname, L'=');
			if (p) *p = NULL;
			wcstombs (buff, wvalue, 0x100);
			buff[len] = 0;
			if (!wcscmp (wname, L"name"))
				fc->name = buff;
			else if (!wcscmp (wname, L"dimx"))
				fc->dimx = atoi (buff);
			else if (!wcscmp (wname, L"dimy"))
				fc->dimy = atoi (buff);
		}
		fc->order = new sdword[fc->dimx * fc->dimy];
		bdlist_add (orders, fc);
	}
	return hr;
}

long __stdcall CSaxContent::raw_endElement (ushort *wsNsUri, int nNsUri, ushort *wsName, int nName,
					    ushort *wsQName, int nQName) {
	long l, i;
	char* ptr;
	long hr = S_OK;

	if (!wcscmp (wsName, L"cmdsys")) cls = 0;
	else if (!wcscmp (wsName, L"subsys")) cls = 1;
	else if (!wcscmp (wsName, L"class")) cls = 2;
	else if (!wcscmp (wsName, L"div")) cls = 3;
	else if (!wcscmp (wsName, L"cmd")) cls = 4;
	else if (!wcscmp (wsName, L"arg")) cls = 5;
	else if (!wcscmp (wsName, L"ret")) cls = 6;
	else if (!wcscmp (wsName, L"enum")) cls = 7;
	else if (!wcscmp (wsName, L"entry")) cls = 8;
	else if (!wcscmp (wsName, L"factors")) cls = 0x100;
	else if (!wcscmp (wsName, L"order")) cls = 0x101;
	else cls = -1;

	if (cls <= 4 && cls >= 0)
		if (m_tviCurr != TVI_ROOT)
			m_tviCurr = m_pTree->GetParentItem (m_tviCurr);
	if (cls == 0x100) {
		ksmtx_factors* fc = (ksmtx_factors*)bdlist_tail (factors);
		for (i = 0, ptr = chars.GetBuffer (); i < fc->dimx * fc->dimy && ptr; ++i)
			fc->factors[i] = strtod (ptr, &ptr);
	}
	if (cls == 0x101) {
		ksmtx_order* fc = (ksmtx_order*)bdlist_tail (orders);
		for (i = 0, ptr = chars.GetBuffer (); i < fc->dimx * fc->dimy && ptr; ++i) {
			l = strtol (ptr, &ptr, 10);
			fc->order[i*2 + 0] = l % fc->dimy - (fc->dimx - 1) / 2;
			fc->order[i*2 + 1] = l / fc->dimx - (fc->dimy - 1) / 2;
		}
	}
	cls = -1;
	return hr;
}

long __stdcall CSaxContent::raw_characters (ushort *wsChars, int nChars) {
	long hr = S_OK;
	if (cls >= 0x100) {
		wcstombs (buff, wsChars, nChars);
		buff[nChars] = 0x00;
		chars += buff;
	}
	return hr;
}

long __stdcall CSaxContent::raw_ignorableWhitespace (ushort *wsChars, int nChars) {
	long hr = S_OK;
	return hr;
}

long __stdcall CSaxContent::raw_processingInstruction (ushort *wsTarget, int nTarget,
						       ushort *wsData, int nData) {
	long hr = S_OK;
	return hr;
}

long __stdcall CSaxContent::raw_skippedEntity (ushort *wsName, int nName) {
	long hr = S_OK;
	return hr;
}


/////////// Class CSaxError

ulong CSaxError::references = 0;

CSaxError::CSaxError () { }

CSaxError::~CSaxError () { }

long __stdcall CSaxError::raw_error (ISAXLocator *pLocator, ushort *wsMessage, long hrCode) {
	wcstombs (buff, wsMessage, wcslen (wsMessage));
	MessageBox (NULL, buff, "SAX Parser Interface Wrapper", MB_ICONEXCLAMATION | MB_OK);
	return hrCode;
}

long __stdcall CSaxError::raw_fatalError (ISAXLocator *pLocator, ushort *wsMessage, long hrCode) {
	wcstombs (buff, wsMessage, wcslen (wsMessage));
	MessageBox (NULL, (char*)wsMessage, "SAX Parser Interface Wrapper", MB_ICONSTOP | MB_OK);
	return hrCode;
}

long __stdcall CSaxError::raw_ignorableWarning (ISAXLocator *pLocator, ushort *wsMessage, long hrCode) {
	wcstombs (buff, wsMessage, wcslen (wsMessage));
	MessageBox (NULL, buff, "SAX Parser Interface Wrapper", MB_ICONWARNING | MB_OK);
	return hrCode;
}

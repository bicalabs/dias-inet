#if !defined(AFX_DIASAPICHNLRANGE_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
#define AFX_DIASAPICHNLRANGE_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_

#pragma once

class CDiasAPIChnlRange : public CaImAPIRange<CChannel> {
friend class CDiasAPIChannels;
public:
	CDiasAPIChnlRange ();
	~CDiasAPIChnlRange ();

public:
	uvar32_64 Select (bool blSelect = true);
	uvar32_64 Show (bool blShow = true);
	uvar32_64 Export (uchar* fullname, udword format, bool verbal = true, udword flags = 0, uchar errormsg = true) throw (CaImAPIException);
	void GetBits (ubyte* pbBits, uvar32_64 len, bool transform = false) throw (CaImAPIException);
	void SetBits (ubyte* pbBits, uvar32_64 len) throw (CaImAPIException);
};

#endif // !defined(AFX_DIASAPICHNLRANGE_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)

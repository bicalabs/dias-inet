#if !defined(AFX_DIASAPIPTCHRANGE_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
#define AFX_DIASAPIPTCHRANGE_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_

#pragma once

class CDiasAPIPtchRange : public CaImAPIRange<CPatch> {
friend class CDiasAPIPatches;
public:
	CDiasAPIPtchRange ();
	~CDiasAPIPtchRange ();

public:
	uvar32_64 Select (bool blSelect = true);
	uvar32_64 Show (bool blShow = true);
	uvar32_64 Export (uchar* fullname, udword format, bool verbal = true, udword flags = 0, uchar errormsg = true);
};

#endif // !defined(AFX_DIASAPIPTCHRANGE_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)

#if !defined(AFX_DIASAPIDOCRANGE_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
#define AFX_DIASAPIDOCRANGE_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_

#pragma once

class CDiasAPIDocRange : public CaImAPIRange<CStorageDoc> {
friend class CDiasAPIDocs;
public:
	CDiasAPIDocRange ();
	~CDiasAPIDocRange ();

public:
	uvar32_64 Save ();
	uvar32_64 Close ();
};

#endif // !defined(AFX_DIASAPIDOCRANGE_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)

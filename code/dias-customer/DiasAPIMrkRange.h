#if !defined(AFX_DIASAPIMRKRANGE_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)
#define AFX_DIASAPIMRKRANGE_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_

#pragma once

class CDiasAPIMrkRange : public CaImAPIRange<CMark> {
friend class CDiasAPIMarks;
public:
	CDiasAPIMrkRange ();
	~CDiasAPIMrkRange ();

public:
	const CMark& operator= (const CMark& mrk);

	uvar32_64	Select (bool blSelect = true);
	uvar32_64	Show (bool blShow = true);
	uvar32_64	Move (CSize offset);
	uvar32_64	Duplicate (CSize offset);
	uvar32_64	Import (uchar* fullname, udword format, bool verbal = true, udword flags = 0, uchar errormsg = true);
	uvar32_64	Export (uchar* fullname, udword format, bool verbal = true, udword flags = 0, uchar errormsg = true);

	bool		IsKernelsEqual ();
	bool		IsShapesEqual ();
	bool		IsKernelSizesEqual ();
	bool		IsShapeSizesEqual ();
	bool		IsAnglesEqual ();
	bool		IsSizeLocksEqual ();
	bool		IsAngleLocksEqual ();
	bool		IsKernelColorsEqual ();
	bool		IsShapeColorsEqual ();
	bool		IsFillColorsEqual ();

	CMark::MarkKernel	GetKernels ();
	CMark::MarkShape	GetShapes ();
	uvar16_32	GetKernelSizes ();
	CSize		GetShapeSizes ();
	sbyte		GetAngles ();
	bool		GetSizeLocks ();
	bool		GetAngleLocks ();
	COLORREF	GetKernelColors ();
	COLORREF	GetShapeColors ();
	COLORREF	GetFillColors ();

	void		SetKernels (CMark::MarkKernel krnl);	
	void		SetShapes (CMark::MarkShape shape);	
	void		SetKernelSizes (uvar16_32 size);	
	void		SetShapeSizes (CSize size);	
	void		SetAngles (rtbyte angle);	
	void		SetSizeLocks (bool lock = true);		
	void		SetAngleLocks (bool lock = true);		
	void		SetKernelColors (COLORREF color);	
	void		SetShapeColors (COLORREF color);	
	void		SetFillColors (COLORREF color);	
};

#endif // !defined(AFX_DIASAPIMRKRANGE_H__A868D42F_C1E0_4794_A96E_85CF74A2634E__INCLUDED_)

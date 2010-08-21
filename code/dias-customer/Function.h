#if !defined(AFX_FUNCTION_H__4D16360B_3DB8_4659_A1EB_96BCA8702289__INCLUDED_)
#define AFX_FUNCTION_H__4D16360B_3DB8_4659_A1EB_96BCA8702289__INCLUDED_

#pragma once

class CFunction : public CObject {
	DECLARE_SERIAL (CColor);
// Construction
public:
	CColor ();
	CColor (uvar32_64 c1);
	CColor (uvar32_64 c1, uvar32_64 c2);
	CColor (uvar32_64 c1, uvar32_64 c2, uvar32_64 c3);
	CColor (uvar32_64 c1, uvar32_64 c2, uvar32_64 c3, uvar32_64 c4);
	CColor (uvar32_64* pColors, ubyte channels);
	CColor (const CColor& c);
	~CColor ();

	CColor& operator= (const CColor& c);
	uvar32_64& operator[] (ubyte channel);
	bool& operator() (ubyte channel);
	ubyte operator~ ();

// Attributes
protected:
	bool	m_blInitialized;

public:
	ubyte		m_bChannels;
	uvar32_64*	m_pdwValues;
	ubyte*		m_pbMask;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColor)
	//}}AFX_VIRTUAL
};

inline uvar32_64& CColor::operator[] (ubyte channel) {
	return (m_pdwValues[channel]);
}

inline bool& CColor::operator() (ubyte channel) {
	return ((bool&)(m_pbMask[channel]));
}

inline ubyte CColor::operator~ () {
	return (m_bChannels);
}

inline CColor& CColor::operator= (const CColor& c) {
	m_pdwValues = new uvar32_64[m_bChannels = c.m_bChannels];
	memcpy (m_pdwValues, c.m_pdwValues, sizeof (uvar32_64) * m_bChannels);
	m_pbMask = new ubyte[m_bChannels];
	memcpy (m_pbMask, c.m_pdwValues, m_bChannels);
	m_blInitialized = c.m_blInitialized;
	return (*this);
}

#endif // !defined(AFX_FUNCTION_H__4D16360B_3DB8_4659_A1EB_96BCA8702289__INCLUDED_)

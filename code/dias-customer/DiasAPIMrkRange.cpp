#include "stdafx.h"
#include "dias.h"
#include "DiasAPIMrkRange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiasAPIMrks

CDiasAPIMrkRange::CDiasAPIMrkRange () {
}

CDiasAPIMrkRange::~CDiasAPIMrkRange () {                           
}

const CMark& CDiasAPIMrkRange::operator= (const CMark& mrk) {
	for (uvar32_64 n = 0, cnt = GetCount (); n < cnt; ++n)
		operator[] (n) = mrk;
	return (mrk);
}

bool CDiasAPIMrkRange::IsKernelsEqual () {
	CMark::MarkKernel param;
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return (false);
	if (cnt == 1)
		return (true);
	param = operator[](0).GetKernel ();
	for (n = 1; n < cnt; ++n)
		if (param != operator[](n).GetKernel ())
			return (false);
	return (true);
}
bool CDiasAPIMrkRange::IsShapesEqual () {
	CMark::MarkShape param;
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return (false);
	if (cnt == 1)
		return (true);
	param = operator[](0).GetShape ();
	for (n = 1; n < cnt; ++n)
		if (param != operator[](n).GetShape ())
			return (false);
	return (true);
}
bool CDiasAPIMrkRange::IsKernelSizesEqual () {
	uvar16_32 param;
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return (false);
	if (cnt == 1)
		return (true);
	param = operator[](0).GetKernelSize ();
	for (n = 1; n < cnt; ++n)
		if (param != operator[](n).GetKernelSize ())
			return (false);
	return (true);
}
bool CDiasAPIMrkRange::IsShapeSizesEqual () {
	CSize param;
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return (false);
	if (cnt == 1)
		return (true);
	param = operator[](0).GetShapeSize ();
	for (n = 1; n < cnt; ++n)
		if (param != operator[](n).GetShapeSize ())
			return (false);
	return (true);
}
bool CDiasAPIMrkRange::IsAnglesEqual () {
	sword param;
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return (false);
	if (cnt == 1)
		return (true);
	param = operator[](0).GetAngle ();
	for (n = 1; n < cnt; ++n)
		if (param != operator[](n).GetAngle ())
			return (false);
	return (true);
}
bool CDiasAPIMrkRange::IsSizeLocksEqual () {
	bool param;
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return (false);
	if (cnt == 1)
		return (true);
	param = operator[](0).GetSizeLock ();
	for (n = 1; n < cnt; ++n)
		if (param != operator[](n).GetSizeLock ())
			return (false);
	return (true);
}
bool CDiasAPIMrkRange::IsAngleLocksEqual () {
	bool param;
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return (false);
	if (cnt == 1)
		return (true);
	param = operator[](0).GetAngleLock ();
	for (n = 1; n < cnt; ++n)
		if (param != operator[](n).GetAngleLock ())
			return (false);
	return (true);
}
bool CDiasAPIMrkRange::IsKernelColorsEqual () {
	COLORREF param;
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return (false);
	if (cnt == 1)
		return (true);
	param = operator[](0).GetKernelColor ();
	for (n = 1; n < cnt; ++n)
		if (param != operator[](n).GetKernelColor ())
			return (false);
	return (true);
}
bool CDiasAPIMrkRange::IsShapeColorsEqual () {
	COLORREF param;
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return (false);
	if (cnt == 1)
		return (true);
	param = operator[](0).GetShapeColor ();
	for (n = 1; n < cnt; ++n)
		if (param != operator[](n).GetShapeColor ())
			return (false);
	return (true);
}
bool CDiasAPIMrkRange::IsFillColorsEqual () {
	COLORREF param;
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return (false);
	if (cnt == 1)
		return (true);
	param = operator[](0).GetFillColor ();
	for (n = 1; n < cnt; ++n)
		if (param != operator[](n).GetFillColor ())
			return (false);
	return (true);
}

CMark::MarkKernel CDiasAPIMrkRange::GetKernels () {
	CMark::MarkKernel param;
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return (theApp.MarkStyles[aimActive].GetStyle().GetKernel());
	param = operator[](0).GetKernel ();
	for (n = 1; n < cnt; ++n)
		if (param != operator[](n).GetKernel ())
			return (theApp.MarkStyles[aimActive].GetStyle().GetKernel());
	return (param);
}
CMark::MarkShape CDiasAPIMrkRange::GetShapes () {
	CMark::MarkShape param;
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return (theApp.MarkStyles[aimActive].GetStyle().GetShape());
	param = operator[](0).GetShape ();
	for (n = 1; n < cnt; ++n)
		if (param != operator[](n).GetShape ())
			return (theApp.MarkStyles[aimActive].GetStyle().GetShape());
	return (param);
}
uvar16_32 CDiasAPIMrkRange::GetKernelSizes () {
	uvar16_32 param;
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return (theApp.MarkStyles[aimActive].GetStyle().GetKernelSize());
	param = operator[](0).GetKernelSize ();
	for (n = 1; n < cnt; ++n)
		if (param != operator[](n).GetKernelSize ())
			return (theApp.MarkStyles[aimActive].GetStyle().GetKernelSize());
	return (param);
}
CSize CDiasAPIMrkRange::GetShapeSizes () {
	CSize param;
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return (theApp.MarkStyles[aimActive].GetStyle().GetShapeSize());
	param = operator[](0).GetShapeSize ();
	for (n = 1; n < cnt; ++n)
		if (param != operator[](n).GetShapeSize ())
			return (theApp.MarkStyles[aimActive].GetStyle().GetShapeSize());
	return (param);
}
sbyte CDiasAPIMrkRange::GetAngles () {
	sword param;
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return (theApp.MarkStyles[aimActive].GetStyle().GetAngle());
	param = operator[](0).GetAngle ();
	for (n = 1; n < cnt; ++n)
		if (param != operator[](n).GetAngle ())
			return (theApp.MarkStyles[aimActive].GetStyle().GetAngle());
	return (param);
}
bool CDiasAPIMrkRange::GetSizeLocks () {
	bool param;
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return (theApp.MarkStyles[aimActive].GetStyle().GetSizeLock());
	param = operator[](0).GetSizeLock ();
	for (n = 1; n < cnt; ++n)
		if (param != operator[](n).GetSizeLock ())
			return (theApp.MarkStyles[aimActive].GetStyle().GetSizeLock());
	return (param);
}
bool CDiasAPIMrkRange::GetAngleLocks () {
	bool param;
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return (theApp.MarkStyles[aimActive].GetStyle().GetAngleLock());
	param = operator[](0).GetAngleLock ();
	for (n = 1; n < cnt; ++n)
		if (param != operator[](n).GetAngleLock ())
			return (theApp.MarkStyles[aimActive].GetStyle().GetAngleLock());
	return (param);
}
COLORREF CDiasAPIMrkRange::GetKernelColors () {
	COLORREF param;
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return (theApp.MarkStyles[aimActive].GetStyle().GetKernelColor());
	param = operator[](0).GetKernelColor ();
	for (n = 1; n < cnt; ++n)
		if (param != operator[](n).GetKernelColor ())
			return (theApp.MarkStyles[aimActive].GetStyle().GetKernelColor());
	return (param);
}
COLORREF CDiasAPIMrkRange::GetShapeColors () {
	COLORREF param;
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return (theApp.MarkStyles[aimActive].GetStyle().GetShapeColor());
	param = operator[](0).GetShapeColor ();
	for (n = 1; n < cnt; ++n)
		if (param != operator[](n).GetShapeColor ())
			return (theApp.MarkStyles[aimActive].GetStyle().GetShapeColor());
	return (param);
}
COLORREF CDiasAPIMrkRange::GetFillColors () {
	COLORREF param;
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return (theApp.MarkStyles[aimActive].GetStyle().GetFillColor());
	param = operator[](0).GetFillColor ();
	for (n = 1; n < cnt; ++n)
		if (param != operator[](n).GetFillColor ())
			return (theApp.MarkStyles[aimActive].GetStyle().GetFillColor());
	return (param);
}

void CDiasAPIMrkRange::SetKernels (CMark::MarkKernel param) {
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return;
	for (n = 0; n < cnt; ++n)
		operator[](n).SetKernel (param);
}
void CDiasAPIMrkRange::SetShapes (CMark::MarkShape param) {
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return;
	for (n = 0; n < cnt; ++n)
		operator[](n).SetShape (param);
}
void CDiasAPIMrkRange::SetKernelSizes (uvar16_32 param) {
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return;
	for (n = 0; n < cnt; ++n)
		operator[](n).SetKernelSize (param);
}
void CDiasAPIMrkRange::SetShapeSizes (CSize param) {
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return;
	for (n = 0; n < cnt; ++n)
		operator[](n).SetShapeSize (param);
}
void CDiasAPIMrkRange::SetAngles (rtbyte param) {
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return;
	for (n = 0; n < cnt; ++n)
		operator[](n).SetAngle (param);
}
void CDiasAPIMrkRange::SetSizeLocks (bool param) {
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return;
	for (n = 0; n < cnt; ++n)
		operator[](n).SetSizeLock (param);
}
void CDiasAPIMrkRange::SetAngleLocks (bool param) {
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return;
	for (n = 0; n < cnt; ++n)
		operator[](n).SetAngleLock (param);
}
void CDiasAPIMrkRange::SetKernelColors (COLORREF param) {
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return;
	for (n = 0; n < cnt; ++n)
		operator[](n).SetKernelColor (param);
}
void CDiasAPIMrkRange::SetShapeColors (COLORREF param) {
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return;
	for (n = 0; n < cnt; ++n)
		operator[](n).SetShapeColor (param);
}
void CDiasAPIMrkRange::SetFillColors (COLORREF param) {
	uvar32_64 n, cnt = GetCount ();
	if (cnt == 0)
		return;
	for (n = 0; n < cnt; ++n)
		operator[](n).SetFillColor (param);
}

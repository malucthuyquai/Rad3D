/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"

class CameraController
{
public:
	CameraController();
	~CameraController();

	void 
		ForceUpdate();

	void 
		Yaw(float degree);
	void 
		Pitch(float degree);
	void 
		Scroll(float dist);

	void 
		SetLookAt(const Float3 & pos);
	const Float3 &
		GetLookAt() { return mLookAt; }

	void 
		SetYaw(float degree);
	void 
		SetPitch(float degree);
	void 
		SetScroll(float dist);

protected:
	float mMinDist;
	float mMaxDist;

	float mMinPitchAngle;
	float mMaxPitchAngle;

	Float3 mLookAt;
	float mDist;
	float mYawDegree;
	float mPitchDegree;
};



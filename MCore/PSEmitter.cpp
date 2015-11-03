#include "PSEmitter.h"
#include "MParticleSystem.h"
#include "MRoot.h"

namespace Rad {

	ImplementRTTI(PS_Emitter, IObject);

	DF_PROPERTY_BEGIN(PS_Emitter)
		DF_PROPERTY(PS_Emitter, mEnable, "", "Enable", PT_Bool)

		DF_PROPERTY(PS_Emitter, mPosition, "", "Position", PT_Float3)
		DF_PROPERTY(PS_Emitter, mRotation, "", "Rotation", PT_Float3)
		DF_PROPERTY(PS_Emitter, mFirstEmit, "", "FirstEmit", PT_Int)
		DF_PROPERTY(PS_Emitter, mRate, "", "Rate", PT_Float)
		DF_PROPERTY(PS_Emitter, mDurationTime, "", "DurationTime", PT_Float)

		DF_PROPERTY_EX(PS_Emitter, mMinColor, "Randomness", "MinColor", "PT_Color4", PT_Float4)
		DF_PROPERTY_EX(PS_Emitter, mMaxColor, "Randomness", "MaxColor", "PT_Color4", PT_Float4)
		DF_PROPERTY(PS_Emitter, mMinRotation, "Randomness", "MinRotation", PT_Float3)
		DF_PROPERTY(PS_Emitter, mMaxRotation, "Randomness", "MaxRotation", PT_Float3)
		DF_PROPERTY(PS_Emitter, mAngle, "Randomness", "Angle", PT_Float2)
		DF_PROPERTY(PS_Emitter, mLife, "Randomness", "Life", PT_Float2)
		DF_PROPERTY(PS_Emitter, mSpeed, "Randomness", "Speed", PT_Float2)
		DF_PROPERTY(PS_Emitter, mDimension, "Randomness", "Dimension", PT_Float2)
		DF_PROPERTY(PS_Emitter, mSizeable, "Randomness", "Sizeable", PT_Bool)
		DF_PROPERTY(PS_Emitter, mMinSize, "Randomness", "MinSize", PT_Float3)
		DF_PROPERTY(PS_Emitter, mMaxSize, "Randomness", "MaxSize", PT_Float3)
		DF_PROPERTY(PS_Emitter, mUVRect, "Randomness", "UVRect", PT_Int2)
	DF_PROPERTY_END()

	PS_Emitter::PS_Emitter()
		: mParent(NULL)
		, mEnable(true)
		, mSizeable(false)
		, mPosition(Float3::Zero)
		, mRotation(0, 0, 0)
		, mDirection(0, 1, 0)
		, mCommonDirection(1, 0, 0)
		, mMinColor(1, 1, 1), mMaxColor(1, 1, 1)
		, mAngle(0, 180)
		, mLife(2, 4)
		, mSpeed(0.2f * UNIT_METRES, 0.4f * UNIT_METRES)
		, mDimension(0.2f * UNIT_METRES, 0.4f * UNIT_METRES)
		, mMinSize(1, 1, 1), mMaxSize(1, 1, 1)
		, mMinRotation(0, 0, 0), mMaxRotation(0, 0, 0)
		, mUVRect(1, 1)
		, mUVRectStep(1, 1)
		, mFirstEmit(0)
		, mRate(10)
		, mDurationTime(0)
		, mInternalTime(0)
		, mLastEmitTime(0)
		, mEmitCount(0)
	{
		mVersion = 1;
	}

	PS_Emitter::~PS_Emitter()
	{
	}

	void PS_Emitter::Init(PS_Set * parent)
	{
		mParent= parent;
	}

	void PS_Emitter::Reset()
	{
		mLastEmitTime = 0;
		mInternalTime = 0;
		mEmitCount = 0;
	}

	void PS_Emitter::SetEnable(bool enable)
	{
		mEnable = enable;

		mLastEmitTime = 0;
		mInternalTime = 0;
		mEmitCount = 0;
	}

	bool PS_Emitter::IsEnable() const
	{
		return mEnable;
	}

	void PS_Emitter::SetSizeable(bool able)
	{
		mSizeable = able;
	}
	
	bool PS_Emitter::IsSizeable() const
	{
		return mSizeable; 
	}


	void PS_Emitter::SetPosition(const Float3 & pos)
	{
		mPosition = pos;
	}

	const Float3 & PS_Emitter::GetPosition() const
	{
		return mPosition;
	}

	void PS_Emitter::SetRotation(const Float3 & angels)
	{
		mRotation = angels;

		Quat q;
		q.FromEulerAngle(mRotation);

		mDirection = Float3(0, 1, 0) * q;
		mCommonDirection = Float3(1, 0, 0) * q;
	}

	void PS_Emitter::SetDirection(const Float3 & dir, const Float3 & commonDir)
	{
		mDirection = dir;
		mCommonDirection = commonDir;
	}

	const Float3 & PS_Emitter::GetDirection() const
	{
		return mDirection;
	}

	const Float3 & PS_Emitter::GetCommonDirection() const
	{
		return mCommonDirection;
	}

	void PS_Emitter::SetColor(const Float4 & _min, const Float4 & _max)
	{
		mMinColor = _min;
		mMaxColor = _max;
	}

	const Float4 & PS_Emitter::GetMinColor() const
	{
		return mMinColor;
	}

	const Float4 & PS_Emitter::GetMaxColor() const
	{
		return mMaxColor;
	}

	void PS_Emitter::SetAngle(const Float2 & v)
	{
		mAngle = v;
	}

	void PS_Emitter::SetLife(const Float2 & v)
	{
		mLife = v;
	}

	void PS_Emitter::SetSpeed(const Float2 & v)
	{
		mSpeed = v;
	}

	void PS_Emitter::SetDimension(const Float2 & v)
	{
		mDimension = v;
	}

	void PS_Emitter::SetSize(const Float3 & minSize, const Float3 & maxSize)
	{
		mMinSize = minSize;
		mMaxSize = maxSize;
	}

	void PS_Emitter::SetRotation(const Float3 & minRotation, const Float3 & maxRotation)
	{
		mMinRotation = minRotation;
		mMaxRotation = mMaxRotation;
	}

	void PS_Emitter::SetUVRect(const Int2 & xy)
	{
		mUVRect.x = Max(1, xy.x);
		mUVRect.y = Max(1, xy.y);

		mUVRectStep.x = 1.0f / mUVRect.x;
		mUVRectStep.y = 1.0f / mUVRect.y;
	}
	
	const Int2 & PS_Emitter::GetUVRect() const
	{
		return mUVRect;
	}

	RectF PS_Emitter::GetUVRect(int x, int y) const
	{
		d_assert (x < mUVRect.x && y < mUVRect.y);

		RectF rect;
		rect.x1 = x * mUVRectStep.x;
		rect.y1 = y * mUVRectStep.y;
		rect.x2 = rect.x1 + mUVRectStep.x;
		rect.y2 = rect.y1 + mUVRectStep.y;

		return rect;
	}

	RectF PS_Emitter::GetUVRect(int index) const
	{
		int y = index / mUVRect.x;
		int x = index % mUVRect.x;

		return GetUVRect(x, y);
	}

	int PS_Emitter::GetUVRectCount() const
	{
		return mUVRect.x * mUVRect.y;
	}

	void PS_Emitter::SetRate(float rate)
	{
		mRate = rate;
	}

	float PS_Emitter::GetRate() const
	{
		return mRate;
	}

	void PS_Emitter::SetDurationTime(float time)
	{
		mDurationTime = time;
		mInternalTime = 0;
		mLastEmitTime = 0;
	}

	float PS_Emitter::GetDurationTime() const
	{
		return mDurationTime;
	}

	int PS_Emitter::GetEmitCount() const
	{
		return mEmitCount;
	}

	void PS_Emitter::InitParticle(Particle * p)
	{
		p->Emitter = this;

		p->Color = RandomColor();
		p->Direction = RandomDirection();
		p->UVRect = RandomUVRect();
		p->Size = RandomSize();
		p->Speed = RandomSpeed();
		p->Rotation = RandomRotation();
		p->Life = RandomLife();
		p->MaxLife = p->Life;
		p->Param[0].v_int = 0;
		p->Param[1].v_int = 0;
		p->Param[2].v_int = 0;
		p->Param[3].v_int = 0;
	}

	bool PS_Emitter::Emit(float elapsedTime)
	{
		mEmitCount = 0;
		mInternalTime += elapsedTime;

		if (mDurationTime != 0 && mInternalTime > mDurationTime)
		{
			return false;
		}

		if (mLastEmitTime == 0 && mFirstEmit > 0)
		{
			mLastEmitTime = mInternalTime;
			mEmitCount = mFirstEmit;
		}
		else if (mRate > 0)
		{
			float time = mInternalTime - mLastEmitTime;
			float emit_time = 1.0f / mRate;

			while (time > emit_time)
			{
				mLastEmitTime = mInternalTime;
				mEmitCount += 1;
				time -= emit_time;
			}
		}

		return mEmitCount > 0;
	}

	Float3 PS_Emitter::RandomDirection()
	{
		float rads = Math::RandRange(mAngle.x, mAngle.y);
		rads = Math::DegreeToRadian(rads);

		Quat qRotate;
		qRotate.FromAxis(mCommonDirection, rads);

		Float3 dir = mDirection * qRotate;

		rads = Math::RandRange(0.0f, 2 * PI);
		qRotate.FromAxis(mDirection, rads);
		dir = dir * qRotate;

		return dir;
	}

	Float3 PS_Emitter::RandomRotation()
	{
		float x = Math::RandRange(mMinRotation.x, mMaxRotation.x);
		float y = Math::RandRange(mMinRotation.y, mMaxRotation.y);
		float z = Math::RandRange(mMinRotation.z, mMaxRotation.z);

		return Float3(x, y, z);
	}

	Float3 PS_Emitter::RandomSize()
	{
		if (!mSizeable)
		{
			float d = Math::RandRange(mDimension.x, mDimension.y);

			return Float3(d, d, d);
		}
		else
		{
			float x = Math::RandRange(mMinSize.x, mMaxSize.x);
			float y = Math::RandRange(mMinSize.y, mMaxSize.y);
			float z = Math::RandRange(mMinSize.z, mMaxSize.z);

			return Float3(x, y, z);
		}
	}

	Float4 PS_Emitter::RandomColor()
	{
		Float4 color;

		color.r = Math::RandRange(mMinColor.r, mMaxColor.r);
		color.g = Math::RandRange(mMinColor.g, mMaxColor.g);
		color.b = Math::RandRange(mMinColor.b, mMaxColor.b);
		color.a = Math::RandRange(mMinColor.a, mMaxColor.a);

		return color;
	}

	RectF PS_Emitter::RandomUVRect()
	{
		int x = Math::RandRange(0, mUVRect.x - 1);
		int y = Math::RandRange(0, mUVRect.y - 1);

		return GetUVRect(x, y);
	}

	float PS_Emitter::RandomSpeed()
	{
		return Math::RandRange(mSpeed.x, mSpeed.y);
	}

	float PS_Emitter::RandomLife()
	{
		if (mLife.x != 0 || mLife.y != 0)
		{
			return Math::RandRange(mLife.x, mLife.y);
		}
		else
		{
			return MAX_FLOAT;
		}
	}

	IObject * PS_Emitter::Clone()
	{
		PS_Emitter * emitter = (PS_Emitter *)Root::NEW_OBJECT(GetRTTI()->Name());
		if (emitter)
		{
			emitter->Init(mParent);
			emitter->Copy(this);
		}

		return emitter;
	}

	void PS_Emitter::OnPropertyChanged(const Property * p)
	{
		if (p->name == "mUVRect")
		{
			SetUVRect(mUVRect);
		}
		else if (p->name == "mEnable")
		{
			SetEnable(mEnable);
		}
		else if (p->name == "mRotation")
		{
			SetRotation(mRotation);
		}
		else if (p->name == "mDurationTime")
		{
			SetDurationTime(mDurationTime);
		}
		else
		{
			IObject::OnPropertyChanged(p);
		}
	}

	void PS_Emitter::Serialize(Serializer & sl)
	{
		IObject::Serialize(sl);

		if (!sl.IsOut() && mVersion == 0)
		{
			ISerializer & IS = (ISerializer &)sl;

			int ver = 3;
			IS >> ver;
		}

		mVersion = 1;
	}

}
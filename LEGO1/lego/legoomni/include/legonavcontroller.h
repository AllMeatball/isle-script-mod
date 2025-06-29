#ifndef __LEGONAVCONTROLLER_H
#define __LEGONAVCONTROLLER_H

#include "decomp.h"
#include "mxcore.h"
#include "mxtypes.h"

struct LegoLocation;
class Vector3;

//////////////////////////////////////////////////////////////////////////////
//
// LegoMouseController

// VTABLE: LEGO1 0x100d85b8
// VTABLE: BETA10 0x101bcc80
// SIZE 0x70
class LegoNavController : public MxCore {
public:
	LegoNavController();
	~LegoNavController() override;

	MxLong Notify(MxParam& p_param) override; // vtable+0x04

	// FUNCTION: LEGO1 0x10054b80
	// FUNCTION: BETA10 0x1009e5f0
	const char* ClassName() const override // vtable+0x0c
	{
		// STRING: LEGO1 0x100f66d8
		return "LegoNavController";
	}

	// FUNCTION: LEGO1 0x10054b90
	MxBool IsA(const char* p_name) const override // vtable+0x10
	{
		return !strcmp(p_name, ClassName()) || MxCore::IsA(p_name);
	}

	void SetTargets(int p_hPos, int p_vPos, MxBool p_accel);
	void SetControlMax(int p_hMax, int p_vMax);
	void SetToDefaultParams();
	MxBool CalculateNewPosDir(
		const Vector3& p_curPos,
		const Vector3& p_curDir,
		Vector3& p_newPos,
		Vector3& p_newDir,
		const Vector3* p_und
	);

	static void GetDefaults(
		int* p_dz,
		float* p_lv,
		float* p_rv,
		float* p_la,
		float* p_ra,
		float* p_ld,
		float* p_rd,
		float* p_lmina,
		float* p_rmina,
		float* p_rs,
		MxBool* p_urs
	);
	static void SetDefaults(
		int p_dz,
		float p_lv,
		float p_rv,
		float p_la,
		float p_ra,
		float p_ld,
		float p_rd,
		float p_lmina,
		float p_rmina,
		float p_rs,
		MxBool p_urs
	);
	static MxResult UpdateLocation(MxU32 p_location);
	static MxResult UpdateLocation(const char* p_location);
	static MxS32 GetNumLocations();
	static LegoLocation* GetLocation(MxU32 p_location);

	// FUNCTION: BETA10 0x100b0f40
	void SetLinearVel(MxFloat p_linearVel) { m_linearVel = p_linearVel; }

	// FUNCTION: BETA10 0x100c99e0
	void SetDeadZone(MxS32 p_deadZone) { m_deadZone = p_deadZone; }

	// FUNCTION: BETA10 0x100c7880
	void SetTrackDefault(MxS32 p_trackDefault) { m_trackDefault = p_trackDefault; }

	// FUNCTION: BETA10 0x100178a0
	void Reset()
	{
		m_trackDefault = TRUE;
		SetToDefaultParams();
	}

	MxFloat GetLinearVel() { return m_linearVel; }
	MxFloat GetRotationalVel() { return m_rotationalVel; }
	MxFloat GetMaxLinearVel() { return m_maxLinearVel; }
	MxFloat GetMaxLinearAccel() { return m_maxLinearAccel; }
	MxFloat GetMaxLinearDeccel() { return m_maxLinearDeccel; }
	void ResetMaxLinearVel(MxFloat p_maxLinearVel)
	{
		m_maxLinearVel = p_maxLinearVel;
		m_trackDefault = FALSE;
	}
	void ResetMaxLinearAccel(MxFloat p_maxLinearAccel)
	{
		m_maxLinearAccel = p_maxLinearAccel;
		m_trackDefault = FALSE;
	}
	void ResetMaxLinearDeccel(MxFloat p_maxLinearDeccel)
	{
		m_maxLinearDeccel = p_maxLinearDeccel;
		m_trackDefault = FALSE;
	}

	// FUNCTION: BETA10 0x100c9a10
	int GetDefaultDeadZone() { return g_defdeadZone; }

	void Unk0x5dHack(MxBool p_unk0x5d) { m_unk0x5d = p_unk0x5d; }

	// SYNTHETIC: LEGO1 0x10054c10
	// LegoNavController::`scalar deleting destructor'

protected:
	float CalculateNewVel(float p_targetVel, float p_currentVel, float p_accel, float p_time);
	float CalculateNewTargetVel(int p_pos, int p_center, float p_max);
	float CalculateNewAccel(int p_pos, int p_center, float p_max, int p_min);

	MxResult ProcessJoystickInput(MxBool& p_und);
	MxResult ProcessKeyboardInput();

	int m_hMax;                  // 0x08
	int m_vMax;                  // 0x0c
	int m_deadZone;              // 0x10
	float m_zeroThreshold;       // 0x14
	float m_linearVel;           // 0x18
	float m_rotationalVel;       // 0x1c
	float m_targetLinearVel;     // 0x20
	float m_targetRotationalVel; // 0x24
	float m_maxLinearVel;        // 0x28
	float m_maxRotationalVel;    // 0x2c
	float m_linearAccel;         // 0x30
	float m_rotationalAccel;     // 0x34
	float m_maxLinearAccel;      // 0x38
	float m_maxRotationalAccel;  // 0x3c
	float m_minLinearAccel;      // 0x40
	float m_minRotationalAccel;  // 0x44
	float m_maxLinearDeccel;     // 0x48
	float m_maxRotationalDeccel; // 0x4c
	float m_rotSensitivity;      // 0x50
	MxBool m_useRotationalVel;   // 0x54
	MxTime m_lastTime;           // 0x58
	MxBool m_trackDefault;       // 0x5c
	MxBool m_unk0x5d;            // 0x5d
	float m_unk0x60;             // 0x60
	float m_unk0x64;             // 0x64
	float m_unk0x68;             // 0x68
	MxBool m_isAccelerating;     // 0x6c

	// one copy of defaults (these can be set by App.)
	static int g_defdeadZone;
	static float g_defzeroThreshold;
	static float g_defmaxLinearVel;
	static float g_defmaxRotationalVel;
	static float g_defmaxLinearAccel;
	static float g_defmaxRotationalAccel;
	static float g_defminLinearAccel;
	static float g_defminRotationalAccel;
	static float g_defmaxLinearDeccel;
	static float g_defmaxRotationalDeccel;
	static float g_defrotSensitivity;
	static MxBool g_defuseRotationalVel;
};

#endif // __LEGOPOVCONTROLLER_H

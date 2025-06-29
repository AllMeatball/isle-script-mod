#include "legonavcontroller.h"

#include "3dmanager/lego3dmanager.h"
#include "act3.h"
#include "infocenter.h"
#include "legoanimationmanager.h"
#include "legocameracontroller.h"
#include "legocharactermanager.h"
#include "legogamestate.h"
#include "legoinputmanager.h"
#include "legolocations.h"
#include "legomain.h"
#include "legoplantmanager.h"
#include "legosoundmanager.h"
#include "legoutils.h"
#include "legovideomanager.h"
#include "legoworld.h"
#include "misc.h"
#include "mxbackgroundaudiomanager.h"
#include "mxdebug.h"
#include "mxmisc.h"
#include "mxtimer.h"
#include "mxtransitionmanager.h"
#include "mxutilities.h"
#include "realtime/realtime.h"
#include "realtime/realtimeview.h"
#include "viewmanager/viewmanager.h"

#include <SDL3/SDL_stdinc.h>
#include <vec.h>

DECOMP_SIZE_ASSERT(LegoNavController, 0x70)

// MSVC 4.20 didn't define a macro for this key
#ifndef VK_OEM_MINUS
#define VK_OEM_MINUS 0xBD
#endif

//////////////////////////////////////////////////////////////////////

#ifndef M_PI
#define M_PI 3.1416
#endif
#ifdef DTOR
#undef DTOR
#endif
#define DTOR(angle) ((angle) * M_PI / 180.)

//////////////////////////////////////////////////////////////////////

// GLOBAL: LEGO1 0x100f4c28
int LegoNavController::g_defdeadZone = 40;

// GLOBAL: LEGO1 0x100f4c2c
float LegoNavController::g_defzeroThreshold = 0.001f;

// GLOBAL: LEGO1 0x100f4c30
float LegoNavController::g_defmaxLinearVel = 40.0f;

// GLOBAL: LEGO1 0x100f4c34
float LegoNavController::g_defmaxRotationalVel = 20.0f;

// GLOBAL: LEGO1 0x100f4c38
float LegoNavController::g_defmaxLinearAccel = 15.0f;

// GLOBAL: LEGO1 0x100f4c3c
float LegoNavController::g_defmaxRotationalAccel = 30.0f;

// GLOBAL: LEGO1 0x100f4c40
float LegoNavController::g_defminLinearAccel = 4.0f;

// GLOBAL: LEGO1 0x100f4c44
float LegoNavController::g_defminRotationalAccel = 15.0f;

// GLOBAL: LEGO1 0x100f4c48
float LegoNavController::g_defmaxLinearDeccel = 50.0f;

// GLOBAL: LEGO1 0x100f4c4c
float LegoNavController::g_defmaxRotationalDeccel = 50.0f;

// GLOBAL: LEGO1 0x100f4c50
float LegoNavController::g_defrotSensitivity = 0.4f;

// GLOBAL: LEGO1 0x100f4c54
MxBool LegoNavController::g_defuseRotationalVel = FALSE;

// GLOBAL: LEGO1 0x100f66a0
MxU32 g_changeLight = FALSE;

// GLOBAL: LEGO1 0x100f66a4
MxS32 g_locationCalcStep = 0;

// GLOBAL: LEGO1 0x100f66a8
MxS32 g_nextLocation = 0;

// GLOBAL: LEGO1 0x100f66ac
MxBool g_resetPlants = FALSE;

// GLOBAL: LEGO1 0x100f66b0
MxS32 g_animationCalcStep = 0;

// GLOBAL: LEGO1 0x100f66b4
MxS32 g_nextAnimation = 0;

// GLOBAL: LEGO1 0x100f66b8
MxU32 g_switchAct = FALSE;

// GLOBAL: LEGO1 0x100f66bc
LegoAnimationManager::PlayMode g_unk0x100f66bc = LegoAnimationManager::e_unk2;

// GLOBAL: LEGO1 0x100f66c0
SDL_Keycode g_debugPassword[] = {
	// "OGEL"
	SDLK_O,
	SDLK_G,
	SDLK_E,
	SDLK_L,
	0,
};

// GLOBAL: LEGO1 0x100f66c8
SDL_Keycode* g_currentInput = g_debugPassword;

// GLOBAL: LEGO1 0x100f66cc
MxS32 g_nextCharacter = -1;

// GLOBAL: LEGO1 0x100f66d0
MxBool g_enableMusic = TRUE;

// GLOBAL: LEGO1 0x100f66d4
MxU32 g_fpsEnabled = TRUE;

// FUNCTION: LEGO1 0x10054ac0
LegoNavController::LegoNavController()
{
	SetToDefaultParams();

	m_linearVel = 0.0f;
	m_rotationalVel = 0.0f;
	m_targetLinearVel = 0.0f;
	m_targetRotationalVel = 0.0f;
	m_linearAccel = 0.0f;
	m_rotationalAccel = 0.0f;
	m_trackDefault = FALSE;
	m_unk0x5d = FALSE;
	m_isAccelerating = FALSE;
	m_unk0x64 = 0.0f;
	m_unk0x68 = 0.0f;
	m_unk0x60 = 0.0f;

	m_lastTime = Timer()->GetTime();

	InputManager()->Register(this);
}

// FUNCTION: LEGO1 0x10054c30
LegoNavController::~LegoNavController()
{
	InputManager()->UnRegister(this);
}

// FUNCTION: LEGO1 0x10054ca0
void LegoNavController::SetControlMax(int p_hMax, int p_vMax)
{
	m_hMax = p_hMax;
	m_vMax = p_vMax;

	if (VideoManager()->GetVideoParam().Flags().GetFullScreen()) {
		m_hMax = 640;
		m_vMax = 480;
	}
}

// FUNCTION: LEGO1 0x10054cd0
// FUNCTION: BETA10 0x1009ad76
void LegoNavController::SetToDefaultParams()
{
	m_deadZone = g_defdeadZone;
	m_zeroThreshold = g_defzeroThreshold;
	m_maxRotationalAccel = g_defmaxRotationalAccel;
	m_maxLinearAccel = g_defmaxLinearAccel;
	m_minRotationalAccel = g_defminRotationalAccel;
	m_minLinearAccel = g_defminLinearAccel;
	m_maxRotationalDeccel = g_defmaxRotationalDeccel;
	m_maxLinearDeccel = g_defmaxLinearDeccel;
	m_maxRotationalVel = g_defmaxRotationalVel;
	m_maxLinearVel = g_defmaxLinearVel;
	m_useRotationalVel = g_defuseRotationalVel;
	m_rotSensitivity = g_defrotSensitivity;
}

// FUNCTION: LEGO1 0x10054d40
void LegoNavController::GetDefaults(
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
)
{
	*p_dz = g_defdeadZone;
	*p_lv = g_defmaxLinearVel;
	*p_rv = g_defmaxRotationalVel;
	*p_la = g_defmaxLinearAccel;
	*p_ra = g_defmaxRotationalAccel;
	*p_ld = g_defmaxLinearDeccel;
	*p_rd = g_defmaxRotationalDeccel;
	*p_lmina = g_defminLinearAccel;
	*p_rmina = g_defminRotationalAccel;
	*p_rs = g_defrotSensitivity;
	*p_urs = g_defuseRotationalVel;
}

// FUNCTION: LEGO1 0x10054dd0
void LegoNavController::SetDefaults(
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
)
{
	g_defdeadZone = p_dz;
	g_defmaxLinearVel = p_lv;
	g_defmaxRotationalVel = p_rv;
	g_defmaxLinearAccel = p_la;
	g_defmaxRotationalAccel = p_ra;
	g_defmaxLinearDeccel = p_ld;
	g_defmaxRotationalDeccel = p_rd;
	g_defminLinearAccel = p_lmina;
	g_defminRotationalAccel = p_rmina;
	g_defrotSensitivity = p_rs;
	g_defuseRotationalVel = p_urs;
}

// FUNCTION: LEGO1 0x10054e40
void LegoNavController::SetTargets(int p_hPos, int p_vPos, MxBool p_accel)
{
	if (m_trackDefault != FALSE) {
		SetToDefaultParams();
	}

	if (p_accel != FALSE) {
		m_targetRotationalVel = CalculateNewTargetVel(p_hPos, m_hMax / 2, m_maxRotationalVel);
		m_targetLinearVel = CalculateNewTargetVel(m_vMax - p_vPos, m_vMax / 2, m_maxLinearVel);
		m_rotationalAccel = CalculateNewAccel(p_hPos, m_hMax / 2, m_maxRotationalAccel, (int) m_minRotationalAccel);
		m_linearAccel = CalculateNewAccel(m_vMax - p_vPos, m_vMax / 2, m_maxLinearAccel, (int) m_minLinearAccel);
	}
	else {
		m_targetRotationalVel = 0;
		m_targetLinearVel = 0;
		m_linearAccel = m_maxLinearDeccel;
		m_rotationalAccel = m_maxRotationalDeccel;
	}
}

// FUNCTION: LEGO1 0x10054f10
float LegoNavController::CalculateNewTargetVel(int p_pos, int p_center, float p_max)
{
	float newVel;
	int diff = p_pos - p_center;

	if (diff > m_deadZone) {
		newVel = (diff - m_deadZone) * p_max / (p_center - m_deadZone);
	}
	else if (diff < -m_deadZone) {
		newVel = (diff + m_deadZone) * p_max / (p_center - m_deadZone);
	}
	else {
		newVel = 0.0;
	}

	return newVel;
}

// FUNCTION: LEGO1 0x10054f90
float LegoNavController::CalculateNewAccel(int p_pos, int p_center, float p_max, int p_min)
{
	float newAccel;
	int diff = p_pos - p_center;

	newAccel = Abs(diff) * p_max / p_center;

	if (newAccel < p_min) {
		newAccel = (float) p_min;
	}

	return newAccel;
}

// FUNCTION: LEGO1 0x10054fe0
float LegoNavController::CalculateNewVel(float p_targetVel, float p_currentVel, float p_accel, float p_time)
{
	float newVel = p_currentVel;

	float velDiff = p_targetVel - p_currentVel;
	int vSign = velDiff > 0 ? 1 : -1;

	if (Abs(velDiff) > m_zeroThreshold) {
		float deltaVel = p_accel * p_time;
		newVel = p_currentVel + (deltaVel * vSign);

		if (vSign > 0) {
			newVel = Min(newVel, p_targetVel);
		}
		else {
			newVel = Max(newVel, p_targetVel);
		}
	}

	return newVel;
}

// FUNCTION: LEGO1 0x10055080
// FUNCTION: BETA10 0x1009b26b
MxBool LegoNavController::CalculateNewPosDir(
	const Vector3& p_curPos,
	const Vector3& p_curDir,
	Vector3& p_newPos,
	Vector3& p_newDir,
	const Vector3* p_und
)
{
	if (!g_isWorldActive) {
		return FALSE;
	}

	MxBool changed = FALSE;
	MxBool und = FALSE;

	MxTime currentTime = Timer()->GetTime();
	float deltaTime = (currentTime - m_lastTime) / 1000.0;
	m_lastTime = currentTime;

	if (ProcessKeyboardInput() == FAILURE) {
		ProcessJoystickInput(und);
	}

	if (m_useRotationalVel) {
		m_rotationalVel = CalculateNewVel(m_targetRotationalVel, m_rotationalVel, m_rotationalAccel * 40.0f, deltaTime);
	}
	else {
		m_rotationalVel = m_targetRotationalVel * m_maxRotationalVel * deltaTime;
	}

	m_linearVel = CalculateNewVel(m_targetLinearVel, m_linearVel, m_linearAccel, deltaTime);

	if (und || (Abs(m_rotationalVel) > m_zeroThreshold) || (Abs(m_linearVel) > m_zeroThreshold)) {
		float rot_mat[3][3];
		Mx3DPointFloat delta_pos, new_dir, new_pos;

		if (m_linearVel < -(m_maxLinearVel * 0.4f)) {
			m_linearVel = -(m_maxLinearVel * 0.4f);
		}

		VXS3(delta_pos, p_curDir, m_linearVel * deltaTime);
		VPV3(p_newPos, p_curPos, delta_pos);

		float delta_rad;
		if (m_useRotationalVel) {
			delta_rad = DTOR(m_rotationalVel * deltaTime);
		}
		else {
			delta_rad = DTOR(m_rotationalVel * m_rotSensitivity);
		}

		if (p_und != NULL && (*p_und)[1] < 0.0f) {
			delta_rad = -delta_rad;
		}

		IDENTMAT3(rot_mat);
		rot_mat[0][0] = rot_mat[2][2] = cos(delta_rad);
		rot_mat[0][2] = rot_mat[2][0] = sin(delta_rad);
		rot_mat[0][2] *= -1.0f;
		VXM3(p_newDir, p_curDir, rot_mat);

		changed = TRUE;
	}

	if (m_unk0x5d) {
		float rot_mat[3][3];
		Mx3DPointFloat delta_pos, new_pos, new_dir;

		if (changed) {
			SET3(new_pos, p_newPos);
			SET3(new_dir, p_newDir);
		}
		else {
			SET3(new_pos, p_curPos);
			SET3(new_dir, p_curDir);
		}

		if (m_unk0x64 != 0.0f) {
			delta_pos[0] = new_dir[0] * m_unk0x64;
			delta_pos[1] = new_dir[1] * m_unk0x64;
			delta_pos[2] = new_dir[2] * m_unk0x64;
		}
		else {
			FILLVEC3(delta_pos, 0.0f);
		}

		delta_pos[1] += m_unk0x60;
		VPV3(p_newPos, new_pos, delta_pos);

		if (m_unk0x68 != 0.0f) {
			float delta_rad = DTOR(m_unk0x68);
			IDENTMAT3(rot_mat);
			rot_mat[0][0] = rot_mat[2][2] = cos(delta_rad);
			rot_mat[0][2] = rot_mat[2][0] = sin(delta_rad);
			rot_mat[0][2] *= -1.0f;
			VXM3(p_newDir, new_dir, rot_mat);
		}
		else {
			SET3(p_newDir, new_dir);
		}

		m_unk0x60 = m_unk0x64 = m_unk0x68 = 0.0f;
		m_unk0x5d = FALSE;
		changed = TRUE;
	}

	return changed;
}

// FUNCTION: LEGO1 0x10055500
// FUNCTION: BETA10 0x1009bff8
MxResult LegoNavController::UpdateLocation(const char* p_location)
{
	MxResult result = FAILURE;

	for (MxS32 i = 0; i < (MxS32) sizeOfArray(g_locations); i++) {
		if (!SDL_strcasecmp(p_location, g_locations[i].m_name)) {
			MxMatrix mat;
			LegoROI* viewROI = VideoManager()->GetViewROI();

			CalcLocalTransform(g_locations[i].m_position, g_locations[i].m_direction, g_locations[i].m_up, mat);

			Mx3DPointFloat vec;
			vec.Clear();

			viewROI->SetWorldVelocity(vec);
			viewROI->WrappedSetLocal2WorldWithWorldDataUpdate(mat);
			VideoManager()->Get3DManager()->Moved(*viewROI);

			SoundManager()->UpdateListener(
				viewROI->GetWorldPosition(),
				viewROI->GetWorldDirection(),
				viewROI->GetWorldUp(),
				viewROI->GetWorldVelocity()
			);

			result = SUCCESS;
		}
	}

	return result;
}

// FUNCTION: LEGO1 0x10055620
// FUNCTION: BETA10 0x1009c145
MxResult LegoNavController::UpdateLocation(MxU32 p_location)
{
	MxResult result = FAILURE;

	if (p_location < sizeOfArray(g_locations)) {
		MxMatrix mat;
		LegoROI* viewROI = VideoManager()->GetViewROI();

		CalcLocalTransform(
			g_locations[p_location].m_position,
			g_locations[p_location].m_direction,
			g_locations[p_location].m_up,
			mat
		);

		Mx3DPointFloat vec;
		vec.Clear();

		viewROI->SetWorldVelocity(vec);
		viewROI->WrappedSetLocal2WorldWithWorldDataUpdate(mat);
		VideoManager()->Get3DManager()->Moved(*viewROI);

		SoundManager()->UpdateListener(
			viewROI->GetWorldPosition(),
			viewROI->GetWorldDirection(),
			viewROI->GetWorldUp(),
			viewROI->GetWorldVelocity()
		);

		result = SUCCESS;
	}

	return result;
}

// FUNCTION: LEGO1 0x10055720
// FUNCTION: BETA10 0x1009c259
LegoLocation* LegoNavController::GetLocation(MxU32 p_location)
{
	if (p_location < sizeOfArray(g_locations)) {
		return &g_locations[p_location];
	}

	return NULL;
}

// FUNCTION: LEGO1 0x10055740
MxS32 LegoNavController::GetNumLocations()
{
	return sizeOfArray(g_locations);
}

// FUNCTION: LEGO1 0x10055750
MxResult LegoNavController::ProcessJoystickInput(MxBool& p_und)
{
	LegoOmni* instance = LegoOmni::GetInstance();

	if (instance->GetInputManager()) {
		MxS32 joystickX;
		MxS32 joystickY;
		MxS32 povPosition;

		if (instance->GetInputManager()
				->GetJoystickState((MxU32*) &joystickX, (MxU32*) &joystickY, (MxU32*) &povPosition) != FAILURE) {
			MxU32 yVal = (joystickY * m_vMax) / 100;
			MxU32 xVal = (joystickX * m_hMax) / 100;

			if (joystickX <= 45 || joystickX >= 55 || joystickY <= 45 || joystickY >= 55) {
				m_targetLinearVel = CalculateNewTargetVel(m_vMax - yVal, m_vMax / 2, m_maxLinearVel);
				m_linearAccel = CalculateNewAccel(m_vMax - yVal, m_vMax / 2, m_maxLinearAccel, (int) m_minLinearAccel);
				m_targetRotationalVel = CalculateNewTargetVel(xVal, m_hMax / 2, m_maxRotationalVel);
				m_rotationalAccel =
					CalculateNewAccel(xVal, m_hMax / 2, m_maxRotationalAccel, (int) m_minRotationalAccel);
			}
			else {
				m_targetRotationalVel = 0.0;
				m_targetLinearVel = 0.0;
				m_linearAccel = m_maxLinearDeccel;
				m_rotationalAccel = m_maxRotationalDeccel;
			}

			if (povPosition >= 0) {
				LegoWorld* world = CurrentWorld();

				if (world && world->GetCameraController()) {
					world->GetCameraController()->FUN_10012320(DTOR(povPosition));
					p_und = TRUE;
				}
			}

			return SUCCESS;
		}
	}

	return FAILURE;
}

// FUNCTION: LEGO1 0x100558b0
MxResult LegoNavController::ProcessKeyboardInput()
{
	MxBool skipRotationVelAndAccelCalc = FALSE;
	MxBool skipLinearVelAndAccelCalc = FALSE;
	LegoInputManager* inputManager = LegoOmni::GetInstance()->GetInputManager();
	MxU32 keyFlags;

	if (inputManager == NULL || inputManager->GetNavigationKeyStates(keyFlags) == FAILURE) {
		return FAILURE;
	}

	if (keyFlags == 0) {
		if (m_isAccelerating) {
			m_targetRotationalVel = 0.0;
			m_targetLinearVel = 0.0;
			m_rotationalAccel = m_maxRotationalDeccel;
			m_linearAccel = m_maxLinearDeccel;
			m_isAccelerating = FALSE;
		}

		return FAILURE;
	}

	m_isAccelerating = TRUE;

	MxS32 hMax;
	switch (keyFlags & LegoInputManager::c_leftOrRight) {
	case LegoInputManager::c_left:
		hMax = 0;
		break;
	case LegoInputManager::c_right:
		hMax = m_hMax;
		break;
	default:
		m_targetRotationalVel = 0.0;
		m_rotationalAccel = m_maxRotationalDeccel;
		skipRotationVelAndAccelCalc = TRUE;
		break;
	}

	MxS32 vMax;
	switch (keyFlags & LegoInputManager::c_upOrDown) {
	case LegoInputManager::c_up:
		vMax = 0;
		break;
	case LegoInputManager::c_down:
		vMax = m_vMax;
		break;
	default:
		m_targetLinearVel = 0.0;
		m_linearAccel = m_maxLinearDeccel;
		skipLinearVelAndAccelCalc = TRUE;
		break;
	}

	MxFloat maxAccelDivisor = keyFlags & LegoInputManager::c_ctrl ? 1.0f : 4.0f;
	MxFloat minAccelDivisor = keyFlags & LegoInputManager::c_ctrl ? 1.0f : 2.0f;

	if (!skipRotationVelAndAccelCalc) {
		m_targetRotationalVel = CalculateNewTargetVel(hMax, m_hMax / 2, m_maxRotationalVel);
		m_rotationalAccel = CalculateNewAccel(
			hMax,
			m_hMax / 2,
			m_maxRotationalAccel / maxAccelDivisor,
			(int) (m_minRotationalAccel / minAccelDivisor)
		);
	}

	if (!skipLinearVelAndAccelCalc) {
		m_targetLinearVel = CalculateNewTargetVel(m_vMax - vMax, m_vMax / 2, m_maxLinearVel);
		m_linearAccel = CalculateNewAccel(
			m_vMax - vMax,
			m_vMax / 2,
			m_maxLinearAccel / maxAccelDivisor,
			(int) (m_minLinearAccel / minAccelDivisor)
		);
	}

	return SUCCESS;
}

// FUNCTION: LEGO1 0x10055a60
// FUNCTION: BETA10 0x1009c712
MxLong LegoNavController::Notify(MxParam& p_param)
{
	Lego()->RunLuaCallback("NavController_Notify", this, (LegoEventNotificationParam&)p_param);

	return 0;
}

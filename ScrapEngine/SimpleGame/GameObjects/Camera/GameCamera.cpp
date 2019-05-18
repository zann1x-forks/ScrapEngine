#include "GameCamera.h"
#include "Engine/Debug/DebugLog.h"
#include "../Camera/GameCamera.h"

GameCamera::GameCamera(ScrapEngine::Input::InputManager* CreatedInputManagerf, ScrapEngine::Render::Camera* input_GameCameraRef, TestGameObject* input_GameObjectRef) :
	GameCameraRef(input_GameCameraRef), SGameObject("Camera-Controller Object"), InputManagerRef(CreatedInputManagerf), GameObjectRef(input_GameObjectRef)
{
	GameCameraRef->set_max_render_distance(10000);
}

void GameCamera::game_start()
{

}

void GameCamera::game_update(float time)
{
	ScrapEngine::Input::mouse_location mouse = InputManagerRef->get_last_mouse_location();
	
	GameCameraRef->process_mouse_movement((float)mouse.xpos, (float)mouse.ypos, true);

	cameraSpeed = 10.f * time;
	if (InputManagerRef->get_keyboard_key_pressed(KEYBOARD_KEY_W)) {
		GameCameraRef->set_camera_location(GameCameraRef->get_camera_location() + (cameraSpeed * GameCameraRef->get_camera_front()));
	}
	if (InputManagerRef->get_keyboard_key_pressed(KEYBOARD_KEY_S)) {
		GameCameraRef->set_camera_location(GameCameraRef->get_camera_location() - (cameraSpeed * GameCameraRef->get_camera_front()));
	}
	if (InputManagerRef->get_keyboard_key_pressed(KEYBOARD_KEY_D)) {
		GameCameraRef->set_camera_location(GameCameraRef->get_camera_location() + (cameraSpeed * (glm::normalize(glm::cross(GameCameraRef->get_camera_front(), GameCameraRef->get_camera_up())))));
	}
	if (InputManagerRef->get_keyboard_key_pressed(KEYBOARD_KEY_A)) {
		GameCameraRef->set_camera_location(GameCameraRef->get_camera_location() - (cameraSpeed * (glm::normalize(glm::cross(GameCameraRef->get_camera_front(), GameCameraRef->get_camera_up())))));
	}
}


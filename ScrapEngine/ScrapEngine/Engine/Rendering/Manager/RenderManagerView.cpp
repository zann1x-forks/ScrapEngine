#include <Engine/Rendering/Manager/RenderManagerView.h>

ScrapEngine::Render::RenderManagerView::
RenderManagerView(RenderManager* input_render_manager_ref)
	: render_manager_ref_(input_render_manager_ref)
{
}

ScrapEngine::Render::GameWindow* ScrapEngine::Render::RenderManagerView::get_game_window() const
{
	return render_manager_ref_->get_game_window();
}

ScrapEngine::Render::Camera* ScrapEngine::Render::RenderManagerView::get_default_render_camera() const
{
	return render_manager_ref_->get_default_render_camera();
}

ScrapEngine::Render::Camera* ScrapEngine::Render::RenderManagerView::get_current_render_camera() const
{
	return render_manager_ref_->get_render_camera();
}

void ScrapEngine::Render::RenderManagerView::set_render_camera(Camera* new_camera) const
{
	render_manager_ref_->set_render_camera(new_camera);
}

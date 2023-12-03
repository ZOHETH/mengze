#pragma once
#include <glm/glm.hpp>
#include "glm/ext/matrix_clip_space.hpp"

#include "rendering/camera.h"
#include "rendering/renderer.h"
#include "polygon.h"
#include "geometry.h"
#include "core/application.h"

namespace mengze
{

	glm::vec3 simple_shading(const glm::vec3& normal);

	class Rasterizer : public Renderer
	{
	public:
		explicit Rasterizer(Camera& camera, Geometry& geometry);


		void on_update(float ts) override;

		void on_resize(uint32_t width, uint32_t height) override;


		void render() override;

		virtual  void render_triangle() = 0;

		Triangle get_screen_triangle(uint32_t index) const;

		Triangle get_world_triangle(uint32_t index) const;

		bool check_screen_triangle(const Triangle& triangle) const;

		uint32_t get_triangle_count() const { return num_triangles_; }
		float get_vertex_transform_time() const { return vertex_transform_time_; }
		float get_rasterization_time() const { return rasterization_time_; }

	protected:
		uint32_t num_triangles_;
		Camera& camera_;
		Geometry& geometry_;
		std::vector<glm::vec3> screen_vertices_;
		float* depth_buffer_{ nullptr };

	private:
		glm::vec3 to_screen_space(const glm::vec3& vertex) const;

	private:
		float vertex_transform_time_{};
		float rasterization_time_{};
	};
}

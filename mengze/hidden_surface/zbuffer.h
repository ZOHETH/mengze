#pragma once
//#include <ctpl_stl.h>
#include <glm/glm.hpp>

#include "rendering/camera.h"
#include "rasterizer.h"

#include "geometry.h"
#include "core/application.h"


namespace mengze
{

	class ZbufferRasterizer : public Rasterizer
	{
	public:
		explicit ZbufferRasterizer(Camera& camera, Geometry& geometry) : Rasterizer(camera, geometry)
		{
		}

		/*void render_triangle() override
		{
			ctpl::thread_pool pool(std::thread::hardware_concurrency());

			std::vector<std::future<void>> futures;

			for (uint32_t i = 0; i < num_triangles_; ++i)
			{
				futures.push_back(pool.push([this, i](int) {
					const auto& triangle = get_screen_triangle(i);
					if (!check_screen_triangle(triangle))
						return; 

					const auto& world_triangle = get_world_triangle(i);
					rasterize_triangle(triangle, world_triangle);
					}));
			}

			for (auto& f : futures) {
				f.get();
			}

		}*/

		void render_triangle() override
		{

			for (uint32_t i = 0; i < num_triangles_; ++i)
			{
				const auto& triangle = get_screen_triangle(i);
				if (!check_screen_triangle(triangle))
					continue;
				const auto& world_triangle = get_world_triangle(i);

				rasterize_triangle(triangle, world_triangle);
			}

		}
	private:
		void rasterize_triangle(const Triangle& triangle, const Triangle& world_triangle) {
			const glm::vec3& v0 = triangle.vertices[0];
			const glm::vec3& v1 = triangle.vertices[1];
			const glm::vec3& v2 = triangle.vertices[2];


			float min_x = std::min({ v0.x, v1.x, v2.x });
			float max_x = std::max({ v0.x, v1.x, v2.x });
			float min_y = std::min({ v0.y, v1.y, v2.y });
			float max_y = std::max({ v0.y, v1.y, v2.y });

			float area = Rasterizer::edge_func(v0, v1, v2);

			glm::vec3 normal = glm::normalize(glm::cross(world_triangle.vertices[1] - world_triangle.vertices[0], world_triangle.vertices[2] - world_triangle.vertices[0]));
			glm::vec3 color = simple_shading(normal);

			for (uint32_t y = min_y; y <= max_y; ++y) {
				for (uint32_t x = min_x; x <= max_x; ++x) {
					glm::vec3 p(x, y, 0);

					float w0 = edge_func(v1, v2, p);
					float w1 = edge_func(v2, v0, p);
					float w2 = edge_func(v0, v1, p);

					if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
						w0 /= area;
						w1 /= area;
						w2 /= area;

						float depth = w0 * v0.z + w1 * v1.z + w2 * v2.z;

						if (depth < depth_buffer_[y * get_width() + x])
						{
							set_pixel(x, y, color);
							depth_buffer_[y * get_width() + x] = depth;
						}
					}
				}
			}
		}

	};
}

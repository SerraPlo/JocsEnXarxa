#pragma once
#include <string>
#include <glm/detail/type_vec3.hpp>

namespace SerraPlo {
	
	class BaseLight {
	public:
		std::string name;
		glm::vec3 color{ 0.0f, 0.0f, 0.0f };
		float ambientIntensity{ 0.0f };
		float diffuseIntensity{ 0.0f };
		explicit BaseLight() = default;
	};

	class DirectionalLight : public BaseLight {
	public:
		glm::vec3 direction{ 0.0f, 0.0f, 0.0f };
		explicit DirectionalLight() = default;
	};

	struct LightAttenuation {
		float constant;
		float linear;
		float exp;
	};

	class PointLight : public BaseLight {
	public:
		glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
		LightAttenuation Attenuation{ 0.0f, 0.0f, 0.0f };
		explicit PointLight() = default;
	};

	class SpotLight : public PointLight {
	public:
		glm::vec3 direction{ 0.0f, 0.0f, 0.0f };
		float cutoff{ 0.0f };
		explicit SpotLight() = default;
	};

	#define COLOR_WHITE glm::vec3(1.0f, 1.0f, 1.0f)
	#define COLOR_RED	glm::vec3(1.0f, 0.0f, 0.0f)
	#define COLOR_GREEN glm::vec3(0.0f, 1.0f, 0.0f)
	#define COLOR_CYAN	glm::vec3(0.0f, 1.0f, 1.0f)
	#define COLOR_BLUE	glm::vec3(0.0f, 0.0f, 1.0f)

}

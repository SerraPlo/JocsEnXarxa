#pragma once
#include <string>
#include <glm/detail/type_vec3.hpp>
#include <glm/detail/type_vec2.hpp>
#include <vector>

namespace SerraPlo {

	#define COLOR_WHITE glm::vec3(1.0f, 1.0f, 1.0f)
	#define COLOR_RED	glm::vec3(1.0f, 0.0f, 0.0f)
	#define COLOR_GREEN glm::vec3(0.0f, 1.0f, 0.0f)
	#define COLOR_CYAN	glm::vec3(0.0f, 1.0f, 1.0f)
	#define COLOR_BLUE	glm::vec3(0.0f, 0.0f, 1.0f)
	
	class BaseLight {
	public:
		std::string name;
		/*glm::vec3 color{ 0.0f, 0.0f, 0.0f };
		float ambientIntensity{ 0.0f };
		float diffuseIntensity{ 0.0f };*/
		GLuint vao;
		int elements;
		explicit BaseLight() {
			using namespace glm;
			using namespace std;

			const float pi = 3.1415926535897932384626433832795f;
			const float _2pi = 2.0f * pi;

			int radius = 1;
			int slices = 32;
			int stacks = 32;
			elements = (slices * stacks + slices) * 6;

			vector<vec3> positions;
			vector<vec3> normals;
			vector<vec2> textureCoords;

			for (int i = 0; i <= stacks; ++i) {
				// V texture coordinate.
				float V = i / (float)stacks;
				float phi = V * pi;

				for (int j = 0; j <= slices; ++j) {
					// U texture coordinate.
					float U = j / (float)slices;
					float theta = U * _2pi;

					float X = cos(theta) * sin(phi);
					float Y = cos(phi);
					float Z = sin(theta) * sin(phi);

					positions.push_back(vec3(X, Y, Z) * vec3(radius, radius, radius));
					normals.push_back(vec3(X, Y, Z));
					textureCoords.push_back(vec2(U, V));
				}
			}
			// Now generate the index buffer
			vector<GLuint> indicies;

			for (int i = 0; i < slices * stacks + slices; ++i) {
				indicies.push_back(i);
				indicies.push_back(i + slices + 1);
				indicies.push_back(i + slices);

				indicies.push_back(i + slices + 1);
				indicies.push_back(i);
				indicies.push_back(i + 1);
			}
			
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			GLuint vbos[4];
			glGenBuffers(4, vbos);

#define POSITION_ATTRIBUTE 0
#define NORMAL_ATTRIBUTE  1
#define TEXCOORD0_ATTRIBUTE  2

			glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
			glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec3), positions.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
			glEnableVertexAttribArray(POSITION_ATTRIBUTE);

			glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
			glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), normals.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_TRUE, 0, nullptr);
			glEnableVertexAttribArray(NORMAL_ATTRIBUTE);

			glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
			glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(vec2), textureCoords.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(TEXCOORD0_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
			glEnableVertexAttribArray(TEXCOORD0_ATTRIBUTE);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[3]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(GLuint), indicies.data(), GL_STATIC_DRAW);

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	};

	/*class DirectionalLight : public BaseLight {
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
	#define COLOR_BLUE	glm::vec3(0.0f, 0.0f, 1.0f)*/

};
#pragma once
#include <sstream>
#include <fstream>
#include "../ew/ewMath/mat4.h"

namespace librar {

	struct Vertex {
		float x, y, z;
		float u, v;
	};

	std::string loadShaderSourceFromFile(const std::string& filePath);
	unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices);

	class Shader {
	public:
		Shader(const std::string& vertexShader, const std::string& fragmentShader);
		void use();
		void setInt(const std::string& name, int v) const;
		void setFloat(const std::string& name, float v) const;
		void setVec2(const std::string& name, float x, float y) const;
		void setVec3(const std::string& name, float x, float y, float z) const;
		void setVec4(const std::string& name, float x, float y, float z, float w) const;
		void setVec3Array(const std::string& name, int arrLength, float arr[][3]) const;
		void Shader::setMat4(const std::string& name, const ew::Mat4& v) const;
	private:
		unsigned int m_id; //OpenGL program handle
	};
}

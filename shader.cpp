#include "shader.h"

//Private functions
std::string Shader::loadShaderSource(char* fileName) {
	std::string temp = "";
	std::string src = "";

	std::ifstream in_file;

	//Vertex
	in_file.open(fileName);

	if (in_file.is_open())
		while (std::getline(in_file, temp))
			src += temp + "\n";
	else std::cout << "ERROR! SHADER.CPP/LOADSHADERSOURCE : CANNOT_OPEN_FILE: "<< fileName << std::endl;

	in_file.close();

	//std::string versionNr = std::to_string(versionMajor) + std::to_string(versionMinor) + "0";
	//src.replace(src.find("#version"), 12, "#version "+versionNr);

	return src;
}

GLuint Shader::loadShader(GLenum type, char* fileName) {
	char infoLog[512];
	GLint success;

	GLuint shader = glCreateShader(type);
	std::string str_src = this->loadShaderSource(fileName);
	const GLchar* src = str_src.c_str();
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR! SHADER.CPP/LOADSHADER : CANNOT_COMPILE_SHADER: "<< fileName << std::endl << infoLog << std::endl;
	}

	return shader;
}

void Shader::linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader) {
	char infoLog[512];
	GLint success;

	this->id = glCreateProgram();
	glAttachShader(this->id, vertexShader);
	if (geometryShader) glAttachShader(this->id, geometryShader);
	glAttachShader(this->id, fragmentShader);
	glLinkProgram(this->id);

	glGetProgramiv(this->id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->id, 512, NULL, infoLog);
		std::cout << "ERROR! SHADER.CPP/LINKPROGRAM : CANNOT_LINK_PROGRAM" << std::endl << infoLog << std::endl;
	}

	glUseProgram(0);
}

//Ctor & Dtor
Shader::Shader(const int versionMajor, const int versionMinor, char* vertexFile, char* fragmentFile, char* geometryFile) : versionMajor(versionMajor), versionMinor(versionMinor) {
	GLuint vertexShader = 0;
	GLuint fragmentShader = 0;
	GLuint geometryShader = 0;

	vertexShader = loadShader(GL_VERTEX_SHADER, vertexFile);
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentFile);
	if (geometryFile != "") geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryFile);

	this->linkProgram(vertexShader, geometryShader, fragmentShader);

	//End
	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);

}

Shader::~Shader() {
	glDeleteProgram(this->id);
}

//Set uniform functions
void Shader::use() {
	glUseProgram(this->id);
}

void Shader::unUse() {
	glUseProgram(0);
}

void Shader::set1i(GLint value, const GLchar* name) {
	this->use();
	glUniform1i(glGetUniformLocation(this->id, name), value);
	this->unUse();
}

void Shader::set1f(GLfloat value, const GLchar* name) {
	this->use();
	glUniform1f(glGetUniformLocation(this->id, name), value);
	this->unUse();
}

void Shader::setVec2f(glm::fvec2 value, const GLchar* name) {
	this->use();
	glUniform2fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
	this->unUse();
}

void Shader::setVec3f(glm::fvec3 value, const GLchar* name) {
	this->use();
	glUniform3fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
	this->unUse();
}

void Shader::setVec4f(glm::fvec4 value, const GLchar* name) {
	this->use();
	glUniform4fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
	this->unUse();
}

void Shader::setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose) {
	this->use();
	glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));
	this->unUse();
}

void Shader::setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose) {
	this->use();
	glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));
	this->unUse();
}
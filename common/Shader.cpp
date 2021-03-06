//#include "stdafx.h"

#include <vector>
#include <sstream>
#include <fstream>

#include "Shader.h"

static std::vector<CShader*> vecShaders;

GLuint CreateShader(GLenum type, const char* pPath)
{
	GLuint nShaderID = glCreateShader(type);
	std::ifstream ShaderStream;
	ShaderStream.open(pPath, std::ios::out);
	std::string sShaderCode = "";
	if (ShaderStream.is_open())
	{
		std::string sLine = "";
		while (getline(ShaderStream, sLine))
		{
			sShaderCode += "\n" + sLine;
		}
		ShaderStream.close();
	}
	else
	{
		printf("Impossible to open %s. Are you in the right directory?\n", pPath);
		getchar();
		return 0;
	}

	GLint nResult = GL_FALSE;
	int nInfoLogLength;

	char const * pSourcePointer = sShaderCode.c_str();
	glShaderSource(nShaderID, 1, &pSourcePointer, NULL);
	glCompileShader(nShaderID);

	// Check Vertex Shader
	glGetShaderiv(nShaderID, GL_COMPILE_STATUS, &nResult);
	glGetShaderiv(nShaderID, GL_INFO_LOG_LENGTH, &nInfoLogLength);
	if (nInfoLogLength > 1)
	{
		std::vector<char> vecShaderErrorMessage(nInfoLogLength + 1);
		glGetShaderInfoLog(nShaderID, nInfoLogLength, NULL, &vecShaderErrorMessage[0]);
		printf("%s\n", &vecShaderErrorMessage[0]);
	}
	return nShaderID;
}

GLuint CreateShaderProgram(const char* pVertex_file_path, const char* pTessControl_file_path, const char* pTessEvaluation_file_path, const char* pGeometry_file_path, const char* pFragment_file_path, const char* pCompute_file_path)
{
	GLuint nVertexShaderID;
	GLuint nTessControlShaderID;
	GLuint nTessEvaluationShaderID;
	GLuint nGeometryShaderID;
	GLuint nFragmentShaderID;
	GLuint nComputeShaderID;

	// Create the shaders
	if (pVertex_file_path) { nVertexShaderID = CreateShader(GL_VERTEX_SHADER, pVertex_file_path); }
	if (pTessControl_file_path) { nTessControlShaderID = CreateShader(GL_TESS_CONTROL_SHADER, pTessControl_file_path); }
	if (pTessEvaluation_file_path) { nTessEvaluationShaderID = CreateShader(GL_TESS_EVALUATION_SHADER, pTessEvaluation_file_path); }
	if (pGeometry_file_path) { nGeometryShaderID = CreateShader(GL_GEOMETRY_SHADER, pGeometry_file_path); }
	if (pFragment_file_path) { nFragmentShaderID = CreateShader(GL_FRAGMENT_SHADER, pFragment_file_path); }
	if (pCompute_file_path) { nComputeShaderID = CreateShader(GL_COMPUTE_SHADER, pCompute_file_path); }

	// Link the program
	printf("Linking program\n");
	GLuint nProgramID = glCreateProgram();
	if (pVertex_file_path) { glAttachShader(nProgramID, nVertexShaderID); }
	if (pTessControl_file_path) { glAttachShader(nProgramID, nTessControlShaderID); }
	if (pTessEvaluation_file_path) { glAttachShader(nProgramID, nTessEvaluationShaderID); }
	if (pGeometry_file_path) { glAttachShader(nProgramID, nGeometryShaderID); }
	if (pFragment_file_path) { glAttachShader(nProgramID, nFragmentShaderID); }
	if (pCompute_file_path) { glAttachShader(nProgramID, nComputeShaderID); }
	glLinkProgram(nProgramID);

	// Check the program
	GLint nResult = GL_FALSE;
	int nInfoLogLength;

	glGetProgramiv(nProgramID, GL_LINK_STATUS, &nResult);
	glGetProgramiv(nProgramID, GL_INFO_LOG_LENGTH, &nInfoLogLength);
	if (nInfoLogLength > 1)
	{
		std::vector<char> vecProgramErrorMessage(nInfoLogLength + 1);
		glGetProgramInfoLog(nProgramID, nInfoLogLength, NULL, &vecProgramErrorMessage[0]);
		printf("%s\n", &vecProgramErrorMessage[0]);
		return 0;
	}

	if (pVertex_file_path) { glDetachShader(nProgramID, nVertexShaderID); }
	if (pTessControl_file_path) { glDetachShader(nProgramID, nTessControlShaderID); }
	if (pTessEvaluation_file_path) { glDetachShader(nProgramID, nTessEvaluationShaderID); }
	if (pGeometry_file_path) { glDetachShader(nProgramID, nGeometryShaderID); }
	if (pFragment_file_path) { glDetachShader(nProgramID, nFragmentShaderID); }
	//if (pCompute_file_path) { glDetachShader(nProgramID, nComputeShaderID); }

	return nProgramID;
}

CShader::CShader(int ID)
{
	this->ID = ID;
}

CShader::~CShader()
{
    vecShaders.clear();
}

GLuint CShader::getID()
{
	return this->ID;
}

std::vector<CShader*> CShader::getShaders()
{
	return vecShaders;
}

CShader* CShader::createShaderProgram(const char* pVertex_file_path, const char* pTessControl_file_path, const char* pTessEvaluation_file_path, const char* pGeometry_file_path, const char* pFragment_file_path, const char* pCompute_file_path)
{
	GLuint nProgramID = CreateShaderProgram(pVertex_file_path, pTessControl_file_path, pTessEvaluation_file_path, pGeometry_file_path, pFragment_file_path, pCompute_file_path);
	CShader* pShader = new CShader(nProgramID);
	vecShaders.push_back(pShader);
	return pShader;
}

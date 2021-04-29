#include"ResourceManager.h"
#include"../Renderer/ShaderProgram.h"
#include<sstream>
#include<fstream>
#include<iostream>

ResourceManager::ResourceManager(const std::string& executablePath)
{
	size_t found = executablePath.find_last_of("/\\");
  	m_path = executablePath.substr(0, found);
}

std::string ResourceManager::getFileString(const std::string& relativefilePath) const
{
	std::ifstream f;
	f.open(m_path + "/" + relativefilePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open())
	{
		std::cerr << "Failed to open file " << relativefilePath << std::endl;
		return std::string{};
	}

	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexShtring = getFileString(vertexPath);
	if (vertexShtring.empty())
	{
		std::cerr << "No vertex shader" << std::endl;
		return nullptr;
	}

	std::string fragmentShtring = getFileString(fragmentPath);
	if (fragmentShtring.empty())
	{
		std::cerr << "No fragment shader" << std::endl;
		return nullptr;
	}
	std::shared_ptr<Renderer::ShaderProgram>& newShader = m_shaderPrograms.emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexShtring, fragmentShtring)).first ->second;
	if (newShader->isCompiled())
	{
		return newShader;
	}

	std::cerr << "Can't load shader program:\n" << "Vertex: " << vertexPath << "\n" << "Fragment: " << fragmentPath << std::endl;
	return nullptr;
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName)
{
	ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
	if (it != m_shaderPrograms.end())
	{
		return it->second;
	}
	std::cerr << "Can't find the shader program: " << shaderName << std::endl;
	return nullptr;
}
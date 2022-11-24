#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Utils.h"
#include <iostream>

using namespace std;

glm::vec3 Utils::Vec3fFromStream(std::istream& issLine)
{
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 Utils::Vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

std::shared_ptr<MeshModel> Utils::LoadMeshModel(const std::string& filePath)
{
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::ifstream ifile(filePath.c_str());

	glm::vec3 Vertix; // curr vertix 
	std::vector<glm::vec2> TextureCoordinates;

	float xMax = 0, yMax = 0, zMax = 0, xMin = 0, yMin = 0, zMin = 0, Max = 0, Min = 0;

	// while not end of file
	while (!ifile.eof())
	{
		// get line
		std::string curLine;
		std::getline(ifile, curLine);

		// read the type of the line
		std::istringstream issLine(curLine);
		std::string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v")
		{
			//vertices.push_back(Utils::Vec3fFromStream(issLine));
			//cout << issLine.str() << endl;

			Vertix = Utils::Vec3fFromStream(issLine);
			xMax = max(xMax, Vertix.x);
			yMax = max(yMax, Vertix.y);
			zMax = max(zMax, Vertix.z);

			xMin = min(xMin, Vertix.x);
			yMin = min(yMin, Vertix.y);
			zMin = min(zMin, Vertix.z);

			vertices.push_back(Vertix);
		}
		else if (lineType == "vn")
		{
			normals.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vt")
		{
			// TODO: Handle texture coordinates
			TextureCoordinates.push_back(Utils::Vec2fFromStream(issLine));
		}
		else if (lineType == "f")
		{
			faces.push_back(Face(issLine));
			//cout << issLine.str() << endl;
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			std::cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}
	Max = max(zMax, max(xMax, yMax));
	//Min = min(zMin, min(xMin, yMin));

	glm::mat4x4 scaleMat = glm::scale(glm::vec3(500 / Max, 500 / Max, 500 / Max));
	glm::mat4x4 translationMat = glm::translate(glm::vec3(abs(xMin), abs(yMin), abs(zMin)));

	//return std::make_shared<MeshModel>(faces, vertices, normals, Utils::GetFileName(filePath), scaleMat * translationMat, xMax, yMax, zMax, xMin, yMin, zMin, TextureCoordinates);
	return std::make_shared<MeshModel>(faces, vertices, normals, Utils::GetFileName(filePath), scaleMat * translationMat);

	// return std::make_shared<MeshModel>(faces, vertices, normals, Utils::GetFileName(filePath));
}

std::string Utils::GetFileName(const std::string& filePath)
{
	if (filePath.empty()) {
		return {};
	}

	auto len = filePath.length();
	auto index = filePath.find_last_of("/\\");

	if (index == std::string::npos) {
		return filePath;
	}

	if (index + 1 >= len) {

		len--;
		index = filePath.substr(0, len).find_last_of("/\\");

		if (len == 0) {
			return filePath;
		}

		if (index == 0) {
			return filePath.substr(1, len - 1);
		}

		if (index == std::string::npos) {
			return filePath.substr(0, len);
		}

		return filePath.substr(index + 1, len - index - 1);
	}

	return filePath.substr(index + 1, len - index);
}
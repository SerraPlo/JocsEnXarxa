#pragma once
#include <glm/glm.hpp>
#include <string>
#include<iostream>
#include<fstream>

struct Circle {
	glm::vec2 c;
	float r;
};
struct Box {
	glm::vec2 v[4];
	glm::vec2 axis[2];
	Circle ahorro;
};

class ColManager{
public:
	Box* boxs;
	Circle* circles;
	int nBoxs;
	int nCircles;
	ColManager() {};
	~ColManager() { delete boxs, delete circles;};
	float projectionScalar(glm::vec2 a, glm::vec2 b) {
		float projection = ((b.x*a.x) + (b.y*a.y)) / (a.x*a.x + a.y*a.y);
		return (projection*a.x*a.x) + (projection *a.y*a.y);
	}
	void InitStructures(std::string path) {
		std::ifstream myReadFile;
		myReadFile.open(path);
		if (myReadFile.is_open()) {
			char actual;
			std::string nBox;
			std::string nCircle;
			
			myReadFile.get(actual);
			while (actual!=' '){
				nBox += actual;
				myReadFile.get(actual);
			}myReadFile.get(actual);
			while (actual != '\n') {
				nCircle += actual;
				myReadFile.get(actual);
			}myReadFile.get(actual);
			
			nBoxs = atoi(nBox.c_str());
			nCircles = atoi(nCircle.c_str());
			boxs = new Box[nBoxs];
			circles = new Circle[nCircles];

			std::string temp = "";
			for (int i = 0; i < nBoxs; i++) {
				for (int j = 0; j < 4; j++) {
					
					while (actual != '/') {
						temp += actual;
						myReadFile.get(actual);
					}myReadFile.get(actual); 
					boxs[i].v[j].x = atof(temp.c_str());
					temp = "";

					while (actual != '\n') {
						temp += actual;
						myReadFile.get(actual);
					}myReadFile.get(actual);
					boxs[i].v[j].y = atof(temp.c_str());
					temp = "";
				}
			}
			for (int i = 0; i <nCircles; i++) {
				//center
				while (actual != '/') {
					temp += actual;
					myReadFile.get(actual);
				}myReadFile.get(actual);
				circles[i].c.x = atof(temp.c_str());
				temp = "";
				while (actual != '/') {
					temp += actual;
					myReadFile.get(actual);
				}myReadFile.get(actual);
				circles[i].c.y = atof(temp.c_str());
				temp = "";
				//rad
				glm::vec2 temporal;
				while (actual != '/') {
					temp += actual;
					myReadFile.get(actual);
				}myReadFile.get(actual);
				temporal.x = atof(temp.c_str());
				temp = "";
				while (actual != '\n' && !myReadFile.eof()) {
					temp += actual;
					myReadFile.get(actual);
				}myReadFile.get(actual);
				temporal.y = atof(temp.c_str());
				temporal.x *= -1;
				temporal.y *= -1;
				circles[i].c.x *= -1;
				circles[i].c.y *= -1;
				temp = "";
				circles[i].r = glm::length(temporal - circles[i].c);
			}
			
			for (int i = 0; i <nBoxs; i++) {
				for (int j = 0; j < 4; j++) {
					boxs[i].v[j].y *= -1;
					boxs[i].v[j].x *= -1;
					//std::cout << i<<": (" << boxs[i].v[j].x << "," << boxs[i].v[j].y << ")" << std::endl;
				}
				boxs[i].axis[0] = boxs[i].v[1] - boxs[i].v[0];
				boxs[i].axis[1] = boxs[i].v[2] - boxs[i].v[1];
				std::cout << i << ": A1 (" << boxs[i].axis[0].x << "," << boxs[i].axis[0].y << "), A2 (" << boxs[i].axis[1].x << "," << boxs[i].axis[1].y <<")"<< std::endl;
			}
		}
		myReadFile.close();

	}
	
	void CalculateCollision(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec2 v4) {
		for (int i = 0; i < nBoxs; i++) {
			glm::vec2 axis[4];
			axis[0] = boxs[i].axis[0];
			axis[1] = boxs[i].axis[1];
			axis[2] = v2 - v1;
			axis[3] = v3 - v2;
			bool colliding = true;
			//box axis
			for (int j = 0; j < 4; j++) {
				float bMax = 0.0f; float oMax = 0.0f;
				float bMin = 0.0f; float oMin = 0.0f;
				if (colliding) {
					float temp[8];
					temp[0] = projectionScalar(axis[j], v1);
					temp[1] = projectionScalar(axis[j], v2);
					temp[2] = projectionScalar(axis[j], v3);
					temp[3] = projectionScalar(axis[j], v4);
					temp[4] = projectionScalar(axis[j], boxs[i].v[0]);
					temp[5] = projectionScalar(axis[j], boxs[i].v[1]);
					temp[6] = projectionScalar(axis[j], boxs[i].v[2]);
					temp[7] = projectionScalar(axis[j], boxs[i].v[3]);
					oMax = temp[0]; bMax = temp[4];
					oMin = temp[0]; bMin = temp[4];
					for (int k = 1; k < 4; k++) { if (temp[k] > oMax) oMax = temp[k]; if (temp[k] < oMin) oMin = temp[k];}
					for (int k = 5; k < 8; k++) { if (temp[k] > bMax) bMax = temp[k]; if (temp[k] < bMin) bMin = temp[k];}
					if (!(oMin <= bMax && oMin >= bMin || bMin <= oMax && bMin>=oMin)) {
						colliding = false;
					}
				}
			}
			if (colliding) std::cout << i << std::endl;
		}
	}
};


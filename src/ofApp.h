#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class Shape {
public:
	Shape() {}
	virtual void draw() {}
	virtual bool inside(glm::vec3 p) {
		return true;
	}
	glm::mat4 getMatrix() {
		glm::mat4 trans = glm::translate(glm::mat4(1.0), glm::vec3(pos));
		glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 0, 1));
		glm::mat4 scal = glm::scale(glm::vec3(scale, scale, scale));
		return (trans * rot * scal);
	}
	glm::vec3 pos;
	float rotation = 0.0;    // degrees 
	float scale = 0;
	ofColor color;

	vector<glm::vec3> verts;
};

class TriangleShape : public Shape {
public:
	TriangleShape() {}
	TriangleShape(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
		verts.push_back(p1);
		verts.push_back(p2);
		verts.push_back(p3);
	}
	void draw();

	bool inside(glm::vec3 p) {

		glm::vec3 transP = glm::vec3(glm::inverse(getMatrix()) * glm::vec4(p, 1));
		if (inside(transP, verts[0], verts[1], verts[2]))
			return true;
		else
			return false;
	}

	bool inside(glm::vec3 p, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
		glm::vec3 v1 = glm::normalize(p1 - p);
		glm::vec3 v2 = glm::normalize(p2 - p);
		glm::vec3 v3 = glm::normalize(p3 - p);
		float a1 = glm::orientedAngle(v1, v2, glm::vec3(0, 0, 1));
		float a2 = glm::orientedAngle(v2, v3, glm::vec3(0, 0, 1));
		float a3 = glm::orientedAngle(v3, v1, glm::vec3(0, 0, 1));
		if (a1 < 0 && a2 < 0 && a3 < 0) return true;
		else return false;
	}
	//------
};

//Player class
class Player : public TriangleShape {
public:

	Player() {}
	Player(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) : TriangleShape(p1, p2, p3)
	{}

	void draw();

	bool inside(glm::vec3 p)
	{
		if (showImage)
		{
			glm::vec3 transP = glm::vec3(glm::inverse(getMatrix()) * glm::vec4(p, 1));
			if (((-(image.getWidth() / 2) <= transP.x) && (transP.x <= image.getWidth() / 2)) &&
				((-(image.getHeight() / 2) <= transP.y) && (transP.y <= image.getHeight() / 2)))
				return true;
			else
				return false;
		}
		else
			return TriangleShape::inside(p);
	}
	//header function
	glm::vec3 header()
	{
		glm::vec3 d = glm::vec3(-cos(ofDegToRad(rotation + 90)), -sin(ofDegToRad(rotation + 90)), 0);
		return glm::normalize(d);
	}


	ofImage image;
	bool showImage = false;
	float speed = 0;
};


class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	TriangleShape tri;
	Player player;

	bool selected = false;
	bool control = false;
	bool bInRotate = false;
	int moveX;
	glm::vec3 lastMouse;
	glm::vec3 delta;

	int headerLength = 75;
	float rotDeg = 4.0;
	map<int, bool> keymap;

	bool bHide;
	ofxFloatSlider sp;
	ofxFloatSlider sc;
	ofxToggle shImage;
	ofxToggle shLine;

	ofxPanel gui;
};

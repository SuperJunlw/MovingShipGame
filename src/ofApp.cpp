#include "ofApp.h"

void TriangleShape::draw() {

	ofSetColor(color);
	ofFill();

	ofPushMatrix();
	ofMultMatrix(getMatrix());
	ofDrawTriangle(verts[0], verts[1], verts[2]);
	ofPopMatrix();
}

void Player::draw() {

	ofSetColor(ofColor::white);
	ofPushMatrix();
	ofMultMatrix(getMatrix());
	image.draw(-image.getWidth() / 2.0, -image.getHeight() / 2.0);
	ofPopMatrix();
}


//--------------------------------------------------------------
void ofApp::setup() {
	player = Player(glm::vec3(-50, 50, 0), glm::vec3(50, 50, 0), glm::vec3(0, -50, 0));
	player.pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0);

	if (!player.image.load("images/ship.png")) {
		cout << "Can't load image" << endl;
		ofExit();
	}

	player.speed = 500;
	player.scale = 1;

	gui.setup();
	gui.add(sp.setup("Speed", player.speed, 1, 2000));
	gui.add(sc.setup("Scale", player.scale, 1, 10));
	gui.add(shImage.setup("Show Sprite", false));
	gui.add(shLine.setup("show heading vector", true));
	bHide = false;
}

//--------------------------------------------------------------
void ofApp::update() {
	player.speed = sp;
	player.scale = sc;
	player.showImage = shImage;
}

//--------------------------------------------------------------
void ofApp::draw() {
	//tri.draw();
	if (player.showImage)
		player.draw();
	else
		player.TriangleShape::draw();

	if (shLine)
	{
		ofSetColor(ofColor::black);
		ofDrawLine(player.pos, player.pos + player.header() * headerLength);
	}

	if (!bHide) gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	keymap[key] = true;

	if (keymap[OF_KEY_LEFT])
		player.rotation -= rotDeg;

	if (keymap[OF_KEY_RIGHT])
		player.rotation += rotDeg;

	if (keymap[OF_KEY_UP])
		player.pos += player.header() * (player.speed / ofGetFrameRate());

	if (keymap[OF_KEY_DOWN])
		player.pos -= player.header() * (player.speed / ofGetFrameRate());

	if (keymap[OF_KEY_CONTROL])
		control = true;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	keymap[key] = false;
	control = false;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	if (player.inside(glm::vec3(x, y, 0)) && !selected)
		player.color = ofColor::green;
	else
		player.color = ofColor::red;

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	//for the rotation of the triangle
	if (control)
	{
		glm::vec3 currentMouse(x, y, 0);
		glm::vec3 diff = currentMouse - lastMouse;
		player.rotation += diff.x / 2;
		lastMouse = currentMouse;
	}
	//for dragging the triangle
	else if (selected)
	{
		player.pos = glm::vec3(x, y, 0) + delta;
		player.color = ofColor::white;
	}
	else
		player.color = ofColor::red;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	if (player.inside(glm::vec3(x, y, 0)))
	{
		if (control) //for rotation
		{
			bInRotate = true;
			lastMouse = glm::vec3(x, y, 0);
		}
		else //for dragging
		{
			selected = true;
			delta = player.pos - glm::vec3(x, y, 0); //from the mouse point to the center of triangle
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	selected = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

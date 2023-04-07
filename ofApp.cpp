#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(0.5);
	ofEnableDepthTest();

	int span = 30;
	for (int x = -200; x <= 200; x += span) {

		for (int y = -200; y <= 200; y += span) {

			for (int z = -1200; z <= 1200; z += span * 1.1) {

				if (abs(x) >= 150 || abs(y) >= 150) {

					this->location_list.push_back(glm::vec3(x, y, z));
					this->rotate_deg_list.push_back(0);
					this->param_list.push_back(0);
					this->word_index_list.push_back(0);
				}
			}
		}
	}

	for (auto& location : this->location_list) {

		vector<int> route_info = vector<int>();
		int index = -1;
		for (auto& other : this->location_list) {

			index++;
			if (location == other) { continue; }

			float distance = glm::distance(location, other);
			if (distance <= span) {

				route_info.push_back(index);
			}
		}

		this->route_info_list.push_back(route_info);
	}

	for (int i = 0; i < 100; i++) {

		this->index_list.push_back((int)ofRandom(this->location_list.size()));
	}

	this->font_size = 23;
	ofTrueTypeFontSettings font_settings("fonts/msgothic.ttc", this->font_size);
	font_settings.antialiased = true;
	font_settings.addRanges(ofAlphabet::Japanese);
	this->font.load(font_settings);

	this->words = {

		u8"±", u8"²", u8"³", u8"´", u8"µ",
		u8"¶", u8"·", u8"¸", u8"¹", u8"º",
		u8"»", u8"¼", u8"½", u8"¾", u8"¿",
		u8"À", u8"Á", u8"Â", u8"Ã", u8"Ä",
		u8"Å", u8"Æ", u8"Ç", u8"È", u8"É",
		u8"Ê", u8"Ë", u8"Ì", u8"Í", u8"Î",
		u8"Ï", u8"Ð", u8"Ñ", u8"Ò", u8"Ó",
		u8"Ô", u8"Õ", u8"Ö",
		u8"×", u8"Ø", u8"Ù", u8"Ú", u8"Û",
		u8"Ü", u8"¦", u8"Ý",
	};
}

//--------------------------------------------------------------
void ofApp::update() {

	if (ofGetFrameNum() % 3 != 0) {

		for (int i = 0; i < this->index_list.size(); i++) {

			this->word_index_list[this->index_list[i]] = (int)ofRandom(this->words.size());
		}
		return;
	}

	for (int i = 0; i < this->index_list.size(); i++) {

		int next_index = this->route_info_list[this->index_list[i]][(int)ofRandom(this->route_info_list[this->index_list[i]].size())];
		for (int k = 0; k < this->route_info_list[this->index_list[i]].size(); k++) {

			if (this->param_list[next_index] <= 0) {

				auto loc = this->location_list[this->index_list[i]];
				auto next = this->location_list[next_index];

				auto angle = std::atan2(next.y - loc.y, next.x - loc.x);
				if (angle != 0) {

					this->rotate_deg_list[next_index] = angle * RAD_TO_DEG + 90;
				}
				this->param_list[next_index] = 60;
				this->index_list[i] = next_index;
				this->word_index_list[next_index] = (int)ofRandom(this->words.size());
				break;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	for (int i = 0; i < this->location_list.size(); i++) {

		if (this->param_list[i] > 0) {

			int z = (int)(this->location_list[i].z + ofGetFrameNum() * 6) % 2400 - 1200;

			ofPushMatrix();
			ofTranslate(this->location_list[i].x, this->location_list[i].y, z);

			ofFill();
			int alpha = ofMap(this->param_list[i], 0, 60, 0, 255);
			int rb = this->param_list[i] >= 57 ? 239 : 39;
			//ofSetColor(ofColor(rb, 239, rb, alpha));
			ofSetColor(ofColor(0, alpha));

			ofRotateZ(this->rotate_deg_list[i]);
			this->font.drawString(this->words[this->word_index_list[i]], -15, -15);

			ofPopMatrix();
		}

		if (this->param_list[i] > 0) { this->param_list[i] -= 1; }
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
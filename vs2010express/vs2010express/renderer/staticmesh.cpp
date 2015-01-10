#include "../system.h"

// StaticMesh
StaticMesh::StaticMesh() {
	this->vao = 0;
	this->size = 0;
}

void StaticMesh::init(std::string fn) {
	std::vector<glm::vec4> tran;
	std::vector<glm::vec3> v;
	std::vector<glm::vec3> n;
	std::vector<glm::vec2> t;
	std::vector<BiTriangle> bt;


	std::ifstream in(fn);

	std::string temp;

	std::string cmd;

	StaticMesh::StaticMeshSections sms = StaticMesh::NONE;

	while(std::getline(in, temp)) {
		std::stringstream ss(temp);
		std::stringstream ss2(temp);
		ss >> cmd;

		if(cmd == "version") {
			ss >> this->version;
			continue;
		} else if(cmd == "transform") {
			sms = StaticMesh::TRANSFORM;
			continue;
		} else if(cmd == "vertex") {
			sms = StaticMesh::VERTEX;
			continue;
		} else if(cmd == "normal") {
			sms = StaticMesh::NORMAL;
			continue;
		} else if(cmd == "texCoord") {
			sms = StaticMesh::TEXCOORD;
			continue;
		} else if(cmd == "face") {
			sms = StaticMesh::FACE;
			continue;
		} else if(cmd == "end") {
			sms = StaticMesh::NONE;
			continue;
		}


		if(sms == StaticMesh::TRANSFORM) {
			glm::vec4 tt;

			ss2 >> tt.x;
			ss2 >> tt.y;
			ss2 >> tt.z;
			ss2 >> tt.w;

			tran.push_back(tt);
		} else if(sms == StaticMesh::VERTEX) {
			glm::vec3 tt;

			ss2 >> tt.x;
			ss2 >> tt.y;
			ss2 >> tt.z;

			v.push_back(tt);
		} else if(sms == StaticMesh::NORMAL) {
			glm::vec3 tt;

			ss2 >> tt.x;
			ss2 >> tt.y;
			ss2 >> tt.z;

			n.push_back(tt);
		} else if(sms == StaticMesh::TEXCOORD) {
			glm::vec2 tt;
			ss2 >> tt.x;
			ss2 >> tt.y;

			t.push_back(tt);
		} else if(sms == StaticMesh::FACE) {
			BiTriangle tt;
			// Vertex-Normal
			ss2 >> tt.t1.v1;
			ss2 >> tt.t1.v2;
			ss2 >> tt.t1.v3;
			// Texture Coord
			ss2 >> tt.t2.v1;
			ss2 >> tt.t2.v2;
			ss2 >> tt.t2.v3;
			bt.push_back(tt);
		}
	}

	std::cout << "Mesh Version is " << this->version << std::endl;
	glm::mat4 mat(
		tran[0],
		tran[1],
		tran[2],
		tran[3]);


	glm::mat4 imat = glm::transpose(glm::inverse(mat));

	for(int i = 0; i < v.size(); i++) {
		glm::vec4 tv(v[i], 1.0);

		tv = mat * tv;

		v[i] = glm::vec3(tv.x, tv.y, tv.z);
		

		glm::vec4 tn(n[i], 0.0);

		tn = imat * tn;

		n[i] = glm::vec3(tn.x, tn.y, tn.z);
	}

	std::vector<glm::vec3> tangent;

	for(int i = 0; i < n.size(); i++) {
		glm::vec3 T, C1, C2;

		C1 = glm::cross(n[i], glm::vec3(0.0f, 0.0f, 1.0f));
		C2 = glm::cross(n[i], glm::vec3(0.0f, 1.0f, 0.0f));

		if(glm::length(C1) > glm::length(C2)) {
			T = C1;
		} else {
			T = C2;
		}

		T = glm::normalize(T);

		tangent.push_back(T);
	}

	std::vector<glm::vec3> vlist;
	std::vector<glm::vec3> nlist;
	std::vector<glm::vec2> tlist;
	std::vector<glm::vec3> tangentList;

	for(int i = 0; i < bt.size(); i++) {
		vlist.push_back(v[bt[i].t1.v1]);
		vlist.push_back(v[bt[i].t1.v2]);
		vlist.push_back(v[bt[i].t1.v3]);
		nlist.push_back(-n[bt[i].t1.v1]);
		nlist.push_back(-n[bt[i].t1.v2]);
		nlist.push_back(-n[bt[i].t1.v3]);
		tlist.push_back(t[bt[i].t2.v1]);
		tlist.push_back(t[bt[i].t2.v2]);
		tlist.push_back(t[bt[i].t2.v3]);
		tangentList.push_back(tangent[bt[i].t1.v1]);
		tangentList.push_back(tangent[bt[i].t1.v2]);
		tangentList.push_back(tangent[bt[i].t1.v3]);
	}

	this->size = vlist.size();

	glGenVertexArrays(1, &this->vao);

	glBindVertexArray(this->vao);

	glGenBuffers(4, this->vbo);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vlist.size() * sizeof(glm::vec3),&vlist[0],GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tlist.size() * sizeof(glm::vec2), &tlist[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nlist.size() * sizeof(glm::vec3), &nlist[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, tlist.size() * sizeof(glm::vec3), &tangentList[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

void StaticMesh::render() {
	glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLES, 0, this->size);
	glBindVertexArray(0);
}

void StaticMesh::release() {
	glDeleteBuffers(4, this->vbo);
	glDeleteVertexArrays(1, &this->vao);
}

int StaticMesh::getVersion() {
	return this->version;
}
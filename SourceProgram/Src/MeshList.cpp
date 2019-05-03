/**
* @file MeshList.cpp
*/
#include "MeshList.h"
#include <fstream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <iostream>



/**
* Vertex Buffer Objectを作成する.
*
* @param size 頂点データのサイズ.
* @param data 頂点データへのポインタ.
*
* @return 作成したVBO.
*/
GLuint CreateVBO(GLsizeiptr size, const GLvoid* data)
{
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vbo;
}

/**
* Index Buffer Objectを作成する.
*
* @param size インデックスデータのサイズ.
* @param data インデックスデータへのポインタ.
*
* @return 作成したIBO.
*/
GLuint CreateIBO(GLsizeiptr size, const GLvoid* data)
{
	GLuint ibo = 0;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return ibo;
}

/**
* Vertex Array Objectを作成する.
*
* @param vbo VAOに関連付けられるVBO.
* @param ibo VAOに関連付けられるIBO.
*
* @return 作成したVAO.
*/
GLuint CreateVAO(GLuint vbo, GLuint ibo)
{
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	const GLsizei stride = sizeof(Vertex);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, sizeof(Vertex::position) / sizeof(float), GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid*>(offsetof(Vertex, position)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, sizeof(Vertex::color) / sizeof(float), GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid*>(offsetof(Vertex, color)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, sizeof(Vertex::texCoord) / sizeof(float), GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid*>(offsetof(Vertex, texCoord)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, sizeof(Vertex::normal) / sizeof(float), GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid*>(offsetof(Vertex, normal)));
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return vao;
}

/**
* コンストラクタ.
*/
MeshList::MeshList()
{
}

/**
* デストラクタ.
*/
MeshList::~MeshList()
{
	Free();
}



/**
* モデルデータからMeshのリストを作成する.
*
* @param modelFiles 読み込むモデルファイル名のリスト.
*
* @retval true  作成成功.
* @retval false 作成失敗.
*/
bool MeshList::Allocate(const std::vector<std::string>& modelFiles)
{
	Free();

	meshes.reserve(100000);
	tmpVertices.reserve(100000);
	tmpIndices.reserve(100000);

	for (size_t i = 0; i < modelFiles.size(); ++i) {
		AddFromObjFile(modelFiles[i].c_str());
	}

	ibo = CreateIBO(tmpIndices.size() * sizeof(GLushort), tmpIndices.data());
	vbo = CreateVBO(tmpVertices.size() * sizeof(Vertex), tmpVertices.data());
	vao = CreateVAO(vbo, ibo);

	std::vector<Vertex>().swap(tmpVertices);
	std::vector<GLushort>().swap(tmpIndices);

	if (!vbo || !ibo || !vao) {
		std::cerr << "ERROR: VAOの作成に失敗.\n";
		return false;
	}

	return true;
}

/**
* メッシュリストを破棄する.
*/
void MeshList::Free()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &vbo);
	vao = 0;
	vbo = 0;
	ibo = 0;
	std::vector<Mesh>().swap(meshes);
}

/**
* メッシュを追加する.
*
* @param vBegin  頂点データのポインタ.
* @param vEnd    頂点データの終端のポインタ.
* @param iBegin  インデックスデータのポインタ.
* @param iEnd    インデックスデータの終端のポインタ.
*/
void MeshList::Add(const Vertex* vBegin, const Vertex* vEnd, const GLushort* iBegin, const GLushort* iEnd)
{
	Mesh m;
	m.mode = GL_TRIANGLES;
	m.count = iEnd - iBegin;
	m.indices = (const GLvoid*)(tmpIndices.size() * sizeof(GLushort));
	m.baseVertex = (GLint)tmpVertices.size();
	meshes.push_back(m);

	tmpVertices.insert(tmpVertices.end(), vBegin, vEnd);
	tmpIndices.insert(tmpIndices.end(), iBegin, iEnd);
}

/**
* OBJファイルからメッシュを読み込む.
*
* @param path 読み込むOBJファイルのパス.
*
* @retval true  読み込み成功.
* @retval false 読み込み失敗.
*/
bool MeshList::AddFromObjFile(const char* path)
{
	// ファイルを開く.
	std::ifstream ifs(path);
	if (!ifs) {
		std::cerr << "ERROR: " << path << "を開けません\n";
		return false;
	}

	// データ読み取り用の変数を準備.
	struct Face {
		int v;
		int vt;
		int vn;
	};
	std::vector<Face> faceList;
	std::vector<Vector3> positionList;
	std::vector<Vector2> texCoordList;
	std::vector<Vector3> normalList;
	faceList.reserve(100000);
	positionList.reserve(100000);
	texCoordList.reserve(100000);
	normalList.reserve(100000);

	// ファイルからモデルのデータを読み込む.
	while (!ifs.eof()) {
		std::string line;
		getline(ifs, line);
		if (line[0] == '#') {
			continue;
		}
		Vector3 v;
		Vector2 vt;
		Vector3 vn;
		Face f[3];
		if (sscanf_s(line.data(), "v %f %f %f", &v.x, &v.y, &v.z) == 3) {
			positionList.push_back(v);
		}
		else if (sscanf_s(line.data(), "vt %f %f", &vt.x, &vt.y) == 2) {
			texCoordList.push_back(vt);
		}
		else if (sscanf_s(line.data(), "vn %f %f %f", &vn.x, &vn.y, &vn.z) == 3) {
			const float length = sqrt(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);
			normalList.push_back(Vector3{ vn.x / length, vn.y / length, vn.z / length });
		}
		else if (sscanf_s(line.data(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
			&f[0].v, &f[0].vt, &f[0].vn,
			&f[1].v, &f[1].vt, &f[1].vn,
			&f[2].v, &f[2].vt, &f[2].vn) == 9) {
			faceList.push_back(f[0]);
			faceList.push_back(f[1]);
			faceList.push_back(f[2]);
		}
	}

	if (positionList.empty()) {
		std::cerr << "WARNING: " << path << "には頂点座標の定義がありません.\n";
		Add(nullptr, nullptr, nullptr, nullptr);
		return false;
	}

	if (texCoordList.empty()) {
		std::cerr << "WARNING: " << path << "にはテクスチャ座標の定義がありません.\n";
		Add(nullptr, nullptr, nullptr, nullptr);
		return false;
	}

	if (normalList.empty()) {
		std::cerr << "WARNING: " << path << "には法線の定義がありません.\n";
		Add(nullptr, nullptr, nullptr, nullptr);
		return false;
	}


	// 頂点データとインデックスデータ用の変数を準備.
	std::vector<Face> faceToVertexList;
	std::vector<Vertex> vertices;
	std::vector<GLushort> indices;
	faceToVertexList.reserve(faceList.size());
	vertices.reserve(faceList.size());
	indices.reserve(faceList.size());


	// モデルのデータを頂点データとインデックスデータに変換する.
	for (size_t i = 0; i < faceList.size(); ++i) {
		size_t n = 0;
		for (; n < faceToVertexList.size(); ++n) {
			if (faceToVertexList[n].v == faceList[i].v &&
				faceToVertexList[n].vt == faceList[i].vt &&
				faceToVertexList[n].vn == faceList[i].vn) {
				break;
			}
		}

		if (n < faceToVertexList.size()) {
			indices.push_back((GLushort)n);
		}
		else {
			indices.push_back((GLushort)vertices.size());

			faceToVertexList.push_back(faceList[i]);

			Vertex vertex;
			int v = faceList[i].v - 1;
			if (v < 0 || v >= (int)positionList.size()) {
				std::cerr << "WARNING: 不正なvインデックス(" << v << ")\n";
				v = 0;
			}
			int vt = faceList[i].vt - 1;
			if (vt < 0 || vt >= (int)texCoordList.size()) {
				std::cerr << "WARNING: 不正なvtインデックス(" << vt << ")\n";
				vt = 0;
			}
			int vn = faceList[i].vn - 1;
			if (vn < 0 || vn >= (int)normalList.size()) {
				std::cerr << "WARNING: 不正なvnインデックス(" << vn << ")\n";
				vn = 0;
			}
			vertex.position = positionList[v];
			vertex.color = { 1,1,1,1 };
			vertex.texCoord = texCoordList[vt];
			vertex.normal = normalList[vn];
			vertices.push_back(vertex);
		}

		if (vertices.size() >= USHRT_MAX - 1) {
			std::cerr << "WARNING: " << path << "の頂点数はGLushortで扱える範囲を超過しています.\n";
			break;
		}
	}

	///std::cout << "INFO: " << path << " [頂点数=" << vertices.size() << " インデックス数=" << indices.size() << "]\n";

	Add(vertices.data(), vertices.data() + vertices.size(), indices.data(), indices.data() + indices.size());

	return true;
}

/**
* 描画に使用するVAOを設定する.
*/
void MeshList::BindVertexArray()
{
	glBindVertexArray(vao);
}

/**
* メッシュを取得する.
*
* @param index 取得するメッシュの番号.
*
* @return index番目のメッシュ.
*/
const Mesh& MeshList::operator[](size_t index) const
{
	return meshes[index];
}

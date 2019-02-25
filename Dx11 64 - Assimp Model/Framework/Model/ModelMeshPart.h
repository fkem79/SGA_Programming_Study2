#pragma once

class ModelMeshPart
{
public:
	friend class Model;
	friend class Models;
	friend class ModelMesh;

private:
	ModelMeshPart();
	~ModelMeshPart();

	void PreRender();
	void Render();
	void RenderInstance(UINT count);

public:
	void Clone(void** clone);

	void Pass(UINT val) { pass = val; }
	UINT Pass() { return pass; }

private:
	UINT pass;

	wstring name;

	Material* material;
	wstring materialName;

	class ModelMesh* parent;

	UINT vertexCount;
	UINT startVertex;

	UINT indexCount;
	UINT startIndex;

	ID3DX11EffectScalarVariable* boneIndexVariable;
};
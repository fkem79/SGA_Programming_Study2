#pragma once

#include "GameModel.h"

class MeshSphere : public GameModel
{
public:
	MeshSphere();
	~MeshSphere();

	void Update() override;
	void Render() override;
	void PostRender() override;

private:
	D3DXCOLOR diffuseColor;
	D3DXCOLOR specularColor;
	float shininess;

	float val;
};
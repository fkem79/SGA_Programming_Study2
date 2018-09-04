#include "stdafx.h"
#include "Terrain.h"

Terrain::Terrain(ExecuteValues* values, Material * material)
	: material(material)
	, values(values)
	, bWireframe(false)
{
	heightTexture = new Texture(Contents + L"HeightMaps/HeightMap256.png");
	colorTexture = new Texture(Textures + L"Dirt.png");
	colorTexture2 = new Texture(Textures + L"Wall.png");
	alphaTexture = new Texture(Contents + L"HeightMaps/ColorMap256.png");

	worldBuffer = new WorldBuffer();
	brushBuffer = new BrushBuffer();
	gridBuffer = new GridBuffer();

	CreateData();
	CreateNormalData();
	CreateBuffer();

	rasterizer[0] = new RasterizerState();
	rasterizer[1] = new RasterizerState();
	rasterizer[1]->FillMode(D3D11_FILL_WIREFRAME);
}

Terrain::~Terrain()
{
	SAFE_DELETE(rasterizer[0]);
	SAFE_DELETE(rasterizer[1]);

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);

	SAFE_DELETE(material);

	SAFE_DELETE(heightTexture);

	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(brushBuffer);
	SAFE_DELETE(gridBuffer);

	SAFE_DELETE(colorTexture);
	SAFE_DELETE(colorTexture2);
	SAFE_DELETE(alphaTexture);
}

void Terrain::Update()
{
	if (brushBuffer->Data.Type > 0)
	{
		D3DXVECTOR3 position;
		bool bPicked = Y(&position);

		if (bPicked == true)
		{
			brushBuffer->Data.Location = position;

			UINT type = brushBuffer->Data.Type;
			// ���콺 Ŭ�� �� ���� ����
			if (Mouse::Get()->Press(0))
			{
				AdjustY(position);
			}
		}
	}
}

void Terrain::Render()
{
	ImGui::Separator();
	ImGui::Text("Terrain");

	ImGui::Separator();

	ImGui::Checkbox("Terrain Wireframe", &bWireframe);

	ImGui::Separator();
	
	ImGui::SliderInt("Brush Type", &brushBuffer->Data.Type, 0, 2);
	ImGui::SliderInt("Brush Range", &brushBuffer->Data.Range, 1, 5);
	ImGui::SliderFloat3("Brush Color", (float*)&brushBuffer->Data.Color, 0, 1);
	
	ImGui::Separator();

	ImGui::SliderInt("Line Type", &gridBuffer->Data.Type, 0, 2);
	ImGui::SliderFloat3("Line Color", (float*)&gridBuffer->Data.Color, 0, 1);
	ImGui::SliderInt("Line Spacing", &gridBuffer->Data.Spacing, 1, 10);
	ImGui::SliderFloat("Line Thickness", &gridBuffer->Data.Thickness, 0.01f, 0.5f);

	ImGui::Separator();
	ImGui::Spacing();

	brushBuffer->SetVSBuffer(10);
	gridBuffer->SetPSBuffer(10);

	colorTexture->SetShaderResource(10);
	colorTexture->SetShaderSampler(10);

	colorTexture2->SetShaderResource(11);
	colorTexture2->SetShaderSampler(11);

	alphaTexture->SetShaderResource(12);
	alphaTexture->SetShaderSampler(12);

	UINT stride = sizeof(VertexTextureNormal);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	worldBuffer->SetVSBuffer(1);
	material->PSSetBuffer();

	if (bWireframe == true)
		rasterizer[1]->RSSetState();
	
	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
	rasterizer[0]->RSSetState();
}

float Terrain::Y(D3DXVECTOR3& position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	if (x < 0 || x >= width) return 0.0f;
	if (z < 0 || z >= height) return 0.0f;

	UINT index[4];

	index[0] = (width + 1) * z + x;
	index[1] = (width + 1) * (z + 1) + x;
	index[2] = (width + 1) * z + (x + 1);
	index[3] = (width + 1) * (z + 1) + (x + 1);

	D3DXVECTOR3 v[4];
	for (int i = 0; i < 4; i++)
		v[i] = vertices[index[i]].Position;

	// ���̿� ���� ����
	float ddx = (position.x - v[0].x) / 1.0f;
	float ddz = (position.z - v[0].z) / 1.0f;

	D3DXVECTOR3 temp;

	// �ظ�
	if (ddx + ddz <= 1)
	{
		temp = v[0] + (v[2] - v[0]) * ddx + (v[1] - v[0]) * ddz;
	}
	// ����
	{
		ddx = 1 - ddx;
		ddz = 1 - ddz;

		temp = v[3] + (v[1] - v[3]) * ddx + (v[2] - v[3]) * ddz;
	}

	return temp.y;
}

bool Terrain::Y(OUT D3DXVECTOR3 * out, D3DXVECTOR3 & position)
{
	D3DXVECTOR3 start = D3DXVECTOR3(position.x, 1000, position.z);
	D3DXVECTOR3 direction = D3DXVECTOR3(0, -1, 0);

	// �ﰢ�� ��ü �浹�ϴ� ��� �ϳ��� �浹�ϴ� ��� �ΰ���������
	// �����ϰ� �Ϸ��� ��ü �ﰢ���� ���� ���ƾ���

	// ���� for�� ��ü ���ƾ��� = �� ���δ� ����ؾ���
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	if (x < 0 || x >= width) return false;
	if (z < 0 || z >= height) return false;

	UINT index[4];
	index[0] = (width + 1) * z + x;
	index[1] = (width + 1) * (z + 1) + x;
	index[2] = (width + 1) * z + (x + 1);
	index[3] = (width + 1) * (z + 1) + (x + 1);

	D3DXVECTOR3 p[4];
	for (int i = 0; i < 4; i++)
		p[i] = vertices[index[i]].Position;

	float u, v, distance;
	if (D3DXIntersectTri(&p[0], &p[1], &p[2], &start,
		&direction, &u, &v, &distance))
	{
		*out = p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;

		return true;
	}

	if (D3DXIntersectTri(&p[3], &p[1], &p[2], &start,
		&direction, &u, &v, &distance))
	{
		*out = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;

		return true;
	}

	return false;
}

bool Terrain::Y(OUT D3DXVECTOR3 * out)
{
	// start ī�޶� ��ġ direction ���콺 ��ġ�κ��� ���� ����
	//D3DXVECTOR3 start = D3DXVECTOR3(position.x, 1000, position.z);
	//D3DXVECTOR3 direction = D3DXVECTOR3(0, -1, 0);

	D3DXVECTOR3 start;
	values->MainCamera->Position(&start);

	D3DXVECTOR3 direction = values->MainCamera->Direction(
		values->Viewport, values->Perspective
	);

	// �ﰢ�� ��ü �浹�ϴ� ��� �ϳ��� �浹�ϴ� ��� �ΰ���������
	// �����ϰ� �Ϸ��� ��ü �ﰢ���� ���� ���ƾ���

	// ���� for�� ��ü ���ƾ��� = �� ���δ� ����ؾ���

	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			UINT index[4];
			index[0] = (width + 1) * z + x;
			index[1] = (width + 1) * (z + 1) + x;
			index[2] = (width + 1) * z + (x + 1);
			index[3] = (width + 1) * (z + 1) + (x + 1);

			D3DXVECTOR3 p[4];
			for (int i = 0; i < 4; i++)
				p[i] = vertices[index[i]].Position;

			float u, v, distance;
			if (D3DXIntersectTri(&p[0], &p[1], &p[2], &start,
				&direction, &u, &v, &distance))
			{
				*out = p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;
				brushBuffer->Data.Location = *out;

				return true;
			}

			if (D3DXIntersectTri(&p[3], &p[1], &p[2], &start,
				&direction, &u, &v, &distance))
			{
				*out = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;
				brushBuffer->Data.Location = *out;

				return true;
			}
		} // for(x)
	} // for(z)

	return false;
}

void Terrain::AdjustY(D3DXVECTOR3 & location)
{
	// ����
	float size = (float)brushBuffer->Data.Range;

	// directx 12�� �ִ� ����ü
	D3D11_BOX box;
	box.left = (UINT)location.x - size;
	box.top = (UINT)location.z + size;
	box.right = (UINT)location.x + size;
	box.bottom = (UINT)location.z - size;

	// ���� üũ
	if (box.left < 0) box.left = 0;
	if (box.top >= height) box.top = height;
	if (box.right >= width) box.right = width;
	if (box.bottom < 0)box.bottom = 0;

	for (UINT z = box.bottom; z <= box.top; z++) {
		for (UINT x = box.left; x <= box.right; x++) {
			UINT index = (width + 1) * z + x;

			vertices[index].Position.y += 10.0 * Time::Delta();
		}
	}

	// normal�� �����������
	CreateNormalData();

	// NULL �̺κ� ����ؼ� ������ ������ ������ ������ �� ����
	D3D::GetDC()->UpdateSubresource(
		vertexBuffer, 0, NULL, &vertices[0], 
		sizeof(VertexTextureNormal) * vertexCount, 0
	);

}

void Terrain::CreateData()
{
	vector<D3DXCOLOR> heights;
	heightTexture->ReadPixels(DXGI_FORMAT_R8G8B8A8_UNORM, &heights);

	width = heightTexture->GetWidth() - 1;
	height = heightTexture->GetHeight() - 1;

	//Create VertexData
	{
		vertexCount = (width + 1) * (height + 1);
		vertices = new VertexTextureNormal[vertexCount];

		for (UINT z = 0; z <= height; z++)
		{
			for (UINT x = 0; x <= width; x++)
			{
				UINT index = (width + 1) * z + x;

				vertices[index].Position.x = (float)x;
				vertices[index].Position.y = (float)(heights[index].r * 255.0f) / 10.0f;
				//vertices[index].Position.y = 0;
				vertices[index].Position.z = (float)z;

				vertices[index].Uv.x = (float)x / width;
				vertices[index].Uv.y = (float)z / height;
			}
		}//for(z)
	}


	//CreateIndexData
	{
		// �⺻ �ڷ��� usinged int �� �����ڷ� �ᵵ �� UINT() �̰ɷ�
		//indices.assign(width * height * 6, UINT());

		indexCount = width * height * 6;
		indices = new UINT[indexCount];

		UINT index = 0;
		for (UINT z = 0; z < height; z++)
		{
			for (UINT x = 0; x < width; x++)
			{
				indices[index + 0] = (width + 1) * z + x; //0
				indices[index + 1] = (width + 1) * (z + 1) + x; //1
				indices[index + 2] = (width + 1) * z + x + 1; //2

				indices[index + 3] = (width + 1) * z + x + 1; //2
				indices[index + 4] = (width + 1) * (z + 1) + x; //1
				indices[index + 5] = (width + 1) * (z + 1) + x + 1; //3

				index += 6;
			}
		}//for(z)
	}
}

void Terrain::CreateNormalData()
{
	// CreateNormal
	{
		for (UINT i = 0; i < (indexCount / 3); i++)
		{
			UINT index0 = indices[i * 3 + 0];
			UINT index1 = indices[i * 3 + 1];
			UINT index2 = indices[i * 3 + 2];

			VertexTextureNormal v0 = vertices[index0];
			VertexTextureNormal v1 = vertices[index1];
			VertexTextureNormal v2 = vertices[index2];

			D3DXVECTOR3 d1 = v1.Position - v0.Position;
			D3DXVECTOR3 d2 = v2.Position - v0.Position;

			D3DXVECTOR3 normal;
			D3DXVec3Cross(&normal, &d1, &d2);

			vertices[index0].Normal += normal;
			vertices[index1].Normal += normal;
			vertices[index2].Normal += normal;
		}

		for (UINT i = 0; i < vertexCount; i++)
			D3DXVec3Normalize(&vertices[i].Normal, &vertices[i].Normal);
	}
}

void Terrain::CreateBuffer()
{
	//CreateVertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexTextureNormal) * vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		// &vertices �̰� ��ü�� �ּ� 0��°�Ÿ� �־����
		data.pSysMem = &vertices[0];

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	//CreateVertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * indexCount;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = &indices[0];

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
		assert(SUCCEEDED(hr));
	}
}


cbuffer VS_ViewProjection : register(b0) // register buffer 0�� ���ڴٴ°�
{
    matrix View;
    matrix Projection;
}

cbuffer VS_World : register(b1)
{
    matrix World;
}

cbuffer PS_Light : register(b0) // ���ؽ� ���̴��� �����̹Ƿ� �ٽ� 0������
{
    float3 Direction;
}

// material ���� �ǹ�
cbuffer PS_Material : register(b1)
{
    float4 Diffuse;
}

Texture2D DiffuseMap : register(t0);
SamplerState DiffuseSampler : register(s0);

struct VertexColor
{
    float4 Position : POSITION0;
    float4 Color : COLOR0;
};

struct VertexColorNormal
{
    float4 Position : POSITION0;
    float4 Color : COLOR0;
    float3 Normal : NORMAL0;
};

struct VertexTexture
{
    float4 Position : POSITION0;
    float2 Uv : TEXCOORD0;
};

struct VertexTextureNormal
{
    float4 Position : POSITION0;
    float2 Uv : TEXCOORD0;
    float3 Normal : NORMAL0;
};

struct VertexColorTextureNormal
{
    float4 Position : POSITION0;
    float4 Polor : COLOR0;
    float2 Uv : TEXCOORD0;
    float3 Normal : NORMAL0;
};

// ������ �� ��
void DiffuseLight(inout float4 color, float3 normal)
{
    //float3 light = _direction * -1;
    // ���� ����
    // saturate 0 ~ 1���� �������ִ� �Լ�
    float intensity = saturate(dot(normal, -Direction));

    color = color + Diffuse * intensity;
}

// inout�� c++ reference �����Ŷ�� ���� ��
// in out�� �ִµ� �����ϴµ� ���������� �� ����־��Ѵٰ� �Ͻ�
// �ؽ�ó �� ��
void DiffuseLight(inout float4 color, float4 diffuse, float3 normal)
{
    //float3 light = _direction * -1;
    // ���� ����
    // saturate 0 ~ 1���� �������ִ� �Լ�
    float intensity = saturate(dot(normal, -Direction));

    color = color + Diffuse * diffuse * intensity;
}
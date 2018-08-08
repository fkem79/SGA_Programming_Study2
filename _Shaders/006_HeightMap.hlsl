cbuffer VS_ViewProjection : register(b0) // register buffer 0�� ���ڴٴ°�
{
    matrix _view;
    matrix _projection;
}

cbuffer VS_World : register(b1)
{
    matrix _world;
}

cbuffer PS_Color : register(b0) // ���ؽ� ���̴��� �����̹Ƿ� �ٽ� 0������
{
    float4 Color; 
}

struct VertexInput
{
    float4 position : POSITION0;
};

struct PixelInput
{
    float4 position : SV_POSITION; // SV�� ������ ����� ������ ���Ѵ�� ���� ��
    float4 color : COLOR0;
};

SamplerState Sampler : register(s0); // sampler�� ���� �ƴ϶� s0
// �⺻�� �ᵵ �� �ȳ־����� �⺻�� ����
Texture2D Map : register(t0); // texture�� t0
Texture2D Map2 : register(t1); // 2��° texture

PixelInput VS(VertexInput input)
{
    PixelInput output;

    float4 color = float4(1, 1, 1, 1);

    if (input.position.y > 10)
        color = float4(1, 1, 0, 1);
    if (input.position.y > 20)
        color = float4(1, 0, 0, 1);
    if (input.position.y > 30)
        color = float4(0, 1, 0, 1);
    if (input.position.y > 50)
        color = float4(0, 0, 1, 1);
    output.color = color;

    // ��� ���� ��ȯ�� ���Ŀ� position�� ���� �ؾߵǱ� ��
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    return input.color;
}
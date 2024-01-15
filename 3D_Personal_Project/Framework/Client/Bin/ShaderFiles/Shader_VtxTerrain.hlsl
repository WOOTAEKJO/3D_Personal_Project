#include	"Shader_Defines.hlsli"

matrix		g_matWorld, g_matView, g_matProj;

texture2D	g_DiffuseTexture[2];
texture2D	g_MaskTexture;
texture2D	g_BrushTexture;

vector		g_vBrushPos;
float		g_fBrushRange;

bool		g_bWireFrame;

struct VS_IN
{
	float3	vPosition : POSITION;
	float3  vTanget : TANGENT;
	float3	vNormal	: NORMAL;
	float2	vTexCoord : TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition : SV_POSITION;
	float4  vTanget : TANGENT;
	float4	vNormal	: NORMAL;
	float2	vTexCoord : TEXCOORD0;
	float4	vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vNormal = mul(float4(In.vNormal, 0.f), g_matWorld);
	Out.vWorldPos = mul(float4(In.vPosition, 1.f),g_matWorld);
	Out.vTexCoord = In.vTexCoord;
    Out.vProjPos = Out.vPosition;
	
	return Out;
}
/* ����� ������ ��� .*/

/* ������ȯ : /w */ /* -> -1, 1 ~ 1, -1 */
/* ����Ʈ��ȯ-> 0, 0 ~ WINSX, WINSY */
/* �����Ͷ����� : ���������� ����Ͽ� �ȼ��� ������ �����. */

struct PS_IN
{
	float4	vPosition : SV_POSITION;
	float4  vTanget : TANGENT;
	float4	vNormal	: NORMAL;
	float2	vTexCoord : TEXCOORD0;
	float4	vWorldPos : TEXCOORD1;
    float4	vProjPos : TEXCOORD2;
};

struct PS_OUT
{
	float4	vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
};

/* �ȼ����̴� : �ȼ��� ��!!!! �� �����Ѵ�. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vSourDiffuse = g_DiffuseTexture[0].Sample(LinearSampler, In.vTexCoord * 300.f);
	vector vDestDiffuse = g_DiffuseTexture[1].Sample(LinearSampler, In.vTexCoord * 300.f);
    //vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord * 300.f);

    vector vMask = g_MaskTexture.Sample(LinearSampler, In.vTexCoord);

    vector vDiffuse = vMask * vDestDiffuse + (1.f - vMask) * vSourDiffuse;

    Out.vDiffuse = vDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
	// In.vProjPos.z / In.vProjPos.w -> ���� �����̽� �� z �� -> z ������ ����
    // In.vProjPos.w -> �� �����̽� �� z ��
    // (/ 1000.f) �� ���� �ȼ� �ɼ��� �뷮�� �������̱� ����
	
	return Out;
}

PS_OUT PS_DTERRAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vBrush = vector(0.f, 0.f, 0.f, 0.f);

	if ((g_vBrushPos.x - g_fBrushRange) <= In.vWorldPos.x && In.vWorldPos.x <= (g_vBrushPos.x + g_fBrushRange) &&
		(g_vBrushPos.z - g_fBrushRange) <= In.vWorldPos.z && In.vWorldPos.z <= (g_vBrushPos.z + g_fBrushRange)) {

		float2 vUV;

		vUV.x = (In.vWorldPos.x - (g_vBrushPos.x - g_fBrushRange)) / (2.f * g_fBrushRange);
		vUV.y = ((g_vBrushPos.z + g_fBrushRange) - In.vWorldPos.z) / (2.f * g_fBrushRange);

		vBrush = g_BrushTexture.Sample(LinearSampler, vUV);

	}

	if (!g_bWireFrame) {

        vector vSourDiffuse = g_DiffuseTexture[0].Sample(LinearSampler, In.vTexCoord * 300.f);
        vector vDestDiffuse = g_DiffuseTexture[1].Sample(LinearSampler, In.vTexCoord * 300.f);

        vector vMask = g_MaskTexture.Sample(LinearSampler, In.vTexCoord);

        vector vDiffuse = vMask * vDestDiffuse + (1.f - vMask) * vSourDiffuse + vBrush;

        Out.vDiffuse = vDiffuse;
        Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
        Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
    }
	else {
        Out.vDiffuse = vector(0.f, 1.f, 0.f, 1.f) + vBrush;
        Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
        Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
    }

	return Out;
}

technique11 DefaultTechnique
{
	/* ���� ���ϴ� Ư�� ���̴����� �׸��� �𵨿� �����Ѵ�. */
	pass Terrain
	{
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass DTerrain
	{
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_DTERRAIN();
	}
}

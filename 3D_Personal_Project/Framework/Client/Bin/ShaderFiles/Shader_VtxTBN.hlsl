#include	"Shader_Defines.hlsli"

matrix		g_matWorld, g_matView, g_matProj;

vector		g_PointLightPos = vector(100.f, 20.f, 100.f, 1.f);
float		g_PointLightRange = 30.f;

vector		g_LightDir = vector(1.f, -1.f, 1.f, 0.f);
vector		g_LightDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector		g_LightAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector		g_LightSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector		g_MtrlAmbient = vector(0.3f, 0.3f, 0.3f, 1.f);
vector		g_MtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector		g_CamWorldPos;

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
	float3	vBinormal : BINORMAL;
	float3	vNormal	: NORMAL;
	float2	vTexCoord : TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition : SV_POSITION;
	float4  vTanget : TANGENT;
	float4	vBinormal : BINORMAL;
	float4	vNormal	: NORMAL;
	float2	vTexCoord : TEXCOORD0;
	float4	vWorldPos : TEXCOORD1;
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

	return Out;
}
/* 통과된 정점을 대기 .*/

/* 투영변환 : /w */ /* -> -1, 1 ~ 1, -1 */
/* 뷰포트변환-> 0, 0 ~ WINSX, WINSY */
/* 래스터라이즈 : 정점정보에 기반하여 픽셀의 정보를 만든다. */

struct PS_IN
{
	float4	vPosition : SV_POSITION;
	float4  vTanget : TANGENT;
	float4	vBinormal : BINORMAL;
	float4	vNormal	: NORMAL;
	float2	vTexCoord : TEXCOORD0;
	float4	vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

/* 픽셀셰이더 : 픽셀의 색!!!! 을 결정한다. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vSourDiffuse = g_DiffuseTexture[0].Sample(LinearSampler, In.vTexCoord * 100.f);
	vector vDestDiffuse = g_DiffuseTexture[1].Sample(LinearSampler, In.vTexCoord * 100.f);

	vector vMask = g_MaskTexture.Sample(LinearSampler, In.vTexCoord);

	vector vDiffuse = vMask * vDestDiffuse + (1.f - vMask) * vSourDiffuse;

	float fContrast = max(dot(normalize(g_LightDir) * -1.f, normalize(In.vNormal)), 0.f); // 명암

	vector vLook = In.vWorldPos - g_CamWorldPos;
	vector vReflect = reflect(normalize(g_LightDir), normalize(In.vNormal)); // 반사벡터

	float fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 30.f); // 빛 반사

	Out.vColor = g_LightDiffuse * vDiffuse * min((fContrast + (g_LightAmbient * g_MtrlAmbient)), 1.f)
		+ (g_LightSpecular * g_MtrlSpecular) * fSpecular;

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

		vector vSourDiffuse = g_DiffuseTexture[0].Sample(LinearSampler, In.vTexCoord * 100.f);
		vector vDestDiffuse = g_DiffuseTexture[1].Sample(LinearSampler, In.vTexCoord * 100.f);

		vector vMask = g_MaskTexture.Sample(LinearSampler, In.vTexCoord);

		vector vDiffuse = vMask * vDestDiffuse + (1.f - vMask) * vSourDiffuse + vBrush;

		float fContrast = max(dot(normalize(g_LightDir) * -1.f, normalize(In.vNormal)), 0.f); // 명암

		vector vLook = In.vWorldPos - g_CamWorldPos;
		vector vReflect = reflect(normalize(g_LightDir), normalize(In.vNormal)); // 반사벡터

		float fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 30.f); // 빛 반사

		Out.vColor = g_LightDiffuse * vDiffuse * min((fContrast + (g_LightAmbient * g_MtrlAmbient)), 1.f)
			+ (g_LightSpecular * g_MtrlSpecular) * fSpecular;
	}
	else {
		Out.vColor = vector(0.f, 1.f, 0.f, 1.f) + vBrush;
	}

	return Out;
}

PS_OUT PS_MODEL(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vDiffuse = g_DiffuseTexture[0].Sample(LinearSampler, In.vTexCoord);

	if (vDiffuse.a < 0.3f)
		discard;

	float fContrast = max(dot(normalize(g_LightDir) * -1.f, normalize(In.vNormal)), 0.f); // 명암

	vector vLook = In.vWorldPos - g_CamWorldPos;
	vector vReflect = reflect(normalize(g_LightDir), normalize(In.vNormal)); // 반사벡터

	float fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 30.f); // 빛 반사

	Out.vColor = g_LightDiffuse * vDiffuse * min((fContrast + (g_LightAmbient * g_MtrlAmbient)), 1.f)
		+ (g_LightSpecular * g_MtrlSpecular) * fSpecular;

	return Out;
}

technique11 DefaultTechnique
{
	/* 내가 원하는 특정 셰이더들을 그리는 모델에 적용한다. */
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

	pass Model
	{
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MODEL();
	}

}

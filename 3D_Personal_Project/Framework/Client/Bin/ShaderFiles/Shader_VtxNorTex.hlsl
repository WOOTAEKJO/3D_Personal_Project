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

sampler		DefaultSampler = sampler_state
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = wrap;
	AddressV = wrap;
	//D3D11_SAMPLER_DESC
	// ����
};

struct VS_IN
{
	float3	vPosition : POSITION;
	float3  vNormal	  : NORMAL;
	float2	vTexCoord : TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition : SV_POSITION;
	float4  vNormal	  : NORMAL;
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
	Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_matWorld);
	Out.vTexCoord = In.vTexCoord;

	return Out;
}
/* ����� ������ ��� .*/

/* ������ȯ : /w */ /* -> -1, 1 ~ 1, -1 */
/* ����Ʈ��ȯ-> 0, 0 ~ WINSX, WINSY */
/* �����Ͷ����� : ���������� ����Ͽ� �ȼ��� ������ �����. */

struct PS_IN
{
	float4	vPosition : SV_POSITION;
	float4  vNormal	  : NORMAL;
	float2	vTexCoord : TEXCOORD0;
	float4	vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

/* �ȼ����̴� : �ȼ��� ��!!!! �� �����Ѵ�. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vSourDiffuse = g_DiffuseTexture[0].Sample(DefaultSampler, In.vTexCoord*100.f);
	vector vDestDiffuse = g_DiffuseTexture[1].Sample(DefaultSampler, In.vTexCoord * 100.f);

	vector vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexCoord);

	vector vDiffuse = vMask * vDestDiffuse + (1.f - vMask) * vSourDiffuse;

	float fContrast = max(dot(normalize(g_LightDir) * -1.f, normalize(In.vNormal)), 0.f); // ���

	vector vLook = In.vWorldPos - g_CamWorldPos;
	vector vReflect = reflect(normalize(g_LightDir), normalize(In.vNormal)); // �ݻ纤��

	float fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 30.f); // �� �ݻ�

	Out.vColor = g_LightDiffuse * vDiffuse * min((fContrast + (g_LightAmbient * g_MtrlAmbient)), 1.f)
		+ (g_LightSpecular * g_MtrlSpecular) * fSpecular;

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
		PixelShader = compile ps_5_0 PS_MAIN();
	}

}

#include	"Shader_Defines.hlsli"

matrix		g_matWorld, g_matView, g_matProj;

matrix		g_matViewInv, g_matProjInv;
// ���� ���� Ÿ�ٿ��� ���� z ���� �̿��� �ȼ��� ���� ��ġ�� ���� �� �ʿ��� �ڷ�

vector		g_vLightDir;
// ���⼺ ����

vector		g_vLightPos;
float		g_fLightRange;
// ������

vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector      g_vLightSpecular;

vector		g_vMtrlAmbient = vector(1.f,1.f,1.f,1.f);
vector      g_vMtrlSpecular = vector(0.1f, 0.1f, 0.1f, 1.f);
// ��ü���� ambient�� ������ ���� �� �����ؼ� ���

vector		g_vCameraPos;

float		g_fFar; 
// �� �����̽� �� Far ��

texture2D	g_DiffuseTexture;
texture2D	g_NormalTexture;
texture2D	g_DepthTexture;
texture2D	g_SpecularTexture;
texture2D	g_ShadeTexture;

struct VS_IN
{
	float3	vPosition : POSITION;
	float2	vTexCoord : TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition : SV_POSITION;
	float2	vTexCoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexCoord = In.vTexCoord;

	return Out;
}

struct PS_IN
{
	float4	vPosition : SV_POSITION;
	float2	vTexCoord : TEXCOORD0;
};

struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

    Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);
	
	return Out;
}

struct PS_OUT_LIGHT
{
    float4 vShade : SV_TARGET0;
    float4 vSpecular : SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_DIRECTIONAL(PS_IN In)
{
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;

    vector vNormalDesc = g_NormalTexture.Sample(PointSampler, In.vTexCoord);
    
    float4 vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
    // ���� ��� ���� ������ �����ش�.
    
    Out.vShade = g_vLightDiffuse * min((max(dot(normalize(g_vLightDir) * -1.f, vNormal), 0.f)
    + (g_vLightAmbient * g_vMtrlAmbient)), 1.f);
    // ���� ���� ���� �� ���⸦ �����ְ� ����� ������ 0�� ������ �ʰ� �ϱ� ���� Ambient ���� �����ش�.
    // ������ ���� ������ ������ 0������ �����.
    // �� �� ���Ⱑ 1�� ���� �ʰ� ������ش�.
   
    vector vDepth = g_DepthTexture.Sample(PointSampler, In.vTexCoord);
    // ���� ���� Ÿ�ٿ��� ��� �� ��
   
    float fViewZ = vDepth.y * g_fFar;
    // �� �����̽� �� z ��
    
    vector vWorldPos;
    // �������
    vWorldPos.x = In.vTexCoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexCoord.y * -2.f + 1.f;
    vWorldPos.z = vDepth.x;
    vWorldPos.w = 1.f;
    
    // �����
    vWorldPos = vWorldPos * fViewZ;
    vWorldPos = mul(vWorldPos , g_matProjInv);
    
    // �������
    vWorldPos = mul(vWorldPos , g_matViewInv);
    
    vector vLook = vWorldPos - g_vCameraPos;
    // �ü� ����
    
    vector vReflect = reflect(normalize(g_vLightDir), normalize(vNormal));
    // �ݻ纤��
    
    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) *
    pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 60.f);
	
    return Out;
}

PS_OUT_LIGHT PS_MAIN_POINT(PS_IN In)
{
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;

    vector vNormalDesc = g_NormalTexture.Sample(PointSampler, In.vTexCoord);
    
    float4 vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
    
    vector vDepth = g_DepthTexture.Sample(PointSampler, In.vTexCoord);
   
    float fViewZ = vDepth.y * g_fFar;
    
    vector vWorldPos;
    // �������
    vWorldPos.x = In.vTexCoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexCoord.y * -2.f + 1.f;
    vWorldPos.z = vDepth.x;
    vWorldPos.w = 1.f;
    
    // �����
    vWorldPos = vWorldPos * fViewZ;
    vWorldPos = mul(vWorldPos, g_matProjInv);
    
    // �������
    vWorldPos = mul(vWorldPos, g_matViewInv);
    
    float4 vLightDir = vWorldPos - g_vLightPos;
    float fDistance = length(vLightDir);
    
    float fAtt = max((g_fLightRange - fDistance) / g_fLightRange, 0.f);
    
    Out.vShade = g_vLightDiffuse * min((max(dot(normalize(g_vLightDir) * -1.f, vNormal), 0.f)
    + (g_vLightAmbient * g_vMtrlAmbient)), 1.f) * fAtt;
   
    vector vLook = vWorldPos - g_vCameraPos;
    // �ü� ����
    
    vector vReflect = reflect(normalize(g_vLightDir), normalize(vNormal));
    // �ݻ纤��
    
    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) *
    pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 30.f) * fAtt;
	
    return Out;
}

PS_OUT PS_MAIN_FINAL(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);
    if (vDiffuse.a == 0.f)
        discard;
    
    vector vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexCoord);
    vector vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexCoord);
    
    Out.vColor = vDiffuse * vShade + vSpecular;
	
    return Out;
}

technique11 DefaultTechnique
{
	pass Default
	{
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

    pass Light_Dir
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Blend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL();
    }

    pass Light_Point
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Blend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_POINT();
    }

    pass Final
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FINAL();
    }

}

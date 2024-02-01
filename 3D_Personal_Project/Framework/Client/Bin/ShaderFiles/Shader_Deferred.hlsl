#include	"Shader_Defines.hlsli"

matrix		g_matWorld, g_matView, g_matProj;

matrix		g_matViewInv, g_matProjInv;
// ���� ���� Ÿ�ٿ��� ���� z ���� �̿��� �ȼ��� ���� ��ġ�� ���� �� �ʿ��� �ڷ�
matrix g_matLightView, g_matLightProj;
// ������ ���� Ÿ���� ����
// �׸��� ���� ��� ���

float g_fTexW = 1280.0f;
float g_fTexH = 720.0f;

static const float fWeight[13] =
{
    0.0561, 0.1353, 0.278, 0.4868, 0.7261, 0.9231, 1,
	0.9231, 0.7261, 0.4868, 0.278, 0.1353, 0.0561
};

static const float fTotal = 6.2108;
// static const float fTotal = 2.6054;
// �� �� ��� ��������

float2 rcpres = { 0.0025, 0.0033333333333333333333333333333333 };

float2 PixelKernelH[13] =
{
    { -6, 0 },
    { -5, 0 },
    { -4, 0 },
    { -3, 0 },
    { -2, 0 },
    { -1, 0 },
    { 0, 0 },
    { 1, 0 },
    { 2, 0 },
    { 3, 0 },
    { 4, 0 },
    { 5, 0 },
    { 6, 0 },
};

float2 PixelKernelV[13] =
{
    { 0, -6 },
    { 0, -5 },
    { 0, -4 },
    { 0, -3 },
    { 0, -2 },
    { 0, -1 },
    { 0, 0 },
    { 0, 1 },
    { 0, 2 },
    { 0, 3 },
    { 0, 4 },
    { 0, 5 },
    { 0, 6 },
};

float BlurWeights[13] =
{
    0.002216, 0.008764, 0.026995, 0.064759, 0.120985, 0.176033, 0.199471, 0.176033, 0.120985, 0.064759,
    0.026995, 0.008764, 0.002216,

};

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

float       g_fLightFar;
// �׸��� ���� far ��

texture2D	g_DiffuseTexture;
texture2D	g_NormalTexture;
texture2D	g_DepthTexture;
texture2D	g_SpecularTexture;
texture2D	g_ShadeTexture;
texture2D   g_LightDepthTexture;
texture2D   g_EffectTexture;
texture2D   g_BlurTexture;

float       g_fFogStart;
float       g_fFogEnd;
vector      g_vFogColor;

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
    
    vector vReflect = reflect(normalize(g_vLightDir), vNormal);
    // �ݻ纤��
    
    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) *
    pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 30.f);
	
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
    
    Out.vShade = g_vLightDiffuse * min((max(dot(normalize(vLightDir) * -1.f, vNormal), 0.f)
    + (g_vLightAmbient * g_vMtrlAmbient)), 1.f) * fAtt;
   
    vector vLook = vWorldPos - g_vCameraPos;
    // �ü� ����
    
    vector vReflect = reflect(normalize(vLightDir), vNormal);
    // �ݻ纤��
    
    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) *
    pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 30.f) * fAtt;
	
    return Out;
}

struct PS_OUT_FINAL
{
    vector vResult : SV_TARGET0;

};

PS_OUT_FINAL PS_MAIN_FINAL(PS_IN In)
{
    PS_OUT_FINAL Out = (PS_OUT_FINAL) 0;

    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);
    if (vDiffuse.a == 0.f)
        discard;
    
    vector vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexCoord);
    vector vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexCoord);
    vector vBlur = g_BlurTexture.Sample(LinearSampler, In.vTexCoord);
    vector vEffect = g_EffectTexture.Sample(LinearSampler, In.vTexCoord);
    // �� �߰�
    
    //Out.vResult = vDiffuse * vShade + vSpecular + vEffect + vBlur;
    Out.vResult = vDiffuse * vShade + vSpecular;
    
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
    
    vWorldPos = mul(vWorldPos, g_matLightView);
    vWorldPos = mul(vWorldPos, g_matLightProj);
    // �׸��� ���� ��� ������ ����
    
    float2 vUV = (float2) 0.0f;
    
    vUV.x = (vWorldPos.x / vWorldPos.w) * 0.5f + 0.5f;
    vUV.y = (vWorldPos.y / vWorldPos.w) * -0.5f + 0.5f;
    // ������ǥ�� UV ��ǥ�� ����
    
    float4 vLightDepth = g_LightDepthTexture.Sample(LinearSampler, vUV);
    
    if (vWorldPos.w - 0.1f > vLightDepth.x * g_fLightFar)
        Out.vResult = Out.vResult * 0.7f;
    // z ���� ���ؼ� �׸��� ����
    
    float FogFactor = saturate((g_fFogEnd - fViewZ) / (g_fFogEnd - g_fFogStart));
    Out.vResult = FogFactor * Out.vResult + (1.f - FogFactor) * g_vFogColor;
    // �Ȱ� ���̴�
	
    Out.vResult += vEffect + vBlur;
    return Out;
}

float4 Blur_X(float2 vTexCoord)
{
    float4 vOut = (float4) 0;
    
    float2 vUV = (float2) 0;
    
    for (int i = -6; i < 7; ++i)
    {
        vUV = vTexCoord + float2(1.f / g_fTexW * i, 0);
        vOut += fWeight[6 + i] * g_EffectTexture.Sample(ClampSampler, vUV);
    }

    vOut /= fTotal;

    return vOut;

}

float4 Blur_Y(float2 vTexCoord)
{
    float4 vOut = (float4) 0;

    float2 vUV = (float2) 0;

    for (int i = -6; i < 7; ++i)
    {
        vUV = vTexCoord + float2(0, 1.f / (g_fTexH / 2.f) * i);
        vOut += fWeight[6 + i] * g_EffectTexture.Sample(ClampSampler, vUV);
    }

    vOut /= fTotal;

    return vOut;
}

float4 PSBlur(float2 vTexCoord)
{
    float4 vOut = (float4) 0;

    float4 vColor = g_EffectTexture.Sample(LinearSampler, vTexCoord);
    
    if ((vColor.r == 0) && (vColor.g == 0) && (vColor.b == 0))
    {
        vOut = vColor;

    }
    else
    {
        vColor = pow(vColor, 32.f);
    //vColor = pow(vColor, 64.f);
    
        float4 vColor2 = -0.84f;
    
        for (int i = 0; i < 13; ++i)
        {
            vColor2 += g_EffectTexture.Sample(LinearSampler, vTexCoord + (PixelKernelH[i] * rcpres)) * BlurWeights[i];
            vColor2 += g_EffectTexture.Sample(LinearSampler, vTexCoord + (PixelKernelV[i] * rcpres)) * BlurWeights[i];
        }

        vColor2 *= 0.48f;

        float4 vColor3 = g_EffectTexture.Sample(ClampSampler, vTexCoord);
    
    
        vOut = vColor + vColor2 + vColor3;
    }
   
    return vOut;
}

PS_OUT PS_MAIN_BLUR_X(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = Blur_X(In.vTexCoord);

    return Out;
}

PS_OUT PS_MAIN_BLUR_Y(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = Blur_Y(In.vTexCoord);

    return Out;
}

PS_OUT PS_MAIN_PSBLUR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = PSBlur(In.vTexCoord);

    return Out;
}

technique11 DefaultTechnique
{
	pass Default //0
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

    pass Light_Dir //1
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

    pass Light_Point //2
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

    pass Final //3
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

    pass Blur_X //4
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
       

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLUR_X();
    }

    pass Blur_Y //5
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
       

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLUR_Y();
    }

    pass PSBlur //6
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
       

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_PSBLUR();
    }

}

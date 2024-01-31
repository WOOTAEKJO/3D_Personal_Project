#include	"Shader_Defines.hlsli"

matrix		g_matWorld, g_matView, g_matProj;

texture2D	g_DiffuseTexture;

vector  g_vSolid_Color;

float g_fLightFar;

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
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_matWorld));
	Out.vWorldPos = mul(float4(In.vPosition, 1.f),g_matWorld);
	Out.vTexCoord = In.vTexCoord;
    Out.vProjPos = Out.vPosition;

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
	float4	vNormal	: NORMAL;
	float2	vTexCoord : TEXCOORD0;
	float4	vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct PS_OUT
{
	float4	vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
};

/* 픽셀셰이더 : 픽셀의 색!!!! 을 결정한다. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);

    Out.vDiffuse = vDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
    // In.vProjPos.z / In.vProjPos.w -> 투영 스페이스 상에 z 값 -> z 나누기 해줌
    // In.vProjPos.w -> 뷰 스페이스 상에 z 값
    // (/ 1000.f) 한 이유 픽셀 옵션이 용량이 한정적이기 때문
	
	return Out;
}

PS_OUT PS_MODEL(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);

	if (vDiffuse.a < 0.1f)
		discard;

    Out.vDiffuse = vDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	return Out;
}

PS_OUT PS_MODEL_BLEND(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);

    if (vDiffuse.a < 0.1f)
        discard;

    Out.vDiffuse = vDiffuse * g_vSolid_Color;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

    return Out;
}

struct PS_OUT_SHADOW
{
    vector vLightDepth : SV_TARGET0;
};

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN In)
{
    PS_OUT_SHADOW Out = (PS_OUT_SHADOW) 0;

    Out.vLightDepth = In.vProjPos.w / g_fLightFar;
    
    return Out;
}

technique11 DefaultTechnique
{
	/* 내가 원하는 특정 셰이더들을 그리는 모델에 적용한다. */
	pass Model  //0
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

    pass Cull_Non_Model //1
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MODEL();
    }

    pass Model_Blend //2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MODEL_BLEND();
    }

    pass Shadow //2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
    }

}

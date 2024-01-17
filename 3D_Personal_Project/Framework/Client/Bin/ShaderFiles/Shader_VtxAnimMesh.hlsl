#include	"Shader_Defines.hlsli"

matrix		g_matWorld,g_matView, g_matProj;

matrix		g_BlendMatrix[256];

texture2D	g_DiffuseTexture;
	
int2		g_iDiscardIndx; // 특정 뼈의 메쉬를 투명하게 하기 위함
// 일단 사용하고 수정하자 너무 비효율적이다. 특정 한 메쉬만 생각해서 만들어서

struct VS_IN
{
	float3	vPosition : POSITION;
	float3  vTangent : TANGENT;
	float3	vNormal	: NORMAL;
	float2	vTexCoord : TEXCOORD0;

	uint4	vBlendIndices : BLENDINDEX;
	float4  vBlendWeights : BLENDWEIGHT;
};

struct VS_OUT
{
	float4	vPosition : SV_POSITION;
	float4  vTangent : TANGENT;
	float4	vNormal	: NORMAL;
	float2	vTexCoord : TEXCOORD0;
	float4	vWorldPos : TEXCOORD1;
    float4	vProjPos : TEXCOORD2;
	
    uint4 vBlendIndices : BLENDINDEX;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	float fWeightW = 1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z);

	matrix matBlend = g_BlendMatrix[In.vBlendIndices.x] * In.vBlendWeights.x +
		g_BlendMatrix[In.vBlendIndices.y] * In.vBlendWeights.y +
		g_BlendMatrix[In.vBlendIndices.z] * In.vBlendWeights.z +
		g_BlendMatrix[In.vBlendIndices.w] * fWeightW;

	vector vPosition = mul(vector(In.vPosition, 1.f), matBlend);
    vector vNormal = mul(vector(In.vNormal, 0.f), matBlend);
	
    matrix matWV, matWVP;

    matWV = mul(g_matWorld, g_matView);
    matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vPosition, matWVP);
    Out.vNormal = normalize(mul(vNormal, g_matWorld));
	Out.vWorldPos = mul(float4(In.vPosition, 1.f),g_matWorld);
    Out.vProjPos = Out.vPosition;
	Out.vTexCoord = In.vTexCoord;
    Out.vBlendIndices = In.vBlendIndices;
	return Out;
}
/* 통과된 정점을 대기 .*/

/* 투영변환 : /w */ /* -> -1, 1 ~ 1, -1 */
/* 뷰포트변환-> 0, 0 ~ WINSX, WINSY */
/* 래스터라이즈 : 정점정보에 기반하여 픽셀의 정보를 만든다. */

struct PS_IN
{
	float4	vPosition : SV_POSITION;
	float4  vTangent : TANGENT;
	float4	vNormal	: NORMAL;
	float2	vTexCoord : TEXCOORD0;
	float4	vWorldPos : TEXCOORD1;
    float4	vProjPos : TEXCOORD2;
	
    uint4 vBlendIndices : BLENDINDEX;
};

struct PS_OUT
{
	float4	vDiffuse : SV_TARGET0;
    float4	vNormal: SV_TARGET1;
    float4	vDepth : SV_TARGET2;
};

/* 픽셀셰이더 : 픽셀의 색!!!! 을 결정한다. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);

    Out.vDiffuse = vDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f,0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
    // In.vProjPos.z / In.vProjPos.w -> 투영 스페이스 상에 z 값
    // In.vProjPos.w -> 뷰 스페이스 상에 z 값
    // (/ 1000.f) 한 이유 픽셀 옵션이 용량이 한정적이기 때문
    
	return Out;
}

PS_OUT PS_MODEL(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);

	if (vDiffuse.a < 0.3f)
		discard;
	
    Out.vDiffuse = vDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	return Out;
}

PS_OUT PS_INVISIBLE_MODEL(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);

    if (vDiffuse.a < 0.3f)
        discard;
    
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (g_iDiscardIndx[i] == In.vBlendIndices[j])
            {
                discard;
            }
        }
    } // 일단 사용하고 수정하자 너무 비효율적이다. 특정 한 메쉬만 생각해서 만들어서
        
    Out.vDiffuse = vDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

    return Out;
}

technique11 DefaultTechnique
{
	/* 내가 원하는 특정 셰이더들을 그리는 모델에 적용한다. */
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

    pass NonCul_Model
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

    pass NonCul_InvisibleModel
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INVISIBLE_MODEL();
    }
}

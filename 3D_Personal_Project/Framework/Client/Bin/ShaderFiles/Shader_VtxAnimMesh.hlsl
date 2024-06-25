#include	"Shader_Defines.hlsli"

matrix		g_matWorld,g_matView, g_matProj;
// 월드, 뷰, 투영 행렬

matrix		g_BlendMatrix[256];
// 블랜드 행렬 배열

texture2D	g_DiffuseTexture; // 디퓨즈 텍스쳐
texture2D g_DissolveTexture; // 디졸브 텍스쳐

// 디졸브 효과에 사용되는 변수들
float g_fDissolveAmount;
float g_fDissolveGradiationDistance;
float3 g_vDissolveGradiationStartColor;
float3 g_vDissolveGradiationGoalColor;
	
int2		g_iDiscardIndx; // 특정 뼈의 메쉬를 투명하게 하기 위함
// 일단 사용하고 수정하자 너무 비효율적이다. 특정 한 메쉬만 생각해서 만들어서

bool        g_bHited; // 피격 여부

float       g_fLightFar; // 광원과의 최대 거리

float2 rcpres = { 0.0025, 0.0033333333333333333333333333333333 }; // 역 해상도

// 블러 커널의 수평 및 수직 위치
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

}; // 블러 가중치

struct VS_IN
{
	float3	vPosition : POSITION; // 위치
	float3  vTangent : TANGENT; // 탄젠트
	float3	vNormal	: NORMAL; // 노멀
	float2	vTexCoord : TEXCOORD0; // 텍스쳐 좌표

	uint4	vBlendIndices : BLENDINDEX; // 블렌드 인덱스
	float4  vBlendWeights : BLENDWEIGHT; // 블렌드 가중치
}; // 정점 셰이더 입력 구조체

struct VS_OUT
{
	float4	vPosition : SV_POSITION; // 위치
	float4  vTangent : TANGENT; // 탄젠트
	float4	vNormal	: NORMAL; // 노멀
	float2	vTexCoord : TEXCOORD0; // 텍스쳐 좌표
	float4	vWorldPos : TEXCOORD1; // 월드 좌표
    float4	vProjPos : TEXCOORD2; // 투영 좌표
	
    uint4 vBlendIndices : BLENDINDEX; // 블렌드 인덱스
}; // 정점 셰이더 출력 구조체

VS_OUT VS_MAIN(VS_IN In) // 정점 셰이더 메인 함수
{
	VS_OUT Out = (VS_OUT)0;

	float fWeightW = 1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z);
    // 블렌드 가중치 계산

	matrix matBlend = g_BlendMatrix[In.vBlendIndices.x] * In.vBlendWeights.x +
		g_BlendMatrix[In.vBlendIndices.y] * In.vBlendWeights.y +
		g_BlendMatrix[In.vBlendIndices.z] * In.vBlendWeights.z +
		g_BlendMatrix[In.vBlendIndices.w] * fWeightW;
    // 블렌드 행렬 계산

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
	float4	vPosition : SV_POSITION; // 위치
	float4  vTangent : TANGENT; // 탄젠트
	float4	vNormal	: NORMAL; // 노멀
	float2	vTexCoord : TEXCOORD0; // 텍스쳐 좌표
	float4	vWorldPos : TEXCOORD1; // 월드 좌표
    float4	vProjPos : TEXCOORD2; // 투영 좌표
	
    uint4 vBlendIndices : BLENDINDEX; // 블렌드 인덱스
};// 픽셀 셰이더 입력 구조체

struct PS_OUT
{
	float4	vDiffuse : SV_TARGET0; // 디퓨즈
    float4	vNormal: SV_TARGET1; // 노멀
    float4	vDepth : SV_TARGET2; // 깊이
};// 픽셀 셰이더 출력 구조체

float4 PSBlur(float2 vTexCoord) // 블러 함수
{
    float4 vOut = (float4) 0;

    float4 vColor = g_DiffuseTexture.Sample(LinearSampler, vTexCoord);
    
    if ((vColor.r == 0) && (vColor.g == 0) && (vColor.b == 0))
    {
        vOut = vColor;

    }
    else
    {
        vColor = pow(vColor, 32.f);
    
        float4 vColor2 = -0.84f;
    
        for (int i = 0; i < 13; ++i)
        {
            vColor2 += g_DiffuseTexture.Sample(LinearSampler, vTexCoord + (PixelKernelH[i] * rcpres)) * BlurWeights[i];
            vColor2 += g_DiffuseTexture.Sample(LinearSampler, vTexCoord + (PixelKernelV[i] * rcpres)) * BlurWeights[i];
        }

        vColor2 *= 0.48f;

        float4 vColor3 = g_DiffuseTexture.Sample(ClampSampler, vTexCoord);
    
    
        vOut = vColor + vColor2 + vColor3;
    }
   
    return vOut;
}

/* 픽셀셰이더 : 픽셀의 색!!!! 을 결정한다. */
PS_OUT PS_MAIN(PS_IN In)// 픽셀 셰이더 메인 함수
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

PS_OUT PS_MODEL(PS_IN In)// 픽셀 셰이뎌 모델 함수
{
	PS_OUT Out = (PS_OUT)0;

	vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);

	if (vDiffuse.a < 0.3f)
		discard;
	
    if (!g_bHited)
        Out.vDiffuse = vDiffuse;
    else
        Out.vDiffuse = vDiffuse * float4(1.f,0.f,0.f,1.f);
    // 피격 여부에 따라 색 변경
    
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	return Out;
}

PS_OUT PS_INVISIBLE_MODEL(PS_IN In)// 픽셀 셰이더 투명 모델 함수
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
    // 특정 블랜드 인덱스 디스카드
        
    Out.vDiffuse = vDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

    return Out;
}

struct PS_OUT_SHADOW
{
    vector vLightDepth : SV_TARGET0;
};// 그림자 셰이더 출력 구조체

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN In) // 픽셀 셰이더 그림자 메인 함수
{
    PS_OUT_SHADOW Out = (PS_OUT_SHADOW) 0;

    Out.vLightDepth = In.vProjPos.w / g_fLightFar;
    
    return Out;
}

PS_OUT PS_MODEL_DISSOLVE(PS_IN In) // 픽셀 셰이더 모델 디졸브 함수
{
    PS_OUT Out = (PS_OUT) 0;

	vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);
    
    float4 DissolveTexture = g_DissolveTexture.Sample(LinearSampler, In.vTexCoord);

    float DissolveDesc = DissolveTexture.r;

    float4 DiffuseTexture1 = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);

    clip(DissolveDesc - g_fDissolveAmount);

    if (g_fDissolveAmount + g_fDissolveGradiationDistance >= DissolveDesc)
    {
        float fLerpRatio = (DissolveDesc - g_fDissolveAmount) / g_fDissolveGradiationDistance;
        vDiffuse = vector(lerp(g_vDissolveGradiationStartColor, g_vDissolveGradiationGoalColor, fLerpRatio), 1.f);
       
    }
    else
    {
        vDiffuse = DiffuseTexture1;
    }

    if (vDiffuse.a < 0.3f)
        discard;
	
    if (!g_bHited)
        Out.vDiffuse = vDiffuse;
    else
        Out.vDiffuse = vDiffuse * float4(1.f, 0.f, 0.f, 1.f);
    
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

    return Out;
}

PS_OUT PS_MODEL_JACK(PS_IN In)// 특정 모델인 잭관련 함수
{
    PS_OUT Out = (PS_OUT) 0;

    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);
    
    
    if (!((vDiffuse.r >= 0.937f && vDiffuse.r <= 0.97f) &&
        (vDiffuse.g >= 0.764f && vDiffuse.g <= 0.938f) &&
        (vDiffuse.b >= 0.031f && vDiffuse.b <= 0.224f)))
    {
        discard;
    }
        
    if (vDiffuse.a < 0.3f)
            discard;
	
    Out.vDiffuse = vDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

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
	}// 기본 모델 렌더링

    pass NonCul_Model //1
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MODEL();
    }// 컬링 없는 모델 렌더링

    pass NonCul_InvisibleModel //2
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INVISIBLE_MODEL();
    }// 컬링 없는 투명 모델 렌더링

    pass Shadow //3
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
    }// 그림자 렌더링

    pass Model_Dissolve //4
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MODEL_DISSOLVE();
    }// 디졸브 효과 모델 렌더링

    pass Model_Jack //5
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MODEL_JACK();
    }// 특정 모델 잭 렌더링
}

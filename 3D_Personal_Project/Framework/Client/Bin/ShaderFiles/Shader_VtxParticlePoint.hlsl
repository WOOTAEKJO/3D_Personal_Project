#include	"Shader_Defines.hlsli"

matrix		g_matWorld, g_matView, g_matProj;
texture2D	g_Texture;

struct VS_IN
{
	float3	vPosition : POSITION;
	float2	vPSize : PSIZE;
	
    row_major float4x4 matWorld : WORLD;
    float4 vColor	  : COLOR0;
};

struct VS_OUT
{
	float4	vPosition : POSITION;
    float2 vPSize : PSIZE;
    float4	vColor : COLOR0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
    
    float4 vPosition = mul(float4(In.vPosition, 1.f), In.matWorld);
	
    Out.vPosition = mul(vPosition, g_matWorld);
	Out.vPSize = In.vPSize;
    Out.vColor = In.vColor;

	return Out;
}

struct GS_IN
{
    float4 vPosition : POSITION;
    float2 vPSize : PSIZE;
    float4 vColor : COLOR0;
};

struct GS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vPSize : PSIZE;
    float4 vColor : COLOR0;
};

void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> OutStream)
{
    GS_OUT Out[4];

    //matrix matTranslation = matrix(In.vRight, In.vUp, In.vLook, In.vPos);
    //float4 vPosition = mul(float4(In.vPosition, 1.f), matTranslation);
	
    //Out.vPosition = mul(vPosition, g_matWorld);
    //Out.vTexCoord = In.vTexCoord;
    //Out.vColor = In.vColor;

}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vPSize : PSIZE;
    float4 vColor : COLOR0;
};

struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

/* 픽셀셰이더 : 픽셀의 색!!!! 을 결정한다. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

   // Out.vColor = g_Texture.Sample(PointSampler, In.vTexCoord);
	
    if (In.vColor.a <0.8f)
        discard;
	
    Out.vColor.a = In.vColor.a;
	
     return Out;
}

technique11 DefaultTechnique
{
	/* 내가 원하는 특정 셰이더들을 그리는 모델에 적용한다. */
	pass Default
	{
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}

}

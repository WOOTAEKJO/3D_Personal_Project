#include	"Shader_Defines.hlsli"

matrix		g_matWorld, g_matView, g_matProj;
texture2D	g_Texture;

vector      g_vCameraPos;
vector      g_vCameraLook;

uint        g_iTextureType;

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
    float3 vUp : TEXCOORD0;
    float3 vRigh : TEXCOORD1;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
    
    vector vPosition = mul(float4(In.vPosition, 1.f), In.matWorld);
	
    Out.vPosition = mul(vPosition, g_matWorld);
    Out.vPSize = float2(In.vPSize.x * In.matWorld._11, In.vPSize.y * In.matWorld._22);
    Out.vColor = In.vColor;
    Out.vUp = float3(In.matWorld._21, In.matWorld._22, In.matWorld._23);
    Out.vRigh = float3(In.matWorld._11, In.matWorld._12, In.matWorld._13);

	return Out;
}

struct GS_IN
{
    float4 vPosition : POSITION;
    float2 vPSize : PSIZE;
    float4 vColor : COLOR0;
    float3 vUp : TEXCOORD0;
    float3 vRigh : TEXCOORD1;

};

struct GS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vColor : COLOR0;
};

[maxvertexcount(6)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> OutStream)
{
    GS_OUT Out[4];
    
    //float4 vLook = g_vCameraPos - In[0].vPosition;
    ////float4 vLook = -g_vCameraLook;
    //vLook = normalize(vLook);
    //float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook.xyz)) * In[0].vPSize.x * 0.5f;
    //float3 vUp = normalize(cross(vLook.xyz, vRight)) * In[0].vPSize.y * 0.5f;
    float4 vLook = -g_vCameraLook;
    vLook = normalize(vLook);
    float3 vRight = normalize(cross(In[0].vUp, vLook.xyz)) * length(In[0].vRigh) * 0.5f;
    float3 vUp = normalize(cross(vLook.xyz, vRight)) * length(In[0].vUp) * 0.5f;
 
    matrix matVP = mul(g_matView, g_matProj);
    
    Out[0].vPosition = mul(float4(In[0].vPosition.xyz + vRight + vUp, 1.f), matVP);
    Out[0].vTexcoord = float2(0.f, 0.f);
    Out[0].vColor = In[0].vColor;
    
    Out[1].vPosition = mul(float4(In[0].vPosition.xyz - vRight + vUp, 1.f), matVP);
    Out[1].vTexcoord = float2(1.f, 0.f);
    Out[1].vColor = In[0].vColor;
    
    Out[2].vPosition = mul(float4(In[0].vPosition.xyz - vRight - vUp, 1.f), matVP);
    Out[2].vTexcoord = float2(1.f, 1.f);
    Out[2].vColor = In[0].vColor;
    
    Out[3].vPosition = mul(float4(In[0].vPosition.xyz + vRight - vUp, 1.f), matVP);
    Out[3].vTexcoord = float2(0.f, 1.f);
    Out[3].vColor = In[0].vColor;
    
    OutStream.Append(Out[0]);
    OutStream.Append(Out[1]);
    OutStream.Append(Out[2]);
    OutStream.RestartStrip();
    
    OutStream.Append(Out[0]);
    OutStream.Append(Out[2]);
    OutStream.Append(Out[3]);
    OutStream.RestartStrip();
    
}

[maxvertexcount(6)]
void GS_MAIN_SOLID(point GS_IN In[1], inout TriangleStream<GS_OUT> OutStream)
{
    GS_OUT Out[4];
    
    float4 vLook = -g_vCameraLook;
    vLook = normalize(vLook);
    //float3 vRight = normalize(cross(In[0].vUp, vLook.xyz)) * In[0].vPSize.x * 0.5f;
    //float3 vUp = normalize(cross(vLook.xyz, vRight)) * In[0].vPSize.y * 0.5f;
    float3 vRight = normalize(cross(In[0].vUp, vLook.xyz)) * length(In[0].vRigh) * 0.5f;
    float3 vUp = normalize(cross(vLook.xyz, vRight)) * length(In[0].vUp) * 0.5f;
    //float3 vRight = normalize(cross(In[0].vUp, vLook.xyz)) * length(In[0].vRigh);
    //float3 vUp = normalize(cross(vLook.xyz, vRight)) * length(In[0].vUp);

    matrix matVP = mul(g_matView, g_matProj);
    
    Out[0].vPosition = mul(float4(In[0].vPosition.xyz + vRight + vUp, 1.f), matVP);
    Out[0].vTexcoord = float2(0.f, 0.f);
    Out[0].vColor = In[0].vColor;
    
    Out[1].vPosition = mul(float4(In[0].vPosition.xyz - vRight + vUp, 1.f), matVP);
    Out[1].vTexcoord = float2(1.f, 0.f);
    Out[1].vColor = In[0].vColor;
    
    Out[2].vPosition = mul(float4(In[0].vPosition.xyz - vRight - vUp, 1.f), matVP);
    Out[2].vTexcoord = float2(1.f, 1.f);
    Out[2].vColor = In[0].vColor;
    
    Out[3].vPosition = mul(float4(In[0].vPosition.xyz + vRight - vUp, 1.f), matVP);
    Out[3].vTexcoord = float2(0.f, 1.f);
    Out[3].vColor = In[0].vColor;
    
    OutStream.Append(Out[0]);
    OutStream.Append(Out[1]);
    OutStream.Append(Out[2]);
    OutStream.RestartStrip();
    
    OutStream.Append(Out[0]);
    OutStream.Append(Out[2]);
    OutStream.Append(Out[3]);
    OutStream.RestartStrip();
    
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
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

    vector vColor = g_Texture.Sample(PointSampler, In.vTexcoord);
    
    
    if (vColor.a < 0.6f)
        discard;
        
    Out.vColor = vColor * In.vColor;
  
    return Out;
}

PS_OUT PS_MAIN_SOLID(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vColor = g_Texture.Sample(PointSampler, In.vTexcoord);
    
    if (vColor.a < 0.3f)
        discard;
        
    Out.vColor = In.vColor;
    
    return Out;
}

PS_OUT PS_MAIN_BLEND(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    vector vColor = g_Texture.Sample(PointSampler, In.vTexcoord) * In.vColor;
    
    
    if (vColor.a < 0.1f)
        discard;
        
    Out.vColor = vColor;
   
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

    pass Solid_Color
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN_SOLID();
        PixelShader = compile ps_5_0 PS_MAIN_SOLID();
    }

    pass Blend_Color
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN_SOLID();
        PixelShader = compile ps_5_0 PS_MAIN_BLEND();
    }

}

#include	"Shader_Defines.hlsli"

matrix		g_matWorld, g_matView, g_matProj;
texture2D	g_Texture[2];

texture2D g_DiffuseTexture;
texture2D g_DepthTexture;
texture2D g_MaskTexture;
texture2D g_NoiseTexture;
texture2D g_DissolveTexture;

vector g_vSolid_Color;
//vector g_vSub_Color;

float g_fAlpha;

vector g_vCenter;
float g_fRadius;
float g_fCompare_Radius;

float g_fTimeDelta;

bool g_bRevers;

float g_fDissolveAmount;
float g_fDissolveGradiationDistance;
float3 g_vDissolveGradiationStartColor;
float3 g_vDissolveGradiationGoalColor;

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
/* ����� ������ ��� .*/

/* ������ȯ : /w */ /* -> -1, 1 ~ 1, -1 */
/* ����Ʈ��ȯ-> 0, 0 ~ WINSX, WINSY */
/* �����Ͷ����� : ���������� ����Ͽ� �ȼ��� ������ �����. */

struct PS_IN
{
	float4	vPosition : SV_POSITION;
	float2	vTexCoord : TEXCOORD0;
};

struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

/* �ȼ����̴� : �ȼ��� ��!!!! �� �����Ѵ�. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	//vector vSColor = g_Texture[0].Sample(LinearSampler, In.vTexCoord);
	//vector vDColor = g_Texture[1].Sample(LinearSampler, In.vTexCoord);
   
    float4 vResultColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);

    if (vResultColor.a <0.3f)
        discard;
    
    Out.vColor = vResultColor;
    
    return Out;
}

struct VS_OUT_EFFECT
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float4 vWorldPos : TEXCOORD2;
};

VS_OUT_EFFECT VS_MAIN_EFFECT(VS_IN In)
{
    VS_OUT_EFFECT Out = (VS_OUT_EFFECT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_matWorld, g_matView);
    matWVP = mul(matWV, g_matProj);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexCoord = In.vTexCoord;
    Out.vProjPos = Out.vPosition;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_matWorld);

    return Out;
}

struct PS_IN_EFFECT
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float4 vWorldPos : TEXCOORD2;
};

PS_OUT PS_MAIN_EFFECT(PS_IN_EFFECT In)
{
    PS_OUT Out = (PS_OUT) 0;

    float4 vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);
    
    if (vColor.a < 0.3f)
        discard;
    
    Out.vColor = vColor; //  * g_vSolid_Color;
  
    //float2 vDepthTexcoord;
    //vDepthTexcoord.x = (In.vPosition.x / In.vPosition.w) * 0.5f + 0.5f;
    //vDepthTexcoord.y = (In.vPosition.y / In.vPosition.w) * -0.5f + 0.5f;
	
    //float4 vDepthDesc = g_DepthTexture.Sample(PointSampler, vDepthTexcoord);
	
    //Out.vColor.a = Out.vColor.a * (vDepthDesc.y * 1000.f - In.vProjPos.w) * 2.f;
	// ȭ�鿡 �׷��� �ȼ����� ���� ���� ���ؼ� ���� ���� ũ�� ���İ� ũ�� ������ ���İ� ���� �۾����� ����������.
	
        return Out;
}

PS_OUT PS_MAIN_EFFECT_SOLID(PS_IN_EFFECT In)
{
    PS_OUT Out = (PS_OUT) 0;

    float4 vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);
    
    //Out.vColor = g_vSolid_Color;
   // Out.vColor.a = vColor.a;
	
    if (vColor.a < 0.3f)
        discard;
    
    vColor = g_vSolid_Color;
	
    Out.vColor = vColor;
    
    return Out;
}

PS_OUT PS_MAIN_EFFECT_INVISIBILITY(PS_IN_EFFECT In)
{
    PS_OUT Out = (PS_OUT) 0;

    float4 vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexCoord);
    
    if (vColor.a < 0.8f)
        discard;
    
    vColor.a = max(vColor.a - g_fAlpha, 0.f);
    
    Out.vColor = vColor;

    return Out;
}

PS_OUT PS_MAIN_EFFECT_REAPER(PS_IN_EFFECT In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float4 vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexCoord * 2.f);
    
    if (vColor.a < 0.8f)
        discard;
   
    vColor.a = max(vColor.a - g_fAlpha, 0.f);
    
    vector vDir = g_vCenter - In.vWorldPos;
   
    float vDistance = length(vDir);
    
    if (vDistance>g_fRadius)
        discard;
   
    Out.vColor = vColor * g_vSolid_Color;
    
    return Out;
}

PS_OUT PS_MAIN_EFFECT_BLEND(PS_IN_EFFECT In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float4 vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexCoord);
    
    if (vColor.x < 0.4f && vColor.y < 0.4f && vColor.z < 0.4f)
    {
        vColor.a = 0.1f;

    }
    
    if (vColor.a < 0.3f)
        discard;
   
    Out.vColor = vColor * g_vSolid_Color;
    
    return Out;
}

PS_OUT PS_MAIN_EFFECT_BLEND_INVISIBILITY(PS_IN_EFFECT In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float4 vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexCoord);
    
    if (vColor.x < 0.3f && vColor.y < 0.3f && vColor.z < 0.3f)
    {
        
        if (vColor.x < 0.15f && vColor.y < 0.15f && vColor.z < 0.15f)
            discard;
        else
            vColor = g_vSolid_Color;
    }

    if (vColor.a < 0.1f)
        discard;
       
    vColor.a = min(max(vColor.a - g_fAlpha, 0.f),1.f);
    
    Out.vColor = vColor * g_vSolid_Color;
    
    return Out;
}

PS_OUT PS_MAIN_EFFECT_TARGET(PS_IN_EFFECT In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float4 vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexCoord * 2.f);
    
    if (vColor.a < 0.8f)
        discard;
   
    vColor.a = max(vColor.a - g_fAlpha, 0.f);
    
    vector vDir = g_vCenter - In.vWorldPos;
   
    float vDistance = length(vDir);
    
    if (vDistance < g_fRadius || vDistance>g_fCompare_Radius)
        discard;
   
    Out.vColor = vColor * g_vSolid_Color;
    
    return Out;
}

PS_OUT PS_MAIN_EFFECT_WATER(PS_IN_EFFECT In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float4 vColor = g_DiffuseTexture.Sample(LinearSampler, (In.vTexCoord * 10.f) + g_fTimeDelta * 0.04f);
    
    if (vColor.a < 0.8f)
        discard;
    
    // 0.0196555f
    if (vColor.r < 0.0196078432f)
    {
        vColor.a = max(vColor.a - g_fAlpha * 1.5f, 0.f);
    }else
        vColor.a = max(vColor.a - g_fAlpha, 0.f);
  
    Out.vColor = vColor;
    
    return Out;
}

PS_OUT PS_MAIN_TRAIL(PS_IN_EFFECT In)
{
    PS_OUT Out = (PS_OUT) 0;

    float4 vMask = g_MaskTexture.Sample(LinearSampler, In.vTexCoord);
   
    float4 vColor = g_vSolid_Color;
   
    if (g_bRevers)
    {
        vColor.a = (1.f - vMask.x);
    }
    else
    {
        vColor.a = vMask.x;
    }
    
    if (vColor.a < g_fAlpha)
        discard;
	
    Out.vColor = vColor;
    
    return Out;
}

PS_OUT PS_MAIN_HP(PS_IN_EFFECT In)
{
    PS_OUT Out = (PS_OUT)0;

    float2 vUV = In.vTexCoord * 0.08f;
    vUV.y += g_fTimeDelta * 0.01f;

    float4 vNoiseColor = g_NoiseTexture.Sample(LinearSampler, vUV);
    float4 vMask = g_MaskTexture.Sample(LinearSampler, In.vTexCoord);

    float4 vSubColor = float4(0.6f, 0.2f, 0.2f, 0.3f);

    float4 vColor;

    float2 vUV2 = In.vTexCoord;

    if (vUV2.x <= g_fRadius)
    {
        vColor = g_vSolid_Color;
    }
    else {
        vColor = vSubColor;

    }
    vColor.rgb *= vNoiseColor.rgb * 2 + 1;
    vColor.a *= vMask.x;

    if (vMask.x < 1.f)
    {
        vColor = float4(0.f, 0.f, 0.f, vColor.a);
    }

    if (vColor.a < 0.3f)
        discard;

    if (vColor.x == 0.f)
        vColor.a = 1.f;

    Out.vColor = vColor;

    return Out;
}

PS_OUT PS_MAIN_DISSOLVE(PS_IN In)
{
    PS_OUT Out = (PS_OUT)0;

    
    float4 DissolveTexture = g_DissolveTexture.Sample(LinearSampler, In.vTexCoord + g_fTimeDelta * 0.01f);

    float DissolveDesc = DissolveTexture.r;

    float4 DiffuseTexture1 = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);
    float4 DiffuseTexture2 = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord + DissolveDesc* g_fDissolveAmount);

    float4 DiffuseTexture;

    clip(DissolveDesc - g_fDissolveAmount);

    if (g_fDissolveAmount + g_fDissolveGradiationDistance >= DissolveDesc)
    {
        float fLerpRatio = (DissolveDesc - g_fDissolveAmount) / g_fDissolveGradiationDistance;
        Out.vColor = vector(lerp(g_vDissolveGradiationStartColor, g_vDissolveGradiationGoalColor, fLerpRatio), 1.f);
       
    }
    else {
        Out.vColor = DiffuseTexture2;
    }

    if (DiffuseTexture1.a < 0.3f)
        discard;

    return Out;
}

technique11 DefaultTechnique
{
	/* ���� ���ϴ� Ư�� ���̴����� �׸��� �𵨿� �����Ѵ�. */
	pass Default //0
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

    pass Effect //1
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_EFFECT();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_EFFECT();
    }

    pass Effect_Solid //2
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 1.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_EFFECT();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_EFFECT_SOLID();
    }

    pass Effect_Invisibility //3
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_EFFECT();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_EFFECT_INVISIBILITY();
    }

    pass Effect_Reaper //4
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_EFFECT();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_EFFECT_REAPER();
    }

    pass Effect_Blend //5
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_EFFECT();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_EFFECT_BLEND();
    }

    pass Effect_Blend_Invisibility //6
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_EFFECT();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_EFFECT_BLEND_INVISIBILITY();
    }

    pass Effect_Target //7
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_EFFECT();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_EFFECT_TARGET();
    }

    pass Effect_Water //8
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_EFFECT();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_EFFECT_WATER();
    }

    pass Effect_Trail // 9
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 1.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_EFFECT();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_TRAIL();
    }

    pass Effect_HP // 10
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 1.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_EFFECT();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_HP();
    }

    pass Effect_Dissolve // 11
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 1.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_EFFECT();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DISSOLVE();
    }

}

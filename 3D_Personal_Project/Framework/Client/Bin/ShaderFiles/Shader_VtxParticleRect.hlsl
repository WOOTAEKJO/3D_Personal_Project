#include	"Shader_Defines.hlsli"

matrix		g_matWorld, g_matView, g_matProj;
texture2D	g_Texture;

struct VS_IN
{
	float3	vPosition : POSITION;
	float2	vTexCoord : TEXCOORD0;
	
    float4	vRight	  : TEXCOORD1;
    float4	vUp		  : TEXCOORD2;
    float4	vLook	  : TEXCOORD3;
    float4	vPos	  : TEXCOORD4;
    float4 vColor	  : COLOR0;
};

struct VS_OUT
{
	float4	vPosition : SV_POSITION;
	float2	vTexCoord : TEXCOORD0;
    float4	vColor : COLOR0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

    matrix matTranslation = matrix(In.vRight, In.vUp, In.vLook, In.vPos);
    float4 vPosition = mul(float4(In.vPosition, 1.f), matTranslation);
	
	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

    Out.vPosition = mul(vPosition, matWVP);
	Out.vTexCoord = In.vTexCoord;
    Out.vColor = In.vColor;

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
    float4	vColor : COLOR0;
};

struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

/* �ȼ����̴� : �ȼ��� ��!!!! �� �����Ѵ�. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

    Out.vColor = g_Texture.Sample(PointSampler, In.vTexCoord);
	
    if (In.vColor.a <0.8f)
        discard;
	
    Out.vColor.a = In.vColor.a;
	
     return Out;
}

technique11 DefaultTechnique
{
	/* ���� ���ϴ� Ư�� ���̴����� �׸��� �𵨿� �����Ѵ�. */
	pass Default
	{
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}

}

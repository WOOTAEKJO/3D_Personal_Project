#include	"Shader_Defines.hlsli"

matrix		g_matWorld,g_matView, g_matProj;
// ����, ��, ���� ���

matrix		g_BlendMatrix[256];
// ���� ��� �迭

texture2D	g_DiffuseTexture; // ��ǻ�� �ؽ���
texture2D g_DissolveTexture; // ������ �ؽ���

// ������ ȿ���� ���Ǵ� ������
float g_fDissolveAmount;
float g_fDissolveGradiationDistance;
float3 g_vDissolveGradiationStartColor;
float3 g_vDissolveGradiationGoalColor;
	
int2		g_iDiscardIndx; // Ư�� ���� �޽��� �����ϰ� �ϱ� ����
// �ϴ� ����ϰ� �������� �ʹ� ��ȿ�����̴�. Ư�� �� �޽��� �����ؼ� ����

bool        g_bHited; // �ǰ� ����

float       g_fLightFar; // �������� �ִ� �Ÿ�

float2 rcpres = { 0.0025, 0.0033333333333333333333333333333333 }; // �� �ػ�

// �� Ŀ���� ���� �� ���� ��ġ
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

}; // �� ����ġ

struct VS_IN
{
	float3	vPosition : POSITION; // ��ġ
	float3  vTangent : TANGENT; // ź��Ʈ
	float3	vNormal	: NORMAL; // ���
	float2	vTexCoord : TEXCOORD0; // �ؽ��� ��ǥ

	uint4	vBlendIndices : BLENDINDEX; // ���� �ε���
	float4  vBlendWeights : BLENDWEIGHT; // ���� ����ġ
}; // ���� ���̴� �Է� ����ü

struct VS_OUT
{
	float4	vPosition : SV_POSITION; // ��ġ
	float4  vTangent : TANGENT; // ź��Ʈ
	float4	vNormal	: NORMAL; // ���
	float2	vTexCoord : TEXCOORD0; // �ؽ��� ��ǥ
	float4	vWorldPos : TEXCOORD1; // ���� ��ǥ
    float4	vProjPos : TEXCOORD2; // ���� ��ǥ
	
    uint4 vBlendIndices : BLENDINDEX; // ���� �ε���
}; // ���� ���̴� ��� ����ü

VS_OUT VS_MAIN(VS_IN In) // ���� ���̴� ���� �Լ�
{
	VS_OUT Out = (VS_OUT)0;

	float fWeightW = 1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z);
    // ���� ����ġ ���

	matrix matBlend = g_BlendMatrix[In.vBlendIndices.x] * In.vBlendWeights.x +
		g_BlendMatrix[In.vBlendIndices.y] * In.vBlendWeights.y +
		g_BlendMatrix[In.vBlendIndices.z] * In.vBlendWeights.z +
		g_BlendMatrix[In.vBlendIndices.w] * fWeightW;
    // ���� ��� ���

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
/* ����� ������ ��� .*/

/* ������ȯ : /w */ /* -> -1, 1 ~ 1, -1 */
/* ����Ʈ��ȯ-> 0, 0 ~ WINSX, WINSY */
/* �����Ͷ����� : ���������� ����Ͽ� �ȼ��� ������ �����. */

struct PS_IN
{
	float4	vPosition : SV_POSITION; // ��ġ
	float4  vTangent : TANGENT; // ź��Ʈ
	float4	vNormal	: NORMAL; // ���
	float2	vTexCoord : TEXCOORD0; // �ؽ��� ��ǥ
	float4	vWorldPos : TEXCOORD1; // ���� ��ǥ
    float4	vProjPos : TEXCOORD2; // ���� ��ǥ
	
    uint4 vBlendIndices : BLENDINDEX; // ���� �ε���
};// �ȼ� ���̴� �Է� ����ü

struct PS_OUT
{
	float4	vDiffuse : SV_TARGET0; // ��ǻ��
    float4	vNormal: SV_TARGET1; // ���
    float4	vDepth : SV_TARGET2; // ����
};// �ȼ� ���̴� ��� ����ü

float4 PSBlur(float2 vTexCoord) // �� �Լ�
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

/* �ȼ����̴� : �ȼ��� ��!!!! �� �����Ѵ�. */
PS_OUT PS_MAIN(PS_IN In)// �ȼ� ���̴� ���� �Լ�
{
	PS_OUT Out = (PS_OUT)0;

    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);

    Out.vDiffuse = vDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f,0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
    // In.vProjPos.z / In.vProjPos.w -> ���� �����̽� �� z ��
    // In.vProjPos.w -> �� �����̽� �� z ��
    // (/ 1000.f) �� ���� �ȼ� �ɼ��� �뷮�� �������̱� ����
    
	return Out;
}

PS_OUT PS_MODEL(PS_IN In)// �ȼ� ���̵� �� �Լ�
{
	PS_OUT Out = (PS_OUT)0;

	vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);

	if (vDiffuse.a < 0.3f)
		discard;
	
    if (!g_bHited)
        Out.vDiffuse = vDiffuse;
    else
        Out.vDiffuse = vDiffuse * float4(1.f,0.f,0.f,1.f);
    // �ǰ� ���ο� ���� �� ����
    
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	return Out;
}

PS_OUT PS_INVISIBLE_MODEL(PS_IN In)// �ȼ� ���̴� ���� �� �Լ�
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
    } // �ϴ� ����ϰ� �������� �ʹ� ��ȿ�����̴�. Ư�� �� �޽��� �����ؼ� ����
    // Ư�� ���� �ε��� ��ī��
        
    Out.vDiffuse = vDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

    return Out;
}

struct PS_OUT_SHADOW
{
    vector vLightDepth : SV_TARGET0;
};// �׸��� ���̴� ��� ����ü

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN In) // �ȼ� ���̴� �׸��� ���� �Լ�
{
    PS_OUT_SHADOW Out = (PS_OUT_SHADOW) 0;

    Out.vLightDepth = In.vProjPos.w / g_fLightFar;
    
    return Out;
}

PS_OUT PS_MODEL_DISSOLVE(PS_IN In) // �ȼ� ���̴� �� ������ �Լ�
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

PS_OUT PS_MODEL_JACK(PS_IN In)// Ư�� ���� ����� �Լ�
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
	/* ���� ���ϴ� Ư�� ���̴����� �׸��� �𵨿� �����Ѵ�. */
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
	}// �⺻ �� ������

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
    }// �ø� ���� �� ������

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
    }// �ø� ���� ���� �� ������

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
    }// �׸��� ������

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
    }// ������ ȿ�� �� ������

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
    }// Ư�� �� �� ������
}

struct VS_INPUT {
	float4 position: POSITION0;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
};

struct VS_OUTPUT {
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;//TEXCOORD1
	float3 direction_to_camera: TEXCOORD1;//TEXCOORD1
};

cbuffer constant: register(b0) {
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	float m_time;
	float m_lerp_speed;
	float4 m_light_direction;
	float4 m_camera_position;
	int isLit;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	// World Space
	output.position = mul(input.position, m_world);
	output.direction_to_camera = normalize(output.position.xyz - m_camera_position.xyz);//both -xyz and normalize the vector
	// View Space
	output.position = mul(output.position, m_view);
	// Screen Space
	output.position = mul(output.position, m_proj);

	output.texcoord = input.texcoord;
	output.normal = input.normal;
	return output;
}
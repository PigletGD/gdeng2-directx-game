struct VS_INPUT {
	float4 position: POSITION;
	float3 color: COLOR;
};

struct VS_OUTPUT {
	float4 position: SV_POSITION;
	float3 color: COLOR;
};

cbuffer constant: register(b0) {
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	float m_time;
	float m_lerp_speed;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = input.position;

	// World Space
	output.position = mul(output.position, m_world);
	// View Space
	output.position = mul(output.position, m_view);
	// Screen Space
	output.position = mul(output.position, m_proj);

	output.color = input.color;

	return output;
}
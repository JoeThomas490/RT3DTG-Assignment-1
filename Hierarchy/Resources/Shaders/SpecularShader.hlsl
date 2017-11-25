//////////////////////////////////////////////////////////////////////
// HLSL File:
// This example is compiled using the fxc shader compiler.
// It is possible directly compile HLSL in VS2013
//////////////////////////////////////////////////////////////////////

// This first constant buffer is special.
// The framework looks for particular variables and sets them automatically.
// See the CommonApp comments for the names it looks for.
cbuffer CommonApp
{
	float4x4 g_WVP;
	float4 g_constantColour;
	float4 g_specularColour;
	float g_specularPower;
	float4 g_lightDirections[MAX_NUM_LIGHTS];
	float3 g_lightColours[MAX_NUM_LIGHTS];
	int g_numLights;
	float4x4 g_InvXposeW;
	float4x4 g_W;
};


// When you define your own cbuffer you can use a matching structure in your app but you must be careful to match data alignment.
// Alternatively, you may use shader reflection to find offsets into buffers based on variable names.
// The compiler may optimise away the entire cbuffer if it is not used but it shouldn't remove indivdual variables within it.
// Any 'global' variables that are outside an explicit cbuffer go
// into a special cbuffer called "$Globals". This is more difficult to work with
// because you must use reflection to find them.
// Also, the compiler may optimise individual globals away if they are not used.
cbuffer MyApp
{
	float3 g_cameraPosition;
};

// VSInput structure defines the vertex format expected by the input assembler when this shader is bound.
// You can find a matching structure in the C++ code.
struct VSInput
{
	float4 pos:POSITION;
	float4 colour:COLOUR0;
	float3 normal:NORMAL;
};

// PSInput structure is defining the output of the vertex shader and the input of the pixel shader.
// The variables are interpolated smoothly across triangles by the rasteriser.
struct PSInput
{
	float4 pos:SV_Position;
	float4 colour:COLOUR0;
	float3 normal:NORMAL;
	float3 viewDirection:TEXCOORD0;
};

// PSOutput structure is defining the output of the pixel shader, just a colour value.
struct PSOutput
{
	float4 colour:SV_Target;
};


// The vertex shader entry point. This function takes a single vertex and transforms it for the rasteriser.
void VSMain(const VSInput input, out PSInput output)
{
	output.pos = mul(input.pos, g_WVP);
	output.colour = input.colour * g_constantColour;
	output.normal = mul(input.normal, (float3x3)g_W);

	//Get world position of vertex
	float4 worldPosition = mul(input.pos, g_W);
	output.viewDirection = g_cameraPosition.xyz - worldPosition.xyz;
	output.viewDirection = normalize(output.viewDirection);
}

// The pixel shader entry point. This function writes out the fragment/pixel colour.
void PSMain(const PSInput input, out PSOutput output)
{

	float4 newColour = float4(input.colour.rgb, 1.0);

	//Lighting loop
	float I = 0;
	for (int i = 0; i < g_numLights; i++)
	{
		float3 lightDir = float3(g_lightDirections[i].xyz);
		float lightIntensity = dot(lightDir, input.normal);
		float4 specular = float4(0.0, 0.0, 0.0, 0.0);

		float dist = distance(input.pos.xyz,lightDir);

		I += lightIntensity * g_lightColours[i].xyz;

		if (lightIntensity > 0.0)
		{
			float att = clamp(1.0 - dist / 1.0, 0.0, 1.0); 
			att *= att;

			//Calculate specular reflection component
			float3 reflection = normalize(2 * lightIntensity * input.normal - lightDir);

			//Calculate specular component
			specular = float4(1.0,1.0,1.0,1.0) * pow(saturate(dot(reflection, input.viewDirection)), 64);
		}

		newColour = saturate(newColour + specular);
	}

	newColour *= I;
	newColour *= 2.5;
	newColour = saturate(newColour);

	output.colour = float4(newColour.rgb, 1.0);
}
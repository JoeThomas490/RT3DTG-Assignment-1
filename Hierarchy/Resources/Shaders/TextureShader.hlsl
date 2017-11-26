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
	float	g_frameCount;
}


// VSInput structure defines the vertex format expected by the input assembler when this shader is bound.
// You can find a matching structure in the C++ code.
struct VSInput
{
	float4 pos:POSITION;
	float4 colour:COLOUR0;
	float3 normal:NORMAL;
	float2 tex:TEXCOORD;
};

// PSInput structure is defining the output of the vertex shader and the input of the pixel shader.
// The variables are interpolated smoothly across triangles by the rasteriser.
struct PSInput
{
	float4 pos:SV_Position;
	float4 colour:COLOUR0;
	float3 normal:NORMAL;
	float2 tex:TEXCOORD;
	float4 mat:COLOUR1;
};

// PSOutput structure is defining the output of the pixel shader, just a colour value.
struct PSOutput
{
	float4 colour:SV_Target;
};

// Define several Texture 'slots'
Texture2D g_materialMap;
//Moss
Texture2D g_texture0;
//Grass
Texture2D g_texture1;
//Asphalt
Texture2D g_texture2;


// Define a state setting 'slot' for the sampler e.g. wrap/clamp modes, filtering etc.
SamplerState g_sampler;

// The vertex shader entry point. This function takes a single vertex and transforms it for the rasteriser.
void VSMain(const VSInput input, out PSInput output)
{
	//Modify the vertice's Y position, using the frame count and sin to create a wave effect
	//float3 inPos = input.pos.xyz * float3(1.0, sin(g_frameCount * 0.025 + input.pos.x * 0.01), 1.0);

	//output.pos = mul(float4(inPos, 1.0), g_WVP);
	output.pos = mul(input.pos, g_WVP);
	output.normal = input.normal;

	uint width, height;

	g_materialMap.GetDimensions(width, height);
	//output.colour = float4(newColour, 1.0);
	float2 samplePos = (input.pos.xz + width) / (width * 2);
	samplePos.y = 1 - samplePos.y;
	output.colour = g_materialMap.SampleLevel(g_sampler, samplePos, 0);

	output.tex = input.tex;
}

// The pixel shader entry point. This function writes out the fragment/pixel colour.
void PSMain(const PSInput input, out PSOutput output)
{

	/*float3 lightDir = normalize(float3(1, 1, 1));
	float I = dot(lightDir, input.normal.xyz);*/

	//float3 newColour = float3(input.colour.x, input.colour.y * 0.01, input.colour.z) * I;
	//float3 newColour = g_texture0.Sample(g_sampler, input.tex) * I;

	float3 mossCol = g_texture0.Sample(g_sampler, input.tex);
	float3 grassCol = g_texture1.Sample(g_sampler, input.tex);
	float3 asphaltCol = g_texture2.Sample(g_sampler, input.tex);

	float redPercent = input.colour.r / 255.0;
	float greenPercent = input.colour.g / 255.0;
	float bluePercent = input.colour.b / 255.0;

	//Add lighting loop
	float I = 0;
	for (int i = 0; i < g_numLights; i++)
	{
		float3 lightDir = float3(g_lightDirections[i].xyz);
		I += dot(lightDir, input.normal.xyz);
		I *= g_lightColours[i];
	}


	//Try and simplify with swizzling
	float3 newColour = float3(0.0, 0.0, 0.0);
	newColour.r = lerp(newColour.r, mossCol.r, input.colour.r) + lerp(newColour.r, grassCol.r, input.colour.g) + lerp(newColour.r, asphaltCol.r, input.colour.b);
	newColour.g = lerp(newColour.g, mossCol.g, input.colour.r) + lerp(newColour.g, grassCol.g, input.colour.g) + lerp(newColour.g, asphaltCol.g, input.colour.b);
	newColour.b = lerp(newColour.b, mossCol.b, input.colour.r) + lerp(newColour.b, grassCol.b, input.colour.g) + lerp(newColour.b, asphaltCol.b, input.colour.b);

	//output.colour = float4(abs(cos(g_frameCount * 0.01)) * newColour.r ,abs(sin(g_frameCount*0.01)) * newColour.g, newColour.b,1);	// 'return' the colour value for this fragment.

	

	newColour *= I * max(0.075, abs(cos(g_frameCount * 0.001)));
	//newColour *= I;
	//float greyscale = dot(newColour, float3(0.3, 0.59, 0.11));
	//float3 sampleCol = lerp(newColour, newColour * float3(pow(greyscale * 3.0, 6), pow(greyscale * 3.0, 6), pow(greyscale * 3.0, 3)), abs(sin(g_frameCount * 0.001)));

	//// Merge and tint


	output.colour = float4(newColour.rgb, 1.0);
}
#include "core.h"

#define MRT @shGlobalSettingBool(mrt_output)


#ifdef SH_VERTEX_SHADER

    SH_BEGIN_PROGRAM
        shUniform(float4x4, wvp) @shAutoConstant(wvp, worldviewproj_matrix)
        shInput(float2, uv0)
        shOutput(float2, UV)

    SH_START_PROGRAM
    {
	    shOutputPosition = shMatrixMult(wvp, shInputPosition);
	    UV = uv0;
    }

#else

    SH_BEGIN_PROGRAM
		shSampler2D(diffuseMap)
		shInput(float2, UV)
#if MRT
        shDeclareMrtOutput(1)
#endif
        shUniform(float4, materialDiffuse)                    @shAutoConstant(materialDiffuse, surface_diffuse_colour)
        shUniform(float4, materialEmissive)                   @shAutoConstant(materialEmissive, surface_emissive_colour)
        
        shUniform(float4, atmosphereColour)                   @shSharedParameter(atmosphereColour)

    SH_START_PROGRAM
    {
        
        float4 tex = shSample(diffuseMap, UV);
        
        shOutputColour(0) = float4(materialEmissive.xyz, 1) * tex;
        
        // use a circle for the alpha (compute UV distance to center)
        // looks a bit bad because it's not filtered on the edges,
        // but cheaper than a seperate alpha texture.
        float sqrUVdist = pow(UV.x-0.5,2) + pow(UV.y-0.5, 2);
        shOutputColour(0).a = materialDiffuse.a * (sqrUVdist >= 0.24 ? 0 : 1);
        shOutputColour(0).rgb += (1-tex.a) * shOutputColour(0).a * atmosphereColour.rgb; //fill dark side of moon with atmosphereColour
        shOutputColour(0).rgb += (1-materialDiffuse.a) * atmosphereColour.rgb; //fade bump

#if MRT
        shOutputColour(1) = float4(1,1,1,1);
#endif
    }

#endif

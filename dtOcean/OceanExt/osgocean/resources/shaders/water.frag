uniform bool osgOcean_EnableReflections;
uniform bool osgOcean_EnableRefractions;
uniform bool osgOcean_EnableCrestFoam;

uniform bool osgOcean_EnableDOF;
uniform bool osgOcean_EnableGlare;

uniform float osgOcean_DOF_Near;
uniform float osgOcean_DOF_Focus;
uniform float osgOcean_DOF_Far;
uniform float osgOcean_DOF_Clamp;
uniform float osgOcean_FresnelMul;

uniform samplerCube osgOcean_EnvironmentMap;
uniform sampler2D   osgOcean_ReflectionMap;
uniform sampler2D   osgOcean_RefractionMap;
uniform sampler2D   osgOcean_RefractionDepthMap;
uniform sampler2D   osgOcean_FoamMap;
uniform sampler2D   osgOcean_NoiseMap;
uniform sampler2D   osgOcean_Heightmap;

uniform float osgOcean_UnderwaterFogDensity;
uniform float osgOcean_AboveWaterFogDensity;
uniform vec4  osgOcean_UnderwaterFogColor;
uniform vec4  osgOcean_AboveWaterFogColor;

uniform mat4 osg_ViewMatrixInverse;

uniform mat4 osgOcean_RefractionInverseTransformation;

uniform vec2 osgOcean_ViewportDimensions;

uniform float osgOcean_WaterHeight;
uniform float osgOcean_FoamCapBottom;
uniform float osgOcean_FoamCapTop;

varying vec3 vNormal;
varying vec3 vViewerDir;
varying vec3 vLightDir;
varying vec4 vVertex;
varying vec4 vWorldVertex;

varying vec3 vWorldViewDir;
varying vec3 vWorldNormal;

varying float height;

mat4 worldObjectMatrix;

const float shininess = 2000.0;

// Extinction level for red, green and blue light in ocean water
// (maybe this should be changed into a user configurable shader uniform?)
// Values are taken from "Rendering Water as Post-process Effect", Wojciech Toman
// http://www.gamedev.net/reference/programming/features/ppWaterRender/
// vec4 colorExtinction = vec4(4.5, 75.0, 300.0, 1.0) * 5.0;
const vec4 oneOverColorExtinction = vec4(1.0/22.5, 1.0/375.0, 1.0/1500, 1.0/5.0);

// The amount of light extinction,
// higher values means that less light is transmitted through the water
const float oneOverLightExtinction = 1.0/60.0;

vec4 distortGen( vec4 v, vec3 N )
{
    // transposed
    const mat4 mr =
        mat4( 0.5, 0.0, 0.0, 0.0,
              0.0, 0.5, 0.0, 0.0,
              0.0, 0.0, 0.5, 0.0,
              0.5, 0.5, 0.5, 1.0 );

    mat4 texgen_matrix = mr * gl_ProjectionMatrix * gl_ModelViewMatrix;

    //float disp = 8.0;
    float disp = 4.0;

    vec4 tempPos;

    tempPos.xy = v.xy + disp * N.xy;
    tempPos.z  = v.z;
    tempPos.w  = 1.0;

    return texgen_matrix * tempPos;
}

vec3 reorientate( vec3 v )
{
    float y = v.y;

    v.y = -v.z;
    v.z = y;

    return v;
}

mat3 getLinearPart( mat4 m )
{
    mat3 result;

    result[0][0] = m[0][0];
    result[0][1] = m[0][1];
    result[0][2] = m[0][2];

    result[1][0] = m[1][0];
    result[1][1] = m[1][1];
    result[1][2] = m[1][2];

    result[2][0] = m[2][0];
    result[2][1] = m[2][1];
    result[2][2] = m[2][2];

    return result;
}

vec4 computeCubeMapColor( vec3 N, vec4 V, vec3 E )
{
    mat3 worldObjectMat3x3 = getLinearPart( worldObjectMatrix );
    vec4 world_pos = worldObjectMatrix *  V;

    vec3 normal = normalize( worldObjectMat3x3 * N );
    vec3 eye = normalize( world_pos.xyz - E );

    vec3 coord = reflect( eye, normal );

    vec3 reflection_vector = vec3( coord.x, coord.y, -coord.z );

    return textureCube(osgOcean_EnvironmentMap, reflection_vector.xzy);
}

float calcFresnel( float dotEN, float mul )
{
    float fresnel = clamp( dotEN, 0.0, 1.0 ) + 1.0;
    return pow(fresnel, -8.0) * mul;
}

float alphaHeight( float min, float max, float val)
{
    if(max-min == 0.0)
        return 1.0;

    return (val - min) / (max - min);
}

float computeDepthBlur(float depth, float focus, float near, float far, float clampval )
{
   float f;

   if (depth < focus){
      // scale depth value between near blur distance and focal distance to [-1, 0] range
      f = (depth - focus)/(focus - near);
   }
   else{
      // scale depth value between focal distance and far blur
      // distance to [0, 1] range
      f = (depth - focus)/(far - focus);

      // clamp the far blur to a maximum blurriness
      f = clamp(f, 0.0, clampval);
   }

   // scale and bias into [0, 1] range
   return f * 0.5 + 0.5;
}

float luminance( vec4 color )
{
    return (0.3*color.r) + (0.59*color.g) + (0.11*color.b);
}

float computeFogFactor( float density, float fogCoord )
{
    return exp2(density * fogCoord * fogCoord );
}

// -------------------------------
//          Main Program
// -------------------------------

void main( void )
{
    vec4 final_color;

    vec3 noiseNormal = vec3( texture2D( osgOcean_NoiseMap, gl_TexCoord[0].xy ) * 2.0 - 1.0 );
    noiseNormal += vec3( texture2D( osgOcean_NoiseMap, gl_TexCoord[0].zw ) * 2.0 - 1.0 );

    worldObjectMatrix = osg_ViewMatrixInverse * gl_ModelViewMatrix;

    if(gl_FrontFacing)
    {
        vec3 N = normalize( vNormal + noiseNormal );
        vec3 L = normalize( vLightDir );
        vec3 E = normalize( vViewerDir );
        vec3 R = reflect( -L, N );

        vec4 specular_color;

        float lambertTerm = dot(N,L);

        if( lambertTerm > 0.0 )
        {
            float specCoeff = pow( max( dot(R, E), 0.0 ), shininess );
            specular_color = gl_LightSource[osgOcean_LightID].diffuse * specCoeff * 6.0;
        }

        float dotEN = dot(E, N);
        float dotLN = dot(L, N);

        // Fade out the distortion along the screen edges this reduces artifacts
        // caused by texture coordinates that are distorted out of the [0, 1] range.
        // At very close distance to the surface the distortion artifacts still appear.
        vec2 fade_xy = pow(abs(gl_FragCoord.xy / (osgOcean_ViewportDimensions.xy * 0.5) - 1.0), 10.0);
        float fade = 1.0 - max(fade_xy.x , fade_xy.y);

        vec4 distortedVertex = distortGen(vVertex, N*fade);

        // Calculate the position in world space of the pixel on the ocean floor
        vec4 refraction_ndc = vec4(gl_FragCoord.xy / osgOcean_ViewportDimensions, texture2DProj(osgOcean_RefractionDepthMap, distortGen(vVertex, 0.0 * N)).x, 1.0);
        vec4 refraction_screen = refraction_ndc * 2.0 - 1.0;
        vec4 refraction_world = osgOcean_RefractionInverseTransformation * refraction_screen;
        refraction_world = refraction_world / refraction_world.w;

        // The amount of water behind the pixel
        // (water depth as seen from the camera position)
        float waterDepth = distance(vWorldVertex, refraction_world);

#if SHORETOSINUS
        // The vertical distance between the ocean surface and ocean floor, this uses the projected heightmap
        float waterHeight = (texture2DProj(osgOcean_Heightmap, distortGen(vVertex, 0.0 * N)).x) * 500.0;
#endif

        // Determine refraction color
        vec4 refraction_color = vec4( gl_Color.rgb, 1.0 );

        if(osgOcean_EnableRefractions)
        {
            vec4 refractionmap_color = texture2DProj(osgOcean_RefractionMap, distortedVertex );
			
            vec4 waterColor = mix(refractionmap_color, refraction_color, clamp(pow(waterDepth * oneOverLightExtinction, 0.3), 0.0, 1.0));

#if SHORETOSINUS
            refraction_color = mix(waterColor, refraction_color, clamp(waterHeight * oneOverColorExtinction, 0.0, 1.0));
#else
            refraction_color = waterColor;
#endif
        }

        // To cubemap or not to cubemap that is the question
        // projected reflection looks pretty nice anyway
        // cubemap looks wrong with fixed skydome
        //vec4 env_color = computeCubeMapColor(N, vWorldVertex, osgOcean_EyePosition);

        float fresnel = calcFresnel(dotEN, osgOcean_FresnelMul );

        vec4 env_color;

        if(osgOcean_EnableReflections)
        {
            env_color = texture2DProj( osgOcean_ReflectionMap, distortedVertex );
        }
        else
        {
            env_color = gl_LightSource[osgOcean_LightID].diffuse;
        }

        final_color = mix(refraction_color, env_color, fresnel) + specular_color;

        // Store the color here to compute luminance later, we don't want
        // foam or fog to be taken into account for this calculation.
        vec4 lumColor = final_color;

        if(osgOcean_EnableCrestFoam)
        {
#if SHORETOSINUS
            if( vVertex.z > osgOcean_FoamCapBottom || waterHeight < 10.0)
            {
                vec4 foam_color = texture2D( osgOcean_FoamMap, gl_TexCoord[1].st / 10.0);

                float alpha = max(alphaHeight( osgOcean_FoamCapBottom, osgOcean_FoamCapTop, vVertex.z ) * (fresnel*2.0),
                                  0.8 - clamp(waterHeight / 10.0, 0.0, 0.8));

                final_color = final_color + (foam_color * alpha);
            }
#else
            if( vVertex.z > osgOcean_FoamCapBottom )
            {
                vec4 foam_color = texture2D( osgOcean_FoamMap, gl_TexCoord[1].st / 10.0);
                float alpha = alphaHeight( osgOcean_FoamCapBottom, osgOcean_FoamCapTop, vVertex.z ) * (fresnel*2.0);
                final_color = final_color + (foam_color * alpha);
            }
#endif
        }


        // exp2 fog
        float fogFactor = computeFogFactor( osgOcean_AboveWaterFogDensity, gl_FogFragCoord );

        final_color = mix( osgOcean_AboveWaterFogColor, final_color, fogFactor );

        if(osgOcean_EnableGlare)
        {
            float lum = luminance(lumColor);
            gl_FragData[1] = vec4(lum);
        }

        gl_FragData[0] = final_color;
    }
    else
    {
        vec3 E = normalize( vViewerDir );
        vec3 N = -normalize( (vWorldNormal + noiseNormal) );

        vec3 incident = normalize( vWorldViewDir );

        //------ Find the reflection
        // not really usable as we would need to use cubemap again..
        // the ocean is blue not much to reflect back
        //vec3 reflected = reflect( incident, -N );
        //reflected      = reorientate( reflected );
        //vec3 reflVec   = normalize( reflected );

        //------ Find the refraction from cubemap
        vec3 refracted = refract( incident, N, 1.3333333333 );   // 1.1 looks better? - messes up position of godrays though
        refracted.z = refracted.z - 0.015;                       // on the fringes push it down to show base texture color
        refracted = reorientate( refracted );

        vec4 refractColor = textureCube( osgOcean_EnvironmentMap, refracted );

        //------ Project texture where the light isn't internally reflected
        if(osgOcean_EnableRefractions)
        {
            // if alpha is 1.0 then it's a sky pixel
            if(refractColor.a == 1.0 )
            {
                vec4 env_color = texture2DProj( osgOcean_RefractionMap, distortGen(vVertex, N) );
                refractColor.rgb = mix( refractColor.rgb, env_color.rgb, env_color.a );
            }
        }

        // if it's not refracting in, add a bit of highlighting with fresnel
        if( refractColor.a == 0.0 )
        {
            float fresnel = calcFresnel( dot(E, N), 0.7 );
            refractColor.rgb = osgOcean_UnderwaterFogColor.rgb*fresnel + (1.0-fresnel)* refractColor.rgb;
        }

        float fogFactor = computeFogFactor( osgOcean_UnderwaterFogDensity, gl_FogFragCoord );
        final_color = mix( osgOcean_UnderwaterFogColor, refractColor, fogFactor );

        if(osgOcean_EnableDOF)
        {
            float depthBlur = computeDepthBlur( gl_FogFragCoord, osgOcean_DOF_Focus, osgOcean_DOF_Near, osgOcean_DOF_Far, osgOcean_DOF_Clamp );
            gl_FragData[1] = vec4(depthBlur);
        }

        gl_FragData[0] = final_color;
    }
}





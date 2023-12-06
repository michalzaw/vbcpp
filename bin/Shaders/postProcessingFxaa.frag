#version 330 core

// The implementation is based on:
// https://github.com/hjwdzh/Fluid3D/blob/master/fxaa.hlsl
// NVIDIA FXAA 3.11 by TIMOTHY LOTTES

in vec2 texCoord;

uniform sampler2D texture1;

out vec4 Color;


vec4 FxaaTexTop(sampler2D t, vec2 p)
{
	return textureLod(t, p, 0.0);
}


vec4 FxaaTexOff(sampler2D t, vec2 p, ivec2 o, vec2 r) 
{
	return textureLodOffset(t, p, 0.0, o);
}

float FxaaLuma(vec4 rgba)
{
//#if (FXAA_GREEN_AS_LUMA == 0)
//    return rgba.w;
//    return dot(rgba.rgb, vec3(0.299, 0.587, 0.114));
//#else
    return rgba.y;
//#endif
}

// FXAA QUALITY - MEDIUM DITHER PRESETS
#ifdef QUALITY__PRESET_1
const int FXAA_QUALITY_PS = 3;
const float FXAA_QUALITY[FXAA_QUALITY_PS] = float[FXAA_QUALITY_PS]( 1.5, 3.0, 12.0 );
#endif

#ifdef QUALITY__PRESET_2
const int FXAA_QUALITY_PS = 4;
const float FXAA_QUALITY[FXAA_QUALITY_PS] = float[FXAA_QUALITY_PS]( 1.0, 1.5, 3.0, 12.0 );
#endif

#ifdef QUALITY__PRESET_3
const int FXAA_QUALITY_PS = 5;
const float FXAA_QUALITY[FXAA_QUALITY_PS] = float[FXAA_QUALITY_PS]( 1.0, 1.5, 2.0, 4.0, 12.0 );
#endif

#ifdef QUALITY__PRESET_4
const int FXAA_QUALITY_PS = 6;
const float FXAA_QUALITY[FXAA_QUALITY_PS] = float[FXAA_QUALITY_PS]( 1.0, 1.5, 2.0, 2.0, 4.0, 12.0 );
#endif

#ifdef QUALITY__PRESET_5
const int FXAA_QUALITY_PS = 7;
const float FXAA_QUALITY[FXAA_QUALITY_PS] = float[FXAA_QUALITY_PS]( 1.0, 1.5, 2.0, 2.0, 2.0, 4.0, 12.0 );
#endif

#ifdef QUALITY__PRESET_6
const int FXAA_QUALITY_PS = 8;
const float FXAA_QUALITY[FXAA_QUALITY_PS] = float[FXAA_QUALITY_PS]( 1.0, 1.5, 2.0, 2.0, 2.0, 2.0, 4.0, 12.0 );
#endif

// FXAA QUALITY - LOW DITHER PRESETS
#ifdef QUALITY__PRESET_7
const int FXAA_QUALITY_PS = 3;
const float FXAA_QUALITY[FXAA_QUALITY_PS] = float[FXAA_QUALITY_PS]( 1.5, 2.0, 8.0 );
#endif

#ifdef QUALITY__PRESET_8
const int FXAA_QUALITY_PS = 4;
const float FXAA_QUALITY[FXAA_QUALITY_PS] = float[FXAA_QUALITY_PS]( 1.0, 1.5, 2.0, 8.0 );
#endif

#ifdef QUALITY__PRESET_9
const int FXAA_QUALITY_PS = 5;
const float FXAA_QUALITY[FXAA_QUALITY_PS] = float[FXAA_QUALITY_PS]( 1.0, 1.5, 2.0, 2.0, 8.0 );
#endif

#ifdef QUALITY__PRESET_10
const int FXAA_QUALITY_PS = 6;
const float FXAA_QUALITY[FXAA_QUALITY_PS] = float[FXAA_QUALITY_PS]( 1.0, 1.5, 2.0, 2.0, 2.0, 8.0 );
#endif

#ifdef QUALITY__PRESET_11
const int FXAA_QUALITY_PS = 7;
const float FXAA_QUALITY[FXAA_QUALITY_PS] = float[FXAA_QUALITY_PS]( 1.0, 1.5, 2.0, 2.0, 2.0, 3.0, 8.0 );
#endif

#ifdef QUALITY__PRESET_12
const int FXAA_QUALITY_PS = 8;
const float FXAA_QUALITY[FXAA_QUALITY_PS] = float[FXAA_QUALITY_PS]( 1.0, 1.5, 2.0, 2.0, 2.0, 2.0, 4.0, 8.0 );
#endif

#ifdef QUALITY__PRESET_13
const int FXAA_QUALITY_PS = 9;
const float FXAA_QUALITY[FXAA_QUALITY_PS] = float[FXAA_QUALITY_PS]( 1.0, 1.5, 2.0, 2.0, 2.0, 2.0, 2.0, 4.0, 8.0 );
#endif

#ifdef QUALITY__PRESET_14
const int FXAA_QUALITY_PS = 10;
const float FXAA_QUALITY[FXAA_QUALITY_PS] = float[FXAA_QUALITY_PS]( 1.0, 1.5, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 4.0, 8.0 );
#endif

#ifdef QUALITY__PRESET_15
const int FXAA_QUALITY_PS = 11;
const float FXAA_QUALITY[FXAA_QUALITY_PS] = float[FXAA_QUALITY_PS]( 1.0, 1.5, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 4.0, 8.0 );
#endif

#ifdef QUALITY__PRESET_16
const int FXAA_QUALITY_PS = 12;
const float FXAA_QUALITY[FXAA_QUALITY_PS] = float[FXAA_QUALITY_PS]( 1.0, 1.5, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 4.0, 8.0 );
#endif

// FXAA QUALITY - EXTREME QUALITY
#ifdef QUALITY__PRESET_17
const int FXAA_QUALITY_PS = 12;
const float FXAA_QUALITY[FXAA_QUALITY_PS] = float[FXAA_QUALITY_PS]( 1.0, 1.0, 1.0, 1.0, 1.0, 1.5, 2.0, 2.0, 2.0, 2.0, 4.0, 8.0 );
#endif


// FXAA3 QUALITY - PC
vec4 FxaaPixelShader(
    //
    // Use noperspective interpolation here (turn off perspective interpolation).
    // {xy} = center of pixel
    vec2 pos,
    //
    // Used only for FXAA Console, and not used on the 360 version.
    // Use noperspective interpolation here (turn off perspective interpolation).
    // {xy__} = upper left of pixel
    // {__zw} = lower right of pixel
    //vec4 fxaaConsolePosPos,
    //
    // Input color texture.
    // {rgb_} = color in linear or perceptual color space
    // if (FXAA_GREEN_AS_LUMA == 0)
    //     {___a} = luma in perceptual color space (not linear)
    sampler2D tex,
    //
    // Only used on FXAA Quality.
    // This must be from a constant/uniform.
    // {x_} = 1.0/screenWidthInPixels
    // {_y} = 1.0/screenHeightInPixels
    vec2 fxaaQualityRcpFrame,
    //
    // Only used on FXAA Quality.
    // This used to be the FXAA_QUALITY__SUBPIX define.
    // It is here now to allow easier tuning.
    // Choose the amount of sub-pixel aliasing removal.
    // This can effect sharpness.
    //   1.00 - upper limit (softer)
    //   0.75 - default amount of filtering
    //   0.50 - lower limit (sharper, less sub-pixel aliasing removal)
    //   0.25 - almost off
    //   0.00 - completely off
    float fxaaQualitySubpix,
    //
    // Only used on FXAA Quality.
    // This used to be the FXAA_QUALITY__EDGE_THRESHOLD define.
    // It is here now to allow easier tuning.
    // The minimum amount of local contrast required to apply algorithm.
    //   0.333 - too little (faster)
    //   0.250 - low quality
    //   0.166 - default
    //   0.125 - high quality 
    //   0.063 - overkill (slower)
    float fxaaQualityEdgeThreshold,
    //
    // Only used on FXAA Quality.
    // This used to be the FXAA_QUALITY__EDGE_THRESHOLD_MIN define.
    // It is here now to allow easier tuning.
    // Trims the algorithm from processing darks.
    //   0.0833 - upper limit (default, the start of visible unfiltered edges)
    //   0.0625 - high quality (faster)
    //   0.0312 - visible limit (slower)
    // Special notes when using FXAA_GREEN_AS_LUMA,
    //   Likely want to set this to zero.
    //   As colors that are mostly not-green
    //   will appear very dark in the green channel!
    //   Tune by looking at mostly non-green content,
    //   then start at zero and increase until aliasing is a problem.
    float fxaaQualityEdgeThresholdMin
)
{
	vec2 posM;
	posM.x = pos.x;
	posM.y = pos.y;

	vec4 rgbyM = FxaaTexTop(tex, posM);

	//#if (FXAA_GREEN_AS_LUMA == 0)
        //float lumaM rgbyM.w;
        //float lumaM= dot(rgbyM.rgb, vec3(0.299, 0.587, 0.114));
	//#else
		float lumaM = rgbyM.y;
	//#endif

	float lumaS = FxaaLuma(FxaaTexOff(tex, posM, ivec2(0, 1), fxaaQualityRcpFrame));
	float lumaE = FxaaLuma(FxaaTexOff(tex, posM, ivec2(1, 0), fxaaQualityRcpFrame));
	float lumaN = FxaaLuma(FxaaTexOff(tex, posM, ivec2(0, -1), fxaaQualityRcpFrame));
	float lumaW = FxaaLuma(FxaaTexOff(tex, posM, ivec2(-1, 0), fxaaQualityRcpFrame));

	float maxSM = max(lumaS, lumaM);
    float minSM = min(lumaS, lumaM);
    float maxESM = max(lumaE, maxSM);
    float minESM = min(lumaE, minSM);
    float maxWN = max(lumaN, lumaW);
    float minWN = min(lumaN, lumaW);
    float rangeMax = max(maxWN, maxESM);
    float rangeMin = min(minWN, minESM);
    float rangeMaxScaled = rangeMax * fxaaQualityEdgeThreshold;
    float range = rangeMax - rangeMin;
    float rangeMaxClamped = max(fxaaQualityEdgeThresholdMin, rangeMaxScaled);
    bool earlyExit = range < rangeMaxClamped;

	if (earlyExit)
	{
		return rgbyM;
	}

    float lumaNW = FxaaLuma(FxaaTexOff(tex, posM, ivec2(-1,-1), fxaaQualityRcpFrame));
    float lumaSE = FxaaLuma(FxaaTexOff(tex, posM, ivec2(1, 1), fxaaQualityRcpFrame));
    float lumaNE = FxaaLuma(FxaaTexOff(tex, posM, ivec2(1, -1), fxaaQualityRcpFrame));
    float lumaSW = FxaaLuma(FxaaTexOff(tex, posM, ivec2(-1, 1), fxaaQualityRcpFrame));

/*--------------------------------------------------------------------------*/
    float lumaNS = lumaN + lumaS;
    float lumaWE = lumaW + lumaE;
    float subpixRcpRange = 1.0 / range;
    float subpixNSWE = lumaNS + lumaWE;
    float edgeHorz1 = (-2.0 * lumaM) + lumaNS;
    float edgeVert1 = (-2.0 * lumaM) + lumaWE;
/*--------------------------------------------------------------------------*/
    float lumaNESE = lumaNE + lumaSE;
    float lumaNWNE = lumaNW + lumaNE;
    float edgeHorz2 = (-2.0 * lumaE) + lumaNESE;
    float edgeVert2 = (-2.0 * lumaN) + lumaNWNE;
/*--------------------------------------------------------------------------*/
    float lumaNWSW = lumaNW + lumaSW;
    float lumaSWSE = lumaSW + lumaSE;
    float edgeHorz4 = (abs(edgeHorz1) * 2.0) + abs(edgeHorz2);
    float edgeVert4 = (abs(edgeVert1) * 2.0) + abs(edgeVert2);
    float edgeHorz3 = (-2.0 * lumaW) + lumaNWSW;
    float edgeVert3 = (-2.0 * lumaS) + lumaSWSE;
    float edgeHorz = abs(edgeHorz3) + edgeHorz4;
    float edgeVert = abs(edgeVert3) + edgeVert4;
/*--------------------------------------------------------------------------*/
    float subpixNWSWNESE = lumaNWSW + lumaNESE;
    float lengthSign = fxaaQualityRcpFrame.x;
    bool horzSpan = edgeHorz >= edgeVert;
    float subpixA = subpixNSWE * 2.0 + subpixNWSWNESE;
/*--------------------------------------------------------------------------*/
    if (!horzSpan) lumaN = lumaW;
    if (!horzSpan) lumaS = lumaE;
    if (horzSpan) lengthSign = fxaaQualityRcpFrame.y;
    float subpixB = (subpixA * (1.0 / 12.0)) - lumaM;
/*--------------------------------------------------------------------------*/
    float gradientN = lumaN - lumaM;
    float gradientS = lumaS - lumaM;
    float lumaNN = lumaN + lumaM;
    float lumaSS = lumaS + lumaM;
    bool pairN = abs(gradientN) >= abs(gradientS);
    float gradient = max(abs(gradientN), abs(gradientS));
    if (pairN) lengthSign = -lengthSign;
    float subpixC = clamp(abs(subpixB) * subpixRcpRange, 0.0, 1.0);
/*--------------------------------------------------------------------------*/
    vec2 posB;
    posB.x = posM.x;
    posB.y = posM.y;
    vec2 offNP;
    offNP.x = (!horzSpan) ? 0.0 : fxaaQualityRcpFrame.x;
    offNP.y = ( horzSpan) ? 0.0 : fxaaQualityRcpFrame.y;
    if (!horzSpan) posB.x += lengthSign * 0.5;
    if (horzSpan) posB.y += lengthSign * 0.5;
/*--------------------------------------------------------------------------*/
    vec2 posN;
    posN.x = posB.x - offNP.x * FXAA_QUALITY[0];
    posN.y = posB.y - offNP.y * FXAA_QUALITY[0];
    vec2 posP;
    posP.x = posB.x + offNP.x * FXAA_QUALITY[0];
    posP.y = posB.y + offNP.y * FXAA_QUALITY[0];
    float subpixD = ((-2.0) * subpixC) + 3.0;
    float lumaEndN = FxaaLuma(FxaaTexTop(tex, posN));
    float subpixE = subpixC * subpixC;
    float lumaEndP = FxaaLuma(FxaaTexTop(tex, posP));
/*--------------------------------------------------------------------------*/
    if (!pairN) lumaNN = lumaSS;
    float gradientScaled = gradient * 1.0/4.0;
    float lumaMM = lumaM - lumaNN * 0.5;
    float subpixF = subpixD * subpixE;
    bool lumaMLTZero = lumaMM < 0.0;
/*--------------------------------------------------------------------------*/
    lumaEndN -= lumaNN * 0.5;
    lumaEndP -= lumaNN * 0.5;
    bool doneN = abs(lumaEndN) >= gradientScaled;
    bool doneP = abs(lumaEndP) >= gradientScaled;
    if (!doneN) posN.x -= offNP.x * FXAA_QUALITY[1];
    if (!doneN) posN.y -= offNP.y * FXAA_QUALITY[1];
    bool doneNP = (!doneN) || (!doneP);
    if (!doneP) posP.x += offNP.x * FXAA_QUALITY[1];
    if (!doneP) posP.y += offNP.y * FXAA_QUALITY[1];
/*--------------------------------------------------------------------------*/
    for (int i = 2; i < FXAA_QUALITY_PS; ++i)
    {
        if (doneNP)
        {
            if (!doneN) lumaEndN = FxaaLuma(FxaaTexTop(tex, posN.xy));
            if (!doneP) lumaEndP = FxaaLuma(FxaaTexTop(tex, posP.xy));
            if (!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
            if (!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
            doneN = abs(lumaEndN) >= gradientScaled;
            doneP = abs(lumaEndP) >= gradientScaled;
            if (!doneN) posN.x -= offNP.x * FXAA_QUALITY[i];
            if (!doneN) posN.y -= offNP.y * FXAA_QUALITY[i];
            doneNP = (!doneN) || (!doneP);
            if (!doneP) posP.x += offNP.x * FXAA_QUALITY[i];
            if (!doneP) posP.y += offNP.y * FXAA_QUALITY[i];
        }
    }
/*--------------------------------------------------------------------------*/
    float dstN = posM.x - posN.x;
    float dstP = posP.x - posM.x;
    if (!horzSpan) dstN = posM.y - posN.y;
    if (!horzSpan) dstP = posP.y - posM.y;
/*--------------------------------------------------------------------------*/
    bool goodSpanN = (lumaEndN < 0.0) != lumaMLTZero;
    float spanLength = (dstP + dstN);
    bool goodSpanP = (lumaEndP < 0.0) != lumaMLTZero;
    float spanLengthRcp = 1.0/spanLength;
/*--------------------------------------------------------------------------*/
    bool directionN = dstN < dstP;
    float dst = min(dstN, dstP);
    bool goodSpan = directionN ? goodSpanN : goodSpanP;
    float subpixG = subpixF * subpixF;
    float pixelOffset = (dst * (-spanLengthRcp)) + 0.5;
    float subpixH = subpixG * fxaaQualitySubpix;
/*--------------------------------------------------------------------------*/
    float pixelOffsetGood = goodSpan ? pixelOffset : 0.0;
    float pixelOffsetSubpix = max(pixelOffsetGood, subpixH);
    if (!horzSpan) posM.x += pixelOffsetSubpix * lengthSign;
    if (horzSpan) posM.y += pixelOffsetSubpix * lengthSign;

	return vec4(FxaaTexTop(tex, posM).xyz, lumaM);
}

void main()
{
	//vec3 Color1 = texture2D(texture1, texCoord).rgb;

    vec2 size = textureSize(texture1, 0);
    vec2 sizeInv = vec2(1.0 / size.x, 1.0 / size.y);

	Color.rgb = FxaaPixelShader(texCoord, texture1, sizeInv, 1.0, 0.333, 0.0833).rgb;
	Color.a = 1.0f;
}
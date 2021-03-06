#pragma once

// 型の定義 ================================================================

typedef float(*ESG)(float t, float b, float c, float d);

// 構造体の宣言 ============================================================

typedef enum
{
	ESG_LINEAR,
	ESG_INQUAD,
	ESG_OUTQUAD,
	ESG_INOUTQUAD,
	ESG_INCUBIC,
	ESG_OUTCUBIC,
	ESG_INOUTCUBIC,
	ESG_INQUART,
	ESG_OUTQUART,
	ESG_INOUTQUART,
	ESG_INQUINT,
	ESG_OUTQUINT,
	ESG_INOUTQUINT,
	ESG_INSINE,
	ESG_OUTSINE,
	ESG_INOUTSINE,
	ESG_INEXPO,
	ESG_OUTEXPO,
	ESG_INOUTEXPO,
	ESG_INCIRC,
	ESG_OUTCIRC,
	ESG_INOUTCIRC,
	ESG_INELASTIC,
	ESG_OUTELASTIC,
	ESG_INOUTELASTIC,
	ESG_INBACK,
	ESG_OUTBACK,
	ESG_INOUTBACK,
	ESG_INBOUNCE,
	ESG_OUTBOUNCE,
	ESG_INOUTBOUNCE
} Easings;

// 関数の宣言 ==============================================================

ESG GetEasing(Easings easing);
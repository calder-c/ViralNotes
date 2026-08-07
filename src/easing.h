#pragma once
static float ease(float t) {
    // exponential ease-out, normalised so ease(1) == 1 exactly
    return t >= 1.f ? 1.f : (1.f - std::pow(2.f, -10.f * t)) / (1.f - std::pow(2.f, -10.f));
}
static float expoJump(float t) {
    return t <= 0 ? 0 : std::pow(2.f, 10.f * (t - 1.f));
}
static float smoothStep(float t) {
    return t * t * (3.f - 2.f * t);
}
static float easeInOutExpo(float t){
    return t == 0 ? 0 : t == 1 ? 1 : t < 0.5 ? std::pow(2, 20 * t - 10) / 2: (2 - std::pow(2, -20 * t + 10)) / 2;
}
static float easeOutBounce(float t) {
    constexpr float n1 = 7.5625;
    constexpr float d1 = 2.75;

    if (t < 1 / d1) {
        return n1 * t * t;
    } else if (t < 2 / d1) {
        return n1 * (t -= 1.5 / d1) * t + 0.75;
    } else if (t < 2.5 / d1) {
        return n1 * (t -= 2.25 / d1) * t + 0.9375;
    } else {
        return n1 * (t -= 2.625 / d1) * t + 0.984375;
    }
}
static float easeInElastic(float t){
    constexpr float PI = 3.14159265;
    const float c4 = (2 * PI) / 3;

    return t == 0
      ? 0
      : t == 1
      ? 1
      : -std::pow(2, 10 * t - 10) * std::sin((t * 10 - 10.75) * c4);
}
static float easeOutQuad(float t){
    return 1 - (1 - t) * (1 - t);
}

static float selectEasing(const float t, const std::string &funcName) {
    if (funcName == "ease") {
        return ease(t);
    }
    if (funcName == "easeOutQuad") {
        return easeOutQuad(t);
    }
    if (funcName == "easeOutElastic") {
        return easeInElastic(t);
    }
    if (funcName == "expoJump") {
        return expoJump(t);
    }
    if (funcName == "smoothStep") {
        return smoothStep(t);
    }
    if (funcName == "easeInOutExpo") {
        return easeInOutExpo(t);
    }
    if (funcName == "easeOutBounce") {
        return easeOutBounce(t);
    }
    if (funcName == "linear") {
        return t;
    }
}

inline const char* easings[] = {
    "ease",
    "easeOutQuad",
    "easeOutElastic",
    "expoJump",
    "smoothStep",
    "easeInOutExpo",
    "easeOutBounce",
    "linear"
};
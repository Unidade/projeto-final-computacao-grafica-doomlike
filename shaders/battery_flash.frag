#version 120

// Tempo global em segundos (para animação)
uniform float uTime;
// Progresso do flash: 1.0 = recém coletado, 0.0 = sumido
uniform float uAlpha;
// Resolução da tela (para normalizar coordenadas)
uniform vec2  uResolution;

void main()
{
    // UV normalizadas [0,1] com origem no centro da tela
    vec2 uv = gl_TexCoord[0].st;          // [0,1] origem top-left
    vec2 center = uv - 0.5;               // [-0.5, 0.5] origem no centro
    float aspect = uResolution.x / uResolution.y;
    center.x *= aspect;                   // corrige proporção

    // --- Distância ao centro (para gradiente radial) ---
    float dist = length(center);

    // --- Vignette radial: forte nas bordas, ausente no centro ---
    // Curva suavizada: sem brilho até r=0.25, máximo na borda (r~0.7+)
    float vig = smoothstep(0.25, 0.80, dist);

    // --- Pulso temporal via seno ---
    // Frequência cresce com uAlpha (mais frenético quando recém coletado)
    float freq   = 4.0 + uAlpha * 6.0;
    float pulse  = 0.55 + 0.45 * sin(uTime * freq);

    // --- Shimmer cromático nas bordas ---
    // Pequeno offset radial nos canais R e B para efeito de aberração cromática
    float shimR  = 0.55 + 0.45 * sin(uTime * freq + dist * 8.0);
    float shimB  = 0.55 + 0.45 * sin(uTime * freq - dist * 8.0 + 1.57);

    // --- Cor base verde elétrico ---
    vec3 glowColor = vec3(shimR * 0.15, pulse, shimB * 0.25);

    // --- Intensidade final: vignette × alpha global ---
    float intensity = vig * uAlpha * 0.75;

    // Saída com blending aditivo (GL_ONE src no caller)
    gl_FragColor = vec4(glowColor * intensity, intensity);
}

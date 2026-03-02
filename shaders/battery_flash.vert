#version 120

void main()
{
    // Passa a posição diretamente (usaremos gl_FragCoord no frag shader)
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_TexCoord[0] = gl_MultiTexCoord0;
}

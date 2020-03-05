#version 120

uniform sampler2DRect color_texture;
uniform sampler2DRect v_texture;
uniform sampler2DRect v0_texture;
uniform sampler2DRect sd_texture;
uniform vec2 resolution;

void main() {
    vec2 st = gl_FragCoord.xy;
    st.y = st.y * -1. + resolution.y;
    vec4 c = texture2DRect(color_texture,st);
    vec4 v = texture2DRect(v_texture,st);
    vec4 v0 = texture2DRect(v0_texture,st);
    vec4 d = texture2DRect(sd_texture,st);
    c = clamp(c,0.,1.);
    gl_FragColor = vec4(c.xyz,1);
}

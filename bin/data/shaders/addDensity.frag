#version 120

uniform sampler2DRect sd_texture;
uniform vec2 mpos;
uniform float amount;
uniform vec2 resolution;
uniform float radius;

void main() {
    vec2 st = gl_FragCoord.xy;
    vec4 sd = texture2DRect(sd_texture,st);
    vec4 col = sd;
    if(length(mpos-st)<radius){
        col.y += amount;
    }

    gl_FragColor = vec4(col);
}

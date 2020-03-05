#version 120

uniform sampler2DRect sd_texture;
uniform sampler2DRect color_texture;
uniform vec2 mpos;
uniform vec2 resolution;
uniform float radius;

void main() {
    vec2 st = gl_FragCoord.xy;
    vec4 sd = texture2DRect(sd_texture,st);
    vec4 col = texture2DRect(color_texture,st);
    float dt = sd.y - sd.x;
    if(length(mpos-st)<radius){
        col.x += dt;
    }
    
    gl_FragColor = vec4(col);
}

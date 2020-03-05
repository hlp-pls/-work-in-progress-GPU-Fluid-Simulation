#version 120

uniform sampler2DRect v_texture;
uniform vec2 resolution;
uniform vec2 mpos;
uniform vec2 amt;
uniform float radius;

void main() {
    vec2 st = gl_FragCoord.xy;
    vec4 sampleCol = texture2DRect(v_texture, st);
    vec4 col = sampleCol;
    if(length(mpos-st)<radius){
        col.x += amt.x;
        col.y += amt.y;
    }
    
    gl_FragColor = vec4(col);
}


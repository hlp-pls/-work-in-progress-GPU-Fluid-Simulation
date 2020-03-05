#version 120

uniform sampler2DRect color_texture;
uniform sampler2DRect sd_texture;
uniform sampler2DRect v_texture;
uniform vec2 resolution;
uniform float time_step;

void main() {
    vec2 st = gl_FragCoord.xy;

    vec4 smp_v = texture2DRect(v_texture,st);
    vec4 out_col = texture2DRect(color_texture,st);
    
    /* order
        2 --> advect color with s,vx,vy,time_step
    */
    
    float dtx = time_step * (resolution.x-2.);
    float dty = time_step * (resolution.y-2.);
    float tmp1 = smp_v.x * dtx;
    float tmp2 = smp_v.y * dty;
    float x = st.x - tmp1;
    float y = st.y - tmp2;
    
    if(x < .5) x = .5;
    if(x > resolution.x + .5) x = resolution.x + .5;
    float i0 = floor(x);
    float i1 = i0 + 1.;
    
    if(y < .5) y = .5;
    if(y > resolution.y + .5) y = resolution.y + .5;
    float j0 = floor(y);
    float j1 = j0 + 1.;
    
    float s1 = x - i0;
    float s0 = 1. - s1;
    float t1 = y - j0;
    float t0 = 1. - t1;
    
    i0 = floor(i0);
    j0 = floor(j0);
    i1 = floor(i1);
    j1 = floor(j1);
    
    if(st.x<1.||st.x>resolution.x-1.||st.y<1.||st.y>resolution.y-1.){}else{
        float i0j0 = texture2DRect(sd_texture,vec2(i0,j0)).x;
        float i0j1 = texture2DRect(sd_texture,vec2(i0,j1)).x;
        float i1j0 = texture2DRect(sd_texture,vec2(i1,j0)).x;
        float i1j1 = texture2DRect(sd_texture,vec2(i1,j1)).x;
        //2
        out_col.x = s0 * ( t0 * i0j0 + t1 * i0j1 ) + s1 * ( t0 * i1j0 + t1 * i1j1 );
        out_col.y = s0 * ( t0 * i0j0 + t1 * i0j1 ) + s1 * ( t0 * i1j0 + t1 * i1j1 );
        out_col.z = s0 * ( t0 * i0j0 + t1 * i0j1 ) + s1 * ( t0 * i1j0 + t1 * i1j1 );
    }
    
    gl_FragColor = vec4(out_col);
}

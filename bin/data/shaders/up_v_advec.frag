#version 120

uniform sampler2DRect v0_texture;
uniform sampler2DRect v_texture;
uniform vec2 resolution;
uniform float time_step;

void main() {
    vec2 st = gl_FragCoord.xy;
    
    vec4 smp_v0 = texture2DRect(v0_texture,st);
    vec4 out_v = texture2DRect(v_texture,st);
    
    /* order
         1 --> advect vx with vx0,vx0,vy0,time_step
         2 --> advect vy with vy0,vx0,vy0,time_step
    */

    //1~2 advect vx,vy
    float dtx = time_step * (resolution.x-2.);
    float dty = time_step * (resolution.y-2.);
    float tmp1 = smp_v0.x * dtx;
    float tmp2 = smp_v0.y * dty;
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
        vec2 i0j0 = texture2DRect(v0_texture,vec2(i0,j0)).xy;
        vec2 i0j1 = texture2DRect(v0_texture,vec2(i0,j1)).xy;
        vec2 i1j0 = texture2DRect(v0_texture,vec2(i1,j0)).xy;
        vec2 i1j1 = texture2DRect(v0_texture,vec2(i1,j1)).xy;
        //1
        out_v.x = s0 * ( t0 * i0j0.x + t1 * i0j1.x ) + s1 * ( t0 * i1j0.x + t1 * i1j1.x );
        //2
        out_v.y = s0 * ( t0 * i0j0.y + t1 * i0j1.y ) + s1 * ( t0 * i1j0.y + t1 * i1j1.y );
    }
    
    gl_FragColor = vec4(out_v);
}

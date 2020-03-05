#version 120

uniform sampler2DRect v_texture;
uniform sampler2DRect v0_texture;
uniform vec2 resolution;
uniform float time_step;
uniform float viscosity;

void main() {
    vec2 st = gl_FragCoord.xy;
    
    vec4 smp_v = texture2DRect(v_texture,st);
    
    vec4 out_v0 = texture2DRect(v0_texture,st);
    
    /* order
        1 --> diffuse vx0 with vx,viscosity,time_step
        2 --> diffuse vy0 with vy,viscosity,time_step
    */
    
    //1~2 diffuse vx0,vy0
    float a = time_step * viscosity * (resolution.x-2.) * (resolution.y-2.);
    float c = 1. + 6. * a;
    float cRecip = 1. / c;

    if(st.x<1.||st.x>resolution.x-1.||st.y<1.||st.y>resolution.y-1.){}else{
        vec2 xy1 = texture2DRect(v0_texture,st+vec2(1.,0.)).xy;
        vec2 xy2 = texture2DRect(v0_texture,st+vec2(-1.,0.)).xy;
        vec2 xy3 = texture2DRect(v0_texture,st+vec2(0.,1.)).xy;
        vec2 xy4 = texture2DRect(v0_texture,st+vec2(0.,-1.)).xy;
        //1
        out_v0.x = ( smp_v.x + a * ( xy1.x + xy2.x + xy3.x + xy4.x ) ) * cRecip;
        //2
        out_v0.y = ( smp_v.y + a * ( xy1.y + xy2.y + xy3.y + xy4.y ) ) * cRecip;
        
    }
    
    gl_FragColor = vec4(out_v0);
}


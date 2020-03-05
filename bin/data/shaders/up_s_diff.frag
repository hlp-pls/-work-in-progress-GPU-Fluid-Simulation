#version 120

uniform sampler2DRect color_texture;
uniform sampler2DRect sd_texture;
uniform vec2 resolution;
uniform float time_step;
uniform float diffusion;

void main() {
    vec2 st = gl_FragCoord.xy;
    
    vec4 smp_col = texture2DRect(color_texture,st);
    vec4 out_sd = texture2DRect(sd_texture,st);
    
    /* order
        1 --> diffuse s with color,diffusion,time_step
    */
    
    float a = time_step * diffusion * (resolution.x-2.) * (resolution.y-2.);
    float c = 1. + 6. * a;
    float cRecip = 1. / c;
    
    if(st.x<1.||st.x>resolution.x-1.||st.y<1.||st.y>resolution.y-1.){}else{
        float s1 = texture2DRect(sd_texture,st+vec2(1.,0.)).x;
        float s2 = texture2DRect(sd_texture,st+vec2(-1.,0.)).x;
        float s3 = texture2DRect(sd_texture,st+vec2(0.,1.)).x;
        float s4 = texture2DRect(sd_texture,st+vec2(0.,-1.)).x;
        //1
        out_sd.x = ( smp_col.x + a * ( s1 + s2 + s3 + s4 ) ) * cRecip;
    }
    
    gl_FragColor = vec4(out_sd);
}

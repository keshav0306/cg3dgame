#version 330 core
in vec3 position;
out vec4 FragColor;
uniform sampler2D tex;
uniform int comp;
uniform float val;
in vec3 norm;
in vec2 texxy;
void main(){
    if(comp == 0){
        vec3 an = normalize(norm);
        vec3 al = normalize(-position + vec3(0.0f, 200.0 * val, 0.0));
        vec3 ref = reflect(-al, an);
        normalize(ref);
        float spec = pow(max(dot(ref, an), 0.0f), 32);

        float diff = dot(an, al);
        if(diff > 0){
        FragColor = vec4(vec3(1.0, 0.5, 0.0) * (diff + spec), 1.0) + vec4(1.0, 1.0, 1.0, 1.0) * 0.20;
        }
        else{
        FragColor = vec4(1.0, 1.0, 1.0, 1.0) * 0.20;
        }
        // FragColor = vec4(1.0, 0.5, sin(val * 10.0), 1.0);
    }
    else{
        FragColor = vec4(1.0) * texture(tex, texxy);
    }
    // FragColor = vec4(1.0f);
}
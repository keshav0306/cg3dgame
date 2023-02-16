#version 330 core
in vec3 position;
out vec4 FragColor;
uniform sampler2D tex;
uniform float coeff[3];
uniform int comp;
uniform float val;
in vec3 norm;
in vec2 texxy;
void main(){
    if(comp == 0){
        vec3 an = normalize(norm);
        vec3 al = normalize(position - vec3(0.0f, sin(val), 0.0f));
        float diff = dot(an, al);
        if(diff > 0){
        FragColor = vec4(1.0) * diff;
        }
        else{
        FragColor = vec4(1.0) * diff;

        }
    }
    else{
        FragColor = vec4(1.0) * texture(tex, texxy);
    }
    // FragColor = vec4(1.0f);
}
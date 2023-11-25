#version 450

in vec4 Position;
in vec3 Normal;
uniform vec4 LightPosition;
uniform vec3 LightIntensity; //Ia=Id=Is
uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float Shininess;
out vec4 FragColor;

// Phong Illumination Model
void main()
{
    vec3 N = normalize(Normal);
    vec3 L = normalize(LightPosition.xyz - Position.xyz);
    vec3 V = normalize(-Position.xyz);
    vec3 R = reflect(-L, N);
    vec3 ambient = Ka * LightIntensity;
    vec3 diffuse = Kd * LightIntensity * max(dot(N, L), 0.0);
    vec3 specular = Ks * LightIntensity * pow(max(dot(R, V), 0.0), Shininess);
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}

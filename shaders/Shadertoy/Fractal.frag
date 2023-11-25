float sdPentagon(in vec2 p, in float r, in float speed, in float zoom)
{
    const vec3 k = vec3(0.809016994,0.587785252,0.726542528);
    p.x = abs(p.x);
    p -= 2.0*min(dot(vec2(-k.x,k.y),p),0.)*vec2(-k.x,k.y);
    p -= 2.0*min(dot(vec2( k.x,k.y),p),0.)*vec2( k.x,k.y);
    p -= vec2(clamp(p.x,-r*k.z,r*k.z),r) * speed;
    float rs = (length(p)*sign(p.y)) * zoom;
    if (zoom > 0.)
        return abs(rs);
    return rs;
}

vec3 palette(in float t)
{
    const vec3 a = vec3(0.5, 0.5, 0.5);
    const vec3 b = vec3(0.5, 0.5, 0.5);
    const vec3 c = vec3(1.0, 1.0, 1.0);
    const vec3 d = vec3(0.263,0.416,0.557);

    return a + b*cos(6.28318*(c*t+d));
}

float neon(in float d)
{
    d = sin(d*8. + iTime)/25.;
    d = abs(d);
    d = pow(0.01 / d * 1.5, 1.2);
    return d;
}

vec3 dark_magic(in vec2 unscaled_uv, in vec2 scaled_uv, in vec3 base_color, in float d, float pstep)
{
    float ad = abs(d);

    vec3 c = base_color * 1.0 - exp(-6.0*ad);

    // wtf ?? pentagon
    c *= 0.8 + 0.2*-cos(110.0*ad);
    c *= palette(length(unscaled_uv) + pstep) * length(scaled_uv);
    return c;
}

vec2 render_scale(vec2 fragCoord)
{
    return (fragCoord * 2.0 - iResolution.xy) / iResolution.y;
}

float penta_step(float i)
{
    return 0.1 * i + iTime * 0.1;
}

float dark_magic_step(float i)
{
    return i * 10. + iTime * i / 100.;
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 uv0 = render_scale(fragCoord);
    vec3 full_color = vec3(0.0);

    float step_count = 1.;

    vec3 base_color = vec3(1.0, 0.3, 1.0);
    float distance_ = 0.5; // inverted smaller means bigger
    float speed = 1.5; // inner to out animation - or +


    for (float i = 0.0; i < step_count; ++i) {
        vec2 uv = fract(uv0 * 0.01) - 0.5;
        float d = sdPentagon(uv0, penta_step(i), speed, distance_) * 0.2;
        full_color += dark_magic(uv0, uv, base_color, d, dark_magic_step(i)) * neon(d) / 1.25;
    }

    fragColor = vec4(full_color, 1.0);
    //fragColor = vec4(iTime * 0.1, iTime * 0.3, iTime * 0.5, 1.0);
}
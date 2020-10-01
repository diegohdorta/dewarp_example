precision mediump float;
varying  vec2 vTexcoor;
uniform sampler2D tex;
uniform vec2 iMouse;
uniform vec2 iResolution;

#define EPSILON 0.000011

void main()
{
vec2 p = vTexcoor.xy;
float prop = iResolution.x / iResolution.y;
vec2 m = vec2(0.5, 0.5 / prop);

vec2 d = p - m;
float r = sqrt(dot(d, d));
float power = ( 2.0 * 3.141592 / (2.0 * sqrt(dot(m, m))) ) *(iMouse.x / iResolution.x - 0.5);

float bind;

if (power > 0.0) bind = sqrt(dot(m, m));
else {
 if (prop < 1.0)
  bind = m.x;
 else
  bind = m.y;}

vec2 uv;
if (power > 0.0)
 uv = m + normalize(d) * tan(r * power) * bind / tan( bind * power);
else if (power < 0.0)
 uv = m + normalize(d) * atan(r * (-power) * 10.0) * bind / atan((-power) * bind * 10.0);
else 
 uv = p;

vec3 col = texture2D(tex, vec2(uv.x, uv.y * prop)).xyz;
gl_FragColor = vec4(col, 1.0);   
}

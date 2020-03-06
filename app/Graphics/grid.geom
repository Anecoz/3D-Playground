#version 430
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 geomPosition[];
in vec4 geomShadowSpacePosition[];

flat out vec3 fragNormal;
out vec3 fragPosition;
out vec4 fragShadowSpacePosition;

vec3 getNormal()
{
  vec3 a = vec3(geomPosition[0]) - vec3(geomPosition[1]);
  vec3 b = vec3(geomPosition[2]) - vec3(geomPosition[1]);
  return normalize(cross(b, a));
} 

void main() {
  vec3 normal = getNormal();

  gl_Position = gl_in[0].gl_Position;
  fragNormal = normal;
  fragPosition = geomPosition[0];
  fragShadowSpacePosition = geomShadowSpacePosition[0];
  EmitVertex();

  gl_Position = gl_in[1].gl_Position;
  fragNormal = normal;
  fragPosition = geomPosition[1];
  fragShadowSpacePosition = geomShadowSpacePosition[1];
  EmitVertex();

  gl_Position = gl_in[2].gl_Position;
  fragNormal = normal;
  fragPosition = geomPosition[2];
  fragShadowSpacePosition = geomShadowSpacePosition[2];
  EmitVertex();
  
  EndPrimitive();
}
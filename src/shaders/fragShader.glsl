#version 330 core
in vec2 pos;
out vec4 fragColor;

uniform int maxIterations;
void main()
{
  float x,x0 = pos.x;
  float y,y0 = pos.y;
  int iteration = 0;
  while (x*x + y*y <= 2*2 && iteration < maxIterations)
  {
        float xtemp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtemp;
        ++iteration;
  }

  if(iteration != maxIterations)
  {
    fragColor = vec4(float(iteration)/float(100),0.0,0.0,1.0);
  }
  else
  {
    fragColor = vec4(0.0,0.0,0.0,1.0);
  }

}
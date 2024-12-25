#version 330 core
in vec2 pos;
out vec4 fragColor;

const int maxIterations = 1000;
float xs[maxIterations];
float ys[maxIterations];

uniform vec2 julia;

int getDivergenceIteration(float x0, float y0)
{
  float x = x0;
  float y = y0;
  int iteration = 0;
  while (x*x + y*y <= 2*2 && iteration < maxIterations)
  {
        xs[iteration] = x;
        ys[iteration] = y;
        float xtemp = x*x - y*y + julia.x;//+ x0;
        y = 2*x*y + julia.y;//y0;
        x = xtemp;
        ++iteration;
  }
  return iteration;
}

bool convergesToCycleWithPeriod(int period)
{
  int convergenceIteration = 0;
  while(convergenceIteration + period < maxIterations)
    {
        float prevx = xs[convergenceIteration];
        float prevy = ys[convergenceIteration];
        float x = xs[convergenceIteration + period];
        float y = ys[convergenceIteration + period];
        float xDiff = x-prevx;
        float yDiff = y-prevy;
        if(xDiff*xDiff + yDiff*yDiff < 10e-6)
        {
            return true;
        }
        convergenceIteration++;
    }
    return false;
}
void main()
{
  int divergenceIteration = getDivergenceIteration(pos.x, pos.y);

  // if(divergenceIteration == maxIterations)
  // {
  //   if(convergesToCycleWithPeriod(1))
  //   {
  //     fragColor = vec4(1.0,0.0,0.0,1.0);
  //   }
  //   else if(convergesToCycleWithPeriod(2))
  //   {
  //     fragColor = vec4(0.0,1.0,0.0,1.0);
  //   }
  // }
  // else
  // {
    fragColor = vec4(mod(divergenceIteration,2));
  //}

  // if(iteration != maxIterations)
  // {
  //   float intensity = mod(float(iteration),10)/10;
  //   fragColor = vec4(intensity,intensity,intensity,1.0);
  // }
  // else
  // {
  //   fragColor = vec4(0.0,0.0,0.0,1.0);
  // }
}
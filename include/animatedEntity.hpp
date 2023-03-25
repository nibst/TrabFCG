//curva de bezier sob uma entidade
#include "entity.hpp"
#include <vector>

#define DEFAULT_P1 glm::vec4(0.0,0.0,0.0,1.0)
#define DEFAULT_P2 glm::vec4(50.0,0.0,20.0,1.0)
#define DEFAULT_P3 glm::vec4(10.0,0.0,30.0,1.0)
#define DEFAULT_P4 glm::vec4(60.0,0.0,40.0,1.0)
#define DEFAULT_SPEED 0.2f
#define INITIAL_BEZIER_T 0.0f

class AnimatedEntity:public Entity{
    public:
        AnimatedEntity(Model object);
        void move(const float delta_time);
        void setBezierPoints(const std::vector<glm::vec4> points);
    private:
        long factorial(const int n);

        float bernsteinPolynomial(const int k,const int n); 
        float bezier_t; // [0,1]
        std::vector<glm::vec4> bezierPoints;
        float speed;
};
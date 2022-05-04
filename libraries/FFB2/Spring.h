#pragma once 

#include "Effect.h"
// =================================================== SPRING EFFECT 
class SpringEffect : public Effect {
  public: 

    int center = -1;
    int null_zone = 0;
    float k = 0;
    
    float i_gain = 0.0000;
    int32_t i_anti_wind_up = 0000;
    
		
		
    SpringEffect (float & pos, float coeff ): position(pos) {
      k = coeff;
    }  
    SpringEffect () : position(dummy){
      k = 0;
    }
    SpringEffect (float k_) : position(dummy){
      k = k_;
    }
    virtual ~SpringEffect() {}

    void update() { 
      //Printl("--Updating Spring");  // TODO remove 
      int delta = position-center;
      if (abs(delta) < null_zone) force = 0;
      else if (position > center) force = (delta - null_zone ) * k;
      else                   force = (delta + null_zone ) * k;
      
      // Integral Controls 
      i_sum += delta;
      i_sum  = constrain(i_sum , -i_anti_wind_up, i_anti_wind_up); 
      force += i_sum * i_gain;
       
    }
		
    void update( float v) { 
      //Printl("--Updating Spring");  // TODO remove 
      int delta = v-center;
      if (abs(delta) < null_zone) force = 0;
      else if (v > center) force = (delta - null_zone ) * k;
      else                   force = (delta + null_zone ) * k;
      
      // Integral Controls 
      i_sum += delta;
      i_sum  = constrain(i_sum , -i_anti_wind_up, i_anti_wind_up); 
      force += i_sum * i_gain;
       
    }		
    void moveCenter (int new_center) {
      center = new_center;
    }
    void adjustK (float new_k) {
      k = new_k;
    }
  protected: 
    int32_t i_sum = 0;
    float & position;
		float dummy; 
      
};



class SpringEffect_2D : public Effect_2D {
  public: 

    SpringEffect_2D (float & xpos, float & ypos, float x_coeff, float y_coeff) {
      X = new SpringEffect(xpos, x_coeff);
      Y = new SpringEffect(ypos, y_coeff);      
      enabled = 1;
    }
    SpringEffect_2D (float x_coeff, float y_coeff) {
      X = new SpringEffect(x_coeff);
      Y = new SpringEffect(y_coeff);      
      enabled = 1;
    }
    virtual ~SpringEffect_2D () {
      delete X;
      delete Y;
    }
    void update() { 
      if (!enabled) {
        x_force = 0; 
        y_force = 0;
        return;
      }
      X->update();
      Y->update();    
      x_force = X->getForce();  
      y_force = Y->getForce();  
    }
		void update( float v1, float v2) { 
      if (!enabled) {
        x_force = 0; 
        y_force = 0;
        return;
      }
      X->update(v1);
      Y->update(v2);    
      x_force = X->getForce();  
      y_force = Y->getForce();  
    }		

    void moveCenter (int new_x_center, int new_y_center) {
      X->moveCenter(new_x_center);
      Y->moveCenter(new_y_center);
    }

    void adjustK (float new_x_k, float new_y_k) {
      X->adjustK(new_x_k);
      Y->adjustK(new_y_k);
    }
		
		void adjustKi (float new_ki) {
      X->i_gain = new_ki;
      Y->i_gain = new_ki;
		}
		
		void adjustNullZone(uint16_t nz) {
			X->null_zone = nz;
			Y->null_zone = nz;
		}
		
  //protected: 
    SpringEffect * X; 
    SpringEffect * Y;
      
};


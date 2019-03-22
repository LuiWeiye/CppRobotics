/*************************************************************************
	> File Name: motion_model.h
	> Author: TAI Lei
	> Mail: ltai@ust.hk
	> Created Time: Thu Mar 21 16:06:07 2019
 ************************************************************************/

#ifndef _MOTION_MODEL_H
#define _MOTION_MODEL_H

#include <iostream>
#include <cmath>
#include <boost/math/interpolators/cubic_b_spline.hpp>

#define PI 3.141592653

class State{
  public:
    float x;
    float y;
    float yaw;
    float v;

    State();
    State(float x_, float y_, float yaw_, float v_);
};

State::State(float x_, float y_, float yaw_, float v_){
  x = x_;
  y = y_;
  yaw = yaw_;
  v = v_;
};

class MotionModel{
  public:
    const float base_l;
    const float ds;
    State state;

    MotionModel(float base_l_, float ds_, State state_):
      base_l(base_l_), ds(ds_), state(state_){};
    void update(float v_, float delta, float dt);
    void generate_trajectory(float s, float k0, float k1,
                             float k2, float k3, float k4);
  // private:

};

void MotionModel::update(float v_, float delta, float dt){
    state.v = v_;
    state.x = state.x + state.v * std::cos(state.yaw) * dt;
    state.y = state.y + state.v * std::sin(state.yaw) * dt;
    state.yaw = state.yaw + state.v / base_l * std::tan(delta) * dt;
    if (state.yaw > PI){
      state.yaw = state.yaw - 2 * PI;
    }
};

void MotionModel::generate_trajectory(float s, std::vector<float> kk){
  // NOTE boost cubic b ask for at least 5 points
  // s: distance
  // kk: yaw velocity as specific position
  float n = s / ds;
  float horizon = s / state.v;
  // std::vector<float> tk{0, horizon/4, horizon/2, horizon*3/4, horizon};
  // std::vector<float> kk{k0, k1, k2, k3, kf};

  boost::math::cubic_b_spline<float> spline(
    kk.data(), kk.size(), 0, horizon/kk.size());

  for(float i=0.0; i<horizon; i+=horizon/n){
      float kp = spline(i);
      // std::cout << i << " " <<horizon << std::endl;
      std::cout << kp << std::endl;
  }

}

#endif

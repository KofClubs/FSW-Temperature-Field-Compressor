#include "temperature.h"

#include <cmath>
#include <cstdlib>

#include "exit_status.h"

// 建议比温度精度高至少1个数量级
#define EPSILON 1.0e-7

Temp_t::Temp_t() {}

Temp_t::Temp_t(double t, double p) : epsilon(EPSILON) {
  if (t < 0) /* 开氏温标不能小于零 */ {
    exit(TEMPERATURE_MINUS);
  }
  if (p <= 0) /* 精度不能是非正数 */ {
    exit(PRECISION_NON_POSITIVE);
  }
  intTemp = std::llround(t / p);
  prec = p;
}

long long Temp_t::getIntTemp() { return intTemp; }

double Temp_t::getPrec() { return prec; }

double Temp_t::getTemp() { return intTemp * prec; }

double Temp_t::updateIntTemp(long long it) {
  intTemp += it;
  return getTemp();
}

long long Temp_t::updateTemp(double t) {
  long long predIntTemp = intTemp;
  return (intTemp = std::llround(t / prec)) - predIntTemp;
}

bool Temp_t::operator==(const Temp_t &t) const {
  if (std::abs(prec - t.prec) < epsilon && intTemp == t.intTemp) {
    return true;
  }
  return false;
}

bool Temp_t::operator!=(const Temp_t &t) const { return !(*this == t); }

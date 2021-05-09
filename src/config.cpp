#include "config.h"

RPlayerConfig* RPlayerConfig::createInstance() {
  RPlayerConfig* pConfig = new RPlayerConfig();
  pConfig->_consumer = new RPlayerConfig();
  return pConfig;
}

void RPlayerConfig::resetConsumer() {
  if (_consumer == nullptr) {
    _consumer = new RPlayerConfig();
  }
  memmove(_consumer, this, sizeof(RPlayerConfig));
}

void RPlayerConfig::release() {
  delete _consumer;
  _consumer = nullptr;
}

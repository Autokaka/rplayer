/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/01.
 */

template <typename PointerType>
bool isNull(const PointerType& p) {
  return p == nullptr;
}

template <typename PointerType>
bool isNull(const PointerType*& p) {
  return p == nullptr;
}

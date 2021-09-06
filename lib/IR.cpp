#include <IR.hpp>
auto mkIR(OP op, Addr addr0, Addr addr1, Addr addr2) -> shared_ptr<IR> {
  return make_shared<IR>(op, addr0, addr1, addr2);
}

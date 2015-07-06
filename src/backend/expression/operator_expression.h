#pragma once

#include "backend/common/serializer.h"
#include "backend/common/value_vector.h"

#include "backend/expression/abstract_expression.h"

#include <string>

namespace peloton {
namespace expression {

//===--------------------------------------------------------------------===//
// Operator Expression
//===--------------------------------------------------------------------===//

// Unary operators.

// NOT

class OperatorUnaryNotExpression : public AbstractExpression {
 public:
  OperatorUnaryNotExpression(AbstractExpression *left)
 : AbstractExpression(EXPRESSION_TYPE_OPERATOR_NOT) {
    m_left = left;
    left_expr = left;
  };

  Value Evaluate(const AbstractTuple *tuple1, const AbstractTuple *tuple2) const {
    assert (m_left);
    return m_left->Evaluate(tuple1, tuple2).OpNegate();
  }

  std::string DebugInfo(const std::string &spacer) const {
    return (spacer + "OperatorNotExpression");
  }

 private:
  AbstractExpression *m_left;
};

// MINUS

class OperatorUnaryMinusExpression : public AbstractExpression {
 public:
  OperatorUnaryMinusExpression(AbstractExpression *left)
 : AbstractExpression(EXPRESSION_TYPE_OPERATOR_UNARY_MINUS) {
    m_left = left;
    left_expr = left;
  };

  Value Evaluate(const AbstractTuple *tuple1, const AbstractTuple *tuple2) const {
    assert (m_left);
    return m_left->Evaluate(tuple1, tuple2).OpMultiply(ValueFactory::GetTinyIntValue(-1));
  }

  std::string DebugInfo(const std::string &spacer) const {
    return (spacer + "OperatorMinusExpression");
  }

 private:
  AbstractExpression *m_left;
};
// Binary operators.

class OpPlus {
 public:
  inline Value op(Value left, Value right) const { return left.OpAdd(right); }
};

class OpMinus {
 public:
  inline Value op(Value left, Value right) const { return left.OpSubtract(right); }
};

class OpMultiply {
 public:
  inline Value op(Value left, Value right) const { return left.OpMultiply(right); }
};

class OpDivide {
 public:
  inline Value op(Value left, Value right) const { return left.OpDivide(right); }
};

// Expressions templated on binary operator types

template <typename OPER>
class OperatorExpression : public AbstractExpression {
 public:
  OperatorExpression(ExpressionType type,
                     AbstractExpression *left,
                     AbstractExpression *right)
 : AbstractExpression(type, left, right) {
  }

  Value Evaluate(const AbstractTuple *tuple1, const AbstractTuple *tuple2) const {
    assert(left_expr);
    assert(right_expr);
    return oper.op(left_expr->Evaluate(tuple1, tuple2), right_expr->Evaluate(tuple1, tuple2));
  }

  std::string DebugInfo(const std::string &spacer) const {
    return (spacer + "OptimizedOperatorExpression");
  }

 private:
  OPER oper;
};

} // End expression namespace
} // End peloton namespace



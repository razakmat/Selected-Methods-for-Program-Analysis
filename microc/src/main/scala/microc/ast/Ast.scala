package microc.ast

/**
  * A source code location of an AST node.
  *
  * @param line The line number.
  * @param col  The column number.
  */
case class Loc(line: Int, col: Int) extends Ordered[Loc] {
  override def toString: String = s"$line:$col"

  override def compare(that: Loc): Int = {
    val d = this.line - that.line
    if (d == 0) this.col - that.col else d
  }
}

/** A binary operator */
sealed trait BinaryOperator

object BinaryOperator {
  def apply(s: String): BinaryOperator = s match {
    case "+"  => Plus
    case "-"  => Minus
    case "*"  => Times
    case "/"  => Divide
    case "==" => Equal
    case ">"  => GreatThan
  }
}

case object Plus extends BinaryOperator {
  override def toString: String = "+"
}

case object Minus extends BinaryOperator {
  override def toString: String = "-"
}

case object Times extends BinaryOperator {
  override def toString: String = "*"
}

case object Divide extends BinaryOperator {
  override def toString: String = "/"
}

case object Equal extends BinaryOperator {
  override def toString: String = "=="
}

case object GreatThan extends BinaryOperator {
  override def toString: String = ">"
}

// ----------------------------------------------------------------------------
// BASE NODES
// ----------------------------------------------------------------------------

object AstNode {
  private val Printer = new AstPrinter()
}

/**
  * An AST node
  */
sealed abstract class AstNode {

  /**
    * The source code location of the AST node.
    *
    * @return a source code location.
    */
  def loc: Loc

  def children: Iterable[AstNode] = Iterable.empty

  def tree: Iterable[AstNode] = Iterable(this) ++ children.flatMap(_.tree)

  override def toString: String = {
    AstNode.Printer.print(this) + s"[$loc]"
  }
}

sealed trait Expr extends AstNode

sealed trait Stmt extends AstNode

sealed trait Decl extends AstNode {
  def name: String
}

/**
  * A statement in a nested block.
  *
  * It cannot be a declaration or a return.
  */
sealed trait StmtInNestedBlock extends Stmt

// ----------------------------------------------------------------------------
// EXPRESSIONS
// ----------------------------------------------------------------------------

case class Null(loc: Loc) extends Expr

case class Number(value: Int, loc: Loc) extends Expr

case class Identifier(name: String, loc: Loc) extends Expr

case class BinaryOp(operator: BinaryOperator, left: Expr, right: Expr, loc: Loc) extends Expr {
  override def children: Iterable[AstNode] = List(left, right)
}

case class CallFuncExpr(targetFun: Expr, args: List[Expr], loc: Loc) extends Expr {
  override def children: Iterable[AstNode] = targetFun :: args
}

/**
  * Read of one integer from standard input.
  *
  * @param loc The source code location.
  */
case class Input(loc: Loc) extends Expr

case class Alloc(expr: Expr, loc: Loc) extends Expr {
  override def children: Iterable[AstNode] = List(expr)
}

/**
  * Variable reference.
  *
  * It is used to create a pointer to a variable `id` (&id).
  *
  * @param id  The target variable.
  * @param loc The source code location.
  */
case class VarRef(id: Identifier, loc: Loc) extends Expr {
  override def children: Iterable[AstNode] = List(id)
}

/**
  * Pointer dereference
  *
  * It is used to dereference a pointer `p` (`*p`).
  *
  * @param pointer The pointer to dereference
  * @param loc     The source code location
  */
case class Deref(pointer: Expr, loc: Loc) extends Expr {
  override def children: Iterable[AstNode] = List(pointer)
}

case class Record(fields: List[RecordField], loc: Loc) extends Expr {
  override def children: Iterable[AstNode] = fields
}

case class RecordField(name: String, expr: Expr, loc: Loc) extends AstNode {
  override def children: Iterable[AstNode] = List(expr)
}

/**
  * Accessing a field of a record (x.y).
  *
  * @param record The expression that evaluates to a record value.
  * @param field  The name of the field of the record.
  * @param loc    The source code location.
  */
case class FieldAccess(record: Expr, field: String, loc: Loc) extends Expr {
  override def children: Iterable[AstNode] = List(record)
}

// ----------------------------------------------------------------------------
// STATEMENTS
// ----------------------------------------------------------------------------

/**
  * An Assignment.
  *
  * @param left  The LValue (where to assign)
  * @param right The RValue (what to assign)
  * @param loc   The source code location
  */
case class AssignStmt(left: Expr, right: Expr, loc: Loc) extends StmtInNestedBlock {
  override def children: Iterable[AstNode] = List(left, right)
}

case object DirectWrite {
  def unapply(expr: Expr): Option[Identifier] = expr match {
    case x: Identifier => Some(x)
    case _ => None
  }
}

case object IndirectWrite {
  def unapply(expr: Expr): Option[Deref] = expr match {
    case x: Deref => Some(x)
    case _ => None
  }
}

case object DirectFieldWrite {
  def unapply(expr: Expr): Option[(Identifier, String, Loc)] = expr match {
    case FieldAccess(record: Identifier, field, loc) => Some((record, field, loc))
    case _                                           => None
  }
}

case object IndirectFieldWrite {
  def unapply(expr: Expr): Option[(Deref, String, Loc)] = expr match {
    case FieldAccess(record: Deref, field, loc) => Some((record, field, loc))
    case _ => None
  }
}

sealed trait Block extends Stmt {
  def body: List[Stmt]

  override def children: Iterable[AstNode] = body
}

object Block {
  def unapply(that: Block): Option[List[Stmt]] = Some(that.body)
}

/**
  * Represents a block of statements surrounded by curly braces.
  *
  * {{{
  * {
  *   stmt_1
  *   stmt_2
  *   ...
  *   stmt_n
  * }
  * }}}
  *
  * @param body The list of statements in this block
  * @param loc The location
  */
case class NestedBlockStmt(body: List[StmtInNestedBlock], loc: Loc) extends Block with StmtInNestedBlock

case class FunBlockStmt(vars: List[VarStmt], stmts: List[StmtInNestedBlock], ret: ReturnStmt, loc: Loc) extends Block {
  val body: List[Stmt] = vars ++ (stmts :+ ret)
}

case class ReturnStmt(expr: Expr, loc: Loc) extends Stmt {
  override def children: Iterable[AstNode] = List(expr)
}

case class IfStmt(guard: Expr, thenBranch: StmtInNestedBlock, elseBranch: Option[StmtInNestedBlock], loc: Loc)
    extends StmtInNestedBlock {
  override def children: Iterable[AstNode] = guard :: thenBranch :: elseBranch.map(x => List(x)).getOrElse(Nil)
}

case class WhileStmt(guard: Expr, block: StmtInNestedBlock, loc: Loc) extends StmtInNestedBlock {
  override def children: Iterable[AstNode] = List(guard, block)
}

case class OutputStmt(expr: Expr, loc: Loc) extends StmtInNestedBlock {
  override def children: Iterable[AstNode] = List(expr)
}

/**
  * Function local variables declaration.
  *
  * @param decls The list of variable declarations.
  * @param loc   The source code location.
  */
case class VarStmt(decls: List[IdentifierDecl], loc: Loc) extends Stmt {
  override def children: Iterable[AstNode] = decls
}

case class IdentifierDecl(name: String, loc: Loc) extends Decl

case class FunDecl(name: String, params: List[IdentifierDecl], block: FunBlockStmt, loc: Loc) extends Decl {
  override def toString: String = s"$name(${params.mkString(",")}){...}:$loc"

  override def children: Iterable[AstNode] = params :+ block
}

/**
  * The complete program.
  *
  * A program is just a list of functions.
  * The `main` function is where the execution begins.
  *
  * @param funs The list of functions.
  * @param loc  The source code location.
  */
case class Program(funs: List[FunDecl], loc: Loc) extends AstNode {
  def mainFunOption: Option[FunDecl] =
    funs.find(_.name == "main")

  override def children: Iterable[AstNode] = funs

  def declarations: Iterable[Decl] = tree.collect { case x: Decl => x }
}

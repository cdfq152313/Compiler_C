S
	Program $
Program
    DeclList
DeclList
	DeclList' DeclList
	epsilon
DeclList'
	Type id Decl
Decl
	VarDecl'
	FunDecl
VarDecl
	Type id VarDecl'
VarDecl'
    ;
    [ num ] ;
FunDecl
    ( ParamDeclList ) Block
VarDeclList
    VarDecl VarDeclList
    epsilon
ParamDeclList
    ParamDeclListTail
    epsilon
ParamDeclListTail
    ParamDecl ParamDeclListTail'
ParamDeclListTail'
	, ParamDeclListTail
	epsilon
ParamDecl
    Type id ParamDecl'
ParamDecl'
	[ ]
	epsilon
Block
    { VarDeclList StmtList }
Type
    int
    char
StmtList
    Stmt StmtList'
StmtList'
    StmtList
	epsilon
Stmt
    ;
    Expr ;
    return Expr ;
    break ;
    if ( Expr ) Stmt else Stmt
    while ( Expr ) Stmt
    Block
Expr
    UnaryOp Expr 
    num Expr'    
    ( Expr ) Expr' 
    id ExprIdTail
ExprIdTail //temp
	Expr'
	( ExprList ) Expr'
    [ Expr ] ExprArrayTail
    = Expr
ExprArrayTail
	Expr'
	= Expr
Expr' //op temp
	BinOp Expr
	epsilon
ExprList
    ExprListTail
    epsilon
ExprListTail
    Expr ExprListTail'
ExprListTail'
	, ExprListTail
	epsilon
UnaryOp
    -
    !
BinOp
    +
    -
    *
    /
    ==
    !=
    <
    <=
    >
    >=
    &&
    ||
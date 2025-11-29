from sly import Lexer, Parser

P = 1234577

class GFLexer(Lexer):
    tokens = { NUM, ADD, SUB, MUL, DIV, POW, LPAREN, RPAREN }
    ignore = ' \t'

    ADD = r'\+'
    SUB = r'-'
    MUL = r'\*'
    DIV = r'/'
    POW = r'\^'
    LPAREN = r'\('
    RPAREN = r'\)'

    @_(r'\#.*')
    def COMMENT(self, t):
        pass

    @_(r'\\\n')
    def ESCAPED_NEWLINE(self, t):
        pass
    
    @_(r'\n')
    def NEWLINE(self, t):
        self.lineno += 1

    @_(r'\d+')
    def NUM(self, t):
        t.value = int(t.value) % P
        return t

    def error(self, t):
        print(f"Error: lexical, {t.value[0]}")
        self.index += 1

class GFParser(Parser):
    tokens = GFLexer.tokens

    precedence = (
        ('left', ADD, SUB),
        ('left', MUL, DIV),
        ('right', UMINUS),
        ('nonassoc', POW),
    )
    @_('expr')
    def line(self, p):
        print(f"\nResult: {p.expr}")

    @_('expr ADD expr')
    def expr(self, p):
        print("+ ", end="")
        return (p.expr0 + p.expr1) % P

    @_('expr SUB expr')
    def expr(self, p):
        print("- ", end="")
        return (p.expr0 - p.expr1) % P

    @_('expr MUL expr')
    def expr(self, p):
        print("* ", end="")
        return (p.expr0 * p.expr1) % P

    @_('expr DIV expr')
    def expr(self, p):
        print("/ ", end="")
        if p.expr1 == 0:
            print("\nError: division by zero.")
            return 0
        inv = pow(p.expr1, -1, P)
        return (p.expr0 * inv) % P

    @_('expr POW expr')
    def expr(self, p):
        print("^ ", end="")
        try:
            return pow(p.expr0, p.expr1, P)
        except ValueError:
            return 0

    @_('SUB expr %prec UMINUS')
    def expr(self, p):
        print("NEG ", end="") 
        return (-p.expr) % P

    @_('LPAREN expr RPAREN')
    def expr(self, p):
        return p.expr

    @_('NUM')
    def expr(self, p):
        print(f"{p.NUM} ", end="")
        return p.NUM

    def error(self, p):
        if p:
            print(f"\Error: syntax at {p.value}")
        else:
            print("\nError: syntax at the end")

if __name__ == '__main__':
    lexer = GFLexer()
    parser = GFParser()
    
    while True:
        try:
            text = input('>> ')
            if not text: continue
            parser.parse(lexer.tokenize(text))
            
        except (EOFError, KeyboardInterrupt):
            break
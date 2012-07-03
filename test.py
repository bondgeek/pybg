
class Foo(object):
    @classmethod    
    def fut_strip(cls, n, code=None):
        limit = n
        countr = 0

        while countr < limit:
            code = ql.IMM.nextCode(code)
            yield code
            countr += 1
   

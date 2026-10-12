"""Petit lecteur S-expression pour les bibliothèques KiCad."""
import re,json
def parse(text):
    stack=[];root=None
    for token in re.findall(r'"(?:\\.|[^"\\])*"|[()]|[^\s()]+',text):
        if token=='(':
            n=[]
            if stack:stack[-1].append(n)
            else:root=n
            stack.append(n)
        elif token==')':stack.pop()
        else:stack[-1].append(json.loads(token) if token.startswith('"') else token)
    return root
def field(n,key):return next((x[1:] for x in n if isinstance(x,list) and x and x[0]==key),[])


import re

def parse_enum(source):
  enum_block = re.search(r'enum\s+VariablePosition_T\s*{([^}]*)}', source, re.DOTALL)
  if not enum_block:
    return {}

  entries = [e.strip() for e in enum_block.group(1).split(',') if e.strip()]
  mapping = {name: idx for idx, name in enumerate(entries)}
  return mapping


class output(object):
  def __init__(self,mapping):
    self.pos=0
    self.target=0
    self.out=[]
    self.whilepos=[]
    self.enum_map=mapping
  def _execMove(self):
    d = self.target-self.pos
    code = '>' * d if d > 0 else '<' * (-d)
    self.out.append(code)
    self.pos=self.target
  def goTo(self,var):
    self.target = self.enum_map[var]
  def move(self,n):
    self.target = self.target+n
  def increment(self,pos=None):
    if(pos):
      self.goTo(pos)
    self._execMove()
    self.out.append('+')
  def decrement(self,pos=None):
    if(pos):
      self.goTo(pos)
    self._execMove()
    self.out.append('-')
  def whileStart(self,pos=None):
    if(pos):
      self.goTo(pos)
    self._execMove()
    self.whilepos.append(self.pos)
    self.out.append('[')
  def whileEnd(self):
    self.target = self.whilepos.pop()
    self._execMove()
    self.out.append(']')
  def code(self):
    if self.whilepos:
      print( self.whilepos )
      raise Exception("while not ended")
    return ''.join(self.out)





def translate_instruction(line, out):
  line = line.strip()

  # Inkrement
  m = re.match(r'd\[([^;]+)\]\s*\+\+\s*;', line)
  if m:
    var = m.group(1)
    out.increment( var )
    translate_instruction( line[m.end():], out )
    return

  # Dekrement
  m = re.match(r'd\[([^;]+)\]\s*--;', line)
  # pointer Dekrement
  if m:
    var = m.group(1)
    out.decrement( var )
    translate_instruction( line[m.end():], out )
    return

  # while(d[VAR])
  m = re.match(r'while\s*\(\s*d\[([^;]+)\]\s*\)', line)
  if m:
    var = m.group(1).strip()
    if var=='p':
      out.whileStart()
    else:
      out.whileStart(var)
    translate_instruction( line[m.end():], out )
    return

  # Blockend
  m = re.match(r'}', line)
  if m:
    out.whileEnd()
    translate_instruction( line[m.end():], out )
    return

  # p = V_xyz;
  m = re.match(r'p\s*=\s*([^;]+);', line)
  if m:
    var = m.group(1)
    out.goTo(var)
    translate_instruction( line[m.end():], out )
    return

  # p++
  m = re.match(r'p\s*\+\+\s*;', line)
  if m:
    out.move( 1 )
    translate_instruction( line[m.end():], out )
    return

  # p--
  m = re.match(r'p\s*--\s*;', line)
  if m:
    out.move( -1 )
    translate_instruction( line[m.end():], out )
    return

  m = re.match(r'{', line)
  if m:
    translate_instruction( line[m.end():], out )
    return




def convert_to_brainfuck(source):
  lines = source.splitlines()
  enum_map = parse_enum(source)

  in_enum = False
  out = None
  for line in lines:
    if 'enum' in line:
      in_enum = True
    if in_enum:
      if '};' in line:
        in_enum = False
      out = output( enum_map )
      continue

    translate_instruction(line, out)
  return out


# Beispielnutzung:
if __name__ == '__main__':
  with open('goldbach_simple.c', 'r') as f:
    source = f.read()

  bf = convert_to_brainfuck(source)
  print(bf.code())


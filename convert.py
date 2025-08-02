
import re

def parse_enum(source):
  enum_block = re.search(r'enum\s+VariablePosition_T\s*{([^}]*)}', source, re.DOTALL)
  if not enum_block:
    return {}

  entries = [e.strip() for e in enum_block.group(1).split(',') if e.strip()]
  mapping = {name: idx for idx, name in enumerate(entries)}
  return mapping



class output(object):
  def __init__(self, mapping):
    self.pos = 0
    self.target = 0
    self.out = []
    self.whilepos = []
    self.enum_map = mapping
    self.lastComment="" #used for short/fine debuging output
    self.lastTarget=None
    self.moves = [[0 for _ in range(len(mapping))] for _ in range(len(mapping))]

    self.nextDebugLine = '' # used for long/corse debugging
    self.longDebugCode=[]
    self.longDebugOut=[]

  def emit(self, code, src):
    self.out.append((code, f"// {src}" if src else ""))
    self.longDebugCode.append(code)

  def _execMove(self):
    d = self.target - self.pos
    code = '>' * d if d > 0 else '<' * (-d)
    self.pos = self.target
    self.emit(code, self.lastComment)
    self.lastComment=""

  def _recordMove(self, varables):
    a = self.enum_map[varables[0]]
    b = self.enum_map[varables[1]]
    self.moves[a][b] += 1

  def goTo(self, var, _src=""):
    if self.lastTarget is not None and self.lastTarget!=var:
      self._recordMove([var,self.lastTarget])
    self.lastTarget=var;
    self.target = self.enum_map[var]

  def move(self, n, src=""):
    self.lastComment = self.lastComment + src
    self.pos -= n

  def increment(self, pos=None, src=""):
    if pos:
      self.goTo(pos)
    self._execMove()
    self.emit('+', src)

  def decrement(self, pos=None, src=""):
    if pos:
      self.goTo(pos)
    self._execMove()
    self.emit('-', src)

  def print(self, src=""):
    self._execMove()
    self.emit('.', src)

  def whileStart(self, pos=None, src=""):
    if pos:
      self.goTo(pos)
    self.nextWhilePos=self.lastTarget
    self._execMove()
    self.emit('[', src)

  def blockEnd(self, src=""):
    target = self.whilepos.pop()
    if target is not None:
      self.goTo(target)
      self._execMove()
      self.emit(']', src)

  def blockStart(self, src=""):
    self.whilepos.append(self.nextWhilePos)
    self.nextWhilePos=None
    if src:
      self.emit('', src)


  def appendDebugLine(self,line):
    if self.nextDebugLine or self.longDebugCode:
      self.longDebugOut.append((f"{''.join(self.longDebugCode):<48} // {self.nextDebugLine}"))
    self.nextDebugLine=line
    self.longDebugCode=[]


  def code(self):
    if self.whilepos:
      raise Exception("while not ended")
    return ''.join(code for code, _ in self.out)

  def debug_code(self):
    if self.whilepos:
      raise Exception("while not ended")
    return "\n".join(f"{code:<8} {comment}" for code, comment in self.out if code or comment )

  def longDebug_out(self):
    self.appendDebugLine('')
    return "\n".join(self.longDebugOut)

  def jumpCountString(self):
    W = 4 # with of a cell
    L = 12 # length of the strings
    #creat column titles read from top to bottom
    #creat string array for titles
    sArray = \
        [
          f"{list(self.enum_map.keys())[N//W]:>{L}}"
            if N%W==0 else " "*L
          for N in range(W*len(self.moves))
        ]
    # rotate titles
    s = '\n'.join( " "*(L+W) + ''.join(i) for i in zip(*sArray) )
    return s + '\n' + "\n".join\
      (
        [
          f"{list(self.enum_map.keys())[N]:<{L}}" +
          "[" + "".join(f"{e:>{W}}" for e in zeile) + "]"
          for N,zeile in enumerate(self.moves)
        ]
      ) + '\n'


class MethodenZaehler:
  def __init__ (self):
    self.zaehler = {}

  def meine_methode (self, move):
    if move in self.zaehler:
      self.zaehler[move] += 1
    else:
      self.zaehler[move] = 1

  def statistik_string (self):
    eintraege = sorted(self.zaehler.items(),
                       key=lambda eintrag: eintrag[1],
                       reverse=True)
    return "\n".join(f"{n:>3} : {move}"
                     for move, n in eintraege)

def translate_instruction(line, out):
  line = line.strip()

  if not line or line==';':
    return

  # Inkrement: d[x]++;
  m = re.match(r'd\[([^;]+)\]\s*\+\+\s*;', line)
  if m:
    var = m.group(1)
    out.increment(var, line[:m.end()])
    translate_instruction(line[m.end():], out)
    return

  # Dekrement: d[x]--;
  m = re.match(r'd\[([^;]+)\]\s*--\s*;', line)
  if m:
    var = m.group(1)
    out.decrement(var, line[:m.end()])
    translate_instruction(line[m.end():], out)
    return

  # while (d[x])
  m = re.match(r'while\s*\(\s*d\[([^;]+)\]\s*\)', line)
  if m:
    var = m.group(1).strip()
    out.whileStart(None if var == 'p' else var, line[:m.end()])
    translate_instruction(line[m.end():], out)
    return
  # {
  m = re.match(r'{', line)
  if m:
    out.blockStart(line[:m.end()])
    translate_instruction(line[m.end():], out)
    return

  # Blockende }
  m = re.match(r'}', line)
  if m:
    out.blockEnd(line[:m.end()])
    translate_instruction(line[m.end():], out)
    return

  # p = xyz;
  m = re.match(r'p\s*=\s*([^;]+);', line)
  if m:
    var = m.group(1)
    out.goTo(var, line[:m.end()])
    translate_instruction(line[m.end():], out)
    return

  # p++;
  m = re.match(r'p\s*\+\+\s*;', line)
  if m:
    out.move(1, line[:m.end()])
    translate_instruction(line[m.end():], out)
    return

  # p--;
  m = re.match(r'p\s*--\s*;', line)
  if m:
    out.move(-1, line[:m.end()])
    translate_instruction(line[m.end():], out)
    return

  # print
  m = re.match(r'print\([^)]*?\);', line)
  if m:
    out.print(line[:m.end()])
    translate_instruction(line[m.end():], out)
    return

  # debug
  m = re.match(r'debug\([^)]*?\);', line)
  if m:
    #ignore debug
    translate_instruction(line[m.end():], out)
    return

  # printf
  m = re.match(r'printf\([^)]*?\);', line)
  if m:
    #ignore printf
    translate_instruction(line[m.end():], out)
    return

  raise Exception("could not parse "+line)




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

    out.appendDebugLine(line)
    translate_instruction(line, out)
  return out


if __name__ == '__main__':
  with open('goldbach_simple.c', 'r') as f:
    source = f.read()

  out = convert_to_brainfuck(source)
  with open("out.bf", "w") as f:
    f.write(out.code())
  with open("debug_out_expression.txt", "w") as f:
    f.write(out.debug_code())
  with open("debug_out_line.txt", "w") as f:
    f.write(out.longDebug_out())
  with open("jumpcount.txt", "w") as f:
    f.write(out.jumpCountString())




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
    self.lastComment=""

  def emit(self, code, src):
    self.out.append((code, f"// {src}" if src else ""))

  def _execMove(self):
    d = self.target - self.pos
    code = '>' * d if d > 0 else '<' * (-d)
    self.pos = self.target
    self.emit(code, self.lastComment)
    self.lastComment=""

  def goTo(self, var, _src=""):
    self.target = self.enum_map[var]

  def move(self, n, src=""):
    self.lastComment = self.lastComment + src
    self.target += n
    self._execMove()
    self.pos -= n
    self.target -= n

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

  def whileStart(self, pos=None, src=""):
    if pos:
      self.goTo(pos)
    self._execMove()
    self.whilepos.append(pos)
    self.emit('[', src)

  def whileEnd(self, src=""):
    print( self.whilepos )
    target = self.whilepos.pop()
    if target is not None:
      self.goTo(target)
    self._execMove()
    self.emit(']', src)

  def code(self):
    if self.whilepos:
      raise Exception("while not ended")
    return ''.join(code for code, _ in self.out)

  def debug_code(self):
    if self.whilepos:
      raise Exception("while not ended")
    return "\n".join(f"{code:<8} {comment}" for code, comment in self.out if code or comment )


def translate_instruction(line, out):
  line = line.strip()

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

  # Blockende }
  m = re.match(r'}', line)
  if m:
    out.whileEnd(line[:m.end()])
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

  # {
  m = re.match(r'{', line)
  if m:
    translate_instruction(line[m.end():], out)
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


if __name__ == '__main__':
  with open('goldbach_simple.c', 'r') as f:
    source = f.read()

  out = convert_to_brainfuck(source)
  with open("out.bf", "w") as f:
    f.write(out.code())
  with open("debug_out.txt", "w") as f:
    f.write(out.debug_code())




#!/usr/bin/env python3
# create statistics about which brainfuck commands are used how often

import sys
from collections import Counter

if __name__ == "__main__":
    with open("./out.bf", "r", encoding="utf-8") as f:
        content = f.read()

    commands = "><+-.,[]"
    code = [c for c in content if c in commands]
    total = len(code)

    if total == 0:
        sys.exit(1)

    counter = Counter(code)

    groups = { "<>", "+-", "[]", ".," }

    with open("sizes.txt", "w") as f:
      print(f"{'Total':<6} : {total:4d} ({100:6.2f}%)",file=f)
      print("-" * 33,file=f)
      for c in commands:
          count = counter.get(c, 0)
          percent = (count / total) * 100
          print(f"{c:<6} : {count:4d} ({percent:6.2f}%)",file=f)
      print("-" * 33,file=f)
      for g in groups:
          group_count = sum(counter.get(ch, 0) for ch in g)
          percent = (group_count / total) * 100
          print(f"{g:<6} : {group_count:4d} ({percent:6.2f}%)",file=f)


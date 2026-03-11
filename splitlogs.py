import re
import sys
from collections import defaultdict

def split_logs_by_identifier(log_file, pattern=r"(?:FT|PT)_(?:MAC|PHY)\[[0-9]+\]"):
    files = defaultdict(list)

    with open(log_file) as f:
        for line in f:
            match = re.search(pattern, line)
            if match:
                identifier = match.group()
                files[identifier].append(line)

    for identifier, lines in files.items():
        safe_name = identifier.replace("[", "_").replace("]", "")
        output_file = f"./logs/{safe_name}.log"
        with open(output_file, "w") as f:
            f.writelines(lines)
        print(f"Wrote {len(lines)} lines to {output_file}")

if __name__ == "__main__":
    log_file = sys.argv[1] if len(sys.argv) > 1 else "dect2020ext.log"
    split_logs_by_identifier(log_file)
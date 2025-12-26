import sys


if __name__ == "__main__":
    try:
        num1 = float(sys.argv[1].strip())
        num2 = float(sys.argv[2].strip())
        print(0)
    except ValueError:
        print(1)

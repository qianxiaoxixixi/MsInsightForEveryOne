import pandas as pd
import sys

suffix1 = 'xlsx'
suffix2 = 'csv'

input_file = sys.argv[1]
output_file = input_file[:-len(suffix1)] + suffix2

df = pd.read_excel(input_file, nrows=0)
columns = df.columns
converts = {col: str for col in columns}
df = pd.read_excel(input_file, converters=converts)
df.to_csv(output_file, na_rep="None", index=False)

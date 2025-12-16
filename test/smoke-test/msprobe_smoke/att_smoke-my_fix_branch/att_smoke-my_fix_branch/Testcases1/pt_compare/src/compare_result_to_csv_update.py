import pandas as pd
import sys

suffix1 = 'xlsx'
suffix2 = 'csv'

input_file = sys.argv[1]
output_file = input_file[:-len(suffix1)] + suffix2

df = pd.read_excel(input_file, keep_default_na=False)
df.to_csv(output_file, index=False)

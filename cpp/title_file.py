# Genera un archivo con los títulos de los archivos
print('Loading libraries')
import pandas as pd

dfs = []

for i in range(3):
    print('Reading file', i+1)
    df = pd.read_csv('/opt/datasets/articles{}.csv'.format(i+1))
    print('Processing file', i+1)
    df = df[['id', 'title']]
    df = df.set_index('id')
    dfs.append(df)

print('Writing whole dataset')
df_all = pd.concat(dfs)
df_all.to_csv('../data/title_file.csv'.format(i+1), header=False, encoding='utf-8', index=True, sep=';')

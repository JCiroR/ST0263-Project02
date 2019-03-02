print('Loading libraries')
from sklearn.feature_extraction import text
import pandas as pd

dfs = []
stop = text.ENGLISH_STOP_WORDS

def remove_stopwords(st):
    return ' '.join([x.lower().strip('.?!()" ,;-_”') for x in st.split() if x.lower() not in stop])

for i in range(3):
    print('Reading file', i+1)
    df = pd.read_csv('data/articles{}.csv'.format(i+1))
    print('Processing file', i+1)
    df = df[['id', 'title', 'content']]
    df = df.set_index('id')
    df['content'] = df['content'].map(remove_stopwords)
    dfs.append(df)

df_all = pd.concat(dfs)
df_all = df_all.dropna()
print('{}/{} are NaN'.format(df_all['content'].count(), len(df_all)))
print(df_all.sample(5))
df_all.to_csv('data/all_data.csv', encoding='utf8')
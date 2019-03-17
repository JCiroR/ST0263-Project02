print('Loading libraries')
from sklearn.feature_extraction import text
import pandas as pd

dfs = []
stop = text.ENGLISH_STOP_WORDS

def remove_stopwords(st):
    return ' '.join([x.lower().strip('.?!()" ,;-_”[]()’“&^`’*') for x in st.split() if x.lower() not in stop])

for i in range(3):
    print('Reading file', i+1)
    df = pd.read_csv('/opt/datasets/articles{}.csv'.format(i+1))
    print('Processing file', i+1)
    df = df[['id', 'title', 'content']]
    df['content'] = df['content'].map(remove_stopwords)
    df.to_csv('../data/articles{}_semicolon.csv'.format(i+1), header=False, encoding='utf-8', index=False, sep=';')

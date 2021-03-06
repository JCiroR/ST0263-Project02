import cProfile, pstats, io

from collections import Counter
import heapq
import pandas as pd
import json
print('Loaded libraries')

pr = cProfile.Profile()
pr.enable()
K = 10

df = pd.read_csv('data/sample.csv')
df = df.dropna()
print('Loaded data')

inverse_index = {}

processed_rows = 0
for row in df.itertuples():
    row_id = int(row.id)
    c = Counter(row.content.split())
    for key, value in c.items():
        if key not in inverse_index:
            inverse_index[key] = []
        #inverse_index[key].append((row_id, row.title, value))
        if len(inverse_index[key]) == K:
            heapq.heappushpop(inverse_index[key], (value, row_id, row.title))
        else:
            heapq.heappush(inverse_index[key], (value, row_id, row.title))

    processed_rows += 1
    if processed_rows%1000==0:
        print(processed_rows, 'rows processed')
        print('{} different words so far'.format(len(inverse_index)))

processed_words = 0
for key, value in inverse_index.items():
    inverse_index[key].sort(reverse=True)
    processed_words += 1
    if processed_words%10000==0:
        print(processed_words, 'words processed')

pr.disable()
s = open('profile.txt', 'w')
ps = pstats.Stats(pr, stream=s).sort_stats('tottime')
ps.print_stats()

with open('output.txt', 'w', encoding='utf-8') as f:
    json.dump(inverse_index, f, ensure_ascii=False)


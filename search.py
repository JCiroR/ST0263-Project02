import json

archivo_csv_con_las_weas = open('output.txt', 'r', encoding='utf-8')
f = json.load(archivo_csv_con_las_weas)

while True:
    search_for = input('Ingrese un termino de busqueda:').lower()
    print('*'*40)
    if search_for not in f:
        print('No se han encontrado resultados', end='\n\n')
        continue

    for idx, title, value in f[search_for]:
        print('Id: {} Apariciones: {}'.format(idx, value))
        print(title.strip(), end='\n\n')
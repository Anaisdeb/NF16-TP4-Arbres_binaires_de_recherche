import time
import random
import sys

debut_annee = 2020
fin_annee = 2022
nb_dates = 25
nb_min_vacs = 100
nb_max_vacs = 2000
output_filename = 'exemple.txt'

file = open(output_filename, 'w')
sys.stdout = file

a1 = (debut_annee, 1, 1, 0, 0, 0, 0, 0, 0)  # begin time tuple（YYYY-01-01 00：00：00）
a2 = (fin_annee, 12, 31, 23, 59, 59, 0, 0, 0)  # end time tuple（YYYY-12-31 23：59：59）

start = time.mktime(a1)  # generate begin timestamp
end = time.mktime(a2)  # generate end timestamp

print("// <nbr_dates> <null>\n// <date> <nbr_marques>\n// <marque> <nbr_vaccins>")
print(nb_dates)

# generate nb_dates dates & corresponding vaccines in random
for i in range(nb_dates):
    list_mk = ['Moderna', 'Pfeizer', 'Janssen', 'AstraZeneca', 'Gamaleya', 'Novavax', 'Sinopharm', 'CureVac', 'Sanofi',
               'Sinovac', 'Medicago', 'QazVac', 'CanSino', 'Bharat', 'Razi', 'Pasteur', 'Vektor', 'Valneva', 'Finlay',
               'Cadila', 'BioNTech', 'BARDA', 'RAS', 'Barakat', 'CIGB', 'Minhai', 'AnGes', 'ReiThera', 'Nanogen', 'Codagenix']
    t = random.randint(start, end)  # random timestamp
    date_tuple = time.localtime(t)  # convert timestamp to tuple
    date = time.strftime("%Y-%m-%d", date_tuple)  # convert tuple to sting （YYYY-MM-DD）
    j = random.randint(1, 20)
    print(date, j)
    for k in range(j):
        nbr = random.randrange(nb_min_vacs, nb_max_vacs, 100)
        mk = random.choice(list_mk)
        list_mk.remove(mk)
        print(mk, nbr)

file.close()
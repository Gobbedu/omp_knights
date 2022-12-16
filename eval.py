from ast import literal_eval

def main():
    with open('p.log', 'r') as p:
        paral = literal_eval(p.readline())

    with open('s.log', 'r') as s:
        seria = literal_eval(s.readline())

    # print(f'lens: {len(paral)} & {len(seria)}')


    Speed = list()
    for t1, tn in zip(seria, paral):
        Speed.append(t1/tn)

    S = sum(Speed)/len(Speed)
    print(f'avg speedup: {S}')


if __name__ == "__main__":
    main()
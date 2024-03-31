if __name__ == '__main__':
    size = 4 * 1024 * 1024 + 1
    file_data = bytearray([0] * size)

    for offset in [0, 10000, size - 1]:
        file_data[offset] = 1

    with open('A', mode='wb') as file:
        file.write(file_data)

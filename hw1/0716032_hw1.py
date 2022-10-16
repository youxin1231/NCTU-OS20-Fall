import time
import asyncio
import hashlib
import requests
from lxml.html import fromstring
from concurrent.futures import ProcessPoolExecutor, ThreadPoolExecutor, as_completed


def Task1(input_data):
    for i in range(0x21, 0x7f):
        for j in range(0x21, 0x7f):
            for k in range(0x21, 0x7f):
                for l in range(0x21, 0x7f):
                    for m in range(0x21, 0x7f):
                        sha256 = hashlib.sha256()
                        test = chr(i) + chr(j) + chr(k) + \
                            chr(l) + chr(m) + input_data
                        sha256.update(test.encode('utf-8'))
                        h = sha256.hexdigest()
                        if h[0] == "0" and h[1] == "0" and h[2] == "0" and h[3] == "0" and h[4] == "0":
                            return test


async def Task1_async(loop, input_data):
    for i in range(0x21, 0x7f):
        for j in range(0x21, 0x7f):
            for k in range(0x21, 0x7f):
                for l in range(0x21, 0x7f):
                    for m in range(0x21, 0x7f):
                        sha256 = hashlib.sha256()
                        test = chr(i) + chr(j) + chr(k) + \
                            chr(l) + chr(m) + input_data
                        sha256.update(test.encode('utf-8'))
                        h = sha256.hexdigest()
                        if h[0] == "0" and h[1] == "0" and h[2] == "0" and h[3] == "0" and h[4] == "0":
                            await loop.run_in_executor(None, print,test)
                            return


def Task2(input_data):
    r = requests.get(input_data)
    tree = fromstring(r.content)
    return tree.findtext('.//title')


async def Task2_async(loop, input_data):
    r = await loop.run_in_executor(None, requests.get, input_data)
    tree = fromstring(r.content)
    print(tree.findtext('.//title'))


def main():
    task = int(input())
    method = input()
    if method[0] == "1" or method[0] == "2":
        method, process_num = method.split()
        method = int(method)
        process_num = int(process_num)
    else:
        method = int(method)
    input_num = int(input())
    input_data = []
    for i in range(input_num):
        input_data.append(input())

    start = time.time()

    if task == 1:
        if method == 1:  # Multi-threading
            with ThreadPoolExecutor(max_workers=process_num) as executor:
                results = executor.map(Task1, input_data)
                for r in results:
                    print(r)
        elif method == 2:  # Multi-processing
            with ProcessPoolExecutor(max_workers=process_num) as executor:
                results = executor.map(Task1, input_data)
                for r in results:
                    print(r)
        elif method == 3:
            loop = asyncio.get_event_loop()
            tasks = []
            for string in input_data:
                tasks.append(loop.create_task(Task1_async(loop, string)))
            loop.run_until_complete(asyncio.wait(tasks))

    elif task == 2:
        if method == 1:  # Multi-threading
            with ThreadPoolExecutor(max_workers=process_num) as executor:
                results = executor.map(Task2, input_data[i])
                for r in results:
                    print(r)
        elif method == 2:  # Multi-processing
            with ProcessPoolExecutor(max_workers=process_num) as executor:
                results = executor.submit(Task2, input_data[i])
                for r in results:
                    print(r)
        elif method == 3:
            loop = asyncio.get_event_loop()
            tasks = []
            for url in input_data:
                tasks.append(loop.create_task(Task2_async(loop, url)))
            loop.run_until_complete(asyncio.wait(tasks))
    print("%.2f" % (time.time() - start) + ' seconds')


if __name__ == '__main__':
    main()
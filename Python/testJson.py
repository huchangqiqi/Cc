import json
from urllib import request


def main():
    url = "http://www.lagou.com/jobs/positionAjax.json?"
    data = request.urlopen(url)
    jcontent = data.read().decode('utf-8')
    jsonData = json.loads(jcontent)

    jobList =  jsonData['content']['positionResult']['result']

    for res in jobList:
        print(res)
    pageNo = jsonData["content"]["pageNo"]
    print("page:", pageNo)
    pageSize = jsonData["content"]["pageSize"]
    print(("size:", pageSize))


if __name__ == '__main__':
    main()

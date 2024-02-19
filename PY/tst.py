import os
def findAPI(dir,findStr):
        for dirpath, dirnames, filenames in os.walk(dir):
                for filename in filenames:
                        file_path = os.path.join(dirpath, filename)
                        with open(file_path,"r",encoding="UTF_8") as f:
                                for line in f.readlines():
                                        if findStr in line:
                                                print(dirpath+"   "+line)
                

findAPI(dir="C:\\Users\\19878\\Desktop\\aaa",findStr="sss")


          

import re, os, fnmatch

def getFilesList (basefolder, pattern, discard):
    matches = []
    myfilenames = []
    for root, dirnames, filenames in os.walk (basefolder):
        for filename in fnmatch.filter(filenames, pattern):
            fullname = os.path.join (root, filename)
            count = 0
            for elem in discard:
                if ('.'+elem+'.' in fullname) or ('_'+elem+'/' in fullname):
                    count = 1
                    break
            if count > 0 : continue
            matches.append (os.path.join (os.getcwd (), root, filename))
            myfilenames.append (filename)
    return [matches, myfilenames]

ADC=[]
files_out = getFilesList ('241Am', '*.txt', [])
for i in files_out[0]:
    with open(i, "r") as file:
        scinot = re.compile('ADC Conversion Number:\n(\d*)\n')
        adc = re.findall(scinot,file.read())
        if len(adc)!=0:
            ADC.append([i,int(adc[0])])
        else:
            print(adc,i)

for i in ADC:
    print(i[0]+'\t'+str(i[1]) )

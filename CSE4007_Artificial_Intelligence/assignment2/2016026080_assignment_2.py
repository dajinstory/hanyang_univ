import random
import math
import numpy as np
import pandas as pd
from konlpy.tag import Twitter


twitter=Twitter()

def generate_BigDataCenter(filename, smoothing_K):
    
    samples = pd.read_csv(filename, "\t")
    documents = samples["document"]
    labels = samples["label"]

    big_data_center = []
    dc_idxs={}

    for idx in range(len(documents)):
        if idx%500 == 0:
            print(idx)
        document=documents[idx]
        label=labels[idx]

        # filter nan
        if(document!=document):
            continue

        tokens = ['/'.join(t) for t in twitter.pos(document, norm=True, stem=True)]
        repeat={}
        for token in tokens:
            if token in repeat:
                continue
            repeat[token]=1

            if token in dc_idxs:
                big_data_center[dc_idxs[token]]["counts"]+=1
                big_data_center[dc_idxs[token]]["counts_H" if label==1 else "counts_NH"]+=1
            else:
                dc_idxs[token]=len(big_data_center)
                big_data_center.append({"parameter":token, "counts":1, "counts_H":0, "counts_NH":0, "p_from_H":0, "p_from_NH":0, "priority":0})
                big_data_center[dc_idxs[token]]["counts_H" if label==1 else "counts_NH"]+=1

    H_NUM = len(np.where(labels==1)[0])
    NH_NUM = len(np.where(labels==0)[0])

    for data in big_data_center:
        data["p_from_H"]= (data["counts_H"]+smoothing_K)/(H_NUM+2*smoothing_K)
        data["p_from_NH"]= (data["counts_NH"]+smoothing_K)/(NH_NUM+2*smoothing_K)
        data["priority"]=max([ data["counts_H"],data["counts_NH"] ])/(data["counts"]) 

    sorted_big_data_center = sorted(big_data_center, key=lambda data:(data["counts"], data["priority"]))
    sorted_big_data_center.reverse()
    
    pd.DataFrame(sorted_big_data_center).to_csv("naive_bassian_models/BigDataCenter.csv")
    
    for data in sorted_big_data_center[:10]:
        print(data)

def make_DataCenter(filename, num_of_parameter, min_counts, min_priority):
    big_data_center_pd = pd.read_csv("naive_bassian_models/BigDataCenter.csv")
    priorities=big_data_center_pd["priority"]
    columns=big_data_center_pd.columns
    idxs=np.where(priorities>=min_priority)[0].astype(int)

    data_center=[]
    for idx in idxs:
        if (len(data_center)==num_of_parameter) or (big_data_center_pd["counts"][idx]<min_counts):
            break

        node ={}
        for column in columns:
            node[column]=big_data_center_pd[column][idx]
        data_center.append(node)

    pd.DataFrame(data_center).to_csv(filename)
    print("Make Datacenter with len: "+str(len(data_center)))

def score_model(model_name, sample_name):
    data_center_pd = pd.read_csv(model_name)
    parameters=data_center_pd["parameter"]
    p_H=data_center_pd["p_from_H"]
    p_NH=data_center_pd["p_from_NH"]
    
    samples=pd.read_csv(sample_name, "\t")
    documents=samples["document"]
    labels=samples["label"]
    
    score=0
    for idx in range(len(documents)):
        if idx%50 == 0:
            print(idx, score)
        document=documents[idx]
        label=labels[idx]
        if get_label(parameters, p_H, p_NH, document)==label:
            score+=1
    
    return score*100/len(documents)
            
            

def get_label(parameters, P_H, P_NH, document):
    if document != document:
        return 1
    
    MULFACTOR=10000
    MINFACTOR=0.000001
    p_H=math.log(MULFACTOR*89966/180000)
    p_NH=math.log(MULFACTOR*90034/180000)
    tokens = ['/'.join(t) for t in twitter.pos(document, norm=True, stem=True)]
    for idx in range(len(parameters)):
        parameter=parameters[idx]
        if parameter in tokens:
            p_H += math.log(MULFACTOR * max([P_H[idx], MINFACTOR]))
            p_NH += math.log(MULFACTOR * max([P_NH[idx], MINFACTOR]))
        else:
            p_H += math.log(MULFACTOR * max([1-P_H[idx], MINFACTOR]))
            p_NH += math.log(MULFACTOR * max([1-P_NH[idx], MINFACTOR]))
            
    return 1 if p_H>=p_NH else 0
        

def optimize_parameters():
    scores=[]
    for PARAMS in [100000]:
        for MIN_COUNTS in [1]:
            for MIN_PRIORITY in [0.5]:

                #PARAMS=1000
                #MIN_COUNTS=100
                #MIN_PRIORITY=0.62
                model_name="naive_bassian_models/data_center_cases/data_center_"+str(PARAMS)+"_"+str(MIN_COUNTS)+"_"+str(MIN_PRIORITY)+".csv"
                make_DataCenter(model_name, PARAMS, MIN_COUNTS, MIN_PRIORITY)
                validation_sample = "ratings_valid.txt"
                score = score_model(model_name, validation_sample)
                print(model_name, score)
                scores.append({"combination":str(PARAMS)+"_"+str(MIN_COUNTS)+"_"+str(MIN_PRIORITY), "score": score})

    scores_pd = pd.DataFrame(scores)
    scores_pd.to_csv("naive_bassian_models/Result_optimizing_parameters2.csv")

def predict(model_name, sample_name):
    data_center_pd = pd.read_csv(model_name)
    parameters=data_center_pd["parameter"]
    p_H=data_center_pd["p_from_H"]
    p_NH=data_center_pd["p_from_NH"]
    
    samples=pd.read_csv(sample_name, "\t")
    documents=samples["document"]
    labels=[]
    
    for idx in range(len(documents)):
        if idx%50==0:
            print(idx)
        document=documents[idx]
        label=get_label(parameters, p_H, p_NH, document)
        labels.append(label)
    
    labels_pd=pd.Series(labels,dtype="int32")
        
    samples["label"] = labels_pd
    samples.astype({'label':'int'})
    samples.to_csv(sample_name.split("_")[0]+"_result.txt")
    print(samples.dtypes)
    
            

training_sample="ratings_train.txt"
#generate_BigDataCenter(training_sample, 2)

#optimize_parameters()

PARAMS=100000
MIN_COUNTS=1
MIN_PRIORITY=0.5

model_name="naive_bassian_models/data_center_cases/data_center_"+str(PARAMS)+"_"+str(MIN_COUNTS)+"_"+str(MIN_PRIORITY)+".csv"
#make_DataCenter(model_name, PARAMS, MIN_COUNTS, MIN_PRIORITY)

#validation_sample = "ratings_valid.txt"
#score = score_model(model_name, validation_sample)
#print(score)

#model_name="naive_bassian_models/naive_bassian_model.csv"
test_sample = "ratings_test.txt"
predict(model_name, test_sample)







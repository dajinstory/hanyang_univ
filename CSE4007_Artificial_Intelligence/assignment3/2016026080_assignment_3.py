#!/usr/bin/env python
# coding: utf-8

# In[1]:


import random
import math
import numpy as np
import pandas as pd


# ## Load Map Data

# In[2]:


def load_map(file_name):
    file_txt = open(file_name, 'r')

    # load num, x, y
    line = file_txt.readline()
    line_list = list(map(int, line.rstrip("\n").split()))
    num,x,y = line_list

    # loat maze data
    maze=[]
    for i in range(int(x)):
        line = file_txt.readline()
        maze.append(list(line)[:y])

    file_txt.close()
    return num,np.array(maze)


# In[3]:


def save_result(tc, maze, path):
    file_name = 'FrozenLake_data/FrozenLake_%d_output.txt'%tc
    file_txt = open(file_name, 'w')
    mx, my = maze.shape[:2]
    
    # update maze with the explored place
    for pt in path:
        if maze[pt['x']][pt['y']]=='G' or maze[pt['x']][pt['y']]=='S':
            continue
        maze[pt['x']][pt['y']]='R'
    # write result
    file_txt.writelines(' '.join([str(tc),str(mx),str(my)]))
    for i in range(mx):
        #file_txt.writelines(' '.join(str(x) for x in maze[i]))
        file_txt.write('\n')    
        file_txt.writelines(str(x) for x in maze[i])


# In[4]:


#tc, maze = load_map("FrozenLake_data/FrozenLake_1.txt")


# ## Functions for maze info

# In[5]:


def make_data_table(maze):
    data_table=[]
    point_start={}
    point_end={}
    xx=[1,0,-1,0]
    yy=[0,1,0,-1]
    mx, my = maze.shape[:2]
    for x in range(mx):
        data_table.append([])
        for y in range(my):
            if maze[x][y]=='G':
                point_end={'x':x, 'y':y}
            elif maze[x][y]=='S':
                point_start={'x':x, 'y':y}
                
            adj_points=[]
            for idx in range(4):
                nx = x+xx[idx]
                ny = y+yy[idx]
                score=0
                if nx>=0 and nx<mx and ny>=0 and ny<my: #and maze[nx][ny] != 'H':
                    if maze[nx][ny]=='H':
                        score=-1
                    adj_points.append({'x':nx,'y':ny,'score':score})
            data_table[x].append(adj_points)
    return data_table, point_start, point_end


# In[6]:


#data_table, point_start, point_end=make_data_table(maze)


# ## Q Learning Example on class material

# In[7]:


def q_learning_example(maze, point_end, data_table, epoch):
    mx,my=maze.shape[:2]
    print("maze size: ", mx, my)
    
    ## training
    while epoch>0:
        x=random.randint(1,mx)-1
        y=random.randint(1,my)-1
    
        isSuccess, path, new_data_table = dfs_all(maze, data_table, {'x':x,'y':y}, point_end)
        epoch-=1
    
    return new_data_table


# In[8]:


def dfs_all(maze, data_table, point_start, point_end, path="RANDOM"):
    # set the arguments
    mx,my=maze.shape[:2]

    stack=[]
    visit=[[-1 for _ in range(my)] for _ in range(mx)]
    trace=[[{'x':-1, 'y':-1, 'score':0} for _ in range(my)] for _ in range(mx)]
    isSuccess=False

    # dfs search
    visit[point_start['x']][point_start['y']]=0
    stack.append({'pt':point_start, 'level':0})
    while stack:
        now = stack.pop()
        pt=now['pt']
        level=now['level']
        
        ## End Condition
        if pt['x'] == point_end['x'] and pt['y'] == point_end['y']:
            isSuccess=True
            break

        ## Repeat Condition
        if level > visit[pt['x']][pt['y']]:
            continue
    
        if path=="RANDOM":
            # get the next point. with random. for training
            nums = len(data_table[pt['x']][pt['y']])
            num = random.randint(1,nums)-1
            plus=0
            for plus in range(nums):
                idx=(num+plus)%nums
                pt_next = data_table[pt['x']][pt['y']][idx]
                if maze[pt_next['x']][pt_next['y']]=='H':
                    continue
                elif visit[pt_next['x']][pt_next['y']] != -1 and visit[pt_next['x']][pt_next['y']] <= level+1:
                    continue
                
                ## get the first random next_spot
                if pt_next['x'] == point_end['x'] and pt_next['y'] == point_end['y']:
                    data_table[pt['x']][pt['y']][idx]['score'] = 100 
                else:
                    max_score = max(data_table[pt_next['x']][pt_next['y']]  , key = (lambda k : k['score']))['score']
                    data_table[pt['x']][pt['y']][idx]['score'] = 0.5*max_score
                    
                visit[pt_next['x']][pt_next['y']]=level+1
                stack.append({'pt':pt_next, 'level':level+1})
                trace[pt_next['x']][pt_next['y']]=pt
        
        else:
            # get the next point. with highest score
            pt_next = max(data_table[pt['x']][pt['y']], key = (lambda k : k['score']))

            visit[pt_next['x']][pt_next['y']]=level+1
            stack.append({'pt':pt_next, 'level':level+1})
            trace[pt_next['x']][pt_next['y']]=pt
        
        
    # searching failed
    if not isSuccess:
        return isSuccess, [], data_table

    # trace the path
    path=[]
    while point_end != point_start:
        point_end = trace[point_end['x']][point_end['y']]
        path.append(point_end)
    path.reverse()
    
    return isSuccess, path, data_table


# In[9]:


maze=np.array([['S','F','F'],['F','F','G']])
data_table, point_start, point_end = make_data_table(maze)
mx,my=maze.shape[:2]
data_table = q_learning_example(maze, {'x':1, 'y':2}, data_table, epoch=100)
for x in range(mx):
    for y in range(my):
        print(x,y, data_table[x][y])


# ## Q Learning Code

# In[10]:


def q_learning(func, maze, point_start, point_end, data_table, epoch):
    mx,my=maze.shape[:2]
    print("maze size: ", mx, my)
    
    ## training
    while epoch>0:
        if epoch%100000 == 0:
            print(epoch)
        isSuccess, path, new_data_table = func(maze, data_table, point_start, point_end)
        epoch-=1
    
    return new_data_table


# In[11]:


def dfs(maze, data_table, point_start, point_end, path="RANDOM"):
    # set the arguments
    mx,my=maze.shape[:2]
    stack=[]
    trace=[]
    isSuccess=False
    threshold=0.1
    
    # dfs search
    stack.append({'pt':point_start, 'level':0})
    while stack:
        now = stack.pop()
        pt=now['pt']
        level=now['level']
        if pt not in trace:
            trace.append(pt)

        ## End Condition
        if pt['x'] == point_end['x'] and pt['y'] == point_end['y']:
            isSuccess=True
            break
        elif maze[pt['x']][pt['y']]=='H':
            isSuccess=False
            break
        
        
        if path=="RANDOM":
            # get the next point. for training
            if random.uniform(0,1)>0.0:
                #print("random")
                nums = len(data_table[pt['x']][pt['y']])
                num = random.randint(1,nums)-1
                idx=num
                pt_next_info = data_table[pt['x']][pt['y']][idx]
                pt_next = {'x':pt_next_info['x'],'y':pt_next_info['y']}
                
                cnt=2
                while pt_next in trace:
                    num = random.randint(1,nums)-1
                    idx=num
                    pt_next_info = data_table[pt['x']][pt['y']][idx]
                    pt_next = {'x':pt_next_info['x'],'y':pt_next_info['y']}
                    
                    cnt-=1
                    if cnt==0:
                        break
                        
            else:
                #print("best")
                pt_next_info = max(data_table[pt['x']][pt['y']], key = (lambda k : k['score']))
                pt_next = {'x':pt_next_info['x'],'y':pt_next_info['y']}
                
                idx = get_pt_idx(data_table[pt['x']][pt['y']], pt_next)

            ## get the first random next_spot
            if pt_next['x'] == point_end['x'] and pt_next['y'] == point_end['y']:
                data_table[pt['x']][pt['y']][idx]['score'] = 1
            elif maze[pt_next['x']][pt_next['y']]=='H':
                data_table[pt['x']][pt['y']][idx]['score'] = -1
            else:
                #max_score = max(data_table[pt_next['x']][pt_next['y']]  , key = (lambda k : k['score']))['score']
                max_score = get_max_score(data_table[pt_next['x']][pt_next['y']], pt)
                data_table[pt['x']][pt['y']][idx]['score'] = 0.5*max_score

            stack.append({'pt':pt_next, 'level':level+1})
        
        else:
            # get the next point. with highest score
            pt_next_info = max(data_table[pt['x']][pt['y']], key = (lambda k : k['score']))
            pt_next = {'x':pt_next_info['x'],'y':pt_next_info['y']}
            
            stack.append({'pt':pt_next, 'level':level+1})
        
        
    # searching failed
    if not isSuccess:
        return isSuccess, [], data_table
    #print("SUCCESS!!!")
    return isSuccess, trace, data_table


# In[12]:


def get_max_score(dt, pt_from):
    score=-1
    for data in dt:
        if data['x']==pt_from['x'] and data['y']==pt_from['y']:
            continue
        if score<data['score']:
            score=data['score']
            
    return score


# In[13]:


def get_pt_idx(table, pt):
    for idx in range(len(table)):
        if pt['x']==table[idx]['x'] and pt['y']==table[idx]['y']:
            return idx


# ## Run Code

# In[14]:


def print_dt(maze, dt):
    mx,my=len(dt), len(dt[0])
    for x in range(mx):
        for y in range(my):
            if maze[x][y]=='H':
                continue
            print("x, y : ", x, y, dt[x][y])


# In[15]:


TC=3
for _ in range(1,1+TC):
    filename = "FrozenLake_data/FrozenLake_%d.txt"%_
    tc, maze = load_map(filename)
    data_table, point_start, point_end = make_data_table(maze)
    
    ## training
    print("training")
    data_table = q_learning(dfs, maze, point_start, point_end, data_table, epoch=1000000)
    #data_table = q_learning(dfs_all, maze, point_start, point_end, data_table, epoch=50)
    
    ## check
    print_dt(maze, data_table)
    
    ## get answer
    print("get_answer")
    isSuccess, path, new_data_table = dfs(maze, data_table, point_start, point_end, path="SHORTEST")
    print("save")
    save_result(tc, maze, path)


# In[ ]:





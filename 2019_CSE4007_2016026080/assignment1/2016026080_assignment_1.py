#import argparse
#import queue

def push_pq(pq, node):
	pq.append(node)
	idx = len(pq)-1
	nidx=(idx-idx%2)/2
	while nidx>0 and pq[nidx][0]>pq[idx][0]:
		pq[idx]=pq[nidx]
		idx=nidx
		nidx=(idx-idx%2)/2
	pq[idx]=node

def pop_pq(pq):
	re = pq[1]
	max_idx=len(pq)-1
	idx = 1
	nidx = 2 + (3<=max_idx and pq[3][0]<pq[2][0])
	while nidx<=max_idx and pq[nidx][0]<pq[max_idx][0]:
		pq[idx]=pq[nidx]
		idx=nidx
		nidx=idx*2+(idx*2 + 1 <= max_idx and pq[idx*2+1][0]<pq[idx*2])
	pq[idx]=pq[max_idx]
	pq.pop()
	return re

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
	
	return num,maze

def save_result(test_num, algorithm_name, maze, path, time):
	file_name = 'Maze_'+str(test_num)+'_'+algorithm_name+'_output.txt'
	file_txt = open(file_name, 'w')
	mx, my = get_map_size(maze)

	# update maze with the explored place
	for pt in path:
		maze[pt['x']][pt['y']]='5'
	# write result
	for i in range(mx):
		file_txt.writelines(' '.join(str(x) for x in maze[i]))
		file_txt.write('\n')

	file_txt.write('---\n')
	file_txt.write('length='+str(len(path)+1)+'\n')
	file_txt.write('time='+str(time))
	file_txt.close()
	'''	
	# check explored maze on kernel
	print(algorithm_name)
	for i in range(mx):
		print(' '.join(str(x) for x in maze[i]))
	print('---')
	print('length='+str(len(path)+1))
	print('time='+str(time)+'\n')
	'''

def get_map_size(maze):
	return len(maze), len(maze[0])

def extract_elements_from_map(maze):
	# 3: start point
	# 4: end point
	# 5: key point
	mx, my = get_map_size(maze)
	for i in range(mx):
		for j in range(my):
			now = maze[i][j]
			if now == '3':
				point_start = {'x' : i, 'y' : j}
			elif now == '4':
				point_end = {'x' : i, 'y' : j}
			elif now == '6':
				point_key = {'x' : i, 'y' : j}
	return point_start, point_end, point_key


def get_adj_points(maze, point):
	mx, my = get_map_size(maze)
	adj_points=[]
	xx=[1,0,-1,0]
	yy=[0,1,0,-1]
	for idx in range(4):
		px=xx[idx]
		py=yy[idx]
		nx=point['x']+px
		ny=point['y']+py
		if nx>=0 and nx<mx and ny>=0 and ny<my and maze[nx][ny] != '1':
			adj_points.append({'x':nx, 'y':ny})
	return adj_points


def bfs(maze, point_start, point_end):

	# set the arguments
	mx,my=get_map_size(maze)
	
	queue=[]
	visit=[[False for _ in range(my)] for _ in range(mx)]
	trace=[[{'x':-1, 'y':-1} for _ in range(my)] for _ in range(mx)]
	time=-1

	# bfs search
	visit[point_start['x']][point_start['y']]=True
	queue.append(point_start)
	while queue:
		pt = queue.pop(0)
		time += 1

		if pt == point_end:
			break
		
		adj_points=get_adj_points(maze, pt)
		for pt_next in adj_points:
			if visit[pt_next['x']][pt_next['y']]:
				continue
			visit[pt_next['x']][pt_next['y']]=True
			queue.append(pt_next)
			trace[pt_next['x']][pt_next['y']]=pt
	
	
	# trace the path
	path=[]
	while point_end != point_start:
		point_end = trace[point_end['x']][point_end['y']]
		path.append(point_end)
	path.reverse()
	
	return path, time

def dfs_wrong(maze, point_start, point_end, length):

	# set the arguments
	mx,my=get_map_size(maze)
	
	stack=[]
	visit=[[False for _ in range(my)] for _ in range(mx)]
	trace=[[{'x':-1, 'y':-1} for _ in range(my)] for _ in range(mx)]
	time=-1
	isSuccess=False

	# dfs search
	visit[point_start['x']][point_start['y']]=True
	stack.append({'pt':point_start, 'level':0})
	while stack:
		now = stack.pop()
		pt=now['pt']
		level=now['level']

		time += 1

		if pt == point_end:
			isSuccess=True
			break
		if level == length:
			continue

		adj_points=get_adj_points(maze, pt)
		for pt_next in adj_points:
			if visit[pt_next['x']][pt_next['y']]:
				continue
			visit[pt_next['x']][pt_next['y']]=True
			stack.append({'pt':pt_next, 'level':level+1})
			trace[pt_next['x']][pt_next['y']]=pt
	
	# searcg failed
	if not isSuccess:
		return isSuccess, [], time

	# trace the path
	path=[]
	while point_end != point_start:
		point_end = trace[point_end['x']][point_end['y']]
		path.append(point_end)
	path.reverse()
	
	return isSuccess, path, time

def dfs(maze, point_start, point_end, length):
	# set the arguments
	mx,my=get_map_size(maze)
	
	stack=[]
	visit=[[-1 for _ in range(my)] for _ in range(mx)]
	trace=[[{'x':-1, 'y':-1} for _ in range(my)] for _ in range(mx)]
	time=-1
	isSuccess=False

	# dfs search
	visit[point_start['x']][point_start['y']]=0
	stack.append({'pt':point_start, 'level':0})
	while stack:
		now = stack.pop()
		pt=now['pt']
		level=now['level']
		time += 1
		
		if pt == point_end:
			isSuccess=True
			break
		if level == length or level > visit[pt['x']][pt['y']]:
			continue

		adj_points=get_adj_points(maze, pt)
		for pt_next in adj_points:
			if visit[pt_next['x']][pt_next['y']] != -1 and visit[pt_next['x']][pt_next['y']] <= level+1:
				continue
			visit[pt_next['x']][pt_next['y']]=level+1
			stack.append({'pt':pt_next, 'level':level+1})
			trace[pt_next['x']][pt_next['y']]=pt
	
	# searcg failed
	if not isSuccess:
		return isSuccess, [], time

	# trace the path
	path=[]
	while point_end != point_start:
		point_end = trace[point_end['x']][point_end['y']]
		path.append(point_end)
	path.reverse()
	
	return isSuccess, path, time

def ids_wrong(maze, point_start, point_end):
	isSuccess=False	
	length=1
	total_time=0
	while not isSuccess:
		isSuccess, path, time = dfs_wrong(maze, point_start, point_end, length)
		total_time+=time
		length+=1
	return path, total_time

def ids(maze, point_start, point_end):
	isSuccess=False	
	length=1
	total_time=0
	while not isSuccess:
		isSuccess, path, time = dfs(maze, point_start, point_end, length)
		total_time+=time
		length+=1
	return path, total_time


def get_dist(point1, point2):
	return abs(point1['x']-point2['x'])+abs(point1['y']-point2['y'])

def greedy_bfs(maze, point_start, point_end):
	
	# set the arguments
	mx,my=get_map_size(maze)
	
	#pq=PriorityQueue()
	pq=[]
	pq.append([-1, {-1, -1}])

	visit=[[False for _ in range(my)] for _ in range(mx)]
	trace=[[{'x':-1, 'y':-1} for _ in range(my)] for _ in range(mx)]
	time=-1

	# greedy-bfs search
	visit[point_start['x']][point_start['y']]=True

	#pq.put((get_dist(point_start, point_end), point_start))
	push_pq(pq, [get_dist(point_start, point_end), point_start])
	while len(pq)>1:
		#pt = pq.get()[1]
		pt = pop_pq(pq)[1]

		time += 1
		if pt == point_end:
			break
		
		adj_points=get_adj_points(maze, pt)
		for pt_next in adj_points:
			if visit[pt_next['x']][pt_next['y']]:
				continue
			visit[pt_next['x']][pt_next['y']]=True
			#pq.put((get_dist(pt_next, point_end), pt_next))
			push_pq(pq, [get_dist(pt_next, point_end), pt_next])
			trace[pt_next['x']][pt_next['y']]=pt
	
	
	# trace the path
	path=[]
	while point_end != point_start:
		point_end = trace[point_end['x']][point_end['y']]
		path.append(point_end)
	path.reverse()
	
	return path, time

def a_star(maze, point_start, point_end):
	# set the arguments
	mx,my=get_map_size(maze)
	
	#pq=PriorityQueue()
	pq=[]
	pq.append([-1, {-1, -1}])
	visit=[[False for _ in range(my)] for _ in range(mx)]
	cost=[[my*mx+10 for _ in range(my)] for _ in range(mx)]
	trace=[[{'x':-1, 'y':-1} for _ in range(my)] for _ in range(mx)]
	time=-1

	# a-star search
	cost[point_start['x']][point_start['y']]=0

	# pq.put((
	# 	0 + get_dist(point_start, point_end), 
	# 	point_start))
	push_pq(pq, [0+get_dist(point_start, point_end), point_start])

	while len(pq)>1:
		#now = pq.get()
		now = pop_pq(pq)
		pt = now[1]
		now_cost = now[0]-get_dist(pt, point_end)

		if visit[pt['x']][pt['y']]:
			continue
		visit[pt['x']][pt['y']]=True

		time += 1
		if pt == point_end:
			break

		adj_points=get_adj_points(maze, pt)
		for pt_next in adj_points:
			if visit[pt_next['x']][pt_next['y']] or cost[pt_next['x']][pt_next['y']] <= now_cost+1:
				continue
			#pq.put((cost+1+get_dist(pt_next, point_end), pt_next))
			push_pq(pq, [now_cost+1+get_dist(pt_next, point_end), pt_next])
			trace[pt_next['x']][pt_next['y']]=pt
			cost[pt_next['x']][pt_next['y']] = now_cost+1
	
	# trace the path
	path=[]
	while point_end != point_start:
		point_end = trace[point_end['x']][point_end['y']]
		path.append(point_end)
	path.reverse()
	
	return path, time
	

def main():
	
	'''
	# with argparse library
	parser = argparse.ArgumentParser()
	parser = argparse.ArgumentParser(description='manage test arguments')
 
	parser.add_argument('--tc_num', type=int, default=1)
	parser.add_argument('--tc_path', type=str, default='./')
	parser.add_argument('--tc_name', type=str, default='Maze')
	args=parser.parse_args()

	tc_num = args.tc_num
	tc_path = args.tc_path
	tc_name = args.tc_name
	'''
	
	# without argparse library
	tc_num=5
	tc_path='./'
	tc_name='Maze'
	

	search_algorithms = [
		{'func':bfs, 'name':'BFS'},
		{'func':ids_wrong, 'name':'IDS_WRONG'},
		{'func':ids, 'name':'IDS'},
		{'func':greedy_bfs, 'name':'GBFS'},
		{'func':a_star, 'name':'A_star'}]

	for test_idx in range(1,tc_num+1):
		# load the input data
		file_name = tc_path+tc_name+'_'+str(test_idx)+'.txt'
		num, maze = load_map(file_name)
		point_start, point_end, point_key = extract_elements_from_map(maze)

		for algorithm in search_algorithms:
			# get each algorithm functions
			func_name = algorithm['name']
			func = algorithm['func']
			
			# solve
			print('tc #'+str(test_idx)+' with algorithm, '+func_name)
			path1, time1 = func(maze,point_start,point_key)
			path2, time2 = func(maze,point_key,point_end)
			path = path1 + path2
			time = time1 + time2
			print('path: ' + str(len(path)) + ', time: ' + str(time))
			path.pop(0) # remove duplicated point
			save_result(test_idx, func_name, maze, path, time)
		print('\n')
	
if __name__=='__main__':
	main()



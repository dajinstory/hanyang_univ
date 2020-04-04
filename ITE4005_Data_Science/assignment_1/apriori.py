#!/usr/bin/env python
# coding: utf-8

# ##### Load Libraries

# In[1]:


import numpy as np
import json
import sys

# ##### Apriori Class

# In[2]:


class Apriori:
    def __init__(self, min_support):
        self.MIN_SUPPORT=min_support/100

    # Load Transactions from input file
    def load_transactions(self, filename='input.txt'):
        transactions=[]
        with open(filename, "r") as file:
            for line in file:
                items = line[:-1].split('\t')
                transactions.append(list(map(int, items)))
        return transactions
 
    # Save Rules to output file
    def save_rules(self, rules, filename='output.txt'):
        with open(filename, "w") as file:
            for rule in rules:
                file.write(
                    str(rule['items_1'])+'\t'+
                    str(rule['items_2'])+'\t'+
                    str('%.2f' % round(rule['support']*100,2)) + '\t' +
                    str('%.2f' % round(rule['confidence']*100,2)) + '\n'
                )

    # Make set list (size of set=1)
    def init_sets(self, transactions):
        # find all items
        items={}
        for idx, transaction in enumerate(transactions):
            for item in transaction:
                if item in items:
                    items[item]['nums']+=1
                    items[item]['idxs'].append(idx)
                else:
                    items[item]={'nums':1,'idxs':[idx]}

        # make set list      
        sets=[]
        for item in items.keys():
            sets.append({'items':{item}, 'nums':items[item]['nums'], 'idxs':items[item]['idxs']})
        return sets
    
    def run(self, transactions):

        # get sets
        sets = self.init_sets(transactions)
        rules = []    

        start_point=0
        end_point=len(sets)
        while(start_point < end_point):

            # get idx_A and set_A
            for idx_A, set_A in enumerate(sets[start_point:], start_point):
                # get idx_B and set_B
                for idx_B, set_B in enumerate(sets[:idx_A]):

                    # check if set_A and set_B is mutually exclusive.
                    if set_A['items']&set_B['items'] == set({}):

                        # make new set
                        new_set=set_A['items']|set_B['items']
                        nums=0
                        idxs=[]

                        # find current set from transactions
                        for idx in set_A['idxs']:
                            if set(transactions[idx])&set_B['items']==set_B['items']:
                                nums+=1
                                idxs.append(idx)

                        # MIN_SUPPORT
                        if nums>=int(len(transactions)* self.MIN_SUPPORT):
                            sets.append({'items':new_set, 'nums':nums, 'idxs':idxs})
                            rules.append({
                                'items_1':set_A['items'], 
                                'items_2':set_B['items'],
                                'support':set_A['nums']/len(transactions),
                                'confidence':nums/set_A['nums'],
                                'lift':(nums*len(transactions))/(set_A['nums']*set_B['nums'])
                            })
                            rules.append({
                                'items_1':set_B['items'], 
                                'items_2':set_A['items'],
                                'support':set_B['nums']/len(transactions),
                                'confidence':nums/set_B['nums'],
                                'lift':(nums*len(transactions))/(set_B['nums']*set_A['nums'])
                            })
            start_point=end_point
            end_point=len(sets)

        return rules


# In[3]:

if __name__ == '__main__':
    MIN_SUPPORT = int(sys.argv[1])
    input_file = sys.argv[2]
    output_file = sys.argv[3]
    apriori = Apriori(MIN_SUPPORT)

    transactions = apriori.load_transactions(input_file)
    rules = apriori.run(transactions)
    apriori.save_rules(rules)
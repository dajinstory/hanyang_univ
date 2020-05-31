import sys
import pandas as pd
import math


def load_dataset(filename_train, filename_test, filename_answer):
    # load raw files
    train_dataset_df = pd.read_csv(filename_train, delimiter='\t')
    test_dataset_df = pd.read_csv(filename_test, delimiter='\t')
    answer_dataset_df = pd.read_csv(filename_answer, delimiter='\t')
    key_label = train_dataset_df.keys()[-1]
    
    return train_dataset_df, test_dataset_df, answer_dataset_df, key_label


class DecisionTree:
    def __init__(self, dataset_df, key_label, measure='entrophy', alpha=0.05):
        self.dataset_df = dataset_df
        self.key_label = key_label
        self.measure = measure
        self.get_impurity = self.get_entrophy if measure is 'entrophy' else self.get_GI
        self.calc_leaf_impurity = self.calc_leaf_entrophy if measure is 'entrophy' else self.calc_leaf_GI
        self.calc_info_gain = self.calc_info_gain_entrophy if measure is 'entrophy' else self.calc_info_gain_GI
        
        self.N = len(dataset_df[key_label])
        self.impurity = self.calc_leaf_impurity(dataset_df[key_label])
        
        # attribute(=key) and functions to predict test sample
        self.branch_condition = False
        
        self.alpha = alpha
        self.childs = [False, False]
        self.terminal_nodes=1
        self.is_parent = False
        
        

    # num of data in current subtree
    def get_N(self):
        return self.N
    
    # get the impurity - entrophy
    def get_entrophy(self):
        if self.is_parent:
            E_l = self.childs[0].get_entrophy()
            E_r = self.childs[1].get_entrophy()
            N_l = self.childs[0].get_N()
            N_r = self.childs[1].get_N()
            entrophy = (E_l*N_l + E_r*N_r) / self.N    
            self.impurity=entrophy
        return self.impurity
    # get the impuritiy - Gini Index
    def get_GI(self):
        if self.is_parent:
            GI_l = self.childs[0].get_GI()
            GI_r = self.childs[1].get_GI()
            N_l = self.childs[0].get_N()
            N_r = self.childs[1].get_N()
            GI = ( (GI_l - 1)*N_l + (GI_r - 1)*N_r ) / self.N + 1
            self.impurity = GI
        return self.impurity
    
    
    
    # get the impurity - entrophy - leaf
    def calc_leaf_entrophy(self, labels):
        label_counts = labels.value_counts()
        entrophy = 0.0
        for key in label_counts.keys():
            p = label_counts[key]/len(labels)
            entrophy -= p*math.log(p,2)
        return entrophy
    # get the impurity - Gini Index - leaf
    def calc_leaf_GI(self, labels):
        label_counts = labels.value_counts()
        GI = 1.0
        for key in label_counts.keys():
            p = label_counts[key]/len(labels)
            GI -= p*p
        return GI
    
    
    
    # get info gain
    def calc_info_gain_entrophy(self, labels, midpoint):
        E_l = self.calc_leaf_entrophy(labels[:midpoint])
        E_r = self.calc_leaf_entrophy(labels[midpoint:])
        N_l = midpoint
        N_r = self.N-midpoint
        E = (E_l*N_l + E_r*N_r) / self.N
        return E
    # get info gain
    def calc_info_gain_GI(self, labels, midpoint):
        GI_l = self.calc_leaf_GI(labels[:midpoint])
        GI_r = self.calc_leaf_GI(labels[midpoint:])
        N_l = midpoint
        N_r = self.N-midpoint
        GI = ( (GI_l - 1)*N_l + (GI_r - 1)*N_r ) / self.N + 1
        return GI
  


    def partitioning(self):
        for key in self.dataset_df.keys():
            if key is key_label:
                continue
            dataset_df_sorted = self.dataset_df.sort_values(by=[key], axis=0)
            value_counts = dataset_df_sorted[key].value_counts().sort_index()
            
            midpoint=0
            for value in value_counts.keys()[:-1]:
                midpoint += value_counts[value]
                impurity = self.calc_info_gain(dataset_df_sorted[key_label], midpoint)
                if impurity<self.impurity :                    
                    self.impurity = impurity
                    dataset_sorted = dataset_df_sorted.to_dict('records')
                    dataset_df_l = pd.DataFrame(dataset_sorted[:midpoint])
                    dataset_df_r = pd.DataFrame(dataset_sorted[midpoint:])
                    
                    self.is_parent=True
                    self.childs[0] = DecisionTree(dataset_df_l, key_label, self.measure, self.alpha)
                    self.childs[1] = DecisionTree(dataset_df_r, key_label, self.measure, self.alpha)
                    
                    # set conditions
                    self.branch_condition = {'key':key, 'value':value}

        if self.is_parent:
            self.childs[0].partitioning()
            self.childs[1].partitioning()
            self.terminal_nodes = self.childs[0].terminal_nodes + self.childs[1].terminal_nodes
    
    def pruning(self, N_total):
        if not self.is_parent:
            return
        
        # pruning child nodes first
        self.childs[0].pruning(N_total)
        self.childs[1].pruning(N_total)
        # renew info 
        self.terminal_nodes = self.childs[0].terminal_nodes + self.childs[1].terminal_nodes
        self.impurity = self.get_impurity()
        
        labels = self.dataset_df[self.key_label]
        leaf_impurity = self.calc_leaf_impurity(labels)
        if leaf_impurity*self.N/N_total + self.alpha*1 < self.impurity*self.N/N_total + self.alpha*self.terminal_nodes:
        #if (leaf_impurity-1)*self.N/N_total + 1 + self.alpha*1 < (self.impurity-1)*self.N/N_total + 1 + self.alpha*self.terminal_nodes:
            self.impurity = leaf_impurity
            self.childs[0]=False
            self.childs[1]=False
            self.is_parent=False
            self.terminal_nodes = 1
            
    def predict(self, data):
        if self.is_parent:
            branch_key = self.branch_condition['key']
            branch_value = self.branch_condition['value']
            child_num = 0 if data[branch_key]<=branch_value else 1
            return self.childs[child_num].predict(data)
        labels = self.dataset_df[self.key_label]
        return labels.value_counts().idxmax()


# In[3]:

if __name__ == '__main__':
    filename_train = sys.argv[1]
    filename_test = sys.argv[2]
    filename_result = sys.argv[3]
    filename_answer = filename_test
    if len(sys.argv)>4:
        filename_answer = sys.argv[4]

    # load dataset
    print('\n# Load dataset')
    train_dataset_df, test_dataset_df, answer_dataset_df, key_label = load_dataset(filename_train, filename_test, filename_answer)
    print('len of dataset \t: %d' % len(train_dataset_df))
    print('len of dataset \t: %d' % len(test_dataset_df))
    
    # make decision_tree object
    print("\n# Make decision tree object")
    decision_tree = DecisionTree(train_dataset_df, key_label, measure='entrophy', alpha=0.01)
    print('decision tree object \t\t: ', decision_tree)
    
    # partitioning
    print("\n# partitioning")
    decision_tree.partitioning()

#     # pruning
#     print("\n\n# pruning")
#     decision_tree.pruning(len(train_dataset_df))
    
    # predict with decision tree
    print("\n# predict with decision tree")
    test_dataset = test_dataset_df.to_dict('records')
    preds = [decision_tree.predict(data) for data in test_dataset]
    test_dataset_df[key_label] = preds
    
    # save the result
    print('\n# Save results')
    print(test_dataset_df)
    test_dataset_df.to_csv(filename_result, '\t')

    # get the score
    if len(sys.argv)>4:
        results = preds == answer_dataset_df[key_label]
        print('Accuracy : {} / {}'.format(results.value_counts()[True], len(results)))

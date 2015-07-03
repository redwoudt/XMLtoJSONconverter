class Solution {
public:
    vector<vector<int> > combinationSum(vector<int> &candidates, int target) {
        vector<vector<int> > * combos = combinationSum(candidates, 0,  target);
        return *combos;
    };
    
    vector<vector<int> > * combinationSum(vector<int> &candidates, int index, int target) {
        int sum = 0;
        vector<vector<int>> * combinations = new vector<vector<int>>();
        //get linear sums for full sets of canditates
        vector<int> * combo1 = new vector<int>();
        for (int i = index; i < candidates.size(); i++){
            sum = candidates[i];
            if (combo1 == nullptr){
                combo1 = new vector<int>();
            }
            //insert in order
            insertInOrder(*combo1, candidates[i]);
            
            //combo1->push_back(candidates[i]);
            if (sum == target){
                combinations->push_back(*combo1);
                delete combo1;
                combo1 = nullptr;
            }
            else {
                for (int j = i+1; j < candidates.size(); j++){
                    sum += candidates[j];
                    if (sum == target){
                        if (combo1 == nullptr){
                            combo1 = new vector<int>();
                        }
                        //insert in order
                        insertInOrder(*combo1, candidates[j]);
                        //combo1->push_back(candidates[j]);
                        combinations->push_back(*combo1);
                        delete combo1;
                        combo1 = nullptr;
                    }
                    else if (sum < target){
                        if (combo1 == nullptr){
                            combo1 = new vector<int>();
                        }
                        //insert in order
                        insertInOrder(*combo1, candidates[j]);
                        //combo1->push_back(candidates[j]);
                    }
                    else { //sum > target
                        delete combo1;
                        combo1 = nullptr;
                        break;
                    }
                }
            }
        }
        
        //then use recursion to get duplicate options for each entry
        for (int i = index; i < candidates.size(); i++){
            int newTarget = target - candidates[i];
            if (target > 0){
                vector<vector<int>> * repeatCombos = combinationSum(candidates, i, newTarget);
                for (int k = 0; k < repeatCombos->size(); k++){
                    //insert in order
                    insertInOrder((*repeatCombos)[k], candidates[i]);
                    //(*repeatCombos)[k].insert((*repeatCombos)[k].begin(),candidates[i]);
                    if (notPresent((*repeatCombos)[k], combinations )){
                        combinations->push_back((*repeatCombos)[k]);
                    }
                }
                delete repeatCombos;
            }
            
        }
        
        return combinations;
    };
    
    void insertInOrder(vector<int> & combo, int value){
        if (combo.size()==0){
            combo.push_back(value);
        }
        else{
            for (int i = 0; i < combo.size(); i++){
                if (combo[i] >= value){
                    combo.insert(combo.begin() + i, value);
                    return;
                }
            }
            
            combo.push_back(value);
        }
    };
    
    bool notPresent(vector<int> combo, vector<vector<int> > * combos){
        for (const auto & c : *combos){
            if (c.size() == combo.size()){
                int i;
                for (i = 0; i < c.size(); i++){
                    if (c[i] != combo[i]){
                        break;
                    }
                }
                if (i == c.size()){
                    return false;
                }
            }
        }
        return true;
        
    }
    
    
    
};
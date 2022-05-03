#include <iostream>
#include "OrderStatisticTree.cpp"
using namespace std;

int main() {
    OrderStatisticTree os_tree;

    while(true) {
        cout<<"Enter the operation that you want to perform: "<<endl;
        cout<<"1) Insert a key"<<endl;
        cout<<"2) Delete a key"<<endl;
        cout<<"3) Find the rank of a key"<<endl;
        cout<<"4) Find the k'th smallest key"<<endl;
        cout<<"5) Quit"<<endl;

        int op;
        cin>>op;

        cout<<endl;

        if(op == 1) {
            int key;
            cout<<"Enter the key that you want to insert: ";
            cin>>key;

            os_tree.Insert(key);

            cout<<"Key inserted successfully!"<<endl;
        }
        else if(op == 2) {
            int key;
            cout<<"Enter the key that you want to delete: ";
            cin>>key;

            os_tree.Delete(key);

            cout<<"Key deleted successfully!"<<endl;
        }
        else if(op == 3) {
            int key;
            cout<<"Enter the key whose rank you want to find: ";
            cin>>key;

            int rank = os_tree.RankOfKey(key);

            if(rank == -1) {
                cout<<"Key not found in the tree!"<<endl;
            }
            else {
                cout<<"The rank of "<<key<<" is "<<rank<<endl;
            }
        }
        else if(op == 4) {
            int rank;
            cout<<"Enter the rank: ";
            cin>>rank;

            Node* node = os_tree.FindByRank(rank);

            if(node == nullptr) {
                cout<<"No node with the given rank exists in the tree!"<<endl;
            }
            else {
                cout<<"The key with rank "<<rank<<" is "<<node->key<<endl;
            }
        }
        else if(op == 5) {
            break;
        }
        else {
            cout<<"Invalid Operation!"<<endl;
        }

        cout<<"\nCurrent status of the tree: "<<endl;
        os_tree.Preorder();

        cout<<endl<<endl;
    }
}
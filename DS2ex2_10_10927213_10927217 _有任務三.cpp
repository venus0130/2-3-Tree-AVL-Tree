// 10927213 陳可中 10927217 張維娜
// 在最新版本的Devc++中可成功執行

#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <cstring>

using namespace std;

int filenum = 0;

struct data   // AVL樹用
{
    int schoolCode;    // 學校代碼
    string schoolName; // 學校名稱
    int departCode;    // 科系代碼
    string departName; // 科系名稱
    string day;		   // 日間部
    string advanced;  // 進修別
    string level;  	   // 等級別
    int studentNum;   // 學生人數
    int number;        // 編號
};

struct DataTree   // AVL樹用
{
    int schoolCode;    // 學校代碼
    string schoolName; // 學校名稱
    string departName; // 科系名稱
    struct DataTree * left;    // 左子樹
    struct DataTree * right;   // 柚子樹
    int number;				   // 編號
    vector<int> node;          // 同科系存入同一節點
};

struct Data // 2-3樹用
{
    int row_num ; // 序號
    string snumber ;  // 學校代碼
    string sname ;  //學校名稱-此作業重點
    string dnumber ;  //科系代碼
    string dname ;  //科系名稱
    string day ;  // 日進 / 進修別
    string level ;  //等級別
    int student_num ;  //學生數
    int teacher_num ;  //教師數
    int graduates ;  //上學年度畢業生數
    string city ;  //縣市名稱
    string system ;  //體系別

};
int t1 = 0;
class ttTree
{
public :
    ttTree *leftnode = NULL;
    ttTree *rightnode = NULL;
    ttTree *midnode = NULL;
    ttTree *parent = NULL;
    ttTree *root = NULL;
    vector<Data> key1 ;
    vector<Data> key2 ;
    int keynum ;

    void clear() {
        key1.clear();
        key2.clear();
    }


    void ttTree_insert( Data data )
    {
        ttTree * newnode ;
        ttTree * cur ;
        bool quit = false;
        bool haveput = false;

        if ( root == NULL )
        {
            newnode = new ttTree  ;
            newnode -> key1.push_back( data ) ;
            newnode -> keynum = 1 ;
            root = newnode ;
        }
        cur = root ;


        while( !quit )
        {

            if ( cur -> key1[0].sname == data.sname )    // 與key1同校名
            {
                cur -> key1.push_back( data ) ;
                quit = true;
                haveput = true;

            }
            else if ( cur -> key2.empty() == false && cur -> key2[0].sname == data.sname )   // 與key2同校名
            {
                cur -> key2.push_back( data ) ;
                quit = true;
                haveput = true;
            }
            else if ( !quit && cur -> leftnode == NULL && cur -> rightnode == NULL && cur -> midnode == NULL ) // 找到最底了
                quit = true;
            else if ( cur -> key1[0].sname > data.sname ) //比key1小
                cur =  cur -> leftnode ;
            else if ( cur -> key2.empty() )    // 只有一個key，且比key大
            {
                if ( data.sname > cur -> key1[0].sname )
                    cur =  cur -> rightnode ;
            }
            else if ( cur -> key2.empty() == false )   // 兩個key
            {
                if (  data.sname > cur -> key2[0].sname ) // 大於key2
                    cur =  cur -> rightnode ;
                else if ( cur -> key2[0].sname > data.sname  &&  data.sname > cur -> key1[0].sname ) // 介於key1&key2之間
                    cur = cur -> midnode ;
            }
            else ;

        } // while


        if ( !haveput )
        {
            if ( cur -> keynum == 1 ) // 只有一個key // key1 always < key2
            {
                if (  data.sname < cur -> key1[0].sname  )
                {
                    vector<Data> temp ;
                    temp = cur -> key1 ;
                    cur -> key2 = temp ;
                    cur -> key1.clear() ;
                    cur -> key1.push_back( data );
                    cur -> keynum = 2 ;
                }
                else
                {
                    cur -> key2.push_back( data ) ;
                    cur -> keynum = 2 ;
                }
            }
            else if ( cur -> keynum == 2 ) // 已經有兩個key了
            {
                vector<Data> t ; // 第三個key
                t.push_back( data ) ;
                ttTree_split( cur, t, NULL, NULL, NULL, NULL ) ;
                t.clear() ;
            }
        }

    }

    void ttTree_split( ttTree * cur, vector<Data> newkey, ttTree * treeA, ttTree * treeB, ttTree * treeC, ttTree * treeD )
    {
        vector<Data> big;
        vector<Data> small;
        vector<Data> mid;
        if ( cur -> key2[0].sname > cur -> key1[0].sname && cur -> key1[0].sname > newkey[0].sname ) // newkey < key1 < key2
        {
            big = cur -> key2;
            mid = cur -> key1;
            small = newkey;
        }
        else if ( cur -> key2[0].sname > newkey[0].sname && cur -> key1[0].sname < newkey[0].sname ) // key1 < newkey < key2
        {
            big = cur -> key2;
            mid = newkey;
            small = cur -> key1;
        }
        else if ( cur -> key2[0].sname < newkey[0].sname && cur -> key1[0].sname < cur -> key2[0].sname ) // key1 < key2 < newkey
        {
            big = newkey;
            mid = cur -> key2;
            small = cur -> key1;
        }
        else ;

        ttTree * p;
        if ( cur == root )
            p = new ttTree;
        else
            p = cur -> parent;


        ttTree * n1 = new ttTree  ;  // n1存smallest key
        n1-> key1 = small ;
        n1-> parent = p ;
        n1-> keynum = 1 ;

        ttTree * n2 = new ttTree  ;   // n2存biggest key
        n2-> key1 = big ;
        n2-> parent = p ;
        n2-> keynum = 1 ;

        if ( cur -> leftnode != NULL || cur -> midnode != NULL || cur -> rightnode != NULL )   // 當已進入遞迴
        {
            n1 -> leftnode = treeA ;
            n1 -> rightnode = treeB ;
            treeA -> parent = n1 ;
            treeB -> parent = n1 ;
            n2 -> leftnode = treeC ;
            n2 -> rightnode = treeD ;
            treeC -> parent = n2 ;
            treeD -> parent = n2 ;
        }


        if ( p -> keynum == 1 ) // cur的parent只有一個key
        {
            if (  p -> key1[0].sname > mid[0].sname  )
            {
                p -> key2 = p -> key1 ;
                p -> key1 = mid ;
                p -> leftnode = n1 ;
                p -> midnode = n2 ;
                p -> keynum = 2 ;
            }
            else
            {
                p -> key2 = mid  ;
                p -> midnode = n1 ;
                p -> rightnode= n2 ;
                p -> keynum = 2 ;
            }
        }
        else if ( p -> keynum == 2 ) // cur的parent已有兩個key
        {
            if ( p -> leftnode == cur )
                ttTree_split( p, mid, n1, n2, p -> midnode, p -> rightnode ) ;
            else if ( p -> midnode == cur )
                ttTree_split( p, mid, p -> leftnode, n1, n2, p -> rightnode);
            else if ( p -> rightnode == cur )
                ttTree_split( p, mid, p -> leftnode, p -> midnode, n1, n2 ) ;
            else ;
        }
        else // cur為root
        {
            p -> key1 = mid;
            p -> leftnode = n1;
            p -> rightnode = n2;
            p -> keynum = 1;
            root = p;
        }


    }

    void ttTree_print()
    {
        vector<Data> rootlist ;
        if ( root -> key2.empty() == false ) // 兩個key
        {
            if ( root -> key1[0].row_num < root -> key2[0].row_num )
            {
                rootlist = root -> key1 ;
                for( int i = 0 ; i < root -> key2.size() ; i++)
                    rootlist.push_back( root -> key2[i]) ;
            }
            else
            {
                rootlist = root -> key2 ;
                for( int i = 0 ; i < root -> key1.size() ; i++)
                    rootlist.push_back( root -> key1[i]) ;
            }
        }
        else
            rootlist = root -> key1 ;


        cout << "Tree height = " << ttTree_treetall( root ) << endl;

        for ( int n = 0 ; n < rootlist.size() ; n++ )
        {
            cout << n + 1 << ": " << "[" << rootlist[n].row_num << "]" << " " ;
            cout << rootlist[n].sname << ", "  ;
            cout << rootlist[n].dname << ", "  ;
            cout << rootlist[n].day << ", "  ;
            cout << rootlist[n].level << ", "  ;
            cout << rootlist[n].student_num << endl  ;
        }

    }

    int ttTree_treetall ( ttTree * t )    // 樹高
    {

        if ( t == NULL )
            return 0 ;

        int left = ttTree_treetall( t -> leftnode ) ;
        int right = ttTree_treetall( t -> rightnode ) ;

        if ( left > right )
            return left+1 ;
        else
            return right+1 ;
    }

    vector<Data> ttTree_m3list ( string name ) {
        vector<Data> m3list;
        ttTree * cur = root;
        bool quit = false;
        while( !quit )
        {

            if ( cur -> key1[0].sname == name )    // 與key1同校名
            {
                m3list = cur -> key1;
                quit = true;
            }
            else if ( cur -> key2.empty() == false && cur -> key2[0].sname == name )   // 與key2同校名
            {
                m3list = cur -> key2 ;
                quit = true;
            }
            else if ( !quit && cur -> leftnode == NULL && cur -> rightnode == NULL && cur -> midnode == NULL ) // 找到最底了
                quit = true;
            else if ( cur -> key1[0].sname > name ) //比key1小
                cur =  cur -> leftnode ;
            else if ( cur -> key2.empty() )    // 只有一個key，且比key大
            {
                if ( name > cur -> key1[0].sname )
                    cur =  cur -> rightnode ;
            }
            else if ( cur -> key2.empty() == false )   // 兩個key
            {
                if (  name > cur -> key2[0].sname ) // 大於key2
                    cur =  cur -> rightnode ;
                else if ( cur -> key2[0].sname > name  &&  name > cur -> key1[0].sname ) // 介於key1&key2之間
                    cur = cur -> midnode ;
            }
            else ;

        } // while

        return m3list;


    }

    void run( ttTree * root ) {
        if ( root ) {
            t1 += sizeof( root ) ;
            run( root -> leftnode ) ;
            run( root -> midnode );
            run( root -> rightnode ) ;

        }
    }

};

int t2 = 0 ;
class AVL {
public:
    vector<data> schoolList; // 存所有名單
    vector<data> mission3Tree;
    string fileName;
    DataTree * DepartNameTree = NULL;
    DataTree * temp = NULL;
    DataTree * m3Tree = NULL;



    AVL() {
        DepartNameTree = NULL ;
    }
    void clear() {
        schoolList.clear();
    }

    bool loadData() {
        struct data test;
        fstream fin;
        string garbage;   // 記事本的第一二三行
        string x;         // 不需要的資料
        test.number = 0;  // 編號
        int i = 0;

        int t = filenum;
        fileName = "input" + to_string(t) + ".txt" ;
        fin.open( fileName.c_str(), ios::in ) ;

        if ( ! fin )   // 開啟檔案失敗或是沒有檔案
        {
            cout << endl << "### input" << fileName << ".txt does not exist! ###" << endl << endl;
            return false;
        }
        else
        {
            getline(fin, garbage);   // 讀掉垃圾
            getline(fin, garbage);   // 讀掉垃圾
            getline(fin, garbage);   // 讀掉垃圾

            while (fin >> test.schoolCode)
            {
                getline(fin, x, '\t');   				// 讀掉垃圾
                getline(fin, test.schoolName, '\t');
                getline(fin, x, '\t');   			   // 讀掉垃圾
                getline(fin, test.departName, '\t');
                getline(fin, test.day, '\t');
                getline(fin, test.level, '\t');
                int Num = 0;
                if (fin >> x && x.at(0) == '\"') {    // 當資料碰到協槓的處理方式
                    for ( int j = 1 ; j < x.size() ; j++ ) {
                        if (x[j] >= '0' && x[j] <= '9') Num = (x[j]-'0')+ (Num*10);
                    }
                }
                else {
                    for ( int j = 0 ; j < x.size() ; j++ ) {
                        if (x[j] >= '0' && x[j] <= '9') Num = (x[j]-'0')+ (Num*10);
                    }
                }

                test.studentNum = Num;
                getline(fin, garbage);             // 剩餘不需要的資料
                test.number++;                     // 編號

                schoolList.push_back(test);
            }
            fin.close();
            return true;
        }
    } // bool loadData

    DataTree *delTree(DataTree *root)                 // 當讀新的檔案 當前二元樹不被需要時 遞迴釋放所有空間
    {
        if ( root != NULL )
        {
            root->left = delTree(root->left);
            root->right = delTree(root->right);
            delete root;
            root = NULL;
        }
        return NULL;
    }

    int treeHeight(DataTree *root) {	// 計算樹高
        if (root == NULL) return 0;
        else {
            int left = treeHeight(root->left);
            int right = treeHeight(root->right);
            return 1 + (( left >= right ) ? left : right);   //分別堆疊計算左右子樹高，回傳較大者
        }
    }

    DataTree *singleRotate(DataTree *root, int op) { // LL or RR
        DataTree *x = root;
        if (op == 1) {   // LL的情況
            DataTree *y = x->left;
            x->left = y->right;
            y->right = x;
            root = y;
            return root;
        }
        else if (op == 2) {  // RR的情況
            DataTree *y = x->right;
            x->right = y->left;
            y->left = x;
            root = y;
            return root;
        }
    } // single

    DataTree *doubleRotate(DataTree *root, int op) {
        DataTree *x = root;
        if (op == 1) {  // LR的情況
            x->left = singleRotate(x->left, 2);  // RR
            return singleRotate(root, 1); // LL
        }
        else if (op == 2) {   //RL的情況
            x->right = singleRotate(root->right, 1); // LL
            return singleRotate(root, 2); // RR
        }

    } // double


    DataTree *insertDepartNameTree(DataTree *&root, int i)  {       // 科系名稱為鍵值的二元樹插入
        if ( root == NULL ) {
            root = new DataTree();
            root->schoolName = schoolList.at(i).schoolName;
            root->departName = schoolList.at(i).departName;
            root->node.push_back(schoolList.at(i).number);
            root->left = NULL;
            root->right = NULL;
        }
        else if ( schoolList.at(i).departName.compare(root->departName) > 0 )  {  // 比較字串鍵值的大小，比當前大，插入右邊
            root->right = insertDepartNameTree(root->right, i);
            if (treeHeight(root->left) - treeHeight(root->right) == -2) {        // 要旋轉，且左邊樹高小於右邊樹高(RR or RL)
                if (treeHeight(root->right->left) - treeHeight(root->right->right) == 1) root = doubleRotate(root, 2);  // RL
                else root = singleRotate(root, 2); // RR
            }
        }

        else if (schoolList.at(i).departName.compare(root->departName) == 0)   root->node.push_back(schoolList.at(i).number);
      																		   // 比較字串鍵值的大小，跟當前一樣，插入相同節點
        else {  															   // 比較字串鍵值的大小，比當前大，插入右邊
		    root->left = insertDepartNameTree(root->left, i);

            if (treeHeight(root->left) - treeHeight(root->right) == 2) {	   // 要旋轉，且左邊樹高大於右邊樹高(LL or LR)
                if (treeHeight(root->left->left) - treeHeight(root->left->right) == -1) root = doubleRotate(root, 1);  // LR
                else root = singleRotate(root, 1); // RL
            }
        }
        return root;
    }

    DataTree *buildTree(DataTree *&root)  {   /////////////////////////////////////////建樹
		for ( int i = 0 ; i < schoolList.size() ; i++ )  {
            root = insertDepartNameTree(root, i);
        }
        return root;
    }

    vector<data> searchTree(DataTree * root, string name) {
        vector<data>missionThreeUse;
        while (root !=NULL) {
            if (root->departName < name ) root = root->right;
            else if (root->departName == name) {

                for (int i = 0 ; i < root->node.size() ; i++) {
                    for (int j = 0 ; j < schoolList.size() ; j++) {
                        if (root->node[i] == schoolList.at(j).number) {
                                missionThreeUse.push_back(schoolList[j]);
                                //cout << "fsfsdfwefwer";
                        }
                    }
                }
                root = NULL;

            }
            else root = root->left;
        }

        return missionThreeUse;
    }


    void printData(DataTree *root) {   // 印出動態陣列中的資料
        int n = 0;
        for (int i = 0 ; i < root->node.size() ; i++) {
            for (int j = 0 ; j < schoolList.size() ; j++) {
                if (root->node[i] == schoolList.at(j).number) {
                    n++;
                    cout <<n<<": "<<"["<<j+1<<"]"<<" "<<schoolList.at(j).schoolName<<", "<<schoolList.at(j).departName<<", "
                         <<schoolList.at(j).day<<", "<<schoolList.at(j).level<<", "<<schoolList.at(j).studentNum<<endl;
                }
            }
        }
    }


    void run(DataTree *root ) {
        if ( root ) {
            t2 += sizeof( root ) ;
            run( root -> left ) ;
            run( root -> right ) ;

        }
    }



};


bool input_file( vector<Data> &v, int filenumber )  ;
int panel() ;


int main()
{
    vector<Data> da ;
    vector<Data> m3ttall; // m3 的ttTree 為 '*'
    vector<Data> collegelist ; // m3 的ttTree
    vector<data> missionThreeUse;
    bool quit = false ;        // 如果filenumber為0就quit
    int command =  panel() ;
    bool have_run_m1 = false;  // 確認有沒有跑過任務一
	int missionTwoAgain = 0;   // command = 1->2->2的情況
	bool haveData = false;
    AVL schoolList;
    AVL mission3Tree;



    ttTree m3tt;
    int height = 0;			   // 跑AVL最大樹高用

    while ( command )
    {
        if  ( command == 1 ) // 2-3 Tree
        {
            m3ttall.clear();
        	missionTwoAgain = 0;
            ttTree tt;
            cout << endl << "Input a file number ([0] Quit):" ;
            int filenumber ;
            cin >> filenumber ;
            if ( filenumber == 0 )
                quit = true;
            while( !input_file( da, filenumber ) && !quit )
            {
                cout << endl << "Input a file number ([0] Quit):" ;
                cin >> filenumber ;
                if ( filenumber == 0 )
                    quit = true;
            }
            if ( !quit )
            {
                for ( int i = 0 ; i < da.size() ; i++ ) {
                    tt.ttTree_insert( da[i] );
                    m3ttall.push_back( da[i] ); // m3*
                }
                tt.ttTree_print();
                m3tt  = tt ;
                //delete tt.root;
            }
            quit = false;
            have_run_m1 = true;
            filenum = filenumber;
            t1 = 0;
             tt.run(tt.root);
             cout << t1 << endl;
        }

        else if ( command == 2 ) {	//AVL Tree
            if ( have_run_m1 ) {
               	schoolList.DepartNameTree = schoolList.delTree(schoolList.DepartNameTree);
              	schoolList.clear();
            	missionTwoAgain++;
            	haveData = schoolList.loadData();
                schoolList.DepartNameTree = schoolList.buildTree(schoolList.DepartNameTree);
                height = schoolList.treeHeight(schoolList.DepartNameTree);   // 最大樹高
                if ( missionTwoAgain > 1 ) cout << "### AVL tree has been built. ###" << endl;
                cout << "Tree height = " << height << endl;
                schoolList.printData(schoolList.DepartNameTree);
                cout << endl << endl;
                t2 = 0;
                schoolList.run(schoolList.DepartNameTree);
                cout << t2 << endl;
            }
            else cout << "### Choose 1 first. ###" << endl << endl;
        }
        else if ( command == 3 ) { // intersection query
            if ( have_run_m1 ) {
                string college, department ;
                cout << "Enter a college name to search [*]:" ;
                cin >> college ;
                if ( college == "*" )
                    collegelist = m3ttall ; // 全資料
                else {
                   ttTree temp = m3tt;
                   collegelist = temp.ttTree_m3list( college );
                }

                cout << "Enter a department name to search [*]:" ;
                cin >> department ;

                if ( department != "*" )
                    missionThreeUse = schoolList.searchTree(schoolList.DepartNameTree,department);



                vector<Data> anslist;
                if ( department != "*" ){
                for ( int i = 0 ; i < collegelist.size() ; i ++ ){
                    for ( int j = 0 ; j < missionThreeUse.size() ; j++ ){
                        if ( collegelist[i].row_num == missionThreeUse[j].number )
                            anslist.push_back(collegelist[i]);
                    }
                }
                }
                else{
                    anslist = collegelist;
                }

                for ( int n = 0 ; n < anslist.size() ; n++ )
                {
                    cout << n + 1 << ": " << "[" << anslist[n].row_num << "]" << " " ;
                    cout << anslist[n].sname << ", "  ;
                    cout << anslist[n].dname << ", "  ;
                    cout << anslist[n].day << ", "  ;
                    cout << anslist[n].level << ", "  ;
                    cout << anslist[n].student_num << endl  ;
                }

            }
            else cout << "### Choose 1 first. ###" << endl << endl;
        }

        command =  panel() ;
        da.clear() ;
        collegelist.clear();

    }

}

int panel() // 顯示功能清單
{
    cout << endl << "*** Search Tree Utilities **" << endl;
    cout << "* 0. QUIT                  *" << endl;
    cout << "* 1. Build 2-3 tree        *" << endl;
    cout << "* 2. Build AVL tree        *" << endl;
    cout << "* 3. Intersection Query    *" << endl;
    cout << "*************************************" << endl;
    cout << "Input a choice(0, 1, 2, 3): " ;

    int command ;

    while ( cin >> command  && ( command < 0 || command > 3 ) )
    {
        cout << "this command is not exist" << endl ;
        cout << "Input a choice(0, 1, 2, 3):" ;
    }
    return command ;
}

bool input_file( vector<Data> &v, int filenumber )   //讀檔
{
    fstream file ;
    string filename = "input" + to_string(filenumber) + ".txt" ;
    file.open( filename.c_str(), ios::in ) ;
    Data d ;
    if ( !file)
    {
        if ( filenumber != 0 )
            cout << endl << "### " << filename << " does not exist! ###" << endl ;
        return false ;
    }
    else
    {
        char cstr[255];
        int fNo,pre,pos;

        file.getline(cstr,255,'\n');

        file.getline(cstr,255,'\n');

        file.getline(cstr,255,'\n');

        int row_n = 1 ;

        while (file.getline(cstr,255,'\n'))
        {

            Data oneR;
            string buf, cut;

            fNo = 0;
            pre = 0;
            buf.assign(cstr);
            // f( buf ) ;
            oneR.row_num = row_n ;
            row_n ++ ;

            do
            {
                pos = buf.find_first_of('\t',pre);
                cut = buf.substr(pre,pos - pre);
                switch(++fNo)
                {
                case 2:
                    oneR.sname = cut;
                    break;

                case 4:
                    oneR.dname = cut;
                    break;

                case 5:
                    oneR.day = cut;
                    break;

                case 6:
                    oneR.level = cut;
                    break;

                case 7:
                    oneR.student_num = atoi(cut.c_str());
                    break;

                case 8:
                    oneR.teacher_num = atoi(cut.c_str());
                    break;

                case 9:
                    oneR.graduates = atoi(cut.c_str());
                    break;

                default:
                    break;
                }
                pre = ++pos;
            }
            while((pos>0) && (fNo<10));
            v.push_back(oneR);
        }
        file.close() ;
        return true ;

    }

}

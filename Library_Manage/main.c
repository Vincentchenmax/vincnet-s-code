#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 书的结构体（箱子）
struct Book {
    int id;
    char name[50];
    char author[30];
    int total;          // 一共有几本
    int borrowed;       // 已借几本
    int remain;  
    struct Book* next;
};

// 链表头（手提袋）
struct List {
    struct Book* head;
};

// 造一个空链表（空手提袋）
struct List* initList() {
    // 申请一块内存，做链表头
    struct List* list = (struct List*)malloc(sizeof(struct List));
    
    // 空链表 = 没有书
    list->head = NULL;
    
    // 把造好的袋子返回去
    return list;
}

// 添加书籍
void addBook(struct List* list)
{
    struct Book* newBook = (struct Book*)malloc(sizeof(struct Book));

    // 1. 输入编号
    printf("输入编号：");
    scanf("%d", &newBook->id);

    // ===================== 查重：不允许重复 =====================
    struct Book* p = list->head;
    while (p != NULL) {
        if (p->id == newBook->id) {
            printf("编号已存在，不能重复添加！\n");
            free(newBook);
            return;
        }
        p = p->next;
    }

    // 2. 输入其他信息
    printf("输入书名：");
    scanf("%s", newBook->name);
    printf("输入作者：");
    scanf("%s", newBook->author);
    printf("输入总数量：");
    scanf("%d", &newBook->total);
    printf("输入已借出：");
    scanf("%d", &newBook->borrowed);

    newBook->remain = newBook->total - newBook->borrowed;
    newBook->next = NULL;

    // ===================== 核心：按编号从小到大插入 =====================
    // 如果链表为空 或者 新编号比第一个还小，就插在最前面
    if (list->head == NULL || newBook->id < list->head->id) {
        newBook->next = list->head;
        list->head = newBook;
    }
    else {
        // 找到“比我小的最后一个”，插在它后面
        struct Book* cur = list->head;
        while (cur->next != NULL && cur->next->id < newBook->id) {
            cur = cur->next;
        }
        newBook->next = cur->next;
        cur->next = newBook;
    }

    printf("添加成功！\n");
}

// 显示所有书
void showAllBooks(struct List* list)
{
    // 1. 用 p 指向第一本书
    struct Book* p = list->head;

    // 2. 如果一本书都没有（空链表）
    if (p == NULL) {
        printf("当前没有书籍！\n");
        return;
    }

    // 3. 一边走一边打印，直到没有书为止
    printf("\n===== 文学书籍信息 =====\n");
    while (p != NULL) {
        printf("编号：%d\n", p->id);
        printf("书名：%s\n", p->name);
        printf("作者：%s\n", p->author);
        printf("总数量：%d 本\n", p->total);
        printf("已借出：%d 本\n", p->borrowed);
        printf("剩余：%d 本\n", p->remain);
        printf("------------------------\n");

        // 【关键】p 走到下一本书
        p = p->next;
    }
}

// 根据编号查找书
void findBook(struct List* list)
{
    // 1. 定义一个变量，存要找的编号
    int findId;
    printf("请输入要查找的书籍编号：");
    scanf("%d", &findId);

    // 2. 让指针 p 从头开始找
    struct Book* p = list->head;

    // 3. 循环遍历每一本书
    while (p != NULL) {
        // 4. 判断：当前这本书的编号，是不是我要找的？
        if (p->id == findId) {
            // 找到了！打印信息
            printf("✅ 找到了！书名：%s\n", p->name);
            // 找到了就不用再找了，直接结束函数
            return;
        }
        // 没找到就继续往下走
        p = p->next;
    }

    // 5. 如果循环走完了还没找到，就执行这里
    printf("❌ 没有找到这本书！\n");
}


// 按书名查找书籍
void findBookByName(struct List* list)
{
    char name[50];
    printf("请输入要查找的书名：");
    scanf("%s", name);

    struct Book* p = list->head;
    int found = 0;

    // 遍历所有书
    while (p != NULL)
    {
        // 如果书名一样
        if (strcmp(p->name, name) == 0)
        {
            printf("\n找到了！\n");
            printf("编号：%d\n", p->id);
            printf("书名：%s\n", p->name);
            printf("作者：%s\n", p->author);
            printf("总数量：%d\n", p->total);
            printf("已借出：%d\n", p->borrowed);
            printf("剩余：%d\n", p->remain);
            found = 1;
            break;
        }
        p = p->next;
    }

    if (found == 0)
    {
        printf("没找到这本书！\n");
    }
}


// 修改书籍信息（可用来借还书）
void modifyBook(struct List* list)
{
    int id;
    printf("请输入要修改的书籍编号：");
    scanf("%d", &id);

    struct Book* p = list->head;

    while (p != NULL)
    {
        if (p->id == id)
        {
            printf("找到书籍：%s\n", p->name);
            printf("请输入新的总数量：");
            scanf("%d", &p->total);
            printf("请输入新的已借出：");
            scanf("%d", &p->borrowed);

            // 自动算剩余
            p->remain = p->total - p->borrowed;
            printf("修改成功！\n");
            return;
        }
        p = p->next;
    }

    printf("没有这本书！\n");
}

// 删除书籍
void deleteBook(struct List* list)
{
    if (list->head == NULL) {
        printf("没有书可以删！\n");
        return;
    }

    int id;
    printf("请输入要删除的编号：");
    scanf("%d", &id);

    struct Book* cur = list->head;
    struct Book* pre = NULL;

    // 找要删的那个编号
    while (cur != NULL && cur->id != id) {
        pre = cur;
        cur = cur->next;
    }

    // 没找到
    if (cur == NULL) {
        printf("找不到这个编号！\n");
        return;
    }

    // 找到了 → 删除
    if (pre == NULL) {
        // 删第一个
        list->head = cur->next;
    } else {
        // 删中间或最后一个
        pre->next = cur->next;
    }

    free(cur);
    printf("删除成功！\n");
}

// 保存书籍到文件
void saveToFile(struct List* list)
{
    // 1. 打开文件 books.txt
    FILE* fp = fopen("books.txt", "w");

    // 2. 定义指针p，从头开始
    struct Book* p = list->head;

    // 3. 循环把每一本书写进文件
    while (p != NULL)
    {
        fprintf(fp, "%d %s %s %d %d %d\n",
            p->id,
            p->name,
            p->author,
            p->total,
            p->borrowed,
            p->remain);

        p = p->next;
    }

    // 4. 关闭文件
    fclose(fp);
    printf("✅ 保存成功！\n");
}


// 从文件读取书籍数据
void loadFromFile(struct List* list)
{
    FILE* fp = fopen("books.txt", "r");

    if (fp == NULL) {
        printf("没有找到数据文件！\n");
        return;
    }

    int id, total, borrowed, remain;
    char name[50], author[30];

    while (fscanf(fp, "%d %s %s %d %d %d",
           &id, name, author, &total, &borrowed, &remain) != EOF)
    {
        struct Book* newBook = (struct Book*)malloc(sizeof(struct Book));
        newBook->id = id;
        strcpy(newBook->name, name);
        strcpy(newBook->author, author);
        newBook->total = total;
        newBook->borrowed = borrowed;
        newBook->remain = remain;
        newBook->next = NULL;

        if (list->head == NULL) {
            list->head = newBook;
        } else {
            struct Book* p = list->head;
            while (p->next != NULL) {
                p = p->next;
            }
            p->next = newBook;
        }
    }

    fclose(fp);
    printf("✅ 读取成功！\n");
}

// 主函数：程序入口
int main()
{
    // 1. 创建一个空链表（空袋子）
    struct List* lib = initList();

    int choice; // 存用户选择

    // 死循环：一直显示菜单
    while (1)
    {
        printf("\n===== 书籍管理系统 =====\n");
        printf("1. 添加书籍\n");
        printf("2. 显示所有书籍\n");
        printf("3. 按编号查找\n");
        printf("4. 按书名查找\n");      // 新加
        printf("5. 删除书籍\n");
        printf("6. 修改书籍信息\n");    // 新加
        printf("7. 保存数据到文件\n");
        printf("8. 从文件读取数据\n");
        printf("0. 退出\n");
        printf("========================\n");
        printf("请输入选项：");
        scanf("%d", &choice);

        // 根据选择调用函数
        if (choice == 1)
        addBook(lib);
    else if (choice == 2)
        showAllBooks(lib);
    else if (choice == 3)
        findBook(lib);
    else if (choice == 4)
        findBookByName(lib);   
    else if (choice == 5)
        deleteBook(lib);
    else if (choice == 6)
        modifyBook(lib);       
    else if (choice == 7)
        saveToFile(lib);
    else if (choice == 8)
        loadFromFile(lib);
    else if (choice == 0)
{
        printf("程序退出！\n");
        break;
}
    else
        printf("输入错误！\n");
    }

    return 0;
}


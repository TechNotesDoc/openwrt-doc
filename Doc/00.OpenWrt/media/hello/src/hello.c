#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <uci.h>

//根据选项来访问服务器。
struct Hello
{
    char agent[50]; //代理字符串
    char url[256];  //访问的url，此处设置为我们在Ubuntu中使用python命令开启的一个HTTPS服务器网址
    int delay;      //启动后延迟多长时间访问
};

int getValue(struct uci_context *ctx, char *key, char*value, int n)
{
    char strKey[100];
    struct uci_ptr ptr;
    snprintf(strKey, sizeof(strKey), "hello.globe.%s",key); //将hello.globe.agent/url/delay 写入strKey中
    //uci_lookup_ptr：分割字符串并查找
    //参数1：UCI上下文环境对象 2：查找的结果保存到ptr中 3：待查找的字符串 4：是否允许扩展查找
    if (uci_lookup_ptr(ctx, &ptr, strKey, true) == UCI_OK) //从/etc/config/hello配置文件中找（备注：默认从/etc/config目录下读取）
    {
        printf("%s\n", ptr.o->v.string);
        strncpy(value, ptr.o->v.string, n-1); //将查找的字符串保存到value中
    }
    return 0;
}

int read_conf( struct Hello *hello)
{
    struct uci_context *ctx = uci_alloc_context(); //分配UCI上下文环境对象
    if (!ctx)
    {
        fprintf(stderr, "No memory\n");
        return 1;
    }
    getValue(ctx, "agent", hello->agent, sizeof(hello->agent)); //将配置文件中agent的值读取到hello->agent中
    getValue(ctx, "url", hello->url, sizeof(hello->url)); //将配置文件中url的值读取到hello->url中
    char delay[20];
    getValue(ctx, "delay", delay, sizeof(delay)); //将配置文件中delay的值读取到hello->delay中
    hello->delay = atoi(delay); //字符串转换为整型
    uci_free_context(ctx); //释放UCI上下文环境对象
    return 0;
}

struct Hello hello;
int main(int argc, char* argv[])
{
    /*
    char agent[50] = "openwrt";
    char url[100] = "http://192.168.43.20:8080";
    */
    
    //从/etc/config/hello配置文件中读取参数，并将读取到的内容设置到struct Hello中
    read_conf(&hello);
    
    //打印一下从配置文件中获取的数据
    printf("agent=%s\n", hello.agent);
    printf("url=%s\n", hello.url);
    printf("delay=%d\n", hello.delay);
    
    char cmd[512] = {0};
    snprintf(cmd, sizeof(cmd), "wget --user-agent=%s %s",hello.agent, hello.url); //将wget命令设置到cmd字符串中
    //system("wget --user-agent=openwrt http://192.168.43.20:8080");
    printf("cmd=%s\n", cmd);

    //srand(time(NULL));
    int delay_time = rand() % hello.delay; //延迟时间
    printf("delay_time=%d\n", delay_time);
    sleep(delay_time);

    system(cmd);  //执行wge命令
    return 0;
}
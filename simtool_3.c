#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

struct dic_node{
    int stop;
    int son[26];
    int sum;
    char s[75];
    int pos;//�ڼ������� ��word_sum
}words[1000009]; //�ֵ���
int N,M;



typedef struct dic_node dic_node;

struct sigword{
    int sum;
    int pos;
};
typedef struct sigword sigword;
sigword word_sum[1060009];//ͳ���ܸ���
int tot_word=0;

int vis_t[1000009];
int vis_s[1000009];

struct stor_txt{
    sigword tx[6600];
    int tx_sum;
    char name[20];
    int code[130];
}arti[16090];                        //ÿƪ���µĸ���
typedef struct stor_txt stor_txt;
int is_f[1000009];


int hash[10009][130];

int reans[5][5009]={0};
int ans_tot[5]={0};

char buf[1<<26];
char *p1,*p2;

int tot_pos=0;    //�ֵ������
int tot_art=0;    //art��������

int sam_code[130];
int sam_sum[1060900];

char see[1009]={'\0'};
int selen=0;

void in_stop_word();                       //����
void in_hash_value();                      //����
void select_hashword();
int pan_char(char x);
int pan_zimu(char x);
int cmp1(const void* a,const void* b);
int cmp2(const void* a,const void* b);
void do_sample();

int get_num(char *s)
{
    int tmp=0;
    for(int i=0;s[i];i++){
        tmp=tmp*10+s[i]-'0';
    }
    return tmp;
}
int main(int argc,char *argv[]){
    words[0].stop=1;
    //scanf("%d %d",&N,&M);
   
    //N=get_num(argv[1]);
    //M=get_num(argv[2]);

    N=1000,M=16;
    in_stop_word();
    in_hash_value();
    select_hashword();
    do_sample();
}
void in_stop_word(){//����stopword
    char s[89];
    FILE *in=fopen("stopwords.txt","r");
    int len,now;
    while(fscanf(in,"%s",s)!=EOF){
        len=strlen(s);
        now=0;
        for(int i=0;i<len;i++){
            char x=tolower(s[i]);
            if(words[now].son[x-'a']==0){
                tot_pos++;
                words[now].son[x-'a']=tot_pos;
            }
            now=words[now].son[x-'a'];
        }
        words[now].stop=1;
    }
    fclose(in);
}
void in_hash_value(){
    FILE *in=fopen("hashvalue.txt", "r");
    int ch;
    for(int i=1;i<=10000;i++){
        for(int j=1;j<=128;j++){
            fscanf(in,"%1d",&ch);
            if(ch==0)hash[i][j]=-1;
            else hash[i][j]=1;
        }
        //ch=fgetc(in);
    }
    fclose(in);
}
void select_hashword(){
    FILE *in=fopen("article.txt","r");
    //FILE *out=fopen("myans.txt","w");
    p1=buf;
    p2=buf+fread(buf,1,1<<26,in);
    tot_art=0;
    int is_new=1;
    while(p1<p2){
    	
        if(is_new==1){
            tot_art++;            //��ҳ��һ��
            arti[tot_art].tx_sum=0;
            while(pan_char(*p1)==0&&p1<p2){//�����ո�֮���
                p1++;
            }
            int d=-1;
            while(pan_char(*p1)==1&&p1<p2){//��ȡ����
                arti[tot_art].name[++d]=*p1;
                p1++;
            }
            //puts(arti[tot_art].name);    //puts

            int now;                       //��ȡ����

            while(*p1!='\f'&&p1<p2){
                now=0;
                while(pan_zimu(*p1)==0&&p1<p2&&(*p1)!='\f'){//���������ϵ��ַ�
                    //putchar(*p1);
                    p1++;
                }
                selen=0;
                while(pan_zimu(*p1)!=0){               //�Ѿ��ǵ��ʲ�����
                    see[selen++]=tolower(*p1);
                    char kksk= tolower(*p1);
                    //putchar(*p1);
                    if(words[now].son[kksk-'a']==0){       //�½��ڵ�
                        tot_pos++;
                        //words[now].sum=0;
                        words[now].son[kksk-'a']=tot_pos;
                    }
                    now=words[now].son[kksk-'a'];
                    p1++;
                }
                if(words[now].stop==0){                  //����ͣ��
                    //puts(see);
                    if(vis_t[now]==0){                     //����ҳ��һ������
                        if(vis_s[now]==0){
                            word_sum[++tot_word].pos=now;   //˫���� �ֱ����ֵ�����ָ������ ��������ָ���ֵ����е�λ��
                            words[now].pos=tot_word;
                            vis_s[now]=1;
                            for (int o = 0; o < selen; o++) {
                                words[now].s[o]=see[o];
                            }
                            words[now].s[selen]='\0';
                        }
                        arti[tot_art].tx[++arti[tot_art].tx_sum].pos=now;//��¼�����ֵ������λ�� now
                        //printf("%d %d\n",arti[tot_art].tx_sum,tot_art);

                        //puts("wow");
                        vis_t[now]=1;                      //���ϱ�ǡ�
                    }
                    words[now].sum++;
                    //printf("%s->%d\n",chars[now],words[now].sum);
                }
                //printf("%d %d\n",arti[tot_art].tx_sum,tot_art);
            }
            int p;
            //printf("%d %d\n",arti[tot_art].tx_sum,tot_art);
            int t_cnt=0;
            for(int i=1;i<=arti[tot_art].tx_sum;i++){  //��������  ��ÿ�����ʽ��в���
                p=arti[tot_art].tx[i].pos;
                if(words[p].pos>tot_word)continue;
                arti[tot_art].tx[i].sum=words[p].sum;   //��ȡ����
                word_sum[words[p].pos].sum+=words[p].sum; //�������ۼ�
                t_cnt+=words[p].sum;
                words[p].sum=0;           //����ֵ����еļ���
                vis_t[p]=0;              //�����ҳ������ʱ���
            }
            //qsort(arti[tot_art].tx+1,arti[tot_art].tx_sum,sizeof(sigword),cmp1);//���򷽱��������
            is_new=0;
            //printf("%d",arti[tot_art].tx_sum);
        }
        if((*p1)=='\f'){
            is_new=1;
        }
        while(pan_char(*p1)==0&&p1<p2)p1++;
    }
    //puts("wow");
    qsort(word_sum+1,tot_word,sizeof(sigword),cmp2);//���������� ��ʼ��������������hashֵ
    for(int i=1;i<=N;i++){
    	printf("%s %d\n",words[word_sum[i].pos].s,word_sum[i].sum); 
        is_f[word_sum[i].pos]=i;
    }
    //����hashֵ
    for(int i=1;i<=tot_art;i++){
        int maxn=arti[i].tx_sum;
        int p;
        int fir_i=1;
        int artinum;
        for(int j=1;j<=maxn;j++){
            p=arti[i].tx[j].pos;
            if(is_f[p]!=0){
                artinum=arti[i].tx[j].sum;
                for(int k=1;k<=M;k++){
                    if(fir_i==1){
                        arti[i].code[k]=(hash[is_f[p]][k]*artinum);
                        fir_i=0;
                    }
                    else arti[i].code[k]+=(hash[is_f[p]][k]*artinum);
                }
            }
        }
        for(int k=1;k<=M;k++){
            arti[i].code[k]=((arti[i].code[k]>0)?1:0);
        }


    }
}
int pan_char(char x){
    return ((x)=='\f'||(x)==' '||(x)=='\n'||(x)=='\r') ? 0 : 1;
}
int pan_zimu(char x){
    if(x<='z'&&x>='a')return 1;
    if(x<='Z'&&x>='A')return 2;
    return 0;
}
int cmp1(const void* a,const void* b){
    sigword *x=(sigword *)a;
    sigword *y=(sigword *)b;
    return x->pos-y->pos;
}
int cmp2(const void* a,const void* b){
    sigword *x=(sigword *)a;
    sigword *y=(sigword *)b;
    if(y->sum==x->sum){
        return strcmp(words[x->pos].s,words[y->pos].s);
    }
    return y->sum-x->sum;
}
void do_sample(){
    FILE *in=fopen("sample.txt","r");
    FILE *out=fopen("result.txt","w");
    p1=buf;
    p2=buf+fread(buf,1,1<<24,in);
    int is_new=1;
    while(p1<p2){
        if(is_new==1){
            while(pan_char(*p1)==0&&p1<p2){//�����ո�֮���
                p1++;
            }
            int i=-1;
            while(pan_char(*p1)==1&&p1<p2){//��ȡ����
                fprintf(out,"%c",*p1);
                p1++;
            }fprintf(out,"\n");
            int now;                       //��ȡ����
            int pan_all=1;
            while(*p1!='\f'&&p1<p2){
                pan_all=1;
                now=0;
                while(pan_zimu(*p1)==0&&p1<p2&&(*p1)!='\f'){//���������ϵ��ַ�
                    //fprintf(out,"%c",*p1);
                    p1++;
                }
                while(pan_zimu(*p1)!=0){               //�Ѿ��ǵ��ʲ�����
                    char kksk= tolower(*p1);
                    //fprintf(out,"%c",*p1);
                    if(pan_all==0){
                        p1++;
                        continue;
                    }
                    if(words[now].son[kksk-'a']==0){       //�½��ڵ�
                        pan_all=0;
                        p1++;
                        continue;
                    }
                    now=words[now].son[kksk-'a'];
                    p1++;
                }
                if(pan_all==1)words[now].sum++;
            }
            int p;
            for(i=1;i<=N;i++){  //��������  ��ÿ�����ʽ��в���
                p=word_sum[i].pos;
                sam_sum[i]=words[p].sum;   //��ȡ����
                //printf("%d ",words[p].sum);
                words[p].sum=0;       //����ֵ����еļ���
            }//puts("");
            for(int j=1;j<=N;j++){
                if(sam_sum[j]==0)continue;
                for(int k=1;k<=M;k++){
                    if(j==1)sam_code[k]=(hash[j][k]*sam_sum[j]);
                    else sam_code[k]+=(hash[j][k]*sam_sum[j]);
                }

            }
            for(int k=1;k<=M;k++){
                //fprintf(out,"%d",sam_code[k]=(sam_code[k]>0?1:0));
                sam_code[k]=(sam_code[k]>0?1:0);
                //printf("%d ",sam_code[k]);
            }//fprintf(out,"\n");
            //getchar();
            //��ʼ������Щ�͵�ǰ�ĺ�����������
            int cnum;
            ans_tot[0]=ans_tot[1]=ans_tot[2]=ans_tot[3]=0;
            for(i=1;i<=tot_art;i++){
                cnum=0;
                for(int j=1;j<=M;j++){
                    if(arti[i].code[j]!=sam_code[j])cnum++;
                    if(cnum>3)break;
                }
                if(cnum>3)continue;
                reans[cnum][++ans_tot[cnum]]=i;
            }
            for(i=0;i<=3;i++){
                if(ans_tot[i]==0){
                    continue;
                }
                fprintf(out,"%d:",i);
                for(int j=1;j<=ans_tot[i];j++){
                    fprintf(out,"%s ",arti[reans[i][j]].name);
                }
                fprintf(out,"\n");
            }
            is_new=0;
        }
        if((*p1)=='\f'){
            is_new=1;
        }
        while(pan_char(*p1)==0&&p1<p2)p1++;
    }
}

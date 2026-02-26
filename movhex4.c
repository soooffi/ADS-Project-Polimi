#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <inttypes.h>

#define MAX_DIM 32
#define INF (UINT32_MAX/4)

typedef struct rotta_aerea {
    int destinazione;
    struct rotta_aerea *next;
} rotta_aerea;

typedef struct {
    uint8_t valore;
    int vicini[6];
    rotta_aerea *rotte;
}esagono_mappa;

typedef struct {
    int coordinate;
    uint32_t distanza;
}minheap;

esagono_mappa *mat=NULL;
int righe=0;
int colonne=0;

minheap *arr=NULL;
int mindim=0;
int mincap =0;


void init(int ncol, int nrig);
void change_cost(int x, int y, int v, int raggio);
void toggle_air_route(int x1, int y1, int x2, int y2);
void travel_cost(int xp, int yp, int xd, int yd);
static inline void heap_init(int cap);
static  inline void heap_push(minheap min);
static inline minheap heap_pop(void);

int main(){
    char str[MAX_DIM];
    int col,rig;
    int x,y,v,r,x1,y1,x2,y2,xp,yp,xd,yd;

    while (scanf("%s", str)>0) {
        switch (str[1]) {
            case 'n': {
               if ( scanf("%d %d", &col, &rig)== 2) {
                   init(col,rig);
                   break;
               } else {
                   return 1;
               }

            }
            case 'h': {
                if ( scanf("%d %d %d %d", &x, &y, &v, &r)==4) {
                    change_cost(x,y,v,r);
                    break;;
                } else {
                    return 1;
                }
            }

            case 'o': {
                if ( scanf("%d %d %d %d", &x1, &y1, &x2, &y2) == 4) {
                    toggle_air_route(x1,y1,x2,y2);
                    break;
                } else {
                    return 1;
                }

            }
            case 'r':{
                if (scanf("%d %d %d %d", &xp, &yp, &xd, &yd)==4) {
                    travel_cost(xp,yp,xd,yd);
                    break;
                } else {
                    return 1;
                }

            }


        }

    }

    if (mat) {
        free(mat);
        mat=NULL;
    }

    return 0;
}

void init(int ncol, int nrig){
    int i, j,ind;

    if (nrig <= 0 || ncol <= 0) {
        printf("KO\n");
        return;
    }

    if (mat) {
        free(mat);
        mat=NULL;
    }

    mat = malloc(sizeof(esagono_mappa) * (size_t)nrig * (size_t)ncol);

    if (mat) {

        righe=nrig;
        colonne=ncol;

        for(i=0; i<nrig;i++) {
            for(j=0;j<ncol;j++) {

                ind=i*ncol + j;
                mat[ind].valore=1;
                mat[ind].rotte=NULL;

                if ((i % 2) == 0) {
                    if (i-1 >= 0 && j-1 >= 0) {
                        mat[ind].vicini[0] =(i-1)*ncol +j-1;
                    }else
                        mat[ind].vicini[0] =-1;

                    if (i-1 >= 0) {
                        mat[ind].vicini[1] =(i-1)*ncol +j;

                    }else
                        mat[ind].vicini[1] =-1;

                    if (j+1 < ncol) {
                        mat[ind].vicini[2] =(i)*ncol +j+1;

                    }else
                        mat[ind].vicini[2]=-1;

                    if (i+1 < nrig) {
                        mat[ind].vicini[3] =(i+1)*ncol +j;

                    }else
                        mat[ind].vicini[3] =-1;

                    if (i+1 < nrig && j-1 >= 0) {
                        mat[ind].vicini[4] =(i+1)*ncol + j-1;

                    }else
                        mat[ind].vicini[4]=-1;

                    if (j-1 >= 0) {
                        mat[ind].vicini[5] =i*ncol +j-1;

                    }else
                        mat[ind].vicini[5] = -1;

                } else {
                    if (i-1>=0) {
                        mat[ind].vicini[0] = (i-1)*ncol +j;

                    }else
                        mat[ind].vicini[0] = -1;

                    if (i-1 >= 0 && j+1 < ncol) {
                        mat[ind].vicini[1] =(i-1)*ncol + j+1;

                    }else
                        mat[ind].vicini[1] =-1;

                    if (j+1 < ncol) {
                        mat[ind].vicini[2] = (i)*ncol +j+1;

                    }else
                        mat[ind].vicini[2] =-1;

                    if (i+1<nrig && j+1 < ncol) {
                        mat[ind].vicini[3] = (i+1)*ncol + j+1;

                    }else
                        mat[ind].vicini[3] =-1;

                    if (i+1 < nrig) {
                        mat[ind].vicini[4]= (i+1)*ncol + j;

                    }else
                        mat[ind].vicini[4] =-1;

                    if (j-1 >= 0) {
                        mat[ind].vicini[5] =i*ncol + (j-1);

                    }else
                        mat[ind].vicini[5] =-1;
                }

            }
        }

    printf("OK\n");

    } else {

        printf("KO\n");
        return;

    }

}

void change_cost(int x, int y, int v, int raggio) {
    int ind, i,t, tail, head, ind2, n, a,b,c,d;
    int* queue;
    int* distanza;

    if (x<0 || x>=colonne || y<0 || y>=righe || v<-10 || v>10 || raggio<=0) {
        printf("KO\n");
        return;

    }

    n=1+3*raggio*(raggio+1);
    queue=malloc(sizeof(int)*n);


    if (queue) {
        a = x - raggio;
        if (a < 0) {
            a = 0;
        }

        b = x + raggio;
        if (b >= colonne) {
            b = colonne - 1;
        }

        c = y - raggio;
        if (c < 0) {
            c = 0;
        }

        d = y + raggio;
        if (d >= righe) {
            d = righe - 1;
        }

        distanza=malloc(sizeof(int)*colonne*righe);
        if (distanza) {
            for (i=c, head=0,tail=0;i<=d;i++) {
                for (t=a;t<=b;t++) {
                    distanza[i*colonne+t]=-1;
                }
            }

            ind=y*colonne+x;
            queue[tail]=ind;
            tail++;
            distanza[ind]=0;
            n= (int)mat[ind].valore+ v;

            if (n<0) {
                n=0;

            } else if (n>100) {
                n=100;
            }
            mat[ind].valore=(uint8_t)n;

            while (head<tail) {
                ind=queue[head];
                head++;

                for (i=0;i<6;i++) {
                    ind2=mat[ind].vicini[i];

                    if (ind2!=-1 && distanza[ind2]==-1) {
                        distanza[ind2]=distanza[ind]+1;

                        if (distanza[ind2]<raggio) {
                            queue[tail]=ind2;
                            tail++;
                            int num=v*(raggio-distanza[ind2]);
                            if ( num>=0) {
                                n=(int)mat[ind2].valore+ num/raggio;
                            } else {
                                n=(int)mat[ind2].valore+ (num-(raggio-1))/raggio;
                            }


                            if (n<0) {
                                n=0;

                            } else if (n>100) {
                                n=100;
                            }
                            mat[ind2].valore=(uint8_t)n;
                        }
                    }
                }
            }

            printf("OK\n");
            free(queue);
            free(distanza);
        }

    } else {
        printf("KO\n");
        return;
    }
}

void toggle_air_route(int x1, int y1, int x2, int y2){
    int ind1, ind2;

    if (x1<0 || x1>=colonne || y1<0 || y1>=righe || y2<0 || y2>=righe || x2<0 || x2>=colonne) {
        printf("KO\n");
        return;
    }

    ind1=y1*colonne+x1;
    ind2=y2*colonne+x2;

    rotta_aerea *current = mat[ind1].rotte;
    rotta_aerea *prev=NULL;

    while (current != NULL) {
        if (current->destinazione == ind2) {

            if (prev == NULL) {
                mat[ind1].rotte = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            printf("OK\n");
            return;
        }
        prev = current;
        current = current->next;
    }


    rotta_aerea *nuova_rotta = malloc(sizeof(rotta_aerea));
    if (nuova_rotta == NULL) {
        printf("KO\n");
        return;
    }

    nuova_rotta->destinazione = ind2;
    nuova_rotta->next = mat[ind1].rotte;
    mat[ind1].rotte = nuova_rotta;

    printf("OK\n");

}

void travel_cost(int xp, int yp, int xd, int yd){
    uint32_t *dist;
    int i, t, ind, partenza, destinazione;
    minheap minimo;

    if (xp<0 || xp>=colonne || xd<0 || xd>=colonne || yd<0 || yd>=righe || yp<0 || yp>=righe) {
        printf("-1\n");
        return;
    }

    partenza=yp*colonne+xp;
    destinazione=yd*colonne+xd;
    if (partenza == destinazione) {
        printf("0\n");
        return;
    }

    dist = malloc(sizeof(uint32_t) * (size_t)righe * (size_t)colonne);

    if (dist) {
        for (i=0;i<righe;i++) {
            for(t=0;t<colonne;t++) {

                dist[i*colonne+t]= INF;
            }
        }

        dist[partenza]=0;
        heap_init(righe*colonne*3);

        heap_push((minheap){.coordinate=partenza,.distanza=0});

        while (mindim!=0) {
            minimo=heap_pop();
            ind=minimo.coordinate;
            if (minimo.distanza==dist[ind]) {

                if (ind==destinazione) {
                    break;
                } else {
                    if (mat[ind].valore>0){
                        for(i=0;i<6;i++) {
                            int a=mat[ind].vicini[i];
                            if (a!=-1) {
                                uint32_t b=dist[ind]+(uint32_t)mat[ind].valore;
                                if (b<dist[a]) {
                                    dist[a]=b;
                                    heap_push((minheap){.coordinate=a,.distanza=b});
                                }
                            }
                        }
                    }
                    uint32_t d = (uint32_t)mat[ind].valore;
                    if (d > 0) {
                        for (rotta_aerea *r = mat[ind].rotte; r != NULL; r = r->next) {
                            int c = r->destinazione;
                            uint32_t e = dist[ind] + d;
                            if (e < dist[c]) {
                                dist[c] = e;
                                heap_push((minheap){ .coordinate=c, .distanza=e });
                            }
                        }
                    }

                }
            }
        }

        if (dist[destinazione]==INF) {
            printf("-1\n");
        } else {
            printf("%" PRIu32 "\n", dist[destinazione]);
        }

        if (arr) {
            free(arr);
            arr=NULL;
        }
        mindim=0;
        mincap=0;
        free(dist);
    } else {
        printf("-1\n");
        return;
    }

}

static inline void heap_init(int cap) {
    if (arr) {
        free(arr);
        arr=NULL;
    }
    mincap= cap;
    mindim=0;
    arr = malloc(sizeof(minheap) * (mincap+1));
    if (!arr) {
        mincap=0;
        printf("KO\n");
        return;
    }
}

static inline void heap_push(minheap min) {
    if (mindim>=mincap) {
        return;
    }
    mindim++;
    arr[mindim]=min;
}


static inline minheap heap_pop(void) {
    if (mindim==0) {
        minheap s={.coordinate = -1, .distanza = (uint32_t)INF};
        return s;
    }
    minheap min=arr[1];
    arr[1]=arr[mindim];
    mindim--;
    int i=1;
    while (1) {
        int l=2*i;
        int r=2*i+1;
        int best=i;
        if (l<=mindim && arr[l].distanza<arr[best].distanza) {
            best=l;
        }
        if (r<=mindim && arr[r].distanza<arr[best].distanza) {
            best=r;
        }
        if (best!=i) {
            minheap temp=arr[i];
            arr[i]=arr[best];
            arr[best]=temp;
            i=best;
        } else {
            break;
        }
    }
    return min;
}


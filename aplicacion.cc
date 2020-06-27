#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <cstdio>
using namespace std;

//Prototipos de funcion->
int leerPrimerasPalabras(string[]);

void buscarOcurrencias(string);
void comparacion(string, string[]);

void asignarParrafo(string[]);
string leerEntradaEntera(int &);

void mostrarOcurrencias(string[], string);

struct PalabraFL{           //Estructura para las palabras
    int posicionI;          //puntero a donde empieza la palabra en texto original
    int posicionF;          //puntero a donde termina la palabra en el texto original
    string nombre1;         //valor de la palabra original
    string nombre2;         //valor de la palabra normalizada
    string tag;         
}ocurrencias[40];           //Array para las ocurrencias
int cantOcurrencias=0;      //Global para la cantidad de ocurrencias, se usa en todas las funciones

//Funcion main->
int main(){
    string palabrasABuscar[5];  //Array con las palabras a buscar ocurrencias en el texto
    int cantABuscar=leerPrimerasPalabras(palabrasABuscar);  //Se manda a leer el archivo de palabras a buscar
    
    for(int p=0; p<cantABuscar;p++){ //bucle para cada palabra
        string palabra=palabrasABuscar[p];  //Esta es la palabra a buscar
        
        buscarOcurrencias(palabra);      //Manda a leer archivo y guardar ocurrencias

        string* parrafos= new string[cantOcurrencias];//Creo array para los parrafos

        asignarParrafo(parrafos);//Manda a leer archivo y guarda el parrafo de la ocurrencia

        mostrarOcurrencias(parrafos, palabra);//Muestra las ocurrencias una por una de forma ordenada    

        delete[] parrafos; //Se vacia parrafos
        for(int a=0;a<cantOcurrencias; a++){// truncar los datos
            ocurrencias[a].nombre1='\0';
            ocurrencias[a].nombre2='\0';
            ocurrencias[a].posicionF=0;
            ocurrencias[a].posicionI=0;
            ocurrencias[a].tag='\0';
        }
        cantOcurrencias=0; //las ocurrencias cambian a 0 para no dejar basura
    }
   
    getchar();
    return 0;
}

//Funciones->
//Obtener las palabras a buscar leyendo un archivoIN->
//------------------------------------------------------------------------------------------
int leerPrimerasPalabras(string palabrasABuscar[]){
    ifstream archivoPalabrasI; //Archivo de entrada
    string lectura; //Contenido del archivo

    archivoPalabrasI.open("palabrasIN.txt",ios::in);   //abrir archivo en modo lectura, asumo que esta en mismo path
    if(archivoPalabrasI.fail()){
        cout<<"Error al abrir el archivo";
        exit(1);
    } 

    getline(archivoPalabrasI,lectura);    //Leer linea
  
    char* aux2= new char[lectura.length()+1];   //length+1 para evitar exception   
    strcpy(aux2,lectura.c_str());  //convierto la linea leida de string a char
        
    char *token = strtok(aux2,"-");    //Se corta la cadena en cada -
    int i=0;            //contador para guardar los valores

    while (token !=NULL){       //Se eliminan los guiones y se guarda en array
        token = strtok(NULL,"-");  //cambio el puntero del token
        if(token !=NULL){
            palabrasABuscar[i]=token; //se guarda la palabra a buscar sin el guion y cada una por separado
            i++;
        }    
    }
    archivoPalabrasI.close();
    delete[] aux2; //Se vacia aux2
    return i;
}
//------------------------------------------------------------------------------------------

//Obtener las ocurrencias leyendo la salida de freeling->
//------------------------------------------------------------------------------------------
void buscarOcurrencias(string palabra){
    ifstream archivoOutF; //Archivo de entrada
    string aux;     //auxiliar para leer la linea entera
    string linea[5]; //array para la palabra de la linea

    archivoOutF.open("salidaXML.txt",ios::in);   //abrir archivo en modo lectura, asumo que esta en mismo path
    if(archivoOutF.fail()){
        cout<<"Error al abrir el archivo";
        exit(1);
    } 

    while(!archivoOutF.eof()){//Siempre que no sea el final del archivo
        getline(archivoOutF,aux);    //Leer linea

        if(aux.length()>19){            //No cuentan las lineas que no sean palabras analizadas   
            char* aux2= new char[aux.length()+1];   //length+1 para evitar exception 
            strcpy(aux2,aux.c_str());   //convierto la linea leida de string a char
        
            char *token = strtok(aux2,"\"");    //Se corta la cadena en cada "
            int i=1;            //contador para guardar los valores

            while (token !=NULL){       //Se eliminan las comillas y se guarda en array
                token = strtok(NULL,"\"");  //cambio el puntero del token
                if(token !=NULL){
                    switch (i){//sirve la 3(inicio) 5(final) 6(nombre1) 9(nombre2) 11(tag)
                        case 3: linea[0]=token; break;
                        case 5: linea[1]=token; break;
                        case 7: linea[2]=token; break;
                        case 9: linea[3]=token; break;
                        case 11:linea[4]=token; break;
                        default: break;
                    }
                    i++;
                }    
            }

            delete [] aux2; //Se vacia aux2
            comparacion(palabra,linea);//llamado a comparacion y asignacion de ocurrencia
 //Estaria bueno vaciar  linea--------------------------------------------------------------------------------------------------------------------------------
        }
    }
    archivoOutF.close();
}

void comparacion(string palabra, string linea[]){
    if(palabra.compare(linea[3])==0){      //se compara la palabra a buscar con la palabra leida
        ocurrencias[cantOcurrencias].posicionI=stoi(linea[0]); //Se guarda en las
        ocurrencias[cantOcurrencias].posicionF=stoi(linea[1]); //ocurrencias
        ocurrencias[cantOcurrencias].nombre1=linea[2];
        ocurrencias[cantOcurrencias].nombre2=linea[3];
        ocurrencias[cantOcurrencias].tag=linea[4];
        cantOcurrencias++; //aumenta el contador para la proxima ocurrencia.
    }
}
//------------------------------------------------------------------------------------------

//Leer entrada de freeling y obtener los parrafos de las ocurrencias->
//------------------------------------------------------------------------------------------
void asignarParrafo(string parrafos[]){
    int tamanio=0;
    string entradaCompleta=leerEntradaEntera(tamanio);// manda a leer el texto original

    for(int j=0; j<cantOcurrencias; j++){       //for para asignar a cada ocurrencia un parrafo
        if(ocurrencias[j].posicionI-100<0){    //si hay menos de 100 carac antes, corta desde el 0 un total de la posicion inicial + 100 + tamanio de palabra
            parrafos[j]=entradaCompleta.substr(0, (ocurrencias[j].posicionI+100)+(ocurrencias[j].nombre1.length()));
        } 
        else if(ocurrencias[j].posicionF+100>tamanio){     //si hat menos de 100 carac despues, corta desde 100 antes un total de 100+(tamanio total - posicion inicial)
            parrafos[j]=entradaCompleta.substr((ocurrencias[j].posicionI-100), 100+(tamanio-ocurrencias[j].posicionI));
        } 
        else{    //corta desde 100 caracteres antes un total de 200 mas el tamanio de la palabra
            parrafos[j]=entradaCompleta.substr(ocurrencias[j].posicionI-100, 200+(ocurrencias[j].nombre1.length()));
        }
    }
}

string leerEntradaEntera(int &tamanio){
    ifstream archivoInF; //archivo de entrada, el original
    string salida;

    archivoInF.open("entrada.txt",ios::in);   //abrir archivo en modo lectura, asumo que esta en mismo path
    if(archivoInF.fail()){
        cout<<"Error al abrir el archivo";
        exit(1);
    } 
    if (archivoInF) {    // obtener archivo completo
        archivoInF.seekg (0, archivoInF.end);     //voy a posicion final
        tamanio = archivoInF.tellg();        //obtengo el tamanio
        archivoInF.seekg (0, archivoInF.beg);     //vuelvo al inicio
        
        char * entrada = new char [tamanio];        // creo el char para todo el archivo
        archivoInF.read (entrada,tamanio);       // leo todo el archivo en un solo char[]
        salida=string(entrada);     //convierto en string la entrada

        archivoInF.close();
    }
    return salida;
}
//------------------------------------------------------------------------------------------

//Mostrar las ocurrencias de una palabra y permitir que el usuario fluya entre los parrafos->
//------------------------------------------------------------------------------------------
void mostrarOcurrencias(string parrafos[], string palabra){
    int k=0;
    char control;
    cout<<" HUBO "<<cantOcurrencias<<" OCURRENCIAS DE "<<palabra<<" \n\tOCURRENCIAS:\r\n";
    while(k<cantOcurrencias && k>=0){
        cout<<"\n~"<<palabra<<"~Ocurrencia nro "<<k+1<<"; encontrada en "<<ocurrencias[k].posicionI<<".\r\n\n";
        cout<<"\t"<<parrafos[k]<<"\r\n";

        cout<<"\n Ocurrencia Anterior-> 1\n Ocurrencia Siguiente-> 3\n Cambiar de Palabra->0\n";
        fflush(stdin);
        cin >>control;
        
        if(control=='1'){
            if(k==0){
                cout<<"No hay ocurrencias anteriores.\n Ocurrencia Siguiente-> 3\n Cambiar de Palabra->0\n";
                fflush(stdin);
                cin >>control;

                if(control=='3') k++;
                if(control=='0'){
                    k=cantOcurrencias;
                    cout<<"\n\n\n\n\n";
                }
            }
            else k--;

        } else if(control=='3'){
            if(k==cantOcurrencias-1){
                cout<<"No hay ocurrencias siguientes.\n Ocurrencia Anterior-> 1\n Cambiar de Palabra->0\n";
                fflush(stdin);
                cin >>control;

                if(control=='1') k--;
                if(control=='0'){
                    k=cantOcurrencias;
                    cout<<"\n\n\n\n\n";
                }
            }
            else k++;

        }else if(control=='0'){
            k=cantOcurrencias;
            cout<<"\n\n\n\n\n";
        }
    }
}
//------------------------------------------------------------------------------------------

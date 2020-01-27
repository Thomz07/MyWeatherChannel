#define tBufferEnvoi 64 /// taille de buffer TCPIP

void load_file(String fichier,EthernetClient cl)
{
    File myFile = SD.open(fichier);
    if (myFile) {
    byte clientBuf[64];
    byte clientCount = 0;
    while (myFile.available())
    {
    clientBuf[clientCount] = myFile.read();
    clientCount++;
    if(clientCount > 63)
    {
    cl.write(clientBuf,64);
    clientCount = 0;
    }
    }
    if(clientCount > 0) cl.write(clientBuf,clientCount);
    myFile.close();
    }
}

void print_message(String tabChar, boolean new_ligne,EthernetClient cl)
{
        char m_Buff_Envoi[tBufferEnvoi];
        byte m_cursBuffer=0;/// curseur de buffer
        byte i=0,x=0;
        while( i < tabChar.length())
        {
        if(m_cursBuffer==tBufferEnvoi)/// on verifie que l'on ne depasse pas la taille du buffer
        {
        // transmission
        cl.write(m_Buff_Envoi,m_cursBuffer);/// on envoi le buffer
        delay(1);
        m_cursBuffer=0;/// on reposition notre curseur en debut de buffer
        //********************
        }
        else
        {
        m_Buff_Envoi[m_cursBuffer]= char(tabChar[i]);/// ecrire dans le buffer le caractere reçu
        m_cursBuffer++; ///sinon on incremente notre curseur
        i++; ///lire le caractere suivant
        }
        }
        if(m_cursBuffer > 0) cl.write(m_Buff_Envoi,m_cursBuffer); // envoie des dernieres donnees
        if (new_ligne== true) cl.println();
}

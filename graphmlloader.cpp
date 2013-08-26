#include "graphmlloader.h"

GraphMlLoader::GraphMlLoader(const QString filePath)
{
    QFile *file = new QFile(filePath);

    file->open((QIODevice::ReadOnly | QIODevice::Text));


    Gmodel = new GraphModel();
    graph = new GraphScene(Gmodel,-1000, -1000, 2000, 2000, 0);

    reader.setDevice(file);
}

void GraphMlLoader::getNode()
{        
    QString path = "";
    QString comment = "";
    
    while(!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "node"))
    {
        if(reader.tokenType() == QXmlStreamReader::StartElement && reader.name() == "data")
        {

            QXmlStreamAttributes attributes = reader.attributes();

            reader.readNext();//Переходим к содержимому

            if (attributes.value("id").toString() == "path")
            {

                path = reader.text().toString();
            }
            if(attributes.value("id").toString() == "comment")
            {
                comment = reader.text().toString();
            }
        }
        reader.readNext();
    }
    
    if(path == "")
        return;
    
    QFileInfo *fileInfo = new QFileInfo(path);
    
    GraphVisNode* node = graph->addNode(fileInfo);

    if(comment != "")
    {
        graph->addComment(node, comment);
    }
}

void GraphMlLoader::getEdge()
{
    QString comment = "";

    int target_id = 0;
    int source_id = 0;

    if(reader.name() == "edge")
    {
        QXmlStreamAttributes attributes = reader.attributes();

        source_id = attributes.value("source").toString().toInt();
        target_id = attributes.value("target").toString().toInt();
    }

    while(!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "edge"))
    {
        if(reader.tokenType() == QXmlStreamReader::StartElement && reader.name() == "data")
        {
            QXmlStreamAttributes attributes = reader.attributes();

            reader.readNext();
            if(attributes.value("id").toString() == "comment")
            {
                comment = reader.text().toString();
            }
        }

        reader.readNext();
    }

    qDebug() << source_id << target_id;
    GraphVisNode *source_node = Gmodel->getNodes().at(source_id)->mdata;
    GraphVisNode *target_node = Gmodel->getNodes().at(target_id)->mdata;

    if(source_node!= NULL && target_node!=NULL)
    {
        qDebug() << "edge added";
        GraphVisEdge *edge = graph->addEdge(source_node, target_node);

        if(comment!="");
            graph->addComment(edge, comment);
    }

}



GraphScene *GraphMlLoader::getGraph()
{
    
    while(!reader.atEnd() && !reader.hasError())
    {
        QXmlStreamReader::TokenType token = reader.readNext(); 
        
        if(token == QXmlStreamReader::StartDocument){
            continue;
        }
        
        if (token == QXmlStreamReader::StartElement)
        {
            if(reader.name() == "key" || reader.name() == "graph")
            {
                continue;
            }
            
            if(reader.name() == "node")
            {
                getNode();
            }
            if(reader.name() == "edge")
            {
                getEdge();
            }
        }
    }

    return graph;
}

<?php
class Model_Demandaarquivo extends Abstract_Model
{
    
    /**
     * Nome do schema
     * @var string
     */
    protected $_schema = 'demandasse';

    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'demandaarquivo';

    /**
     * Entidade
     * @var string / array
     */
    public $entity = array();

    /**
     * Montando a entidade
     * 
     */
    public function __construct($commit = true)
    {
        parent::__construct($commit);
        
        $this->entity['dmaid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['dmdid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
        $this->entity['arqid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['dmadsc'] = array( 'value' => '' , 'type' => 'text' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
        $this->entity['usucpfinclusao'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '11' , 'contraint' => 'fk');
        $this->entity['dmadtinclusao'] = array( 'value' => '' , 'type' => 'timestamp with time zone' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
        $this->entity['usucpfalteracao'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '11' , 'contraint' => 'fk');
        $this->entity['dmadtalteracao'] = array( 'value' => '' , 'type' => 'timestamp with time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmastatus'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['usucpfinativacao'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '11' , 'contraint' => 'fk');
        $this->entity['dmadtinativacao'] = array( 'value' => '' , 'type' => 'timestamp with time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
    }

    public function setArqId($descricao = "Arquivo Demandas SE", $campo = "arqid")
    {
        $file = new FilesSimec($this->_name, null, $this->_schema);
        $file->setUpload($descricao, $campo, false, false);
        $arqid = (int)$file->getIdArquivo();
        $this->setAttributeValue('arqid', $arqid);
    }

    public function getArquivo($idArquivo)
    {
        $file = new FilesSimec();
        $file->getDownloadArquivo($idArquivo);
    }
}

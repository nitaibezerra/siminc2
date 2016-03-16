<?php
class Model_Demanda extends Abstract_Model
{
    
    /**
     * Nome do schema
     * @var string
     */
    protected $_schema = 'demandas';

    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'demanda';

    /**
     * Entidade
     * @var string / array
     */
    public $entity = array();

    public function carregarPrioridades()
    {
        $sql = 'SELECT * FROM demandas.prioridade';
        return $this->_db->carregar($sql);
    }
    
    public function carregarClassificacao()
    {
        $classificacao = array();
        $classificacao[] = array('codigo' => 'I','descricao' => 'Incidente' );
        $classificacao[] = array('codigo' => 'P','descricao' => 'Resolução de problema' );
        $classificacao[] = array('codigo' => 'M','descricao' => 'Requisição de mudança' );
        $classificacao[] = array('codigo' => 'S','descricao' => 'Solicitação de Serviço' );
        
        return $classificacao;
    }
    
    public function carregarTipoDemanda()
    {
        $tipoDemanda = array();
        $tipoDemanda[] = array('codigo' => '1', 'descricao' => 'Inicial');
        $tipoDemanda[] = array('codigo' => '2', 'descricao' => 'Consultiva');
        $tipoDemanda[] = array('codigo' => '3', 'descricao' => 'Investigativa');
        $tipoDemanda[] = array('codigo' => '4', 'descricao' => 'Manutenção corretiva');
        $tipoDemanda[] = array('codigo' => '5', 'descricao' => 'Manutenção evolutiva');
        return $tipoDemanda;
    }
    
    public function isValid()
    {
        $isValid = parent::isValid();
        
        if($isValid){
            if(strpos($this->entity['dmddatainiprevatendimento']['value'], ':') && !strpos($this->entity['dmddatainiprevatendimento']['value'], '-') && !strpos($this->entity['dmddatainiprevatendimento']['value'], '/')){
                $this->error[] = array("name" => 'dmddatainiprevatendimento' , "msg" => utf8_encode("Não pode ter hora sem data"));
                $isValid =  false;
            } 
            
            if(strpos($this->entity['dmddatafimprevatendimento']['value'], ':') && !strpos($this->entity['dmddatafimprevatendimento']['value'], '-') && !strpos($this->entity['dmddatafimprevatendimento']['value'], '/')){
                $this->error[] = array("name" => 'dmddatafimprevatendimento' , "msg" => utf8_encode("Não pode ter hora sem data"));
                $isValid = false;
            }
        } 
        
        return $isValid;
    }
    
    /**
     * Montando a entidade
     * 
     */
    public function __construct($commit = true)
    {
        parent::__construct($commit);
        
        $this->entity['dmdid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['usucpfdemandante'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '11' , 'contraint' => 'fk');
        $this->entity['usucpfanalise'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '11' , 'contraint' => 'fk');
        $this->entity['usucpfexecutor'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '11' , 'contraint' => 'fk');
        $this->entity['docid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['dmdidorigem'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['tipid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['sidid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['dmdtitulo'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '250' , 'contraint' => '');
        $this->entity['dmddsc'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '4000' , 'contraint' => '');
        $this->entity['dmdreproducao'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '4000' , 'contraint' => '');
        $this->entity['dmddatainiprevatendimento'] = array( 'value' => '' , 'type' => 'timestamp without time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmddataconclusao'] = array( 'value' => '' , 'type' => 'timestamp without time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmddatainclusao'] = array( 'value' => '' , 'type' => 'timestamp without time zone' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmdporcentoatend'] = array( 'value' => '' , 'type' => 'smallint' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmdstatus'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['dmdprioridadeequipe'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['dmddatafimprevatendimento'] = array( 'value' => '' , 'type' => 'timestamp without time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['usucpfinclusao'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '11' , 'contraint' => '');
        $this->entity['dmdnomedemandante'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '50' , 'contraint' => '');
        $this->entity['dmdclassificacao'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['dmdclassificacaosistema'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['dmdcomplementolocal'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '50' , 'contraint' => '');
        $this->entity['priid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['dmdemaildemandante'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '50' , 'contraint' => '');
        $this->entity['dmdavaliacao'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['dmdavaliacaocomentario'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '1000' , 'contraint' => '');
        $this->entity['dmdsalaatendimento'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '50' , 'contraint' => '');
        $this->entity['laaid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['unaid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['motid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['dmdnumdocrastreamento'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '50' , 'contraint' => '');
        $this->entity['dmddatadocrastreamento'] = array( 'value' => '' , 'type' => 'timestamp without time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmdunidadedocrastreamento'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmdqtde'] = array( 'value' => '' , 'type' => 'bigint' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmdhorarioatendimento'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['celid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['dmdtempoadicional'] = array( 'value' => '' , 'type' => 'time without time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmdatendremoto'] = array( 'value' => '' , 'type' => 'boolean' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmdobstempoadicional'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '300' , 'contraint' => '');
        $this->entity['dmdcodseg'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '10' , 'contraint' => '');
        $this->entity['dmddatainiatendefetivo'] = array( 'value' => '' , 'type' => 'timestamp without time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['usucpfclassificador'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '11' , 'contraint' => '');
        $this->entity['dmddataclassificacao'] = array( 'value' => '' , 'type' => 'timestamp without time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmdatendurgente'] = array( 'value' => '' , 'type' => 'boolean' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmdjusturgente'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '4000' , 'contraint' => '');
        $this->entity['dmdjustprioridade'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '3000' , 'contraint' => '');
        $this->entity['odsid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['dmdmaterial'] = array( 'value' => '' , 'type' => 'boolean' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmdmaterialentregue'] = array( 'value' => '' , 'type' => 'boolean' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmdmaterialdevolvido'] = array( 'value' => '' , 'type' => 'boolean' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['usucpfmigracao'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '11' , 'contraint' => '');
        $this->entity['dmddatamigracao'] = array( 'value' => '' , 'type' => 'timestamp without time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmddataclassificacaosi'] = array( 'value' => '' , 'type' => 'timestamp without time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['usucpfclassificadorsi'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '11' , 'contraint' => '');
        $this->entity['dmdjudicial'] = array( 'value' => '' , 'type' => 'boolean' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['atiid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['scsid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => 'fk');
    }
}

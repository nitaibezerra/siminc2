<?php
class Model_Usuarioresponsabilidade extends Abstract_Model
{
    
    /**
     * Nome do schema
     * @var string
     */
    protected $_schema = 'scrum';

    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'usuarioresponsabilidade';

    /**
     * Entidade
     * @var string / array
     */
    public $entity = array();

    /**
     * Montando a entidade
     * 
     */
    public function __construct()
    {
        parent::__construct();
        
        $this->entity['rpuid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['pflcod'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['usucpf'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '11' , 'contraint' => 'fk');
        $this->entity['rpustatus'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['rpudata_inc'] = array( 'value' => '' , 'type' => 'timestamp without time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['prgid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'fk');
    }
    
    public function carregarEquipe($prgid = null)
    {
        $sql = 'SELECT * 
                FROM scrum.usuarioresponsabilidade usr
                LEFT JOIN seguranca.usuario usu ON (usr.usucpf = usu.usucpf)
                WHERE usr.rpustatus = \'A\'';
        
        if($prgid) $sql .= "AND prgid = {$prgid}";
        return $this->_db->carregar($sql);
    }
    
    public function carregarUltimaDemanda($cpf)
    {
        $sql = "SELECT dmd.dmddatainiprevatendimento, dmd.dmddatafimprevatendimento
                , dmdtitulo,dmddsc , usunome, usuemail, usufoneddd, usufonenum, dmd.dmdid
                ,dmd.sidid
                ,sis.sisdsc, sis.sisid, sis.sisabrev , sis.sisdiretorio
                --, * 
                FROM seguranca.usuario usu
                LEFT JOIN demandas.demanda dmd on (usu.usucpf = dmd.usucpfexecutor)
                LEFT JOIN demandas.sididassociasisid sid on (sid.sidid = dmd.sidid)
                LEFT JOIN seguranca.sistema sis ON (sis.sisid = sid.sisid)
                WHERE usucpf = '{$cpf}'
                AND dmd.dmddatafimprevatendimento IS NOT NULL
                ORDER BY dmd.dmddatafimprevatendimento DESC";
        return $this->_db->pegaLinha($sql);
    }
    
    public function desactiveAllByProgram($id)
    {
        $sql = "UPDATE scrum.usuarioresponsabilidade SET rpustatus = 'I' WHERE prgid = {$id}";
        return $this->_db->executar($sql);
    }
}

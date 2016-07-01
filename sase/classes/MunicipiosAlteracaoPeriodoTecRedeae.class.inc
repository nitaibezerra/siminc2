<?php

/**
 * Classe de manipulação da tabela sase.estruturacarreiraremuneracao
 *
 * @author Victor Martins Machado <VictorMachado@mec.gov.br>
 *
 * Objeto de Modelo de Avaliador Educacional
 */

require_once APPRAIZ . 'includes/library/simec/Listagem.php';

class MunicipiosAlteracaoPeriodoTecRedeae extends Modelo {
    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "sase.municipiosalteracaoperiodotecredeae";

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("matid");

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'matid' => null,
        'marid' => null,
        'esdid_ant' => null,
        'esdid_atual' => null
    );

    /**
     * Atributos String da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributosStr = array(
    );

    /**
     * @name $arCampos
     * @var array
     * @access protected
     * Campos da Tabela
     */
    protected $arCampos = array(
        'matid',
        'marid',
        'esdid_ant',
        'esdid_atual'
    );

    /**
     * Campos Obrigatórios da Tabela
     * @name $arCampos
     * @var array
     * @access protected
     */
    protected $arAtributosObrigatorios = array(
    );

    /**
     * Menságens gerais
     * @name $msg
     * @var string
     * @access public
     */
    public $msg;
    protected $ravid;
    protected $ratdata1periodo;
    protected $ratdata2periodo;

    /**
     * Popula Objeto com Array
     *
     * @param array $arDados
     * @return $this
     * @author Sávio Resende
     */
    public function popula( Array $arDados ){
        $this->arAtributos[$this->arChavePrimaria[0]] = $arDados[$this->arChavePrimaria[0]];
        $this->popularObjeto( $this->arCampos, $arDados );
        return $this;
    }

    public function __construct($id = null, $ravid = null, $ratdata1periodo = null, $ratdata2periodo = null){
        parent::__construct($id);
        $this->ravid = $ravid;
        $this->ratdata1periodo = $ratdata1periodo;
        $this->ratdata2periodo = $ratdata2periodo;
    }

    public function setRavid($ravid){
        $this->ravid = $ravid;
    }

    public function montaSqlRelatorio(){
        $ravid = $this->ravid;
        $ratdata1periodo = $this->ratdata1periodo;
        $ratdata2periodo = $this->ratdata2periodo;

        if(isset($this->ravid)) {
            $sql = <<<DML
                select
                    aed.aedid,
                    pcp.muncod,
                    mun.mundescricao,
                    esdO.esddsc as origem,
                    esdO.esdid as esdorigem,
                    esdD.esddsc as destino,
                    esdD.esdid as esddestino,
                    mat.matid,
                    mar.marid
                from sase.planocarreiraprofessor pcp
                inner join territorios.municipio mun on pcp.muncod = mun.muncod
                inner join sase.municipiosassistidosredeae mar on mar.pcpid = pcp.pcpid
                inner join sase.relatorioavaliadorredeae rav on rav.ravid = mar.ravid
                inner join sase.usuarioresponsabilidade rpu on rpu.usucpf = rav.usucpf and rpu.rpustatus = 'A'
                left join workflow.historicodocumento hst on hst.docid = pcp.docid
                left join workflow.acaoestadodoc aed on aed. aedid = hst.aedid
                left join workflow.estadodocumento esdO on esdO.esdid = aed.esdidorigem
                left join workflow.estadodocumento esdD on esdD.esdid = aed.esdiddestino
                left join sase.municipiosalteracaoperiodotecredeae mat on mat.marid = mar.marid and mat.esdid_ant = esdO.esdid and mat.esdid_atual = esdD.esdid
                where rav.ravid = {$ravid}
                and (date(htddata) >= date('{$ratdata1periodo}') and date(htddata) <= date('{$ratdata2periodo}'))
                order by htddata
DML;
        } else {
            $this->msg = "Informe o código do relatório, campo 'ravid'";
            return false;
        }
        return $sql;

    }

    public function getTabelaRelatorio(){
        $sql = $this->montaSqlRelatorio();
        if(!$sql){
            return false;
        }

        $lista = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
        $lista
            ->setQuery($sql)
            ->setCabecalho(array('Municípios', 'Situação anterior', 'Situação atual'))
            ->setCampos(array('mundescricao', 'origem', 'destino'))
            ->esconderColunas(array('aedid', 'muncod', 'esdorigem', 'esddestino', 'matid', 'marid'))
            ->addCallbackDeCampo('mundescricao', 'retornaColunaMunicipioQuadro4PDC')
            ->setLarguraColuna(array('mundescricao' => '33.33%', 'origem' => '33.33%', 'destino' => '33.33%'));


        $lista
            ->setFormOff();

        return $lista->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }
}
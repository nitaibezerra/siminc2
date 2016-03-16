<?php

/**
 * Classe de manipulação da tabela sase.estruturacarreiraremuneracao
 *
 * @author Victor Martins Machado <VictorMachado@mec.gov.br>
 *
 * Objeto de Modelo de Avaliador Educacional
 */

require_once APPRAIZ . 'includes/library/simec/Listagem.php';

class EstruturaCarreiraRemuneracao extends Modelo {
    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "sase.estruturacarreiraremuneracao";

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("ecrid");

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'ecrid' => null,
        'marid' => null,
        'esdid' => null,
        'ecrpercentdispersao' => null,
        'ecrrelprofaluno' => null,
        'ecrneceshorasdocente' => null,
        'ecrhorasdocentecontrat' => null
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
        'ecrid',
        'marid',
        'esdid',
        'ecrpercentdispersao',
        'ecrrelprofaluno',
        'ecrneceshorasdocente',
        'ecrhorasdocentecontrat'
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

    public function __construct($id = null, $ravid = null){
        parent::__construct($id);
        $this->ravid = $ravid;
    }

    public function montaSqlRelatorio(){
        $ravid = $this->ravid;

        if(isset($this->ravid)) {
            $sql = <<<DML
                select
                    mar.marid,
                    mun.mundescricao,
                    doc.esdid,
                    ecr.ecrid,
                    ecr.ecrpercentdispersao,
                    ecr.ecrrelprofaluno,
                    ecr.ecrneceshorasdocente,
                    ecr.ecrhorasdocentecontrat
                from sase.municipiosassistidosredeae mar
                inner join sase.planocarreiraprofessor pcp on mar.pcpid = pcp.pcpid
                inner join territorios.municipio mun on pcp.muncod = mun.muncod
                left join workflow.documento doc on pcp.docid = doc.docid
                left join sase.estruturacarreiraremuneracao ecr on ecr.marid = mar.marid
                where mar.ravid = {$ravid}
DML;
        } else {
            $this->msg = "Informe o código do relatório, campo 'ravid'";
            return false;
        }
        return $sql;

    }

    public function getTabelaRelatorio($campos = false, $layout = false){
        $sql = $this->montaSqlRelatorio();
        if(!$sql){
            return false;
        }

        $lista = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
        $lista
            ->setQuery($sql)
            ->setCabecalho(array('Municípios', 'Percentuais de Dispersão', 'Relaçao Professor Aluno', 'Necessidade de Horas docente', 'Horas docentes Contratadas'))
            ->setCampos(array('mundescricao', 'ecrpercentdispersao', 'ecrrelprofaluno', 'ecrneceshorasdocente', 'ecrhorasdocentecontrat'))
            ->esconderColunas(array('marid', 'esdid', 'ecrid'))
            ->setLarguraColuna(array('mundescricao' => '20%', 'ecrpercentdispersao' => '20%', 'ecrrelprofaluno' => '20%', 'ecrneceshorasdocente' => '20%', 'ecrhorasdocentecontrat' => '20%'));

        if($campos) {
            $lista
                ->addCallbackDeCampo('ecrpercentdispersao', 'retornaCampoPerDispersao')
                ->addCallbackDeCampo('ecrrelprofaluno', 'retornaCampoRelProfAluno')
                ->addCallbackDeCampo('ecrneceshorasdocente', 'retornaCampoNecesHorasDocente')
                ->addCallbackDeCampo('ecrhorasdocentecontrat', 'retornaCampoHorasDocenteContrat');
        }

        $lista
            ->setFormOff();

        return $lista->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }
}
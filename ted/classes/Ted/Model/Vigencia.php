<?php


class Ted_Model_Vigencia extends Modelo
{

    /**
     * Nome da Tabela
     * @var String
     */
    protected $stNomeTabela = 'ted.aditivovigencia';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('vigid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'vigid' => NULL,
        'vigdata' => NULL,
        'vigjustificativa' => NULL,
        'usucpf' => NULL,
        'tcpid' => NULL,
        'dataultimaalteracao' => NULL
    );

    /**
     * @var mixed
     */
    protected $_tcpid;

    /**
     * Construtor da classe
     */
    public function __construct()
    {
        $this->_tcpid = Ted_Utils_Model::capturaTcpid();
        $this->arAtributos['usucpf'] = $_SESSION['usucpf'];
        if (is_null($this->_tcpid)) {
            throw new Exception("Nenhum Termo encontrado.");
        }
    }

    private function getQueryList()
    {
        return sprintf("
            SELECT
              v.vigid,
              TO_CHAR(v.vigdata, 'DD/MM/YYYY') AS vigdata,
              v.vigjustificativa, u.usunome ||' - '|| v.usucpf AS usuario
              , TO_CHAR(dataultimaalteracao, 'DD/MM/YYYY') AS dataultimaalteracao
            FROM ted.aditivovigencia v
            JOIN seguranca.usuario u ON (u.usucpf = v.usucpf)
            WHERE v.tcpid = %d ORDER BY v.vigid DESC
        ", $this->_tcpid);
    }

    /**
     * Retonar uma listagem com os prazos cadastrados
     * @return @String
     */
    public function getList()
    {
        require_once APPRAIZ . 'includes/library/simec/Listagem.php';

        $list = new Simec_Listagem();
        $list->setCabecalho(array(
            'Fim da Vigência',
            'Justificativa',
            'Usuário',
            'Criado em'
        ));
        $list->addAcao('delete', 'deleteVigencia');
        $list->setQuery($this->getQueryList());

        $list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $list->turnOnPesquisator();
        $list->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
    }

    /**
     * Retorna uma vigencia válida para o Termo
     * @return array|bool
     */
    public function getDataVigencia()
    {
        $strSQL = "
            select
                to_char(t.dtvigenciaincial, 'DD/MM/YYYY') as dtvigenciaincial,
                to_char(t.dtvigenciafinal, 'DD/MM/YYYY') as dtvigenciafinal,
                a.vigid
            from ted.termocompromisso t
            left join ted.aditivovigencia a on (a.tcpid = t.tcpid)
            where t.tcpid = %d
            limit 1
        ";

        $dataSet = $this->pegaLinha(sprintf($strSQL, $this->_tcpid));
        if ($dataSet) {
            if ($dataSet['vigid']) {
                return array(
                    'inicio' => $dataSet['dtvigenciaincial'],
                    'fim' => $dataSet['dtvigenciafinal']
                );
            } else {
                return array(
                    'inicio' => $dataSet['dtvigenciaincial'],
                    'fim' => $dataSet['dtvigenciafinal']
                );
            }
        }

        return false;
    }

    /**
     * @param array $vigencia
     * @return string
     */
    private function getTamplateVigencia(array $vigencia)
    {
        $templateString = '<form class="form-horizontal">
            <div class="form-group">
                <label class="control-label col-md-2" for="dtexecucao">Inicio da Vigência:</label>
                <div class="col-md-10">
                    <input type="text" id="data-vigencia" disabled="disabled" value="%s" class="form-control">
                </div>
            </div>
            <div class="form-group">
                <label class="control-label col-md-2" for="vigdata">Fim da Vigência:</label>
                <div class="col-md-10">
                    <input type="text" disabled="disabled" value="%s" class="form-control">
                </div>
            </div>';

        if ($this->permiteIncluirAditivo()) {
            $templateString.= '<div class="form-group ">
                    <div class="col-md-12">
                        <button class="btn btn-primary" type="button" id="add-vigencia">
                            <span class="glyphicon glyphicon-plus" aria-hidden="true"></span> Incluir Aditívo
                        </button>
                    </div>
                </div>
            </form>';
        }
        return sprintf($templateString, $vigencia['inicio'], $vigencia['fim']);
    }

    /**
     * @return string
     */
    public function getVigencia()
    {
        $vigenciaAtual = $this->getDataVigencia();
        if ($vigenciaAtual) {
            return $this->getTamplateVigencia($vigenciaAtual);
        } else {
            return '';
        }
    }

    private function permiteIncluirAditivo()
    {
        $strSQL = sprintf("
            select * from workflow.historicodocumento
            where aedid in (2442, 1612) and docid = %d
        ", Ted_Utils_Model::getDocid($this->_tcpid));

        return (boolean) $this->pegaUm($strSQL);
    }
}

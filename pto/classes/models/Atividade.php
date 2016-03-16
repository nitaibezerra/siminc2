<?php
include_once APPRAIZ . 'includes/workflow.php';

class Model_Atividade extends Abstract_Model
{

    protected $_schema = 'pto';
    protected $_name = 'atividade';
    public $entity = array();

    public function __construct($commit = true)
    {
        parent::__construct($commit);
        $this->perfilUsuario = new Model_PerfilUsuario();
        $this->entity['atvid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk', 'label' => 'Código');
        $this->entity['etpid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '', 'label' => 'Atividade');
        $this->entity['docid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk', 'label' => 'DOCID');
        $this->entity['usucpf'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '11', 'contraint' => 'fk', 'label' => 'Executor');
        $this->entity['atvordem'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Ordem');
        $this->entity['atvdsc'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '500', 'contraint' => '', 'label' => 'Atividade');
        $this->entity['atvprazo'] = array('value' => '', 'type' => 'date', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Prazo');
        $this->entity['atvstatus'] = array('value' => '', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '1', 'contraint' => '', 'label' => 'Status');
        $this->entity['atvcritico'] = array('value' => '', 'type' => 'boolean', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '', 'label' => 'Crítico');
        $this->entity['atvobs'] = array('value' => '', 'type' => 'text', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Observação');
    }

    public function getDados($id = null, $painel = false)
    {
        $campos = 'atv.atvid, atv.atvdsc as atvdsc , atv.usucpf, atvprazo, e.esddsc, atv.atvcritico, atv.atvobs';
        if ($painel) {
            $campos = 'atv.atvid, atv.atvdsc as atvdsc , atv.usucpf, atvprazo, e.esddsc, atv.atvordem, atv.atvobs';
        }

        $etpid = (is_int($id) ? $id : $_SESSION['etpid']);
        $sql = "
             SELECT {$campos}
                FROM pto.atividade atv
                LEFT JOIN seguranca.usuario usu ON usu.usucpf = atv.usucpf
                INNER JOIN workflow.documento d ON atv.docid = d.docid
                INNER JOIN workflow.estadodocumento  e ON e.esdid = d.esdid
                WHERE atv.atvstatus = 'A'
                AND etpid = {$etpid}
                ORDER BY atv.atvordem ASC
         ";

        $dados = $this->_db->carregar($sql);
        if (is_array($dados)) {
            foreach ($dados as $key => $valor) {
                if ($valor['usucpf']) {
                    $usuario = new Model_Usuario();
                    $result = $usuario->getUsuarioByCpf($valor['usucpf']);
                    if (is_array($result))
                        $user = $result[0];
                    $dados[$key]['usucpf'] = $user['usunome'];
                }
                if ($valor['atvprazo']) {
                    $dados[$key]['atvprazo'] = date('d/m/Y', strtotime($valor['atvprazo']));
                }
				if ($valor['atvcritico']) {
					$dados[$key]['atvcritico'] = $valor['atvcritico'] == 't' ? 'SIM' : 'NÃO';
				}
            }
        }
        return $dados;
    }

    public function getListing()
    {
        $listing = new Listing(false);
        $listing->setPerPage(999999);
        $listing->setIdTable('table_atividade');
        $listing->setClassTable('table table-striped table-bordered sorted_table');

        if ($this->perfilUsuario->possuiAcessoConsulta()) {
            $listing->setHead(array(
                $this->getAttributeLabel('atvdsc'),
                $this->getAttributeLabel('usucpf'),
                $this->getAttributeLabel('atvprazo'),
                'Situação',
                $this->getAttributeLabel('atvcritico'),
		$this->getAttributeLabel('atvobs'),
            ));
            $listing->setActions(array('edit' => 'editar_atividade'));
        } else {
            $listing->setHead(array(
                $this->getAttributeLabel('atvdsc'),
                $this->getAttributeLabel('usucpf'),
                $this->getAttributeLabel('atvprazo'),
                'Situação',
                $this->getAttributeLabel('atvcritico'),
		$this->getAttributeLabel('atvobs'),
            ));
            $listing->setActions(array('resize-vertical' => 'ordenar_atividade', 'edit' => 'editar_atividade', 'delete' => 'excluir_atividade'));
        }

        return $listing;
    }

    public function salvarAtividade()
    {
        $this->populateEntity($_POST);

		if( !isset($_POST['atvcritico']) ){
			$this->setAttributeValue('atvcritico', 'f');
		}
        $this->setAttributeValue('etpid', $_SESSION['etpid']);
        $this->setAttributeValue('atvstatus', 'A');

        $cpf = $this->removeMask($this->getAttributeValue('usucpf'));
        $this->setAttributeValue('usucpf', $cpf);

        $docid = $this->getAttributeValue('docid');
        if ($this->isValid() and empty($docid)) {
            $docid = wf_cadastrarDocumento(WF_PTO, 'Planos Táticos Operacionais - PTO', WF_ESTADO_EM_EXECUCAO);
            $this->setAttributeValue('docid', $docid);
        }
        $atvordem = $this->getAttributeValue('atvordem');
        if (empty($atvordem)) {
            $dados = $this->getAllByValues(array('atvstatus' => 'A'));
            if ($dados) {
                $count = count($dados);
            }
            $this->setAttributeValue('atvordem', $count + 1);
        }
        $idAtividade = $this->save(false);

        if ($idAtividade == false) {
            throw new Exception('Erro ao inserir Atividade.');
        } else {
            return $idAtividade;
        }
    }

    public function inativar($id)
    {
        $this->populateEntity(array('atvid' => $id));
        $this->setAttributeValue('atvdsc', ($this->getAttributeValue('atvdsc')));
        $this->setAttributeValue('atvstatus', 'I');
        $this->treatEntityToUser();
        $id = $this->update();
        if ($id == false) {
            throw new Exception('Erro ao excluir a Atividade.');
        } else {
            return $id;
        }
    }

    public function removeMask($val)
    {
        return str_replace('.', '', str_replace('-', '', str_replace('/', '', $val)));
    }

    public function alterarOrdem($atvid, $ordem, $etpid)
    {
        $this->populateEntity(array('atvid' => $atvid));
        $this->setAttributeValue('atvordem', $ordem);
        $this->setAttributeValue('etpid', $etpid);
        $this->setDecode(false);
        $id = $this->update(false);
        if ($id == false) {
            ver($this->error, d);
            throw new Exception('Erro ao ordenar a Atividade.');
        } else {
            return $id;
        }
    }

    public function possuiAtividade($etpid)
    {
        $sql = " SELECT count(atvid) as total FROM pto.atividade WHERE atvstatus = 'A' AND etpid = {$etpid} ; ";
        $result = $this->_db->carregar($sql);
        $total = (int)$result[0]['total'][0];
        return ($total > 0 ? true : false);
    }

    public function getAtividadesExecucao()
    {
        $esdid = WF_ESTADO_EM_EXECUCAO;
        $tpdid = WF_PTO;
        $usucpf = $_SESSION['usucpf'];

        $sql = "
             SELECT atv.atvid, atv.atvdsc as atvdsc , atv.usucpf, atvprazo
                FROM pto.atividade atv
                LEFT JOIN seguranca.usuario usu ON usu.usucpf = atv.usucpf
                INNER JOIN workflow.documento documento ON documento.docid = atv.docid
                INNER JOIN workflow.estadodocumento estadodocumento ON estadodocumento.esdid = documento.esdid
                WHERE atv.atvstatus = 'A'
                  AND estadodocumento.esdid = {$esdid}
                  AND estadodocumento.tpdid = {$tpdid}
                  AND atv.usucpf = '{$usucpf}'
                ORDER BY atv.atvordem ASC
         ";

        $dados = $this->_db->carregar($sql);

        if (is_array($dados)) {
            foreach ($dados as $key => $valor) {
                if ($valor['usucpf']) {
                    $usuario = new Model_Usuario();
                    $result = $usuario->getUsuarioByCpf($valor['usucpf']);
                    if (is_array($result))
                        $user = $result[0];
                    $dados[$key]['usucpf'] = $this->mask($user['usucpf'], '###.###.###-##') . ' - ' . $user['usunome'];
                }
                if ($valor['atvprazo']) {
                    $dados[$key]['atvprazo'] = date('d/m/Y', strtotime($valor['atvprazo']));
                }
            }
        }
        return $dados;
    }

    public function getListingExecucao()
    {
        $listing = new Listing(false);
        $listing->setPerPage(999999);
        $listing->setIdTable('table_atividade');
        $listing->setClassTable('table table-striped table-bordered sorted_table');
        $listing->setHead(array(
            $this->getAttributeLabel('atvdsc'),
            $this->getAttributeLabel('usucpf'),
            $this->getAttributeLabel('atvprazo')
        ));
        $listing->setActions(array('eye-open' => 'selecionar_atividade'));
        return $listing;
    }

    function getDataPrazoComCor($atvprazo)
    {
        $dataArray = explode('/', $atvprazo);
        $dataPrazo = $dataArray[2] . '-' . $dataArray[1] . '-' . $dataArray[0];
        $dataAtual = date('Y-m-d');

        $time_inicial = strtotime($dataPrazo);
        $time_final = strtotime($dataAtual);
        $diferenca = $time_final - $time_inicial;
        $dias = (int)floor($diferenca / (60 * 60 * 24));

        if ($dias > 30 ) {
            return "<span style='color: red !important;'>{$atvprazo}</span>";
        } elseif ( $dias >= 0 and $dias <= 30 ) {
            return "<span style='color: darkorange !important;'>{$atvprazo}</span>";
        }else{
            return "<span style='color: darkolivegreen !important;'>{$atvprazo}</span>";
        }
    }
}

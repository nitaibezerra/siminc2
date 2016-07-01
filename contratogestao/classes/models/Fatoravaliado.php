<?php

include_once APPRAIZ . "includes/classes/file.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

class Model_Fatoravaliado extends Abstract_Model {

    protected $_schema = 'contratogestao';
    protected $_name = 'fatoravaliado';
    public $entity = array();
    private $dadosTabela = array();

    const ESTADO_EXECUTOR = 'executor';
    const ESTADO_VALIDADOR = 'validador';
    const ESTADO_CERTIFICADOR = 'certificador';

    public function __construct($commit = true) {
        parent::__construct($commit);

        $this->entity['fatid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk', 'label' => 'ID');
        $this->entity['conid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk', 'label' => 'Contrato');
        $this->entity['docid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk', 'label' => 'Documento');
	$this->entity['satid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk', 'label' => 'Satisfação');
        $this->entity['fatordem'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Ordem');
        $this->entity['fatdsc'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '1000', 'contraint' => '', 'label' => 'Fator Avaliado');
        $this->entity['fatprazo'] = array('value' => '', 'type' => 'date', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '', 'label' => 'Prazo');
        $this->entity['cofid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk', 'label' => 'Conformidade');
        $this->entity['entidexecutor'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Entidade Executor');
        $this->entity['entidvalidador'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Entidade Validador');
        $this->entity['entidcertificador'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Entidade Certificador');
        $this->entity['arqid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Arquivo');
        $this->entity['usucpfexecutor'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '11', 'contraint' => '', 'label' => 'Descrição');
        $this->entity['usucpfvalidador'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '11', 'contraint' => '', 'label' => 'Descrição');
        $this->entity['usucpfcertificador'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '11', 'contraint' => '', 'label' => 'Descrição');
        $this->entity['fatstatus'] = array('value' => 'A', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '1', 'contraint' => '', 'label' => 'Status');
        $this->entity['temid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk', 'label' => 'Tempestividade');
        $this->entity['fatvalordesembolso'] = array('value' => '', 'type' => 'numeric', 'is_null' => 'YES', 'contraint' => '', 'label' => 'Valor de Desembolso');
    }

    public function getDados($idAtividade) {
        $this->dadosTabela = $this->getDadosGrid($idAtividade);
        $dados = $this->corrigirDados();
        $dados = $this->adicionarHistorico($dados);
        $dados = $this->adicionarDownload($dados);
        return $dados;
    }

    public function corrigirDados($dados = array()) {
        if (!empty($dados)) {
            $this->dadosTabela = $dados;
        }
        if ($this->dadosTabela) {
            foreach ($this->dadosTabela as $key => $valores) {
                foreach ($valores as $indice => $valor) {
                    $this->corrirDadosGerais($indice, $key, $valor);
                    $this->corrirDadosPessoaFisica($indice, $key, $valor);
                    $this->corrirDadosPessoaJuridica($indice, $key, $valor);
                }
            }
        }
        return $this->dadosTabela;
    }

    public function getListing() {
        $listing = new Listing();
        $listing->setPerPage(1000);
        $listing->setHead(array('Histórico', 'Download', 'Fator Avaliado', 'Prazo', 'Documento', 'Conformidade', 'Tempestividade', 'Satisfação', 'Valor de Desembolso', 'Controle'));
        $listing->setActions(array('edit' => 'editar', 'delete' => 'excluir'));
        return $listing;
    }

    public function getDadosGrid($idAtividade) {
        $sql = " SELECT 
                    fatid, docid as docid_, arqid, fatdsc, fatprazo, docid, cofid, temid, satdsc,fatvalordesembolso, usucpfexecutor, entidexecutor, usucpfvalidador, entidvalidador, usucpfcertificador, entidcertificador
                 FROM contratogestao.fatoravaliado fat
                 LEFT JOIN contratogestao.satisfacao sat ON sat.satid = fat.satid
                 WHERE conid = {$idAtividade} AND fatstatus = 'A'
                 ORDER BY fatprazo ASC; ";

        return $this->_db->carregar($sql);
    }

    function getPessoaExecutor($tipo, $perfilFisica, $perfilJuridica) {
        if ($tipo === 'fisica') {
            $sql = " SELECT usu.usucpf as codigo, usu.usunome as descricao 
                        FROM seguranca.usuario usu
                        INNER JOIN seguranca.usuario_sistema us ON us.usucpf = usu.usucpf
                        INNER JOIN seguranca.perfilusuario pu ON pu.usucpf = usu.usucpf
                        WHERE pu.pflcod = {$perfilFisica} AND sisid = " . ID_SISTEMA . ";";
        } elseif ($tipo === 'juridica') {
            $sql = "SELECT  ent.entnumcpfcnpj as codigo,  ent.entnome as descricao 
	    		FROM  entidade.entidade ent 
	    		INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
	    		WHERE  fen.funid= {$perfilJuridica};";
        }
        return $this->_db->carregar($sql);
    }

    function getCombo($parans) {
        $dados = array();
        $options = "<option value=''>Nenhuma pessoa encontrada - Cadastre abaixo</option>";

        if ($parans['executor']) {
            $dados = $this->getPessoaExecutor($parans['tipo'], CONTRATO_PERFIL_EXECUTOR, FUNCAO_EXECUTOR);
        } elseif ($parans['validador']) {
            $dados = $this->getPessoaExecutor($parans['tipo'], CONTRATO_PERFIL_VALIDADOR, FUNCAO_VALIDADOR);
        } elseif ($parans['certificador']) {
            $dados = $this->getPessoaExecutor($parans['tipo'], CONTRATO_PERFIL_CERTIFICADOR, FUNCAO_CERTIFICADOR);
        }

        if ($dados) {
            $options = "<option value=''> Selecione </option>";
            foreach ($dados as $valor) {
                $options .="<option value='{$valor['codigo']}'>{$valor['descricao']}</option>";
            }
        }
        return $options;
    }

    function getNomesEtapaControle($tipo) {
        $nome = false;
        if ($this->getAttributeValue('usucpf' . $tipo)) {
            $usuario = new Model_Usuario();
            $dado = $usuario->getByValues(array('usucpf' => $this->getAttributeValue('usucpf' . $tipo)));
            if ($dado) {
                $nome = $dado['usunome'];
            }
        } elseif ($this->getAttributeValue('entid' . $tipo)) {
            $entidade = new Model_Entidade();
            $dado = $entidade->getByValues(array('entid' => $this->getAttributeValue('entid' . $tipo)));
            if ($dado) {
                $nome = $dado['entnome'];
            }
        }

        return $nome;
    }

    public function setDocumentoId($conid) {
        $contrato = new Model_Contrato();
        $atividade = $contrato->getContratoById($conid);
        $docdsc = $atividade['conid'] . ' - ' . $atividade['condescricao'] . '<br> ' . $_POST['fatdsc'];
        $docid = wf_cadastrarDocumento(WF_CONTRATO_GESTAO, $docdsc, WF_CONTRATO_GESTAO_EXECUTACAO);
        $this->setAttributeValue('docid', $docid);
    }

    public function salvar($conid) {
        $fatid = $this->getAttributeValue('fatid');

        if (empty($fatid)) {
            $this->setDocumentoId($conid);
            return $this->insert(true);
        } else {
            return $this->update();
        }
    }

    private function corrirDadosGerais($indice, $key, $valor) {
        $documento = new Model_Documento();
        $contrato = new Model_Contrato();
        $conformidade = new Model_Conformidade();
        $tempestividade = new Model_Tempestividade();
        
        if ($indice === 'conid' && !empty($valor)) {
            $dados = $contrato->getContratoById($valor);
            $this->dadosTabela[$key][$indice] = "{$dados['consigla']} - {$dados['condescricao']}";
        }
        if ($indice === 'fatprazo' && !empty($valor)) {
            $this->dadosTabela[$key][$indice] = date('d/m/Y', strtotime($valor));
        }
        if ($indice === 'cofid' && !empty($valor)) {
            $this->dadosTabela[$key][$indice] = $conformidade->getConformidadeById($valor);
        }
        if ($indice === 'temid' && !empty($valor)) {
            $this->dadosTabela[$key][$indice] = $tempestividade->getTempestividadeById($valor);
        }
        if ($indice === 'docid' && !empty($valor)) {
            $this->dadosTabela[$key][$indice] = $documento->getEstadoDocumentoById($valor);
        }

        if ($indice === 'fatvalordesembolso' && !empty($valor)) {
            $this->dadosTabela[$key]['fatvalordesembolso'] = number_format($valor, 2, ',', '.');
        }
    }

    private function corrirDadosPessoaFisica($indice, $key, $valor) {
        $camposControleUsuario = array('usucpfexecutor', 'usucpfvalidador', 'usucpfcertificador');
        $usuario = new Model_Usuario();

        if (in_array($indice, $camposControleUsuario)) {
            $pessoaFisica = $usuario->getByValues(array('usucpf' => $valor));
            $pessoa = $pessoaFisica['usunome'];
            if ($indice === 'usucpfexecutor') {
                $this->dadosTabela[$key]['controle'] .= ($valor ? '<b>Executor:</b> ' . $pessoa . ' <br> ' : '' );
                unset($this->dadosTabela[$key][$indice]);
            }
            if ($indice === 'usucpfvalidador') {
                $this->dadosTabela[$key]['controle'] .= ($valor ? '<b>Validador:</b> ' . $pessoa . ' <br> ' : '' );
                unset($this->dadosTabela[$key][$indice]);
            }
            if ($indice === 'usucpfcertificador') {
                $this->dadosTabela[$key]['controle'] .= ($valor ? '<b>Certificador:</b> ' . $pessoa : '' );
                unset($this->dadosTabela[$key][$indice]);
            }
        }
    }

    private function corrirDadosPessoaJuridica($indice, $key, $valor) {
        $entidade = new Model_Entidade();
        $camposControleEntidade = array('entidexecutor', 'entidvalidador', 'entidcertificador');
        if (in_array($indice, $camposControleEntidade)) {
            $pessoaJuridica = $entidade->getByValues(array('entid' => (int) $valor));
            if ($pessoaJuridica) {
                $pessoa = $pessoaJuridica['entnome'];
            }
            if ($indice === 'entidexecutor') {
                $this->dadosTabela[$key]['controle'] .= ($valor ? '<b>Executor:</b> ' . $pessoa . ' <br> ' : '' );
                unset($this->dadosTabela[$key][$indice]);
            }
            if ($indice === 'entidvalidador') {
                $this->dadosTabela[$key]['controle'] .= ($valor ? '<b>Validador:</b> ' . $pessoa . ' <br> ' : '' );
                unset($this->dadosTabela[$key][$indice]);
            }
            if ($indice === 'entidcertificador') {
                $this->dadosTabela[$key]['controle'] .= ($valor ? '<b>Certificador:</b> ' . $pessoa : '' );
                unset($this->dadosTabela[$key][$indice]);
            }
        }
    }

    function adicionarHistorico($dados) {
        if ($dados) {
            foreach ($dados as $key => $value) {
                $dados[$key]['docid_'] = "<a href='javascript:void(0);' class='historico_workflow'"
                        . " onclick=\"window.open('../geral/workflow/historico.php?modulo=principal/tramitacao&acao=A&docid={$value['docid_']}', 'alterarEstado','width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no');\"><i class='glyphicon glyphicon-time'></i></a>";
            }
        }
        return $dados;
    }

    function adicionarDownload($dados) {
        if ($dados) {
            foreach ($dados as $key => $value) {
                if($value['arqid']){
                    $dados[$key]['arqid'] = "<a href='contratogestao.php?modulo=principal/download&acao=A&arqid={$value['arqid']}' class='download_grid'><i class='glyphicon glyphicon-download-alt'></i></a>";
                }
            }
        }
        return $dados;
    }

}

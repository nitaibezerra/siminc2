<?php

class Model_PerfilUsuario extends Abstract_Model {

    protected $_schema = 'seguranca';
    protected $_name = 'perfilusuario';
    public $entity = array();

    const EXECUTOR = 'executor';
    const VALIDADOR = 'validador';
    const CERTIFICADOR = 'certificador';
    const ACESSO_TOTAL = 1;
    const ACESSO_EDICAO_CONTRATOS_VINCULADOS_RESTANTE_CONSULTA = 2;
    const ACESSO_SOMENTE_CONSULTA = 3;
    const NAO_POSSUI_ACESSO = 4;

    public function __construct($commit = true) {
        parent::__construct($commit);
        $this->entity['usucpf'] = array('value' => '', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '11', 'contraint' => 'pk');
        $this->entity['pflcod'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
    }

    public function setPerfilUsuario($cpf, $etapa) {
        $pflcod = '';
        switch ($etapa) {
            case self::EXECUTOR:
                $pflcod = CONTRATO_PERFIL_EXECUTOR;
                break;
            case self::VALIDADOR:
                $pflcod = CONTRATO_PERFIL_VALIDADOR;
                break;
            case self::CERTIFICADOR:
                $pflcod = CONTRATO_PERFIL_CERTIFICADOR;
                break;
        }
        $this->entity['usucpf']['value'] = $cpf;
        $this->entity['pflcod']['value'] = $pflcod;
    }

    public function salvar($cpf) {
        $dado = $this->getByValues(array('usucpf' => $cpf, 'pflcod' => $this->getAttributeValue('pflcod')));

        if ($dado === false) {
            return $this->insert(true, true);
        } else {
            return $this->update();
        }
    }

    public function validaAcessoTelaContrato() {
        $acessos = $this->getAllByValues(array('usucpf' => $_SESSION['usucpf']));
        $arrayAcessos = array();

        foreach ($acessos as $acesso) {
            $arrayAcessos[] = (int) $acesso['pflcod'];
        }

        if (in_array(CONTRATO_PERFIL_SUPER_USUARIO, $arrayAcessos) OR in_array(CONTRATO_PERFIL_ADMINISTRADOR, $arrayAcessos)) {
            $_SESSION['acesso_contrato'] = self::ACESSO_TOTAL;
        } else {
            if (in_array(CONTRATO_PERFIL_GESTOR_CONTRATO, $arrayAcessos)) {
                $_SESSION['acesso_contrato'] = self::ACESSO_EDICAO_CONTRATOS_VINCULADOS_RESTANTE_CONSULTA;
                $_SESSION['idsContratosQuePossuiAcesso'] = $this->contratosQuePossuiAcesso();
            } elseif (in_array(CONTRATO_PERFIL_CONSULTA, $arrayAcessos)) {
                $_SESSION['acesso_contrato'] = self::ACESSO_SOMENTE_CONSULTA;
            } else {
                $_SESSION['acesso_contrato'] = self::NAO_POSSUI_ACESSO;
                echo "<script>
                        alert('Você não tem permissão para acessar esta tela.');
                        history.back(-1);
                  </script>";
                die;
            }
        }
    }

    public function validarAcessoModificacao($conid) {
        $conid = (int) $conid;
        if ($_SESSION['acesso_contrato'] === self::ACESSO_TOTAL) {
            return true;
        } elseif ($_SESSION['acesso_contrato'] === self::ACESSO_EDICAO_CONTRATOS_VINCULADOS_RESTANTE_CONSULTA) {
            $idsContrato = $this->getIdsContrato();
            $itens = $this->getIdsContratosFilhos();
            $valida = ( in_array($conid, $idsContrato) OR ( $itens && in_array($conid, $itens) ));
            return (bool)$valida;
        } elseif ($_SESSION['acesso_contrato'] === self::NAO_POSSUI_ACESSO) {
            return false;
        }
    }
    
    public function possuiAcesso() {
        return ($_SESSION['acesso_contrato'] == Model_PerfilUsuario::ACESSO_TOTAL or 
                $_SESSION['acesso_contrato'] == Model_PerfilUsuario::ACESSO_EDICAO_CONTRATOS_VINCULADOS_RESTANTE_CONSULTA);
    }

    public function getIdsContratosFilhos() {
        $hierarquiacontrato = new Model_Hierarquiacontrato();
        $itens = array();
        foreach ($_SESSION['idsContratosQuePossuiAcesso'] as $contrato) {
            $itensFilho = $hierarquiacontrato->getNos($contrato['hqcid']);
            if ($itensFilho) {
                foreach ($itensFilho as $value) {
                    $itens[] = (int) $value['conid'];
                }
            }
        }
        return $itens;
    }
    
    public function getIdsContrato() {
        $idsContrato = array();
        foreach ($_SESSION['idsContratosQuePossuiAcesso'] as $contrato) {
            $idsContrato[] = $contrato['conid'];
        }
        return $idsContrato; 
    }

    public function contratosQuePossuiAcesso() {
        $usuarioResponsabilidade = new Model_UsuarioResponsabilidade();
        $contratosResp = $usuarioResponsabilidade->getAllByValues(array('usucpf' => $_SESSION['usucpf']));
        $contratoModel = new Model_Contrato();
        $arrayContratosId = array();
        foreach ($contratosResp as $key => $valor) {
            $contrato = $contratoModel->getContratoById((int) $valor['conid']);
            $arrayContratosId[$key]['hqcid'] = (int) $contrato['hqcid'];
            $arrayContratosId[$key]['conid'] = (int) $contrato['conid'];
        }
        return $arrayContratosId;
    }

}

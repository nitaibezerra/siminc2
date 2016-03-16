<?php

class Model_PerfilUsuario extends Abstract_Model
{

    protected $_schema = 'seguranca';
    protected $_name = 'perfilusuario';
    public $entity = array();

    const ACESSO_TOTAL = 1;
    const ACESSO_SOMENTE_CONSULTA = 2;
    const NAO_POSSUI_ACESSO = 3;
    const ACESSO_EXECUTOR = 4;

    public function __construct($commit = true)
    {
        parent::__construct($commit);
        $this->entity['usucpf'] = array('value' => '', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '11', 'contraint' => 'pk');
        $this->entity['pflcod'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
    }

    public function setPerfilUsuario($cpf, $pflcod = null)
    {
        $this->setAttributeValue('usucpf', $cpf);
		if(is_null($pflcod)){
			$this->setAttributeValue('pflcod', SOLUCAO_PERFIL_EXECUTOR);
		}else{
			$this->setAttributeValue('pflcod', $pflcod);
		}

    }

    public function salvar($cpf)
    {
        $dado = $this->getByValues(array('usucpf' => $cpf, 'pflcod' => $this->getAttributeValue('pflcod')));

        if ($dado === false) {
            return $this->insert(true, true);
        } else {
            return $this->update();
        }
    }

    public function validaAcesso()
    {
        $acessos = $this->getAllByValues(array('usucpf' => $_SESSION['usucpf']));
        $arrayAcessos = array();

        foreach ($acessos as $acesso) {
            $arrayAcessos[] = (int)$acesso['pflcod'];
        }

//        $arrayAcessos[15]= SOLUCAO_PERFIL_EXECUTOR;

        if (in_array(SOLUCAO_PERFIL_SUPER_USUARIO, $arrayAcessos) OR in_array(SOLUCAO_PERFIL_ADMINISTRADOR, $arrayAcessos)) {
            $_SESSION['acesso_contrato'] = self::ACESSO_TOTAL;
        } else {
            if (in_array(SOLUCAO_PERFIL_CONSULTA, $arrayAcessos)) {
                $_SESSION['acesso_contrato'] = self::ACESSO_SOMENTE_CONSULTA;

            } elseif (in_array(SOLUCAO_PERFIL_EXECUTOR, $arrayAcessos)) {
                $_SESSION['acesso_contrato'] = self::ACESSO_EXECUTOR;
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

    public function possuiAcessoEdicao()
    {
        return ($_SESSION['acesso_contrato'] == Model_PerfilUsuario::ACESSO_TOTAL);
    }

    public function possuiAcessoConsulta()
    {
        return (
            $_SESSION['acesso_contrato'] == Model_PerfilUsuario::ACESSO_SOMENTE_CONSULTA
            OR
            $_SESSION['acesso_contrato'] == Model_PerfilUsuario::ACESSO_EXECUTOR
        );
    }
}

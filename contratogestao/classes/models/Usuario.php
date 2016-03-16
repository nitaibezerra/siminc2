<?php

include APPRAIZ . 'includes/classes/EmailAgendado.class.inc';

class Model_Usuario extends Abstract_Model
{

    protected $_schema = 'seguranca';
    protected $_name = 'usuario';
    public $entity = array();

    public function __construct($commit = true)
    {
        parent::__construct($commit);

        $this->entity['usucpf'] = array('value' => '', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '11', 'contraint' => 'pk', 'label' => 'CPF');
        $this->entity['regcod'] = array('value' => '', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '2', 'contraint' => '', 'label' => 'UF');
        $this->entity['usunome'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '100', 'contraint' => '', 'label' => 'Nome');
        $this->entity['usuemail'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '50', 'contraint' => '', 'label' => 'E-mail');
        $this->entity['usustatus'] = array('value' => 'A', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '', 'label' => 'Status');
        $this->entity['usufoneddd'] = array('value' => '', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '2', 'contraint' => '', 'label' => 'DDD');
        $this->entity['usufonenum'] = array('value' => '', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '10', 'contraint' => '', 'label' => 'Telefone');
        $this->entity['ususenha'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '100', 'contraint' => '', 'label' => 'Senha');
        $this->entity['usudataultacesso'] = array('value' => '', 'type' => 'date', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Data Ultimo Acesso');
        $this->entity['usunivel'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Nivel');
        $this->entity['usufuncao'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '100', 'contraint' => '', 'label' => 'Função');
        $this->entity['ususexo'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '', 'label' => 'Sexo');
        $this->entity['orgcod'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '5', 'contraint' => '', 'label' => 'Cod Orgão');
        $this->entity['unicod'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '5', 'contraint' => '', 'label' => 'Unidade Orçamentária');
        $this->entity['usuchaveativacao'] = array('value' => '', 'type' => 'boolean', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'dddd');
        $this->entity['usutentativas'] = array('value' => '', 'type' => 'smallint', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'dddd');
        $this->entity['usuprgproposto'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '1000', 'contraint' => '', 'label' => 'dddd');
        $this->entity['usuacaproposto'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '1000', 'contraint' => '', 'label' => 'dddd');
        $this->entity['usuobs'] = array('value' => '', 'type' => 'text', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Observação');
        $this->entity['ungcod'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '6', 'contraint' => '', 'label' => 'dddd');
        $this->entity['usudatainc'] = array('value' => '', 'type' => 'timestamp without time zone', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'dddd');
        $this->entity['usuconectado'] = array('value' => '', 'type' => 'boolean', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'dddd');
        $this->entity['pflcod'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'dddd');
        $this->entity['suscod'] = array('value' => 'A', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '', 'label' => 'dddd');
        $this->entity['usunomeguerra'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '20', 'contraint' => '', 'label' => 'dddd');
        $this->entity['orgao'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '100', 'contraint' => '', 'label' => 'Órgao');
        $this->entity['muncod'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '7', 'contraint' => '', 'label' => 'Município');
        $this->entity['usudatanascimento'] = array('value' => '', 'type' => 'date', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Data de Nascimento');
        $this->entity['usudataatualizacao'] = array('value' => '', 'type' => 'timestamp without time zone', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'dddd');
        $this->entity['entid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk', 'label' => 'Órgão');
        $this->entity['tpocod'] = array('value' => '', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '1', 'contraint' => '', 'label' => 'Tipo Órgão');
        $this->entity['carid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk', 'label' => 'dddd');
    }

    public function isValid()
    {
        parent::isValid();

        foreach ($this->entity as $nameColumn => $column) {

            if ($nameColumn === 'usucpf' && empty($column['value'])) {
                $validate = new Zend_Validate_NotEmpty();
                $validate->isValid($column['value']);
                $this->error[] = array("name" => $nameColumn, "msg" => reset($validate->getMessages()));
            }

            if ($nameColumn === 'usucpf' && !empty($column['value'])) {
                if (!validaCPF($column['value'])) {
                    $this->error[] = array("name" => $nameColumn, "msg" => 'CPF Inválido');
                }
            }

            if ($nameColumn === 'usuemail' && !empty($column['value'])) {
                if (!$this->validarEmail($column['value'])) {
                    $this->error[] = array("name" => $nameColumn, "msg" => 'E-mail Inválido');
                }
            }
        }


        if ($this->error)
            return false;
        else
            return true;
    }

    public function treatEntity()
    {
        parent::treatEntity();
        foreach ($this->entity as $columnName => &$column) {
            if ($columnName === 'usucpf') {
                $column['value'] = $this->removeMaskCpf($column['value']);
            }
        }
    }

    public function getDadosUsuarioFatorAvaliado($dados)
    {
        $retorno = array();

        if (!empty($dados)) {
            $retorno = array(
                'usucpf' => $this->mask($this->getAttributeValue('usucpf'), '###.###.###-##'),
                'usunome' => $this->getAttributeValue('usunome'),
                'usuemail' => $this->getAttributeValue('usuemail'),
                'usufoneddd' => $this->getAttributeValue('usufoneddd'),
                'usufonenum' => $this->getAttributeValue('usufonenum'),
                'ususexo' => $this->getAttributeValue('ususexo'),
                'usuobs' => $this->getAttributeValue('usuobs'),
                'usudatanascimento' => $this->getAttributeValue('usudatanascimento'),
                'regcod' => $dados['regcod'],
                'muncod' => $dados['muncod'],
                'tpocod' => $dados['tpocod'],
                'entid' => $dados['entid'],
                'orgao' => $dados['orgao'],
            );
        }
        return $retorno;
    }

    public function getDadosUsuarioFatorAvaliadoReceitaFederal($cpf)
    {
        $usu_receita = recuperarUsuarioReceita($cpf);
        $retorno = array();
        if ($usu_receita['usuarioexiste']) {
            $dados = $usu_receita['dados'];
            $usudatanascimento = date('d/m/Y', strtotime($dados['dt_nascimento_rf']));
            list($ddd, $telefone) = explode('-', $dados['ds_contato_pessoa']);
            $retorno = array(
                'usunome' => $dados['no_pessoa_rf'],
                'usufoneddd' => $ddd,
                'usufonenum' => $telefone,
                'ususexo' => $dados['sg_sexo_rf'],
                'usudatanascimento' => $usudatanascimento,
                'regcod' => $dados['regcod'],
                'muncod' => $dados['muncod'],
                'tpocod' => $dados['tpocod'],
                'entid' => $dados['entid'],
                'orgao' => $dados['orgao'],
            );
        }
        return $retorno;
    }

    public function removeMaskCpf($cpf)
    {
        return str_replace('.', '', str_replace('-', '', $cpf));
    }

    function enviarEmail($etapa)
    {
        $assunto = 'Inscrição no Cadastro do SIMEC - Contrato Gestão';
        $comprimento = 'Prezado Sr.  ';

        if (str_replace('\'', '', $this->getAttributeValue('ususexo')) === 'F') {
            $comprimento = 'Prezada Sra. ';
        }

        $ususenha = md5_decrypt_senha($this->getAttributeValue('ususenha'), '');
        $usunome = str_replace('\'', '', $this->getAttributeValue('usunome'));
        $mensagem = $comprimento . $usunome . ',';

        $mensagem .= "<br><br>Você foi cadastrado(a) no SIMEC como {$etapa}, no sistema de Gestão de Contrato.";
        $mensagem .= "<br><br> <b>Sua Senha é {$ususenha}.</b> <br><br>Ao se conectar, altere esta senha para a sua senha preferida.<br><br> Para maiores informações entre em contato conosco:";
        $mensagem .= "<br><b>Telefone:</b> xxx xxxx-xxxx";
        $mensagem .= "<br><b>E-mail:</b> simec@teste.com.br";

        $e = new EmailAgendado();
        $e->setTitle($assunto);
        $e->setText($mensagem);
        $e->setName($this->getAttributeValue('usunome'));
        $e->setEmailOrigem("no-reply@mec.gov.br");
        $e->setEmailsDestino($this->getAttributeValue('usuemail'));
        $e->enviarEmails();
    }

    function salvar($cpf, $etapa)
    {
        $dados = $this->getAllByValues(array('usucpf' => $cpf));

        if (empty($dados)) {
            $senha = strtoupper(senha());
            $this->setAttributeValue('ususenha', $senha);
            $this->setAttributeValue('usuchaveativacao', 'f');
            $this->setAttributeValue('ususenha', md5_encrypt_senha($senha, ''));

            if (IS_PRODUCAO) {
                $this->enviarEmail($etapa);
            }
            return $this->insert(true, true);
        } else {
            $this->setAttributeValue('suscod', 'A');
            return $this->update();
        }
    }

    public function getComboUfs()
    {
        $sql = "SELECT regcod AS codigo, regcod||' - '||descricaouf AS descricao FROM uf WHERE codigoibgeuf IS NOT NULL ORDER BY 2";
        $dados = $this->_db->carregar($sql);
        return $this->getOptions($dados, array('prompt' => ' Selecione '), 'regcod');
    }

    public function getComboMunicipios($estuf)
    {
        $sql = "SELECT muncod AS codigo, mundescricao AS descricao  FROM territorios.municipio WHERE estuf = '" . $estuf . "' ORDER BY descricao";
        $dados = $this->_db->carregar($sql);
        return $this->getOptions($dados, array('prompt' => ' Selecione '), 'muncod');
    }

    public function getComboTipoOrgao()
    {
        $sql = "SELECT tpocod as codigo, tpodsc as descricao FROM public.tipoorgao WHERE tpostatus='A' ";
        $dados = $this->_db->carregar($sql);
        return $this->getOptions($dados, array('prompt' => ' Selecione '), 'tpocod');
    }

    public function getComboOrgaos($tpocod, $regcod, $muncod)
    {
        $inner = ($tpocod == 3 || $tpocod == 2) ? ' INNER JOIN entidade.endereco eed ON eed.entid = ee.entid ' : '';
        $uniao = ($tpocod == 3 || $tpocod == 2) ? " UNION ALL ( SELECT 999999 AS codigo, 'OUTROS' AS descricao )" : '';

        if ($tpocod == 2) {
            $clausula = " AND eed.estuf = '{$regcod}' ";
        } elseif ($tpocod == 3) {
            $clausula = " AND eed.muncod = '{$muncod}' ";
        }

        $sql = "(SELECT
                        ee.entid AS codigo,
                        CASE WHEN ee.entorgcod is not null THEN ee.entorgcod ||' - '|| ee.entnome
                        ELSE ee.entnome END AS descricao
                FROM
                        entidade.entidade ee
                INNER JOIN entidade.funcaoentidade ef ON ef.entid = ee.entid
                INNER JOIN public.tipoorgaofuncao tpf ON ef.funid = tpf.funid
                        " . $inner . "
                WHERE
                    ee.entstatus = 'A' and
                        tpf.tpocod = '{$tpocod}'
                        " . $clausula . " AND
                        ( ee.entorgcod is null or ee.entorgcod <> '73000' )

                ORDER BY
                        ee.entnome)" . $uniao;
        $dados = $this->_db->carregar($sql);
        if (is_array($dados)){
            return $this->getOptions($dados, array('prompt' => ' Selecione '), 'entid');
        }
        return '';

    }

    public function getOptions(array $dados, array $htmlOptions = array(), $idCampo = null, $descricaoCampo = null)
    {
        $html = '';
        $selected = '';


        if (isset($htmlOptions['prompt'])) {
            $html .= '<option value="">' . strtr($htmlOptions['prompt'], array('<' => '&lt;', '>' => '&gt;')) . "</option>\n";
        }

        if ($dados) {
            foreach ($dados as $data) {
                if ($idCampo) {
                    $selected = ($data['codigo'] === $this->getAttributeValue($idCampo) ? "selected='true' " : "");
                }
                $html .= "<option {$selected}  title=\"{$data['descricao']}\" value= " . $data['codigo'] . ">  " . simec_htmlentities($data['descricao']) . " </option> ";
            }
        }
        return $html;
    }

}

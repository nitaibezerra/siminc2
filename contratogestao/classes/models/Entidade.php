<?php

include_once APPRAIZ. 'www/includes/webservice/PessoaJuridicaClient.php';

class Model_Entidade extends Abstract_Model {

    protected $_schema = 'entidade';
    protected $_name = 'entidade';
    public $entity = array();

    public function __construct($commit = true) {
        parent::__construct($commit);

        $this->entity['entid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
        $this->entity['njuid'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '4', 'contraint' => 'fk');
        $this->entity['entnumcpfcnpj'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '18', 'contraint' => '');
        $this->entity['entnome'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '255', 'contraint' => '');
        $this->entity['entemail'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '100', 'contraint' => '');
        $this->entity['entnuninsest'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '14', 'contraint' => '');
        $this->entity['entobs'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '500', 'contraint' => '');
        $this->entity['entstatus'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '');
        $this->entity['entnumrg'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '40', 'contraint' => '');
        $this->entity['entorgaoexpedidor'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '15', 'contraint' => '');
        $this->entity['entsexo'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '');
        $this->entity['entdatanasc'] = array('value' => '', 'type' => 'date', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '');
        $this->entity['entdatainiass'] = array('value' => '', 'type' => 'date', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '');
        $this->entity['entdatafimass'] = array('value' => '', 'type' => 'date', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '');
        $this->entity['entnumdddresidencial'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '3', 'contraint' => '');
        $this->entity['entnumresidencial'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '10', 'contraint' => '');
        $this->entity['entnumdddcomercial'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '3', 'contraint' => '');
        $this->entity['entnumramalcomercial'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '4', 'contraint' => '');
        $this->entity['entnumcomercial'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '10', 'contraint' => '');
        $this->entity['entnumdddfax'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '3', 'contraint' => '');
        $this->entity['entnumramalfax'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '4', 'contraint' => '');
        $this->entity['entnumfax'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '10', 'contraint' => '');
        $this->entity['tpctgid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'entidade_tpcategoria');
        $this->entity['tpcid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'entidade_tipoclassificacao');
        $this->entity['tplid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'entidade_tplocalizacao');
        $this->entity['tpsid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'entidade_tpsituacao');
        $this->entity['entcodentsup'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '10', 'contraint' => '');
        $this->entity['entcodent'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '10', 'contraint' => 'entidade_entcodent_key');
        $this->entity['entcodent'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '10', 'contraint' => 'entidade_entid_key');
        $this->entity['entescolanova'] = array('value' => '', 'type' => 'boolean', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '');
        $this->entity['entdatainclusao'] = array('value' => '', 'type' => 'date', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '');
        $this->entity['entsig'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '50', 'contraint' => '');
        $this->entity['entunicod'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '5', 'contraint' => '');
        $this->entity['entungcod'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '6', 'contraint' => '');
        $this->entity['entproep'] = array('value' => '', 'type' => 'boolean', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '');
        $this->entity['entnumdddcelular'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '3', 'contraint' => '');
        $this->entity['entnumcelular'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '10', 'contraint' => '');
        $this->entity['entorgcod'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '5', 'contraint' => '');
        $this->entity['entsede'] = array('value' => '', 'type' => 'boolean', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '');
        $this->entity['entrazaosocial'] = array('value' => '', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '200', 'contraint' => '');
        $this->entity['entescolaespecializada'] = array('value' => '', 'type' => 'boolean', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '');
        $this->entity['entanocenso'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '4', 'contraint' => '');
    }

    public function isValid() {
        parent::isValid();

        foreach ($this->entity as $nameColumn => $column) {

            if ($nameColumn === 'entnumcpfcnpj' && !empty($column['value'])) {
                if (!validarCnpj($column['value'])) {
                    $this->error[] = array("name" => $nameColumn, "msg" => 'CNPJ Inválido');
                }
            }

            if ($nameColumn === 'entemail' && !empty($column['value'])) {
                if (!$this->validarEmail($column['value'])) {
                    $this->error[] = array("name" => $nameColumn, "msg" => 'E-mail Inválido');
                }
            }
        }
        if ($this->error) {
            return false;
        } else {
            return true;
        }
    }

    public function getSqlComboNaturezaJuridica() {
        $sql = 'SELECT njuid as codigo, njudsc as descricao FROM entidade.naturezajuridica ORDER BY descricao';
        return $this->_db->carregar($sql);
    }

    function getCombo() {
        $dados = $this->getSqlComboNaturezaJuridica();
        $options = "<option value=''> Selecione </option>";
        if ($dados) {
            foreach ($dados as $valor) {
                $options .="<option value='{$valor['codigo']}'>{$valor['descricao']}</option>";
            }
        }
        return $options;
    }

    public function removeMaskCnpj($cnpj) {
        return str_replace('.', '', str_replace('-', '', str_replace('/', '', $cnpj)));
    }

    public function getDadosEntidadeFatorAvaliado() {
        $retorno = array(
            'entnumcpfcnpj' => ( $this->getAttributeValue('entnumcpfcnpj') ? $this->mask($this->getAttributeValue('entnumcpfcnpj'), '##.###.###/####-##') : ''),
            'entnome' => $this->getAttributeValue('entnome'),
            'entrazaosocial' => $this->getAttributeValue('entrazaosocial'),
            'entsig' => $this->getAttributeValue('entsig'),
            'entobs' => $this->getAttributeValue('entobs'),
            'njuid' => $this->getAttributeValue('njuid'),
            'entemail' => $this->getAttributeValue('entemail'),
            'entnumdddcomercial' => $this->getAttributeValue('entnumdddcomercial'),
            'entnumcomercial' => $this->getAttributeValue('entnumcomercial'),
            'entnumramalcomercial' => $this->getAttributeValue('entnumramalcomercial'),
            'entnumdddfax' => $this->getAttributeValue('entnumdddfax'),
            'entnumfax' => $this->getAttributeValue('entnumfax'),
            'entnumramalfax' => $this->getAttributeValue('entnumramalfax'),
        );
        return $retorno;
    }

    public function getDadosEntidadeFatorAvaliadoReceitaFederal($cnpj) {
        $objXml = $this->getEntidadeReceitaFederalByCnpj($cnpj);
        return $this->setEntidadeByObjetoReceitaFederal($objXml);
    }

    public function getEntidadeReceitaFederalByCnpj($cnpj) {
        $objPessoaJuridica = new PessoaJuridicaClient("http://ws.mec.gov.br/PessoaJuridica/wsdl");
        $xml = $objPessoaJuridica->solicitarDadosPessoaJuridicaPorCnpj($cnpj);
        $xmlCorrigido = str_replace(array("& "), array("&amp; "), $xml);
        return simplexml_load_string($xmlCorrigido);
    }

    public function setEntidadeByObjetoReceitaFederal($objXml) {
        $error = (string) $objXml->PESSOA->ERRO;
        if (!empty($error)) {
            return false;
        }

        $entidade = new Model_Entidade();
        $entidade->setAttributeValue('entnumcpfcnpj', (string) $objXml->PESSOA->nu_cnpj_rf);
        $entidade->setAttributeValue('entnome', (string) $objXml->PESSOA->no_fantasia_rf);
        $entidade->setAttributeValue('entrazaosocial', (string) $objXml->PESSOA->no_empresarial_rf);
        $entidade->setAttributeValue('njuid', (integer) $objXml->PESSOA->co_natureza_juridica_rf);

        $contatos = (array) $objXml->PESSOA->CONTATOS;
        if (is_array($contatos['CONTATO'])) {
            if (count($contatos['CONTATO']) >= 2) {

                if (strpos((string) $contatos["CONTATO"][0]->ds_contato_pessoa, '-') !== false) {
                    list($ddd, $telefone) = explode('-', (string) $contatos["CONTATO"][0]->ds_contato_pessoa);
                    $entidade->setAttributeValue('entnumdddcomercial', $ddd);
                    $entidade->setAttributeValue('entnumcomercial', $telefone);
                }
                if (strpos((string) $contatos["CONTATO"][1]->ds_contato_pessoa, '-') !== false) {
                    list($ddd2, $telefone2) = explode('-', (string) $contatos["CONTATO"][1]->ds_contato_pessoa);
                    $entidade->setAttributeValue('entnumdddfax', $ddd2);
                    $entidade->setAttributeValue('entnumfax', $telefone2);
                }
            } elseif (count($contatos) === 1) {
                if (strpos((string) $contatos["CONTATO"][0]->ds_contato_pessoa, '-') !== false) {
                    list($ddd, $telefone) = explode('-', (string) $contatos["CONTATO"][0]->ds_contato_pessoa);
                    $entidade->setAttributeValue('entnumdddcomercial', (integer) $ddd);
                    $entidade->setAttributeValue('entnumcomercial', (integer) $telefone);
                }
            }
        }
        return $entidade;
    }

}

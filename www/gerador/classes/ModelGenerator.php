<?php

/**
 * Created by PhpStorm.
 * User: juniosantos
 * Date: 06/10/2015
 * Time: 11:40
 */
class ModelGenerator
{
    public $schema;
    public $prefixoClasse;
    public $extensao;
    public $tabela;
    public $atributos;
    public $nomeClasse;

    public $column_name;
    public $is_nullable;
    public $data_type;
    public $character_maximum_length;
    public $constraint_name;

    private $_data;

    public function __construct(Array $properties = array())
    {
        if (!empty($properties)) {
            foreach ($properties as $key => $value) {
                $this->{$key} = $value;
            }
        }
        $this->_data = $properties;
    }

    public function setAtributos(Array $atributos = array())
    {
        if (!empty($atributos)) {
            foreach ($atributos as $key => $value) {
                $this->{$key} = $value;
            }
        }
    }

    public function __set($property, $value)
    {
        return $this->_data[$property] = $value;
    }

    public function __get($property)
    {
        return array_key_exists($property, $this->_data) ? $this->_data[$property] : $this->$property;
    }

    protected function getIncludes()
    {
        if ('s' == $_REQUEST['include']) {
            return <<<PHP
require_once APPRAIZ .'includes/classes/Modelo.class.inc';
\n
PHP;
        }
    }

    protected function getComentarioTopo()
    {
        $usunome = $_SESSION['usunome'];
        $usuemail = $_SESSION['usuemail'];
        $data = date('d-m-Y');
        return <<<PHP
<?php
/**
 * Classe de mapeamento da entidade {$this->schema}.{$this->tabela}
 *
 * @category Class
 * @package  A1
 * @author   {$usunome} <{$usuemail}>
 * @license  GNU simec.mec.gov.br
 * @version  Release: {$data}
 * @link     no link
 */

\n
PHP;
    }

    protected function getNomeClasse()
    {
        return <<<PHP

/**
 * {$this->prefixoClasse}{$this->nomeClasse}
 *
 * @category Class
 * @package  A1
 * @author   {$usunome} <{$usuemail}>
 * @license  GNU simec.mec.gov.br
 * @version  Release: $data
 * @link     no link
 */
class {$this->prefixoClasse}{$this->nomeClasse} extends Modelo
{
PHP;
    }

    protected function getGravar($pkData)
    {
        $pk = ( !empty($pkData) ? $pkData[0]['column_name'] : '');
        return <<<PHP
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function gravar()
    {
        global \$url;
        \$this->popularDadosObjeto();
        \$url .= '&{$pk}=' . \$this->{$pk};

        try{
            \$sucesso = \$this->salvar();
            \$this->commit();
        } catch (Simec_Db_Exception \$e) {
            simec_redirecionar(\$url, 'error');
        }

        if(\$sucesso){
            simec_redirecionar(\$url, 'success');
        }
        simec_redirecionar(\$url, 'error');
    }//end gravar()\n
\n
PHP;
    }

    protected function getExcluir()
    {
        $pk = ( !empty($pkData) ? $pkData[0]['column_name'] : '');
        return <<<PHP
   /**
     * Função excluir
     * - grava os dados
     *
     */
    public function excluir()
    {
        global \$url;
       // \$url = 'aspar.php?modulo=principal/proposicao/index&acao=A';
        try{
            \$this->excluir();
            \$this->commit();
            simec_redirecionar(\$url, 'success');
        } catch (Simec_Db_Exception \$e) {
            simec_redirecionar(\$url, 'error');
        }
    }//end excluir()\n
\n
PHP;
    }

    protected function getAtributos($pkData, $fkData)
    {
        $stringClass = <<<PHP

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected \$stNomeTabela = '{$this->schema}.{$this->tabela}';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected \$arChavePrimaria = array(\n
PHP;
        foreach ($pkData ? $pkData : array() as $pk) {
            $stringClass .= <<<PHP
        '{$pk['column_name']}',\n
PHP;
        }
        $stringClass .= <<<PHP
    );
PHP;

        $stringClass .= <<<PHP

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected \$arChaveEstrangeira = array(\n
PHP;
        foreach ($fkData ? $fkData : array() as $fk) {
            $matches = array();
            $pattern = "/FOREIGN KEY \(([\w, ]+)\) REFERENCES ([\w_\.]+)\(([\w, ]+)\)/";
            preg_match($pattern, $fk['condef'], $matches);
            list(, $fk, $tabela, $pk) = $matches;

            $stringClass .= <<<PHP
        '{$fk}' => array('tabela' => '{$tabela}', 'pk' => '{$pk}'),\n
PHP;
        }

        $stringClass .= <<<PHP
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected \$arAtributos = array(\n
PHP;
        foreach ($this->atributos as $srAtributo) {
            $stringClass .= <<<PHP
        '{$srAtributo['column_name']}' => null,\n
PHP;
        }
        $stringClass .= <<<PHP
    );\n
PHP;
        return $stringClass;
    }

    protected function getValidacao()
    {
        $stringClass = <<<PHP
    /**
     * Atributos
     * @var \$dados array
     * @access protected
     */
    public function getCamposValidacao(\$dados = array())
    {
        return array(\n
PHP;


        foreach ($this->atributos as $atributo) {

            $strValidacao = $this->getValidacaoCampo($atributo);

            $stringClass .= <<<PHP
            '{$atributo['column_name']}' => $strValidacao,\n
PHP;
        }
        $stringClass .= <<<PHP
        );
    }//end getCamposValidacao(\$dados)\n
\n
PHP;
        return $stringClass;
    }

    private function getValidacaoCampo($atributo)
    {
        $validacao = array();

        if ($atributo['is_nullable'] == 'YES') {
            $validacao[] = "'allowEmpty' => true";
        }

        switch ($atributo['data_type']) {
            case 'integer':
                $validacao[] = " 'Digits' ";
                break;
            case 'character varying':
            case 'character':
                $validacao[] = " new Zend_Validate_StringLength(array('max' => {$atributo['character_maximum_length']})) ";
                break;
            case 'text':
                break;
            case 'date':
                //$validacao[] = " new Zend_Validate_Date() ";
                break;
            case 'boolean':
                break;
        }
        $str = implode(',', $validacao);
        return "array( $str )";
    }

    public function gerarModel($pkData, $fkData)
    {
        $this->nomeClasse = ucFirst($this->tabela);

        if (!$arquivo = fopen(APPRAIZ . "www/gerador/arquivos_gerados/model/{$this->nomeClasse}{$this->extensao}", "w+")) {
            return false;
        }

        $classe = $this->getModel($pkData, $fkData);


        if (!fwrite($arquivo, $classe)) {
            echo "Erro ao escrever no arquivo";
        } else {
            echo "Classe <b>{$this->tabela}</b> criada com sucesso.<br>";
        }
        fclose($arquivo);
    }


    public function getModel($pkData, $fkData)
    {
        $stringClass = $this->getComentarioTopo();
        $stringClass .= $this->getIncludes();
        $stringClass .= $this->getNomeClasse();
        $stringClass .= $this->getAtributos($pkData, $fkData);
//        $stringClass .= $this->getValidacao();
//        $stringClass .= $this->getGravar($pkData);
//        $stringClass .= $this->getExcluir();

        $stringClass .=  <<<PHP
\n}//end Class
?>
PHP;
        return $stringClass;
    }
}
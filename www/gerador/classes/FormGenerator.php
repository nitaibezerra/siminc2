<?php

/**
 * Created by PhpStorm.
 * User: juniosantos
 * Date: 06/10/2015
 * Time: 11:40
 */
class FormGenerator
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
        $tabela = ucFirst($this->tabela);
            return <<<PHP
include_once APPRAIZ .'includes/classes/{$this->schema}/{$this->prefixoClasse}_{$tabela}';
\n
PHP;
    }

    protected function getComentarioTopo()
    {
        $usunome = $_SESSION['usunome'];
        $usuemail = $_SESSION['usuemail'];
        $data = date('d-m-Y');
        return <<<PHP
<?php
/**
 * Formulário da entidade {$this->schema}.{$this->tabela}
 *
 * @package  A1
 * @author   {$usunome} <{$usuemail}>
 * @license  GNU siminc.cultura.gov.br
 * @version  Release: {$data}
 */

\n
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

    public function gerar($pkData, $fkData)
    {
        $this->nomeClasse = ucFirst($this->tabela);

        if (!$arquivo = fopen(APPRAIZ . "www/gerador/arquivos_gerados/{$this->tabela}{$this->extensao}", "w+")) {
            return false;
        }

        $classe = $this->getCodigo($pkData, $fkData);

        if (!fwrite($arquivo, $classe)) {
            echo "Erro ao escrever no arquivo";
        } else {
            echo "Classe <b>{$this->tabela}</b> criada com sucesso.<br>";
        }
        fclose($arquivo);
    }


    public function getCodigo($pkData, $fkData)
    {
        $tabela = ucFirst($this->tabela);
        $codigo = <<<PHP
<?php
include_once APPRAIZ .'{$this->schema}/classes/{$this->prefixoClasse}{$tabela}{$this->extensao}';

\$model{$tabela} = new {$tabela}(\$_REQUEST['{$pkData[0]['column_name']}']);

// -- Mensagens para a interface do usuário
\$fm = new Simec_Helper_FlashMessage('demandasfies/demandas');
switch (\$_REQUEST['action']) {
	case 'salvar':

		\$model{$tabela}->popularDadosObjeto();
		\$camposNulos = array();

		\$sucesso = \$model{$tabela}->salvar(null, null, \$camposNulos);
		\$modelDemanda->commit();

		\$msg = \$sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
		\$tipo = \$sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

		\$url .= '&dmdid=' . \$model{$tabela}->{$pkData[0]['column_name']};

		ob_clean();
		\$fm->addMensagem(\$msg, \$tipo);
		header("Location: {\$url}");

		die;
}

include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row">
    <div class="col-lg-12">
        <div class="page-header">
            <h3 id="forms"><?php echo \$model{$tabela}->{$pkData[0]['column_name']} ? 'Código: ' . \$model{$tabela}->{$pkData[0]['column_name']} : 'Novo'; ?></h3>
        </div>
    </div>
</div>
<div class="row">
    <div class="row col-md-7">
        <form id="form-save" method="post" class="form-horizontal">
            <input name="action" type="hidden" value="salvar">
            <input name="{$pkData[0]['column_name']}" id="{$pkData[0]['column_name']}" type="hidden" value="<?php echo \$model{$tabela}->{$pkData[0]['column_name']}; ?>">
            
            <div class="well">
PHP;
        foreach ($this->atributos as $srAtributo) {

            $required = 'NO' == $srAtributo['is_nullable'] ? 'required="required"': '';
            ver($srAtributo);

            $codigo .= <<<PHP
                
                <div class="form-group">
                    <label for="{$srAtributo['column_name']}" class="col-lg-4 col-md-4 control-label">{$srAtributo['column_name']}:</label>
                    <div class="col-lg-8 col-md-8">
                        <input type="text" id="{$srAtributo['column_name']}" name="{$srAtributo['column_name']}" class="form-control" value="<?php echo \$model{$tabela}->{$srAtributo['column_name']}; ?>" $required />
                    </div>                
                </div>
PHP;
        }

        $codigo .= <<<PHP
            
            </div>
            <div>
                <button title="Salvar" class="btn btn-success" id="btn-salvar" type="button"><span class="glyphicon glyphicon-thumbs-up"></span> Salvar </button>
                <a title="Voltar" class="btn btn-danger" href="/demandasfies/demandasfies.php?modulo=inicio&acao=C"><span class="glyphicon glyphicon-hand-left"></span> Voltar</a>
            </div>            
        </form>
    </div>
</div>
PHP;



        return $codigo;
    }
}
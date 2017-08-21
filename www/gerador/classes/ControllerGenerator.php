<?php

/**
 * Created by PhpStorm.
 * User: juniosantos
 * Date: 06/10/2015
 * Time: 11:40
 */
class ControllerGenerator
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

    public function gerar($pkData, $fkData)
    {
        $this->nomeClasse = ucFirst($this->tabela);

        if (!$arquivo = fopen(APPRAIZ . "www/gerador/arquivos_gerados/controller/{$this->tabela}{$this->extensao}", "w+")) {
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
        $tabela = ucFirst(str_replace(['_'], [''], $this->tabela));

        $nulos = '';
        $ano = $status = false;

        foreach ($this->atributos as $srAtributo) {
            if($srAtributo['is_nullable'] == 'YES'){
                $nulos .= "'" . $srAtributo['column_name'] . "', ";
            }
            if($srAtributo['column_name'] == 'prsano'){
                $ano = '$m' . $tabela . '->' . $srAtributo['column_name'] . ' = $m' . $tabela . '->' . $srAtributo['column_name'] . ' ? $m' . $tabela . '->' . $srAtributo['column_name'] . ' : $_SESSION[\'exercicio\'];
                ';
            }
            if(false !== strpos($srAtributo['column_name'], 'status')){
                $status = true;
            }
        }

        $codigo = <<<PHP
<?php

class {$this->controller}
{
    public function salvar(\$dados)
    {
        \$url = '?modulo=apoio/arquivo&acao=A';

        try {
            \$m{$tabela} = new {$this->model}(\$dados['{$pkData[0]['column_name']}']);
            \$m{$tabela}->popularDadosObjeto(\$dados);
            {$ano}
            \$m{$tabela}->salvar(null, null, [{$nulos}]);
            \$m{$tabela}->commit();
            simec_redirecionar(\$url, 'success');
        } catch (Exception \$e){
            \$m{$tabela}->rollback();
            simec_redirecionar(\$url, 'error');
        }
    } //end salvar()
    
    public function excluir(\${$pkData[0]['column_name']})
    {
        \$url = '?modulo=apoio/arquivo&acao=A';

        try {
            \$m{$tabela} = new {$this->model}(\${$pkData[0]['column_name']});
            
            if(\$m{$tabela}->existeVinculo()){
                \$mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                \$url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . \$m{$tabela}->{$pkData[0]['column_name']};
                simec_redirecionar(\$url, 'error', \$mensagem);
            }            
            
            \$m{$tabela}->status = 'I';
            \$m{$tabela}->salvar();
            \$m{$tabela}->commit();
            simec_redirecionar(\$url, 'success');
        } catch (Exception \$e){
            \$m{$tabela}->rollback();
            simec_redirecionar(\$url, 'error');
        }
    } //end salvar()
}            
PHP;
        return $codigo;
    }
}
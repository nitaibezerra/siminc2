<?php

class generatorModel
{

    public function generate($schema, $table, $entity)
    {
        
        
        $htmlEntity = '';
        foreach ($entity as $column) {
            if (strstr($column['constraint_name'], 'pk')){
                $column['constraint_name'] = 'pk';
            } else if(strstr($column['constraint_name'], 'fk')){
                $column['constraint_name'] = 'fk';
            }
            
            $htmlEntity .= '
        $this->entity[\'' . $column['column_name'] . '\'] = array( \'value\' => \'\' , \'type\' => \'' . $column['data_type'] . '\' ,  \'is_null\' => \'' . $column['is_nullable'] . '\' , \'maximum\' => \'' . $column['character_maximum_length'] . '\' , \'contraint\' => \'' . $column['constraint_name'] . '\');';
        }

$htmlModel = '<?php
class Model_' . ucFirst($table) . ' extends Abstract_Model
{
    
    /**
     * Nome do schema
     * @var string
     */
    protected $_schema = \'' . $schema . '\';

    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = \'' . $table . '\';

    /**
     * Entidade
     * @var string / array
     */
    public $entity = array();

    /**
     * Montando a entidade
     * 
     */
    public function __construct($commit = true)
    {
        parent::__construct($commit);
        ' . $htmlEntity . '
    }
}
';

        $files = APPRAIZ . "www/gerador/classes/";
        if (!file_exists($files))
        {
            if (mkdir($files, 0777))
            {
                $msg[] = array('status' => '1', 'msg' => 'Pasta de arquivos criada com sucesso!');
            } else
            {
                $msg[] = array('status' => '0', 'msg' => 'Não pode ser criado a pasta de arquivos!');
            }
        }

        $schemaFile = $files . $schema;
        if (!file_exists($schemaFile))
        {
            if (mkdir($schemaFile, 0777))
            {
                $msg[] = array('status' => '1', 'msg' => "Pasta {$schema} criada com sucesso!");
            } else
            {
                $msg[] = array('status' => '0', 'msg' => "Não pode ser criado a pasta '{$schema}'!");
            }
        }

        $modelsFile = $files . $schema . "/models/";
        if (!file_exists($modelsFile))
        {
            if (mkdir($modelsFile, 0777))
            {
                $msg[] = array('status' => '1', 'msg' => "Pasta 'models' criada com sucesso!");
            } else
            {
                $msg[] = array('status' => '0', 'msg' => "Não pode ser criado a pasta 'models'!");
            }
        }

        $tablePathFile = $modelsFile . ucFirst($table) . '.php';
        if ($tableFile = fopen($tablePathFile, "w+"))
        {
            $msg[] = array('status' => '1', 'msg' => "Arquivo {$table}.php criada com sucesso!");
//            $teste = chmod($tablePathFile, '777');
            // Dando permissao no arquivo
            exec("chmod -R 777 $tablePathFile");
            
        } else
        {
            $msg[] = array('status' => '0', 'msg' => "Não pode ser criado o arquivo '{$table}.php'!");
        }

        
        fwrite($tableFile, $htmlModel);
        
//        $teste = exec("chmod -R 777 $tableFile");
        
//        ver($teste, $tableFile, $html , $msg , d);
//        exit;
        

        return $msg;
        
        
    }

}
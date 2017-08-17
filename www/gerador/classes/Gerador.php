<?php

/**
 * Created by PhpStorm.
 * User: juniosantos
 * Date: 06/10/2015
 * Time: 11:40
 */
class Gerador
{

    public $stSchema;
    public $db;

    function __construct()
    {
        $this->db = new cls_banco();
    }

    public function getTables()
    {

        $sql = "SELECT col.table_name, col.column_name, col.is_nullable, col.data_type, col.character_maximum_length , kcu.constraint_name
	FROM information_schema.columns col

	LEFT JOIN information_schema.table_constraints tc
		ON tc.table_catalog = col.table_catalog
		AND tc.table_schema = col.table_schema
		AND tc.table_name = col.table_name
		AND tc.constraint_type = 'PRIMARY KEY'
	LEFT JOIN information_schema.key_column_usage kcu
		ON kcu.table_catalog = tc.table_catalog
		AND kcu.table_schema = tc.table_schema
		AND kcu.table_name = tc.table_name
		AND kcu.column_name = col.column_name

	WHERE col.table_schema = '%s'
	AND col.table_name in (%s)
";
        $sql = sprintf(
            $sql,
            $this->stSchema,
            "'" . implode("', '", $this->stTabela) . "'"
        );

        $data = $this->db->carregar($sql);
        $arTabelas = array();
        foreach ($data as $arResultado) {
            $table_name = $arResultado["table_name"];
            unset($arResultado["table_name"]);
            $arTabelas[$table_name][] = $arResultado;
        }
        return $arTabelas;
    }

    public function getPrimaryKeys($stTabela)
    {
        //foreach
        $sql = " SELECT tc.table_schema, tc.table_name, kc.column_name
                    FROM information_schema.table_constraints tc
                      JOIN information_schema.key_column_usage kc ON (kc.table_name = tc.table_name AND kc.table_schema = tc.table_schema)
                    WHERE tc.constraint_type = 'PRIMARY KEY'
                    AND kc.position_in_unique_constraint IS NULL
                    AND tc.table_schema = '%s'
                    AND tc.table_name = '%s'
                    ORDER BY tc.table_schema, tc.table_name, kc.position_in_unique_constraint
";
        $sql = sprintf($sql, $this->stSchema, $stTabela);
        $pkData = $this->db->carregar($sql);
        return is_array($pkData) ? $pkData : array();
    }

    public function getForeignKeys($stTabela)
    {
        $sql = "SELECT r.conname, pg_catalog.pg_get_constraintdef(r.oid, true) as condef FROM pg_catalog.pg_constraint r WHERE r.conrelid = '{$this->stSchema}.{$stTabela}'::regclass AND r.contype = 'f'";
        $fkData = $this->db->carregar($sql);
        return is_array($fkData) ? $fkData : array();
    }

    public function getAttributesFull($stTabela){
        $sqlGetEntity = "SELECT col.column_name, col.is_nullable, col.data_type, col.character_maximum_length, kcu.constraint_name
                FROM information_schema.columns col
                LEFT JOIN information_schema.table_constraints tc
                        ON tc.table_catalog = col.table_catalog
                        AND tc.table_schema = col.table_schema
                        AND tc.table_name = col.table_name
                        AND tc.constraint_type = 'PRIMARY KEY'
                LEFT JOIN information_schema.key_column_usage kcu
                        ON kcu.table_catalog = tc.table_catalog
                        AND kcu.table_schema = tc.table_schema
                        AND kcu.table_name = tc.table_name
                        AND kcu.column_name = col.column_name
                WHERE col.table_schema = '{$this->stSchema}'
                AND col.table_name = '{$stTabela}'";

        $entity = $this->db->carregar($sqlGetEntity);
        return is_array($entity) ? $entity : array();
    }

    public function gerarArquivos($stPrefixoClasse, $stExtensao = '.inc')
    {
        $tabelas = $this->getTables();
        foreach ($tabelas as $tabela => $atributos){
            $pkData = $this->getPrimaryKeys($tabela);
            $fkData = $this->getForeignKeys($tabela);

            $controller = ucfirst($pkData[0]['table_schema']) . '_Controller_' . ucfirst(str_replace(['_'], [''], $pkData[0]['table_name']));
            $model = ucfirst($pkData[0]['table_schema']) . '_Model_' . ucfirst(str_replace(['_'], [''], $pkData[0]['table_name']));

            $modelGenerator =  new ModelGenerator(array('prefixoClasse'=>$stPrefixoClasse, 'extensao'=> $stExtensao, 'tabela'=> $tabela, 'atributos'=> $atributos, 'schema'=> $this->stSchema));
            $modelGenerator->gerarModel($pkData, $fkData);

            $formGenerator =  new FormGenerator(array('controller'=>$controller, 'model'=>$model, 'extensao'=> $stExtensao, 'tabela'=> $tabela, 'atributos'=> $atributos, 'schema'=> $this->stSchema));
            $formGenerator->gerar($pkData, $fkData);

            $controllerGenerator =  new ControllerGenerator(array('controller'=>$controller, 'model'=>$model, 'extensao'=> $stExtensao, 'tabela'=> $tabela, 'atributos'=> $atributos, 'schema'=> $this->stSchema));
            $controllerGenerator->gerar($pkData, $fkData);
        }
    }
}
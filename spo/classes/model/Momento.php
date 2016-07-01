<?php
/**
 * Classe de mapeamento da entidade wssof.ws_momentosdto.
 *
 * $Id: Momento.php 100919 2015-08-06 19:01:37Z maykelbraz $
 * @filesource
 */

/**
 * Mapeamento da entidade wssof.ws_momentosdto.
 *
 * @see Modelo
 */
class Spo_Model_Momento extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'wssof.ws_momentosdto';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
    );

    /**
     * @var string[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * @var mixed[] Atributos
     */
    protected $arAtributos = array(
        'codigomomento' => null,
        'datahoraalteracao' => null,
        'descricao' => null,
        'snativo' => null,
        'dataultimaatualizacao' => null,
    );

    public static function queryCombo()
    {
        $sql = <<<DML
SELECT codigomomento AS codigo,
       codigomomento || ' - ' || descricao AS descricao
  FROM wssof.ws_momentosdto m
  WHERE m.snativo = '1'
  ORDER BY codigomomento::int
DML;
        return $sql;
    }
}
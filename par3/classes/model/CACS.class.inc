<?php

class Par3_Model_CACS extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "integracao.cacs";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array();

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
		'co_mun_ibge_conselho' => null,
	    'esf_adm_conselho' => null,
	    'sit_mandato' => null,
	    'uf_conselho' => null,
	    'mun_conselho' => null,
	    'email_conselho' => null,
	    'cpf_conselheiro' => null,
	    'no_conselheiro' => null,
	    'email_conselheiro' => null,
	    'sit_conselheiro' => null,
	    'ds_segmento' => null,
	    'tp_membro' => null,
	    'ds_funcao' => null
	);
    
    public function __construct() 
    {
    	$GLOBALS['nome_bd'] = '';
    	$GLOBALS['servidor_bd'] = '';
    	$GLOBALS['porta_bd'] = '';
    	$GLOBALS['usuario_db'] = '';
    	$GLOBALS['senha_bd'] = '';
    }
    
	public function listarConselheiros($muncod)
	{
		$sql = "SELECT * 
			    FROM {$this->stNomeTabela}
			    WHERE co_mun_ibge_conselho = '{$muncod}'";

		return $this->carregar($sql);
	}
}

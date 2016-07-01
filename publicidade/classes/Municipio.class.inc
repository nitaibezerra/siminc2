<?php

/**
 * Modelo de Municípios
 * @author Alysson Rafael
 */
class Municipio extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'territorios.municipio';

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('muncod,estuf');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos	= array(
		'miccod' => null,
		'mescod' => null,
		'mundescricao' => null,
		'munprocesso' => null,
		'muncodcompleto' => null,
		'munmedlat' => null,
		'munmedlog' => null,
		'munhemis' => null,
		'munaltitude' => null,
		'munmedarea' => null,
		'muncepmenor' => null,
		'muncepmaior' => null,
		'munmedraio' => null,
		'munmerid' => null,
		'muncodsiafi' => null,
		'munpopulacao' => null,
		'munpopulacaohomem' => null,
		'munpopulacaomulher' => null,
		'munpopulacaourbana' => null,
		'munpopulacaorural' => null
	);
	
	
	/**
     * Gerar a combo de município
     * @name monta_combo_municipio
     * @author Alysson Rafael
     * @access public
     * @param string $estuf       - Identificador de estado(uf)
     * @param int $muncod         - Identificador de município
     * @param string $obrigatorio - Se a combo vai ou não exibir a imagem de obrigatório(S ou N)
     * @return void
     */
	public function monta_combo_municipio($estuf,$muncod='',$obrigatorio='N'){
		
		if(!empty($estuf)){

			$estuf = pg_escape_string($estuf);
			
			$where = "WHERE estuf='{$estuf}' ";
		
			$sql = "SELECT muncod AS codigo,mundescricao AS descricao FROM $this->stNomeTabela {$where} ORDER BY mundescricao ";

			$resultado = $this->carregar($sql);
		}
		else{
			$resultado = array();
		}
		
		$this->monta_combo('muncod',$resultado,'S','Selecione...','','','','',$obrigatorio,'muncod','',$muncod);
	}
	
	
	
	

	
}
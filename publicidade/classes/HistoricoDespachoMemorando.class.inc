<?php

/**
 * Modelo de Histórico de despacho no relatório memorando
 * @author Alysson Rafael
 */
class HistoricoDespachoMemorando extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'publicidade.historicodespachomemorando';

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('hdmid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'padnumero' => null,
		'dataregistro' => null,
		'despacho' => null
	);
	
	/**
     * Carrega o último despacho gravado
     * 
     * @name carrega_ultimo_despacho
     * @author Alysson Rafael
     * @access public
     * @return array
     */
	public function carrega_ultimo_despacho(){
		$sql = "SELECT * FROM $this->stNomeTabela ORDER BY dataregistro DESC LIMIT 1 ";
		return $this->pegaLinha($sql);
	}
	
	/**
     * Insere os dados de despacho do memorando
     * 
     * @name salvar
     * @author Alysson Rafael
     * @param array $get - Array com os dados do form
     * @access public
     * @return int
     */
	public function salvar($get){
		$padnumero = str_replace('/', '', pg_escape_string($get['padnumero']));
		$despacho = pg_escape_string($get['despacho']);
		$data = date('Y-m-d H:i:s');
		
		$sql = "INSERT INTO $this->stNomeTabela(padnumero,dataregistro,despacho)
		        VALUES('{$padnumero}','{$data}','{$despacho}') ";
		
		return $this->pegaUm($sql);
	}

	
}
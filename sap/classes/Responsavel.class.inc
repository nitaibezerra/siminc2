<?php

/**
 * Modelo de Responsável
 * @author Alysson Rafael
 */
class Responsavel extends Modelo{
	
	
	/**
	 * Nome da tabela especificada
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "siape.vwservidorativo";

	/**
     * Método responsável por carregar um responsável pela matrícula
     * @name carregaResponsavel
     * @author Alysson Rafael
     * @access public
     * @param int $nu_matricula_siape - Matrícula a ser pesquisada
     * @return array
     */
	function carregaResponsavel($nu_matricula_siape){
		
		$nu_matricula_siape = addslashes($nu_matricula_siape);
		
		$sql = "SELECT no_servidor,co_uorg_lotacao_servidor FROM $this->stNomeTabela WHERE nu_matricula_siape='".$nu_matricula_siape."' ";
		$resultado = $this->pegaLinha($sql);
		
		return $resultado;
		
	}
	
}
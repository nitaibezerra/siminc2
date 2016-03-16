<?php

/**
 * Modelo de Histórico dos Cadastros
 * @author Alysson Rafael
 */
class HistoricoCadastro extends Modelo{
	
	
	/**
	* Nome da tabela especificada
	* @name $stNomeTabela
	* @var string
	* @access protected
	*/
	protected $stNomeTabela = "sap.historico_cadastro";

	/**
	* Chave primária
	* @name $arChavePrimaria
	* @var array
	* @access protected
	*/
	protected $arChavePrimaria = array('hicid');

	/**
	* Atributos da Tabela
	* @name $arAtributos
	* @var array
	* @access protected
	*/
	protected $arAtributos = array(
		'hiccpf' => null,
		'hicdata' => null,
		'hicdsc' => null,
		'hicalteracao' => null
	);
	
	
	/**
     * Registrar histórico dos cadastros
     * @name salvarHistorico
     * @author Alysson Rafael
     * @access public
     * @param int $cpf - CPF do usuário que realizou o cadastro
     * @param string $descricao - Descrição do cadastro realizado
     * @param int $id - Id utilizado no cadastro
     * @return void
     */
	public function salvarHistorico($cpf,$descricao,$id){
		$sqlHis = "INSERT INTO $this->stNomeTabela(hiccpf,hicdata,hicdsc,hicalteracao) ";
		$sqlHis .= "VALUES('".$cpf."','".date('Y-m-d')."','".$descricao."',$id) ";
		$sqlHis .= " returning hicid ";
       	$hicid = $this->pegaUm($sqlHis);
       	$this->commit();
	}
	
	
}
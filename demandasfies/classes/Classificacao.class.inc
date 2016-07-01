<?php

class Classificacao extends Modelo
{

	/**
	 * Nome da tabela especificada
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "demandasfies.classificacao";

	/**
	 * Chave primaria.
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array("claid");

	/**
	 * Atributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array('claid' => null, 'clanome' => null, 'clastatus' => null, 'clatipo' => null,);

	public static function getDescricaoById($claid){
		$classificacao = new Classificacao();
		$sql = "select claid as codigo, clanome as descricao from demandasfies.classificacao
		        where clastatus = 'A'
		        and clatipo = 'G'
		        and claid = {$claid}
		        order by descricao";

		$dados = $classificacao->carregar($sql);
		$dados = $dados ? $dados : array();
		return $dados[0]['descricao'];
	}
}
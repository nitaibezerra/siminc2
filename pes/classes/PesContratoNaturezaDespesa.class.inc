<?php

class PesContratoNaturezaDespesa extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pes.pescontratonaturezadespesa";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("cndcodigo");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'cndcodigo' => null,
									  	'entcodigo' => null,
									  	'tidcodigo' => null,
									  	'ddgcodigo' => null,
									  	'unicodigo' => null,
									  	'natano' => null,
									  	'natcodigo' => null,
									  	'cndtitulo' => null,
									  	'cndobservacao' => null,
									  	'cndusunome' => null,
									  	'cndano' => null,
									  	'cndunidadeoutros' => null,
									  	'cnddatacriacao' => null,
									  	'cndusucpfcriacao' => null,
									  	'cndsissiglacriacao' => null,
									  	'cnddataalteracao' => null,
									  	'cndusucpfalteracao' => null,
									  	'cndsissiglaalteracao' => null,
									  );
}
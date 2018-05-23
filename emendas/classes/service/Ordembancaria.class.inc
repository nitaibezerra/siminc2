<?php
	
class Ordembancaria extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "emenda.ordembancaria";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "orbid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'orbid' => null,
										'exfid' => null,
										'orbmesparcela' => null,
										'orbanoparcela' => null,
										'orbvalorparcela' => null,
										'orbnumsolicitacao' => null,
										'spgcodigo' => null,
										'orbnumordembancaria' => null,
										'orbvalorpagamento' => null,
										'orbdataemissao' => null,
										'orbdatapagamento' => null,
										'usucpf' => null,
										'orbdatainclusao' => null,
										'orbdataalteracao' => null     
									  );
}
<?php

class DeclaracaoConformidade extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.declaracaoconformidade";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "dlcid" );
    
    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'dlcid'         => null,
									  	'usucpf'        => null,
									  	'oblid'      	=> null,
									  	'estuf'        	=> null,
									  	'muncod'     	=> null,
									  	'dlcnumero' 	=> null,
									  	'dlcdtregistro' => null
									  );
    
	public function carregaPorObrid( $obrid ){
		$sql = "SELECT
					dc.*
				FROM
					obras2.declaracaoconformidade dc
				JOIN obras2.obralicitacao ol ON ol.oblid = dc.oblid AND
												ol.oblstatus = 'A' AND
												ol.obrid = {$obrid}";
		$dados = $this->pegaLinha( $sql );
		$dados = ($dados ? $dados : array());
		$this->popularDadosObjeto( $dados );
		
		return $dados;
	}

	public function dadosUsuarioCadastrante( $dlcid ){
		if ( empty($dlcid) ){
			return array();
		}
		
		$sql = "SELECT
					u.usucpf,
					u.usunome
				FROM
					obras2.declaracaoconformidade dc
				JOIN seguranca.usuario u ON u.usucpf = dc.usucpf	
				WHERE
					dlcid = {$dlcid}";
		
		$dados = $this->pegaLinha( $sql );
		$dados = ( $dados ? $dados : array() );
		$dados['usucpf'] = formatar_cpf( $dados['usucpf'] ); 
		
		return $dados;
	}
}
<?php
	
class UsuarioResponsabilidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.usuarioresponsabilidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "rpuid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'rpuid' => null, 
									  	'agdid' => null, 
									  	'aevid' => null, 
									  	'usucpf' => null, 
									  	'rpustatus' => null, 
									  	'rpudata_inc' => null, 
									  	'pflcod' => null, 
									  	'rpudata_ini' => null, 
									  	'rpudata_fim' => null, 
									  );

	public function pegaRespAgenda( $usucpf = null, $pflcod = false ){
		$cpf = ($cpf ? $cpf : $_SESSION['usucpf']);
		
		$sql = "SELECT
					DISTINCT agdid
				FROM
					agenda.usuarioresponsabilidade
				WHERE
					rpustatus = 'A' AND 
					agdid IS NOT NULL AND
					usucpf = '{$cpf}'" . 
					($pflcod ? " AND pflcod = {$pflcod}" : "");
					
		$arAgdid = $this->carregarColuna( $sql );
		
		return ($arAgdid ? $arAgdid : array());					
	}
	
	public function pegaRespArea( $usucpf = null, $pflcod = false ){
		$cpf = ($cpf ? $cpf : $_SESSION['usucpf']);
		
		$sql = "SELECT
					DISTINCT aevid
				FROM
					agenda.usuarioresponsabilidade
				WHERE
					rpustatus = 'A' AND 
					aevid IS NOT NULL AND
					usucpf = '{$cpf}'" . 
					($pflcod ? " AND pflcod = {$pflcod}" : "");
					
		$arAevid = $this->carregarColuna( $sql );
		
		return ($arAevid ? $arAevid : array());					
	}
}

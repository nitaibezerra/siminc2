<?php


class FormaExecucao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.formaexecucao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "frmid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'frmid' => null, 
									  	'frmdsc' => null, 
									  	'frmtipo' => null, 
									  	'frmbrasilpro' => null, 
									  );
									  
	public function antesExcluir( $id = null ){ 
		
		$sql = "select count(*) from cte.subacaoindicador where frmid = $this->frmid";
		
		if( $this->pegaUm( $sql ) ){
			echo "<script>alert( 'Não foi possível excluir, pois há ligação com uma subação' );</script>";
			return false;
		}
		return true;
		
	}
									  
	function recuperarTipos(){

		$sql = "select  distinct frmtipo as codigo,
					case 
						when ( frmtipo = 'E' ) then 'Estadual'
						else 'Municipal'
					end as descricao
				from cte.formaexecucao";
		
		return $this->carregar( $sql );
	}
	
	function recuperarListagem(){

		$sql = "select
				'<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar Forma de Execução\" onclick=\"altera_forma_execucao('||frmid||')\">&nbsp;&nbsp;&nbsp;
				 <img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir Forma de Execução\" onclick=\"excluir_forma_execucao('||frmid||')\">' 
				as acao, frmdsc,
				case 
					when ( frmtipo = 'E' ) then 'Estadual'
					else 'Municipal'
				end as frmtipo,
				case 
					when ( frmbrasilpro = true ) then 'Sim'
					else 'Não'
				end as frmbrasilpro
				from cte.formaexecucao
				order by frmbrasilpro, frmdsc ";

		return $this->carregar( $sql );
		
	}
}
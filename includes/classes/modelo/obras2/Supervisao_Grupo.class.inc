<?php
	
class Supervisao_Grupo extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.supervisao_grupo";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sgrid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'sgrid' => null, 
									  	'sgrnum' => null, 
									  	'sgrqtd' => null, 
									  	'sgrnome' => null, 
									  	'sgrstatus' => null, 
									  );

	public function listaCombo(){
		$join = array();
		
		if( !possui_perfil(Array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_CONTRATO_SUPERVISAO_MEC)) ){
			
			$usuarioResp = new UsuarioResponsabilidade();
			$arEntidEmpresa = $usuarioResp->pegaEmpresaPermitida( $_SESSION['usucpf'] );
			$arEntidEmpresa = ($arEntidEmpresa ? $arEntidEmpresa : array(0));
										
			$join['usuarioresponsabilidade'] = "JOIN obras2.supervisao_grupo_empresa sge ON sge.sgrid = sg.sgrid AND
																							sge.sgestatus = 'A' AND 
																						    sge.entid IN(" . implode(' AND ', $arEntidEmpresa) . ")";
		}
		
		$sql = "SELECT 
			   		sg.sgrid AS codigo, 
			   		sg.sgrnum || ' - ' || sg.sgrnome AS descricao 
			    FROM 
					obras2.supervisao_grupo sg
					" . ($join ? implode(" ", $join) : "") . "
				WHERE
					sgrstatus='A'
			    ORDER BY 
					sgrnum";
		$dados = $this->carregar($sql);
		
		return ($dados ? $dados : array());
	}
									  
}
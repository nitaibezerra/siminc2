<?php
	
class Agenda extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.agenda";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "agdid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'agdid' 	=> null, 
									  	'usucpf' 	=> null, 
									  	'agddsc' 	=> null, 
									  	'agdstatus' => null, 
									  	'agdnomeresp' => null, 
									  	'agdcargoresp' => null 
									  );

	public function listaSQL( Array $where = array() ){
		
		$arWhere = array();
		
		// Regra por usuarioresponsabilidade na agenda
		// Caso seja super usuário ou admnistrador pode ver todas as agendas
		
		if ( !($this->testa_superuser() || verificaPerfil( PERFIL_ADMINISTRADOR ) ) ){
			$userResp 		= new UsuarioResponsabilidade();
			$where['agdid'] = $userResp->pegaRespAgenda();
			$where['agdid'] = ($where['agdid'] ? $where['agdid'] : array());
			
			$arAevid = $userResp->pegaRespArea();
			$arAevid = ($arAevid ? $arAevid : array());
			
			if ( $arAevid ){
				$arAgdidArea = $this->pegaAgendaPorArea( $arAevid );
				$where['agdid'] = array_merge( $where['agdid'], $arAgdidArea, (array) 2 );
			}
			$where['agdid'] = array_unique( $where['agdid'] );
		}
		
		switch (true){
			case isset($where['agddsc']):
				$arWhere[] = "agddsc ILIKE '%{$where['agddsc']}%'"; 
				break;
			case $where['agdid']:
				$arWhere[] = "agdid IN (" . implode(", ", $where['agdid']) . ")"; 
				break;
		}
		
		$acao = "'<center>
				<img
				  	id=\"edit_' || agdid || '\"
 					align=\"absmiddle\"
 					src=\"/imagens/schedule.png\"
 					style=\"cursor: pointer\"
 					onclick=\"javascript: andamentoAgenda(\'' || agdid || '\');\"
 					title=\"Andamento da Agenda (calendário)\">&nbsp;
				<img
 					align=\"absmiddle\"
 					src=\"/imagens/principal.gif\"
 					style=\"cursor: pointer\"
 					onclick=\"javascript: eventoAgenda(\'' || agdid || '\');\"
 					title=\"Eventos da Agenda\">";
		
		if ( verificaPerfil( array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR, PERFIL_GESTOR) ) || $where['agdid'] ){
				if ( $arAgdidArea ){
					$acao .= "' || 
							  CASE
							  	WHEN ( agdid != " . implode(" AND agdid != ", $arAgdidArea) . " ) THEN	
									  '&nbsp;
									  <img
									  	id=\"edit_' || agdid || '\"
					 					align=\"absmiddle\"
					 					src=\"/imagens/alterar.gif\"
					 					style=\"cursor: pointer\"
					 					onclick=\"javascript: alterarAgenda(\'' || agdid || '\');\"
					 					title=\"Alterar Cadastro Agenda\">&nbsp;
					 				  <img
									  	id=\"exc_' || agdid || '\"
					 					align=\"absmiddle\"
					 					src=\"/imagens/excluir.gif\"
					 					style=\"cursor: pointer\"
					 					onclick=\"javascript: excluirAgenda(\'' || agdid || '\');\"
					 					title=\"Excluir Cadastro Agenda\">'
			 				  	ELSE
			 				  		  '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
			 				  END || '";
				}else{
					$acao .= "&nbsp;
							  <img
							  	id=\"edit_' || agdid || '\"
			 					align=\"absmiddle\"
			 					src=\"/imagens/alterar.gif\"
			 					style=\"cursor: pointer\"
			 					onclick=\"javascript: alterarAgenda(\'' || agdid || '\');\"
			 					title=\"Alterar Cadastro Agenda\">&nbsp;
			 				  <img
							  	id=\"apag_' || agdid || '\"
			 					align=\"absmiddle\"
			 					src=\"/imagens/excluir.gif\"
			 					style=\"cursor: pointer\"
			 					onclick=\"javascript: excluirAgenda(\'' || agdid || '\');\"
			 					title=\"Excluir Cadastro Agenda\">";
				}					
		}
		
		$acao .= "</center>'";
				
		$sql = "SELECT
					$acao AS acao,
					agddsc
				FROM
					agenda.agenda
				WHERE
					agdstatus = 'A'" . 
				(count( $arWhere ) ? " AND " . implode(" AND ", $arWhere) : "");
//		dbg($sql, d);
		return $sql;
	}
	
	public function listaCombo(){
		$sql = "SELECT
					agdid AS codigo,
					agddsc AS descricao
				FROM
					agenda.agenda
				WHERE
					agdstatus = 'A'"; 
		
		$dados = $this->carregar($sql);
		
		return (is_array($dados) ? $dados : array());
	}
	
	public function carregaResponsavel( $agdid ){
		$sql = "SELECT
					agdnomeresp,
					agdcargoresp
				FROM
					agenda.agenda
				WHERE
					agdstatus = 'A' AND
					agdid = {$agdid}";
		
		$dados = $this->pegaLinha( $sql );
		
		return ($dados ? $dados : array()); 
	}
	
	public function pegaAgendaPorArea( array $aevid ){
		$sql = "SELECT
					DISTINCT e.agdid
				FROM
					agenda.evento e
				JOIN agenda.eventoarea ea ON ea.eveid = e.eveid AND
											 ea.evastatus = 'A' AND
											 ea.aevid IN (" . implode(", ", $aevid) . ") 											 							 
				WHERE
					e.evestatus = 'A'";	

		$dados = $this->carregarColuna( $sql );
		
		return ($dados ? $dados : array()); 
	}
	
	/*
	 * Este método devolve somente os AGDID das agendas que o usuário tem permissão total, ou seja, pode alterar os dados cadastrais
	 * da agenda. 
	 * Carateriza-se pela responsabilidade na agenda por meio do perfil GESTOR MEC
	 */
	public function possuiPermissaoTotalAgenda( $agdid ){
		$return = true;
		
		if ( !($this->testa_superuser() || verificaPerfil( PERFIL_ADMINISTRADOR )) ){
			$userResp 		= new UsuarioResponsabilidade();
			$where['agdid'] = $userResp->pegaRespAgenda();
			
			$return = (in_array($agdid, $where['agdid']) ? true : false);
		}

		return $return;
	}
	
	/*
	 * Este método devolve os AGDID das agendas que o usuário tem permissão total ou parcial, ou seja, NÃO deve ser usado na validação
	 * dos dados cadastrais da agenda.
	 * Carateriza-se pela responsabilidade na agenda e na área por meio do perfil GESTOR MEC e UNIDADE MEC
	 */
	public function possuiPermissaoAgenda( $agdid ){
		$return = true;
		
		if ( !($this->testa_superuser() || verificaPerfil( PERFIL_ADMINISTRADOR )) ){
			$userResp 		= new UsuarioResponsabilidade();
			$where['agdid'] = $userResp->pegaRespAgenda();
			
			$arAevid = $userResp->pegaRespArea();
			if ( $arAevid ){
				$arAgdidArea 	= $this->pegaAgendaPorArea( $arAevid );
				$where['agdid'] = array_merge( $where['agdid'], $arAgdidArea, (array) 2 );
			}
			$where['agdid'] = array_unique( $where['agdid'] );
			
			$return = (in_array($agdid, $where['agdid']) ? true : false);
		}

		return $return;
	}
}
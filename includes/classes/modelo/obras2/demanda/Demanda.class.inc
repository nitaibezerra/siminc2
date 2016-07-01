<?php
	
class Demanda extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.demanda";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "demid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'demid' => null, 
									  	'demid_pai' => null,  
									  	'entid' => null,
									  	'entid_campus' => null,  
									  	'orgid' => null, 
									  	'endid' => null, 
									  	'dclid' => null, 
									  	'dtoid' => null, 
									  	'dspid' => null, 
									  	'docid' => null, 
									  	'demnome' => null, 
									  	'demdesc' => null, 
									  	'demarea' => null, 
									  	'demvalortotal_prev' => null, 
									  	'demano' => null, 
									  	'demqtdalunos' => null, 
									  	'demstatus' => null, 
									  );
    
    public function listaSql( Array $param = array(), $tipoSelecao = null ){
    	$where = array();
    	$join  = array();
    	 
    	$param['orgid'] = (isset($param['orgid']) ? $param['orgid'] : $_SESSION['obras2']['orgid']);
    
    	if ( $param['orgid'] ){
    		$param['orgid'] = (array) $param['orgid'];
    		$where[] = "d.orgid IN(" . implode(", ", $param['orgid']) . ")";
    	}
    
    	if ( $param['dembuscatexto'] ){
            $dembuscatextoTmp = removeAcentos(str_replace("-"," ",$param['dembuscatexto']));
    		$where[] = " ( UPPER(public.removeacento(d.demnome)) ILIKE ('%" . $dembuscatextoTmp . "%') OR
    		d.demid::CHARACTER VARYING ILIKE UPPER('%" . $param['dembuscatexto'] . "%') ) ";
    	}

    	if ( $param['dclid'] ){
    		$param['dclid'] = (array) $param['dclid'];
    		$where[] = "d.dclid IN(" . implode(", ", $param['dclid']) . ")";
    	}
    
    	if ( $param['dtoid'] ){
    		$param['dtoid'] = (array) $param['dtoid'];
    		$where[] = "d.dtoid IN(" . implode(", ", $param['dtoid']) . ")";
    	}
    
    	if ( $param['dspid'] ){
    		$param['dspid'] = (array) $param['dspid'];
    		$where[] = "d.dspid IN(" . implode(", ", $param['dspid']) . ")";
    	}
    	
    	if ( $param['demarea_menor'] ){
    		$where[] = "d.demarea < " . str_replace(array(".", ","), array("", "."), $param['demarea_menor']);
    	}
    	
    	if ( $param['demarea_maior'] ){
    		$where[] = "d.demarea > " . str_replace(array(".", ","), array("", "."), $param['demarea_maior']);
    	}
    	
    	if ( $param['demano_menor'] ){
    		$where[] = "d.demano::integer < " . str_replace(array(".", ","), array("", "."), $param['demano_menor']);
    	}
    	
    	if ( $param['demano_maior'] ){
    		$where[] = "d.demano::integer > " . str_replace(array(".", ","), array("", "."), $param['demano_maior']);
    	}
    	
    	if ( $param['demvalortotal_prev_menor'] ){
    		$where[] = "d.demvalortotal_prev::integer < " . str_replace(array(".", ","), array("", "."), $param['demvalortotal_prev_menor']);
    	}
    	
    	if ( $param['demano_maior'] ){
    		$where[] = "d.demvalortotal_prev::integer > " . str_replace(array(".", ","), array("", "."), $param['demvalortotal_prev_maior']);
    	}
    	
    	if ( $param['demqtdalunos_menor'] ){
    		$where[] = "d.demqtdalunos::integer < " . str_replace(array(".", ","), array("", "."), $param['demqtdalunos_menor']);
    	}
    	
    	if ( $param['demqtdalunos_maior'] ){
    		$where[] = "d.demqtdalunos::integer > " . str_replace(array(".", ","), array("", "."), $param['demqtdalunos_maior']);
    	}
    
    	if ( $param['estuf'] ){
    		$param['estuf']   = (array) $param['estuf'];
    		$where[] 		  = "ede.estuf IN('" . implode("', '", $param['estuf']) . "')";
    		$join['endereco'] = "LEFT JOIN entidade.endereco ede ON ede.endid = o.endid AND ede.endstatus = 'A'";
    	}
    
    	if ( $param['muncod'] ){
    		$param['muncod']  = (array) $param['muncod'];
    		$where[] 		  = "ede.muncod IN('" . implode("', '", $param['muncod']) . "')";
    		$join['endereco'] = "LEFT JOIN entidade.endereco ede ON ede.endid = o.endid AND ede.endstatus = 'A'";
    	}
    
    	if ( $param['empesfera'] ){
    		$param['empesfera'] = (array) $param['empesfera'];
    		$where[] = "e.empesfera IN('" . implode("', '", $param['empesfera']) . "')";
    	}
    
    	if( !possui_perfil(Array(PFLCOD_SUPER_USUARIO)) ){
    		$arrEst = Array(PFLCOD_CADASTRADOR_INSTITUCIONAL,
    				PFLCOD_CONSULTA_ESTADUAL,
    				PFLCOD_EMPRESA_CONTRATADA);
    		$arrObr = Array(PFLCOD_EMPRESA_CONTRATADA,
    				PFLCOD_SUPERVISOR_UNIDADE);
    		$arrOrg = Array(PFLCOD_ADMINISTRADOR,
    				PFLCOD_CADASTRADOR_INSTITUCIONAL,
    				PFLCOD_CONSULTA_ESTADUAL,
    				PFLCOD_CONSULTA_TIPO_DE_ENSINO,
    				PFLCOD_SUPERVISOR_MEC,
    				PFLCOD_GESTOR_MEC);
    		$arrUni = Array(PFLCOD_AUDITOR_INTERNO,
    				PFLCOD_CADASTRADOR_INSTITUCIONAL,
    				PFLCOD_CONSULTA_UNIDADE,
    				PFLCOD_GESTOR_UNIDADE,
    				PFLCOD_SUPERVISOR_UNIDADE);
    			
    		$resp = Array();
    			
    		if(possui_perfil($arrEst)){
    			$resp[] = "urs.estuf = end.estuf ";
    		}
//     		if(possui_perfil($arrObr)){
//     			$resp[] = "JOIN obras2.usuarioresponsabilidade urs ON urs.obrid = obr.obrid ";
//     		}
    		if(possui_perfil($arrOrg)){
    			$resp[] = "urs.orgid = d.orgid ";
    		}
    		if(possui_perfil($arrUni)){
    			$resp[] = "urs.entid = d.entid ";
    		}
    			
    		if( count($resp) ){
    			$join['usuarioresponsabilidade'] = "INNER JOIN obras2.usuarioresponsabilidade urs ON ".implode(' AND ',$resp);
    		}
    	}
    
    	switch ( $tipoSelecao ){
//     		case 'vinculoContrato':
//     			$acao = "'<center>
// 			    			<input name=\"obrid[]\" id=\"obrid_' || o.obrid || '\" type=\"checkbox\" value=\"' || o.obrid || '\" onclick=\"vincularObra(this)\" >
// 			    			<input name=\"obr_dsc_' || o.obrid || '\" id=\"obr_dsc_' || o.obrid || '\" type=\"hidden\" value=\"' || o.obrdsc || '\">
// 			    			<input name=\"obr_emp_' || o.obrid || '\" id=\"obr_emp_' || o.obrid || '\" type=\"hidden\" value=\"' || COALESCE(e.empdsc, '') || '\">
// 		    			</center>'";
    
//     			$select = "	{$acao} AS acao,
//     			obrdsc,
//     			empdsc,
//     			clodsc,
//     			tobdesc,
//     			obrvalorprevisto";
//     			break;
    		default:
    			$acao = "'<center>
			    			<img
				    			align=\"absmiddle\"
				    			src=\"/imagens/alterar.gif\"
				    			style=\"cursor: pointer\"
				    			onclick=\"javascript: alterarDem(\'' || d.demid || '\');\"
				    			title=\"Alterar Obra\">
			    			<img
				    			align=\"absmiddle\"
				    			src=\"/imagens/excluir.gif\"
				    			style=\"cursor: pointer; margin-left: 3px;\"
				    			onclick=\"javascript: excluirDem(\'' || d.demid || '\');\"
				    			title=\"Excluir Empreendimento\">
		    			</center>'";
    
    			$select = "	{$acao} AS acao,
			    			demnome,
			    			dcldesc,
			    			dtodesc,
			    			demvalortotal_prev";
    		break;
    				
    	}
    
    	$sql = "SELECT
		    		{$select}
		    	FROM
		    		obras2.demanda d
		    	INNER JOIN obras2.orgao						org ON org.orgid = d.orgid
	    		LEFT  JOIN obras2.demanda_tipoobra			dto ON dto.dtoid = d.dtoid
		    	LEFT  JOIN obras2.demanda_classificacaoobra dcl ON dcl.dclid = d.dclid
		    	" . (count($join) ? implode(" ", $join) : "") . "
		    	WHERE
			    	d.demstatus = 'A'
			    	" . (count($where) ? " AND " . implode(' AND ',$where) : "") . "
		    	ORDER BY
		    		2";
    		
    	return $sql;
    }

    
	public function listaCombo( Array $param = array() ){
		$where = array();
		
		if ($param['orgid']){
			$where[] = "orgid = {$param['orgid']}";
		}
		
		if ($param['entid']){
			$where[] = "entid = {$param['entid']}";
		}
		
		if ($param['entid_campus']){
			$where[] = "entid_campus = {$param['entid_campus']}";
		}
		
		$sql = "SELECT
					demid AS codigo,
					demnome AS descricao
				FROM
					obras2.demanda
				WHERE
					demstatus = 'A'" . 
				(count($where) ? " AND " . implode(" AND ", $where) : "") . 
				"ORDER BY
					demnome;";
		$dados = $this->carregar($sql);
		
		return (is_array($dados) ? $dados : array());
	}
    
}
<?php
	
class ObrasArquivos extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.obras_arquivos";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "oarid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
                                        'oarid' => null, 
                                        'obrid' => null, 
                                        'arqid' => null, 
                                        'tpaid' => null, 
                                        'oardesc' => null, 
                                        'oardata' => null, 
                                        //'oartipo' => null, 
                                        'oarstatus' => null, 
                                        'oardtinclusao' => null, 
                                        'obrid_1' => null,
                                        'oarimp' => null
                                      );
    
    public function listaSql( Array $param = array(), $tipoSelecao = null ){
    
    	$where = array("oarstatus = 'A'");
    	
    	if( $param['obrid'] != '' ){
    		$where[] = "oa.obrid = '".$param['obrid']."' ";
    	}
    	
    	if( $param['tpaid'] != '' ){
    		$where[] = "ta.tpaid = '".$param['tpaid']."' ";
    	}
    	
    	if( $param['not(img)'] == true ){
    		$where[] = "(arqtipo != 'image/jpeg' AND arqtipo != 'image/gif' AND arqtipo != 'image/png')";
    	}
    	
    	if( $param['only(img)'] == true ){
    		$where[] = "(arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png')";
    	}
        
        if(isset($param['tpaid(IN)']) && $param['tpaid(IN)'] != ''){
            $where[] = "ta.tpaid IN (".$param['tpaid(IN)'].")";
        }
    	
//    	if( $param['oartipo'] != '' ){
//    		$where[] = "oartipo = '".$param['oartipo']."' ";
//    	}
        
        $perfil_de_nao_exclusao = possui_perfil(Array(PFLCOD_GESTOR_UNIDADE,PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_CALL_CENTER));
    	
    	if( $param['block_img_excluir'] !== true && !$perfil_de_nao_exclusao){
    		$img_excluir = "'<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/excluir_2.gif\"
                                    width=\"19px\"
                                    style=\"cursor: pointer; margin-left: 3px;\"
                                    id=\"' || oarid || '\"
                                    class=\"excluir\"
                                    title=\"Excluir Arquivo\">' ||

			 		CASE
		 				WHEN oarimp = true THEN
			 				'<img
			 					align=\"absmiddle\"
			 					src=\"/imagens/estrela.png\"
			 					style=\"cursor: pointer; margin-left: 3px;\"
			 					onclick=\"javascript: retirarImportancia(\'' || oa.oarid || '\');\"
			 					title=\"Retirar importancia\">'
		 				ELSE
			 				'<img
			 					align=\"absmiddle\"
			 					src=\"/imagens/estrela1.png\"
			 				    style=\"cursor: pointer; margin-left: 3px;\"
			 					onclick=\"javascript: tornarImportante(\'' || oa.oarid || '\');\"
			 					title=\"Marcar como importante \">'
			 		END ||
		 			  '</center>";
    	}else{
    		$img_excluir = "'";
    	}
    	
    	$sql = "SELECT
                        '<center>
                        <img
                            align=\"absmiddle\"
                            src=\"/imagens/icone_lupa.png\"
                            style=\"cursor: pointer\"
                            id=\"' || oa.arqid || '\"
                            class=\"download\"
                            title=\"Baixar Arquivo\">'
                            $img_excluir
                        ' as acao,
                        ta.tpadesc,
                        COALESCE(oardesc, ''),
                        '<a href=\"javascript:void(0);\" 
                                id=\"' || oa.arqid || '\" 
                                class=\"download\" 
                                title=\"Baixar Arquivo\">' 
                                || a.arqnome || '.' || a.arqextensao || 
                        '</a>',
                        to_char(oardtinclusao,'DD/MM/YYYY') AS data,
                        usu.usunome
                FROM
                        obras2.obras_arquivos oa
                JOIN obras2.tipoarquivo ta ON ta.tpaid = oa.tpaid
                JOIN public.arquivo      a ON a.arqid = oa.arqid
                JOIN seguranca.usuario usu ON usu.usucpf = a.usucpf
                WHERE
                        ".implode(' AND ',$where)."
                ORDER BY
                        2";

    	return $sql;
    }

    public function listaSqlVistoria( Array $param = array(), $tipoSelecao = null ){

        $where = array("oarstatus = 'A'");

        if( $param['obrid'] != '' ){
            $where[] = "oa.obrid = '".$param['obrid']."' ";
        }

        if( $param['tpaid'] != '' ){
            $where[] = "ta.tpaid = '".$param['tpaid']."' ";
        }

        if( $param['not(img)'] == true ){
            $where[] = "(arqtipo != 'image/jpeg' AND arqtipo != 'image/gif' AND arqtipo != 'image/png')";
        }

        if( $param['only(img)'] == true ){
            $where[] = "(arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png')";
        }

        if(isset($param['tpaid(IN)']) && $param['tpaid(IN)'] != ''){
            $where[] = "ta.tpaid IN (".$param['tpaid(IN)'].")";
        }

//    	if( $param['oartipo'] != '' ){
//    		$where[] = "oartipo = '".$param['oartipo']."' ";
//    	}

        $perfil_de_nao_exclusao = possui_perfil(Array(PFLCOD_GESTOR_UNIDADE,PFLCOD_SUPERVISOR_UNIDADE));

        if( $param['block_img_excluir'] !== true && !$perfil_de_nao_exclusao){
            $img_excluir = "'<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/excluir_2.gif\"
                                    width=\"19px\"
                                    style=\"cursor: pointer; margin-left: 3px;\"
                                    id=\"' || oa.oarid || '\"
                                    class=\"excluir\"
                                    title=\"Excluir Arquivo\">' ||


		 			  '</center>";
        }else{
            $img_excluir = "'";
        }

        $sql = "SELECT
                        '<center>
                        <img
                            align=\"absmiddle\"
                            src=\"/imagens/icone_lupa.png\"
                            style=\"cursor: pointer\"
                            id=\"' || oa.arqid || '\"
                            class=\"download\"
                            title=\"Baixar Arquivo\">'
                            $img_excluir
                        ' as acao,
                        ta.tpadesc,
                        COALESCE(oardesc, ''),
                        '<a href=\"javascript:void(0);\"
                                id=\"' || oa.arqid || '\"
                                class=\"download\"
                                title=\"Baixar Arquivo\">'
                                || a.arqnome || '.' || a.arqextensao ||
                        '</a>',
                        to_char(oardtinclusao,'DD/MM/YYYY') AS data,
                        usu.usunome,
                        (select usunome from seguranca.usuario usu where usu.usucpf = fra.usucpf) as usucpf,
                        (select  entnome from entidade.entidade ent where ent.entid = fra.entidempresa) as entidempresa
                FROM
                        obras2.obras_arquivos oa
                JOIN obras2.tipoarquivo ta ON ta.tpaid = oa.tpaid
                JOIN public.arquivo      a ON a.arqid = oa.arqid
                JOIN seguranca.usuario usu ON usu.usucpf = a.usucpf
                left JOIN obras2.fiscalregistroatividade fra ON fra.oarid = oa.oarid
                WHERE
                        ".implode(' AND ',$where)."
                ORDER BY
                        2";

        return $sql;
    }
    
    public function listaFotosSql( Array $param = array(), $tipoSelecao = null ){
    	$where = Array("oarstatus = 'A'");
    	
    	$where[] = "(arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png')";
    	
    	if ( $param['arqid'] ){
    		$param['arqid'] = (array) $param['arqid'];
    		$where[] 		= "arq.arqid IN(" . implode(", ", $param['arqid']) . ")"; 
    	}
    	
    	if ( $param['obrid'] ){
    		$where[] = "oar.obrid = {$param['obrid']}"; 
    	}
    	 
    	if( $param['tpaid'] != '' ){
    		$where[] = "tpaid = '".$param['tpaid']."' ";
    	}
    	
    	$sql = "SELECT DISTINCT
                        arqnome, arq.arqid, 
                        arq.arqextensao, arq.arqtipo, 
                        arq.arqdescricao,							
                        to_char(arq.arqdata, 'DD/MM/YYYY') as data,
                        oarid
                FROM
                        obras2.obras_arquivos oar
                INNER JOIN public.arquivo arq ON arq.arqid = oar.arqid
                WHERE
                        ".implode(' AND ',$where)."
                ORDER BY
                2";
    	
    	return $sql;
    }
/*    
//    public function listaHomologadoSql( Array $param = array() ){
//    	$where 	 = array("oar.oarstatus = 'A'");
//		$where[] = "oar.tpaid = " . TIPO_OBRA_ARQUIVO_HOMOLOGACAO;
//    	
//    	if ( $param['obrid'] ){
//    		$where[] = "oar.obrid = {$param['obrid']}"; 
//    	}
//    	 
//		$sql = "SELECT
//					to_char(oar.oardtinclusao,'DD/MM/YYYY'),
//					tarq.tpadesc,
//					'<a style=\"cursor: pointer; color: blue;\" onclick=\"DownloadArquivoFase(' || arq.arqid || ');\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
//					arq.arqtamanho || ' kbs' as tamanho ,
//					arq.arqdescricao,								
//					usu.usunome
//				FROM
//					((public.arquivo arq 
//						INNER JOIN obras2.obras_arquivos oar ON oar.arqid = arq.arqid) 
//						INNER JOIN obras.tipoarquivo tarq ON tarq.tpaid = oar.tpaid) 
//						INNER JOIN seguranca.usuario usu ON usu.usucpf = arq.usucpf
//				WHERE
//					(arqtipo <> 'image/jpeg' AND arqtipo <> 'image/png' AND arqtipo <> 'image/gif') AND " . 
//					implode(' AND ',$where);    	
//    	
//    	return $sql;
//    }
*/
    public function buscaDadosPorArqid( $arqid ){
		$sql = "SELECT
                                *
                        FROM
                                obras2.obras_arquivos
                        WHERE
                                oarstatus = 'A' AND
                                arqid = {$arqid}";
		
		$dado = $this->carregar( $sql );
		
		return ($dado ? $dado : array());
	}

    public function inativar($oarid = null)
    {
        $oarid = ($oarid) ? $oarid : $this->oarid;

        $sql = "UPDATE {$this->stNomeTabela} SET oarstatus = 'I' WHERE oarid = {$oarid}";

        return $this->executar($sql);
    }

}
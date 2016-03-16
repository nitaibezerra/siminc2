<?php
	
class RegistroAtividade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.registroatividade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "rgaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
                                        'rgaid' => null, 
                                        'obrid' => null, 
                                        'usucpf' => null, 
                                        'empid' => null, 
                                        'arqid' => null, 
                                        'rgadscsimplificada' => null, 
                                        'rgadsccompleta' => null, 
                                        'rgadtinclusao' => null, 
                                        'rgastatus' => null, 
                                        'rgaautomatica' => null, 
                                        'rgaid_1' => null,
                                        'traid' => null,
                                        'rgaimp' => null
                                      );

	function listaSql( Array $param = array() ){
            $arWhere = array();

            if ( !empty($param['empid']) ){
                    $param['empid'] = (array) $param['empid'];
                    $arWhere[] 		= "ra.empid IN(" . implode(", ", $param['empid']) . ")";
            }

            if ( !empty($param['obrid']) ){
                    $param['obrid'] = (array) $param['obrid'];
                    $arWhere[] 		= "ra.obrid IN(" . implode(", ", $param['obrid']) . ")";
            }

            if ( !empty($param['rgadtinclusaoinicio']) ){
                    $param['rgadtinclusaoinicio'] = formata_data_sql( $param['rgadtinclusaoinicio'] );
                    $arWhere[] 		= "ra.rgadtinclusao >= '" . $param['rgadtinclusaoinicio'] . "'";
            }

            if ( !empty($param['rgadtinclusaofim']) ){
                    $param['rgadtinclusaofim'] = formata_data_sql( $param['rgadtinclusaofim'] );
                    $arWhere[] 		= "ra.rgadtinclusao <= '" . $param['rgadtinclusaofim'] . "'";
            }

            if ( !empty($param['usunome']) ){
                    $usunomeTmp = removeAcentos(str_replace("-"," ",$param['usunome']));
                    $arWhere[] 		= "(UPPER(public.removeacento(usu.usunome))) ILIKE '%" . $usunomeTmp . "%'";
            }

            if ( !empty($param['rgadscsimplificada']) ){
                    $rgadscsimplificadaTmp = removeAcentos(str_replace("-"," ",$param['rgadscsimplificada']));
                    $arWhere[] 		= "(UPPER (public.removeacento(ra.rgadscsimplificada) )) ILIKE '%" . $rgadscsimplificadaTmp . "%'";
            }

            if ( !empty($param['rgadsccompleta']) ){
                $rgadsccompletaTmp = removeAcentos(str_replace("-"," ",$param['rgadsccompleta']));
                $arWhere[] 		= "(UPPER (public.removeacento(ra.rgadsccompleta))) ILIKE '%" . $rgadsccompletaTmp . "%'";
            }

//            $habilitadoAcoes = ( possui_perfil( array(PFLCOD_CALL_CENTER) ) && !possui_perfil(Array(PFLCOD_SUPER_USUARIO)) ) ? false : true;
        
            if ( ($param['habilitado'] != false) && possui_perfil(PFLCOD_SUPER_USUARIO) ){
            /*
//                $acoesCpf = '';
//                if(!$habilitadoAcoes){
//                    $acoesCpf = 'style="display:none" class="acoes-\' ||usu.usucpf|| \'"';
//                }
//
//                $acao = "'<center $acoesCpf >
//						<img
//		 					align=\"absmiddle\"
//		 					src=\"/imagens/alterar.gif\"
//		 					style=\"cursor: pointer\"
//		 					onclick=\"javascript: alterarReg(\'' || ra.rgaid || '\');\"
//		 					title=\"Alterar\">' || 
//		 			CASE 
//		 				WHEN rgaautomatica = true THEN 
//			 				'<img
//			 					align=\"absmiddle\"
//			 					src=\"/imagens/excluir.gif\"
//			 					style=\"cursor: pointer; margin-left: 3px;\"
//			 					onclick=\"javascript: excluirReg(\'' || ra.rgaid || '\');\"
//			 					title=\"Excluir\">'
//		 				ELSE
//			 				'<img
//			 					align=\"absmiddle\"
//			 					src=\"/imagens/excluir_01.gif\"
//			 					style=\"cursor: pointer; margin-left: 3px;\"
//			 					title=\"Botão desabilitado\">'
//			 		END ||
//		 			  '</center>'";
                */
                
                if(possui_perfil(PFLCOD_SUPER_USUARIO)){
                    $acao = "'<center>
                        <img
		 					align=\"absmiddle\"
		 					src=\"/imagens/icone_lupa.png\"
		 					style=\"cursor: pointer\"
		 					onclick=\"javascript: verReg(\'' || ra.rgaid || '\');\"
		 					title=\"Alterar\">
						<img
		 					align=\"absmiddle\"
		 					src=\"/imagens/alterar.gif\"
		 					style=\"cursor: pointer\"
		 					onclick=\"javascript: alterarReg(\'' || ra.rgaid || '\');\"
		 					title=\"Alterar\">' || 
		 			CASE 
		 				WHEN rgaautomatica = false THEN 
			 				'<img
			 					align=\"absmiddle\"
			 					src=\"/imagens/excluir.gif\"
			 					style=\"cursor: pointer; margin-left: 3px;\"
			 					onclick=\"javascript: excluirReg(\'' || ra.rgaid || '\');\"
			 					title=\"Excluir\">'
		 				ELSE
			 				'<img
			 					align=\"absmiddle\"
			 					src=\"/imagens/excluir_01.gif\"
			 					style=\"cursor: pointer; margin-left: 3px;\"
			 					title=\"Botão desabilitado\">'
			 		END ||

			 		CASE
		 				WHEN rgaimp = true THEN
			 				'<img
			 					align=\"absmiddle\"
			 					src=\"/imagens/estrela.png\"
			 					style=\"cursor: pointer; margin-left: 3px;\"
			 					onclick=\"javascript: retirarImportancia(\'' || ra.rgaid || '\');\"
			 					title=\"Retirar importancia\">'
		 				ELSE
			 				'<img
			 					align=\"absmiddle\"
			 					src=\"/imagens/estrela1.png\"
			 				    style=\"cursor: pointer; margin-left: 3px;\"
			 					onclick=\"javascript: tornarImportante(\'' || ra.rgaid || '\');\"
			 					title=\"Marcar como importante \">'
			 		END ||
		 			  '</center>'";
                }
                
		}else{
			$acao = "'<center>
                        <img
		 					align=\"absmiddle\"
		 					src=\"/imagens/icone_lupa.png\"
		 					style=\"cursor: pointer\"
		 					onclick=\"javascript: verReg(\'' || ra.rgaid || '\');\"
		 					title=\"Alterar\">
                                  </center>'";
		}
		
		$sql = "SELECT
					{$acao} AS acao,
					ra.rgaid,
					ra.rgadscsimplificada,
					t.tranome,
					TO_CHAR(ra.rgadtinclusao , 'DD/MM/YYYY' ) AS data,
					CASE WHEN ra.arqid IS NOT NULL THEN
						'<a href=\"javascript:void(0);\" 
				    		id=\"' || a.arqid || '\" 
				    		class=\"download\" 
				    		title=\"Baixar Arquivo\">' 
				    		|| a.arqnome || '.' || a.arqextensao || 
				    	'</a>'
				    ELSE 
				    	''
				    END AS arquivo,
                    CASE WHEN usu.usunome IS NULL THEN
					 'SISTEMA'
                    ELSE
                        usu.usunome
                    END as usunome
				FROM
					obras2.registroatividade ra
				LEFT JOIN obras2.tiporegistroatividade t ON ra.traid = t.traid
				LEFT JOIN seguranca.usuario usu ON usu.usucpf = ra.usucpf
				LEFT JOIN public.arquivo a ON a.arqid = ra.arqid
				WHERE
					ra.rgastatus = 'A' AND " . 
					( implode(" AND ", $arWhere) ) . "
				ORDER BY
						ra.rgaid";
					
                    
                    
		return $sql;		
	}
									  
}
<?php 

class Documentos {
	
	public static function lista( $arConfiguracao = array() ){
		global $db;

		if($arConfiguracao['filtro']){
			$arConfiguracao = array_merge($arConfiguracao, $arConfiguracao['filtro']);
		}
		
		if(!empty($arConfiguracao) && is_array($arConfiguracao) ){
			extract($arConfiguracao);
		}

		//ver($arConfiguracao,$filtro);
        $arWhere = array();

        array_push($arWhere, " ( nd.usucpf = '{$_SESSION['usucpf']}' OR ent.entnumcpfcnpj = '{$_SESSION['usucpf']}' ) and a.spdid is null ");
        // não vai dar problema com a códiçã acima pois somente administrador e super usuario pode pesquisar
        if($usucpf){
            array_push($arWhere, " nd.usucpf = '".$usucpf."' ");
        }
        if($tpdid_pesq){
            array_push($arWhere, " td.tpdid = '".$tpdid_pesq."' ");
        }
        if($nudnumero){
            array_push($arWhere, " nd.spdnumero = '".$nudnumero."' ");
        }
        if($prcnumsidoc_pesq){
            array_push($arWhere, " p.prcnumsidoc = '".$prcnumsidoc_pesq."' ");
        }
        if($dtinicio){
            array_push($arWhere, " nd.spdddatainclusao >= '".formata_data_sql($dtinicio)."' ");
        }
        if($dtfim){
            array_push($arWhere, " nd.spdddatainclusao <= '".formata_data_sql($dtfim)."' ");
        }
        if($nudstatus){
            array_push($arWhere, " spdstatus = '".$nudstatus."' ");
        }else{
            array_push($arWhere, " spdstatus = 'A' ");
        }
        if( $advid ){
            array_push($arWhere, " adv.advid = $advid and adv.advstatus = 'A' ");
        }
        if( $administrativo == 'true' ){
            array_push($arWhere, " adv.advid IS NULL ");
        }
        $arCabecalho = array("Tipo Documento","Número Sidoc/Emec","Número do Documento","Data de Geração");

        if(possuiPerfil(array(PRF_ADMINISTRADOR,PRF_SUPERUSUARIO))){
            unset($arWhere[0]);
            $arCabecalho = array("Ação","Tipo Documento","Número Sidoc","Número do Documento","Data de Geração", "Nome", "Advogado Requerinte");
            $campo = ",us.usunome";
            $left = "left join seguranca.usuario AS us ON us.usucpf = nd.usucpf";
        }

        $sql = "SELECT
                    nd.spdid,
					p.prcid,
					coalesce(a.spdid,0) as nudidanexo,
					a.arqid,
					td.tpddsc,
					p.prcnumsidoc,
					nd.spdnumero,
					nd.spdid,
					to_char(nd.spdddatainclusao, 'DD/MM/YYYY') as spdddatainclusao,
					coalesce(ent.entnome,'ADMINISTRATIVO') as advogado
					$campo
				FROM conjur.processoconjur p
				INNER JOIN conjur.sapiensdocumento nd ON p.prcid  = nd.prcid
				INNER JOIN conjur.tipodocumento 	 td ON nd.tpdid = td.tpdid
				LEFT  JOIN conjur.sapiensanexo 			  a ON a.spdid  = nd.spdid AND a.spastatus = 'A'
				LEFT  JOIN conjur.advogados			adv ON adv.advid = nd.advid
				LEFT  JOIN entidade.entidade		ent ON ent.entid = adv.entid
				$left
				$leftAnexo
				WHERE
					1=1
					" . ( is_array($arWhere) && count($arWhere) ? ' and ' . implode(' AND ', $arWhere) : '' ) ."
					$where
				ORDER BY nd.spdid DESC ";
		$i = 0;			
		if(possuiPerfil(array(PRF_ADMINISTRADOR,PRF_SUPERUSUARIO))){
			$arParamCol[$i] = array("type" => Lista::TYPESTRING, 
					   "style" => "width:10px;",
					   "html"  => "{nudid}",
					   "align" => "center",
					   "php"   => array(
										"expressao" => "({nudidanexo})",
										"true"      => "<a style=\"margin: 0 -5px 0 5px;\" href=\"#\"><img src=\"/imagens/excluir_01.gif\" border=0 title=\"Numero possui documento.\">",
										"false"     => "<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"excluirNudid({spdid})\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir Numero\">",
										"type"      => "numeric",
									  ));
			$i++;
		}		
									  
//		$arParamCol[$i] = array("type" => Lista::TYPESTRING,
//							   "style" => "width:10px;",
//							   "html"  => "{nudidanexo}",
//							   "align" => "center",
//							   "php"   => array(
//												"expressao" => "({nudidanexo})",
//												"true" => "<a href=\"#\" onclick=\"window.location.href='?modulo=principal/documento&acao=A&requisicao=download&arqid={arqid}'\"> <img src=\"/imagens/anexo.gif\" border=0 title=\"Anexo\"> </a> ",
//												"false" => "",
//												"type" => "numeric",
//											  ));
//
		$arParamCol[$i++] = array("type" => "string", 
							   "style" => "width:400px;",
							   "html"  => "{tpddsc}",
							   "align" => "left");
		
		$arParamCol[$i++] = array("type"  => "string",
							   "style" => "width:300px;",
							   "html"  => "<a href=\"#\" onclick=\"window.location.href='?modulo=principal/editarprocesso&acao=A&prcid={prcid}'\">{prcnumsidoc}</a>",
							   "align" => "left");
		
		$arParamCol[$i++] = array("type"  => "numeric",
							   "style" => "width:200px;",
							   "html"  => "{spdnumero}",
							   "align" => "right");
		
		$arParamCol[$i++] = array("type"  => "date",
							   "style" => "width:120px;",
							   "html"  => "{spdddatainclusao}",
							   "align" => "center");
		
		$arParamCol[$i++] = array("type"  => "string",
							   "style" => "width:300px;",
							   "html"  => "{usunome}",
							   "align" => "left");
		
		$arParamCol[$i++] = array("type"  => "string",
							   "style" => "width:300px;",
							   "html"  => "{advogado}",
							   "align" => "left");
		
		// ARRAY de parametros de configuração da tabela
		$arConfig = array(//"style" => "width:95%;",
						  "totalLinha" => false,
						  "totalRegistro" => true);
		
		$oPaginacaoAjax = new PaginacaoAjax();
		$oPaginacaoAjax->setNrPaginaAtual($nrPaginaAtual);
		$oPaginacaoAjax->setNrRegPorPagina($nrRegPorPagina);
		$oPaginacaoAjax->setNrBlocoPaginacaoMaximo($nrBlocoPaginacaoMaximo);
		$oPaginacaoAjax->setNrBlocoAtual($nrBlocoAtual);
		$oPaginacaoAjax->setDiv( 'listaNumeracao' );
		$oPaginacaoAjax->setCabecalho( $arCabecalho );
		 //ver($sql,d);
		$oPaginacaoAjax->setSql( $sql );
		// $oPaginacaoAjax->setAcao( $acao );
		$oPaginacaoAjax->setParamCol( $arParamCol );
		$oPaginacaoAjax->setConfig( $arConfig );
		$oPaginacaoAjax->show();
		
		
		/*
		 * FIM - VIEW
		 * o método show() renderiza os parametros, mostrando na tela a lista.
		 */	
	}
}
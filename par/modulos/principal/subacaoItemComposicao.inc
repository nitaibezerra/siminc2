<?php //novo

//Atualizar Valor Referência Itens
if($_REQUEST['atualizar_valor_referencia']){
	ob_clean();
	
	global $db;
	
	$sql = "SELECT
				iu.itrid,
				CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE iu.mun_estuf END as estuf,
				CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE iu.muncod END as entidade
			FROM
				par.subacao s
			INNER JOIN par.acao a ON a.aciid = s.aciid
			INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
			INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
			WHERE
				s.sbaid = ".$_REQUEST['sbaid'];
			
	$entidadeItem = $db->pegaLinha($sql);
	
	//ver($_SESSION['par']['itrid'],1);
	$estuf = $entidadeItem['estuf'];

	if( $_REQUEST['picid'] != 0 ){
		$picid = "AND 			dic.picid = '{$_REQUEST['picid']}'";
	}
	
	$sql = "SELECT 			dic.picid, dic.dicvalor, sic.sbaid, sic.icoano, sic.icovalor
			FROM 			par.detalheitemcomposicao dic
			INNER JOIN 		par.ufdetalheitemcomposicao ufdi ON ufdi.dicid = dic.dicid
			INNER JOIN 		par.subacaoitenscomposicao sic ON sic.picid = dic.picid
			WHERE 			sic.sbaid = {$_REQUEST['sbaid']}
			AND				(now()::date between dicdatainicial AND dicdatafinal) 
			AND 			dicstatus = 'A' 
			AND				estuf = '{$estuf}' 
			AND 			sic.icoano = '{$_REQUEST['ano']}' 
			$picid";
	
	$aryDados = $db->carregar($sql);
	$aryDados = $aryDados ? $aryDados : array();
	
	if( is_array($aryDados) ){
		foreach($aryDados as $dados){
			$sql = "UPDATE  	par.subacaoitenscomposicao 
					SET 		icovalor = '{$dados['dicvalor']}'
					WHERE 		sbaid = {$dados['sbaid']}
					AND 		icoano = '{$dados['icoano']}'
					AND 		picid = {$dados['picid']}";
			$db->executar($sql);
			$db->commit();
		}
		echo "t";
	}
	die();
}

$sbaid = $_REQUEST['sbaid'];
if( $_REQUEST['sbaid'] ){
		
	function comboUnidadeMedida($nome = "umiid[]", $value = null){
		global $db;
		$nome = !$_POST['nome'] ? $nome : $_POST['nome'];
		$sql = "SELECT
					umiid as codigo, 
					umidescricao as descricao 
				FROM
					par.unidademedidadetalhamentoitem 
				WHERE 
					umistatus = 'A'
				ORDER BY
					umidescricao";
	
		return $db->monta_combo($nome,$sql,"S","Selecione...","","","","","N","","",$value);
	}
	
	function isOnibusAcessivel($sbaid)
	{
		global $db;
		$sqlOnibusAcessivel = "SELECT
			prg.prgid
			FROM par.subacao 	 	 s
			INNER JOIN par.programa prg ON prg.prgid = s.prgid
			WHERE s.sbaid = {$_GET['sbaid']}
			and prg.prgid = 153
		";
			
		$isOnibusAcessivel = (is_array($db->carregar($sqlOnibusAcessivel))) ? true : false;
		
		return $isOnibusAcessivel;
	} 
	
				
											
	
	function comboItem($nome = "item[]", $value = null, $numero = null, $condicao = null, $grupo = null, $dicid = null, $icosequencia = null){
	
		global $db;
		
		$sql = "SELECT 
					iu.itrid,
					CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE iu.mun_estuf END as estuf,
					CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE iu.muncod END as entidade
				FROM
					par.subacao s
				INNER JOIN par.acao a ON a.aciid = s.aciid
				INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
				INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
				WHERE
					s.sbaid = ".$_REQUEST['sbaid'];
					
		$entidadeItem = $db->pegaLinha($sql);
		
		$estuf = $entidadeItem['estuf'];
		
		$sql = "SELECT ppsid, ptsid FROM par.subacao WHERE sbaid = ".$_REQUEST['sbaid'];
		$sba = $db->pegaLinha( $sql );
	
		$nome = !$_POST['nome'] ? $nome : $_POST['nome'];
		$numero = !$_POST['numero'] ? $numero : $_POST['numero'];
		$condicao = !$_POST['condicao'] ? $condicao : str_replace( "|", ",", $_POST['condicao']);
		$condicao = str_replace( array("\\", "'"), "", $condicao);
		$grupo = !$_POST['grupo'] ? $grupo : $_POST['grupo'];
		$where = '';
		if( $condicao ){
// 			$where = " AND pic.picid NOT IN (".$condicao.")";
		}
		$testaReprogramacao = false;
		if( $_REQUEST['sbaid'] && $_REQUEST['ano'] && $_REQUEST['pregoesvencidos'] ){
			$sql = "SELECT ssuid FROM par.subacaodetalhe WHERE sbaid = ".$_REQUEST['sbaid']." AND sbdano = ".$_REQUEST['ano'];
			$ssuid = $db->pegaUm( $sql );
			
			if( $ssuid == 20 || $ssuid == 21 ){
				$testaReprogramacao = true;
				$where = "";
			}
		}
		
		if( $sba['ppsid'] ){ // veio do Guia
			if( $grupo ){
				$sql = "SELECT  
							pic.picid as codigo, 
				            pic.picdescricao as descricao 
		               FROM par.propostaitemcomposicao pic
		               INNER JOIN par.propostaitem_grupoitem g ON g.picid = pic.picid
		               WHERE 
		               		g.gicid = ".$grupo." AND
		               		pic.picstatus = 'A' --AND 
		               	--	pic.picid NOT IN (SELECT picid FROM par.subacaoitenscomposicao WHERE sbaid = ".$_REQUEST['sbaid']." AND icoano = ".$_REQUEST['ano'].") 
		               		".$where ." 
		               	ORDER BY
		               		descricao";

			} else {
				if( $testaReprogramacao == true ){ // É uma reprogramação, então eu mostro os itens de pregões antigos.
//					$cond = "pic.picid as codigo,";
					if( $value ){
						$valor = $value;
// 						if(!$dicid){
// 							$dicid = $db->pegaUm("SELECT dicid FROM par.subacaoitenscomposicao WHERE sbaid = ".$_REQUEST['sbaid']." AND icoano = ".$_REQUEST['ano']." AND picid = ".$value);
// 						}
						
						$cond = "pic.picid ||'_'|| dic.dicid as codigo,";
						if( $dicid ){
							$whereRe = " AND sic.dicid = ".$dicid; 
							$value = $value."_".$dicid."_".$icosequencia;
						}else{
							$whereRe = " AND sic.dicid IS NULL "; 
						}
						$whereRe .= " AND sic.picid = ".$valor." AND sic.sbaid = ".$_REQUEST['sbaid']." AND icoano = ".$_REQUEST['ano'];
					} else {
						$cond = "pic.picid ||'_'|| dic.dicid as codigo,";
						$whereRe = " AND sic.sbaid = ".$_REQUEST['sbaid']." AND icoano = ".$_REQUEST['ano'];
					}
					$sql = "SELECT DISTINCT codigo, descricao, title, dicdatainicial, dicdatafinal, status FROM
						(SELECT
								$cond
				            	pic.picdescricao || ' - '||dic.dicvalor||' - ('||to_char(dic.dicdatainicial,'dd/mm/YYYY')||' - '||to_char(dic.dicdatafinal,'dd/mm/YYYY')||')' || COALESCE(' - ' || ptp.ptpnumeroata, '') as descricao,
					            pic.picdetalhe as title,
					            dic.dicvalor as valoritem,
					            dic.dicdatainicial, dic.dicdatafinal,
					            to_char(dic.dicdatainicial, 'YYYY') as anoini,
					            to_char(dic.dicdatafinal, 'YYYY') as anofim, 
					            udi.estuf,
		    					CASE WHEN ((now()::date between dic.dicdatainicial and dic.dicdatafinal) AND dicstatus = 'A') THEN 'A' ELSE 'I' END as status
							FROM
								par.propostaitemcomposicao pic
							INNER JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND (now()::date between dic.dicdatainicial and dic.dicdatafinal) AND dicstatus = 'A'
							INNER JOIN par.propostasubacaoitem psi ON psi.picid = pic.picid AND psi.ppsid = {$sba['ppsid']}
							INNER JOIN par.ufdetalheitemcomposicao udi ON udi.dicid = dic.dicid AND udi.estuf = '{$estuf}'
							LEFT JOIN par.pregaouf puf ON puf.ptpid = dic.ptpid  and puf.estuf = '{$estuf}'
							LEFT JOIN par.propostatipopregao ptp on ptp.ptpid = puf.ptpid AND ptpstatus = 'A'
							WHERE
								pic.picstatus = 'A'
								".$where ." "; 
					if( $value ){								
						$sql .= "UNION ALL 
						
							 SELECT
									
								CASE WHEN dic.dicid IS NULL
									THEN pic.picid::text
									ELSE pic.picid ||'_'|| dic.dicid ||'_'|| icosequencia 
								END as codigo,
								CASE WHEN dic.dicid IS NULL
									THEN pic.picdescricao
									ELSE pic.picdescricao || ' - '||dic.dicvalor||' - ('||to_char(dic.dicdatainicial,'dd/mm/YYYY')||' - '||to_char(dic.dicdatafinal,'dd/mm/YYYY')||')' || COALESCE(' - ' || ptp.ptpnumeroata, '')
								END as descricao,
								pic.picdetalhe as title,
									dic.dicvalor as valoritem,
									dic.dicdatainicial, dic.dicdatafinal,
									to_char(dic.dicdatainicial, 'YYYY') as anoini,
									to_char(dic.dicdatafinal, 'YYYY') as anofim, 
									udi.estuf,
				    				CASE WHEN ((now()::date between dic.dicdatainicial and dic.dicdatafinal) AND dicstatus = 'A') THEN 'A' ELSE 'I' END as status
							FROM
								par.subacaoitenscomposicao sic
							INNER JOIN par.propostaitemcomposicao pic ON pic.picid = sic.picid
							LEFT  JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND sic.dicid = dic.dicid
							LEFT  JOIN par.ufdetalheitemcomposicao udi ON udi.dicid = dic.dicid AND udi.estuf = '{$estuf}'
							INNER JOIN par.propostasubacaoitem psi ON psi.picid = pic.picid  AND psi.ppsid = {$sba['ppsid']}
							LEFT JOIN par.pregaouf puf ON puf.ptpid = dic.ptpid  and puf.estuf = '{$estuf}'
							LEFT JOIN par.propostatipopregao ptp on ptp.ptpid = puf.ptpid AND ptpstatus = 'A'
							WHERE 1 = 1 {$whereRe} "; 
					
					}
					$sql .= ") as foo
					ORDER BY
						descricao, status, dicdatainicial";

				} else {
					$sql = "SELECT DISTINCT codigo, descricao, title FROM
						(SELECT
								pic.picid as codigo,
				            	pic.picdescricao as descricao,
					            pic.picdetalhe as title,
					            dic.dicvalor as valoritem,
					            dic.dicdatainicial, dic.dicdatafinal,
					            to_char(dic.dicdatainicial, 'YYYY') as anoini,
					            to_char(dic.dicdatafinal, 'YYYY') as anofim, 
					            puf.estuf
							FROM
								par.propostaitemcomposicao pic
							INNER JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A' AND (now()::date between dic.dicdatainicial and dic.dicdatafinal)
							INNER JOIN par.propostasubacaoitem psi ON psi.picid = pic.picid AND psi.ppsid = {$sba['ppsid']}
							INNER JOIN par.ufdetalheitemcomposicao udi ON udi.dicid = dic.dicid AND udi.estuf = '{$estuf}'
							LEFT JOIN par.pregaouf puf ON puf.ptpid = dic.ptpid  and puf.estuf = '{$estuf}'
							LEFT JOIN par.propostatipopregao ptp on ptp.ptpid = puf.ptpid AND ptpstatus = 'A'
							WHERE
								pic.picstatus = 'A' ".$where ." 
				UNION ALL
					 SELECT
							pic.picid as codigo,
				            pic.picdescricao as descricao,
							pic.picdetalhe as title,
							dic.dicvalor as valoritem,
							dic.dicdatainicial, dic.dicdatafinal,
							to_char(dic.dicdatainicial, 'YYYY') as anoini,
							to_char(dic.dicdatafinal, 'YYYY') as anofim, 
							'' AS estuf
					FROM
						par.propostaitemcomposicao pic
					INNER JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A' AND dicpercentual is not null
					INNER JOIN par.ufdetalheitemcomposicao udi ON udi.dicid = dic.dicid AND udi.estuf = '{$estuf}'
					INNER JOIN par.propostasubacaoitem psi ON psi.picid = pic.picid  AND psi.ppsid = {$sba['ppsid']}
					WHERE pic.picstatus = 'A' {$where} ) as foo
					ORDER BY
						descricao";
				}
			}
		} else { //não veio do guia!
			if($sba['ptsid']){
			
				$sql = "";
			
				$sql = "SELECT DISTINCT * FROM (
				
						SELECT  
							pc.picid ||'_'|| dic.dicid as codigo,
						--	pc.picid as codigo,
				            pc.picdescricao as descricao 
		               FROM par.propostaitemcomposicao pc
		               INNER JOIN par.propostaitemtiposubacao pi ON pc.picid = pi.picid
		               INNER JOIN par.detalheitemcomposicao dic ON dic.picid = pc.picid AND (now()::date between dic.dicdatainicial and dic.dicdatafinal)  
		               INNER JOIN par.ufdetalheitemcomposicao ufdi ON ufdi.dicid = dic.dicid
		               WHERE 
		               		pi.ptsid = ".$sba['ptsid']."
		               		AND pc.picstatus = 'A'  
		               		AND dicstatus = 'A'
		               		AND ufdi.estuf = '{$estuf}'"; 
				if( $value ){							
					
					$valor = $value;
					if( !$dicid ){
						$dicid = $db->pegaUm("SELECT dicid FROM par.subacaoitenscomposicao WHERE sbaid = ".$_REQUEST['sbaid']." AND icoano = ".$_REQUEST['ano']." AND picid = ".$value);
					} 
					
					if( $dicid ){
						$where2 = " AND sic.dicid = {$dicid} ";
						$value = $value."_".$dicid."_".$icosequencia;
					}
					
					$sql .= "
					
						UNION ALL
					
						 SELECT
							sic.picid ||'_'|| sic.dicid ||'_'|| icosequencia as codigo,
						--	sic.picid as codigo,
				            pc.picdescricao as descricao
						FROM
							par.subacaoitenscomposicao sic
						INNER JOIN par.propostaitemcomposicao pc ON pc.picid = sic.picid
						INNER JOIN par.detalheitemcomposicao dic ON dic.picid = sic.picid AND sic.dicid = dic.dicid
						WHERE 
							sic.picid = ".$valor." AND 
							sic.sbaid = ".$_REQUEST['sbaid']." 
							{$where2} "; 
				
				} else {
					$sql .= "
					
						UNION ALL
					
						 SELECT DISTINCT
							sic.picid ||'_'|| sic.dicid as codigo,
						--	sic.picid as codigo,
				            pc.picdescricao as descricao
						FROM
							par.subacaoitenscomposicao sic
						INNER JOIN par.propostaitemcomposicao pc ON pc.picid = sic.picid
						INNER JOIN par.detalheitemcomposicao dic ON dic.picid = sic.picid AND sic.dicid = dic.dicid
						WHERE 
							sic.sbaid = ".$_REQUEST['sbaid']; 
				}
		        $sql .= "ORDER BY descricao ) as foo";
	
			} else {
				echo "<script>
						alert('Falta o plano de trabalho!');
						window.close();
					</script>";
			}
		}
// 		if( $numero == 4 ){
// 			ver($sql);
// 		}
		$perfil = possuiPerfil(array(PAR_PERFIL_SUPER_USUARIO, PAR_PERFIL_ADMINISTRADOR));
		$complementoOption = array();
		if( $testaReprogramacao == true ){ // É uma reprogramação, então eu mostro os itens de pregões antigos.
			$codigos = array();
			$dadosItens = $db->carregar($sql);

			if(is_array($dadosItens)){
				foreach($dadosItens as $d){
					if( $d['status'] == 'A'){
						array_push($codigos, $d['codigo']);
					}
				}
			}
			$complementoOption = array(	
									array('codigo'=>$codigos, 'complemento'=>'style="color: blue;"'),
									array('codigo'=>array(0=>$value), 'complemento'=>'style="color: red;"')
									);
		}
		
		$isOnibusAcessivel = isOnibusAcessivel($_REQUEST['sbaid']);
		$habil = ($isOnibusAcessivel) ? 'N' : 'S';
		return $db->monta_combo($nome,$sql,$habil,"Selecione...","javascript:carregaItens(this.value, '$numero', '$perfil');","","","100","N","","",$value, $title= null, $complemento = null, $classe = null, $complementoOption, true);
	}
	
	function valorUnitario($nome = "valor[]", $numero = null, $picid = null, $grupo = null, $dicid = null){
		global $db;
		$nome = !$_POST['nome'] ? $nome : $_POST['nome'];
		$numero = !$_POST['numero'] ? $numero : $_POST['numero'];
		$picid = !$_POST['picid'] ? $picid : $_POST['picid'];
		$grupo = !$_POST['grupo'] ? $grupo : $_POST['grupo'];
		$dicid = !$_POST['dicid'] ? $dicid : $_POST['dicid'];
		
		$sql = "SELECT 
			iu.itrid,
			CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE iu.mun_estuf END as estuf,
			CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE iu.muncod END as entidade
		FROM
			par.subacao s
		INNER JOIN par.acao a ON a.aciid = s.aciid
		INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
		INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
		WHERE
			s.sbaid = ".$_REQUEST['sbaid'];
					
		$entidadeItem = $db->pegaLinha($sql);
		
		$sql = "SELECT ppsid, ptsid FROM par.subacao WHERE sbaid = ".$_REQUEST['sbaid'];
		$sba = $db->pegaLinha( $sql );

		$estuf = $entidadeItem['estuf'];
		
		if( $sba['ppsid'] ){ //veio do guia
			
			if( $grupo ){
				$sql = "SELECT
				            dic.dicvalor as valoritem,
				            uni.umidescricao as descricao,
							dic.dicpercentual as percentual
						FROM
							par.propostaitemcomposicao pic
						LEFT JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A' AND (now()::date between dic.dicdatainicial and dic.dicdatafinal)
						INNER JOIN par.propostaitem_grupoitem pgi ON pgi.picid = pic.picid
						INNER JOIN par.propostagrupoitem pg ON pg.gicid = pgi.gicid
						LEFT JOIN par.pregaouf puf ON puf.ptpid = dic.ptpid  and estuf = '{$estuf}'
						LEFT JOIN par.propostatipopregao ptp on ptp.ptpid = puf.ptpid AND ptpstatus = 'A'
						INNER JOIN par.subacao s ON s.ppsid = pg.ppsid
						INNER JOIN par.unidademedidadetalhamentoitem uni ON pic.umiid = uni.umiid
						INNER JOIN par.ufdetalheitemcomposicao ufdic ON ufdic.dicid = dic.dicid  and ufdic.estuf = '{$estuf}'
						WHERE
							pic.picid = ".$picid." AND
							pic.picstatus = 'A' AND
							sbaid = ".$_REQUEST['sbaid']; 
			} else {
			
				if( $dicid ){ // Reprogramação
					$w = " dic.dicid = ".$dicid." AND ";
				} else {
					$inn = " AND dicstatus = 'A' AND (now()::date between dic.dicdatainicial and dic.dicdatafinal) ";
				}
				$sql = "SELECT
				            dic.dicvalor as valoritem,
				            uni.umidescricao as descricao,
							dic.dicpercentual as percentual
						FROM
							par.propostaitemcomposicao pic
						LEFT JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid $inn
						INNER JOIN par.propostasubacaoitem psi ON psi.picid = pic.picid
						LEFT JOIN par.pregaouf puf ON puf.ptpid = dic.ptpid  and estuf = '{$estuf}'
						LEFT JOIN par.propostatipopregao ptp on ptp.ptpid = puf.ptpid AND ptpstatus = 'A'
						INNER JOIN par.subacao s ON s.ppsid = psi.ppsid
						INNER JOIN par.unidademedidadetalhamentoitem uni ON pic.umiid = uni.umiid
						INNER JOIN par.ufdetalheitemcomposicao ufdic ON ufdic.dicid = dic.dicid  and ufdic.estuf = '{$estuf}'
						WHERE
							pic.picid = ".$picid." AND
							$w
							sbaid = ".$_REQUEST['sbaid']; 
			}
		} else { //não veio do guia!
	
			$sql = "SELECT  
						dic.dicvalor as valoritem,
						uni.umidescricao as descricao,
						'0' as pregao,
						'0' as percentual
	               FROM par.propostaitemcomposicao pc
	               INNER JOIN par.propostaitemtiposubacao pi ON pc.picid = pi.picid
	               INNER JOIN par.detalheitemcomposicao dic ON dic.picid = pc.picid  AND (now()::date between dic.dicdatainicial and dic.dicdatafinal)  
	               INNER JOIN par.ufdetalheitemcomposicao ufdi ON ufdi.dicid = dic.dicid
	               INNER JOIN par.unidademedidadetalhamentoitem uni ON pc.umiid = uni.umiid
	               WHERE 
	               		pi.ptsid = ".$sba['ptsid']."
	               		AND pc.picstatus = 'A'  
	               		AND dicstatus = 'A'
	               		AND pc.picid = ".$picid."
	               		AND ufdi.estuf = '{$estuf}'";
	
		}
		$valor = $db->pegaLinha( $sql );
		echo number_format($valor['valoritem'],2,',','.').'|'.$valor['descricao'].'|'.$valor['percentual'];
	
	}
	
	function texto_qtde($nome = "icoquantidade[]", $numero = null, $value = null){
		$nome = !$_POST['nome'] ? $nome : $_POST['nome'];
		$numero = !$_POST['numero'] ? $numero : $_POST['numero'];
		
		$isOnibusAcessivel = isOnibusAcessivel($_REQUEST['sbaid']);
		$habil = ($isOnibusAcessivel) ? 'N' : 'S';
		
		echo campo_texto($nome,"N",$habil,"","10","20","[.###]","","right","","","onclick='verificaEscolas($numero);'","calculaValorItem(this.value, '$numero')", number_format($value,0,',','.') );
	//	calculaValorItem(this.value,'{$dado['picid']}','{$dado['valor']}')
	}
	
	function texto_valor($nome = "valor[]", $value = null, $numero = null){
		$nome   = !$_POST['nome'] ? $nome : $_POST['nome'];
		$numero = !$_POST['numero'] ? $numero : $_POST['numero'];

		$isOnibusAcessivel = isOnibusAcessivel($_REQUEST['sbaid']);
		$habil = ($isOnibusAcessivel) ? 'N' : 'S';
		
		
		echo campo_texto($nome,"N",$habil,"","10","20","[.###],##","","right","","","","calculaValorItem(this.value, $numero)", number_format($value,2,',','.') );
	//	calculaValorItem(this.value,'{$dado['picid']}','{$dado['valor']}')
	
	}
	
	function recuperaItensGrupo(){
		
		global $db;
		
		$gicid = $_POST['gicid'];
		
		$sql = "SELECT 
					pg.picid 
				FROM 
					par.propostaitem_grupoitem pg
				INNER JOIN par.propostaitemcomposicao pic ON pic.picid = pg.picid AND pic.picstatus = 'A' 
				WHERE 
					pg.gicid = ".$gicid;
		$itens = $db->carregarColuna( $sql );
		
		echo implode("|",$itens);
	}
	
	function texto_valorTotal($nome = "valortotal[]", $value = null){
		$nome = !$_POST['nome'] ? $nome : $_POST['nome'];
		echo number_format($arrDadosIC[$dado['picid']]*$dado['valor'],2,',','.');
	//	calculaValorItem(this.value,'{$dado['picid']}','{$dado['valor']}')
	}

	
	$sql = "SELECT ppsid, ptsid FROM par.subacao WHERE sbastatus = 'A' AND sbaid = ".$_REQUEST['sbaid'];
	$sba = $db->pegaLinha( $sql );
	
	if(!$sbaid){
		echo "<script>alert('Subação não encontrada!');window.close();</script>";
	}
	
	$sql = "SELECT mun_estuf
			FROM par.instrumentounidade iu
			INNER JOIN par.pontuacao p ON p.inuid = iu.inuid AND p.ptostatus = 'A'
			INNER JOIN par.acao a ON a.ptoid = p.ptoid AND a.acistatus = 'A'
			INNER JOIN par.subacao s ON s.aciid = a.aciid AND s.sbastatus = 'A'
			WHERE s.sbaid = ".$_REQUEST['sbaid'];
	
	$_SESSION['par']['mun_estuf'] = $db->pegaUm( $sql );
	
	
	
	function contemItenRepetido($post)
	{
		global $db;
		$oIC = new SubacaoItensComposicao();
			//Valida se não possuem itens duplicados
			foreach($post['item'] as $key => $picid)
			{
				$dicid = "";
				if( strpos($picid, "_") ){
					list($picid, $dicid) = split('_',$picid);
					$icoid = $oIC->recuperaItemPorSubacao($_GET['sbaid'],$_GET['ano'],$picid,$dicid);
					$dado = $db->pegaLinha("select * from par.propostaitemcomposicao pic LEFT JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid WHERE pic.picid = ".$picid." AND dic.dicid = ".$dicid);
				} else {
					$icoid = $oIC->recuperaItemPorSubacao($_GET['sbaid'],$_GET['ano'],$picid);
					if( $icoid ){
						$dicid = $db->pegaUm("SELECT dicid FROM par.subacaoitenscomposicao WHERE icoid = ".$icoid);
					}
					if( $dicid ){
						$dado = $db->pegaLinha("select * from par.propostaitemcomposicao pic LEFT JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid WHERE pic.picid = ".$picid." AND dic.dicid = ".$dicid);
					} else {
						$dado = $db->pegaLinha("select 
													pic.*, dic.*
												from 
													par.propostaitemcomposicao pic 
												INNER JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A' AND (now()::date between dic.dicdatainicial and dic.dicdatafinal)
												INNER JOIN par.ufdetalheitemcomposicao udi ON udi.dicid = dic.dicid AND udi.estuf = '{$estuf}'
												WHERE 
													pic.picid = $picid");
					}
				}
				$valor = $dado['dicvalor'];
				if($picid)
				{
					$texto = $db->pegaUm("select picdescricao from  par.propostaitemcomposicao  where picid = {$picid} ");
				}
				if(!$valor){
					$valor = $post['valorH'][$n];
				}
				
				if( $post['valorMin'][0] || $post['valorMax'][0] ){
					$valor = $post['valorP'][$n];
				}
				
				if( $post['valorP'][$n] != '' ){
					$valor = $post['valorP'][$n];
				} else {
					$valor = $post['valorH'][$n];
				}
	
				if( $post['valor'][$n] != '' ){
					$valor = $post['valor'][$n];
				}
				
				if( strpos($valor, ',') ){
					$valor = str_replace(",",".", (str_replace(".","",$valor)));
				}
				
				if( !$valor ){
					if($dicid){
						$valor = $db->pegaUm( "select dicvalor from par.detalheitemcomposicao where picid = ".$picid." AND dicid = ".$dicid );
					} else {
						$valor = $db->pegaUm( "select dicvalor from par.detalheitemcomposicao where picid = ".$picid );
					}
				}
	
				$arrComparativo[$key] = array(
					'picid' => $picid,
					'dicid' => $dicid,
					'valor' => $valor,
					'icodescricao' => $texto
				);  
			}
			
			
			foreach($arrComparativo as $key => $item)
			{
				foreach($arrComparativo as $k => $v)
				{
					if($key != $k)
					{
						if($item['dicid'])
						{
							if( ($item['picid'] === $v['picid'] ) && ($item['dicid'] === $v['dicid'] ) && ($item['valor'] === $v['valor'] ))
							{
								return array( 'retorno' => true , 'item' => $item['icodescricao'] );
							}
						}
						else
						{
							if( ($item['picid'] === $v['picid'] ) && ($item['valor'] === $v['valor'] ))
							{
								return array( 'retorno' => true , 'item' => $item['icodescricao'] );
							}
						}
					}
				}
			}
			return array( 'retorno' => false , 'item' => $item['icodescricao'] );
			
	}
	
	function salvarItemComposicao(){
		global $db;
		
//		ver($_POST, d);
		
		$sql = "SELECT 
			iu.itrid,
			CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE iu.mun_estuf END as estuf,
			CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE iu.muncod END as entidade
		FROM
			par.subacao s
		INNER JOIN par.acao a ON a.aciid = s.aciid
		INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
		INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
		WHERE
			s.sbaid = ".$_GET['sbaid'];
					
		$entidadeItem = $db->pegaLinha($sql);
		
		$oIC = new SubacaoItensComposicao();
		$estuf = $entidadeItem['estuf'];

		if( is_array($_POST['item']) ){
			
			$arrPicid = array();
			foreach($_POST['item'] as $item){
//				$arPicid = explode('_', $item);
				list($arPicid, $dicid, $sequencia) = split('_',$item);
				array_push($arrPicid, $arPicid);
			}

			$sql = "UPDATE par.subacaoitenscomposicao SET
						icostatus = 'I'
					WHERE
						picid NOT IN (".(implode(',',$arrPicid)).") 
						AND sbaid = {$_GET['sbaid']}
						AND icoano = {$_GET['ano']}";
			
			$db->executar($sql);
			$db->commit();
			
// 			$validaItensRepetidos = contemItenRepetido($_POST);
// 			if($validaItensRepetidos['retorno'])
// 			{
// 				$txtItem = $validaItensRepetidos['item'];
// 				echo "<script>
// 					alert('O item \"{$txtItem}\" está duplicado!');
// 				</script>";	
// 				return false;
// 			}

			foreach($_POST['item'] as $n => $picid){
				$dicid = "";
				$sequencia = "";
				if( strpos($picid, "_") ){
					list($picid, $dicid, $sequencia) = split('_',$picid);
					if(!$sequencia){
						$sequencia = $db->pegaUm("SELECT COALESCE(MAX(icosequencia), 0) + 1 FROM par.subacaoitenscomposicao WHERE picid = ".$picid." AND icoano = ".$_GET['ano']);
						$sequencia = $sequencia ? $sequencia : 1;
					}
					$icoid = $oIC->recuperaItemPorSubacao($_GET['sbaid'],$_GET['ano'],$picid,$dicid,$sequencia);
					$dado = $db->pegaLinha("select * from par.propostaitemcomposicao pic LEFT JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid WHERE pic.picid = ".$picid." AND dic.dicid = ".$dicid);
				} else {
					$sequencia = $db->pegaUm("SELECT COALESCE(MAX(icosequencia), 0) + 1 FROM par.subacaoitenscomposicao WHERE picid = ".$picid." AND icoano = ".$_GET['ano']);
					$sequencia = $sequencia ? $sequencia : 1;
					$icoid = $oIC->recuperaItemPorSubacao($_GET['sbaid'],$_GET['ano'],$picid,null,$sequencia);
					if( $icoid ){
						$dicid = $db->pegaUm("SELECT dicid FROM par.subacaoitenscomposicao WHERE icoid = ".$icoid);
					}
					if( $dicid ){
						$dado = $db->pegaLinha("select * from par.propostaitemcomposicao pic LEFT JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid WHERE pic.picid = ".$picid." AND dic.dicid = ".$dicid);
					} else {
						$dado = $db->pegaLinha("select 
													pic.*, dic.*
												from 
													par.propostaitemcomposicao pic 
												INNER JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A' AND (now()::date between dic.dicdatainicial and dic.dicdatafinal)
												INNER JOIN par.ufdetalheitemcomposicao udi ON udi.dicid = dic.dicid AND udi.estuf = '{$estuf}'
												WHERE 
													pic.picid = $picid");
					}
				}
				$valor = $dado['dicvalor'];
				if(!$valor){
					$valor = $_POST['valorH'][$n];
				}

				if( $_POST['valorMin'][0] || $_POST['valorMax'][0] ){
					$valor = $_POST['valorP'][$n];
				}
				
				if( $_POST['valorP'][$n] != '' ){
					$valor = $_POST['valorP'][$n];
				} else {
					$valor = $_POST['valorH'][$n];
				}

				if( $_POST['valor'][$n] != '' ){
					$valor = $_POST['valor'][$n];
				}
				
				if( strpos($valor, ',') ){
					$valor = str_replace(",",".", (str_replace(".","",$valor)));
				}
				
				if( !$valor ){
					if($dicid){
						$valor = $db->pegaUm( "select dicvalor from par.detalheitemcomposicao where picid = ".$picid." AND dicid = ".$dicid );
					} else {
						$valor = $db->pegaUm( "select dicvalor from par.detalheitemcomposicao where picid = ".$picid );
					}
				}

				if( $_POST['grupopedido'] ){
					list($gicid, $demais) = split('-',$_POST['grupopedido']);
					$arDados = explode("|", trim($demais));
				}

//				if( $_POST['valor'][$n] == '' ){
					//unset($icoid);
//				}

				$oIC = new SubacaoItensComposicao($icoid);	
				$oIC->icoano 			 = $_GET['ano'];
				$oIC->icodescricao 	 	 = $dado['picdescricao'];
				if( $_POST['sbacronograma'] == 1 ){			
					$oIC->icoquantidade		 = $_POST['icoquantidade'][$n] ? str_replace(".","",$_POST['icoquantidade'][$n]) : 0;
					$oIC->icovalortotal		 = $_POST['valortotalH'][$n] ? str_replace(",",".", (str_replace(".","",$_POST['valortotalH'][$n]))) : 0;
				}
				$oIC->icovalor 			 = $valor ? $valor : NULL;
				$oIC->icostatus	 		 = "A";
				$oIC->sbaid 			 = $_GET['sbaid'];
				$oIC->unddid 			 = $dado['umiid'];
				$oIC->icodetalhe 		 = $dado['picdetalhe'];
				$oIC->usucpf 	 		 = $_SESSION['usucpf'];
				$oIC->dtatualizacao 	 = "'now()'";
				if($_POST['grupopedido']){
					if( in_array( $picid, $arDados ) ){
						$oIC->gicid      = $gicid;	
					}
				}
				$oIC->icosequencia		 = $sequencia ? $sequencia : 1;
				$oIC->picid 			 = $picid;
				$oIC->dicid			 	 = $dicid ? $dicid : $dado['dicid'];
				$oIC->salvar();
				//ver($oIC, d);
				$oIC->commit();
				$n++;
			}

			if($_GET['sbaid'] &&  $_GET['ano'])
			{	
				atualizaValorSubacaoDetalhe($_GET['sbaid'], $_GET['ano']);
			}
			
			echo "<script>
					alert('Operação realizada com sucesso!');
					parent.window.opener.location.href = 'par.php?modulo=principal/subacao&acao=A&sbaid={$_GET['sbaid']}&anoatual={$_GET['ano']}';
					window.close();
				</script>";	
			exit;
		} else {
			echo "<script>
					alert('Por favor preencha corretamente!');
				</script>";	
		}
			
	}
	
	function deletaRegistro(){
		global $db;
		$id = $_POST['id'];
		
		$sql = "DELETE FROM par.subescolas_subitenscomposicao WHERE icoid = ".$id;
		$db->executar( $sql );
		
		$sql = "DELETE FROM par.subacaoitenscomposicao WHERE icoid = ".$id;
		$db->executar( $sql );
		$db->commit();
		
		exit;
	}
	
	if($_POST['requisicao']){
		$_POST['requisicao']();
	}
	
	if($_POST['requisicaoAjax']){
		header('content-type: text/html; charset=ISO-8859-1');
		$_POST['requisicaoAjax']();
		die;
	}
	
	$sql = "SELECT ssuid FROM par.subacaodetalhe WHERE sbaid = {$sbaid} AND sbdano = {$_REQUEST['ano']}";
	$ssuid = $db->pegaUm($sql);

	/** Carrega informações de localização e dados da subação  **/
	$sql = "SELECT  s.sbaid,
					s.sbadsc,
					s.ptsid,
					s.foaid,
					d.dimid,
					d.dimcod,
					d.dimdsc,
					ar.areid,
					ar.arecod,
					ar.aredsc,
					i.indcod,
					i.indid,
					i.inddsc,
					a.acidsc,
					p.ptoid,
					s.sbaestrategiaimplementacao,
					s.undid,
					s.prgid,
					s.frmid,
					s.sbareformulacao,
					pps.ppscronograma
			FROM par.subacao 	 	 s
			INNER JOIN par.acao 	 a  ON a.aciid  = s.aciid AND a.acistatus = 'A' 
			INNER JOIN par.pontuacao p  ON p.ptoid  = a.ptoid AND p.ptostatus = 'A'
			INNER JOIN par.criterio  c  ON c.crtid  = p.crtid AND c.crtstatus = 'A' 
			INNER JOIN par.indicador i  ON i.indid  = c.indid AND i.indstatus = 'A'
			INNER JOIN par.area 	 ar ON ar.areid = i.areid AND ar.arestatus = 'A'
			INNER JOIN par.dimensao  d  ON d.dimid  = ar.dimid AND d.dimstatus = 'A'
			LEFT JOIN par.propostasubacao pps ON pps.ppsid = s.ppsid
			WHERE s.sbastatus = 'A' AND s.sbaid = {$sbaid}";
	
	$subacao = $db->pegaLinha($sql);
	
	$sql = "SELECT sbdreformulacao FROM par.subacaodetalhe WHERE sbaid = {$sbaid} AND sbdano = '{$_REQUEST['ano']}';";
	$sbdreformulacao = $db->pegaUm($sql);
//	ver($sbdreformulacao);
	
	$oSes = new SubacaoEscola();
	$escolas = $oSes->conta($sbaid,$_REQUEST['ano']);
	
	//Pegar perfis do Usuário
	$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']); ?>
	<html>
		<head>
		    <meta http-equiv="Cache-Control" content="no-cache">
		    <meta http-equiv="Pragma" content="no-cache">
		    <meta http-equiv="Connection" content="Keep-Alive">
		    <meta http-equiv="Expires" content="Fri, 9 Oct 1998 05:00:00 GMT">
		    <title>PAR 2010 - Plano de Metas - Subação</title>
		    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		    <link rel="stylesheet" type="text/css" href="../par/css/subacao.css"/>
		    <script type="text/javascript" src="../includes/funcoes.js"></script>
		    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
		</head>
		<body>
			<script type="text/javascript">
				
				$(function() {
					if(!($("[name*='valorP']").val())){
						$("[name*='valorP']").hide();
					}
				});
				
				function verTabelaApoio( picid ){
					url = 'par.php?modulo=principal/cadastraItensComposicao&acao=A&carregaitem=' + picid;
					window.open(url, 'popupTabelaApoio', "height=600,width=800,scrollbars=yes,top=50,left=200" );
				}
				
				function verificaEscolas(numero){
					sbacronograma = document.getElementsByName("sbacronograma")[0].value;
					if( sbacronograma == 2 ){
						escolas = document.getElementsByName("escolas")[0].value;
						if( escolas == 0 ){
							alert("Você precisa cadastrar uma escola primeiramente!");
							document.getElementsByName("item[1]")[0].focus();
							return false;
						} else {
							document.getElementsByName("item[" + numero + "]")[0].focus();
							item = document.getElementsByName("item[" + numero + "]")[0].value;
							if(item){
								sbaid = document.getElementsByName("sbaid")[0].value;
								ano = document.getElementsByName("ano")[0].value;
								url = "par.php?modulo=principal/quantidadeEscola&acao=A&ano=" + ano + "&sbaid=" + sbaid;
		    					janela(url,600,500,"Quantidade por escolas")
		    				} else {
								alert("Primeiro selecione o item de composição!");
								return false;
							}
						}
					}
				}
				
				function salvarItemComposicao()
				{
					var erro = 0;
					$("[name*='item']").each(function() {
	   	    			if(this.value == 0){
	   	    				erro = 1;
							alert('Favor preencher o item de composição!');
							this.focus();
							return false;
	   	    			}
					});
	
					$("[name*='icoquantidade']").each(function() {
	   	    			if(this.value == 0){
	   	    				erro = 1;
							alert('Favor preencher a quantidade do item!');
							this.focus();
							return false;
	   	    			}
					});

					$("[name*='valor[']").each(function() {
	   	    			if(this.value == "0,00"){
	   	    				erro = 1;
							alert('Favor preencher o valor do item!');
							this.focus();
							return false;
	   	    			}
					});
					
					$("[class~=obrigatorio]").each(function() { 
						if(!this.value || this.value == "Selecione..."){
							erro = 1;
							alert('Favor preencher todos os campos obrigatórios!');
							this.focus();
							return false;
						}
					});
					if(erro == 0){
						$("#form_itemcomposicao").submit();
					}
				}
				
				function carregaItens(picid, num, perfil, grupo){
					var dicid = 0;
					if(picid){
						if (picid.indexOf("_") >= 0) { // Reprogramação
							dados = picid.split('_');
				       		picid = dados[0];
				       		dicid = dados[1];
						}
						if( grupo ){
							str = "requisicaoAjax=valorUnitario&nome=valor[" + num + "]&numero="+num+"&picid="+picid+"&grupo="+grupo;
						} else {
							if( dicid > 0 ){ // Reprogramação
								var reprogramacao = 1;
								str = "requisicaoAjax=valorUnitario&nome=valor[" + num + "]&numero="+num+"&picid="+picid+"&dicid="+dicid;
							} else {
								str = "requisicaoAjax=valorUnitario&nome=valor[" + num + "]&numero="+num+"&picid="+picid;
							}
						}
						$.ajax({
							   type: "POST",
							   url: window.location,
							   data: str,
							   assync: false,
							   success: function(msg){
							   		dados = msg;
			   	    			 	dados = dados.split('|');
			   	    			 	if( perfil ){
										$("#imagem_" + num).html( num + " " + '<img class="middle link" title="Excluir Item" onclick="removerLinha(\'' + num + '\')"  src="../imagens/excluir.gif" />&nbsp;&nbsp;<img class="middle link" title="Visualizar Tabela de Apoio" onclick="verTabelaApoio( ' + picid + ' )"  src="../imagens/consultar.gif" />' );
									}
			   	    			 	if( dados[0].trim() == '0,00' && reprogramacao != 1 ){
										$("#umiid_" + num).html( dados[1] );
										$("#valor_" + num).html( dados[0] );
										$('[name="valorH[' + num + ']"]').val( dados[0] );
										$('[name="icoquantidade['+num+']"]').val( '0' );
										$('[name="icoquantidade['+num+']"]').attr( 'disabled', 'disabled' );
										$("#valortotal_" + num).html( '0,00' );
										alert("Não existe valor cadastrado para o item.");
			   	    			 	}else{
										$("#umiid_" + num).html( dados[1] );
										$('[name="icoquantidade['+num+']"]').attr( 'disabled', '' );
			   	    			 		if( dados[2] == '' && reprogramacao != 1 ){ //É pregão
											$('[name="valorP[' + num + ']"]').hide();
											$('[name="valorH[' + num + ']"]').val( dados[0] );
											$("#valor_" + num).html( dados[0] );
										} else { //Não é pregão
											percent = new Number( dados[2] );
											valor = new Number( dados[0].replace(",", ".") );
											valorminimo = new Number( valor - ( (valor*percent)/100 ) );
											valormaximo = new Number( valor + ( (valor*percent)/100 ) );
											$('[name="valorMin[' + num + ']"]').val( valorminimo );
											$('[name="valorMax[' + num + ']"]').val( valormaximo );
											//adicionei agora!!!
											$('[name="valorP[' + num + ']"]').val(dados[0]);
											$('[name="valorP[' + num + ']"]').show();
											$('[name="valor[' + num + ']"]').val(dados[0]);
										}
			   	    			 	}
							   }
						 });
					} else {
						return false;
					}
				}
	
				function calculaValorItem(valor, numero)
				{

					if( document.getElementsByName("valorP[" + numero + "]")[0].value ){
						valor = document.getElementsByName("valorP[" + numero + "]")[0].value;
					} else {
						valor = document.getElementsByName("valorH[" + numero + "]")[0].value;
					}
					
					qtde = document.getElementsByName("icoquantidade[" + numero + "]")[0].value;
					
					if(qtde){
						qtde = mascaraglobal('[#]',qtde);
					}else{
						qtde = 0;
					}
					
					if(valor){
						valor = mascaraglobal('[#]',valor);
					}else{
						valor = 0; 
					}
					
					var total = qtde * valor;
					$("#valortotal_" + numero).html( mascaraglobal('[.###],##',total) );
					$('[name="valortotalH[' + numero + ']"]').val( mascaraglobal('[.###],##',total) );
				}
				function novoItem(grupo)
				{ 
					var num = $("#tbl_item tr").length;
					var str = 0;
					sbacronograma = document.getElementsByName("sbacronograma")[0].value;
	
					if( sbacronograma == 1 ){
						$("#tbl_item").append('<tr id="tr_' + num + '" ><td id="imagem_' + num + '" align="center" >' + num + " " + '<img class="middle link" title="Excluir Item" onclick="removerLinha(\'' + num + '\')"  src="../imagens/excluir.gif" /><input type="hidden" name="valorMin[' + num + ']"><input type="hidden" name="valorMax[' + num + ']"><input type="hidden" name="valorH[' + num + ']"><input type="hidden" name="valorP[' + num + ']"><input type="hidden" name="valortotalH[' + num + ']"></td><td id="item_' + num + '" align="center" ></td><td id="umiid_' + num + '" align="center" ></td><td id="icoquantidade_' + num + '" align="center" ></td><td  id="valor_' + num + '" align="center" ></td><td  id="valortotal_' + num + '" align="center" ></td></tr>');
					} else {
						$("#tbl_item").append('<tr id="tr_' + num + '" ><td id="imagem_' + num + '" align="center" >' + num + " " + '<img class="middle link" title="Excluir Item" onclick="removerLinha(\'' + num + '\')"  src="../imagens/excluir.gif" /><input type="hidden" name="valorH[' + num + ']"><input type="hidden" name="valortotalH[' + num + ']"></td><td id="item_' + num + '" align="center" ></td><td id="umiid_' + num + '" align="center" ></td><td  id="valor_' + num + '" align="center" ></td></tr>');
					}
					$("[name*='item']").each(function() {
	   	    			if($(this).val()){
	   	    				str = str + '|' + $(this).val();
	   	    			}
					});

					if(grupo){
						req1 = "requisicaoAjax=comboItem&nome=item[" + num + "]&numero="+num+"&grupo="+grupo+"&condicao='"+str+"'";
					} else {
						req1 = "requisicaoAjax=comboItem&nome=item[" + num + "]&numero="+num+"&condicao='"+str+"'";
					}

					JqueryAjax(req1,"item_" + num);
					if( sbacronograma == 1 ){
						JqueryAjax("requisicaoAjax=texto_qtde&nome=icoquantidade[" + num + "]&numero="+num,"icoquantidade_" + num);
						JqueryAjax("requisicaoAjax=texto_valorTotal&nome=valortotal[" + num + "]","valortotal_" + num);
						$('[name="icoquantidade['+num+']"]').attr( 'disabled', 'disabled' );
					}
					JqueryAjax("requisicaoAjax=texto_valor&nome=valorP[" + num + "]&numero="+num,"valor_" + num);
					$("[name='valorP["+num+"]']").hide();
				}
				function novoGrupo(){
					document.getElementById('div_grupo').style.display='block';
				}
				function removerLinha(num)
				{
					$("#tr_" + num).remove();
				}
				
				function carregarGrupoItem(){
					gicid = $('#gicid').val();
					document.getElementById('div_grupo').style.display='none';
					
					if( gicid != '' ){
				   		if( confirm("Tem certeza que deseja adicionar os itens do grupo selecionado?") ){
						divCarregando();
						$.ajax({
							   type: "POST",
							   url: window.location,
							   data: "requisicaoAjax=recuperaItensGrupo&gicid="+gicid,
							   assync: false,
							   success: function(msg){
							   		dados = msg.split('|');
							   		
							   		var st;
							   		var numeroInicio = $('select[name^="item["]').length + 1;
							   		for( i=0; i<dados.length; i++ ){
								   		
							   			verdadeiro = true;
							   			numero = $('select[name^="item["]').length + 1;
							   			$('select[name^="item["]').each(function(){
								   			if( dados[i].trim() == $(this).val() ){
								   				verdadeiro = false;
								   			}
										});
							   			
							   			if( verdadeiro ){
								   			novoItem(gicid);
								   			$('select[name="item['+numero+']"]').val(dados[i]);
								   			//carregaItens(dados[i], numero, 1, gicid);
								   			st = st + '|' + dados[i];
	
								   		}
							   		}
							   		$('#grupopedido').val(gicid+'-'+st);
	
							   		for( i=0; i<dados.length; i++ ){
							   			carregaItens(dados[i], numeroInicio, 1, gicid);
							   			numeroInicio++;
							   		}
							   		
									divCarregado();
									$('#td_1').append('<img src="../imagens/excluir.gif" onclick="removerLinha(\'1\')" title="Excluir Item" class="middle link">');
							   }
						 });
						}
					} else {
						alert('Selecione um grupo!');
						return false;
					}
				}
				
				function deletarRegistro( id, num ){
				
					if( confirm( "Tem certeza que deseja excluir esse registro?" ) ){
						$.ajax({
							   type: "POST",
							   url: window.location,
							   data: "requisicaoAjax=deletaRegistro&id="+id,
							   success: function(msg){
							   		removerLinha(num);
							   		//window.opener.document.location.reload();
							   }
						 });
					} else {
						return false;
					}
				}
				function JqueryAjax(params,destino)
				{
					$.ajax({
						   type: "POST",
						   url: window.location,
						   data: params,
						   async: false,
						   success: function(msg){
								$("#" + destino).html(msg);
						   }
					 });
				}

				function atualizarValorReferenciaItens(sbaid,ano,sbacronograma,picid){
					if(confirm("Deseja Atualizar o Valor Referência dos itens?")) {
						$.ajax({
							type: "POST",
							url: 'par.php?modulo=principal/subacaoItemComposicao&acao=A&sbaid='+sbaid+'&ano='+ano+'&sbacronograma='+sbacronograma+'&picid='+picid,
							data: { atualizar_valor_referencia: "true"},
							async: false,
							success: function(result) {
							$("#debug").html(result);
							if(result=='t'){
								alert("Valor de Referência atualizado com sucesso!");
							}else{
								alert("Valor de Referência não atualizado!");
							}
		  				}});
					} else {
						alert("Valor de Referência não atualizado!");
					}
					window.location.href = window.location.href;
				}
				
			</script>
			<form name="form_itemcomposicao" id="form_itemcomposicao" method="post" action="" >
				<input type="hidden" name="sbaid" value="<?php echo $sbaid ?>" />
				<input type="hidden" name="requisicao" value="salvarItemComposicao" />
				<input type="hidden" name="grupopedido" id="grupopedido" value="" />
				<input type="hidden" name="sbacronograma" value="<?php echo $_REQUEST['sbacronograma']; ?>" />
				<input type="hidden" name="ano" value="<?php echo $_REQUEST['ano']; ?>" />
				<input type="hidden" name="ppsid" value="<?php echo $sba['ppsid']; ?>" />
				<input type="hidden" name="escolas" value="<?php echo $escolas; ?>" />
				<div id="debug"></div>
				<table border="0" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<?php 
					
					$sql = "SELECT 
								iu.itrid,
								CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE iu.mun_estuf END as estuf,
								CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE iu.muncod END as entidade
							FROM
								par.subacao s
							INNER JOIN par.acao a ON a.aciid = s.aciid
							INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
							INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
							WHERE
								s.sbaid = ".$_REQUEST['sbaid'];
										
					$entidadeItem = $db->pegaLinha($sql);
					
					$sql = "SELECT true FROM par.empenhosubacao WHERE sbaid = {$_REQUEST['sbaid']} AND eobano = {$_REQUEST['ano']} AND eobstatus = 'A'";
					$empenhado = $db->pegaUm($sql);
					
					$sql = "SELECT 't' FROM par.termocomposicao tc
							INNER JOIN par.vm_documentopar_ativos   dp ON dp.dopid = tc.dopid
							INNER JOIN par.subacaodetalhe sd ON sd.sbdid = tc.sbdid
							WHERE
								sd.sbaid = {$_REQUEST['sbaid']} AND sd.sbdano = {$_REQUEST['ano']}";

					$termo = $db->pegaUm($sql);
					
					if( (
							($subacao['sbareformulacao'] == 't' && ( $ssuid == 20 || $ssuid == 21 )) || 
						//	($sbdreformulacao != 't' && $empenhado != 't')
							($termo != 't')
						) && 
						( in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ) ){ 
					
// 						if(in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) 
// 							|| in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) 
// 							|| in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrayPerfil) 
// 							|| in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrayPerfil) 
// 							|| in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) 
// 							|| in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrayPerfil)
// 							|| in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil)){?>
					<tr>
						<td align="center" colspan="2"><input type="button" name="escolas" value="Atualizar Valor Referência de TODOS os itens." onclick="atualizarValorReferenciaItens(<?php echo $_REQUEST['sbaid']; ?>,<?php echo $_REQUEST['ano']; ?>,<?php echo $_REQUEST['sbacronograma']; ?>,0);"/></td>
					</tr>
					<?php } //} ?>
					<tr style="background-color: #cccccc;">
						<td align="center" colspan="2"><strong>Itens de Composição</strong></td>
					</tr>
					<tr>
						<td align="left" colspan="2">
							<?php $oIC = new SubacaoItensComposicao();?>
							<?php $arrDadosIC = $oIC->retornaItensComposicaoPorSubacao($_GET['sbaid'],$_GET['ano'])?>
							<?php 
							if( $_REQUEST['sbacronograma'] == 1 ){
								//$w = "psi.ppsid = (select ppsid from par.subacao where sbaid = {$_GET['sbaid']}) AND";
								$td1 = '<td class="bold" >Qtde</td>';
								$td2 = '<td class="bold" >Valor Total(R$)</td>';
							}
							$sql = "SELECT DISTINCT
                                                                    sic.icoid,
                                                                    pic.picid,
                                                                    sic.dicid,
                                                                    pic.umiid,
                                                                    uni.umidescricao,
                                                                    pic.picdescricao as item,
                                                                    sic.icoquantidade,
                                                                    sic.icovalor as valor,
                                                                    --dicdatainicial,
                                                                    --dicdatafinal,
                                                                    uf.estuf,
                                                                    dic.dicpercentual as percentual,
                                                                    sic.gicid,
                                                                    sic.dicid as dicidsubacao,
                                                                    (select count(sccid) FROM par.subacaoitenscomposicaocontratos WHERE icoid = sic.icoid) as contrato,
                                                                    icosequencia
                                                                FROM par.propostaitemcomposicao pic
                                                                LEFT JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A' --AND now() between dicdatainicial and dicdatafinal
                                                                LEFT JOIN par.propostasubacaoitem psi ON psi.picid = pic.picid
                                                                LEFT JOIN par.propostatipopregao ptp ON ptp.ptpid = dic.ptpid AND ptpstatus = 'A'
                                                                INNER JOIN par.ufdetalheitemcomposicao uf   ON dic.dicid   = uf.dicid ".( $entidadeItem['itrid'] == 1 ? " AND estuf = '{$entidadeItem['entidade']}' " : " AND estuf = ( select estuf from territorios.municipio where muncod = '{$entidadeItem['entidade']}' ) ")."
                                                                INNER JOIN par.subacaoitenscomposicao sic ON sic.picid = pic.picid AND sic.sbaid = {$_GET['sbaid']} AND sic.icostatus = 'A' AND sic.icoano = {$_GET['ano']} AND sic.dicid = dic.dicid
                                                                INNER JOIN par.unidademedidadetalhamentoitem uni ON pic.umiid = uni.umiid AND uni.umistatus = 'A'
                                                                WHERE pic.picstatus = 'A'
                                                                    {$w} 
                                                                ORDER BY pic.picdescricao";
                                                        $isOnibusAcessivel = isOnibusAcessivel($_GET['sbaid'])
												
							?>
							<?php 
								$arrDados = $db->carregar($sql); ?>
							<table id="tbl_item" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
								<tr class="SubTituloTabela bold" >
									<td class="bold" >Ação</td>
									<td class="bold" >Item</td>
									<td class="bold" >Unidade de Medida</td>
									<?php echo $td1; ?>
									<td class="bold" >Valor Referência(R$)</td>
									<?php echo $td2; ?>
								</tr>
								<?php $condicao = 0; ?>
								<?php 
									if( possuiPerfil(array(PAR_PERFIL_SUPER_USUARIO)) ){
										$tabelaApoio = '&nbsp;&nbsp;<img class="middle link" title="Visualizar Tabela de Apoio" onclick="verTabelaApoio( '.$dado['picid'].' )"  src="../imagens/consultar.gif" />';	
									}
									if($arrDados){
										foreach($arrDados as $chave => $dado){
											$chave % 2 ? $cor = "#dedfde" : $cor = "";
											$chave = $chave + 1; 
											$condicao .= ", ".$dado['picid'];
											$valorminimo = '';
											$valormaximo = '';
											if( !is_null($dado['percentual']) ){
												$valorminimo = ( $dado['valor'] - ( ($dado['valor']*$dado['percentual'])/100 ) );
												$valormaximo = ( $dado['valor'] + ( ($dado['valor']*$dado['percentual'])/100 ) );
											}
										?>
										<tr bgcolor="<?=$cor ?>" id="tr_<?=$chave; ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
										<!--<tr id="tr_<?php echo $chave ?>" >-->
											<td align="center" >
												<?php 
                                                                                                
                                                                                                #Verifica se a subacao esta em situacao de aguardando Municipio / Estado
                                                                                                $sql = "
                                                                                                    SELECT DISTINCT 
                                                                                                        count(s.sbaid)
                                                                                                    FROM 
                                                                                                        par.processopar prp
                                                                                                    INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
                                                                                                    INNER JOIN par.subacaodetalhe sd  ON sd.sbdid = ppc.sbdid
                                                                                                    INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
                                                                                                    INNER JOIN par.vm_documentopar_ativos dop ON dop.prpid = prp.prpid
                                                                                                    INNER JOIN par.modelosdocumentos mdo ON mdo.mdoid = dop.mdoid
                                                                                                    INNER JOIN par.documentoparreprogramacaosubacao dpr ON dpr.dopid = dop.dopid
                                                                                                    INNER JOIN par.instrumentounidade inu ON inu.inuid = prp.inuid
                                                                                                    LEFT  JOIN territorios.municipio mun ON mun.muncod = inu.muncod
                                                                                                    LEFT  JOIN territorios.estado est ON est.estuf = inu.estuf
                                                                                                    WHERE
                                                                                                        dopreformulacao IS TRUE 
                                                                                                        AND (
                                                                                                            SELECT count(sd2.sbdid) 
                                                                                                            FROM par.processoparcomposicao ppc 
                                                                                                            LEFT JOIN par.subacaodetalhe sd2 ON ppc.sbdid = sd2.sbdid AND sd2.ssuid IN (20, 23) 
                                                                                                            WHERE ppc.ppcstatus = 'A' 
                                                                                                            and dop.prpid = ppc.prpid
                                                                                                        ) > 0 
                                                                                                        AND dpr.dpsstatus = 'A'
                                                                                                        AND s.sbaid = {$_REQUEST['sbaid']}";
                                                                                                        
                                                                                                        $booSitMunicipioEstado = $db->pegaLinha($sql);
                                                                                                
                                                                                                
                                                                                                echo $chave;
                                                                                                if( (
                                                                                                        ($subacao['sbareformulacao'] == 't' && ( $ssuid == 20 || $ssuid == 21 )) || 
                                                                                                        ($termo != 't')
                                                                                                    ) && 
                                                                                                    (in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) 
                                                                                                    || in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil)) ){?>
												<img class="middle link" title="Atualizar Valor Referência do Item" src="../imagens/refresh.gif" 
													onclick="atualizarValorReferenciaItens(<?=$_REQUEST['sbaid']; ?>,<?=$_REQUEST['ano']; ?>,<?=$_REQUEST['sbacronograma']; ?>, <?=$dado['picid'] ?>);" />
												<?php }
                                                                                                #Dar permissão ao municipio e estado para atualizar o valor do item da subacao Demanda do PAR 1361
												else if ($booSitMunicipioEstado > 0 && (in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrayPerfil) 
                                                                                                    || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrayPerfil) 
                                                                                                    || in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) 
                                                                                                    || in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrayPerfil)
                                                                                                    || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrayPerfil)))
												{ ?>
													<img class="middle link" title="Atualizar Valor Referência do Item" src="../imagens/refresh.gif" 
													onclick="atualizarValorReferenciaItens(<?=$_REQUEST['sbaid']; ?>,<?=$_REQUEST['ano']; ?>,<?=$_REQUEST['sbacronograma']; ?>, <?=$dado['picid'] ?>);" />
												<?php } else {
													if($isOnibusAcessivel)
													{?>
														<img class="middle link" title="Atualizar Valor Referência do Item" src="../imagens/refresh.gif" 
														onclick="atualizarValorReferenciaItens(<?=$_REQUEST['sbaid']; ?>,<?=$_REQUEST['ano']; ?>,<?=$_REQUEST['sbacronograma']; ?>, <?=$dado['picid'] ?>);" />
													<?php
													}
												}
												if( $dado['contrato'] == 0 ){
													if(!$isOnibusAcessivel)
													{												
												?>
													<img class="middle link" title="Excluir Item" onclick="deletarRegistro( <?php echo $dado['icoid']; ?>, <?php echo $chave; ?> )" src="../imagens/excluir.gif" />
												<?	}	
												}else{?>
													<img class="middle link" title="Item não pode ser excluido porque tem contrato vinculado" src="../imagens/excluir_01.gif" />
												<?}?>
												<?php echo $tabelaApoio; echo $dado['acao']; ?>
												<input type="hidden" name="valorH[<?php echo $chave ?>]" 
													value="<?php echo number_format($dado['valor'],2,',','.') ?>">
												<input type="hidden" name="valorP[<?php echo $chave ?>]" value="">
												<input type="hidden" name="valortotalH[<?php echo $chave ?>]" value="<?php echo number_format($dado['icoquantidade']*$dado['valor'],2,',','.') ?>">
												<input type="hidden" name="valorMin[<?php echo $chave ?>]" value="<?php echo $valorminimo ?>">
												<input type="hidden" name="valorMax[<?php echo $chave ?>]" value="<?php echo $valormaximo ?>">
											</td>
											<td align="center" ><?php comboItem("item[$chave]", $dado['picid'], $chave, null, $dado['gicid'], $dado['dicidsubacao'], $dado['icosequencia']) ?></td>
											<td align="center" id="umiid_<?php echo $chave ?>"><?php echo $dado['umidescricao'] ?></td>
											<?php if( $_REQUEST['sbacronograma'] == 1 ){ ?>
												<td align="center" ><?php texto_qtde("icoquantidade[$chave]", $chave, $dado['icoquantidade']) ?></td>
											<?php } ?>
											<td align="center" id="valor_<?php echo $chave ?>">
											<?php 
											if( possuiPerfil(array(PAR_PERFIL_SUPER_USUARIO)) ){ echo texto_valor("valor[$chave]", $dado['valor'], $chave); } else { echo number_format($dado['valor'],2,',','.'); } ?>
											</td>
											<?php if( $_REQUEST['sbacronograma'] == 1 ){ ?>
												<td align="center" id="valortotal_<?php echo $chave ?>" ><?php echo number_format($dado['icoquantidade']*$dado['valor'],2,',','.') ?></td>
											<?php } ?>
										</tr>	
									<?php 
										}
									} else {
										$num = $arrDados ? count($arrDados) : "1" ?>
								<tr id="tr_<?php echo $num ?>" >
									<td align="center"  id="td_<?php echo $num ?>" > <?php echo $num ?> <input type="hidden" name="valorMin[<?php echo $num ?>]"><input type="hidden" name="valorMax[<?php echo $num ?>]"><input type="hidden" name="valorH[<?php echo $num ?>]"><input type="hidden" name="valortotalH[<?php echo $num ?>]"></td>
									<td align="center" ><?php comboItem("item[$num]", null, $num, $condicao, $dado['gicid']) ?><input type="hidden" name="picid[]" value="<?php echo $dado['picid'] ?>"  /></td>
									<td align="center" id="umiid_<?php echo $num ?>"><?php // comboUnidadeMedida("umiid[$num]") ?></td>
									<?php if( $_REQUEST['sbacronograma'] == 1 ){ ?>
										<td align="center" ><?php texto_qtde("icoquantidade[$num]", $num) ?></td>
									<?php } ?>
									<td align="center" id="valor_<?php echo $num ?>"><?php texto_valor("valorP[$num]", null, $num) ?></td>
									<?php if( $_REQUEST['sbacronograma'] == 1 ){ ?>
										<td align="center" id="valortotal_<?php echo $num ?>" ></td>
									<?php } ?>
								</tr>
								<?php } ?>
							</table>
						</td>
					</tr>
						<?php 
							$strAdd = ' / <a href="javascript:novoGrupo()">Adicionar Grupo de Itens</a>';
							$strObs = '<br>*Obs: Para confirmar as alterações clique no botão "Salvar".'; 
							
							if( $_REQUEST['pregoesvencidos'] == 1 ){
								$sql = "SELECT ssuid FROM par.subacaodetalhe WHERE sbaid = ".$_REQUEST['sbaid']." AND sbdano = ".$_REQUEST['ano'];
								$ssuid = $db->pegaUm( $sql );
								
								$strObs .= '<br>**Obs: Itens em <span style="color:red" >vermelho</span> indicam o item atual da subação.';
								$strObs .= '<br>***Obs: Itens em <span style="color:blue" >azul</span> indicam o pregão vigênte de cada item.';
							}
						?>
					<tr style="background-color: #cccccc;">
						<td align="left" colspan="2">
							<img class="middle link" src="../imagens/gif_inclui.gif" /> <a href="javascript:novoItem()">Adicionar Item</a> <?php echo $strAdd; ?>
							<span class="bold" ><?=$strObs ?></span>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita"></td>
						<td class="SubTituloEsquerda" >
							<?php 
								$isOnibusAcessivel = isOnibusAcessivel($_GET['sbaid']);
								if ( ($subacao['sbareformulacao'] == 't') && ($isOnibusAcessivel) ){ ?>
									
								<?php 
								}
								else
								{
								?>
									<input type="button" value="Salvar" name="btn_salvar" onclick="salvarItemComposicao()" />
								<?php 
								}
								?>
							<input type="button" value="Fechar" name="btn_fechar" onclick="window.opener.document.location.reload(); window.close();" />
						</td>
					</tr>
				</table>
				<div id="div_grupo" style="width:270px; height:120px; position:absolute; z-index:0; top:50%; left:50%; margin-top:-125px; margin-left:-60px; border:solid 2px black; background-color:#FFFFFF; display:none;" >
					<div style="width:100%;text-align:right">
						<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('div_grupo').style.display='none'" />
					</div>
					<div style="padding:5px;text-align:justify;">
						<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
						<tr>
							<td width="30%" class="SubtituloDireita">Grupo:</td>
							<td><?php
									$sql = "SELECT 
												gic.gicid as codigo, 
												gic.gicdescricao as descricao
											FROM 
												par.grupo_itemcomposicao gic
											INNER JOIN par.propostagrupoitem pgi ON pgi.gicid = gic.gicid
											INNER JOIN par.subacao s ON s.ppsid = pgi.ppsid
											WHERE  
												gic.gicstatus = 'A' AND
												s.sbaid = ".$_REQUEST['sbaid']."
											ORDER BY 
												gic.gicdescricao";
				
									$db->monta_combo('gicid', $sql, 'S', 'Selecione', '', '', '', '200', 'N', 'gicid'); 
									?></td>
						</tr>
						<tr>
							<td align="center" bgcolor="#D5D5D5" colspan="2">
								<input type="button" name="btn_enviar" onclick="carregarGrupoItem()" value="ok" />
								<input type="button" name="btn_cancelar" onclick="document.getElementById('div_grupo').style.display='none'" value="cancelar" />
							</td>
						</tr>
						</table>
					</div>
				</div>
			</form>
		</body>
	</html>
<?php } else { //VEM DO MENU!!
	
	if( $_REQUEST['adicionaItens'] ){
		global $sb;

		$ano = $_REQUEST['ano'];
		/*
		$sql = "SELECT DISTINCT
					'<center><img class=\"middle link\" title=\"Excluir Escola\" src=\"../imagens/excluir.gif\" onclick=\"excluirItem(this,\'' || pic.picid || '\')\"  /></center>' as acao,
					pic.picdescricao as descricao,
		            uni.umidescricao as unidade,
		            '<input type=\"text\" id=\"itemvalor\" name=\"itemvalor[{$ano}][' || pic.picid || ']\" value=\"' || dic.dicvalor || '\">' as valor
				FROM
					par.propostaitemcomposicao pic
				LEFT JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A' AND (now()::date between dic.dicdatainicial and dic.dicdatafinal)
				INNER JOIN par.propostasubacaoitem psi ON psi.picid = pic.picid
				LEFT JOIN par.pregaouf puf ON puf.ptpid = dic.ptpid
				LEFT JOIN par.propostatipopregao ptp on ptp.ptpid = puf.ptpid AND ptpstatus = 'A'
				INNER JOIN par.subacao s ON s.ppsid = psi.ppsid AND s.sbastatus = 'A'
				INNER JOIN par.unidademedidadetalhamentoitem uni ON pic.umiid = uni.umiid
				INNER JOIN par.ufdetalheitemcomposicao ufdic ON ufdic.dicid = dic.dicid
				WHERE
					pic.picid IN (".$_REQUEST['itensselecionados'].")";
		*/
		
		
		$itens = array();

		if( $_POST['grupopedido'] ){
			list($gicid, $demais) = split('-',$_POST['grupopedido']);
			$arDados = explode("|", trim($demais));
		}
		$i = 1;
		//ver($gicid, $demais, $arDados, d);
		foreach( $_POST['item'] as $v => $item ){
			$sql = "SELECT DISTINCT
						pic.picdescricao as descricao,
			            uni.umidescricao as unidade
			        FROM
						par.propostaitemcomposicao pic
					INNER JOIN par.unidademedidadetalhamentoitem uni ON pic.umiid = uni.umiid
					WHERE
						pic.picid = ".$item;
			$dados = $db->pegaLinha( $sql );
			if($_POST['grupopedido'] && in_array( $item, $arDados )){
				$itens[] = array(
					0 => '<center>'. $i . ' <img class="middle link" title="Excluir Escola" src="../imagens/excluir.gif" onclick="excluirItem(this,'.$item.')"  /></center>',
					1 => $dados['descricao'],
					2 => $dados['unidade'],
					3 => '<input type="text" id="itemvalor" name="itemvalor['.$ano.']['.$item.']['.$gicid.']" value="'.$_POST['icoquantidade'][$v].'">'
				);
			} else {
				$itens[] = array(
					0 => '<center>'. $i . ' <img class="middle link" title="Excluir Escola" src="../imagens/excluir.gif" onclick="excluirItem(this,'.$item.')"  /></center>',
					1 => $dados['descricao'],
					2 => $dados['unidade'],
					3 => '<input type="text" id="itemvalor" name="itemvalor['.$ano.']['.$item.'][]" value="'.$_POST['icoquantidade'][$v].'">'
				);
			}
			$i++;
		}

		$cabecalho = array("Ação","Descrição","Unidade de Medida","Quantidade");
		return $db->monta_lista_simples($itens,$cabecalho,100,5,"N","100%");
	}
	
	function recuperaItensGrupo(){
		
		global $db;
		
		$gicid = $_POST['gicid'];
		
		$sql = "SELECT 
					pg.picid||'-'||pg.pgiqtd 
				FROM 
					par.propostaitem_grupoitem pg
				INNER JOIN par.propostaitemcomposicao pic ON pic.picid = pg.picid AND pic.picstatus = 'A' 
				WHERE 
					pg.gicid = ".$gicid." 
				ORDER BY 
					pg.picid";
		$itens = $db->carregarColuna( $sql );
		
		echo implode("|",$itens);
	}
	
	function comboItem($nome = "item[]", $value = null, $numero = null, $condicao = null, $grupo = null){
		global $db;
		
		$estuf = $_SESSION['par']['mun_estuf'] ? $_SESSION['par']['mun_estuf'] : $_SESSION['par']['estuf'];
	
		//$sql = "SELECT ppsid, ptsid FROM par.subacao WHERE sbaid = ".$_REQUEST['sbaid'];
		//$sba = $db->pegaLinha( $sql );
	
		$nome = !$_POST['nome'] ? $nome : $_POST['nome'];
		$numero = !$_POST['numero'] ? $numero : $_POST['numero'];
		$condicao = !$_POST['condicao'] ? $condicao : $_POST['condicao'];
		$condicao = str_replace( array("\\", "'"), "", $condicao);
		$grupo = !$_POST['grupo'] ? $grupo : $_POST['grupo'];
		$where = '';
		if( $condicao ){
			$where = " AND pic.picid NOT IN (".$condicao.")";
		}
		
			if( $grupo ){
				$sql = "SELECT  
							pic.picid as codigo, 
				            pic.picdescricao as descricao 
		               FROM par.propostaitemcomposicao pic
		               INNER JOIN par.propostaitem_grupoitem g ON g.picid = pic.picid
		               WHERE 
		               		g.gicid = ".$grupo." AND
		               		pic.picstatus = 'A' --AND 
		               	--	pic.picid NOT IN (SELECT picid FROM par.subacaoitenscomposicao WHERE sbaid = ".$_REQUEST['sbaid']." AND icoano = ".$_REQUEST['ano'].") 
		               		".$where ." 
		               	ORDER BY
		               		descricao";
			} else {
		
		//if( $sba['ppsid'] ){ // veio do Guia
			$sql = "SELECT DISTINCT codigo, descricao, title FROM
				(SELECT
						pic.picid as codigo,
						pic.picdescricao as descricao,
			            pic.picdetalhe as title,
			            dic.dicvalor as valoritem,
			            dic.dicdatainicial, dic.dicdatafinal,
			            to_char(dic.dicdatainicial, 'YYYY') as anoini,
			            to_char(dic.dicdatafinal, 'YYYY') as anofim, 
			            puf.estuf
					FROM
						par.propostaitemcomposicao pic
					INNER JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A' AND (now()::date between dic.dicdatainicial and dic.dicdatafinal)
					INNER JOIN par.propostasubacaoitem psi ON psi.picid = pic.picid AND psi.ppsid = {$_REQUEST['ppsid']}
					LEFT JOIN par.pregaouf puf ON puf.ptpid = dic.ptpid  and estuf = '{$estuf}'
					LEFT JOIN par.propostatipopregao ptp on ptp.ptpid = puf.ptpid AND ptpstatus = 'A'
					WHERE
						pic.picstatus = 'A' ".$where ." 
		UNION ALL
			 SELECT
					pic.picid as codigo,
					pic.picdescricao as descricao,
					pic.picdetalhe as title,
					dic.dicvalor as valoritem,
					dic.dicdatainicial, dic.dicdatafinal,
					to_char(dic.dicdatainicial, 'YYYY') as anoini,
					to_char(dic.dicdatafinal, 'YYYY') as anofim, 
					'' AS estuf
			FROM
				par.propostaitemcomposicao pic
			LEFT JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A' AND dicpercentual is not null
			INNER JOIN par.propostasubacaoitem psi ON psi.picid = pic.picid AND psi.ppsid = {$_REQUEST['ppsid']}
			WHERE pic.picstatus = 'A' {$where} ) as foo
			ORDER BY
				descricao";
		/*} else { //não veio do guia!
			if($sba['ptsid']){
			
			$sql = "SELECT  
						pc.picid as codigo, 
			            pc.picdescricao as descricao 
	               FROM par.propostaitemcomposicao pc
	               INNER JOIN par.propostaitemtiposubacao pi ON pc.picid = pi.picid
	               INNER JOIN par.detalheitemcomposicao dic ON dic.picid = pc.picid  AND (now()::date between dic.dicdatainicial and dic.dicdatafinal)  
	               INNER JOIN par.ufdetalheitemcomposicao ufdi ON ufdi.dicid = dic.dicid
	               WHERE 
	               		pi.ptsid = ".$sba['ptsid']."
	               		AND pc.picstatus = 'A'  
	               		AND dicstatus = 'A'
	               		AND ufdi.estuf = '{$_SESSION['par']['estuf']}'";
	
			} else {
				echo "<script>
						alert('Falta o plano de trabalho!');
						window.close();
					</script>";
			}
		}*/
			}
	
		$perfil = possuiPerfil(array(PAR_PERFIL_SUPER_USUARIO));

		return $db->monta_combo($nome,$sql,"S","Selecione...","javascript:carregaItens(this.value, '$numero', '$perfil');","","","150","N","","",$value);
	}
	
	function texto_valorTotal($nome = "valortotal[]", $value = null){
		$nome = !$_POST['nome'] ? $nome : $_POST['nome'];
		echo number_format($arrDadosIC[$dado['picid']]*$dado['valor'],2,',','.');
	//	calculaValorItem(this.value,'{$dado['picid']}','{$dado['valor']}')
	}
	
	function texto_qtde($nome = "icoquantidade[]", $numero = null, $value = null){
		$nome = !$_POST['nome'] ? $nome : $_POST['nome'];
		$numero = !$_POST['numero'] ? $numero : $_POST['numero'];

		echo campo_texto($nome,"N","S","","10","20","[.###]","","right","","","onclick='verificaEscolas($numero);'","calculaValorItem(this.value, '$numero')", number_format($value,0,',','.') );
	//	calculaValorItem(this.value,'{$dado['picid']}','{$dado['valor']}')
	}
	
	function valorUnitario($nome = "valor[]", $numero = null, $picid = null, $grupo = null){
		global $db;
		$nome = !$_POST['nome'] ? $nome : $_POST['nome'];
		$numero = !$_POST['numero'] ? $numero : $_POST['numero'];
		$picid = !$_POST['picid'] ? $picid : $_POST['picid'];
		$grupo = !$_POST['grupo'] ? $grupo : $_POST['grupo'];
		
		//$sql = "SELECT ppsid, ptsid FROM par.subacao WHERE sbaid = ".$_REQUEST['sbaid'];
		//$sba = $db->pegaLinha( $sql );
	
		//if( $sba['ppsid'] ){ //veio do guia
			
		if( $grupo ){
			
			$sql = "SELECT DISTINCT
						dic.dicvalor as valoritem,
						uni.umidescricao as descricao,
						dic.dicpercentual as percentual
					FROM
						par.propostaitemcomposicao pic
					LEFT JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A' AND (now()::date between dic.dicdatainicial and dic.dicdatafinal)
					INNER JOIN par.propostaitem_grupoitem pgi ON pgi.picid = pic.picid
					INNER JOIN par.propostagrupoitem pg ON pg.gicid = pgi.gicid
					LEFT JOIN par.pregaouf puf ON puf.ptpid = dic.ptpid  and estuf = '{$estuf}'
					LEFT JOIN par.propostatipopregao ptp on ptp.ptpid = puf.ptpid AND ptpstatus = 'A'
					LEFT JOIN par.subacao s ON s.ppsid = pg.ppsid
					INNER JOIN par.unidademedidadetalhamentoitem uni ON pic.umiid = uni.umiid
					INNER JOIN par.ufdetalheitemcomposicao ufdic ON ufdic.dicid = dic.dicid -- and ufdic.estuf = '{$estuf}'
					WHERE
						pic.picid = ".$picid." AND
						pic.picstatus = 'A' --AND
						--sbaid = ".$_REQUEST['sbaid'];
/*
			$sql = "SELECT DISTINCT
			            dic.dicvalor as valoritem,
			            uni.umidescricao as descricao,
						dic.dicpercentual as percentual
					FROM
						par.propostaitemcomposicao pic
					LEFT JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A' AND (now()::date between dic.dicdatainicial and dic.dicdatafinal)
					LEFT JOIN par.propostasubacaoitem psi ON psi.picid = pic.picid
					LEFT JOIN par.pregaouf puf ON puf.ptpid = dic.ptpid -- and estuf = '{$_SESSION['par']['mun_estuf']}'
					LEFT JOIN par.propostatipopregao ptp on ptp.ptpid = puf.ptpid AND ptpstatus = 'A'
					LEFT JOIN par.subacao s ON s.ppsid = psi.ppsid AND s.sbastatus = 'A'
					INNER JOIN par.unidademedidadetalhamentoitem uni ON pic.umiid = uni.umiid
					INNER JOIN par.ufdetalheitemcomposicao ufdic ON ufdic.dicid = dic.dicid -- and ufdic.estuf = '{$_SESSION['par']['mun_estuf']}'
					WHERE
						pic.picid = ".$picid." --AND
						--sbaid = ".$_REQUEST['sbaid'];
						*/
		} else {
			$sql = "SELECT DISTINCT
						dic.dicvalor as valoritem,
						uni.umidescricao as descricao,
						dic.dicpercentual as percentual
					FROM
						par.propostaitemcomposicao pic
					LEFT JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A' AND (now()::date between dic.dicdatainicial and dic.dicdatafinal)
					INNER JOIN par.propostasubacaoitem psi ON psi.picid = pic.picid
					LEFT JOIN par.pregaouf puf ON puf.ptpid = dic.ptpid -- and estuf = '{$_SESSION['par']['mun_estuf']}'
					LEFT JOIN par.propostatipopregao ptp on ptp.ptpid = puf.ptpid AND ptpstatus = 'A'
					INNER JOIN par.subacao s ON s.ppsid = psi.ppsid AND s.sbastatus = 'A'
					INNER JOIN par.unidademedidadetalhamentoitem uni ON pic.umiid = uni.umiid
					INNER JOIN par.ufdetalheitemcomposicao ufdic ON ufdic.dicid = dic.dicid -- and ufdic.estuf = '{$_SESSION['par']['mun_estuf']}'
					WHERE
						pic.picid = ".$picid." --AND
						--sbaid = ".$_REQUEST['sbaid'];
		} 
		/*} else { //não veio do guia!
	
			$sql = "SELECT  
						dic.dicvalor as valoritem,
						uni.umidescricao as descricao,
						'0' as pregao,
						'0' as percentual
	               FROM par.propostaitemcomposicao pc
	               INNER JOIN par.propostaitemtiposubacao pi ON pc.picid = pi.picid
	               INNER JOIN par.detalheitemcomposicao dic ON dic.picid = pc.picid  AND (now()::date between dic.dicdatainicial and dic.dicdatafinal)  
	               INNER JOIN par.ufdetalheitemcomposicao ufdi ON ufdi.dicid = dic.dicid
	               INNER JOIN par.unidademedidadetalhamentoitem uni ON pc.umiid = uni.umiid
	               WHERE 
	               		pi.ptsid = ".$sba['ptsid']."
	               		AND pc.picstatus = 'A'  
	               		AND dicstatus = 'A'
	               		AND pc.picid = ".$picid."
	               		AND ufdi.estuf = '{$_SESSION['par']['estuf']}'";
	
		}*/
	
		$valor = $db->pegaLinha( $sql );
		echo number_format($valor['valoritem'],2,',','.').'|'.$valor['descricao'].'|'.$valor['percentual'];
	
	}
	
	function texto_valorMenu($nome = "valor[]", $value = null, $numero = null){
		$nome   = !$_POST['nome'] ? $nome : $_POST['nome'];
		$numero = !$_POST['numero'] ? $numero : $_POST['numero'];
		echo campo_texto($nome,"N","S","","10","20","[.###],##","","right","","","","calculaValorItem(this.value, $numero)", number_format($value,2,',','.') );
	//	calculaValorItem(this.value,'{$dado['picid']}','{$dado['valor']}')
	}	
		
	if($_POST['requisicao']){
		$_POST['requisicao']();
	}
	
	if($_POST['requisicaoAjax']){
		header('content-type: text/html; charset=ISO-8859-1');
		$_POST['requisicaoAjax']();
		die;
	}
		
	?>
	<html>
		<head>
		    <meta http-equiv="Cache-Control" content="no-cache">
		    <meta http-equiv="Pragma" content="no-cache">
		    <meta http-equiv="Connection" content="Keep-Alive">
		    <meta http-equiv="Expires" content="Fri, 9 Oct 1998 05:00:00 GMT">
		    <title>PAR 2010 - Plano de Metas - Subação</title>
			
		    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		    <link rel="stylesheet" type="text/css" href="../par/css/subacao.css"/>
		    <script type="text/javascript" src="../includes/funcoes.js"></script>
		    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
		</head>
		<body>
			<script type="text/javascript">
				
				$(function() {
					if(!($("[name*='valorP']").val())){
						$("[name*='valorP']").hide();
					}
				});
				
				function verTabelaApoio( picid ){
					url = 'par.php?modulo=principal/cadastraItensComposicao&acao=A&carregaitem=' + picid;
					window.open(url, 'popupTabelaApoio', "height=600,width=800,scrollbars=yes,top=50,left=200" );
				}
				
				function verificaEscolas(numero){
					sbacronograma = document.getElementsByName("sbacronograma")[0].value;
					if( sbacronograma == 2 ){
						escolas = document.getElementsByName("escolas")[0].value;
						if( escolas == 0 ){
							alert("Você precisa cadastrar uma escola primeiramente!");
							document.getElementsByName("item[1]")[0].focus();
							return false;
						} else {
							document.getElementsByName("item[" + numero + "]")[0].focus();
							item = document.getElementsByName("item[" + numero + "]")[0].value;
							if(item){
								sbaid = document.getElementsByName("sbaid")[0].value;
								ano = document.getElementsByName("ano")[0].value;
								url = "par.php?modulo=principal/quantidadeEscola&acao=A&ano=" + ano + "&sbaid=" + sbaid;
		    					janela(url,600,500,"Quantidade por escolas")
		    				} else {
								alert("Primeiro selecione o item de composição!");
								return false;
							}
						}
					}
				}
				
				function salvarItemComposicaoMenu()
				{
					
					var erro = 0;
					$("[name*='item']").each(function() {
	   	    			if(this.value == 0){
	   	    				erro = 1;
							alert('Favor preencher o item de composição!');
							this.focus();
							return false;
	   	    			}
					});
	
					$("[name*='icoquantidade']").each(function() {
	   	    			if(this.value == 0){
	   	    				erro = 1;
							alert('Favor preencher a quantidade do item!');
							this.focus();
							return false;
	   	    			}
					});
					
					$("[class~=obrigatorio]").each(function() { 
						if(!this.value || this.value == "Selecione..."){
							erro = 1;
							alert('Favor preencher todos os campos obrigatórios!');
							this.focus();
							return false;
						}
					});
					
					jQuery( '#itens' ).val( '0' );

					$("[name^='item[']").each(function(){
						itens = jQuery( '#itens' ).val();
						jQuery( '#itens' ).val( itens + ',' + $(this).val() );
					});

					itensselecionados = jQuery( '#itens' ).val();
					if( jQuery( '#sbacronograma' ).val() == 1 ){
						quantidade = jQuery( '#itens' ).val()
					}
					ano = jQuery( '#ano' ).val();

					if(erro == 0){
						divCarregando();
						jQuery.ajax({
							  type		: 'post',
							  url		: window.location,
				//			  data		: 'adicionaItens=1&itensselecionados=' + itensselecionados + '&' + $('form_itemcomposicao').serialize(),
							  data		: jQuery('#form_itemcomposicao').serialize() + '&adicionaItens=1',
							  success	: function(res) {
						  			window.opener.jQuery( '#itens_' + ano ).html( res );
						  			alert('Itens incluídos com sucesso');
						  			divCarregando();
						  			window.close();
							  }
						});
					}
				}
				
				function carregaItens(picid, num, perfil, grupo){
					if( grupo ){
						str = "requisicaoAjax=valorUnitario&nome=valor[" + num + "]&numero="+num+"&picid="+picid+"&grupo="+grupo;
					} else {
						str = "requisicaoAjax=valorUnitario&nome=valor[" + num + "]&numero="+num+"&picid="+picid;
					}
					if(picid){
						$.ajax({
							   type: "POST",
							   url: window.location,
							   assync: false,
							   data: str,
							   success: function(msg){
							   		dados = msg;
			   	    			 	dados = dados.split('|');
			   	    			 	if( perfil ){
										$("#imagem_" + num).html( num + " " +'<img class="middle link" title="Excluir Item" onclick="removerLinha(\'' + num + '\')"  src="../imagens/excluir.gif" />&nbsp;&nbsp;<img class="middle link" title="Visualizar Tabela de Apoio" onclick="verTabelaApoio( ' + picid + ' )"  src="../imagens/consultar.gif" />' );
									}
			   	    			 	if( dados[0] == '0,00' ){
										$("#umiid_" + num).html( dados[1] );
										/* $("#valor_" + num).html( dados[0] );
										$('[name="valorH[' + num + ']"]').val( dados[0] );
										$('[name="icoquantidade['+num+']"]').val( '0' );
										$('[name="icoquantidade['+num+']"]').attr( 'disabled', 'disabled' );
										$("#valortotal_" + num).html( '0,00' );
										alert("Não existe valor cadastrado para o item.");*/
			   	    			 	}else{
										$("#umiid_" + num).html( dados[1] );
// 										$('[name="icoquantidade['+num+']"]').attr( 'disabled', '' );
			   	    			 		/* if( dados[2] == '' ){
											$('[name="valorP[' + num + ']"]').hide();
											$('[name="valorH[' + num + ']"]').val( dados[0] );
											$("#valor_" + num).html( dados[0] );
										$("#valortotal_" + num).html( dados[0] );
										} else {
											percent = new Number( dados[2] );
											valor = new Number( dados[0].replace(",", ".") );
											valorminimo = new Number( valor - ( (valor*percent)/100 ) );
											valormaximo = new Number( valor + ( (valor*percent)/100 ) );
											$('[name="valorMin[' + num + ']"]').val( valorminimo );
											$('[name="valorMax[' + num + ']"]').val( valormaximo );
											$('[name="valorP[' + num + ']"]').show();
										} */
			   	    			 	}
							   }
						 });
					} else {
						return false;
					}
				}
	
				function calculaValorItem(valor, numero)
				{
					if( document.getElementsByName("valorP[" + numero + "]")[0].value ){
						valor = document.getElementsByName("valorP[" + numero + "]")[0].value;
					} else {
						valor = document.getElementsByName("valorH[" + numero + "]")[0].value;
					}
					qtde = document.getElementsByName("icoquantidade[" + numero + "]")[0].value;
					if(qtde){
						qtde = mascaraglobal('[#]',qtde);
					}else{
						qtde = 0;
					}
					
					if(valor){
						valor = mascaraglobal('[#]',valor);
					}else{
						valor = 0; 
					}
					
					var total = qtde * valor;
					$("#valortotal_" + numero).html( mascaraglobal('[.###],##',total) );
					$('[name="valortotalH[' + numero + ']"]').val( mascaraglobal('[.###],##',total) );
				}
				function novoItem(grupo)
				{
					var num = $("#tbl_item tr").length;
					var str = 0;
					sbacronograma = document.getElementsByName("sbacronograma")[0].value;
	
					if( sbacronograma == 1 ){
						$("#tbl_item").append('<tr id="tr_' + num + '" ><td id="imagem_' + num + '" align="center" >' + num + " " + '<img class="middle link" title="Excluir Item" onclick="removerLinha(\'' + num + '\')"  src="../imagens/excluir.gif" /><input type="hidden" name="valorMin[' + num + ']"><input type="hidden" name="valorMax[' + num + ']"><input type="hidden" name="valorH[' + num + ']"><input type="hidden" name="valorP[' + num + ']"><input type="hidden" name="valortotalH[' + num + ']"></td><td id="item_' + num + '" align="center" ></td><td id="umiid_' + num + '" align="center" ></td><td id="icoquantidade_' + num + '" align="center" ></td><td  id="valortotal_' + num + '" align="center" ></td></tr>');
					} else {
						$("#tbl_item").append('<tr id="tr_' + num + '" ><td id="imagem_' + num + '" align="center" >' + num + " " + '<img class="middle link" title="Excluir Item" onclick="removerLinha(\'' + num + '\')"  src="../imagens/excluir.gif" /><input type="hidden" name="valorH[' + num + ']"><input type="hidden" name="valortotalH[' + num + ']"></td><td id="item_' + num + '" align="center" ></td><td id="umiid_' + num + '" align="center" ></td></tr>');
					}
					$("[name*='item']").each(function() {
	   	    			if($(this).val()){
	   	    				str = str + ', ' + $(this).val();
	   	    			}
					});
					
					if(grupo){
						req1 = "requisicaoAjax=comboItem&nome=item[" + num + "]&numero="+num+"&grupo="+grupo+"&condicao='"+str+"'";
					} else {
						req1 = "requisicaoAjax=comboItem&nome=item[" + num + "]&numero="+num+"&condicao='"+str+"'";
					}
	
					JqueryAjax(req1,"item_" + num);
				//	JqueryAjax("requisicaoAjax=comboUnidadeMedida&nome=umiid[" + num + "]","umiid_" + num);
				//	JqueryAjax("requisicaoAjax=texto_valorMenu&nome=valor[" + num + "]&numero="+num,"valor_" + num);
					if( sbacronograma == 1 ){
						JqueryAjax("requisicaoAjax=texto_qtde&nome=icoquantidade[" + num + "]&numero="+num,"icoquantidade_" + num);
// 						JqueryAjax("requisicaoAjax=texto_valorTotal&nome=valortotal[" + num + "]","valortotal_" + num);
// 						$('[name="icoquantidade['+num+']"]').attr( 'disabled', 'disabled' );
					}
// 					JqueryAjax("requisicaoAjax=texto_valorMenu&nome=valorP[" + num + "]&numero="+num,"valor_" + num);
// 					$("[name='valorP["+num+"]']").hide();
				}
				function removerLinha(num)
				{
					$("#tr_" + num).remove();
				}
				function novoGrupo(){
					document.getElementById('div_grupo').style.display='block';
				}
				function carregarGrupoItem(){
					gicid = $('#gicid').val();
					document.getElementById('div_grupo').style.display='none';
					
			   		if( confirm("Tem certeza que deseja adicionar os itens do grupo selecionado?") ){
					divCarregando();
					$.ajax({
						   type: "POST",
						   url: window.location,
						   data: "requisicaoAjax=recuperaItensGrupo&gicid="+gicid,
						   assync: true,
						   success: function(msg){
						   		dados = msg.split('|');
						   		var st;
						   		var t = 0;
						   		var numeroInicio = $('select[name^="item["]').length + 1;
//						   		var numero = $('select[name^="item["][value!=""]').length;
						   		var numero = $('select[name^="item["]').length;

						   		for( i=1; i<=dados.length; i++ )
							   	{
						   			verdadeiro = true;
						   			numero = numero + 1;
							   		dadositem = dados[t].split('-');
							   		var item = Number(dadositem[0]);
							   		var qtd = dadositem[1];
						   			$('select[name^="item["][value!=""]').each(function(){
							   			if( item == $(this).val() ){
							   				verdadeiro = false;
							   			}
									});
						   			
						   			if( verdadeiro ){
							   			novoItem(gicid);
							   			$('select[name="item['+numero+']"]').val(item);
							   			$('[name="icoquantidade['+numero+']"]').val(qtd);
							   			//carregaItens(item, numero, 1, gicid);
							   			st = st + '|' + item;
							   		}
							   		t++;
						   		}

						   		$('#grupopedido').val(gicid+'-'+st);
	
						   		var t = 0;
						   		for( i=0; i<dados.length; i++ ){
							   		dadositem = dados[t].split('-');
							   		var item = Number(dadositem[0]);
						   			carregaItens(item, numeroInicio, 1, gicid);
						   			numeroInicio++;
							   		t++;
						   		}
						   		
						   		$('#grupopedido').val(gicid+'-'+st);
								
						   		$('#td_1').append('<img src="../imagens/excluir.gif" onclick="removerLinha(\'1\')" title="Excluir Item" class="middle link">');
						   		
								divCarregado();
								
						   }
					   
					 });
					}
				}
				function deletarRegistro( id, num ){
				
					if( confirm( "Tem certeza que deseja excluir esse registro?" ) ){
						$.ajax({
							   type: "POST",
							   url: window.location,
							   data: "requisicaoAjax=deletaRegistro&id="+id,
							   success: function(msg){
							   		removerLinha(num);
							   		//window.opener.document.location.reload();
							   }
						 });
					} else {
						return false;
					}
				}
				function JqueryAjax(params,destino)
				{
					$.ajax({
						   type: "POST",
						   url: window.location,
						   data: params,
						   async: false,
						   success: function(msg){
								$("#" + destino).html(msg);
						   }
					 });
				}
				
			</script>
			<form name="form_itemcomposicao" id="form_itemcomposicao" method="post" action="" >
				<input type="hidden" name="sbaid" value="<?php echo $sbaid ?>" />
				<input type="hidden" name="requisicao" value="salvarItemComposicao" />
				<input type="hidden" name="grupopedido" id="grupopedido" value="" />
				<input type="hidden" name="sbacronograma" value="<?php echo $_REQUEST['sbacronograma']; ?>" />
				<input type="hidden" name="itens" id="itens" value="" />
				<input type="hidden" name="ano" id="ano" value="<?php echo $_REQUEST['ano']; ?>" />
				<input type="hidden" name="ppsid" value="<?php echo $sba['ppsid']; ?>" />
				<input type="hidden" name="escolas" value="<?php echo $escolas; ?>" />
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr style="background-color: #cccccc;">
						<td align="center" colspan="2"><strong>Itens de Composição</strong></td>
					</tr>
					<tr>
						<td align="left" colspan="2">
							<?php $oIC = new SubacaoItensComposicao();?>
							<?php //$arrDadosIC = $oIC->retornaItensComposicaoPorSubacao($_GET['sbaid'],$_GET['ano'])?>
							<?php 
							if( $_REQUEST['sbacronograma'] == 1 ){
								//$w = "psi.ppsid = (select ppsid from par.subacao where sbaid = {$_GET['sbaid']}) AND";
								$td1 = '<td class="bold" >Qtde</td>';
							//	$td2 = '<td class="bold" >Valor Total(R$)</td>';
							}
							$sql = "	SELECT DISTINCT
											       sic.icoid,
												   pic.picid,
												   dic.dicid,
												   pic.umiid,
												   uni.umidescricao,
												   pic.picdescricao as item,
												   sic.icoquantidade,
												   sic.icovalor as valor,
												   dicdatainicial,
												   dicdatafinal,
												   uf.estuf,
												   dic.dicpercentual as percentual
											FROM par.propostaitemcomposicao pic
											LEFT JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A' AND now() between dicdatainicial and dicdatafinal
											LEFT JOIN par.propostasubacaoitem psi ON psi.picid = pic.picid
											LEFT JOIN par.propostatipopregao ptp ON ptp.ptpid = dic.ptpid AND ptpstatus = 'A'
											INNER JOIN par.ufdetalheitemcomposicao uf   ON dic.dicid   = uf.dicid ".( $_SESSION['par']['itrid'] == 1 ? " AND estuf = '{$_SESSION['par']['estuf']}' " : " AND estuf = ( select estuf from territorios.municipio where muncod = '{$_SESSION['par']['muncod']}' ) ")."
											INNER JOIN par.subacaoitenscomposicao sic ON sic.picid = pic.picid AND sic.sbaid = {$_GET['sbaid']}
											INNER JOIN par.unidademedidadetalhamentoitem uni ON pic.umiid = uni.umiid
											WHERE
											{$w} 
											pic.picstatus = 'A'
											AND sic.icoano = {$_GET['ano']}
											
											ORDER BY pic.picdescricao";
							?>
							<?php // $arrDados = $db->carregar($sql); ?>
							<table id="tbl_item" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
								<tr class="SubTituloTabela bold" >
									<td class="bold" >Ação</td>
									<td class="bold" >Item</td>
									<td class="bold" >Unidade de Medida</td>
									<?php echo $td1; ?>
									<?php echo $td2; ?>
								</tr>
								<?php $condicao = 0; ?>
								<?php 
									if( possuiPerfil(array(PAR_PERFIL_SUPER_USUARIO)) ){
										$tabelaApoio = '&nbsp;&nbsp;<img class="middle link" title="Visualizar Tabela de Apoio" onclick="verTabelaApoio( '.$dado['picid'].' )"  src="../imagens/consultar.gif" />';	
									}
									if($arrDados){ ?>
									<?php foreach($arrDados as $chave => $dado):
										$chave = $chave + 1; 
										$condicao .= ", ".$dado['picid'];
										$valorminimo = '';
										$valormaximo = '';
										if( !is_null($dado['percentual']) ){
											$valorminimo = ( $dado['valor'] - ( ($dado['valor']*$dado['percentual'])/100 ) );
											$valormaximo = ( $dado['valor'] + ( ($dado['valor']*$dado['percentual'])/100 ) );
										}
										?>
										<tr id="tr_<?php echo $chave ?>" >
											<td align="center" ><?php echo $chave ?> <img class="middle link" title="Excluir Item" onclick="deletarRegistro( <?php echo $dado['icoid']; ?>, <?php echo $chave; ?> )"  src="../imagens/excluir.gif" /><?php echo $tabelaApoio; echo $dado['acao']; ?><input type="hidden" name="valorH[<?php echo $chave ?>]" value="<?php echo number_format($dado['valor'],2,',','.') ?>"><input type="hidden" name="valorP[<?php echo $chave ?>]" value=""><input type="hidden" name="valortotalH[<?php echo $chave ?>]" value="<?php echo number_format($dado['icoquantidade']*$dado['valor'],2,',','.') ?>"><input type="hidden" name="valorMin[<?php echo $chave ?>]" value="<?php echo $valorminimo ?>"><input type="hidden" name="valorMax[<?php echo $chave ?>]" value="<?php echo $valormaximo ?>"></td>
											<td align="center" ><?php comboItem("item[$chave]", $dado['picid'], $chave) ?></td>
											<td align="center" id="umiid_<?php echo $chave ?>"><?php echo $dado['umidescricao'] ?></td>
											<?php if( $_REQUEST['sbacronograma'] == 1 ){ ?>
												<td align="center" ><?php texto_qtde("icoquantidade[$chave]", $chave, $dado['icoquantidade']) ?></td>
											<?php } ?>
											<?php if( $_REQUEST['sbacronograma'] == 1 ){ ?>
												<td align="center" id="valortotal_<?php echo $chave ?>" ><?php echo number_format($dado['icoquantidade']*$dado['valor'],2,',','.') ?></td>
											<?php } ?>
										</tr>	
									<?php endforeach; ?>
								<?php } else { ?>
								<?php $num = $arrDados ? count($arrDados) : "1" ?>
								<tr id="tr_<?php echo $num ?>" >
									<td id="td_<?php echo $num ?>" align="center" > <?php echo $num ?> <input type="hidden" name="valorMin[<?php echo $num ?>]"><input type="hidden" name="valorMax[<?php echo $num ?>]"><input type="hidden" name="valorP[<?php echo $num ?>]"><input type="hidden" name="valorH[<?php echo $num ?>]"><input type="hidden" name="valortotalH[<?php echo $num ?>]"></td>
									<td align="center" ><?php comboItem("item[$num]", null, $num, $condicao) ?><input type="hidden" name="picid[]" value="<?php echo $dado['picid'] ?>"  /></td>
									<td align="center" id="umiid_<?php echo $num ?>"><?php // comboUnidadeMedida("umiid[$num]") ?></td>
									<?php if( $_REQUEST['sbacronograma'] == 1 ){ ?>
										<td align="center" ><?php texto_qtde("icoquantidade[$num]", $num) ?></td>
									<?php } ?>
									<?php if( $_REQUEST['sbacronograma'] == 1 ){ ?>
										<td align="center" id="valortotal_<?php echo $num ?>" ></td>
									<?php } ?>
								</tr>
								<?php } ?>
							</table>
						</td>
					</tr>
					<tr style="background-color: #cccccc;">
						<td align="left" colspan="2">
							<img class="middle link" onclick="novoItem()"  src="../imagens/gif_inclui.gif" /> <a href="javascript:novoItem()">Adicionar Item</a> / <a href="javascript:novoGrupo()">Adicionar Grupo de Itens</a>
							<span style="margin-left:50px;color:red" class="bold" >*Obs: Para confirmar as alterações clique no botão "Salvar".</span>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita"></td>
						<td class="SubTituloEsquerda" >
							<input type="button" value="Salvar" name="btn_salvar" onclick="salvarItemComposicaoMenu()" />
						</td>
					</tr>
				</table>
				<div id="div_grupo" style="width:270px; height:120px; position:absolute; z-index:0; top:50%; left:50%; margin-top:-125px; margin-left:-60px; border:solid 2px black; background-color:#FFFFFF; display:none;" >
					<div style="width:100%;text-align:right">
						<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('div_grupo').style.display='none'" />
					</div>
					<div style="padding:5px;text-align:justify;">
						<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
						<tr>
							<td width="30%" class="SubtituloDireita">Grupo:</td>
							<td><?php
									$sql = "SELECT 
												gic.gicid as codigo, 
												gic.gicdescricao as descricao
											FROM 
												par.grupo_itemcomposicao gic
											INNER JOIN par.propostagrupoitem pgi ON pgi.gicid = gic.gicid
											WHERE  
												gic.gicstatus = 'A' AND
												pgi.ppsid = ".$_REQUEST['ppsid']."
											ORDER BY 
												gic.gicdescricao";
				
									$db->monta_combo('gicid', $sql, 'S', 'Selecione', '', '', '', '200', 'N', 'gicid'); 
									?></td>
						</tr>
						<tr>
							<td align="center" bgcolor="#D5D5D5" colspan="2">
								<input type="button" name="btn_enviar" onclick="carregarGrupoItem()" value="ok" />
								<input type="button" name="btn_cancelar" onclick="document.getElementById('div_grupo').style.display='none'" value="cancelar" />
							</td>
						</tr>
						</table>
					</div>
				</div>
			</form>
		</body>
	</html>
<?php } ?>
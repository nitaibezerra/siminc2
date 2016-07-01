<?php


/*
 * Função que verifica se a subação tem parecer 
 * @param int $sbaid - Identificação da subação
 * @return int - Retorna 1 ou 0 (se tem ou não tem parecer)   
 */
function existeParecerPorSubacao( $sbaid ){
	
	global $db;
	
	 $sql = '
	    SELECT count(spt.ssuid)
	    FROM cte.subacaoparecertecnico spt
	    	INNER JOIN cte.statussubacao ssu ON ssu.ssuid = spt.ssuid
	    WHERE spt.sbaid = ' . $sbaid . '
 		AND trim(spt.sptparecer) <> \'\'
		AND spt.ssuid IS NOT NULL';

	    return (integer) $db->pegaUm($sql) > 0;
}

/*
 * Função que exclui uma subação 
 * @param int $sbaid - Identificação da subação   
 * @return bool - Retorna true ou false em caso de sucesso ou não, respectivamente   
 */
function excluirSubacao( $sbaid ){
	
	global $db;
	
	// Verifica se há parecer para essa subação
    $sql = '
        SELECT COUNT(sbaid)
        FROM cte.subacaoparecertecnico spt
        WHERE sbaid = ' . $sbaid . ' 
        AND (
				ssuid IS NOT NULL or
                trim(sptparecer) <> \'\'
            )';

	$parec = $db->pegaUm($sql);

	// Verifica se há convênio para essa subação
	$sql = '
        SELECT COUNT(sac.sbaid)
        FROM cte.projetosapesubacao sac
        	INNER JOIN cte.subacaoindicador sba ON sba.sbaid = sac.sbaid
        WHERE sac.sbaid = ' . $sbaid;

	$conv  = $db->pegaUm($sql);

	// Se tiver parecer ou convênio para a subação não permite exclusão.
	// Caso contrário, exclui.
	if( $parec >= 1 || $conv >= 1 ){
		return false;
	} 
	else{
        $db->executar('
        			   DELETE FROM cte.subacaoparecertecnico
                       WHERE sbaid = ' . (integer) $sbaid .';

        			   DELETE FROM cte.subacaobeneficiario
                       WHERE sbaid = ' . (integer) $sbaid .';

                       DELETE FROM cte.subacaoobra
                       WHERE sbaid = ' . (integer) $sbaid .';
                       
                       DELETE FROM cte.composicaosubacao
                       WHERE sbaid = ' . (integer) $sbaid .';

                       DELETE FROM cte.qtdfisicoano
                       WHERE sbaid = ' . (integer) $sbaid .';

                       DELETE FROM cte.termosubacaoindicador
                       WHERE sbaid = ' . (integer) $sbaid .';

                       DELETE FROM cte.subacaoindicador
                       WHERE sbaid = ' . (integer) $sbaid );

        $db->commit();

		return true;
		
	}

}

/*
 * Função que insere nova subação
 * @param array $arDados - Array de dados que serão inseridos
 * @return int - Retorna o identificador da nova subação
 */
function inserirNovaSubacao( $arDados ){
	
	global $db;
	
	$sql = '
        INSERT INTO cte.subacaoindicador ( aciid, undid,
								           frmid, sbadsc,
								           sbastgmpl, sbaprm,
								           sbapcr, sbadata,
								           usucpf, foaid,
								           prgid, sbaporescola,
								           ppsid, sbacategoria,
								           sbaordem ) 
        						  VALUES ( %s, %d,
            							   %s, \'%s\',
							               \'%s\', %d,
							               \'%s\', CURRENT_TIMESTAMP,
							               \'%s\', %s,
							               %d, %s,
							               %s, %d,
										   %s ) 
								  returning sbaid';

	$sql = sprintf( $sql, (string)  $arDados['aciid'],
						  (integer) $arDados['undid'] == 0 ? 'null' : (integer) $arDados['undid'],
						  (integer) $arDados['frmid'] == 0 ? 'null' : (integer) $arDados['frmid'],
						  (string)  $arDados['sbadsc'],
						  (string)  $arDados['sbastgmpl'],
						  (integer) $arDados['sbaprm'],
						  (string)  $arDados['sbapcr'],
						  (string)  $arDados['usucpf'],
						  (integer) $arDados['foaid'] == 0 ? 'null' : (integer) $arDados['foaid'],
						  (integer) $arDados['prgid'] == 0 ? 'null' : (integer) $arDados['prgid'],
						  trim($arDados['sbaporescola'])     == ''? 'false': (string)  $arDados['sbaporescola'],
						  (integer) $arDados['ppsid'] == 0 ? 'null' : (integer) $arDados['ppsid'],
						  (integer) $arDados['sbacategoria'],
						  (integer) $arDados['sbaordem']);

	$sbaid = $db->pegaUm($sql);
	$db->commit();

	return $sbaid;
}

/*
 * Função que verifica os campos não preenchidos da subação
 * @return array - Array de erros encontrados ou array vazio, caso não encontre erro.
 */
function verificarErros(){
	global $db;
	
	$sql = '
            SELECT
                CASE WHEN sba.frmid = 11 OR sba.frmid = 3 
                THEN count(ben.benid)
                ELSE 1
                END as benidtotal,
                CASE WHEN sba.frmid = 11 OR sba.frmid = 3 
                THEN vlrurbano + vlrrural
                ELSE 1
                END as benvlr,
                sbaporescola,
                frmid
            FROM cte.subacaoindicador sba
            	LEFT JOIN cte.subacaobeneficiario ben on sba.sbaid = ben.sbaid
            WHERE sba.sbaid = ' . $_REQUEST['sbaid'] . '
            GROUP BY vlrurbano, vlrrural, frmid, sbaporescola';

	$res = $db->pegaLinha( $sql );
	$err = array();

	if ($_REQUEST['sptinicio_' . $_REQUEST['anoExercicio']] == '0')
        $err[] = ' - O mês inicial do cronograma deve ser preenchido';

	if ($_REQUEST['sptfim_'    . $_REQUEST['anoExercicio']] == '0')
        $err[] = ' - O mês final do cronograma deve ser preenchido';

	if ($res['sbaporescola'] == 't') {
                
		$sql = '
	            SELECT qfaqtd
				FROM cte.qtdfisicoano
	            WHERE qfaano = ' . $_REQUEST['anoExercicio'] . '
	            and sbaid    = ' . $_REQUEST['sbaid'];

		$qtd = $db->carregar($sql);

		if (!$qtd) {
			$err[] = ' - Pelo menos uma escola deve ser adicionada à subação';
		} 
		else {
			while (list(, $qtdfisicoano) = each($qtd)) {
				if ($qtdfisicoano['qfaqtd'] <= 0) {
					$err[] = ' - O quantitativo por escola deve ser informado individualmente';
					break;
				}
			}

			$sql = '
	                SELECT DISTINCT qfaqtd
	                FROM cte.subacaoindicador sba
	                INNER JOIN cte.qtdfisicoano qfa on sba.sbaid = qfa.sbaid
	                INNER JOIN cte.composicaosubacao cos on sba.sbaid = cos.sbaid
	                INNER JOIN cte.escolacomposicaosubacao ecs on qfa.qfaid = ecs.qfaid AND cos.cosid = ecs.cosid
	                WHERE sba.sbaid = ' . $_REQUEST['sbaid'] . '
	                GROUP BY qfaqtd';

			$ecs = $db->carregar($sql);
                    
			/*if (!$ecs) {
                $err[] = ' - O quantitativo da subação por escola deve ser preenchido';
                }
			else {
                while (list(, $a) = each($ecs)) {
                	if ($a['qfaqtd'] <= 0) {
						$err[] = ' - O quantitativo da subação por escola deve ser preenchido';
					}
				}
			}*/
		}
	} 
	else{
		if ($_REQUEST['sptunt_' . $_REQUEST['anoExercicio']] <= '0' || $_REQUEST['sptunt_' . $_REQUEST['anoExercicio']] == '')
			$err[] = ' - A quantidade deve ser preenchida';
	}

	if ($res['benidtotal'] == 0)
		$err[] = ' - Para sub-ações executadas com assistência ou contemplação financeira,\n   beneficiários devem ser informados';
	elseif ($res['benvlr'] == 0 || $res['benvlr'] == '')
		$err[] = ' - O quantitativo de beneficiários da sub-ação deve ser informado';

	$sql = '
            SELECT DISTINCT coalesce(frmid, 0) as frmid,
                			count(cosid) as total
            FROM cte.subacaoindicador sba
            	LEFT JOIN cte.composicaosubacao cos on sba.sbaid = cos.sbaid AND cosano    = ' . $_REQUEST['anoExercicio'] . '
            WHERE sba.sbaid = ' . $_REQUEST['sbaid'] . '
            GROUP BY frmid';

	$res = (array) $db->pegaLinha($sql);

	if ($res['frmid'] != 10 && $res['frmid'] != 4 && $res['frmid'] != 8) {
		if ($res['total'] <= 0) {
			$err[] = ' - Devem ser adicionados itens de composição';
		} 
		else{
			if (is_array($_REQUEST['cosvlruni']) && is_array($_REQUEST['cosqtd'])){
				foreach ($_REQUEST['cosqtd'] as $cosid => $cosqtd) {
					if ($cosqtd <= 0 || $_REQUEST['cosvlruni'][$cosid] <= 0) {
						$err[] = ' - O quantitativo e o valor unitário dos itens de composição\n   devem ser preenchidos';
						break;
					}
				}
			}
		}
	}

	if ($_REQUEST['ssuid_' . $_REQUEST['anoExercicio']] == '0')
		$err[] = 'O status do parecer deve ser informado';

	return $err;
}

/*
 * Função que grava uma subação existente
 * @param int $anoExercicio - Ano em exercício
 */
function gravarSubAcao( $anoExercicio ){
	
	global $db; 
	
	$testaValorSubPai = true;
	$sql=	"SELECT si.sbaid ".
			"FROM cte.subacaoindicador si ". 
			"LEFT JOIN cte.projetosapesubacao sc on sc.sbaid = si.sbaid ". 
			"LEFT JOIN cte.projetosape c on sc.prsid = c.prsid ". 
			"WHERE si.sbaidpai = ".$_REQUEST['sbaid']." ". 
			"AND sbaanoreformulacao = '".$anoExercicio."'";
	$testaReformulacao = $db->pegaUm($sql);
//	echo "<script> alert(\"".$sql."\"); </script>";
	if($testaReformulacao != ''){
		$valorTotalSub = $db->pegaUm("
								SELECT
									trim(to_char(sum(cos.cosqtd * cos.cosvlruni), '99999999.99')) as vlrtotal
								FROM cte.composicaosubacao cos
								INNER JOIN cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
								INNER JOIN cte.subacaoindicador si ON cos.sbaid = si.sbaid
								WHERE si.sbaidpai  = " . $_REQUEST['sbaid']  . "
								AND si.sbaseqreformulacao = 1
								AND cos.cosano = " . $anoExercicio);
		$valorTotalSub = str_replace('.', '', $valorTotalSub);
		
//		$valorTotalSubNovo = number_format($_POST['valorTotalSubAtual'.$anoExercicio], 2, ',', '.');
		$valorTotalSubNovo = str_replace(',','',$_POST['valorTotalSubAtual'.$anoExercicio]);
		$valorTotalSubNovo = str_replace('.','',$valorTotalSubNovo);
		
		
//		echo "<script> alert(\"".$valorTotalSub." e ".$valorTotalSubNovo."\"); </script>";
		if($valorTotalSub != $valorTotalSubNovo){
			$msg = 'Valor atual difere do valor da subação original. Operação cancelada.\n'.$valorTotalSubNovo.' e \n'.$valorTotalSub;
			$testaValorSubPai = false;
			return $msg;
		}
	}
	
	if ($testaValorSubPai == true){
//		echo "<script> alert('Valor de teta = ".$testaValorSubPai."'); </script>";
		$sql = '
		        SELECT count(sptid)
		        FROM cte.subacaoparecertecnico
		        WHERE sbaid  = ' . $_REQUEST['sbaid'] . '
				AND sptano   = ' . $anoExercicio;		
	        
		if( ( $sbaid = $db->pegaUm($sql) ) <= 0 ) {
			$sql = "
		            INSERT INTO cte.subacaoparecertecnico (
		                sbaid,
		                sptparecer,
		                sptunt,
		                sptuntdsc,
		                sptano,
		                sptinicio,
		                sptfim,
		                sptanoterminocurso,
		                tppid,
		                ssuid,
		                sptplanointerno,
		                sptnumprocesso
		            ) VALUES (%d, '%s', %s, '%s', %d, %d, %d, %d, 4, %s, '".$_REQUEST['plinumplanointerno_'.$anoExercicio]."', '".$_REQUEST['cvrnumprocesso_'.$anoExercicio]."')";
		}
		else {
			$sql = "
		            UPDATE cte.subacaoparecertecnico SET
		                sbaid              = %d,
		                sptparecer         = '%s',
		                sptunt             = %s,
		                sptuntdsc          = '%s',
		                sptano             = %d,
		                sptinicio          = %d,
		                sptfim             = %d,
		                sptanoterminocurso = %d,
		                tppid              = 4,
		                ssuid              = %s,
		                sptplanointerno		= '".$_REQUEST['plinumplanointerno_'.$anoExercicio]."',
		                sptnumprocesso	   = '".$_REQUEST['cvrnumprocesso_'.$anoExercicio]."'
		            WHERE
		                sbaid              = %d
		                AND
		                sptano             = %d";
		}
	
		$sql = sprintf($sql, (integer) $_REQUEST['sbaid'                              ],
							 (string)  $_REQUEST['sptparecer_' . $anoExercicio],
							 (string)  $_REQUEST['sptunt_'             . $anoExercicio] == '' ||
							 (integer) $_REQUEST['sptunt_'             . $anoExercicio] == 0  ? 'NULL' : (integer) $_REQUEST['sptunt_' . $anoExercicio],
							 (string)  $_REQUEST['sptuntdsc_' . $anoExercicio],
							 (integer) $_REQUEST['anoExercicio'                       ],
							 (integer) $_REQUEST['sptinicio_'          . $anoExercicio],
							 (integer) $_REQUEST['sptfim_'             . $anoExercicio],
							 (integer) $_REQUEST['sptanoterminocurso_' . $anoExercicio],
							 (integer) $_REQUEST['ssuid_'              . $anoExercicio] == 0  ? 'NULL' : (integer) $_REQUEST['ssuid_'  . $anoExercicio],
							 (integer) $_REQUEST['sbaid'                              ],
							 (integer) $anoExercicio);
	
		$db->executar($sql);
	
		$db->executar('UPDATE cte.subacaoindicador
							SET usucpf  = \'' . (string)  $_SESSION['usucpf'] . '\',
							sbadata = CURRENT_TIMESTAMP
					   WHERE sbaid  = '   . (integer) $_REQUEST['sbaid']);
	
		if (is_array($_REQUEST['cosqtd'])) {
			$sql = '
		            UPDATE cte.composicaosubacao SET
		                cosqtd    = %f,
		                cosvlruni = %f
		            WHERE cosid   = %d';
	
			if (is_array($_REQUEST['cosqtd'])) {
				foreach ($_REQUEST['cosqtd'] as $cosid => $cosqtd) {
					if (strlen($cosqtd) > 1) {
	                	$cosqtd = str_replace('.', '', $cosqtd);
						$cosqtd = str_replace(',', '.', $cosqtd);
					}
	
					if (strlen($_REQUEST['Hcosvlruni'][$cosid]) > 1) {
						$cosvlruni = str_replace('.', '', $_REQUEST['Hcosvlruni'][$cosid]);
						$cosvlruni = str_replace(',', '.', $cosvlruni);
					}
					else {
						$_REQUEST['Hcosvlruni'][$cosid] .= ',00';
	
						$cosvlruni = str_replace('.', '', $_REQUEST['Hcosvlruni'][$cosid]);
						$cosvlruni = str_replace(',', '.', $cosvlruni);
					}
	
					$cosqtd    = number_format($cosqtd,    6, '.', '');
					$cosvlruni = number_format($cosvlruni, 6, '.', '');
					
					$db->executar(sprintf($sql, $cosqtd, $cosvlruni, $cosid));
				}
			}
		}
		$msg = 'Dados salvos com sucesso.';
		$db->commit(); 
		return $msg;
	}
}

/*
 * Função que limpa uma subação
 * @return bool - True ou False em caso de sucesso ou não, respectivamente
 */
function limparSubAcao(){

	global $db;
	
	$sql = '
	        SELECT count(spt.ssuid)
	        FROM cte.subacaoparecertecnico spt
	        	INNER JOIN cte.statussubacao ssu ON ssu.ssuid = spt.ssuid
	        WHERE sptano  = ' . (integer) $_REQUEST['anoExercicio']. '
			AND spt.sbaid = ' . $_REQUEST['sbaid'] . '
	        AND spt.ssuid IS NOT NULL';

	$parecer = (integer) $db->pegaUm($sql) > 0;

	if( !$parecer ){
		$db->executar('
                        DELETE FROM cte.subacaoparecertecnico
                        WHERE sptano = ' . (integer) $_REQUEST['anoExercicio']. '
						AND sbaid 	 = ' . (integer) $_REQUEST['sbaid'] .';
    
						DELETE FROM cte.subacaobeneficiario
						WHERE sabano = ' . (integer) $_REQUEST['anoExercicio']. '
						AND sbaid 	 = ' . (integer) $_REQUEST['sbaid'] .';
						    
						DELETE FROM cte.composicaosubacao
						WHERE cosano = ' . (integer) $_REQUEST['anoExercicio']. '
						AND    sbaid = ' . (integer) $_REQUEST['sbaid'] .';
						    
						DELETE FROM cte.qtdfisicoano
						WHERE qfaano = ' . (integer) $_REQUEST['anoExercicio']. '
						AND    sbaid = ' . (integer) $_REQUEST['sbaid'] .';
						
						UPDATE cte.subacaoindicador
							   SET usucpf  = \'' . (string)  $_SESSION['usucpf'] . '\',
								   sbadata = CURRENT_TIMESTAMP
						WHERE sbaid  = '   . (integer) $_REQUEST['sbaid']);
						    
		$db->commit();
		
		return true;
	} 
	else{
		return false;
	}
}

/*
 * Função que monta o SQL para carregar Itens de Composição
 * @return string - SQL dos itens de composição
 */
function montarSQLItensComposicao(){
	
	global $db;
	
	$sql = '
	        SELECT count(sba.undid)
	        FROM cte.subacaoindicador sba
	        	INNER JOIN cte.unidademedida und ON sba.undid  = und.undid
	        	INNER JOIN cte.laboratorio lab ON und.undid = lab.undid
	        WHERE sbaid = ' . $_REQUEST['sbaid'];

	if( !CTE_PRM_EDITAR_SUBACAO || $db->pegaUm($sql) == 1 || $_REQUEST['ref']){
		$readonly = ' readonly="readonly"';
		$remover  = "alert(\\'Não é possível excluir itens de composição de laboratórios.\\');return false;";
	} 
	else{
		$readonly = '';
		$remover  = 'removerItemComposicao';
	}

	if( $_REQUEST['sbaporescola'] == 't' ){
		if( $readonly == '' ){
			$sql = '
                	SELECT
                    	\'<center><img id="removeItens" name="removeItens" onclick="return ' .$remover. '(\' || cos.cosid || \')" src="/imagens/excluir.gif" title="Excluir" alt="Excluir \' || cos.cosdsc || \'" /></center>\' as cosid
                    	,cos.cosdsc
                    	,und.undddsc
	                    ,\'<input size="15" id="cosqtd\' || cos.cosid || \'"
								type="text"
								class="CampoEstilo"
								onmouseover="MouseOver(this);"
								onfocus="MouseClick(this);"
								onmouseout="MouseOut(this);"
								onblur="MouseBlur(this); verificarModificaoItensComposicao(this); calcularValorTotal(\' || cos.cosid || \');"
								onfocus="this.select()"
								onclick="return popupSelecionarEscolasItemComposicao(this,\' || cos.cosid || \')"
								name="cosqtd[\'|| cos.cosid ||\']"
								readonly="readonly"
								value="\' || 
									case 
							    		when substring( trim(to_char(coalesce(ecs.total, 0), \'9999999990D999999\')), position(\',\' in trim(to_char(coalesce(ecs.total, 0), \'9999999990D999999\'))) +1, 6 ) = \'000000\'
							    			then trim(to_char(coalesce(ecs.total, 0), \'999999999\'))
							    		else trim(to_char(coalesce(ecs.total, 0), \'9G999G999G990D999999\'))
							    	END
								|| \'" />\' as cosqtd
                    	,\'<input size="20" id="cosvlruni\' || cos.cosid || \'"
								type="text"
								class="itemComposicao"
								style="border-right: #888888 1px solid;border-top: #888888 1px solid;font-size: 10px;vertical-align: middle;border-left: #000000 3px solid;border-bottom: #888888 1px solid;font-family: verdana;background-color: #ffffff"
								onmouseover="MouseOver(this);"
								onmouseout="MouseOut(this);"
								onblur="MouseBlur(this);if(this.value==\'\'\'\')this.value=this.defaultValue;verificarModificaoItensComposicao(this); calcularValorTotal(\' || cos.cosid || \');"
								onkeyup="this.value=mascaraglobal(\'\'###.###.###.###,##\'\', this.value);"
								onfocus="if(this.value==this.defaultValue)this.value=\'\'\'\';"
								onclick="this.select()"
								name="cosvlruni[\'|| cos.cosid ||\']"
								' . $readonly . '
								value="\' || trim(to_char(coalesce(cos.cosvlruni, 0), \'999G999G999G990D99\')) || \'" />
							<input size="20" id="Hcosvlruni\' || cos.cosid || \'"
								type="hidden"
								class="itemComposicao"
								style="border-right: #888888 1px solid;border-top: #888888 1px solid;font-size: 10px;vertical-align: middle;border-left: #000000 3px solid;border-bottom: #888888 1px solid;font-family: verdana;background-color: #ffffff"
								name="Hcosvlruni[\'|| cos.cosid ||\']"
								' . $readonly . '
								value="\' || trim(to_char(coalesce(cos.cosvlruni, 0), \'999G999G999G990D999999\')) || \'" />\' as cosvlruni
						,\'<input size="20" id="cosvlrtotal\' || cos.cosid || \'" 
                			type="text"
                			class="itemComposicao" 
	                        style="border-right: #888888 1px solid;border-top: #888888 1px solid;font-size: 10px;vertical-align: middle;border-left: #000000 3px solid;border-bottom: #888888 1px solid;font-family: verdana;background-color: #ffffff"
	                        onmouseover="MouseOver(this);"
	                        onfocus="MouseClick(this);"
	                        onmouseout="MouseOut(this);"
	                        onblur="MouseBlur(this); verificarModificaoItensComposicao(this); calcularValorUnitario(\' || cos.cosid || \');"
	                        onkeyup="this.value=mascaraglobal(\'\'###.###.###.###,##\'\', this.value);"
	                        onfocus="this.select()"
	                        onclick="this.select()"
	                        ' . $readonly . '
	                        name="cosvlrtotal"
	                        value="\' || trim(to_char(coalesce(cos.cosqtd, 0) * coalesce(cos.cosvlruni, 0), \'999G999G999G990D99\')) || \'">
                    	<input size="20" id="Hcosvlrtotal\' || cos.cosid || \'" 
                			type="hidden"
                			class="itemComposicao" 
	                        style="border-right: #888888 1px solid;border-top: #888888 1px solid;font-size: 10px;vertical-align: middle;border-left: #000000 3px solid;border-bottom: #888888 1px solid;font-family: verdana;background-color: #ffffff"
	                        ' . $readonly . '
	                        name="Hcosvlrtotal"
	                        value="\' || trim(to_char(coalesce(cos.cosqtd, 0) * coalesce(cos.cosvlruni, 0), \'999G999G999G990D999999\')) || \'">\' as vlrtotal
                    	--,\'<span style="display:block;width:50px;" id="cosvlrtotal\' || cos.cosid || \'">\' || trim(to_char(coalesce(ecs.total, 0) * coalesce(cos.cosvlruni, 0), \'0D99\')) || \'</span>\' as vlrtotal
					FROM cte.composicaosubacao cos
						LEFT JOIN (SELECT cosid, SUM(ecsqtd) as total FROM cte.escolacomposicaosubacao GROUP BY cosid) ecs ON cos.cosid = ecs.cosid
                		INNER JOIN cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
                	WHERE cos.sbaid  = ' . $_REQUEST['sbaid']  . '
                    AND cos.cosano = ' . $_REQUEST['cosano'] . '
                	ORDER BY cos.cosdsc';

			$sql = $db->carregar($sql);

			$dat = $db->carregar('
									SELECT
				                        \' <div align="right"> Totais:</div>\' AS cosid,
										\'\' AS cosdsc,
										\'\' AS undddsc,
										\'\' AS cosqtd,
										\'\' AS cosvlruni,
										\'<input type="hidden" id="valorTotalSubAtual'.$_REQUEST['cosano'].'" name="valorTotalSubAtual'.$_REQUEST['cosano'].'" value=\' || trim(to_char(sum(cos.cosqtd * cos.cosvlruni), \'999G999G990D999999\')) || \'>
										  <div id="vlrTotalSubAtual'.$_REQUEST['cosano'].'" > R$ \' || trim(to_char(sum(cos.cosqtd * cos.cosvlruni), \'999G999G990D99\')) || \'</div>\' as vlrtotal
			                		FROM cte.composicaosubacao cos
			                			LEFT JOIN (SELECT cosid, SUM(ecsqtd) as total FROM cte.escolacomposicaosubacao GROUP BY cosid) ecs ON cos.cosid = ecs.cosid
			                			INNER JOIN cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
			                		WHERE cos.sbaid  = ' . $_REQUEST['sbaid']  . '
			                    	AND cos.cosano = ' . $_REQUEST['cosano']);

			$sql[] = $dat[0];
			
		}
		else {
			$sql = '
                	SELECT
                    	\'<center><img id="removeItens" name="removeItens" onclick="return ' .$remover. '(\' || cos.cosid || \')" src="/imagens/excluir.gif" title="Excluir" alt="Excluir \' || cos.cosdsc || \'" /></center>\' as cosid
                    	,cos.cosdsc
                    	,und.undddsc
	                    ,\'<input size="15" id="cosqtd\' || cos.cosid || \'" 
                            	type="text"
                            	class="CampoEstilo"
                            	onmouseover="MouseOver(this);"
	                            onfocus="MouseClick(this);"
	                            onmouseout="MouseOut(this);"
	                            onblur="MouseBlur(this); verificarModificaoItensComposicao(this); calcularValorTotal(\' || cos.cosid || \');"
	                            onfocus="this.select()"
	                            onclick="this.select()"
	                            name="cosqtd[\'|| cos.cosid ||\']"
	                            ' . $readonly . '
								value="\' || 
									case 
							    		when substring( trim(to_char(coalesce(cos.cosqtd, 0), \'9999999990D999999\')), position(\',\' in trim(to_char(coalesce(cos.cosqtd, 0), \'9999999990D999999\'))) +1, 6 ) = \'000000\'
							    			then trim(to_char(coalesce(cos.cosqtd, 0), \'999G999G999\'))
							    		else trim(to_char(coalesce(cos.cosqtd, 0), \'9G999G999G990D999999\'))
							    	END
								|| \'" />\' as cosqtd
                    	,\'<input size="20" id="cosvlruni\' || cos.cosid || \'"
                            	type="text"
                            	class="itemComposicao"
	                            style="border-right: #888888 1px solid;border-top: #888888 1px solid;font-size: 10px;vertical-align: middle;border-left: #000000 3px solid;border-bottom: #888888 1px solid;font-family: verdana;background-color: #ffffff"
	                            onmouseover="MouseOver(this);"
	                            onfocus="MouseClick(this);"
	                            onmouseout="MouseOut(this);"
	                            onblur="MouseBlur(this); verificarModificaoItensComposicao(this); calcularValorTotal(\' || cos.cosid || \');"
	                            onkeyup="this.value=mascaraglobal(\'\'###.###.###.###,##\'\', this.value);"
	                            onfocus="this.select()"
	                            onclick="this.select()"
	                            name="cosvlruni[\'|| cos.cosid ||\']"
	                            ' . $readonly . '
	                            value="\' || trim(to_char(coalesce(cos.cosvlruni, 0), \'999G999G999G990D99\')) || \'" />
                            <input size="20" id="Hcosvlruni\' || cos.cosid || \'"
                            	type="hidden"
                            	class="itemComposicao"
	                            style="border-right: #888888 1px solid;border-top: #888888 1px solid;font-size: 10px;vertical-align: middle;border-left: #000000 3px solid;border-bottom: #888888 1px solid;font-family: verdana;background-color: #ffffff"
	                            name="Hcosvlruni[\'|| cos.cosid ||\']"
	                            ' . $readonly . '
	                            value="\' || trim(to_char(coalesce(cos.cosvlruni, 0), \'999G999G999G990D999999\')) || \'" />\' as cosvlruni
	                    ,\'<input size="20" id="cosvlrtotal\' || cos.cosid || \'" 
                			type="text"
                			class="itemComposicao" 
	                        style="border-right: #888888 1px solid;border-top: #888888 1px solid;font-size: 10px;vertical-align: middle;border-left: #000000 3px solid;border-bottom: #888888 1px solid;font-family: verdana;background-color: #ffffff"
	                        onmouseover="MouseOver(this);"
	                        onfocus="MouseClick(this);"
	                        onmouseout="MouseOut(this);"
	                        onblur="MouseBlur(this); verificarModificaoItensComposicao(this); calcularValorUnitario(\' || cos.cosid || \');"
	                        onkeyup="this.value=mascaraglobal(\'\'###.###.###.###,##\'\', this.value);"
	                        onfocus="this.select()"
	                        onclick="this.select()"
	                        ' . $readonly . '
	                        name="cosvlrtotal"
	                        value="\' || trim(to_char(coalesce(cos.cosqtd, 0) * coalesce(cos.cosvlruni, 0), \'999G999G999G990D99\')) || \'">
                        <input size="20" id="Hcosvlrtotal\' || cos.cosid || \'" 
                			type="hidden"
                			class="itemComposicao" 
	                        style="border-right: #888888 1px solid;border-top: #888888 1px solid;font-size: 10px;vertical-align: middle;border-left: #000000 3px solid;border-bottom: #888888 1px solid;font-family: verdana;background-color: #ffffff"
	                        ' . $readonly . '
	                        name="Hcosvlrtotal"
	                        value="\' || trim(to_char(coalesce(cos.cosqtd, 0) * coalesce(cos.cosvlruni, 0), \'999G999G999G990D999999\')) || \'">\' as vlrtotal	                   
                    	--,\'<span style="display:block;width:50px;" id="cosvlrtotal\' || cos.cosid || \'">\' || trim(to_char(coalesce(cos.cosqtd, 0) * coalesce(cos.cosvlruni, 0), \'0D99\')) || \'</span>\' as vlrtotal
                	FROM cte.composicaosubacao cos
                		INNER JOIN cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
                	WHERE cos.sbaid  = ' . $_REQUEST['sbaid']  . '
                    AND cos.cosano = ' . $_REQUEST['cosano'] . '
                	ORDER BY cos.cosdsc';

			$sql   = $db->carregar($sql);
			$dat   = $db->carregar('
									SELECT
				                        \' <div align="right"> Totais:</div>\' AS cosid,
										\'\' AS cosdsc,
										\'\' AS undddsc,
										\'\' AS cosqtd,
										\'\' AS cosvlruni,
										trim(to_char(sum(cos.cosqtd * cos.cosvlruni), \'999G999G990D99\')) as vlrtotal
				                	FROM cte.composicaosubacao cos
				                		LEFT JOIN (SELECT cosid, SUM(ecsqtd) as total FROM cte.escolacomposicaosubacao GROUP BY cosid) ecs ON cos.cosid = ecs.cosid
				                		INNER JOIN cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
				                	WHERE cos.sbaid  = ' . $_REQUEST['sbaid']  . '
				                    AND cos.cosano = ' . $_REQUEST['cosano']);

			$sql[] = $dat[0];
		}
	} 
	else{
		$sql = '
            	SELECT
					\'<center><img id="removeItens" name="removeItens" onclick="return ' .$remover. '(\' || cos.cosid || \')" src="/imagens/excluir.gif" title="Excluir" alt="Excluir \' || cos.cosdsc || \'" /></center>\' as cosid
					,cos.cosdsc
					,und.undddsc
					,\'<input size="15" id="cosqtd\' || cos.cosid || \'"
							type="text"
							class="CampoEstilo"
							onmouseover="MouseOver(this);"
							onfocus="MouseClick(this);"
							onmouseout="MouseOut(this);"
							onblur="MouseBlur(this); verificarModificaoItensComposicao(this); calcularValorTotal(\' || cos.cosid || \');"
							onfocus="this.select()"
							onclick="this.select()"
							name="cosqtd[\'|| cos.cosid ||\']"
							' . $readonly . '
								value="\' || 
									case 
							    		when substring( trim(to_char(coalesce(cos.cosqtd, 0), \'9999999990D999999\')), position(\',\' in trim(to_char(coalesce(cos.cosqtd, 0), \'9999999990D999999\'))) +1, 6 ) = \'000000\'
							    			then trim(to_char(coalesce(cos.cosqtd, 0), \'999G999G999\'))
							    		else trim(to_char(coalesce(cos.cosqtd, 0), \'9G999G999G990D999999\'))
							    	END
								|| \'" />\' as cosqtd
					,\'<input size="20" id="cosvlruni\' || cos.cosid || \'"
	                        type="text"
	                        class="itemComposicao"
	                        style="border-right: #888888 1px solid;border-top: #888888 1px solid;font-size: 10px;vertical-align: middle;border-left: #000000 3px solid;border-bottom: #888888 1px solid;font-family: verdana;background-color: #ffffff"
	                        onmouseover="MouseOver(this);"
	                        onfocus="MouseClick(this);"
	                        onmouseout="MouseOut(this);"
	                        onblur="MouseBlur(this); verificarModificaoItensComposicao(this); calcularValorTotal(\' || cos.cosid || \');"
	                        onkeyup="this.value=mascaraglobal(\'\'###.###.###.###,##\'\', this.value);"
	                        onfocus="this.select()"
	                        onclick="this.select()"
	                        name="cosvlruni[\'|| cos.cosid ||\']"
	                        ' . $readonly . '
	                        value="\' || trim(to_char(coalesce(cos.cosvlruni, 0), \'999G999G999G990D99\')) || \'" />
                        <input size="20" id="Hcosvlruni\' || cos.cosid || \'"
	                        type="hidden"
	                        class="itemComposicao"
	                        style="border-right: #888888 1px solid;border-top: #888888 1px solid;font-size: 10px;vertical-align: middle;border-left: #000000 3px solid;border-bottom: #888888 1px solid;font-family: verdana;background-color: #ffffff"
	                        name="Hcosvlruni[\'|| cos.cosid ||\']"
	                        ' . $readonly . '
	                        value="\' || trim(to_char(coalesce(cos.cosvlruni, 0), \'999G999G999G990D999999\')) || \'" />\' as cosvlruni
	                ,\'<input size="20" id="cosvlrtotal\' || cos.cosid || \'" 
                			type="text"
                			class="itemComposicao" 
	                        style="border-right: #888888 1px solid;border-top: #888888 1px solid;font-size: 10px;vertical-align: middle;border-left: #000000 3px solid;border-bottom: #888888 1px solid;font-family: verdana;background-color: #ffffff"
	                        onmouseover="MouseOver(this);"
	                        onfocus="MouseClick(this);"
	                        onmouseout="MouseOut(this);"
	                        onblur="MouseBlur(this); verificarModificaoItensComposicao(this); calcularValorUnitario(\' || cos.cosid || \');"
	                        onkeyup="this.value=mascaraglobal(\'\'###.###.###.###,##\'\', this.value);"
	                        onfocus="this.select()"
	                        onclick="this.select()"
	                        ' . $readonly . '
	                        name="cosvlrtotal"
	                        value="\' || trim(to_char(coalesce(cos.cosqtd, 0) * coalesce(cos.cosvlruni, 0), \'999G999G999G990D99\')) || \'">
                        <input size="20" id="Hcosvlrtotal\' || cos.cosid || \'" 
                			type="hidden"
                			class="itemComposicao" 
	                        style="border-right: #888888 1px solid;border-top: #888888 1px solid;font-size: 10px;vertical-align: middle;border-left: #000000 3px solid;border-bottom: #888888 1px solid;font-family: verdana;background-color: #ffffff"
	                        ' . $readonly . '
	                        name="Hcosvlrtotal"
	                        value="\' || trim(to_char(coalesce(cos.cosqtd, 0) * coalesce(cos.cosvlruni, 0), \'999G999G999G990D999999\')) || \'">\' as vlrtotal
                	--,\'<span style="display:block;width:50px;" id="cosvlrtotal\' || cos.cosid || \'">\' || trim(to_char(coalesce(cos.cosqtd, 0) * coalesce(cos.cosvlruni, 0), \'0D99\')) || \'</span>\' as vlrtotal
				FROM cte.composicaosubacao cos
            		INNER JOIN cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
            	WHERE cos.sbaid  = ' . $_REQUEST['sbaid']  . '
                AND cos.cosano = ' . $_REQUEST['cosano'] . '
            	GROUP BY cos.cosdsc ,und.undddsc , cosqtd ,cosvlruni ,cos.cosid
            	ORDER BY cos.cosdsc';

		$sql = $db->carregar($sql);

		$dat = $db->carregar('
								SELECT
									\' <div align="right" id="Total"> totais : </div>\' AS cosid,
									\'\' AS cosdsc,
									\'\' AS undddsc,
									\'\' AS cosqtd,
									\'\' AS cosvlruni,
									trim(to_char(sum(cos.cosqtd * cos.cosvlruni), \'999G999G990D99\')) as vlrtotal
								FROM cte.composicaosubacao cos
									INNER JOIN cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
								WHERE cos.sbaid  = ' . $_REQUEST['sbaid']  . '
								AND cos.cosano = ' . $_REQUEST['cosano']);
		
		$sql[] = $dat[0];
		
	}
	return $sql;
}

/*
 * Função que monta o SQL para carregar os Beneficiários
 * @return string - SQL dos Beneficiários
 */
function montarSQLBeneficiarios(){
	
	$sql = '
	        SELECT 
	        	\'<center><img id="removeBeneficiario" name="removeBeneficiario" onclick="return removerBeneficiario(\' || sb.benid || \')" src="/imagens/excluir.gif" title="Excluir" style="border:none" alt="Excluir item" /></center>\' as benid,
	            be.bendsc,
	            sb.vlrurbano as vlrurbano,
	            sb.vlrrural  as vlrrural,
	            sb.vlrurbano + sb.vlrrural as sabvlrtotal
	        FROM cte.subacaobeneficiario sb
	        	INNER JOIN cte.beneficiario be ON sb.benid = be.benid
	        WHERE sb.sbaid  = ' . $_REQUEST['sbaid']   . '
			AND sb.sabano   = ' . $_REQUEST['sabano'];
	
	return $sql;
}

/*
 * Função que monta o SQL para carregar as Escolas
 * @return string - SQL das Escolas
 */
function montarSQLEscolas(){
	
	global $db;
	
	$sql = ' 
			SELECT
	            \'<center><img id="removeEscolas" name="removeEscolas" onclick="return removerEscola(\' || q.qfaid || \')" src="/imagens/excluir.gif" style="border:none" title="Excluir" alt="Excluir item" /> <img onclick="return extratoEscolas(\' || q.qfaid || \')" src="/imagens/consultar.gif" style="border:none" title="Extrato" alt="Extrato item" /></center>\' as qfaid,
	            e.entcodent,
	            e.entnome,
	            mundescricao,
	            CASE
			       WHEN entd.entdoferta_ept = true THEN \'Sim\'
			    ELSE \'Não\'
			       END AS oferta_ept,
	            sum(coalesce(qfaqtd,0))
        	FROM cte.qtdfisicoano q
        		INNER JOIN entidade.entidade e on q.entid = e.entid
				LEFT JOIN entidade.endereco en on en.entid = e.entid
        		LEFT JOIN territorios.municipio m on m.muncod = en.muncod
        		LEFT JOIN entidade.entidadedetalhe entd on entd.entcodent = e.entcodent
        	WHERE q.qfaano = ' . $_REQUEST['qfaano'] . '
            AND q.sbaid  = ' . $_REQUEST['sbaid']  . '
        	GROUP BY q.qfaid, e.entcodent, e.entnome, mundescricao, oferta_ept
        	ORDER BY entnome';
		$arEscolas = $db->carregar($sql);	
	
		if( $arEscolas ){
			$soma = 0;
			foreach( $arEscolas as $escola ){
				$soma += isset( $escola["sum"] ) ? $escola["sum"] : 0;
			}
		
	        $arTotais["qfaid"] = "";
	        $arTotais["entcodent"] = ""; 
	        $arTotais["entnome"] = "<b>Total de Escolas: &nbsp;&nbsp;". count($arEscolas)."</b>";
	        $arTotais["mundescricao"] = ""; 
	        $arTotais["oferta_ept"] = ""; 
	        $arTotais["sum"] = "<div style='text-align: right;' ><b>Total: &nbsp;&nbsp;$soma</b></div>";
	        
	        $arEscolas[] = $arTotais;
		}
		else{
			$arEscolas = $sql;
		}
	
	return $arEscolas; 	
}

/*
 * Função que importa os dados de um ano para outro
 * @param int $sbaid - Identificação da subação 
 * @param int $anoExercicio  - Ano de destino dos dados  
 * @param int $anoImportacao - Ano de origem dos dados 
 * @return bool
 */
function importarDadosSubAcao( $sbaid, $anoExercicio, $anoImportacao, $sbaidDestino = NULL ){
	
	global $db;

	/****************************************************************************
	* 							ITENS DE COMPOSIÇÃO								*
	****************************************************************************/
	
	// Recuperando itens de composição do ano de origem
	$sql = "select  cosid, sbaid, cosord, cosdsc, cosqtd, cosvlruni, 
					coalesce( unddid, 999 ) as unddid, 
					coalesce( ilbid, 999 ) as ilbid, 
					cosano
			from cte.composicaosubacao 
			where cosano = $anoImportacao
			and sbaid = $sbaid";
	
	// Recuperando itens de composição do ano de destino
	$resComposicaoImportacao = $db->carregar( $sql ) ? $db->carregar( $sql ) : array();

	$sbaidPesquisa = $sbaidDestino ? $sbaidDestino : $sbaid;
	$sql = "select *
			from cte.composicaosubacao 
			where cosano = $anoExercicio
			and sbaid = $sbaidPesquisa";
	
	$resComposicaoExercicio = (array) $db->carregar( $sql );
	
	$arEquivalenciaComposicao = array();
	
	// Varre os itens de composição do ano de origem
	if(is_array($resComposicaoImportacao) ){
		foreach( $resComposicaoImportacao as $indice => $composicaoImportacao ){
		
		$cosid = 0;
		$cosqtd = 0;
		$boItemExistente = false;

		// Varre os itens de composição do ano de destino e verifica se já existe o mesmo item de origem nesse
		foreach( $resComposicaoExercicio as $composicaoExercicio ){
			
			if( strtolower( $composicaoImportacao["cosdsc"] ) == strtolower( $composicaoExercicio["cosdsc"] ) && $composicaoImportacao["unddid"] == $composicaoExercicio["unddid"] ){
				$boItemExistente = true;
				$cosqtd = $composicaoExercicio['cosqtd'];
				$cosid = $composicaoExercicio['cosid'];
				break;
			}
			else{
				$boItemExistente = false;
			}
		}
		
		// Se o item de composição de origem já existe no destino somente atualiza o destino somando suas quantidades
		if( $boItemExistente ){
			$sql = "update cte.composicaosubacao 
						set cosqtd = ". $composicaoImportacao['cosqtd'] ." + ". $cosqtd ."
					where cosid = ". $cosid;

			$db->executar( $sql );
			
		}
		// Se o item de composição de origem não existe no destino insere os mesmos valores da origem
		else{
			$sbaidDestinoIC = $sbaidDestino ? $sbaidDestino : $composicaoImportacao['sbaid'];
			
			$sql = "insert into cte.composicaosubacao ( sbaid, cosord, cosdsc, cosqtd, cosvlruni, unddid, ilbid, cosano )
			 									values( ".$sbaidDestinoIC.", 
			 											".$composicaoImportacao['cosord'].",
			 											'". str_replace( "'", '"', $composicaoImportacao['cosdsc'] ) ."',
			 											".$composicaoImportacao['cosqtd'].",
			 											".$composicaoImportacao['cosvlruni'].",";
			$sql .= $composicaoImportacao['unddid'] == 999 ? 'null, ' : $composicaoImportacao['unddid'].",";
			$sql .= $composicaoImportacao['ilbid']  == 999 ? 'null, ' : $composicaoImportacao['ilbid'].",";
			$sql .=										"$anoExercicio )
														returning cosid";		
			$cosid = $db->pegaUm( $sql );
		
		}
		
		// Armazena valores dos Ids de origem e destino equivalentes
		// para salvar dados na tabela cte.escolacomposicaosubacao de acordo com os dados originais 
		$arEquivalenciaComposicao[$indice]["cosIdNovo"] = $cosid;
		$arEquivalenciaComposicao[$indice]["cosIdAntigo"] = $composicaoImportacao['cosid'];
		}
	}
	
	/****************************************************************************
	* 							PARECER E CRONOGRAMA							*
	****************************************************************************/
	
	if($sbaidDestino){
		// Recuperando cronograma e parecer do ano de origem
		$sql = "select sbaid, sptparecer, sptunt, sptuntdsc, sptano, sptinicio, sptfim, tppid, ssuid, sptanoterminocurso,  sptplanointerno, sptnumprocesso 
				from cte.subacaoparecertecnico 
				where sbaid = ".$sbaid."
				and sptano = ".$anoImportacao;
		
		$resParecerCronograma = $db->pegaLinha( $sql ) ? $db->pegaLinha( $sql ) : array();
			
		if(is_array($resParecerCronograma)){
			// insere cronograma do ano de origem
			$resParecerCronograma['sptparecer'] 		= $resParecerCronograma['sptparecer'] ? $resParecerCronograma['sptparecer'] : "''";
			$resParecerCronograma['sptunt'] 			= $resParecerCronograma['sptunt'] ? $resParecerCronograma['sptunt'] : 0;
			$resParecerCronograma['sptuntdsc'] 			= $resParecerCronograma['sptuntdsc'] ? $resParecerCronograma['sptuntdsc'] : "''";
			$resParecerCronograma['ssuid'] 				= $resParecerCronograma['ssuid'] ? $resParecerCronograma['ssuid'] : "''";
			$resParecerCronograma['sptanoterminocurso'] = $resParecerCronograma['sptanoterminocurso'] ? $resParecerCronograma['sptanoterminocurso'] : "null";
			$resParecerCronograma['sptplanointerno'] 	= $resParecerCronograma['sptplanointerno'] ? $resParecerCronograma['sptplanointerno'] : "''";
			$resParecerCronograma['sptnumprocesso'] 	= $resParecerCronograma['sptnumprocesso'] ? $resParecerCronograma['sptnumprocesso'] : "''";
			
			if($resParecerCronograma['sptinicio']){
			$sql = "insert into cte.subacaoparecertecnico ( sbaid, sptparecer, sptunt, sptuntdsc, sptano, 
															sptinicio, sptfim, tppid, ssuid, sptanoterminocurso,  
															sptplanointerno, sptnumprocesso )
			 									values( ".$sbaidDestino.",
			 											'".$resParecerCronograma['sptparecer']."',
			 											".$resParecerCronograma['sptunt'].",
														'123',
														".$resParecerCronograma['sptano'].",
														".$resParecerCronograma['sptinicio'].",
														".$resParecerCronograma['sptfim'].",
														".$resParecerCronograma['tppid'].",
														".$resParecerCronograma['ssuid'].",
														".$resParecerCronograma['sptanoterminocurso'].",
														'".$resParecerCronograma['sptplanointerno']."',
														".$resParecerCronograma['sptnumprocesso']."
														 )";
			
				$db->executar( $sql );
//	echo $sql; 
//	die();
			}
			
		}
			// Recuperando beneficiarios do ano de origem
			$sql = "select *
					from cte.subacaobeneficiario where sabano = $anoImportacao
					and sbaid = ".$sbaid;
			
			$resBenImportacao = $db->carregar( $sql ) ? $db->carregar( $sql ) : array();
			if(is_array($resBenImportacao) ){
				// Varre beneficiario do ano de origem
				foreach( $resBenImportacao as $indice => $benImportacao ){
					if($benImportacao['benid']){
						$sql = "insert into cte.subacaobeneficiario ( sbaid, sabano, vlrurbano, vlrrural, benid )
						 									values( ".$sbaidDestino.", 
						 											".$anoExercicio.",
						 											".$benImportacao['vlrurbano'].",
						 											".$benImportacao['vlrrural'].",
						 											".$benImportacao['benid']."
																	 )";
			
						$db->executar( $sql );
					}
				}
			}
	}
	
	
	
	/****************************************************************************
	* 								  ESCOLAS									*
	****************************************************************************/

	// Recuperando escolas do ano de origem
	$sql = "select *
			from cte.qtdfisicoano
			where qfaano = $anoImportacao
			and sbaid = $sbaid";

	$resEscolaImportacao = $db->carregar( $sql ) ? $db->carregar( $sql ) : array();
	if(is_array($resEscolaImportacao) ){
		$sbaidPesquisa = $sbaidDestino ? $sbaidDestino : $sbaid;

		// Recuperando escolas do ano de destino
		$sql = "select *
				from cte.qtdfisicoano
				where qfaano = $anoExercicio
				and sbaid = $sbaidPesquisa";
	
		$resEscolaExercicio = (array) $db->carregar( $sql );
	
		$arEquivalenciaEscola = array();

		// Varre as escolas do ano de origem
		foreach( $resEscolaImportacao as $indice => $escolaImportacao ){
		
			$qfaid = 0;
			$qfaqtd = 0;
			$boEscolaExistente = false;
		
			// Varre as escolas do ano de destino e verifica se já existe a mesma escola de origem nesse
			foreach( $resEscolaExercicio as $escolaExercicio ){
			
				if( $escolaImportacao["entid"] == $escolaExercicio["entid"] ){
					$boEscolaExistente = true;
					$qfaqtd = $escolaExercicio['qfaqtd'];
					$qfaid  = $escolaExercicio['qfaid'];
					break;
				}
				else{
					$boEscolaExistente = false;
				}
			}
		
		// Se a escola de origem já existe no destino somente atualiza o destino somando suas quantidades
			if( $boEscolaExistente ){
				$sql = "update cte.qtdfisicoano 
							set qfaqtd = ". $escolaImportacao['qfaqtd'] ." + ". $qfaqtd ."
						where qfaid = ". $qfaid;
	
				$db->executar( $sql );
				
			}
			// Se a escola de origem não existe no destino insere os mesmos valores da origem
			else{
				$sbaidDestinoQ = $sbaidDestino ? $sbaidDestino : $escolaImportacao['sbaid'];
				$sql = "insert into cte.qtdfisicoano ( sbaid, qfaqtd, entid, qfaano )
				 									values( ".$sbaidDestinoQ.", 
				 											".$escolaImportacao['qfaqtd'].",
				 											".$escolaImportacao['entid'].",
															$anoExercicio )
															returning qfaid";

				$qfaid = $db->pegaUm( $sql );
		
			}
		
			// Armazena valores dos Ids de origem e destino equivalentes
			// para salvar dados na tabela cte.escolacomposicaosubacao de acordo com os dados originais 		
			$arEquivalenciaEscola[$indice]["qfaIdNovo"] = $qfaid;
			$arEquivalenciaEscola[$indice]["qfaIdAntigo"] = $escolaImportacao['qfaid'];
		}
	}

	/****************************************************************************
	* 							ESCOLA_COMPOSIÇÃO_SUBAÇÃO						*
	****************************************************************************/	

	// Varre o array de equivalência dos itens de composição 
	if(is_array($arEquivalenciaComposicao)){
		foreach( $arEquivalenciaComposicao as $indice => $arComposicao ){
		
		// Carrega todos os registros que possuem os itens na tabela cte.escolacomposicaosubacao
		$sql = "select qfaid, ecsqtd from cte.escolacomposicaosubacao where cosid = ". $arComposicao["cosIdAntigo"];
		$res = $db->carregar( $sql );

		if( is_array( $res ) ){
			foreach( $res as $arEscolaComposicao ){
				
				// Varre o array de equivalência das escolas
				foreach( $arEquivalenciaEscola as $arEscolas ){
					
					// Se posuir registro com mesmo item de composição e mesma escola de origem
					// Verifica se existe registro equivalente do destino
					// Se existir, somente soma as quantidades de origem e destino
					// Se não existir, insere novo registro
					if( $arEscolaComposicao["qfaid"] == $arEscolas["qfaIdAntigo"] ){
						 
						$sql = "select count(*) 
								from cte.escolacomposicaosubacao 
								where cosid = ".$arComposicao["cosIdNovo"]."
								and qfaid = ".$arEscolas["qfaIdNovo"];

						if( $db->pegaUm( $sql ) ){
							$sql = "update cte.escolacomposicaosubacao 
												set ecsqtd = ".$arEscolaComposicao["ecsqtd"]." + ecsqtd
							 		where cosid = ".$arComposicao["cosIdNovo"]."
							 		and qfaid = ".$arEscolas["qfaIdNovo"]; 
						}
						else{
							$sql = "insert into cte.escolacomposicaosubacao 
							 					 values( ".$arComposicao["cosIdNovo"].", 
							 					 		 ".$arEscolas["qfaIdNovo"].", 
							 					 		 ".$arEscolaComposicao["ecsqtd"]." )";
						}
						$db->executar( $sql );
					}
				}
			}
		}
	}
	
	$db->commit();	
	}
	return true;
	
}

	/*******************************
	* 
	*	FUNCTION: subacaoConveniada();
	* 	17/06/2008
	* 	DESCRIÇÃO:
	* 	Verifica se a subação foi gerada convênio com o FNDE
	* 
	* 	@PARAM 	RETORNO : $conveniada - Ano do convênio 
	* 
	*******************************/
	function subacaoConveniada(){
		global $db;
		/*
		$sql = 'select sc.sbcano
			from cte.subacaoconvenio sc
			inner join cte.convenio c on c.cnvid = sc.cnvid
			inner join cte.convenioretorno cr on cr.cnvid = c.cnvid
			where sbaid = '.trim($_REQUEST['sbaid']);
		*/
		$sql="select pssano as sbcano from cte.projetosapesubacao where sbaid = ".trim($_REQUEST['sbaid']);
//		 "-- and sbcano = date_part('year', current_date)";
		//$conveniada = $db->pegaUm($sql);	
		$conveniada = $db->carregar($sql);
//		echo $_REQUEST['sbaid'];
//		die();
		return $conveniada;
	}
	
/*
 * Função que monta o SQL para carregar as Escolas do Relatorio da Subação
 * @return string - SQL das Escolas
 */
function montarSQLEscolasRelatorioCompleto($strSubacoesFilhos){
	if($strSubacoesFilhos){
		$strSubacoesFilhos = ",".$strSubacoesFilhos;
	}
	global $db;
	$strSubacoesFilhos = $strSubacoesFilhos;
	
	$sql = ' 
			SELECT
				CASE WHEN q.sbaid = ' . $_REQUEST['sbaid']   . ' THEN \'Original\' ELSE \'Aditivo\' || sba.sbaseqaditivo END as tipo,
	            e.entcodent,
	            e.entnome,
	            sum(coalesce(qfaqtd,0))
        	FROM cte.qtdfisicoano q
        		INNER JOIN entidade.entidade e on q.entid = e.entid
        		INNER JOIN cte.subacaoindicador sba ON sba.sbaid = q.sbaid
        	WHERE q.qfaano = ' . $_REQUEST['qfaano'] . '
            AND q.sbaid  in ( ' . $_REQUEST['sbaid']   .$strSubacoesFilhos.')
        	GROUP BY q.sbaid, q.qfaid, e.entcodent, e.entnome, sba.sbaseqaditivo
        	ORDER BY q.sbaid , entnome';
		
		$arEscolas = $db->carregar($sql);	
	
		if( $arEscolas ){
			$soma = 0;
			foreach( $arEscolas as $escola ){
				$soma += isset( $escola["sum"] ) ? $escola["sum"] : 0;
			}
		
	        $arTotais["qfaid"] = "";
	        $arTotais["entcodent"] = ""; 
	        $arTotais["entnome"] = "<b>Total de Escolas: &nbsp;&nbsp;<span id='somaTotalEscolas".$_REQUEST['qfaano']."'>". count($arEscolas)."</span></b>";
	        $arTotais["sum"] = "<div style='text-align: right;' ><b>Total: &nbsp;&nbsp;$soma</b></div>";
	        
	        $arEscolas[] = $arTotais;
		}
		else{
			$arEscolas = $sql;
		}
	
	return $arEscolas; 	
}

/*
 * Função que monta o SQL para carregar Itens de Composição do Relatorio da Subação
 * @return string - SQL dos itens de composição
 */
function montarSQLItensComposicaoRelatorioCompleto($strSubacoesFilhos){
	if($strSubacoesFilhos){
		$strSubacoesFilhos = ",".$strSubacoesFilhos;
	}
	global $db;
	$strSubacoesFilhos = $strSubacoesFilhos;

	$sql = '
	        SELECT count(sba.undid)
	        FROM cte.subacaoindicador sba
	        	INNER JOIN cte.unidademedida und ON sba.undid  = und.undid
	        	INNER JOIN cte.laboratorio lab ON und.undid = lab.undid
	        WHERE sbaid in ( ' . $_REQUEST['sbaid']. $strSubacoesFilhos.')';

	if( $_REQUEST['sbaporescola'] == 't' ){
		if( $readonly == '' ){
			$sql = '
                	SELECT
                		CASE WHEN cos.sbaid = ' . $_REQUEST['sbaid']   . ' THEN \'Original\' ELSE \'Aditivo\'|| sba.sbaseqaditivo  END as tipo,
                    	 cos.cosdsc
                    	,und.undddsc
                    	,\'<div id="cosqtd\' || cos.cosid || \'"> \' ||trim(to_char(coalesce(ecs.total, 0), \'9999999990D9\'))  || \' </div> \'as cosqtd
						,\'<div id="cosvlruni\' || cos.cosid || \'"> \' || trim(to_char(coalesce(cos.cosvlruni, 0), \'0D99\'))  || \' </div> \'as cosvlruni
					
                    	,\'<span style="display:block;width:50px;" id="cosvlrtotal\' || cos.cosid || \'">\' || trim(to_char(coalesce(ecs.total, 0) * coalesce(cos.cosvlruni, 0), \'0D99\')) || \'</span>\' as vlrtotal
					FROM cte.composicaosubacao cos
						LEFT JOIN (SELECT cosid, SUM(ecsqtd) as total FROM cte.escolacomposicaosubacao GROUP BY cosid) ecs ON cos.cosid = ecs.cosid
                		INNER JOIN cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
                		INNER JOIN cte.subacaoindicador sba ON sba.sbaid = cos.sbaid
                	WHERE cos.sbaid  in ( ' . $_REQUEST['sbaid']  .$strSubacoesFilhos. ')
                    AND cos.cosano = ' . $_REQUEST['cosano'] . '
                	ORDER BY cos.sbaid, cos.cosdsc';
			
			$sql = $db->carregar($sql);

			$dat = $db->carregar('
									SELECT
				                        \' <div align="right"> Totais:</div>\' AS cosid,
				                        \'\' AS tipo,
										\'\' AS undddsc,
										\'\' AS cosqtd,
										\'\' AS cosvlruni,
										trim(to_char(sum(cos.cosqtd * cos.cosvlruni), \'99,999,999.99\')) as vlrtotal
			                		FROM cte.composicaosubacao cos
			                			LEFT JOIN (SELECT cosid, SUM(ecsqtd) as total FROM cte.escolacomposicaosubacao GROUP BY cosid) ecs ON cos.cosid = ecs.cosid
			                			INNER JOIN cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
			                		WHERE cos.sbaid in ( ' . $_REQUEST['sbaid']  .$strSubacoesFilhos. ')
			                    	AND cos.cosano = ' . $_REQUEST['cosano']);

			$sql[] = $dat[0];
			
		}
		else {
			$sql = '
                	SELECT
                		 CASE WHEN cos.sbaid = ' . $_REQUEST['sbaid']   . ' THEN \'Original\' ELSE \'Aditivo\' || sba.sbaseqaditivo  END as tipo,
                    	cos.cosdsc
                    	,und.undddsc
                    	,\'<div id="cosqtd\' || cos.cosid || \'"> \' ||trim(to_char(coalesce(cos.cosqtd, 0), \'9999999990D9\')) || \' </div> \'as cosqtd
						,\'<div id="cosvlruni\' || cos.cosid || \'"> \' ||  trim(to_char(coalesce(cos.cosvlruni, 0), \'0D99\')) || \' </div> \'as cosvlruni
					
                    	,\'<span style="display:block;width:50px;" id="cosvlrtotal\' || cos.cosid || \'">\' || trim(to_char(coalesce(cos.cosqtd, 0) * coalesce(cos.cosvlruni, 0), \'0D99\')) || \'</span>\' as vlrtotal
                	FROM cte.composicaosubacao cos
                		INNER JOIN cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
                		INNER JOIN cte.subacaoindicador sba ON sba.sbaid = cos.sbaid
                	WHERE cos.sbaid  in ( ' . $_REQUEST['sbaid']  .$strSubacoesFilhos. ')
                    AND cos.cosano = ' . $_REQUEST['cosano'] . '
                	ORDER BY cos.sbaid, cos.cosdsc';

			$sql   = $db->carregar($sql);
			$dat   = $db->carregar('
									SELECT
				                        \' <div align="right"> Totais:</div>\' AS cosid,
				                        \'\' AS tipo,
										\'\' AS undddsc,
										\'\' AS cosqtd,
										\'\' AS cosvlruni,
										trim(to_char(sum(cos.cosqtd * cos.cosvlruni), \'999999990D99\')) as vlrtotal
				                	FROM cte.composicaosubacao cos
				                		LEFT JOIN (SELECT cosid, SUM(ecsqtd) as total FROM cte.escolacomposicaosubacao GROUP BY cosid) ecs ON cos.cosid = ecs.cosid
				                		INNER JOIN cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
				                	WHERE cos.sbaid  in ( ' . $_REQUEST['sbaid']  .$strSubacoesFilhos. ')
				                    AND cos.cosano = ' . $_REQUEST['cosano']);

			$sql[] = $dat[0];
		}
	} 
	else{
		$sql = '
            	SELECT
            		 CASE WHEN cos.sbaid = ' . $_REQUEST['sbaid']   . ' THEN \'Original\' ELSE \'Aditivo\' || sba.sbaseqaditivo  END as tipo,
					 cos.cosdsc
					,und.undddsc
					,\'<div id="cosqtd\' || cos.cosid || \'"> \' || trim(to_char(coalesce(cos.cosqtd, 0), \'9999999990D9\'))  || \' </div> \'as cosqtd
					,\'<div id="cosvlruni[\'|| cos.cosid ||\']"> \' ||  trim(to_char(coalesce(cos.cosvlruni, 0), \'0D99\'))  || \' </div> \'as cosvlruni
					
                	,\'<span style="display:block;width:50px;" id="cosvlrtotal\' || cos.cosid || \'">\' || trim(to_char(coalesce(cos.cosqtd, 0) * coalesce(cos.cosvlruni, 0), \'0D99\')) || \'</span>\' as vlrtotal
				FROM cte.composicaosubacao cos
            		INNER JOIN cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
            		INNER JOIN cte.subacaoindicador sba ON sba.sbaid = cos.sbaid
            	WHERE cos.sbaid  in ( ' . $_REQUEST['sbaid']  .$strSubacoesFilhos. ')
                AND cos.cosano = ' . $_REQUEST['cosano'] . '
            	GROUP BY cos.cosdsc ,und.undddsc , cosqtd ,cosvlruni ,cos.cosid, cos.sbaid, sba.sbaseqaditivo
            	ORDER BY cos.sbaid, cos.cosdsc';
		//dbg($sql,1);
		$sql = $db->carregar($sql);

		$dat = $db->carregar('
								SELECT
									\' <div align="right"> totais : </div>\' AS cosid,
									\'\' AS tipo,
									\'\' AS undddsc,
									\'\' AS cosqtd,
									\'\' AS cosvlruni,
									trim(to_char(sum(cos.cosqtd * cos.cosvlruni), \'99,999,999.99\')) as vlrtotal
								FROM cte.composicaosubacao cos
									INNER JOIN cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
								WHERE cos.sbaid  in ( ' . $_REQUEST['sbaid']  .$strSubacoesFilhos. ')
								AND cos.cosano = ' . $_REQUEST['cosano']);

		$sql[] = $dat[0];
		
	}
	return $sql;
}



/*
 * Função que monta o SQL para carregar os Beneficiários do Relatorio da Subação
 * @return string - SQL dos Beneficiários
 */
function montarSQLBeneficiariosRelatorioCompleto($strSubacoesFilhos){
	if($strSubacoesFilhos){
		$strSubacoesFilhos = ",".$strSubacoesFilhos;
	}
	$strSubacoesFilhos = $strSubacoesFilhos;

	$sql = '
	        SELECT 
	        	CASE WHEN sb.sbaid = ' . $_REQUEST['sbaid']   . ' THEN \'Original\' ELSE \'Aditivo\' || sba.sbaseqaditivo  END as tipo,
	            be.bendsc,
	            sb.vlrurbano as vlrurbano,
	            sb.vlrrural  as vlrrural,
	            sb.vlrurbano + sb.vlrrural as sabvlrtotal
	        FROM cte.subacaobeneficiario sb
	        	INNER JOIN cte.beneficiario be ON sb.benid = be.benid
	        	INNER JOIN cte.subacaoindicador sba ON sba.sbaid = sb.sbaid
	        WHERE sb.sbaid  in ( ' . $_REQUEST['sbaid']  .$strSubacoesFilhos.')
			AND sb.sabano   = ' . $_REQUEST['sabano'] . 'ORDER BY sb.sbaid';
	return $sql;
	
}
?>
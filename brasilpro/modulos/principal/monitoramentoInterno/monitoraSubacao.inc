<?php
if( $_REQUEST['req'] == 'baixar'){
	include_once APPRAIZ."includes/classes/fileSimec.class.inc";
	$file 		= new FilesSimec(NULL, NULL, 'cte');
	$file->getDownloadArquivo($_REQUEST['arqid']);
	echo"<script>location.href = '?". str_replace("&req=baixar&arqid=".$_REQUEST['arqid'],"",$_SERVER['QUERY_STRING'])."';</script>";
	exit();
}

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
$sql = "SELECT DISTINCT
			inu.estuf
		FROM 
			cte.subacaoindicador sa 
		INNER JOIN cte.acaoindicador               ai ON ai.aciid  = sa.aciid 
		INNER JOIN cte.pontuacao                    p ON p.ptoid   = ai.ptoid AND p.ptostatus = 'A'
		INNER JOIN cte.instrumentounidade         inu ON inu.inuid = p.inuid
		WHERE
			sa.sbaid = ".$_REQUEST['sbaid'];
$estuf = $db->pegaUm($sql);
$bloqueioSys = 'f';
$sql = "SELECT 
			madbloqueio,
			madestados
  		FROM 
  			cte.monitoramentoadministracao
  		WHERE
  			madid = 1
  		LIMIT 1";

$dados = $db->pegaLinha($sql);
$bloc 	 = $dados['madbloqueio'];
$blocEst = explode(",",$dados['madestados']);
if( $bloc == 't' && in_array($estuf,$blocEst) && !cte_possuiPerfil(array(CTE_PERFIL_SUPER_USUARIO,CTE_PERFIL_ADMINISTRADOR )) ){
	$bloqueioSys = 't';
}
if( $_POST['req'] == 'anexar' &&  $bloqueioSys != 't' ){
	$campos	= array("mosid"	        => $_POST['mosid'.$_POST['num']],
					"mnxdataanexo"	=>"'".date('Y-m-d')."'",
					"mnxstatus"     =>"'A'");

	$file = new FilesSimec("monitoramentoanexos", $campos,"cte");
	
	if(is_file($_FILES["arquivo".$_POST['num']]["tmp_name"])){	
//		$arquivoSalvo = $file->setUpload("arquivo brasilpro");
		$arquivoSalvo = $file->setUpload("arquivo brasilpro","arquivo".$_POST['num']);
		if($arquivoSalvo){
			echo '<script type="text/javascript"> alert(" Arquivo anexado.");</script>';
			$alteraanexo[$_POST['num']] = 'S';
		}
	}
}

if( $_POST['req'] == 'excluiranexo'  &&  $bloqueioSys != 't' ){
	
	global $db;
	
	$sql = "UPDATE cte.monitoramentoanexos
			SET mnxstatus = 'I'
			WHERE 
				arqid = ".$_REQUEST['arqid'].";";
	
	$db->executar($sql);
	
	$db->commit();
	
	echo "<script>alert('Arquivo excluido.');</script>";
	$alteraanexo[$_POST['num']] = 'S';
//	die();
}
if( cte_possuiPerfil( Array(CTE_PERFIL_ADMINISTRADOR, CTE_PERFIL_SUPER_USUARIO) ) ){
	if( $_REQUEST['mosalterado'] == 't' && $_SESSION['alt'] == '1' ){
		$_SESSION['alt'] = '0';
		$sql = "UPDATE 
					cte.monitoramentosubacoes
				SET 
					mosalterado = FALSE
				WHERE 
					sbaid = {$_REQUEST['sbaid']}";
		
		$db->executar($sql);
		$db->commit();
	}
}

if( $_POST['req'] == 'gravarDuvida' ){
	
	$sql = "INSERT INTO 
				cte.monitoramentoduvidas(
            		mosid, 
            		mdupergunta, 
            		mdudatapergunta,
            		mdusituacao)
    		VALUES (
					{$_REQUEST['mosid']},
					'".utf8_decode($_REQUEST['mdupergunta'])."',
					now(),
					'D')";
					
	$db->executar($sql);
	
	$db->commit();
	
	echo "Dúvida gravada.";
	die();
}

function listaParecer( $mosid ){
	
	global $db;
	
	$sql = "SELECT
				mpaid,
				mpaparecer
			FROM
				cte.monitoramentoparecer
			WHERE
				mosid = {$mosid}
				AND mpasituacao = 'P'";
	
	$dados = $db->carregar($sql);
	
	
	
	if( is_array($dados) ){
		echo "<table border=\"0\">
			  	<tr><td colspan=\"2\" style=\"border:0px\"><img align=\"midle\" src=\"/imagens/atencao.png\"> Parecer: <br></td></tr>";
		foreach($dados as $parecer){
			echo "<tr>
					<td style=\"border:0px\">{$parecer['mpaparecer']}</td>
					<td style=\"border:0px\"><img align=\"midle\" src=\"/imagens/valida1.gif\" onclick=\"atendeParecer({$parecer['mpaid']},{$mosid});\" title=\"Atender\"></td>
				  </tr>";
		}
		echo "</table>";	
	}
	
}

function listaHistoricoParecer( $sbaid, $mosano, $csuid ){
	
	global $db;
	
	if( $sbaid && $mosano ){
		
		$csuid = $csuid != 'nulo' ? 'AND csuid = '.$csuid : 'AND csuid is null';
		
		$sql = "SELECT
					fc.facordem,
					mp.mpaid,
					mp.mpaparecer,
					ms.mosid,
					mp.mpasituacao,
					to_char(mp.mpadataresposta, 'DD/MM/YYYY') as mpadataresposta
				FROM
					cte.monitoramentoparecer mp
				INNER JOIN
					cte.monitoramentosubacoes ms ON ms.mosid = mp.mosid
				INNER JOIN
					cte.fasescategoria fc ON fc.facid = ms.facid
				WHERE
					ms.sbaid = {$sbaid}
					{$csuid}
					AND ms.mosano = {$mosano}
				ORDER BY 
					fc.facordem,
					mp.mpaparecer";
					
		$dados = $db->carregar($sql);
		
		echo "<table width=\"100%\" class=\"listagem\"  >";
		$linha = 0;
		
		if( is_array($dados) ){
			echo "<tr style=\"background-color: rgb(220, 220, 220);\">
				  	<td colspan=\"4\" aling=\"center\" style=\"padding: 5px; font-size: 110%; font-size:12;border:0;\">
						<strong>Histórico do parecer. </strong>
					</td>
				  </tr>";
			foreach($dados as $fases){
				
				if( $fases['facordem'] > $linha ){
					echo "
						  <tr style=\"background-color: rgb(230, 230, 230);\">
						  	<td colspan=\"4\" style=\"padding: 5px; border:0px;\">
								<strong>{$fases['facordem']}ª Fase </strong>
							</td>
						  </tr>
						  <tr style=\"background-color: rgb(240, 240, 240);\">
						  	<td width=\"5%\" align=\"center\" style=\"padding: 3px; border:0px;\">
								Ação
							</td>
						  	<td width=\"10%\" style=\"padding: 3px; border:0;\">
								Situação
							</td>
						  	<td width=\"70%\" style=\"padding: 3px; border:0;\">
								Parecer
							</td>
						  	<td width=\"15%\" style=\"padding: 3px; border:0;\">
								Data da resposta
							</td>
						  </tr>
						  ";
					$linha = $fases['facordem'];
					$i=1;
				} 
				
				if( ($i % 2) == 1 ){
					$cor = "rgb(255,255,255)";
				}else{
					$cor = "rgb(245,245,245)";
				}
				$i++;
				$situacao = '';
				switch($fases['mpasituacao']){
					case 'D':
						$situacao = "Com duvida";
						break;
					case 'P':
						$situacao = "Parecer";
						break;
					case 'A':
						$situacao = "Atendida";
						break;
					case 'V':
						$situacao = "Validada";
						break;
				}
				
				echo "
					  <tr style=\"background-color: {$cor};\">
					  	<td width=\"5%\" align=\"center\" style=\"padding: 3px; border: 0;\">
							<img src=\"/imagens/valida3.gif\" onclick=\"excluirParecer({$fases['mpaid']});\" title=\"Excluir\">
						</td>
					  	<td style=\"padding: 3px; border:0px;\">
							{$situacao}
						</td>
					  	<td style=\"padding: 3px; border:0px;\">
							{$fases['mpaparecer']}
						</td>
					  	<td style=\"padding: 3px; border:0px;\">
							{$fases['mpadataresposta']}
						</td>
					  </tr>
					 ";
			}
		}
		echo "</table>";
		
	}
}

function listaValidaParecer( $sbaid, $mosano, $csuid ){
	
	global $db;
	
	if( $sbaid && $mosano ){
		
		$csuid = $csuid != 'nulo' ? 'AND csuid = '.$csuid : 'AND csuid is null';
		
		$sql = "SELECT
					fc.facordem,
					mp.mpaid,
					mp.mpaparecer,
					ms.mosid,
					to_char(mp.mpadataresposta, 'DD/MM/YYYY') as mpadataresposta
				FROM
					cte.monitoramentoparecer mp
				INNER JOIN
					cte.monitoramentosubacoes ms ON ms.mosid = mp.mosid
				INNER JOIN
					cte.fasescategoria fc ON fc.facid = ms.facid
				WHERE
					ms.sbaid = {$sbaid}
					{$csuid}
					AND ms.mosano = {$mosano}
					AND mp.mpasituacao = 'A'
				ORDER BY 
					fc.facordem,
					mp.mpaparecer";
					
		$dados = $db->carregar($sql);
		
		echo "<table width=\"100%\" class=\"listagem\"  >";
		$linha = 0;
		
		if( is_array($dados) ){
			echo "<tr style=\"background-color: rgb(220, 220, 220);\">
				  	<td colspan=\"3\" aling=\"center\" style=\"padding: 5px; font-size: 110%; font-size:12;border:0;\">
						<strong>Validação do parecer. </strong>
					</td>
				  </tr>";
			foreach($dados as $fases){
				
				if( $fases['facordem'] > $linha ){
					echo "
						  <tr style=\"background-color: rgb(230, 230, 230);\">
						  	<td colspan=\"3\" style=\"padding: 5px; border:0px;\">
								<strong>{$fases['facordem']}ª Fase </strong>
							</td>
						  </tr>
						  <tr style=\"background-color: rgb(240, 240, 240);\">
						  	<td width=\"5%\" align=\"center\" style=\"padding: 3px; border:0px;\">
								Ação
							</td>
						  	<td width=\"80%\" style=\"padding: 3px; border:0;\">
								Parecer
							</td>
						  	<td width=\"15%\" style=\"padding: 3px; border:0;\">
								Data da resposta
							</td>
						  </tr>
						  ";
					$linha = $fases['facordem'];
					$i=1;
				} 
				
				if( ($i % 2) == 1 ){
					$cor = "rgb(255,255,255)";
				}else{
					$cor = "rgb(245,245,245)";
				}
				$i++;
				
				echo "
					  <tr style=\"background-color: {$cor};\">
					  	<td width=\"5%\" align=\"center\" style=\"padding: 3px; border: 0;\">
							<img src=\"/imagens/valida1.gif\" onclick=\"validaParecer({$fases['mpaid']});\" title=\"Validar\">
						</td>
					  	<td style=\"padding: 3px; border:0px;\">
							{$fases['mpaparecer']}
						</td>
					  	<td style=\"padding: 3px; border:0px;\">
							{$fases['mpadataresposta']}
						</td>
					  </tr>
					 ";
			}
		}
		echo "</table>";
		
	}
}

function listaItensComposição( $sbaid, $ano, $csuid, $blocAll ){
	
	global $db, $_REQUEST;
	
	$blocAll = $blocAll == 'N' ? 'disabled="disabled"' : '';
	
	$csuid = $csuid == 'nulo' ? 'is null' : " = ".$csuid;
	
	$sql = "SELECT DISTINCT
				cs.cosid,
				mc.csuid, 
				cs.cosdsc, 
				cs.cosqtd, 
				cs.cosvlruni, 
				cs.cosano,
				mc.mcoqtd,
				mc.mcovlrtot,
				mc.mcovlruni
			FROM 
				cte.composicaosubacao cs
			LEFT JOIN 
				cte.conveniosubacao 	   css ON css.sbaid = cs.sbaid
			LEFT JOIN
				cte.monitoramentocomposicao mc ON  mc.cosid = cs.cosid AND mc.csuid {$csuid}
			WHERE 
				cs.sbaid  = {$sbaid} AND
				cs.cosano = {$ano} AND
				css.csuid {$csuid}
			ORDER BY 
				cosid";
//	ver($sql, $_REQUEST);
	$itens = $db->carregar($sql);
	
	if( $itens ){
		echo "
			<tr style=\"background-color: rgb(235,235,235);\">
				<td align=\"center\" width=\"40%\" style=\"padding:3px; border: 0px\"><strong> Item </strong></td>
				<td align=\"center\" width=\"5%\" style=\"padding:3px; border: 0px\"><strong> Quantidade </strong></td>
				<td align=\"center\" width=\"15%\" style=\"padding:3px; border: 0px\"><strong> Valor Unitário </strong></td>
				<td align=\"center\" width=\"10%\" style=\"padding:3px; border: 0px\"><strong> Quantidade Executada </strong></td>
				<td align=\"center\" width=\"15%\" style=\"padding:3px; border: 0px\"><strong> Valor Unitário Executado </strong></td>
				<td align=\"center\" width=\"15%\" style=\"padding:3px; border: 0px\"><strong> Valor Total Pago</td>
			</tr>
			";
		$i=1;
		$qtdTotal = 0; 
		$vlrTotal = 0; 
		$somamcoqtd	   = 0;
		$somamcovlrtot = 0;
		$somamcovlruni = 0;
		
		foreach( $itens as $iten ){
			
			if( ($i % 2) == 1 ){
				$cor = "rgb(255,255,255)";
			}else{
				$cor = "rgb(245,245,245)";
			}
			
			$qtdTotal = $qtdTotal + $iten['cosqtd']; 
			$Total = $iten['cosqtd'] * $iten['cosvlruni']; 
			$vlrTotal = $Total + $vlrTotal;
			
			$somamcoqtd	   = $somamcoqtd    + $iten['mcoqtd'];
			$somamcovlrtot = $somamcovlrtot + $iten['mcovlrtot'];
			$somamcovlruni = $somamcovlruni + $iten['mcovlruni'];
			$mcoqtd	   	   = $iten['mcoqtd']    ? number_format($iten['mcoqtd'], 0, ',', '.') : '0,00';
			$mcovlrtot 	   = $iten['mcovlrtot'] ? number_format($iten['mcovlrtot'], 2, ',', '.') : '0,00';
			$mcovlruni	   = $iten['mcovlruni'] ? number_format($iten['mcovlruni'], 2, ',', '.') : '0,00';
			
			echo "
				<tr style=\"background-color: {$cor};\">
					<td align=\"left\" width=\"40%\" style=\"padding:3px; border: 0px\"> {$iten['cosdsc']}</td>
					<td align=\"center\" width=\"5%\" style=\"padding:3px; border: 0px\"> ".number_format($iten['cosqtd'],0,",",".")."</td>
					<td align=\"center\" width=\"15%\" style=\"padding:3px; border: 0px\"> R$ ".number_format($iten['cosvlruni'],2,",",".")."</td>
					<td align=\"center\" width=\"10%\" style=\"padding:3px; border: 0px\">
						<input type=\"text\" title=\"\" style=\"width: 18ex; text-align: left;\" id=\"qtdExec{$iten['cosid']}\" 
						class=\"normal\" value=\"{$mcoqtd}\" maxlength=\"11\" size=\"8\" name=\"qtdExec[]\"
						onkeyup=\"this.value=mascaraglobal('[###.]###',this.value);\"
						onblur=\"MouseBlur( this );this.value=mascaraglobal('[###.]###',this.value); calculoQtdExecutada({$iten['cosid']}); somatorio();\"
						onmouseover=\"MouseOver(this);\"
						onfocus=\"MouseClick(this);this.select();\"
						onmouseout=\"MouseOut(this);\" {$blocAll}>
					</td>
					<td align=\"center\" width=\"15%\" style=\"padding:3px; border: 0px\"> 
						R$ <input type=\"text\" title=\"\" style=\"width: 25ex; text-align: left;\" id=\"vlrExec{$iten['cosid']}\" 
							class=\"normal\" value=\"{$mcovlruni}\" maxlength=\"18\" size=\"10\" name=\"vlrExec[]\"
							onkeyup=\"this.value=mascaraglobal('[###.]###,##',this.value);\"
							onblur=\"MouseBlur( this );this.value=mascaraglobal('[###.]###,##',this.value); calculoVlrExecutado({$iten['cosid']}); somatorio();\"
							onmouseover=\"MouseOver(this);\"
							onfocus=\"MouseClick(this);this.select();\"
							onmouseout=\"MouseOut(this);\" {$blocAll}>
					</td>
					<td align=\"center\" width=\"15%\" style=\"padding:3px; border: 0px\"> 
						R$ <input type=\"text\" title=\"\" style=\"width: 25ex; text-align: left;\" id=\"vlrUniExec{$iten['cosid']}\" 
							class=\"normal\" value=\"{$mcovlrtot}\" maxlength=\"18\" size=\"10\" name=\"vlrUniExec[]\"
							onkeyup=\"this.value=mascaraglobal('[###.]###,##',this.value);\"
							onblur=\"MouseBlur( this );this.value=mascaraglobal('[###.]###,##',this.value); calculoVlrUniExecutado({$iten['cosid']}); somatorio();\"
							onmouseover=\"MouseOver(this);\"
							onfocus=\"MouseClick(this);this.select();\"
							onmouseout=\"MouseOut(this);\" {$blocAll}>
					</td>
				</tr>
				";
			$i++;
		}
		echo "
			<tr style=\"background-color: rgb(230,230,230);\">
				<td align=\"right\" width=\"40%\" style=\"padding:3px; border: 0px\"><strong> Total: </strong></td>
				<td align=\"center\" width=\"5%\" style=\"padding:3px; border: 0px\">    ".number_format($qtdTotal,0,',','.')."</td>
				<td align=\"center\" width=\"15%\" style=\"padding:3px; border: 0px\"> R$ ".number_format($vlrTotal,2,',','.')."</td>
				<td align=\"center\" width=\"10%\" style=\"padding:3px; border: 0px\" id=\"tdsomamcoqtd\">       ".number_format($somamcoqtd,0,',','.')."</td>
				<td align=\"center\" width=\"15%\" style=\"padding:3px; border: 0px\" id=\"tdsomamcovlruni\"> R$ ".number_format($somamcovlruni,0,',','.')."</td>
				<td align=\"center\" width=\"15%\" style=\"padding:3px; border: 0px\" id=\"tdsomamcovlrtot\"> R$ ".number_format($somamcovlrtot,2,',','.')."</td>
			</tr>
			";
		
	}else{
		
		echo "
				<tr>
					<td style=\"color:red; background-color: rgb(230,230,230);\" > Esta subação não possui itens de composição.</td>
				</tr>
				";
	}
}

function escreveDuvidas( $request, $possuiConvenio ){
	
	global $db;
	
	$request['csuid'] = $request['csuid'] == "nulo" ? "" : "AND mns.csuid = {$request['csuid']}";
	
	$sql = "SELECT DISTINCT
				fc.facordem as facordem,
				mns.mosid as mosid
			FROM cte.fasescategoria fc
			INNER JOIN cte.categoriasubacao cs ON cs.catid = fc.catid
			LEFT  JOIN (SELECT 
							ms.mosid,
							ms.facid,
							ms.sbaid,
							ms.csuid,
							ms.mosano
						FROM 
							cte.monitoramentosubacoes ms
						LEFT JOIN 
							cte.monitoramentoanexos ma ON ms.mosid = ma .mosid AND mnxstatus = 'mnxstatus' ) as mns ON mns.facid = fc.facid AND 
																						   mns.sbaid = cs.sbaid AND
																						   mns.mosano = {$request['ano']} 
																						   {$request['csuid']}
	 		WHERE 
	 			cs.sbaid = {$request['sbaid']} 
	 			{$possuiConvenio}
			ORDER BY fc.facordem";
	
	$dados = $db->carregar($sql);
	if( !$dados ){
		
		$sql = "SELECT DISTINCT
					fc.facordem as facordem,
					mns.mosid as mosid
				FROM cte.fasescategoria fc
				INNER JOIN cte.categoriasubacao cs ON cs.catid = fc.catid
				LEFT  JOIN (SELECT 
								ms.mosid,
								ms.facid,
								ms.sbaid,
								ms.csuid,
								ms.mosano
							FROM 
								cte.monitoramentosubacoes ms
							LEFT JOIN 
								cte.monitoramentoanexos ma ON ms.mosid = ma .mosid AND mnxstatus = 'A') as mns ON mns.facid = fc.facid AND 
																							  mns.sbaid = cs.sbaid AND
																						   	  mns.csuid is null    AND
																						      mns.mosano = {$request['ano']}
		 		WHERE 
		 			cs.sbaid = {$request['sbaid']} 
				ORDER BY fc.facordem";
		
		$dados = $db->carregar($sql);
	}
	
	
	echo "<table width=\"100%\"cellspacing=\"1\" cellpadding=\"3\" border=\"0\" 
			align=\"center\" style=\"margin-bottom: 0px;\" >";
	
	if($dados[0]['mosid']){
		foreach($dados as $fases){
			
			$sql = "SELECT
						mduid,
						mdupergunta,
						mdudatapergunta
					FROM
						cte.monitoramentoduvidas
					WHERE
						mosid 	    = {$fases['mosid']} AND
						mdusituacao = 'D'";
			
			$duvidas = $db->carregar($sql);
			
			if(is_array($duvidas)){
	
				echo "  <tr>
							<td colspan=\"4\" style=\"border:0px; background-color:#DCDCDC;\">
								Dúvidas - {$fases['facordem']}ª Fase
							</td>
						</tr>
						<tr>
							<td width=\"5%\"  style=\"border:0px;background-color: rgb(230, 230, 230);\">Ação</td>
							<td width=\"5%\"  style=\"border:0px;background-color: rgb(230, 230, 230);\">ID</td>
							<td width=\"15%\" style=\"border:0px;background-color: rgb(230, 230, 230);\">Data da Dúvida</td>
							<td width=\"75%\" style=\"border:0px;background-color: rgb(230, 230, 230);\">Dúvida</td>
						</tr>";
				
				$counter = 1;
				$cor = '';
				
				foreach($duvidas as $duvida){
					
					if( ( $counter % 2 ) == 1 ){
						$cor = 'white';
					}else{
						$cor = 'rgb(245,245,245)';
					}
					
					if( cte_possuiPerfil( Array(CTE_PERFIL_ADMINISTRADOR, CTE_PERFIL_SUPER_USUARIO) ) ){
						
						$excluir = "<img src=\"/imagens/excluir.gif\" onclick=\"deletaDuvida({$duvida['mduid']},{$_REQUEST['sbaid']}, '{$possuiConvenio}')\">";
						
					}else{

						$excluir = "";
						
					}
					
					echo "  <tr>
								<td style=\"border:0px; background-color: {$cor}\" align=\"center\">
									{$excluir}
								</td>
								<td style=\"border:0px; background-color: {$cor}\" align=\"center\">
									{$counter}
								</td>
								<td style=\"border:0px; background-color: {$cor}\">
									{$duvida['mdudatapergunta']}
								</td>
								<td style=\"border:0px; background-color: {$cor}\"\">
									<br>
									<b>{$duvida['mdupergunta']}</b><br>
									<br>
									<img src=\"/imagens/email_lido.gif\" align=\"middle\"/>
									<a onclick=\"abreResposta({$duvida['mduid']});\">Responder</a>
									<div id=\"resposta{$duvida['mduid']}\" style=\"display:none\">";
									
										echo campo_textarea( 'mduresposta'.$duvida['mduid'], 'S', 'S', '', 100, 5, 1000, '', '', '', '', '', '');
					echo				"<input type=\"button\" value=\"Responder Dúvida\" onclick=\"gravaResposta({$duvida['mduid']},{$_REQUEST['sbaid']}, '{$possuiConvenio}' );\">
									</div>
								</td>
							</tr>";
				
				$counter++;
				}
					
			}
		}
	}else{
		echo "<tr>
				<td style=\"color:red; font: bold;\">
					É necessário salvar o monitoramento para poder cadastrar dúvidas.
				</td>
			  </tr>";

	}
	echo "</table>";
}
	
if( $_POST['req'] == 'deletaDuvida'){
	
	$sql = "DELETE FROM 
				cte.monitoramentoduvidas
		 	WHERE 
		 		mduid = {$_REQUEST['mduid']};";
					
	$db->executar($sql);
	
	$db->commit();
	
	escreveDuvidas( $_REQUEST, $_REQUEST['possuiConvenio'] );
	
	die();
}
	
if( $_POST['req'] == 'gravarResposta'){
	
	$sql = "UPDATE 
				cte.monitoramentoduvidas
		   	SET 	
		   		mduresposta = '".utf8_decode($_REQUEST['mduresposta'])."',
		   		mdudataresposta = now(), 
		   		mdusituacao = 'A'
		 	WHERE 
		 		mduid = {$_REQUEST['mduid']};";
					
	$db->executar($sql);
	
	$db->commit();
	
	escreveDuvidas( $_REQUEST, $_REQUEST['possuiConvenio'] );
	
	die();
}
if( $_REQUEST['req'] == 'parecer'){
	
	global $db;
	$_POST['mosid'] = str_replace(array("\\","'"),"",$_POST['mosid']);
	$arMosid = explode(",",$_POST['mosid']);
	$texto   = str_replace('{#sep!}','',$_POST['parecer']);
//	ver($arMosid,$texto,d);
	foreach ( $arMosid as $mosid ){
		if( $mosid != '' ){
			$sql = "INSERT INTO 
						cte.monitoramentoparecer(
					            mosid, mpaparecer, mpadataparecer, mpasituacao)
					VALUES 
						({$mosid}, '".utf8_decode($texto)."', now(), 'P'); 
					UPDATE cte.monitoramentosubacoes
					SET
						mosfinalizado = FALSE
					WHERE 
						mosid = {$mosid}";
			$db->executar($sql);
		}
	}
	$db->commit();
	echo "Parecer gerado com sucesso.";
	die();
}

if( $_REQUEST['req'] == 'atendeparecer' ){
	
	if( $_REQUEST['mpaid'] ){
		$sql = "UPDATE cte.monitoramentoparecer
			    SET 
			    	mpasituacao = 'A',
			    	mpadataresposta = now()
			 	WHERE 
			 		mpaid = {$_REQUEST['mpaid']}";
		$db->executar($sql);
		$db->commit();
	}
	listaParecer( $_REQUEST['mosid'] );
	
	die();
}

if( $_REQUEST['req'] == 'validaparecer' ){
	
	if( $_REQUEST['mpaid'] ){
		$sql = "UPDATE cte.monitoramentoparecer
			    SET 
			    	mpasituacao = 'V'
			 	WHERE 
			 		mpaid = {$_REQUEST['mpaid']}";
		$db->executar($sql);
		$db->commit();
	}
	listaValidaParecer($_REQUEST['sbaid'], $_REQUEST['ano'], $_REQUEST['csuid'] ); 
	die();
}

if( $_REQUEST['req'] == 'historicoparecer' ){
	
	listaHistoricoParecer($_REQUEST['sbaid'], $_REQUEST['ano'], $_REQUEST['csuid'] ); 
	die();
}

if( $_REQUEST['req'] == 'excluirparecer' ){
	
	if( $_REQUEST['mpaid'] && cte_possuiPerfil(array(CTE_PERFIL_ADMINISTRADOR,CTE_PERFIL_SUPER_USUARIO)) ){
		$sql = "DELETE FROM 
					cte.monitoramentoparecer
			 	WHERE 
			 		mpaid = {$_REQUEST['mpaid']}";
		$db->executar($sql);
		$db->commit();
	}
	listaHistoricoParecer($_REQUEST['sbaid'], $_REQUEST['ano'], $_REQUEST['csuid'] ); 
	die();
}


if( $_REQUEST['req'] == 'finalizar' &&  $bloqueioSys != 't' ){
	
	global $db;
	
//	$_POST['mosid'] = str_replace("\\", "", $_POST['mosid']);
//	$arMosid        = json_decode($_POST['mosid'],true);
//	$mosid          = $arMosid[$_POST['ordem']];

	$sql = "SELECT 
				COUNT(*) as qtdAnexos
			FROM cte.monitoramentoanexos
			WHERE mnxstatus = 'A' AND mosid = ".$_POST['mosid'];
	
	$qtdAnexos = 0;
	
	$qtdAnexos = $db->pegaUm($sql);
	
	if( $qtdAnexos > 0 ){
		$sql = "UPDATE cte.monitoramentosubacoes
				SET
					mosfinalizado = TRUE
				WHERE 
					mosid = {$_POST['mosid']}";
		
		$db->executar($sql);
		$db->commit();
		
		echo "Fase finalizada";
		die();
	}
	
	echo "É necessário pelo menos um anexo para finalizar a fase.";
	die();
	/* Verificar se possui anexo
	 * */
}


if( $_REQUEST['req'] == 'salvar'  &&  $bloqueioSys != 't' ){
	
	
	$_REQUEST['texto'] 			    = str_replace("{}{#sep!}","",$_REQUEST['texto']);
	$_REQUEST['textoant'] 			= str_replace("{}{#sep!}","",$_REQUEST['textoant']);
	$_REQUEST['facid'] 			    = str_replace("{},","",$_REQUEST['facid']);
	$_REQUEST['finalizado']         = str_replace("{},","",$_REQUEST['finalizado']);
	$_REQUEST['armosdatainicio']    = str_replace("{},","",$_REQUEST['armosdatainicio']);
	$_REQUEST['armosdatafinal']     = str_replace("{},","",$_REQUEST['armosdatafinal']);
	$_REQUEST['armosdatainicioant'] = str_replace("{},","",$_REQUEST['armosdatainicioant']);
	$_REQUEST['armosdatafinalant']  = str_replace("{},","",$_REQUEST['armosdatafinalant']);
	$_REQUEST['alteraanexo']        = str_replace("{},","",$_REQUEST['alteraanexo']);
	$_REQUEST['ultimoanexo']        = str_replace("{},","",$_REQUEST['ultimoanexo']);
	$_REQUEST['cosid']  		    = str_replace("{},","",$_REQUEST['cosid']);
	$_REQUEST['qtd']  			    = str_replace("{}{","",$_REQUEST['qtd']);
	$_REQUEST['vlrExecut']  		= str_replace("{}{","",$_REQUEST['vlrExecut']);
	$_REQUEST['vlrExec']  		    = str_replace("{}{","",$_REQUEST['vlrExec']);
	
	//$_csuid = $_REQUEST['csuid'] == 'nulo' ? 'is null' : " = ".$_REQUEST['csuid'];
	$_csuid = ($_REQUEST['csuid'] != 'nulo' && $_REQUEST['csuid'] ) ? " = ".$_REQUEST['csuid'] : 'is null';
	//$csuid = $_REQUEST['csuid'] == 'nulo' ? 'null' : $_REQUEST['csuid'];
	$csuid = ($_REQUEST['csuid'] != 'nulo' && $_REQUEST['csuid'] ) ? $_REQUEST['csuid'] : 'null';
	
	if( $_REQUEST['cosid'] != '{}' ){
		$sql = "SELECT
					cosid
				FROM
					cte.monitoramentocomposicao
				WHERE
					cosid in ({$_REQUEST['cosid']}) AND
					csuid {$_csuid}";
		
		$itensMonitorados = $db->carregarColuna($sql);
	}
	
	$arMosdsc        = explode("{#sep!}",$_REQUEST['texto']);
	$arMosdscAnt     = explode("{#sep!}",$_REQUEST['textoant']);
	$arAnexo         = explode(",",$_REQUEST['arquivo']);
	$arAlteraAnexo   = explode(",",$_REQUEST['alteraanexo']);
	$arUltimoAnexo   = explode(",",$_REQUEST['ultimoanexo']);
	$arFacid         = explode(",",$_REQUEST['facid']);
	$arFinalizado    = explode(",",$_REQUEST['finalizado']);
	$arDataInicio    = explode(",",$_REQUEST['armosdatainicio']);
	$arDataFinal     = explode(",",$_REQUEST['armosdatafinal']);
	$arDataInicioAnt = explode(",",$_REQUEST['armosdatainicioant']);
	$arDataFinalAnt  = explode(",",$_REQUEST['armosdatafinalant']);
	$arcosid     	 = explode(",",$_REQUEST['cosid']);
	$arqtd     		 = explode("{",$_REQUEST['qtd']);
	$arvlrExecut     = explode("{",$_REQUEST['vlrExecut']);
	$arvlrExec       = explode("{",$_REQUEST['vlrExec']);
	
	$i=0;
	
	foreach($arFacid as $facid){
		
		$datainicio = $arDataInicio[$i] == '' ? 'NULL' : '\''.formata_data_sql($arDataInicio[$i]).'\'';
		$datafinal  = $arDataFinal[$i]  == '' ? 'NULL' : '\''.formata_data_sql($arDataFinal[$i]).'\'';
		
		if($facid != ''){
			
			$possuiConvenio = ($_REQUEST['csuid'] != 'nulo' && $_REQUEST['csuid'] ) ? "AND csuid = {$_REQUEST['csuid']}" : '';
			$campoConvenio  = ($_REQUEST['csuid'] != 'nulo' && $_REQUEST['csuid'] ) ? "csuid," : '';
			$dadoConvenio   = ($_REQUEST['csuid'] != 'nulo' && $_REQUEST['csuid'] ) ? "{$_REQUEST['csuid']}," : '';
			$upConvenio     = ($_REQUEST['csuid'] != 'nulo' && $_REQUEST['csuid'] ) ? "csuid = {$_REQUEST['csuid']}," : '';
			
			$sql = "SELECT
						CASE WHEN (count(*)) > 0
							THEN true
							ELSE false
						END as possuiRegistro
					FROM cte.monitoramentosubacoes
					WHERE 
						sbaid  = {$_REQUEST['sbaid']} AND
						facid  = {$facid} AND
						mosano = {$_REQUEST['ano']}
						{$possuiConvenio}";
			
			 
			$testa = $db->pegaUm($sql);
			
			if( $testa == 'f'){
				$sql = "INSERT INTO 
							cte.monitoramentosubacoes ( sbaid, 
														facid, 
														mosdsc, 
														mosdatainicio, 
														mosdatafinal, 
														{$campoConvenio}
														mosano)
						VALUES (
							{$_REQUEST['sbaid']}, 
							{$facid}, 
							'".utf8_decode( substr($arMosdsc[$i],0,299) )."', 
							".$datainicio.", 
							".$datafinal.", 
							{$dadoConvenio}
							{$_REQUEST['ano']})
						RETURNING mosid";
			} elseif ( $testa == 't' ) {
				
				if( $arFinalizado[$i] != 't' ){
					$sql = "UPDATE cte.monitoramentosubacoes
							SET
								mosdsc        = '".utf8_decode(  substr($arMosdsc[$i],0,299) )."',
								mosdatainicio = ".$datainicio.",
								{$upConvenio}
								mosdatafinal  = ".$datafinal."
							WHERE  
								sbaid  = {$_REQUEST['sbaid']} AND
								facid  = {$facid} AND
								mosano = {$_REQUEST['ano']} 
								{$possuiConvenio}
							RETURNING mosid";
				}
				
			}
			$mosid['mosid_'.$i+1] = $db->pegaUm($sql);
			if( $arAlteraAnexo[$i] == 'S' || $datainicio != '\''.$arDataInicioAnt[$i].'\'' || $datafinal != '\''.$arDataFinalAnt[$i].'\'' || $arMosdsc[$i] != $arMosdscAnt[$i] ){
				$sql = "INSERT INTO 
							cte.monitoramentohistorico(
            					mosid, mhidsc, mhidatainicio, 
            					mhidatafinal, mhidscanexo, mhistatus)
    					VALUES ({$mosid['mosid_'.$i+1]}, '".utf8_decode(substr($arMosdsc[$i],0,300))."', {$datainicio}, 
    							{$datafinal}, '{$arUltimoAnexo[$i]}', 'A');
    					UPDATE 
							cte.monitoramentosubacoes
						SET 
							mosalterado = TRUE
						WHERE 
							sbaid = {$_REQUEST['sbaid']};";
    			$db->executar($sql);
    			unset($sql);
			}
			$db->commit();
			$i++;
		}
	}
	 
	if( is_array($itensMonitorados) ){
		foreach( $arcosid as $k => $cosid){
	
			$arqtd[$k] 	   = str_replace(".","",$arqtd[$k]);
			$arqtd[$k] 	   = str_replace(",",".",$arqtd[$k]);
			
			$arvlrExecut[$k] = str_replace(".","",$arvlrExecut[$k]);
			$arvlrExecut[$k] = str_replace(",",".",$arvlrExecut[$k]);
			
			$arvlrExec[$k] = str_replace(".","",$arvlrExec[$k]);
			$arvlrExec[$k] = str_replace(",",".",$arvlrExec[$k]);
			
			if( in_array( $cosid, $itensMonitorados ) ){
	
				$sql = "UPDATE 
							cte.monitoramentocomposicao
						SET 
							mcoqtd    = {$arqtd[$k]} , 
							mcovlruni = {$arvlrExecut[$k]} ,
							mcovlrtot = {$arvlrExec[$k]}
						WHERE 
							cosid = {$cosid} AND
							csuid {$_csuid} ";
				
				$db->executar($sql);
				
			}else{
				
				$sql = "INSERT INTO 
							cte.monitoramentocomposicao(
					            cosid, 
					            csuid, 
					            mcoqtd,
					            mcovlruni, 
					            mcovlrtot)
					    VALUES 
					    	({$cosid}, {$csuid}, {$arqtd[$k]}, {$arvlrExecut[$k]}, {$arvlrExec[$k]});";
				
				$db->executar($sql);
				
			}
			$db->commit();
		}
	}
	
	echo simec_json_encode($mosid);
	die();
}

cte_verificaSessao();

$admTemp = cte_possuiPerfil(array(CTE_PERFIL_ADMINISTRATOR_TEMP)) ? 1 : 2; // **@@&& -- retirar apos reformulação

include_once APPRAIZ."brasilpro/modulos/principal/par_subacao_funcoes.inc";

//DEFINE SE UMA SUB-AÇÃO NOVA
define('CTE_NOVA_SUBACAO', trim($_REQUEST['sbaid']) == '');

//DEFINE QUAL O PERFIL QUE PODE EDITAR (ALTERAÇÃO DE DADOS) DE UMA SUB-AÇÃO
define('CTE_PRM_EDITAR_SUBACAO', cte_podeEditarParecer($_SESSION['inuid']) && $_REQUEST['bloqueado'] !='1');

//DEFINE QUAL O PERFIL QUE PODE EXCLUIR A SUB-AÇÃO
define('CTE_PRM_EXCLUIR_SUBACAO', CTE_PRM_EDITAR_SUBACAO);

//DEFINE QUAL O PERFIL QUE PODE EMITIR PARECER
define('CTE_PRM_EMITIR_PARECER', CTE_PRM_EDITAR_SUBACAO && ($db->testa_superuser() ||
                                                            cte_possuiPerfil(array(CTE_PERFIL_SUPER_USUARIO,
                                                                                   CTE_PERFIL_EQUIPE_TECNICA,
                                                                                   147,
                                                                                   CTE_PERFIL_CONSULTORES, CTE_PERFIL_ADMINISTRATOR_TEMP))));

//DEFINE QUAL O PERFIL QUE VAI SOMENTE VISUALIZAR O PARECER
define('CTE_PRM_VISUALIZAR_PARECER', CTE_PRM_EMITIR_PARECER || cte_possuiPerfil(array(CTE_PERFIL_ALTA_GESTAO, CTE_PERFIL_ADMINISTRATOR_TEMP)));

define('CTE_PRM_VISUALIZAR_PI', CTE_PRM_EDITAR_SUBACAO && cte_possuiPerfil(array(CTE_PERFIL_SUPER_USUARIO,CTE_PERFIL_ADMINISTRADOR, CTE_PERFIL_ADMINISTRATOR_TEMP )));

header('content-type: text/html; charset=iso-8859-1;');


/****************************  	REGRA DE NEGÓCIO DO PAR:  ADITIVOS. ********************************/

$anoAtualTrava 	= 0;
$anoParecer2007 = 0;
$anoParecer2008 = 0;
$anoParecer2009 = 0;
$anoParecer2010 = 0;
$anoParecer2011 = 0;
$anoConvenio 	= 0000;
$valorTotalSub  = 0;

if($db->testa_superuser()){
	$super = 1; // É super usuário
}else{
	$super = 2; 
}

if(cte_possuiPerfil(array(CTE_PERFIL_ADMINISTRADOR))){
	$adm = 1; // É Administrador
}else{
	$adm = 2; 
}
	
function sql2html($coluna) {
    if (strtolower($coluna) == 't')
        return true;

    if (strtolower($coluna) == 'f')
        return false;

    if (strtolower($coluna) == 'null' || $coluna == null)
        return '';

    return $coluna;
}

include APPRAIZ . "includes/registraracesso.php";

//--------- SUBAÇÕES - PROXIMA E ANTERIOR
// | | | | | | | | | | | | | | | | | | | |
// V V V V V V V V V V V V V V V V V V V V

if( $_REQUEST['cmoid'] != "nulo" ){
	$convenio = "AND cs.cmoid = {$_REQUEST['cmoid']}" ;
	$joinConvenio = "LEFT JOIN cte.conveniosubacao       cs ON cs.sbaid = sa.sbaid
					 LEFT JOIN cte.conveniomonitoramento cm ON cm.cmoid = cs.cmoid";
}else{
	$convenio = "AND cs.cmoid is null";
	$joinConvenio = "LEFT JOIN cte.conveniosubacao       cs ON cs.sbaid = sa.sbaid
					 LEFT JOIN cte.conveniomonitoramento cm ON cm.cmoid = cs.cmoid";
}

	$sql = "((SELECT DISTINCT
				sa.sbaid,
				dim.dimcod,
				are.ardcod,
				ind.indcod,
				sa.sbaordem,
				sa.sbadsc
			FROM 
				cte.subacaoindicador sa 
			INNER JOIN cte.acaoindicador               ai ON ai.aciid  = sa.aciid 
			INNER JOIN cte.pontuacao                    p ON p.ptoid   = ai.ptoid AND p.ptostatus = 'A'
			INNER JOIN cte.indicador                  ind ON ind.indid = p.indid
			INNER JOIN cte.areadimensao               are ON are.ardid = ind.ardid
			INNER JOIN cte.dimensao                   dim ON dim.dimid = are.dimid
			INNER JOIN cte.instrumentounidade         inu ON inu.inuid = p.inuid
			INNER JOIN cte.subacaoparecertecnico      spt ON spt.sbaid = sa.sbaid AND cast(spt.sptano AS varchar) = '{$_REQUEST['ano']}' AND spt.ssuid = 7
			LEFT  JOIN cte.statussubacao               ss ON ss.ssuid  = spt.ssuid
			LEFT  JOIN seguranca.usuario              usu ON sa.usucpf = usu.usucpf
			LEFT  JOIN cte.formaexecucao              fex ON fex.frmid = sa.frmid 
			{$joinConvenio}
			WHERE 
				    sa.frmid  = 17
				AND inu.estuf = '{$_REQUEST['estuf']}'
				AND sa.sbaidpai is null
				AND inu.itrid = 3 
			{$convenio}
			ORDER BY dim.dimcod,are.ardcod,ind.indcod,sa.sbaordem,sa.sbadsc )
			    
			UNION ALL 
			
			(SELECT DISTINCT
				sa.sbaid, 
				dim.dimcod,
				are.ardcod,
				ind.indcod,
				sa.sbaordem, 
				sa.sbadsc
			FROM 
				cte.subacaoindicador sa 
			INNER JOIN cte.acaoindicador               ai ON ai.aciid  = sa.aciid 
			INNER JOIN cte.pontuacao                    p ON p.ptoid   = ai.ptoid AND p.ptostatus = 'A'
			INNER JOIN cte.indicador                  ind ON ind.indid = p.indid
			INNER JOIN cte.areadimensao               are ON are.ardid = ind.ardid
			INNER JOIN cte.dimensao                   dim ON dim.dimid = are.dimid
			INNER JOIN cte.instrumentounidade         inu ON inu.inuid = p.inuid
			INNER JOIN cte.subacaoparecertecnico      spt ON spt.sbaid = sa.sbaid AND  
									 spt.ssuid = 3 AND 
									 spt.sptano = {$_REQUEST['ano']}
			LEFT  JOIN cte.statussubacao               ss ON ss.ssuid  = spt.ssuid
			LEFT  JOIN seguranca.usuario              usu ON sa.usucpf = usu.usucpf
			LEFT  JOIN cte.formaexecucao              fex ON fex.frmid = sa.frmid
			{$joinConvenio}
		 	WHERE
			 	    sa.frmid  = 15 
				AND inu.estuf = '{$_REQUEST['estuf']}'
				AND sa.sbaidpai is null
				AND inu.itrid = 3
				{$convenio}
			ORDER BY dim.dimcod,are.ardcod,ind.indcod,sa.sbaordem, sa.sbadsc)
			    
			UNION ALL
			
			(SELECT DISTINCT
				sa.sbaid,
				dim.dimcod,
				are.ardcod,
				ind.indcod,
				sa.sbaordem, 
				sa.sbadsc
			FROM 
				cte.subacaoindicador sa 
			INNER JOIN cte.acaoindicador               ai ON ai.aciid  = sa.aciid 
			INNER JOIN cte.pontuacao                    p ON p.ptoid   = ai.ptoid AND p.ptostatus = 'A'
			INNER JOIN cte.indicador                  ind ON ind.indid = p.indid
			INNER JOIN cte.areadimensao               are ON are.ardid = ind.ardid
			INNER JOIN cte.dimensao                   dim ON dim.dimid = are.dimid
			INNER JOIN cte.instrumentounidade         inu ON inu.inuid = p.inuid
			INNER JOIN cte.subacaoparecertecnico      spt ON spt.sbaid = sa.sbaid AND  
									 spt.ssuid = 3 AND 
									 spt.sptano = {$_REQUEST['ano']}
			LEFT  JOIN cte.statussubacao               ss ON ss.ssuid  = spt.ssuid
			LEFT  JOIN seguranca.usuario              usu ON sa.usucpf = usu.usucpf
			LEFT  JOIN cte.formaexecucao              fex ON fex.frmid = sa.frmid
			{$joinConvenio}
			WHERE 
				    sa.frmid  = 13 
				AND inu.estuf = '{$_REQUEST['estuf']}'
				AND sa.sbaidpai is null
				AND inu.itrid = 3
				{$convenio}
			ORDER BY dim.dimcod,are.ardcod,ind.indcod,sa.sbaordem, sa.sbadsc))";
//dbg($sql,1);
$subAcoes        = (array) $db->carregar($sql);
$sbaidAnterior   = false;
$sbaidPosterior  = false;

while (list($k, $subacao) = each($subAcoes)) {
    if ($subacao["sbaid"] == (integer) $_REQUEST['sbaid']) {

        if (array_key_exists($k - 1, $subAcoes)) {
            $sbaidAnterior = $subAcoes[$k - 1]["sbaid"];
            if( $_REQUEST['cmoid'] != "nulo" ){
	            $sql="SELECT
						csuid
					  FROM
					  	cte.conveniosubacao
					  WHERE
					  	sbaid = {$sbaidAnterior} AND
					  	cmoid = {$_REQUEST['cmoid']}";
						
				$csuidAnterior = $db->pegaUm($sql);
            }
        }

        if (array_key_exists($k + 1, $subAcoes)) {
            $sbaidPosterior  = $subAcoes[$k + 1]["sbaid"];
            if( $_REQUEST['cmoid'] != "nulo" ){
	            $sql="SELECT
						csuid
					  FROM
					  	cte.conveniosubacao
					  WHERE
					  	sbaid = {$sbaidPosterior} AND
					  	cmoid = {$_REQUEST['cmoid']}";
						
				$csuidPosterior = $db->pegaUm($sql);
            }
        }

        break;
    }
}

// A A A A A A A A A A A A A A A A A A A A
// | | | | | | | | | | | | | | | | | | | |
//--------- SUBAÇÕES - PROXIMA E ANTERIOR



//------  SUBAÇÕES - CARGA DE DADOS
$sql = 'SELECT
		    s.sbaid,
		    s.aciid,
		    s.undid,
		    s.frmid,
		    s.sbadsc,
		    s.sbastgmpl,
		    s.sbaprm,
		    s.sbapcr,
		    s.sbadata,
		    s.usucpf,
		    s.sbaparecer,
		    s.psuid,
		    s.ssuid,
		    s.foaid,
		    s.prgid,
		    s.sbaporescola,
		    s.ppsid,
		    s.sbaordem,
		    s.sbaobjetivo,
		    s.sbatexto,
		    s.sbacategoria
		FROM
		    cte.subacaoindicador s
		LEFT JOIN
		    cte.programa p ON p.prgid = s.prgid
		WHERE
		    s.sbaid = ' . (integer) $_REQUEST['sbaid'];

$sai = array_map('sql2html', (array) $db->pegaLinha($sql));
extract($sai);

$sql = 'SELECT
		    dim.dimid,
		    dim.dimcod,
		    dim.dimdsc,
		    are.ardid,
		    are.ardcod,
		    are.arddsc,
		    ind.indid,
		    ind.indcod,
		    ind.inddsc,
		    aci.acidsc,
		    aci.acilocalizador,
		    pto.ptoid
		FROM
		    cte.acaoindicador aci
		INNER JOIN
		    cte.pontuacao pto ON pto.ptoid = aci.ptoid and pto.ptostatus = \'A\'
		INNER JOIN
		    cte.indicador ind ON ind.indid = pto.indid
		INNER JOIN
		    cte.areadimensao are ON are.ardid = ind.ardid
		INNER JOIN
		    cte.dimensao dim ON dim.dimid = are.dimid
		WHERE
		    aci.aciid = ' . (trim($aciid) != '' ? $aciid : (integer) $_REQUEST['aciid']);

$aci = (array) $db->pegaLinha($sql);
extract($aci);

$_SESSION['indid'] = $indid;
$aciid             = trim($aciid) != '' ? $aciid : (integer) $_REQUEST['aciid'];

?>
<html>
<head>
	<meta http-equiv="Cache-Control" content="no-cache">
	<meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="-1">
    <title>PAR - Brasil Profissionalizado - Monitoramento Subação </title>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    <link rel="stylesheet" type="text/css" href="../brasilpro/includes/par_subacao.css"/>
    <link rel="stylesheet" type="text/css" href="/includes/ModalDialogBox/modal-message.css"  media="screen" />
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

    <script src="../includes/calendario.js"></script>
    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/prototype.js"></script>
    <script type="text/javascript" src="/includes/ModalDialogBox/modal-message.js"></script>
	<script type="text/javascript" src="/includes/ModalDialogBox/ajax-dynamic-content.js"></script>
	<script type="text/javascript" src="/includes/ModalDialogBox/ajax.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <script type="text/javascript">
	    function getElementText(tag, parentElem, index){
	        if (!index) {
	            index=0;
	        }
	
	        var result = parentElem.responseXML.getElementsByTagName(tag)[index];
	
	        if (result) {
	            if (result.childNodes.length == 0) {
	                return '';
	            } else if (result.childNodes.length > 1) {
	                return result.childNodes[1].nodeValue;
	            } else {
	                return result.firstChild.nodeValue;
	            }
	        } else {
	            return '';
	        }
	    }
    </script>
</head>
<body>
	<div id="loader-container">
		<div id="loader">
			<img src="../imagens/wait.gif" border="0" align="middle">
			<span>Aguarde! Carregando Dados...</span>
		</div>
	</div>
    <div id="container">
<?php 
cte_montaTitulo( $titulo_modulo, '' ); 
?>
	<!-- LANÇAMENTO DE DADOS  -->
<form method="post" id="frmParSubacao" enctype="multipart/form-data" action="">
	<input type="hidden" name="sbaid"   id="sbaid" value="<?php echo $sbaid ?>"/>
	<input type="hidden" name="csuid"   id="csuid" value="<?php echo $_REQUEST['csuid'] ?>"/>
	<input type="hidden" name="req"     id="req"   value="">
	<input type="hidden" name="arqid"   id="arqid" value="">
	<input type="hidden" name="num"     id="num" >

	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
		<colgroup>
			<col width="25%" />
			<col width="75%" />
		</colgroup>
		<tbody>
			<?php if( $bloc == 't' && in_array($estuf,$blocEst) ){?>
			<tr id="buttonsContainer">
				<td style="text-align : center;" colspan="2">
					<label style="color:red;">
						<img src="/imagens/cadiado.png" align="middle"/><br /> Sistema Bloqueado.<br />
					</label>
	            </td>
			</tr>
			<?php }?>
			<tr id="buttonsContainer">
				<td style="text-align : left;">
					<input onclick="subAnterior();" type="button" name="btnAnterior" value="Anterior" id="btnAnterior" <?php echo !$sbaidAnterior ? 'disabled="disabled"' : ''; ?>/>
	            </td>
	            <td style="text-align : right;">
	            	<input onclick="subPosterior();" type="button" name="btnProximo" value="Próxima" id="btnProximo" <?php echo !$sbaidPosterior ? 'disabled="disabled"' : ''; ?>/>
	            </td>
			</tr>
            <tr>
				<td class="SubTituloDireita">Dimensão:</td>
				<td><?php echo $aci['dimcod'] , '. ' , $aci['dimdsc'] ?></td>
            </tr>
            <tr>
				<td class="SubTituloDireita">Área:</td>
				<td><?php echo $aci['ardcod'] , '. ' , $aci['arddsc']; ?></td>
            </tr>
            <tr>
				<td class="SubTituloDireita">Indicador:</td>
				<td><?php echo $aci['indcod']   , '. ' , $aci['inddsc']; ?></td>
            </tr>
            <tr>
              <td class="SubTituloDireita">Demanda:</td>
              <td><?php echo $aci['acilocalizador'] == "E" ? "Rede Estadual" : "Rede Municipal"; ?></td>
            </tr>
            <tr>
              <td class="SubTituloDireita">Ação:</td>
              <td><?php echo $aci['acidsc']; ?></td>
            </tr>

<?php
// CTE_PRM_EDITAR_SUBACAO
	$select_formaAtendimento = $db->pegaUm('SELECT foadsc FROM cte.formaatendimento WHERE foaid = ' . (integer) $foaid);
    $select_programa         = $db->pegaUm('SELECT prgdsc FROM cte.programa WHERE prgid = '         . (integer) $prgid);
    $select_unidadeMedida    = $db->pegaUm('SELECT unddsc FROM cte.unidademedida WHERE undid = '    . (integer) $undid);
    $select_formaExecucao    = $db->pegaUm('SELECT frmdsc FROM cte.formaexecucao WHERE frmid = '    . (integer) $frmid);
    $_REQUEST['csuid']		 = $_REQUEST['csuid'] ? $_REQUEST['csuid'] : 'nulo';
	$possuiConvenio 		 = $_REQUEST['csuid'] != 'nulo' ? "AND mns.csuid = {$_REQUEST['csuid']}" : "AND mns.csuid is null";
	$csuid					 = ($_REQUEST['csuid'] == 'nulo' || !$_REQUEST['csuid'] ) ? "" : " AND mns.csuid = {$_REQUEST['csuid']}";

	$sql = "SELECT DISTINCT
				(SELECT
					count(*) as qtdfases
				FROM cte.fasescategoria
				WHERE catid = fc.catid) as qtdfases,
				cs.catid as catid,
				cs.sbaid as sbaid,
				fc.facid as facid,
				fc.facordem as facordem,
				fc.facdsc as facdsc,
				fc.facobservacao as observacao,
				mns.mosdsc as mosdsc,
				mns.mosid as mosid,
				mns.mosfinalizado as mosfinalizado,
				mns.mosdatainicio as mosdatainicio,
				mns.mosdatafinal as mosdatafinal,
				mns.mosano as mosano,
				mns.csuid,
				mns.possuiresp
			FROM cte.fasescategoria fc
			INNER JOIN cte.categoriasubacao cs ON cs.catid = fc.catid
			LEFT  JOIN (SELECT 
							ms.mosdsc,
							ms.mosid,
							ms.facid,
							ms.mosfinalizado,
							ms.sbaid,
							ms.mosdatainicio,
							ms.mosdatafinal,
							ms.mosano,
							ms.csuid,	
							md.possuiresp
						FROM 
							cte.monitoramentosubacoes ms
						LEFT JOIN
							(SELECT
								count(mduid) as possuiresp,
								mosid
							 FROM
							 	cte.monitoramentoduvidas
							 WHERE
							 	mdusituacao is not null
							 GROUP BY
							 	mosid ) as md ON md.mosid = ms.mosid
						LEFT JOIN 
							cte.monitoramentoanexos ma ON ms.mosid = ma .mosid AND mnxstatus = 'A' ) as mns ON mns.facid = fc.facid AND 
																						   mns.sbaid = cs.sbaid 
																						   {$csuid}  AND
																						   mns.mosano = {$_REQUEST['ano']}
																						  
	 		WHERE 
	 			cs.sbaid = {$_REQUEST['sbaid']}
	 			
	 			{$possuiConvenio}
			ORDER BY fc.facordem";
//	 ver($sql);			
	$dados = $db->carregar($sql);
	if( !$dados ){
		
		$sql = "SELECT DISTINCT
					(SELECT
						count(*) as qtdfases
					FROM cte.fasescategoria
					WHERE catid = fc.catid) as qtdfases,
					cs.catid as catid,
					cs.sbaid as sbaid,
					fc.facid as facid,
					fc.facordem as facordem,
					fc.facdsc as facdsc,
					fc.facobservacao as observacao,
					mns.mosdsc as mosdsc,
					mns.mosid as mosid,
					mns.mosfinalizado as mosfinalizado,
					mns.mosdatainicio as mosdatainicio,
					mns.mosdatafinal as mosdatafinal,
					mns.mosano as mosano,
					mns.csuid,
					mns.possuiresp
				FROM cte.fasescategoria fc
				INNER JOIN cte.categoriasubacao cs ON cs.catid = fc.catid
				LEFT  JOIN (SELECT 
								ms.mosdsc,
								ms.mosid,
								ms.facid,
								ms.mosfinalizado,
								ms.sbaid,
								ms.mosdatainicio,
								ms.mosdatafinal,
								ms.mosano,
								ms.csuid,
								md.possuiresp	
							FROM 
								cte.monitoramentosubacoes ms
							LEFT JOIN
								(SELECT
									count(mduid) as possuiresp,
									mosid
								 FROM
								 	cte.monitoramentoduvidas
								 WHERE
								 	mdusituacao is not null 
								 GROUP BY
								 	mosid) as md ON md.mosid = ms.mosid
							LEFT JOIN 
								cte.monitoramentoanexos ma ON ms.mosid = ma .mosid AND mnxstatus = 'A') as mns ON mns.facid = fc.facid AND 
																							  mns.sbaid = cs.sbaid AND
																						   	  mns.csuid is null AND  mns.mosano = {$_REQUEST['ano']}
		 		WHERE 
		 			cs.sbaid = {$_REQUEST['sbaid']} 
		 			 --AND mosid is not null
				ORDER BY fc.facordem";
//		ver($sql);
		$dados = $db->carregar($sql);
	}
	
	$alertTexto = "";
	
	$testaperfil = cte_possuiPerfil(CTE_PERFIL_SUPER_USUARIO,CTE_PERFIL_ADMINISTRADOR);
	
//	if($dados[0]['mosdatainicio']){
//		$somenteLeitura = "N";
//	}else{
//		$somenteLeitura = "S";
//	}

?>
            <tr style="background-color: #cccccc;">
				<td align="left" colspan="<?php echo sizeof($_cosano) + 2; ?>">
					<strong>Dados da Subação</strong>
				</td>
            </tr>
            <tr>
              <td align='right' class="SubTituloDireita">Forma de atendimento:</td>
              <td><?php echo $select_formaAtendimento; ?></td>
            </tr>
            <tr>
              <td align='right' class="SubTituloDireita">Descrição da Subação:</td>
              <td><?php echo $sbadsc; ?></td>
            </tr>
            <tr>
              <td align='right' class="SubTituloDireita">Estratégia de Implementação:</td>
              <td><?php echo $sbastgmpl; ?></td>
            </tr>
            <tr>
              <td align='right' class="SubTituloDireita">Programa:</td>
              <td><?php echo $select_programa; ?></td>
            </tr>
            <tr>
              <td align="right" class="SubTituloDireita">Unidade de Medida:</td>
              <td><?php echo $select_unidadeMedida; ?></td>
            </tr>
            <tr>
              <td align="right" class="SubTituloDireita">Forma de Execução:</td>
              <td><?php echo $select_formaExecucao ?></td>
            </tr>
            <tr>
              <td align='right' class="SubTituloDireita">Instituição Parceira (se houver):</td>
              <td><?php echo $sbapcr; ?></td>
            </tr>
            <tr>
              <td align='right' class="SubTituloDireita">Cronograma:</td>
              <td><?php echo !$sbaporescola ? 'Global' : 'Por Escola'; ?></td>
            </tr>
		</tbody>
	</table>
	<table align="center" border="0" class="tabela listagem" cellpadding="0" cellspacing="0" id="tabAbasSelecaoCosano">
		<tbody>
			<tr style="background-color: rgb(210,210,220)">
				<td align="left" colspan="4" style="padding: 5px; font-size: 110%">
					<strong>Monitoramento - <?=$_REQUEST['ano']; ?></strong>
				<div id="div"></div>
				</td>
			</tr>
			<tr style="background-color: rgb(230,230,230)">
				<td colspan="4">
					<table>
						<tr>
							<td align="right" width="25%" style="border:0px">
								<strong>Data de Início da Execução: </strong>
							</td>
							<td align="left" width="20%" style="border:0px">
								<?php $mosdatainicio = $dados[0]['mosdatainicio']; ?>
								<?=campo_data2( 'mosdatainicio', 'N', 'N', '', 'S', '', 'checaData();' ); ?>
							</td>
							<td align="right" width="25%" style="border:0px">
								<strong>Data de Término da Execução: </strong>
							</td>
							<td align="left" width="30%" style="border:0px">
								<?php $mosdatafinal = $dados[$dados[0]['qtdfases']-1]['mosdatafinal']; ?>
								<?=campo_data2( 'mosdatafinal', 'N', 'N', '', 'S', '', 'checaData();' ); ?>
							</td>
						</tr>
					</table>
				</td>
			</tr>
<?php 

if( !$dados ){
?>
<script>
	alert('Esta subação não possui categoria.');
	window.close();
</script>
<?php 
}else{
	
	$i = 0;
	
	foreach($dados as $fases){
		
		$ultimo = count($dados);
		
		if( ($i % 2) == 1 ){
			$cor = "rgb(245,245,245)";
		}else{
			$cor = "rgb(255,255,255)";
		}

		$bloc = 'S';
		$disabled = ''; 
		if ( $fases['mosfinalizado'] == 't' || $bloqueioSys == 't'){
			$bloc = 'N'; 
			$disabled = 'disabled="disabled"'; 
			$blocAll = ( $ultimo == $fases['facordem'] ) ? 'N' : 'S'; 
		}
		
		if ( $testaperfil ){
			$disabledData = ''; 
		}else{
			if ( $fases['mosfinalizado'] == 't' ){
				$disabledData = 'disabled="disabled"'; 
			}
		}
?>
			<tr style="background-color: <?=$cor ?>;">
				<td>
					<input type="hidden" name="mosid<?=$fases['facordem']?>" id="mosid<?=$fases['facordem']?>" value="<?=$fases['mosid']?>">
					<label title="<?=$fases['observacao'] ?>"><b><?=$fases['facordem']?>ª Fase:</b></label> <?=$fases['facdsc']?><br>
					<input type="hidden" id="facid<?=$fases['facordem']?>" value="<?=$fases['facid']?>"></input>
					<table>
						<tr>
							<td style="border:0px">Data de Inicio:</td>
							<td style="border:0px">
								<?php $mosdatainicio  = 'mosdatainicio_'.$fases['facordem']; ?>
								<?php $$mosdatainicio = $fases['mosdatainicio']; ?>
								<?=campo_data2( $mosdatainicio , 'S', $bloc, '', 'S', '', 'atualizaDatas();checaData();','', 'atualizaDatas();checaData();' ); ?>
								<input type="hidden" id="mosdatainicioant_<?=$fases['facordem'] ?>" name="mosdatainicioant_<?=$fases['facordem'] ?>" value="<?=$fases['mosdatainicio'] ?>">
							</td>
						</tr>
						<tr>
							<td style="border:0px">Data de Término:</td>
							<td style="border:0px">
								<?php $mosdatafinal  = 'mosdatafinal_'.$fases['facordem']; ?>
								<?php $$mosdatafinal = $fases['mosdatafinal']; ?>
								<?=campo_data2( $mosdatafinal , 'S', $bloc, '', 'S', '', 'atualizaDatas();checaData();','', 'atualizaDatas();checaData();' ); ?>
								<input type="hidden" id="mosdatafinalant_<?=$fases['facordem'] ?>" name="mosdatafinalant_<?=$fases['facordem'] ?>" value="<?=$fases['mosdatafinal'] ?>">
							</td>
						</tr>
					</table>
					<div id="divParecer_<?=$fases['mosid'] ?>">
<?php 
					if( $fases['mosid'] ){
						listaParecer( $fases['mosid'] );
					}
?>
					</div>
					<table>
						<?php
							if($fases['mosid']){ 
						?>
						<tr style="display: <?=($fases['mosfinalizado'] == 't') ? 'none' : 'table-row' ?>">
							<td style="border:0px">
								<a onclick="abreDuvida( <?=$fases['facordem']; ?>);">
									<img src="/imagens/email.gif" align="middle"/></br> Registre sua dúvida:</br>
								</a>
							</td>
						</tr>
						<tr>
							<td style="border:0px">
								<div id="divDuvida_<?=$fases['facordem']; ?>" style="display:none;">
									<?=campo_textarea( 'texto'.$fases['facordem'].'1', 'S', 'S', '', 50, 5, '', 'onkeyup="contaCaracteres('.$fases['facordem'].'1,300);"', $acao = 0, '$txtdica = ', $tab = true, $title = NULL, '');?>
									<?=campo_texto( 'resto'.$fases['facordem'].'1', '', 'N', '', 3, 3, '', '', 'left', '', 0, 'id="resto'.$fases['facordem'].'1"','',300 ); ?>
									<label> caracteres restantes.</label>
									<input type="button" value="Registrar Duvida" onclick="gravaDuvida( <?=$fases['facordem']; ?>,<?=$fases['mosid'] != '' ? $fases['mosid'] : 'NULL'; ?> );"></input>
								</div>
								<?php 
									if( $fases['possuiresp'] > 0 ){
								?>
								<a onclick="abrePopUpResposta( <?=$fases['mosid'] ?> );">
									<img src="/imagens/email_lido.gif" align="middle"/></br> Dúvidas registradas:
								</a>
								<?php 
									}
								?>
								<div id="divhistorico" onclick="abrePopUpHistorico(<?=$fases['mosid'] ?>)"><img align="midle" src="/imagens/eixos-mini2.png"> <a>Histórico do monitoramento.</a></div>
							</td>
						</tr>
						<?php
							}else{
						?>
						<tr>
							<td style="color:red; font: bold;">
								É necessário salvar o monitoramento para poder cadastrar duvidas.
							</td>
						</tr>
						<?php 
								
							} 
						?>
					</table>
				</td>
				<td>
					<label><br><b>&nbsp;Descrição das atividades do período:</b></br></label>
					<?=campo_textarea( 'texto'.$fases['facordem'], 'S', $bloc, '', 50, 5, '', 'onkeyup="contaCaracteres('.$fases['facordem'].',300);"', $acao = 0, '$txtdica = ', $tab = true, $title = NULL, $fases['mosdsc']);?>
					<?=campo_texto( 'resto'.$fases['facordem'], '', 'N', '', 3, 3, '', '', 'left', '', 0, 'id="resto'.$fases['facordem'].'"','',300 ); ?>
					<label> caracteres restantes.</label>
					<input type="hidden" id="textoant_<?=$fases['facordem'] ?>" name="textoant_<?=$fases['facordem'] ?>" value="<?=$fases['mosdsc'] ?>">
				</td>
				<td align="left" width="230px">
<?php 
		$block = "";
		if( $fases['mosid'] == '' ){
			$block = "disabled=\"disabled\"";
?>
					<!--<br>
						<strong style="color: red ">
							É necessário salvar todas as datas antes de anexar um arquivo.
						</strong>
					</br>-->
<?php 
		}
?>
					<br>
					&nbsp;&nbsp;<input type="file" name="arquivo<?=$fases['facordem'];?>" <?=$disabled ?> <?=$block ?>/>
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/><br>
					&nbsp;&nbsp;<input type="button" id="btnAnexar"     name="btnAnexar"     value="Anexar" onclick="anexarArquivo('<?=$fases['facordem'] ?>');" <?=$disabled ?> <?=$block ?>/><br><br>
					<div style="overflow: scroll; height: 160px;">
						<?php 
						
							$mosid = $fases['mosid'] == '' ? 'NULL' : $fases['mosid'];	
						
							$sql = "SELECT 
										CASE WHEN  '{$disabled}' = '' 
											THEN '<img onclick=\"return excluirAnexo('|| max.arqid ||',{$fases['facordem']} )\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\"  />' 
											ELSE ''
										END as acao,
										'<a onclick=\"baixarArquivo(' || max.arqid || ');\">' || date(mnxdataanexo) || ' / ' || arq.arqnome || '</a>' as arqid 
									FROM cte.monitoramentoanexos max
									INNER JOIN public.arquivo arq ON arq.arqid = max.arqid
									WHERE 
										mosid = {$mosid} AND mnxstatus = 'A'";
							$cabecalho = array('desc','teste');
							$db->monta_lista_simples( $sql, array('Ação','Arquivo(s)'), 1000, 1000, 'N','95%');
		
						?>
					</div>
					<?php
						$sql = "SELECT 
									max(date(mnxdataanexo)) || ' / ' || max(mnxid) as data
								FROM cte.monitoramentoanexos 
								WHERE 
									mosid = {$mosid} AND mnxstatus = 'A'";
					?>
					<input type="hidden" id="ultimoanexo_<?=$fases['facordem'] ?>" name="ultimoanexo_<?=$fases['facordem'] ?>" value="<?=$db->pegaUm($sql)?>">
					<input type="hidden" id="alteraanexo_<?=$fases['facordem'] ?>" name="alteraanexo_<?=$fases['facordem'] ?>" value="<?=$alteraanexo[$fases['facordem']] ?>">
				</td>
				<td align="center" width="100px" >
					<input type="hidden" id="finalizado_<?=$fases['facordem'] ?>" value="<?=$fases['mosfinalizado'] ?>"></input>
<?php
		if( $fases['mosfinalizado'] != 't' ){ 
			if( $fases['mosid'] == '' ){
				$fases['mosid'] = 'false';
			}
?>
					<input type="button" id="btnFinalizar" name="btnFinalizar" value="Finalizar" onclick="recarrega(finalizarFase(<?=$fases['mosid'] ?>,<?=$fases['facordem'] ?>));" title="Em caso de finalização desta fase anexe um arquivo demonstrativo e clique em finalizar." <?=$disabled ?>/>
<?php 
		} elseif ( $fases['mosfinalizado'] == 't' ) {
?>
					Esta fase já foi finalizada.
<?php 
		}
?>
				</td>
			</tr>
<?php 
		$i++;
	}
?>
			<tr >
				<td colspan="4">
					<table width="100%" class="listagem" id="listaItensComposicao">
						<tr style="border-top: 2px solid #CCCCCC; border-bottom: 2px solid rgb(0,0,0);" >
							<td align="center" width="100%"  colspan="5" style="padding:3px; border: 0px">
								<strong>Itens de Composição: </strong>
							</td>
						</tr>
<?php 
	listaItensComposição( $_REQUEST['sbaid'], $_REQUEST['ano'], $_REQUEST['csuid'], $blocAll );	
	$disabled = ( $blocAll == 'N' ) ? 'disabled="disabled"' : '';
?>
<?php 

?>
					</table>
				</td>
			</tr>
            <tr>
              	<td align="center" colspan="4">
              		<table width="100%">
<!--			    /************************************************************	
<!--				* 	BOTÕES DO RODAPÉ-->
<!--				* 	DESCRIÇÃO: -->
<!--				*   Se existe sbaid pai então mostra BTNS do Aditivo.-->
<!--				*************************************************************/-->
		              	<tr id="buttonsContainer">
							<td style="width: 25%;text-align : left; border:0px;">
								<input type="hidden" name="vlrProximaSubacao" value="" id="vlrProximaSubacao" />
								<input onclick="subAnterior();" type="button" name="btnAnterior" value="Anterior" id="btnAnterior" <?php echo !$sbaidAnterior ? 'disabled="disabled"' : ''; ?>/>
							</td>
		                	<td style="text-align : center; border:0px;">
			                  	<input onclick="salvarMonitoramento(); subAnterior();"  type="button" name="btnSalvarAnterior" value="Salvar e Anterior" 	id="btnSalvarAnterior" <?php echo !$sbaidAnterior ? 'disabled="disabled"' : ''; ?><?=$disabled ?>/>
			                  	<input onclick="recarrega(salvarMonitoramento());"  type="button" name="btnSalvar"         value="Salvar"  				id="btnSalvar"          <?=$disabled ?>/>
		                 	 	<input onclick="salvarMonitoramento(); subPosterior();"  type="button" name="btnSalvarProximo"  value="Salvar e Posterior"  	id="btnSalvarProximo"  <?php echo !$sbaidPosterior  ? 'disabled="disabled"' : ''; ?><?=$disabled ?>/>
		                 	 	<br />
		                 	 	<div id="divImportardados"></div>
							</td>
							<td style="width: 25%;text-align : right; border:0px;">
								<input onclick="subPosterior();" type="button" name="btnProximo" value="Próxima" id="btnProximo"        <?php echo !$sbaidPosterior ? 'disabled="disabled"' : ''; ?>/>
							</td>
						</tr>
					</table>
				</td>
			</tr>
<?php 
	if( cte_possuiPerfil(array(CTE_PERFIL_SUPER_USUARIO,CTE_PERFIL_ADMINISTRADOR)) ){
?>
			<tr style="background-color: rgb(210,210,220)"">
				<td align="left" colspan="4" style="padding: 5px; font-size: 110%">
					<strong>Parecer</strong>
				</td>
			</tr>
			<tr style="background-color: rgb(245,245,245);">
				<td colspan="4">
					<table widht="100%" height="100%">
						<tr>
							<td style="width:10%"></td>
							<td style="border:0; width:40%">
								<?=campo_textarea( 'textoPar', 'S', 'S', '', 50, 5, '', 'onkeyup="contaCaracteres(\'Par\', 1000);"', $acao = 0, '$txtdica = ', $tab = true, $title = NULL, $value = null);?>
								<?=campo_texto( 'restoPar', '', 'N', '', 4, 4, '', '', 'left', '', 0, 'id="restoPar"','',1000 ); ?> Caracteres restantes.
							</td>
							<td align="right" style="border:0; width:40%">
<?php 
		foreach($dados as $fases){
		
?>
								<INPUT type="checkbox" name="opcao" id="opcao<?=$fases['facordem'] ?>" value="<?=$fases['mosid'] ?>"> <?=$fases['facordem']?>ª Fase
<?php 
		}
?>
								</br>
								</br>
								<input type="button" id="btnSalvarParecer" name="btnSalvarParecer" value="Salvar Parecer" onclick="recarrega(salvarParecer());"/>
							</td>
							<td style="width:10%"></td>
						</tr>
					</table>
					<div id="divValidaParecer">
<?php
					listaValidaParecer($_REQUEST['sbaid'], $_REQUEST['ano'], $_REQUEST['csuid'] ); 
?>	
					</div>
					<div id="divHistoricoParecer">
<?php
					if( cte_possuiPerfil(array(CTE_PERFIL_ADMINISTRADOR,CTE_PERFIL_SUPER_USUARIO)) ) {
						listaHistoricoParecer($_REQUEST['sbaid'], $_REQUEST['ano'], $_REQUEST['csuid'] ); 
					}
?>	
					</div>
				</td>
			</tr>
			<tr style="background-color: rgb(210,210,220)">
				<td align="left" colspan="4" style="padding: 5px; font-size: 110%">
					<strong>Dúvidas</strong>
				</td>
			</tr>
			<tr bgcolor="white" >
				<td colspan="4" style="border:0px">
					<div id="divDuvidas" width="100%" border="0" style="border:0px">
					<?=escreveDuvidas( $_REQUEST, $possuiConvenio ); ?>
					</div>
				</td>
			</tr>
<?php 
	}
}
?>
		</tbody>
	</table>
</form>
<?php
	if ($undid) {
	    $sql = '
	    SELECT il.labid
	    FROM cte.laboratorio l
	    INNER JOIN cte.itenslaboratorio il ON l.labid = il.labid
	    WHERE l.undid = ' . $undid;
	
	    if (($labid = $db->pegaUm($sql)) === false)
	        $labid = 'null';
	} else {
	    $labid = 'null';
	}

?>
<script type="text/javascript">
  
    /**
     * @public
     * @global
     */
    var IExplorer               = !!document.all;
    var safari                  = navigator.userAgent.indexOf( 'Safari' ) > 0;
    var gecko                   = navigator.product == 'Gecko';
    var _displayBlock           = IExplorer ? 'block' : 'table-row';

    var sbaid					= <?=$dados[0]['sbaid']   ?>;
    var catid					= <?=$dados[0]['catid']   ?>;
    var qtdfases				= <?=$dados[0]['qtdfases']?>;

    var estuf                   = '<?=$_REQUEST['estuf']?>';
    var sbaidAnterior           = '<?=$sbaidAnterior    ?>';
    var csuidAnterior           = '<?=$csuidAnterior    ?>';
    var sbaidPosterior          = '<?=$sbaidPosterior   ?>';
    var csuidPosterior          = '<?=$csuidPosterior   ?>';
    var ano                     = '<?=$_REQUEST['ano']  ?>';
    var csuid                   = '<?=$_REQUEST['csuid']  ?>';
    var cmoid                   = '<?=$_REQUEST['cmoid']  ?>';
    var ultima                  = '<?=$i;  ?>';

</script>

<script type="text/javascript" src="../brasilpro/includes/par_subacao.js"></script>

<script type="text/javascript" >

    $('loader-container').hide();
    
    function abreDuvida( ordem ){
		if ( $('divDuvida_'+ordem).style.display == 'none' ){
			$('divDuvida_'+ordem).style.display = 'block';
			return false;
		}
		if ( $('divDuvida_'+ordem).style.display == 'block' ){
			$('divDuvida_'+ordem).style.display = 'none';
        	return false;
		}
    };

    function abreResposta( mduid ){
		if ( $('resposta'+mduid).style.display == 'none' ){
			$('resposta'+mduid).style.display = 'block';
			return false;
		}
		if ( $('resposta'+mduid).style.display == 'block' ){
			$('resposta'+mduid).style.display = 'none';
        	return false;
		}
    };

    function abrePopUpResposta( mosid ){

    	javascript:window.open("brasilpro.php?modulo=principal/monitoramentoInterno/duvidasRespondidas&acao=A&mosid="+mosid,"Respondidas","width=850,height=600,resizable=yes")
		
    };

    function abrePopUpHistorico( mosid ){

    	javascript:window.open("brasilpro.php?modulo=principal/monitoramentoInterno/historicoMonitoramento&acao=A&mosid="+mosid,"Histórico","width=850,height=600,resizable=yes")
		
    };

    function gravaDuvida( ordem, mosid ){

		var texto = $('texto'+ordem+'1');

		if( texto.value == '' ){
			alert('Campo obrigatório');
			texto.focus();
			return false
		}
		
		var duvida = texto.value;

		if ( mosid == 'NULL' ){
			alert('Para gravar esta duvida é necessário salvar o monitoramento.');
			return false;
		}

		var dados = { req : 'gravarDuvida', mosid : mosid, mdupergunta : duvida};
		
		new Ajax.Request(window.location.href,{
			method: 'post',
			asynchronous: false,
			parameters: dados,
			onLoading: function (){
				$('loader-container').show();
			},
			onComplete: function(res){
				alert(res.responseText);
				$('loader-container').hide();
				recarrega( true );
			}
		});
		
    };

    function deletaDuvida( mduid, sbaid, possuiConvenio ){

		var div   = $('divDuvidas');

		var dados = { req : 'deletaDuvida', mduid : mduid, sbaid : sbaid, possuiConvenio : possuiConvenio  };

		if( confirm('Realmente deseja excluir esta dúvida?') ){
			new Ajax.Request(window.location.href,{
				method: 'post',
				asynchronous: false,
				parameters: dados,
				onLoading: function (){
					$('loader-container').show();
				},
				onComplete: function(res){
					alert('Dúvida excluída.');
					$('loader-container').hide();
					div.innerHTML = res.responseText;
				}
			});
		}
		
    };

    function gravaResposta( mduid, sbaid, possuiConvenio ){

		var resposta = $('mduresposta'+mduid);
		var div 	 = $('divDuvidas');

		if( resposta.value == ''){
			alert('Preencha a resposta');
			resposta.focus();
			return false;
		}
		
		var dados = { req : 'gravarResposta', mduid : mduid, mduresposta : resposta.value, sbaid : sbaid, possuiConvenio : possuiConvenio  };
		
		new Ajax.Request(window.location.href,{
			method: 'post',
			asynchronous: false,
			parameters: dados,
			onLoading: function (){
				$('loader-container').show();
			},
			onComplete: function(res){
				alert('Dúvida respondida.');
				$('loader-container').hide();
				div.innerHTML = res.responseText;
			}
		});
		
    };

    function limparDuvida( mosid ){

		if(confirm('Tem certeza que deseja apagar essa dúvida?')){
			new Ajax.Request(window.location.href,{
				method: 'post',
				asynchronous: false,
				parameters: '&req=limparResposta&mosid='+mosid,
				onLoading: function (){
					$('loader-container').show();
				},
				onComplete: function(res){
					alert(res.responseText);
					$('loader-container').hide();
					recarrega( true );
				}
			});
		}
    };

    function atualizaDatas(){
    	$('mosdatainicio').value = $('mosdatainicio_1').value;
    	$('mosdatafinal').value  = $('mosdatafinal_'+qtdfases).value;
    };

    function recarrega( mosid ){
        if ( mosid != false ) {
        	document.forms['frmParSubacao'].submit();
        }
    };

	function toggleDiv( idDiv ){
	
		var obDiv = document.getElementById( idDiv );
	
		if( obDiv.style.display == 'block' )
			obDiv.style.display = 'none';
		else
			obDiv.style.display = 'block';
		
	}
	
	function string2float(str){
        if (str.indexOf(',') == -1)
            return parseFloat(str);
        else
            return parseFloat(str.replace(/\./g, '').replace(/,/g, '.'));
    }

	function float2string(num){
        return mascaraglobal('###.###.###.###.###,##', (parseFloat(num).toFixed(2)) + '');
    }

	function contaCaracteres( ordem, qtd ) {
		texto = $('texto'+ordem).value.length;
		resto = qtd - texto;
		if( resto < 0 ){
			$('texto'+ordem).value = $('texto'+ordem).value.substring(0, qtd);
			$('resto'+ordem).value = 0;
			return false;
		}
		$('resto'+ordem).value = resto;
		return false;
	}

	function anexarArquivo( num ){
		document.getElementById('req').value = 'anexar';
		$('num').value = num;
//		document.write( $('frmParSubacao').serialize() );
		$('frmParSubacao').submit();
	}

	function baixarArquivo( arqid ){

		location.href = "?<?=$_SERVER['QUERY_STRING']?>&req=baixar&arqid="+arqid;
		//location.href = 'brasilpro.php?modulo=principal/monitoramentoInterno/monitoraSubacao&acao=A&req=baixar&arqid='+arqid;
		//return false;
	}

	function finalizarFase(mosid , ordem){

		var mosids   = salvarMonitoramento();
		
		if ( mosids == false ) {
			return false;
		}

		texto   = $('texto'+ordem);
		arquivo = $('arquivo'+ordem);

		if(texto.value == ''){
			alert('Campo obrigatório para finalização!');
			texto.focus();
			return false;
		} 

		if( ordem == ultima ){
			
			var qtds 	   = document.getElementsByName('qtdExec[]');
			var vlrUniExec = document.getElementsByName('vlrUniExec[]');
			var tabela     = document.getElementById('listaItensComposicao');

			var tamanho = tabela.rows.length - 4;
			
			var cosid   = "{}";
			var qtd     = "{}";
			var vlrExec = "{}";
			
			for( i=0; i<=tamanho; i++){

				if( qtds[i].value ==  '0,00' || qtds[i].value ==  ''|| qtds[i].value ==  '0'  ){
					alert('Para finalização da ultima fase é necessário \n o preenchimento de todos itens de composição!');
					qtds[i].focus();
					return false;
				}

				if( vlrUniExec[i].value ==  '0,00' || vlrUniExec[i].value ==  '' || vlrUniExec[i].value ==  '0' ){
					alert('Para finalização da ultima fase é necessário \no preenchimento de todos itens de composição!');
					vlrUniExec[i].focus();
					return false;
				}

			}
		}

		var bool    = confirm('Deseja realmente finalizar essa fase? Esta ação bloqueia qualquer edição na fase e pode ser irreverssivel.')		
		if( bool ){
			new Ajax.Request(window.location.href,{
				method: 'post',
				asynchronous: false,
				parameters: '&req=finalizar&mosid='+mosid+'&ordem='+ordem,
				onLoading: function (){
					$('loader-container').show();
				},
				onComplete: function(res){
					if( res.responseText.length == 15 ){
						alert('Fase finalizada.');
						$('loader-container').hide();
						return true;
					}else{
						alert(res.responseText);
						$('loader-container').hide();
						return false;
					}
				}
			});
		}
	}

	function subAnterior(){
		
		window.location.href = '/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/monitoraSubacao&acao=A&sbaid='+ sbaidAnterior+'&estuf='+estuf+'&ano='+ano+'&cmoid='+cmoid+'&csuid='+csuidAnterior+'&bloqueado=0';
	}

	function subPosterior(){

		window.location.href = '/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/monitoraSubacao&acao=A&sbaid='+ sbaidPosterior+'&estuf='+estuf+'&ano='+ano+'&cmoid='+cmoid+'&csuid='+csuidPosterior+'&bloqueado=0';
	}

	function calculoQtdExecutada(cosid)
	{
		var vQtd 		= document.getElementById('qtdExec'+cosid);
		var vExec 		= document.getElementById('vlrExec'+cosid);
		var vUniExec	= document.getElementById('vlrUniExec'+cosid);

		qtd = vQtd.value.replace('.','');
		qtd = qtd.replace('.','');
		qtd = qtd.replace('.','');
		qtd = qtd.replace('.','');
		qtd = qtd.replace(',','.');

		exec = vExec.value.replace('.','');
		exec = exec.replace('.','');
		exec = exec.replace('.','');
		exec = exec.replace('.','');
		exec = exec.replace(',','.');

		uniExec = vUniExec.value.replace('.','');
		uniExec = uniExec.replace('.','');
		uniExec = uniExec.replace('.','');
		uniExec = uniExec.replace('.','');
		uniExec = uniExec.replace(',','.');

		if( exec != '' && exec != '0.00' )
		{
			var calculo 	= ( Number(qtd) * Number(exec) );
			calculo    		= calculo.toFixed(2);
			vUniExec.value 	= mascaraglobal('[###.]###,##',calculo);
		}
	}
	
	function calculoVlrExecutado(cosid)
	{
		var vQtd 		= document.getElementById('qtdExec'+cosid);
		var vExec 		= document.getElementById('vlrExec'+cosid);
		var vUniExec	= document.getElementById('vlrUniExec'+cosid);

		qtd = vQtd.value.replace('.','');
		qtd = qtd.replace('.','');
		qtd = qtd.replace('.','');
		qtd = qtd.replace('.','');
		qtd = qtd.replace(',','.');

		exec = vExec.value.replace('.','');
		exec = exec.replace('.','');
		exec = exec.replace('.','');
		exec = exec.replace('.','');
		exec = exec.replace(',','.');

		uniExec = vUniExec.value.replace('.','');
		uniExec = uniExec.replace('.','');
		uniExec = uniExec.replace('.','');
		uniExec = uniExec.replace('.','');
		uniExec = uniExec.replace(',','.');

		var calculo = exec;
		
		if( qtd != '' )
		{
			calculo = ( Number(qtd) * Number(exec) );
			calculo	= calculo.toFixed(2);
		}

		vUniExec.value 	= mascaraglobal('[###.]###,##',calculo);
	}

	function calculoVlrUniExecutado(cosid)
	{
		var vQtd 		= document.getElementById('qtdExec'+cosid);
		var vExec 		= document.getElementById('vlrExec'+cosid);
		var vUniExec	= document.getElementById('vlrUniExec'+cosid);

		qtd = vQtd.value.replace('.','');
		qtd = qtd.replace('.','');
		qtd = qtd.replace('.','');
		qtd = qtd.replace('.','');
		qtd = qtd.replace(',','.');

		exec = vExec.value.replace('.','');
		exec = exec.replace('.','');
		exec = exec.replace('.','');
		exec = exec.replace('.','');
		exec = exec.replace(',','.');

		uniExec = vUniExec.value.replace('.','');
		uniExec = uniExec.replace('.','');
		uniExec = uniExec.replace('.','');
		uniExec = uniExec.replace('.','');
		uniExec = uniExec.replace(',','.');
		
		if( qtd != '' && qtd != '0.00' )
		{
			calculo = ( Number(uniExec) / Number(qtd) );
			calculo	= calculo.toFixed(2);
			vExec.value = mascaraglobal('[###.]###,##',calculo);
		}
	}
	
	function somatorio(){

		var qtds 	   = document.getElementsByName('qtdExec[]');
		var vlrExec    = document.getElementsByName('vlrExec[]');
		var vlrUniExec = document.getElementsByName('vlrUniExec[]');
		var tabela     = document.getElementById('listaItensComposicao');
		var celqtd     = document.getElementById('tdsomamcoqtd');
		var celvlrExec = document.getElementById('tdsomamcovlruni');
		var celvlr     = document.getElementById('tdsomamcovlrtot');
		
		var tamanho = tabela.rows.length - 4;
		var somaqtd = 0;
		var somavlr = 0;
		var somavlruni = 0;
		
		for( i=0; i<=tamanho; i++){

			if( qtds[i].value == '' ){
				qtds[i].value = '0,00';
			}
			if( vlrExec[i].value == '' ){
				vlrExec[i].value = '0,00';
			}
			if( vlrUniExec[i].value == '' ){
				vlrUniExec[i].value = '0,00';
			}
			
			var qtd = qtds[i].value.replace('.','');
			qtd = qtd.replace('.','');
			qtd = qtd.replace('.','');
			qtd = qtd.replace('.','');
			qtd = qtd.replace(',','.');

			var vlrEx = vlrExec[i].value.replace('.','');
			vlrEx = vlrEx.replace('.','');
			vlrEx = vlrEx.replace('.','');
			vlrEx = vlrEx.replace('.','');
			vlrEx = vlrEx.replace(',','.');
			
			var vlr = vlrUniExec[i].value.replace('.','');
			vlr = vlr.replace('.','');
			vlr = vlr.replace('.','');
			vlr = vlr.replace('.','');
			vlr = vlr.replace(',','.');
			
			somaqtd = parseFloat(somaqtd) + parseFloat(qtd);
			somavlr = parseFloat(somavlr) + parseFloat(vlrEx);
			somavlruni = parseFloat(somavlruni) + parseFloat(vlr);
			
		}

		somaqtd    = somaqtd;
		somavlr    = somavlr.toFixed(2);
		somavlruni = somavlruni.toFixed(2);
		
		celqtd.innerHTML 		= mascaraglobal('[###.]###',somaqtd);
		celvlrExec.innerHTML 	= 'R$ '+mascaraglobal('[###.]###,##',somavlr);
		celvlr.innerHTML 		= 'R$ '+mascaraglobal('[###.]###,##',somavlruni);
		
	}

	function salvarMonitoramento(){

		var csuid              = $('csuid').value;
		var texto              = new Array();
		var textoant           = new Array();
		var facid              = new Array();
		var finalizado         = new Array();
		var mosid              = '';
		var mosdatainicio      = $('mosdatainicio');
		var mosdatafinal       = $('mosdatafinal');
		var armosdatainicio    = new Array();
		var armosdatafinal     = new Array();
		var armosdatainicioant = new Array();
		var armosdatafinalant  = new Array();
		var textoant           = new Array();
		var alteraanexo        = new Array();
		var ultimoanexo        = new Array();

		for (i=1;i<=qtdfases;i++){
			
			texto[i]              = $('texto'+i).value;
			textoant[i]           = $('textoant_'+i).value;
			facid[i]              = $('facid'+i).value;
			finalizado[i]         = $('finalizado_'+i).value;
			armosdatainicio[i]    = $('mosdatainicio_'+i).value;
			armosdatafinal[i]     = $('mosdatafinal_'+i).value;
			armosdatainicioant[i] = $('mosdatainicioant_'+i).value;
			armosdatafinalant[i]  = $('mosdatafinalant_'+i).value;
			alteraanexo[i]        = $('alteraanexo_'+i).value;
			ultimoanexo[i]        = $('ultimoanexo_'+i).value;

			if( armosdatainicio[i] == '' ){
				alert('Campo obrigatório.');
				if( i==1 ){
					mosdatainicio.focus();
					return false;
				}
				$('mosdatainicio_'+i).focus();
				return false;
			}

			if( armosdatafinal[i] == '' ){
				alert('Campo obrigatório.');
				if( i == qtdfases ){
					mosdatafinal.focus();
					return false;
				}
				$('mosdatafinal_'+i).focus();
				return false;
			}
		}

		if( mosdatainicio.value == '' ){
			alert('Campo obrigatório!');
			mosdatainicio.focus();
			return false;
		}

		if( mosdatafinal.value == '' ){
			alert('Campo obrigatório!');
			mosdatafinal.focus();
			return false;
		}

		armosdatainicio['1']     = mosdatainicio.value;
		armosdatafinal[qtdfases] = mosdatafinal.value;

		var textoR = "{}";
		var textoantR = "{}";
		var facidR = "{}";
		var finalizadoR = "{}";
		var armosdatainicioR = "{}";
		var armosdatafinalR = "{}";
		var armosdatainicioantR = "{}";
		var armosdatafinalantR = "{}";
		var alteraanexoR = "{}";
		var ultimoanexoR = "{}";
		
		for (i=1;i<=qtdfases;i++){
			textoR 			    = textoR+"{#sep!}"+texto[i];
			textoantR 		    = textoantR+"{#sep!}"+textoant[i];
			facidR 			    = facidR+","+facid[i];
			finalizadoR 	    = finalizado[i];
			armosdatainicioR    = armosdatainicioR+","+armosdatainicio[i];
			armosdatafinalR     = armosdatafinalR+","+armosdatafinal[i];
			armosdatainicioantR = armosdatainicioantR+","+armosdatainicioant[i];
			armosdatafinalantR  = armosdatafinalantR+","+armosdatafinalant[i];
			alteraanexoR        = alteraanexoR+","+alteraanexo[i];
			ultimoanexoR        = ultimoanexoR+","+ultimoanexo[i];
		}

		// organiza dados de itens composição
		
		var qtds 	   		= document.getElementsByName('qtdExec[]');
		var vlrExecutado 	= document.getElementsByName('vlrExec[]');
		var vlrUniExec 		= document.getElementsByName('vlrUniExec[]');
		var tabela     		= document.getElementById('listaItensComposicao');

		var tamanho = tabela.rows.length - 4;
		
		var cosid   		= "{}";
		var qtd     		= "{}";
		var vlrExecut 		= "{}";
		var vlrExec 		= "{}";
		
		for( i=0; i<=tamanho; i++){

			cosid   		= cosid+","+qtds[i].id.replace("qtdExec","");
			qtd     		= qtd+"{"+qtds[i].value;
			vlrExecut		= vlrExecut+"{"+vlrExecutado[i].value;
			vlrExec 		= vlrExec+"{"+vlrUniExec[i].value;

		}
		
		var dados = { req : 'salvar', itens : tamanho, cosid : cosid, qtd : qtd, vlrExec : vlrExec, vlrExecut : vlrExecut, csuid : csuid, texto : textoR , textoant : textoantR , sbaid : sbaid , facid : facidR , finalizado : finalizadoR , armosdatainicio : armosdatainicioR , armosdatainicioant : armosdatainicioantR , armosdatafinal : armosdatafinalR , armosdatafinalant : armosdatafinalantR , alteraanexo : alteraanexoR , ultimoanexo : ultimoanexoR , ano : ano, csuid : csuid};
		
		new Ajax.Request(window.location.href,{
			method: 'post',
			asynchronous: false,
			parameters: dados,
			onLoading: function (){
				$('loader-container').show();
			},
			onComplete: function(res){
				mosid = res.responseText;
//				alert(mosid);
				$('loader-container').hide();
				alert('Dados salvos com sucesso.');
			}
		});	

		return mosid;
	}

	function salvarParecer(){

		var mosid   = new Array();
		var checked = 0;

		for (i=0;i<qtdfases;i++){
			if($('opcao'+(i+1)).checked){
				mosid[i] = $('opcao'+(i+1)).value;
				checked = 1;
			}else{
				mosid[i] = '';
			}
		}

		parecer = $('textoPar').value

		if( parecer == '' ){
			alert('Campo obrigatório.');
			$('textoPar').focus();
			return false;
		}

		if( checked == 0 ){
			alert('Pelo menos uma fase deve ser escolhida');
			$('opcao1').focus();
			return false;
		}

		var dados = { req : 'parecer', parecer : "{#sep!}"+parecer+"{#sep!}", mosid : "'"+mosid+"'" };
		
		new Ajax.Request(window.location.href,{
			method: 'post',
			asynchronous: false,
			parameters: dados,
			onLoading: function (){
				$('loader-container').show();
			},
			onComplete: function(res){
				alert(res.responseText);
				$('loader-container').hide();
			}
		});	
		
		return true;
	}

	function excluirAnexo( arqid, num ){

		if(confirm('Realmente deseja excluir este anexo?')){
//			new Ajax.Request(window.location.href,{
//				method: 'post',
//				asynchronous: false,
//				parameters: '&req=excluiranexo&arqid='+arqid,
//				onLoading: function (){
//					$('loader-container').show();
//				},
//				onComplete: function(res){
//					alert(res.responseText);
//					$('loader-container').hide();
//					recarrega( true );
//				}
//			});	
			$('num').value = num;
			$('req').value = 'excluiranexo';
			$('arqid').value = arqid;
			
			recarrega( true );
		}
		
		return false;
	}

	function checaData (){

		var dataInicio      = $('mosdatainicio_1');
		var dataFinal       = $('mosdatafinal_'+qtdfases);
		var armosdatainicio = new Array();
		var armosdatafinal  = new Array();
		var obData 	        = new Data();

		if ( (dataInicio.value != '') && (dataFinal.value != '') ){
			if( obData.comparaData(dataInicio.value,dataFinal.value, '>') ){
				alert('Data Inicial deve ser menor que a data final!');
				dataInicio.value = '';
				dataInicio.focus();
				return false;
			}
		}

		for (i=1;i<=qtdfases;i++){
			armosdatainicio[i] = $('mosdatainicio_'+i);
			armosdatafinal[i]  = $('mosdatafinal_'+i);

			if ( (armosdatainicio[i].value != '') && (armosdatafinal[i].value != '') ){
				if( obData.comparaData(armosdatainicio[i].value,armosdatafinal[i].value, '>')  ){
					alert('Data inicial deve ser menor ou igual que a data final!');
					if( i == 1 ){
						armosdatafinal[i].value = '';
						armosdatafinal[i].focus();
						return false;
					}
					armosdatainicio[i].value = '';
					armosdatainicio[i].focus();
					return false;
				}
			}
			
			if( (armosdatainicio[i].value != '')  && ( i != '1' )){
				if( dataInicio.value != '' ){
					if( obData.comparaData(armosdatainicio[i].value,dataInicio.value, '<')  ){
						alert('A data de inicial da fase deve ser maior ou igual a data de inicio do monitoramento!');
						armosdatafinal[i].value = '';
						armosdatafinal[i].focus();
						return false;
					}
				}
				if( dataFinal.value != '' ){
					if( obData.comparaData(armosdatainicio[i].value,dataFinal.value, '>')  ){
						alert('A data de inicial da fase deve ser menor ou igual a data de término do monitoramento!');
						armosdatafinal[i].value = '';
						armosdatafinal[i].focus();
						return false;
					}
				}
			}
			
			if( (armosdatafinal[i].value != '')  && ( i != qtdfases )){
				if( dataInicio.value != '' ){
					if( obData.comparaData( armosdatafinal[i].value, dataInicio.value, '<')  ){
						alert('A data de final da fase deve ser maior ou igual a data de inicio do monitoramento!');
						armosdatafinal[i].value = '';
						armosdatafinal[i].focus();
						return false;
					}
				}
				if( dataFinal.value != '' ){
					if( obData.comparaData( armosdatafinal[i].value, dataFinal.value, '>')  ){
						alert('A data de final da fase deve ser menor ou igual a data de término do monitoramento!');
						armosdatafinal[i].value = '';
						armosdatafinal[i].focus();
						return false;
					}
				}
			}
		}
	}

	function atendeParecer( mpaid, mosid ){
		if( confirm('Atender parecer?') ){

			var div = $('divParecer_'+mosid);
			
			new Ajax.Request(window.location.href,{
				method: 'post',
				asynchronous: false,
				parameters: '&req=atendeparecer&mpaid='+mpaid+'&mosid='+mosid,
				onLoading: function (){
					$('loader-container').show();
				},
				onComplete: function(res){
					alert('Parecer atendido.');
					div.innerHTML = res.responseText;
					atualizaValidacao();
				}
			});	
		}
	}

	function atualizaValidacao(){
		var div = $('divValidaParecer');
		new Ajax.Request(window.location.href,{
			method: 'post',
			asynchronous: false,
			parameters: '&req=validaparecer&sbaid='+sbaid+'&mosano='+ano+'&csuid='+csuid,
			onLoading: function (){
				$('loader-container').show();
			},
			onComplete: function(res){
				div.innerHTML = res.responseText;
				historicoParecer();
			}
		});	
	}

	function validaParecer( mpaid ){
		if( confirm('Validar parecer?') ){

			var div = $('divValidaParecer');
			
			new Ajax.Request(window.location.href,{
				method: 'post',
				asynchronous: false,
				parameters: '&req=validaparecer&mpaid='+mpaid+'&sbaid='+sbaid+'&mosano='+ano+'&csuid='+csuid,
				onLoading: function (){
					$('loader-container').show();
				},
				onComplete: function(res){
					alert('Parecer validado.');
					div.innerHTML = res.responseText;
					historicoParecer();
				}
			});	
		}
	}

	function historicoParecer(){
		var div = $('divHistoricoParecer');
		
		new Ajax.Request(window.location.href,{
			method: 'post',
			asynchronous: false,
			parameters: '&req=historicoparecer&sbaid='+sbaid+'&mosano='+ano+'&csuid='+csuid,
			onLoading: function (){
				$('loader-container').show();
			},
			onComplete: function(res){
				div.innerHTML = res.responseText;
			}
		});	
	}

	function excluirParecer( mpaid ){
		if( confirm('Excluir parecer?') ){

			var div = $('divHistoricoParecer');
			
			new Ajax.Request(window.location.href,{
				method: 'post',
				asynchronous: false,
				parameters: '&req=excluirparecer&mpaid='+mpaid+'&sbaid='+sbaid+'&mosano='+ano+'&csuid='+csuid,
				onLoading: function (){
					$('loader-container').show();
				},
				onComplete: function(res){
					alert('Parecer excluido.');
					div.innerHTML = res.responseText;
				}
			});	
		}
	}

</script>
</div>
</body>
</html>
<?php die(); ?>
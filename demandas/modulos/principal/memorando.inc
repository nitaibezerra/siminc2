<?php


if($_REQUEST['emitirmemo']){
	$sql = "UPDATE demandas.memorando SET memstatus='E' WHERE memid=".$_REQUEST['emitirmemo'];
	$db->executar($sql);
	$db->commit();

	if($_REQUEST['icone'] == 'ok'){
		echo '<script>
			alert("Memorando Emitido com sucesso!");
			location.href="demandas.php?modulo=principal/memorando&acao=A";
		  </script>';
	}else{
		echo '<script>
			alert("Memorando Emitido com sucesso!");
			opener.location.href="demandas.php?modulo=principal/memorando&acao=A";
			window.close();
		  </script>';
	}

	die;
}

if($_REQUEST['exibirpdf'] || $_REQUEST['exibirmemo']){
	
	if($_REQUEST['exibirmemo']) $_REQUEST['exibirpdf'] = $_REQUEST['exibirmemo'];
	
	ini_set("memory_limit", "1024M");
	set_time_limit(8000);
	ini_set('max_execution_time', 3000);
	
	include_once APPRAIZ . "includes/dompdf/dompdf_config.inc.php";
	
	header('Content-type: text/html; charset=ISO-8859-1');
	
	ob_clean();
	
	$sql = "
        SELECT  memid,
        		memnumero,
                to_char(memdatainclusao, 'DD/MM/YYYY') as datamemo,
                to_char(memdatainclusao, 'DD') as dia,
                memmes as mes,
                memano as ano, 
                memss, 
                memos, 
       			memqtdust, 
       			memvlust,
       			usunome,
       			m.ordid,
       			orddescricao,
       			memstatus,
       			memtexto
		FROM demandas.memorando m
        INNER JOIN seguranca.usuario AS u ON u.usucpf = m.usucpfresp
        INNER JOIN demandas.origemdemanda AS o ON o.ordid = m.ordid
        WHERE memid = ".$_REQUEST['exibirpdf'];
    $dados = $db->pegaLinha($sql);

    $mes = $dados["mes"];
    $ano = $dados["ano"];
	$memstatus = $dados["memstatus"];
	
	$html = '<html>
		<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
		</head>
		<body>
		<table width="100%" cellspacing="1" cellpadding="5" border="0" align="center" class="tabela">
			<tr>
				<td colspan="10">
					<center>
					<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">
						<tr bgcolor="#ffffff">
								<td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>
								<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
									SIMEC - Sistema Integrado de Monitoramento do Ministério da Educação<br>
									Ministério da Educação / SE - Secretaria Executiva<br>
									DTI - Diretoria de Tecnologia da Informação
								</td>		
								<td align="right" nowrap="nowrap" valign="middle" height="1" style="padding:5px 0 0 0;">
									Impresso por: <b>'.$_SESSION["usunome"].'</b><br> Hora da Impressão:'.date("d/m/Y H:i:s").'<br>
								</td>	
						</tr>
					</table>	
					</center>
				</td>
			</tr>
			<tr>
				<td colspan="10" style="font-size: 12pt;">
					Memo nº '.$dados["memnumero"].'/'.$dados["ano"].'/CGD/DTI/SE/MEC
				</td>
			</tr>
			<tr>
				<td colspan="10" align="right" style="font-size: 12pt;">
					'.$dados["dia"].' de '.mesExtenso((int)$dados["mes"]).' de '.$dados["ano"].'
				</td>
			</tr>
			<tr>
				<td colspan="10" style="font-size: 12pt;">
					'.$dados["memtexto"].'
					<br><br><br>
					<center>
						'.$dados["usunome"].'<br><br>
						Fiscal Técnico<br>
						Coordenador Geral de Desenvolvimento<br>
						Diretoria de Tecnologia da Informação
					</center>
					<br><br><br>
					<center>
						Anexo - Ordem de Serviço para pagamento referente ao mês de '.mesExtenso((int)$dados["mes"]).'/'.$dados["ano"].'
					</center>
				</td>
			</tr>
			<tr>
				<td colspan="10">
					<table width="100%">
						<tr style="background:#D9D9D9;">
							<TD colspan="6">
								<div style="font-weight:bold;" align="center">Tipo de Despesa: Custeio</div>
							</TD>
						</tr>
						<TR style="background:#D9D9D9;">
							<TD align="center" valign="top" style="font-weight:bold;">Nº SS</TD>
							<TD align="center" valign="top" style="font-weight:bold;">Nº OS</TD>
							<TD align="center" valign="top" style="font-weight:bold;">Sistema</TD>
							<TD align="center" valign="top" style="font-weight:bold;">Quantidade UST</TD>
							<TD align="center" valign="top" style="font-weight:bold;">Valor UST (R$)</TD>
							<TD align="center" valign="top" style="font-weight:bold;">Valor Total (R$)</TD>
						</TR>
						<tr bgcolor="#E5E5E5">
							<td align="center"  style="font-size:12px;color:#000000;">
								'.$dados["memss"].'
							</td>
							<td align="center"  style="font-size:12px;color:#000000;">
								'.$dados["memos"].'
							</td>
							<td align="center"  style="font-size:12px;color:#000000;">
								'.$dados["orddescricao"].'
							</td>
							<td align="center"  style="font-size:12px;color:#000000;">
								'.$dados["memqtdust"].'
							</td>
							<td align="center"  style="font-size:12px;color:#000000;">
								'.number_format($dados["memvlust"],2,",",".").'
							</td>
							<td align="center"  style="font-size:12px;color:#000000;">
								'.number_format(($dados["memqtdust"]*$dados["memvlust"]),2,",",".").'
							</td>
						</tr>
						<tr bgcolor="#D9D9D9">
							<td colspan="5" align="left" style="font-size:12px;color:#000000;">
								<b>TOTAL A PAGAR</b>
							</td>
							<td align="center"  style="font-size:12px;color:#000000;">
								'.number_format(($dados["memqtdust"]*$dados["memvlust"]),2,",",".").'
							</td>
						</tr>
					</table>
					<br>';
					
					if($dados['memstatus'] == 'P'){
						$esdid = 1337; //pago
						$esddsc = 'PAGO'; 
					}else{
						$esdid = 1336; //aguardando pagamento
						$esddsc = 'AGUARDANDO PAGAMENTO'; 
						//$esdid = 772; //auditada
					}
					
					$mesQ = $dados['mes'];
					if(strlen($mesQ) == 1) $mesQ = "0".$mesQ;
					
					$sql = "select d.dmdid,
								   d.dmdtitulo,
								   to_char(d.dmddatainclusao, 'DD/MM/YYYY') AS datainclusao,
								   to_char(a.htddata, 'DD/MM/YYYY') AS dtsit,
								   d.dmdqtde,
								   t.tsppontuacao
							from demandas.demanda d
							inner join demandas.tiposervicoprioridade t ON t.tipid = d.tipid and t.priid = d.priid
							inner join demandas.tiposervico ts ON ts.tipid = d.tipid
							inner join workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35) and doc.esdid in (".$esdid.") 
							inner join workflow.historicodocumento a ON a.hstid = doc.hstid
							inner join demandas.memorandodemanda md ON md.dmdid = d.dmdid and md.memid = ".$dados['memid']."
							where dmdstatus='A'
							and ts.ordid = ".$dados['ordid']."
							and to_char(a.htddata, 'YYYY') = '".$dados['ano']."'
							and to_char(a.htddata, 'MM') = '".$mesQ."'
							order by 1
							--limit 10
							";
					$dadosDemandas = $db->carregar($sql);
					
					
				$html .= '<table width="100%">
						<TR style="background:#D9D9D9;">
							<TD align="center" valign="top" style="font-weight:bold;">Nº Demanda</TD>
							<TD align="center" valign="top" style="font-weight:bold;">Assunto</TD>
							<TD align="center" valign="top" style="font-weight:bold;">Dt. Abertura</TD>
							<TD align="center" valign="top" style="font-weight:bold;">Dt. Situação ('.$esddsc.')</TD>
							<TD align="center" valign="top" style="font-weight:bold;">Qt. Serviço</TD>
							<TD align="center" valign="top" style="font-weight:bold;">Pontuação</TD>
							<TD align="center" valign="top" style="font-weight:bold;">Total</TD>
						</TR>';
						if($dadosDemandas){
							foreach($dadosDemandas as $v){
								$html .= '<tr bgcolor="#E5E5E5">
									<td align="center"  style="font-size:12px;color:#000000;">
										'.$v["dmdid"].'
									</td>
									<td align="left"  style="font-size:12px;color:#000000;">
										'.$v["dmdtitulo"].'
									</td>
									<td align="center"  style="font-size:12px;color:#000000;">
										'.$v["datainclusao"].'
									</td>
									<td align="center"  style="font-size:12px;color:#000000;">
										'.$v["dtsit"].'
									</td>
									<td align="center"  style="font-size:12px;color:#000000;">
										'.$v["dmdqtde"].'
									</td>
									<td align="center"  style="font-size:12px;color:#000000;">
										'.( (int) $v["tsppontuacao"]).'
									</td>
									<td align="center"  style="font-size:12px;color:#000000;">
										'.( (int) $v["dmdqtde"] * (int) $v["tsppontuacao"]).'
									</td>
								</tr>';
							}
						}
						else{
							$html .= '<TR style="background:#FFFFFF;">
											<TD colspan="7" align="center" style="font-weight:bold; font-size:12px;"><font color="red">Não existem demandas com a situação Aguardando Pagamento.</font></TD>
									  </tr>';
						}
						$html .= '<TR style="background:#FFFFFF;">
							<TD colspan="7" align="left" style="font-weight:bold; font-size:9px; border-top:2px solid black; border-bottom:2px solid black;">Total de demandas: '.($dadosDemandas ? count($dadosDemandas) : 0).'</TD>
						</tr>
					</table>
					<div class="notprint" align="center">
						<br>';
						//if($_REQUEST['exibirmemo']){
						if($memstatus == 'G'){
							$html .= '<input type="button" name="btn_print" value="Emitir" onclick="location.href=\'demandas.php?modulo=principal/memorando&acao=A&emitirmemo='.$_REQUEST['exibirpdf'].'\';">';
						}
					$html .= ' <input type="button" name="btn_print" value="Imprimir" onclick="self.print();">
					</div>
				</td>
			</tr>
		</table>
		</body>
		</html>';
	
	echo $html;
	die;
	
	//$html = ob_get_contents();
	//ob_clean();
	$dompdf = new DOMPDF();
	$dompdf->load_html($html);
	$dompdf->set_paper('A4','portrait');
	$dompdf->render();
	
	$arqnome = "DemandasMemorando_{$mes}_{$ano}";
	
	if(isset($_SERVER['WINDIR']) && $_SERVER['WINDIR'] == "C:\windows"){
		$caminhoArq = "C:/tmp/".$arqnome.".pdf";
	} else {
		$caminhoArq = "/tmp/".$arqnome.".pdf";
	}
	
	file_put_contents($caminhoArq, $dompdf->output());
        
    // caminho completo
    $file = $caminhoArq;

    // cabeçalho
    header("Content-type: application/pdf");
    header("Content-Disposition: attachment;filename=$file");
    
    // mostra o download
    readfile($file);
    
    // destrói a variável do formulário
    unset($_REQUEST["formulario"]);
    
    //header("Location: demandas.php?modulo=principal/memorando&acao=A");
    
    exit;
	
	/*
		$html = ob_get_contents();
		ob_clean();
		$dompdf = new DOMPDF();
		$dompdf->load_html($html);
		$dompdf->render();
		
		$pdfoutput = $dompdf->output();

		//$caminho = APPRAIZ . 'www/par/prefeitos/pdfs/' . $mu['estuf'] . '_' . $mu['mundescricao'] . '.pdf';
		$arqnome = "memorando";
		if(isset($_SERVER['WINDIR']) && $_SERVER['WINDIR'] == "C:\windows"){
			$caminho = "C:/tmp/".$arqnome.".pdf";
		} else {
			$caminho = "/tmp/".$arqnome.".pdf";
		}
    	
    	$fp = fopen($caminho, "w+");
    	

		stream_set_write_buffer($fp, 0);
		fwrite($fp, $pdfoutput);
		fclose($fp);
	*/
	echo '<script>
			//alert("entoru!");
			//location.href="demandas.php?modulo=principal/memorando&acao=A";
		  </script>';
	die;
}

if($_REQUEST['pagarMemo']){
	
	ini_set("memory_limit", "1024M");
	set_time_limit(80000);
	ini_set('max_execution_time', 300);
	
	include APPRAIZ ."includes/workflow.php";
	
	$sql = "
        SELECT  
                memid,
                memmes as mes,
                memano as ano, 
       			m.ordid 
		FROM demandas.memorando m
        INNER JOIN seguranca.usuario AS u ON u.usucpf = m.usucpfresp
        INNER JOIN demandas.origemdemanda AS o ON o.ordid = m.ordid
        WHERE memid = ".$_REQUEST['pagarMemo'];
    $dados = $db->pegaLinha($sql);

	$memid = $dados["memid"];
    $ordid = $dados["ordid"];
    $mes = $dados["mes"];
    $ano = $dados["ano"];	
    
    $mesQ = $dados['mes'];
	if(strlen($mesQ) == 1) $mesQ = "0".$mesQ;
    
    unset($dados);
	
	$sql = "select d.dmdid,
				   doc.esdid,
				   doc.docid
			from demandas.demanda d
			inner join demandas.tiposervicoprioridade t ON t.tipid = d.tipid and t.priid = d.priid
			inner join demandas.tiposervico ts ON ts.tipid = d.tipid
			inner join workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35) and doc.esdid in (1336) --aguardando pagamento
			inner join workflow.historicodocumento a ON a.hstid = doc.hstid
			inner join demandas.memorandodemanda md ON md.dmdid = d.dmdid and md.memid = ".$memid."
			inner join demandas.memorando mem ON mem.memid = md.memid and mem.memstatus='E'
			where dmdstatus='A'
			and ts.ordid = ".$ordid."
			and to_char(a.htddata, 'YYYY') = '".$ano."'
			and to_char(a.htddata, 'MM') = '".$mesQ."'
			order by 1
			";	
	$dados = (array) $db->carregar($sql);
	
	if(!$dados[0]){
		echo '<script>
					alert("Não existem demandas a serem pagas!");
					location.href="demandas.php?modulo=principal/memorando&acao=A";
			  </script>';
		die;
	}
	
	$cmddsc = "Trâmite automático de pagamento.";
	
	for($i=0; $i<=count($dados)-1;$i++)
	{
		$docid = $dados[$i]['docid'];
		$esdid = $dados[$i]['esdid'];
		
		switch($esdid) {
			case '1336': //em aguardando pagamento p/ pago - esdid=1336&aedid=3104
				$aedid = 3104;
				break;
			default:
				$aedid = 0;
		}
		
		$dadosVerificacao = (array) "a:1:{s:6:\"unicod\";s:0:\"\";}";
		
		$_SESSION['flagpagamentoembloco'] = "OK";
		
		if($aedid != 0){
			wf_alterarEstado( $docid, $aedid, $cmddsc, $dadosVerificacao );
		}
		
		
	}
	
	unset($_SESSION['flagpagamentoembloco']);
	
	$sql = "UPDATE demandas.memorando SET memstatus='P' WHERE memid=".$_REQUEST['pagarMemo'];
	$db->executar($sql);
	$db->commit();
	
	echo '<script>
			alert("Memorando pago com sucesso!");
			location.href="demandas.php?modulo=principal/memorando&acao=A";
		  </script>';
	die;
}

if($_REQUEST['excluirMemo']){

	$sql = "DELETE FROM demandas.memorandodemanda WHERE memid=".$_REQUEST['excluirMemo'];
	$db->executar($sql);

	$sql = "UPDATE demandas.memorando SET memstatus='I' WHERE memid=".$_REQUEST['excluirMemo'];
	$db->executar($sql);
	$db->commit();
	
	echo '<script>
			alert("Memorando excluído com sucesso!");
			location.href="demandas.php?modulo=principal/memorando&acao=A";
		  </script>';
	die;
}


if($_POST['requisicao'] == 'emitirMemorando'){

	if(!$_POST['chkdmdid'][0]){
		echo '<script>
					alert("É necessário informar os números das demandas!");
				  </script>';
	}else {

		//verifica se existe ss e os
		$sql = "select count(odsid) as total
				from fabrica.ordemservico
				where scsid = " . $_POST['memss'] . "
				and odsid = " . $_POST['memos'];
		$totalOS = $db->pegaUm($sql);

		if ($totalOS == 0) {
			echo '<script>
					alert("Números da SS e OS são inválidos!");
					//location.href="demandas.php?modulo=principal/memorando&acao=A";
				  </script>';
			//die;
		} else {

			$mesQ = $_POST['memmes'];
			if (strlen($mesQ) == 1) $mesQ = "0" . $mesQ;

			$sql = "select
										   COALESCE( sum(d.dmdqtde * t.tsppontuacao) , 0) as total
									from demandas.demanda d
									inner join demandas.tiposervicoprioridade t ON t.tipid = d.tipid and t.priid = d.priid
									inner join demandas.tiposervico ts ON ts.tipid = d.tipid
									inner join workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35) and doc.esdid in (1336) --aguardando pagamento
									inner join workflow.historicodocumento a ON a.hstid = doc.hstid
									where dmdstatus='A'
									and ts.ordid = " . $_POST['ordid'] . "
									and to_char(a.htddata, 'YYYY') = '" . $_POST['memano'] . "'
									and to_char(a.htddata, 'MM') = '" . $mesQ . "'
									and d.dmdid in (".implode(',',$_POST['chkdmdid']).")";
			$memqtdust = $db->pegaUm($sql);
			if (!$memqtdust) $memqtdust = 0;

			if(!$_POST['memid']) {
				$sql = "INSERT INTO demandas.memorando(vcmid, usucpfresp, usucpfcad, ordid, memmes, memano, memnumero, memss, memos, memdatainclusao, memtexto,
													memqtdust, memvlust, memstatus)
					VALUES (" . $_POST['vcmid'] . ",
							'" . $_POST['usucpfresp'] . "',
							'" . $_SESSION['usucpf'] . "',
							" . $_POST['ordid'] . ",
							" . $_POST['memmes'] . ",
							" . $_POST['memano'] . ",
							'" . $_POST['memnumero'] . "',
							'" . $_POST['memss'] . "',
							'" . $_POST['memos'] . "',
							'" . date('Ymd') . "',
							'" . $_POST['textoMemorando'] . "',
							" . $memqtdust . ",
							" . ($_POST['memvlust'] ? "'" . str_replace(",", ".", str_replace(".", "", $_POST['memvlust'])) . "'" : 'null') . ",
							'G'
							) RETURNING memid";
				$memid = $db->pegaUm($sql);
			}else{
				$memid = $_POST['memid'];
				$sql = "UPDATE demandas.memorando
						SET usucpfresp='" . $_POST['usucpfresp'] . "',
						 	ordid=" . $_POST['ordid'] . ",
							memnumero='" . $_POST['memnumero'] . "',
							memss='" . $_POST['memss'] . "',
							memos='" . $_POST['memos'] . "',
							memmes='" . $_POST['memmes'] . "',
							memano='" . $_POST['memano'] . "',
							memtexto='" . $_POST['textoMemorando'] . "',
							memqtdust=" . $memqtdust . ",
							memvlust=" . ($_POST['memvlust'] ? "'" . str_replace(",", ".", str_replace(".", "", $_POST['memvlust'])) . "'" : 'null') . ",
							vcmid=" . $_POST['vcmid'] . "
						WHERE memid = ". $memid;
				$db->executar($sql);
			}

			//deleta todas as demandas vinculadas
			if($_POST['memid']) {
				$sqle = "DELETE FROM demandas.memorandodemanda WHERE memid = ".$_POST['memid'];
				$db->executar($sqle);
			}


			if($_POST['chkdmdid'][0]){
				$sqlmd = "";

				foreach($_POST['chkdmdid'] as $v){
					$sqlmd .= " INSERT INTO demandas.memorandodemanda(memid, dmdid) VALUES ($memid, $v); ";
				}

				if($sqlmd){
					$db->executar($sqlmd);
				}
			}

			$db->commit();

			echo '<script>
					//alert("Memorando emitido com sucesso!");
					//location.href="demandas.php?modulo=principal/memorando&acao=A";
					popup1 = window.open(
												"demandas.php?modulo=principal/memorando&acao=A&exibirmemo=' . $memid . '",
												"_blank",
												"width=1200,height=750,scrollbars=yes,scrolling=no,resizebled=no"
											);
					popup1.opener.notclose = popup1;
					location.href="demandas.php?modulo=principal/memorando&acao=A";
				  </script>';
			die;
		}

	}

}

include_once APPRAIZ . 'includes/cabecalho.inc';

print "<br>";


monta_titulo( "Memorando", "" );

$vcmidx = $db->pegaUm("SELECT  vcmid
								FROM fabrica.vigenciacontratometricaitem vi
								inner join fabrica.metricaitem mi on mi.mtiid = vi.mtiid
								inner join fabrica.vigenciacontrato vc on vc.vgcid = vi.vgcid
								inner join fabrica.contrato ct on ct.ctrid = vc.ctrid
								inner join entidade.entidade et on et.entid = ct.entidcontratado
								WHERE vc.ctrid = 6 and vi.vgcid = 6 and vi.mtiid in (3)");

$vcmid = $_POST['vcmid'] ? $_POST['vcmid'] : $vcmidx;

if($vcmid){

	$sql = "SELECT  ctrnumero, entnome, vcmvalorcontratado as memvlust
							FROM fabrica.vigenciacontratometricaitem vi
							inner join fabrica.metricaitem mi on mi.mtiid = vi.mtiid
							inner join fabrica.vigenciacontrato vc on vc.vgcid = vi.vgcid
							inner join fabrica.contrato ct on ct.ctrid = vc.ctrid
							inner join entidade.entidade et on et.entid = ct.entidcontratado
							WHERE vi.vcmid = $vcmid";
	$dadosContrato = $db->pegaLinha($sql);
	if($dadosContrato) extract($dadosContrato);


	$textoPadraox = '<div style="font-size:12pt;"><p>Ao, </p>
					<p>Coordenador de Recursos e Tecnologia da Informação - CRTI </p>
					<p>&nbsp;</p>
					<p>
						Assunto:
						<strong>Encaminhamento de ordens de serviço para pagamento </strong>
					</p>
					<p>&nbsp;</p>
					<ol>
						<li>
							Solicitamos providências referentes aos faturamentos das ordens de serviços listadas na
							tabela anexo para a empresa '.$entnome.'. Como sustentação destas,
							encaminhamos anexos os Termos de Abertura da Ordem de Serviço e os respectivos Recebimentos
							Provisórios.<br><br>
						</li>
						<li>
							Pagamento do tipo Custeio/'.date("Y").' referente ao item 2 (Sustentação de Sistemas) do grupo 01 do
							contrato '.$ctrnumero.'.<br><br>
						</li>
						<li>
							Por fim, coloco-me à disposição para os esclarecimentos adicionais eventualmente
							necessários.
						</li>
					</ol>
					<p>&nbsp;</p></div>';

}
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet">
<script type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>

<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>



<script type="text/javascript">
    
	//Editor de textos
	tinyMCE.init({
		mode : "textareas",
		theme : "advanced",
		plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
		theme_advanced_buttons1_add_before : "save,newdocument,separator",
		theme_advanced_buttons1_add : "fontselect,fontsizeselect",
		theme_advanced_buttons2_add : "separator,insertdate,inserttime,preview,zoom,separator,forecolor,backcolor",
		theme_advanced_buttons2_add_before: "cut,copy,paste,pastetext,pasteword,separator,search,replace,separator",
		theme_advanced_buttons3_add_before : "tablecontrols,separator",
		theme_advanced_buttons3_add : "emotions,iespell,flash,advhr,separator,print,separator,ltr,rtl,separator,fullscreen",
		theme_advanced_toolbar_location : "top",
		theme_advanced_toolbar_align : "left",
		theme_advanced_path_location : "bottom",
		plugin_insertdate_dateFormat : "%d-%m-%Y",
	    plugin_insertdate_timeFormat : "%H:%M:%S",
		extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
		external_link_list_url : "example_link_list.js",
		external_image_list_url : "example_image_list.js",
		flash_external_list_url : "example_flash_list.js",
		file_browser_callback : "fileBrowserCallBack",
		//language : "pt_br",
		entity_encoding : "raw",
		width : "90%",
	    height : "500px"
	});
                
                
</script>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script>

function emitirMemo(){
        var erro;
        var radio_entro;
        var check_entro;
        var campos = '';

        $.each($(".obrigatorio"), function(i, v){
            if( ($(this).attr('type') != 'radio') && ($(this).attr('type') != 'checkbox') ){
                if ( $(v).val() == '' ){
                    erro = 1;
                    campos += '- ' + $(this).attr('title') + " \n";
                }
            }else
                if( $(this).attr('type') == 'radio' ){
                    var name  = $(this).attr('name');
                    var value = $(this).attr('value');
                    var radio_box = $('input:radio[name='+name+']:checked');

                    if(!radio_box.val() && (name != radio_entro) ){
                        erro = 1;
                        radio_entro = name;
                        campos += '- ' + $(this).attr('title') + " \n";
                    }
            }else
                if( $(this).attr('type') == 'checkbox' ){
                    var name  = $(this).attr('name');
                    var value = $(this).attr('value');
                    var check_box = $('input:checkbox[name='+name+']:checked');

                    if(!check_box.val() && (name != check_entro) ){
                        erro = 1;
                        check_entro = name;
                        campos += '- ' + $(this).attr('title') + " \n";
                    }
            }
        });

        if( erro > 0 ){
            alert('Existem "campos obrigatórios" vazios! São eles: \n'+campos);
            return false;
        }
        
        if(!erro){
			document.formulario.exibirpdf.value = '';
			document.formulario.target = '';
            $('#requisicao').val('emitirMemorando');
            $('#formulario').submit();
        }

    }

	function emiteMemo(id){
		if(confirm('Deseja emitir este memorando?')){
			location.href="demandas.php?modulo=principal/memorando&acao=A&icone=ok&emitirmemo="+id;
		}
	}

    
    function excluirMemo(id){
    	if(confirm('Deseja excluir este memorando?')){
    		location.href="demandas.php?modulo=principal/memorando&acao=A&excluirMemo="+id;
    	}
    }

	function editarMemo(id){
		location.href="demandas.php?modulo=principal/memorando&acao=A&editarMemo="+id;
	}
    
    function pagarMemo(id){
    	if(confirm('Deseja pagar este memorando?')){
    		location.href="demandas.php?modulo=principal/memorando&acao=A&pagarMemo="+id;
    	}
    }
    
    function emitirPdf(id){
    	
    	var formulario = document.formulario;
    	formulario.exibirpdf.value = id;
    	formulario.target = 'emitirPdf';
		var janela = window.open( '', 'emitirPdf', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
		//jQuery('#dialogAjax').show();
		formulario.submit();
		
    	//location.href="demandas.php?modulo=principal/memorando&acao=A&excluirMemo="+id;
    }
    
    function filtroGeral(id){

    	var formulario = document.formulario;

		/*
		if(formulario.ordid.value==''){
			alert('Informe a Origem');
			formulario.ordid.focus();
			return false;
		}
		if(formulario.memmes.value==''){
			alert('Informe o Mês');
			formulario.memmes.focus();
			return false;
		}
		if(formulario.memano.value==''){
			alert('Informe a Ano');
			formulario.memano.focus();
			return false;
		}
		if(formulario.vcmid.value==''){
			alert('Informe o Contrato');
			formulario.vcmid.focus();
			return false;
		}
		*/

		if(formulario.ordid.value!='' && formulario.memmes.value!='' && formulario.memano.value!='' && formulario.vcmid.value!='') {
			formulario.exibirpdf.value = '';
			formulario.target = '';
			formulario.submit();
		}
    }


	function marcatodas(){

		document.getElementById('mdtodos').innerHTML = '<a href="javascript:desmarcatodas();"> - Desmarcar todos</a>';

		var obj = document.getElementsByTagName("input");
		var ct = 0;
		for(i=0; i<obj.length; i++) {

			if(obj[i].type=='checkbox'){

				obj[i].checked = true;

			}

		}

	}

	function desmarcatodas(){

		document.getElementById('mdtodos').innerHTML = '<a href="javascript:marcatodas();"> - Marcar todos</a>';

		var obj = document.getElementsByTagName("input");
		var ct = 0;
		for(i=0; i<obj.length; i++) {

			if(obj[i].type=='checkbox'){

				obj[i].checked = false;

			}

		}
	}

</script>

<?
if($_REQUEST['editarMemo']) {
	$sql = "
        SELECT  memid,
        		vcmid,
        		memnumero,
                memdatainclusao,
                memmes,
                memano,
                memss,
                memos,
       			memqtdust,
       			memvlust,
       			ordid,
       			memstatus,
       			memtexto
		FROM demandas.memorando
        WHERE memid = " . $_REQUEST['editarMemo'];
	$dadosMemo = $db->pegaLinha($sql);

	if($dadosMemo) extract($dadosMemo);
}
?>

<form id="formulario" name="formulario" method="post">

	<input type="hidden" id="requisicao" name="requisicao" value="">
	<input type="hidden" id="exibirpdf" name="exibirpdf" value="">
	<input type="hidden" id="memid" name="memid" value="<?=$memid?>">

    <table class="tabela listaAtualizacao" cellspacing="1" cellpadding="3" align="center" border="0">
		<tr>
			<td class="SubTituloDireita">Origem:</td>
			<td>
				<?php
				$ordid = $_REQUEST['ordid'] ? $_REQUEST['ordid'] : $ordid;
				$sql = " SELECT
							  ordid AS codigo,
						  	  orddescricao as descricao
							 FROM
							  demandas.origemdemanda
							 WHERE
							  ordstatus = 'A'
							  and ordid in (18,21)
							  order by 2";
				$db->monta_combo( "ordid", $sql, 'S', 'Selecione a Origem...', 'filtroGeral', '','','320','S','ordid','',$ordid,'Origem');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Mês/Ano do Memorando:</td>
			<td>
				<?php
				//$data = $data;
				//echo campo_data2( 'memdatainclusao', 'S', 'S', 'Data do Memorando', 'S', '', '', $memdatainclusao, '', '', 'memdatainclusao' );
				?>
				<?php
				$memmes = $_REQUEST['memmes'] ? $_REQUEST['memmes'] : $memmes;
				$memano = $_REQUEST['memano'] ? $_REQUEST['memano'] : $memano;
				$sql = "select '1' as codigo, 'Janeiro' as descricao
	  	   						union all
	  	   						select '2' as codigo, 'Fevereiro' as descricao
	  	   						union all
	  	   						select '3' as codigo, 'Março' as descricao
	  	   						union all
	  	   						select '4' as codigo, 'Abril' as descricao
	  	   						union all
	  	   						select '5' as codigo, 'Maio' as descricao
	  	   						union all
	  	   						select '6' as codigo, 'Junho' as descricao
	  	   						union all
	  	   						select '7' as codigo, 'Julho' as descricao
	  	   						union all
	  	   						select '8' as codigo, 'Agosto' as descricao
	  	   						union all
	  	   						select '9' as codigo, 'Setembro' as descricao
	  	   						union all
	  	   						select '10' as codigo, 'Outubro' as descricao
	  	   						union all
	  	   						select '11' as codigo, 'Novembro' as descricao
	  	   						union all
	  	   						select '12' as codigo, 'Dezembro' as descricao
	  	   						";

				$db->monta_combo( 'memmes', $sql, 'S', 'Mês...','filtroGeral', '', '','', 'S', 'memmes', '', $memmes, 'Mês' );
				?>
				/
				<?php
				$anoini = (int) 2014;
				$anofim = (int) date("Y");

				for($i=$anoini; $i<=$anofim; $i++){
					$sqlano .= " select '$i' as codigo, '$i' as descricao ";
					if($i != $anofim) $sqlano .= " union ";
				}
				$sqlano .= " order by 1 ";
				$db->monta_combo( 'memano', $sqlano, 'S', 'Ano...', 'filtroGeral', '', '', '', 'S', 'memano', '', $memano, 'Ano' );

				?>
			</td>
		</tr>
		<tr>
            <td class="SubTituloDireita">Contrato:</td>
            <td>
				<?php 
						$sql = "SELECT  vcmid as codigo,
										ctrnumero ||' - '|| entnome ||' - '|| mtinome ||' - R$ '|| vcmvalorcontratado as descricao
								FROM fabrica.vigenciacontratometricaitem vi
								inner join fabrica.metricaitem mi on mi.mtiid = vi.mtiid
								inner join fabrica.vigenciacontrato vc on vc.vgcid = vi.vgcid
								inner join fabrica.contrato ct on ct.ctrid = vc.ctrid
								inner join entidade.entidade et on et.entid = ct.entidcontratado
								WHERE vc.ctrid = 6 and vi.vgcid = 6 and vi.mtiid in (3)";
					
						$db->monta_combo( 'vcmid', $sql, 'S', 'Contrato...','filtroGeral', '', '','', 'S', 'vcmid', '', $vcmid, 'Contrato' );
					?>		
            </td>
        </tr>
		<!--
		<tr style="BACKGROUND-COLOR: #dcdcdc;">
			<td></td>
			<td>
				<input type="button" id="pesquisar" name="pesquisar" value="Pesquisar" onclick="filtroGeral();">
			</td>
		</tr>

        <tr>
            <td class="SubTituloDireita">Quantidade UST:</td>
            <td>
                <? 
                //echo campo_texto( 'memqtdust', 'S', 'S', "Quantidade UST", 11, '', '[#]', '', 'right', '', 0, 'id="memqtdust"', '', $memqtdust, '', '' );
                ?>
            </td>
        </tr>
        -->
        <tr>
            <td class="SubTituloDireita">Valor da UST:</td>
            <td>
                <? 
                if($vcmid){
                	/*
	                $sql = "SELECT vcmvalorcontratado FROM fabrica.vigenciacontratometricaitem 
							WHERE vgcid = 6 and	mtiid = 3";
					
                	$sql = "SELECT vcmvalorcontratado FROM fabrica.vigenciacontratometricaitem 
							WHERE vcmid = $vcmid";
				    $memvlust = $db->pegaUm( $sql );
					*/
                }
                echo campo_texto( 'memvlust', 'S', 'N', "Valor da UST", 11, '', '###.###.###,##', '', 'right', '', 0, 'id="memvlust"', '', number_format($memvlust,2,',','.'), '', '' );
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Nº do Memorando:</td>
            <td>
                <?
				$memnumero = $_REQUEST['memnumero'] ? $_REQUEST['memnumero'] : $memnumero;
				echo campo_texto( 'memnumero', 'S', 'S', "Nº do Memorando", 11, '', '[#]', '', 'right', '', 0, 'id="memnumero"', '', $memnumero, '', '' );
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Nº da SS:</td>
            <td>
                <?
				$memss = $_REQUEST['memss'] ? $_REQUEST['memss'] : $memss;
				echo campo_texto( 'memss', 'S', 'S', "Nº da SS", 11, '', '[#]', '', 'right', '', 0, 'id="memss"', '', $memss, '', '' );
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Nº da OS:</td>
            <td>
                <?
				$memos = $_REQUEST['memos'] ? $_REQUEST['memos'] : $memos;
				echo campo_texto( 'memos', 'S', 'S', "Nº da OS", 11, '', '[#]', '', 'right', '', 0, 'id="memos"', '', $memos, '', '' );
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Responsável pelo Memorando:</td>
            <td>
                  <?php
                    $usucpfresp = $_REQUEST['usucpfresp'] ? $_REQUEST['usucpfresp'] : $_SESSION['usucpf'];
                    
                    if($db->testa_superuser()){
                    	$usucpf = $_SESSION['usucpf'];
                    }else{
                    	$usucpf = '';
                    }
                                        
					$sql = " SELECT
							  u.usucpf AS codigo, 
						  	  u.usunome as descricao 
							 FROM
							  seguranca.usuario u
							 INNER JOIN seguranca.perfilusuario p ON p.usucpf=u.usucpf
							 INNER JOIN seguranca.perfil pr ON pr.pflcod=p.pflcod and pr.sisid=44 and pr.pflcod=214
							 WHERE
							  u.suscod = 'A'
							  --and u.usucpf in ('".$usucpf."')
							  order by 2";
					$db->monta_combo( "usucpfresp", $sql, 'S', 'Selecione o Responsável...', '', '','','320','S','usucpfresp','',$usucpfresp,'Responsável pelo Memorando');
					?>  
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Texto do Memorando:</td>
            <td>
                <?php
                $numeroDeLinhas   = 200;
                $numeroDeColunas  = 10;
                $maximoCaracteres = 2000;
                ?>
                <span id="textAreaDinamico">
                <?php echo campo_textarea( 'textoMemorando', 'S', $habil, 'Texto do Memorando', $numeroDeLinhas, $numeroDeColunas, $maximoCaracteres, null, null, null, null, 'Texto do Memorando', $textoPadraox ); ?>
                </span>
            </td>
        </tr>
		<tr>
			<td class="SubTituloDireita">Nº de Demandas:</td>
			<td>
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr>
						<th>
							<font color="#00008B">Lista (Aguardando Pagamento)</font>
						</th>
					</tr>
					<tr>
						<td valign="top">
							<div style="margin: 0 auto; padding: 0; height: 200px; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="lista_demandas">
								<?
								if($vcmid && $memmes && $memano && $ordid){


									$mesQ = $memmes;
									if(strlen($mesQ) == 1) $mesQ = "0".$mesQ;

									if($memid) {
										$andMemid = " and  md.memid not in ($memid) ";
									}
									$sql = "select d.dmdid,
											   d.dmdtitulo,
											   to_char(d.dmddatainclusao, 'DD/MM/YYYY') AS datainclusao,
											   to_char(a.htddata, 'DD/MM/YYYY') AS dtsit,
											   d.dmdqtde,
											   t.tsppontuacao
												from demandas.demanda d
												inner join demandas.tiposervicoprioridade t ON t.tipid = d.tipid and t.priid = d.priid
												inner join demandas.tiposervico ts ON ts.tipid = d.tipid
												inner join workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35) and doc.esdid in (1336)
												inner join workflow.historicodocumento a ON a.hstid = doc.hstid
												where dmdstatus='A'
												and ts.ordid = " . $ordid . "
												and to_char(a.htddata, 'YYYY') = '" . $memano . "'
												and to_char(a.htddata, 'MM') = '" . $mesQ . "'
												and d.dmdid not in (SELECT dmdid FROM demandas.memorandodemanda md
																	inner join demandas.memorando m on m.memid = md.memid and m.memstatus in ('E','P','G')
																	$andMemid
																	)
												order by 1";
									$dadosDemandas = $db->carregar($sql);


									$html2 = '<table width="100%">
													<TR style="background:#D9D9D9;">
														<TD align="center" valign="top" style="font-weight:bold;"><div id="mdtodos" class="SubtituloEsquerda"	style="padding: 0; margin: 0 auto; text-align: center; width: 100%"><a href="javascript:marcatodas();"> - Marcar todos</a></div></TD>
														<TD align="center" valign="top" style="font-weight:bold;">Nº Demanda</TD>
														<TD align="center" valign="top" style="font-weight:bold;">Assunto</TD>
														<TD align="center" valign="top" style="font-weight:bold;">Dt. Abertura</TD>
														<TD align="center" valign="top" style="font-weight:bold;">Dt. Situação</TD>
														<TD align="center" valign="top" style="font-weight:bold;">Qt. Serviço</TD>
														<TD align="center" valign="top" style="font-weight:bold;">Pontuação</TD>
														<TD align="center" valign="top" style="font-weight:bold;">Total</TD>
													</TR>';



									if($dadosDemandas){
												foreach($dadosDemandas as $v){

													//verifica se existe a demanda no memorando
													$checado = 0;
													if($memid) {
														$sql = "SELECT count(dmdid) FROM demandas.memorandodemanda
															where memid = {$memid}
															and dmdid = " . $v["dmdid"];
														$checado = $db->pegaUm($sql);
													}

													$html2 .= '<tr bgcolor="#E5E5E5">
																	<td align="center"  style="font-size:12px;color:#000000;">';
																		if($checado == 1) {
																			$html2 .= '<input type = "checkbox" name = "chkdmdid[]" id = "chk'.$v["dmdid"].'" value = "'.$v["dmdid"].'" checked>';
																		}else{
																			$html2 .= '<input type = "checkbox" name = "chkdmdid[]" id = "chk'.$v["dmdid"].'" value = "'.$v["dmdid"].'">';
																		}
													$html2 .= '	    </td>
																	<td align="center"  style="font-size:12px;color:#000000;">
																		'.$v["dmdid"].'
																	</td>
																	<td align="left"  style="font-size:12px;color:#000000;">
																		'.$v["dmdtitulo"].'
																	</td>
																	<td align="center"  style="font-size:12px;color:#000000;">
																		'.$v["datainclusao"].'
																	</td>
																	<td align="center"  style="font-size:12px;color:#000000;">
																		'.$v["dtsit"].'
																	</td>
																	<td align="center"  style="font-size:12px;color:#000000;">
																		'.$v["dmdqtde"].'
																	</td>
																	<td align="center"  style="font-size:12px;color:#000000;">
																		'.( (int) $v["tsppontuacao"]).'
																	</td>
																	<td align="center"  style="font-size:12px;color:#000000;">
																		'.( (int) $v["dmdqtde"] * (int) $v["tsppontuacao"]).'
																	</td>
																</tr>';
												}
											}
											else{
												$html2 .= '<TR style="background:#FFFFFF;">
																<TD colspan="8" align="center" style="font-weight:bold; font-size:12px;"><font color="red">Não existem demandas com a situação Aguardando Pagamento.</font></TD>
														   </tr>';
											}

											$html2 .= '<TR style="background:#FFFFFF;">
															<TD colspan="8" align="left" style="font-weight:bold; font-size:9px; border-top:2px solid black; border-bottom:2px solid black;">Total de demandas: '.($dadosDemandas ? count($dadosDemandas) : 0).'</TD>
											 		  </tr>
													</table>';

									echo $html2;

								}else{
									echo '<center><b><font color="red">Informe a Origem, Mês, Ano e Contrato.</font></b></center>';
								}

								?>
							</div>
						</td>
					</tr>
				</table>
			</td>
		</tr>
        <tr style="BACKGROUND-COLOR: #dcdcdc;">
            <td></td>
            <td>
				<?if($memid){?>
                	<input type="button" id="gerar" name="gerar" value="Alterar Memorando" onclick="emitirMemo();">
					&nbsp;
					<input type="button" id="novo" name="Novo" value="Novo" onclick="location.href='demandas.php?modulo=principal/memorando&acao=A';">
				<?}else{?>
					<input type="button" id="gerar" name="gerar" value="Gerar Memorando" onclick="emitirMemo();">
				<?}?>
            </td>
        </tr>
    </table>

	<!--
    <center>
		<div id="dialogAjax" style="display: none" >
			<br>
		    <img src="../imagens/wait.gif" alt="ajax">Aguarde! Gerando PDF...
		    <br><br><br>
		</div>
	</center>
	-->
 
</form>

<?PHP
	/*
    $acao = "
        <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"buscarDadosEditarMantida('||m.memid||');\" title=\"Editar Professor\" >
        <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirDadosMantida('||m.memid||');\" title=\"Editar Professor\" >
    ";
    */

	unset($_POST['textoMemorando']);

	$acaoG = "
			<img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"editarMemo('||m.memid||');\" title=\"Editar Memorando\" >
			&nbsp;
			<img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirMemo('||m.memid||');\" title=\"Excluir Memorando\" >
			&nbsp;
			<img align=\"absmiddle\" src=\"/imagens/incluietapa.gif\" width=\"13px\" height=\"16px\" style=\"cursor: pointer\" onclick=\"emiteMemo('||m.memid||');\" title=\"Emitir Memorando\" >
			&nbsp;
			<img align=\"absmiddle\" src=\"/imagens/consultar.gif\" width=\"13px\" height=\"16px\" style=\"cursor: pointer\" onclick=\"emitirPdf('||m.memid||');\" title=\"Consultar Memorando\" >
			";

	$acaoE = "
			<img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirMemo('||m.memid||');\" title=\"Excluir Memorando\" >
			&nbsp;
			<img align=\"absmiddle\" src=\"/imagens/consultar.gif\" width=\"13px\" height=\"16px\" style=\"cursor: pointer\" onclick=\"emitirPdf('||m.memid||');\" title=\"Consultar Memorando\" >
			&nbsp;
			<img align=\"absmiddle\" src=\"/imagens/money_g.gif\" width=\"13px\" height=\"16px\" style=\"cursor: pointer\" onclick=\"pagarMemo('||m.memid||');\" title=\"Pagar Memorando\" >
			";
	
	$acaoP = "
			<img align=\"absmiddle\" src=\"/imagens/consultar.gif\" width=\"13px\" height=\"16px\" style=\"cursor: pointer\" onclick=\"emitirPdf('||m.memid||');\" title=\"Consultar Memorando\" >
			";

    $sqlLista = "
        SELECT  case when memstatus = 'P' then
        				'{$acaoP}'
					when memstatus = 'G' then
        				'{$acaoG}'
        			else
        				'{$acaoE}'
        		end as acao,
        		case when memstatus = 'P' then
        			 '<font color=blue><b>PAGO</b></font>'
        			when memstatus = 'G' then
        				'<b>NÃO EMITIDO</b>'
        			else
        				'<b>EMITIDO</b>'
        		end as sit,
                memnumero as numero,
                --to_char(memdatainclusao, 'DD/MM/YYYY'), 
                case when memmes = 1 then 'Janeiro' 
                	 when memmes = 2 then 'Fevereiro'
                	 when memmes = 3 then 'Março'
                	 when memmes = 4 then 'Abril'
                	 when memmes = 5 then 'Maio'
                	 when memmes = 6 then 'Junho'
                	 when memmes = 7 then 'Julho'
                	 when memmes = 8 then 'Agosto'
                	 when memmes = 9 then 'Setembro'
                	 when memmes = 10 then 'Outubro'
                	 when memmes = 11 then 'Novembro'
                	 when memmes = 12 then 'Dezembro'
                end ||'/'|| memano as mesano,
               orddescricao as origem,
                memss as ss,
                memos as os,
       			memqtdust as qtd,
       			memvlust as vl,
       			usunome as nome
		FROM demandas.memorando m
        INNER JOIN seguranca.usuario AS u ON u.usucpf = m.usucpfresp
        INNER JOIN demandas.origemdemanda AS o ON o.ordid = m.ordid
        WHERE memstatus not in ('I')"; //I=INATIVO - G=GERADO
    	
    $cabecalho = array("Ação", "Situação", "Nº do Memorando", "Mês/Ano", "Origem", "Nº SS", "Nº OS", "Qtd. UST", "Valor UST", "Responsável");
    $alinhamento = Array('center','center','center','center','left','center','center','center','center');
    $tamanho = Array('10%');
    $db->monta_lista($sqlLista, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
?>

<?
function mesExtenso($mes) {

	switch($mes){
		case 1: $mes = "Janeiro"; break;
		case 2: $mes = "Fevereiro"; break;
		case 3: $mes = "Março"; break;
		case 4: $mes = "Abril"; break;
		case 5: $mes = "Maio"; break;
		case 6: $mes = "Junho"; break;
		case 7: $mes = "Julho"; break;
		case 8: $mes = "Agosto"; break;
		case 9: $mes = "Setembro"; break;
		case 10: $mes = "Outubro"; break;
		case 11: $mes = "Novembro"; break;
		case 12: $mes = "Dezembro"; break;
	}
	
	return $mes;
}
?>


          
<html>
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
	    <meta http-equiv="Pragma" content="no-cache">
	    <meta http-equiv="Connection" content="Keep-Alive">
	    <meta http-equiv="Expires" content="Fri, 9 Oct 1998 05:00:00 GMT">
	    <title>Informaçõe sobre as Obras PAC2</title>
	    
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	    <script src="../includes/funcoes.js"> </script>
	    <script>
	    function abrirObrasPac2(proid) {
	    	window.open('callcenter.php?modulo=principal/listaObrasPac2&acao=A&proid='+proid,'Observações','scrollbars=no,height=300,width=500,status=no,toolbar=no,menubar=no,location=no');
	    }
	    </script>
	</head>
	<body>
	<?
	$sql = "SELECT m.mundescricao, p.pronumeroprocesso, m.muncod, m.estuf FROM par.processoobra p 
			LEFT JOIN territorios.municipio m ON m.muncod = p.muncod  
			WHERE p.prostatus = 'A' and p.proid='".$_REQUEST['proid']."'";
	
	$processo = $db->pegaLinha($sql);
	
	$sql_pref = "SELECT
				max(ent.entid) as entid1				
			FROM entidade.entidade ent
				INNER JOIN entidade.endereco 		eed2 ON eed2.entid = ent.entid
				INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = 1 AND fue.fuestatus = 'A'
				INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid				
			WHERE (ent.entstatus = 'A' OR ent.entstatus IS NULL)
			and eed2.muncod = '".$processo['muncod']."' and eed2.estuf = '".$processo['estuf']."'";
	
	$entPref = $db->pegaUm( $sql_pref );
	
	if( $entPref ){
		$sql = "SELECT DISTINCT
					func2.funid, ent2.entnumcpfcnpj, ent2.entnome, ent2.entnumdddresidencial, ent2.entnumresidencial, ent2.entnumdddcomercial,
				    ent2.entnumcomercial, ent2.entnumdddcelular, ent2.entnumcelular, ent2.entnumdddfax, ent2.entnumfax, ent2.entemail, ende.muncod
				FROM
					entidade.entidade ent
				    left join entidade.funentassoc assoc ON ent.entid = assoc.entid
					left join entidade.funcaoentidade func ON func.fueid = assoc.fueid
					left join entidade.entidade ent2
					    inner join entidade.funcaoentidade func2 ON func2.entid = ent2.entid and func2.funid in (2,15)
		                inner join entidade.endereco ende on ende.endid = ent2.entid
	                ON ent2.entid =  func.entid
				WHERE
					ent.entid = $entPref
	            ORDER BY func2.funid";
		
		$arDados = $db->carregar( $sql );
	}
	$arDados = $arDados ? $arDados : array();
	?>
	<table align="center" border="0" cellpadding="3" cellspacing="1" class="tabela">
	<tr>
		<td class="SubTituloCentro">Informações sobre as Obras PAC 2 do Município (<?=$processo['mundescricao'] ?>)<br> Processo : <?=$processo['pronumeroprocesso'] ?></td>
	</tr>
	</table>
	<table align="center" width="95%" border="0" cellpadding="3" cellspacing="1" class="tabela">
	<tr><td style="width: 50%">
		<? montaInfoMunicipio('Dados do Prefeito', $arDados[0]); ?>
		</td><td style="width: 50%">
		<? montaInfoMunicipio('Dirigente da Educação', $arDados[1]); ?>
	</td></tr></table>
	<br>
	<table align="center" border="0" width="95%" cellpadding="3" cellspacing="1" class="listagem">
	<tr>
		<td class="SubTituloCentro">Nome</td>
		<td class="SubTituloCentro">Tipo</td>
		<td class="SubTituloCentro">Situação</td>
		<td class="SubTituloCentro">Termo</td>
		<td class="SubTituloCentro">Informação do pagamento</td>
		<td class="SubTituloCentro">Banco, Agência e Conta</td>
		<td class="SubTituloCentro">Dados da Licitação</td>
		<td class="SubTituloCentro">Cadastro do Engenheiro</td>
	</tr>
	<?
	
	$sql = "SELECT pre.preid, pre.predescricao, pto.ptodescricao, esd.esddsc, pre.preid FROM par.empenhoobra emo 
			INNER JOIN obras.preobra pre ON pre.preid = emo.preid and eobstatus = 'A'
			LEFT JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid 
			LEFT JOIN workflow.documento doc ON doc.docid = pre.docid 
			LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			LEFT JOIN par.empenho emp ON emp.empid = emo.empid and empstatus = 'A'
			LEFT JOIN par.processoobra poo ON poo.pronumeroprocesso = emp.empnumeroprocesso and poo.prostatus = 'A'
			WHERE poo.proid='".$_REQUEST['proid']."'";
	
	$obras = $db->carregar($sql);

	if($obras[0]) {
		foreach($obras as $obra) {
			$sql = "SELECT 'Gerado<br>('||to_char(ter.terdatainclusao,'dd/mm/YYYY')||')' as c 
					FROM par.termoobra teo 
					LEFT JOIN par.termocompromissopac ter ON ter.terid = teo.terid 
					WHERE teo.preid='".$obra['preid']."'";
			$termo = $db->pegaUm($sql);
			
			/* Trecho que preenche a coluna pagamento */
			$sql = "SELECT pro.nu_conta_corrente, pro.proagencia, pro.probanco, pag.pagvalorparcela, pag.parnumseqob, to_char(pag.pagdatapagamento,'dd/mm/YYYY') as pagdatapagamento 
					FROM par.empenhoobra emo 
					INNER JOIN par.empenho emp ON emp.empid = emo.empid and empstatus = 'A' and eobstatus = 'A' 
					INNER JOIN par.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
					INNER JOIN par.pagamento pag ON pag.empid = emo.empid   
					WHERE emo.preid = '".$obra['preid']."' AND pag.pagstatus='A'";
			
			$pagamentoobra = $db->pegaLinha($sql);
			
			if($pagamentoobra) {
				$pagamentohtml = "<table width=100%>
								  <tr><td colspan=2>Pago</td></tr>
							  	  <tr><td>Valor pagamento(R$):</td><td>".number_format($pagamentoobra['pagvalorparcela'],2,",",".")."</td></tr>
							  	  <tr><td>Nº da Ordem Bancária:</td><td>".$pagamentoobra['parnumseqob']."</td></tr> 
							  	  <tr><td>Data do pagamento:</td><td>".$pagamentoobra['pagdatapagamento']."</td></tr>
							  	  </table>";
				
				$pagamentobancoagencconta = "Banco: ".$pagamentoobra['probanco'].", Agência: ".$pagamentoobra['proagencia'].", Conta: ".$pagamentoobra['nu_conta_corrente'];
				 
			} else {
				$pagamentohtml = "<table width=100%>
								  <tr><td>Não pago</td></tr>
								  </table>";
				
				$pagamentobancoagencconta = "Não cadastrado";

			}
			#Dados para preencher os dados da obras
			$sql = "SELECT 
						ent.entnome,
					    ent.entnumcpfcnpj,
					    case when ent.entnumcomercial is null then '('||ent.entnumdddcelular||') '||ent.entnumcelular 
					    	else '('||ent.entnumdddcomercial||') '||ent.entnumcomercial end as telefone,
					    ent.entnumdddcomercial, ent.entnumcomercial, ent.entnumdddcelular, ent.entnumcelular, ent.entnumdddfax, ent.entnumfax,
					    ent.entemail,
					    rc.tprcid
					FROM 
					  	obras.responsavelcontatos rc 
					    inner join entidade.entidade ent on ent.entid = rc.entid
					    inner join obras.responsavelobra ro on ro.recoid = rc.recoid
					    inner join obras.obrainfraestrutura oi on oi.obrid = ro.obrid
					WHERE
						rc.tprcid IN (6,7)
					    and rc.recostatus = 'A'
						and oi.preid = '".$obra['preid']."' ORDER BY rc.tprcid";
			$arObras = $db->carregar( $sql );
			$arObras = $arObras ? $arObras : array();
			
			#Dados para preencher os dados da licitação
			$sql = "SELECT a.tpaid, o.obrid, f.flchomlicdtprev, f.flcpubleditaldtprev, a.arqid, arq.arqdescricao||'.'||arq.arqextensao as arquivo 
					FROM obras.obrainfraestrutura o
						inner join obras.faselicitacao f on f.obrid = o.obrid and f.tflid in (2,9)
					    inner join obras.arquivosobra a on a.obrid = f.obrid --and a.aqostatus = 'A'
					    inner join public.arquivo arq on arq.arqid = a.arqid --and arq.arqstatus = 'A'
					WHERE o.preid = '".$obra['preid']."'
						and f.flcstatus = 'A'
					    and a.tpaid in (3,24)";
			$arLicitacao = $db->carregar( $sql );
			$arLicitacao = $arLicitacao ? $arLicitacao : array();
			
			if( $arLicitacao ){
				foreach ($arLicitacao as $v) {
					$data = $v['tpaid'] == 3 ? $v['flcpubleditaldtprev'] : $v['flchomlicdtprev'];
					$licitcaohtml .= "<table width=100%>
								  	  <tr><td align=right>Data:</td><td>".$data."</td></tr>
								  	  <tr><td align=right>Arquivo anexado:</td><td>".$v['arquivo']."</td></tr>
								  	  </table>";	
				}
			} else {
				$licitcaohtml = 'Não cadastrado';
			}
			
			if( $arObras ){
				foreach ($arObras as $v) {
					if($v['tprcid'] == 6) $titulo = 'Engenheiro';
					if($v['tprcid'] == 7) $titulo = 'Fiscal Instituição';
					$engenheirohtml .= "<table width=100%>
									  <tr><td colspan=2 align=center><b>".$titulo."</b></td></tr>
								  	  <tr><td align=right><b>Nome:</b></td><td>".$v['entnome']."</td></tr>
								  	  <tr><td align=right><b>CPF:</b></td><td>".$v['entnumcpfcnpj']."</td></tr> 
								  	  <tr><td align=right><b>Telefone:</b></td><td>".$v['telefone']."</td></tr>
								  	  <tr><td align=right><b>E-mail:</b></td><td>".$v['entemail']."</td></tr>
								  	  </table>";
				}
			} else {
				$engenheirohtml = 'Não cadastrado';
			}
			?>
			<tr>
				<td><?=$obra['predescricao'] ?></td>
				<td><?=$obra['ptodescricao'] ?></td>
				<td><?=$obra['esddsc'] ?></td>
				<td><?=(($termo)?$termo:"Não gerado") ?></td>
				<td><?=$pagamentohtml; ?></td>
				<td><?=$pagamentobancoagencconta ?></td>
				<td><?=$licitcaohtml; ?></td>
				<td><?=$engenheirohtml; ?></td>
			</tr>
			<?
		}
	}
	
	
	
	?>
	</table>

	</body>

</html>
<?
function montaInfoMunicipio($titulo = '', $arDados = array() ){
	$html = '<table align="center" border="0" cellpadding="3" cellspacing="1" class="tabela">
				<tr>
					<th colspan="2">'.$titulo.'</th>
				</tr>
				<tr>
					<td class="SubTituloDireita" width="25%">CPF:</td>
					<td>'.formatar_cpf_cnpj($arDados['entnumcpfcnpj']).'</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" width="25%">Nome:</td>
					<td>'.$arDados['entnome'].'</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Telefone(s):</td>
					<td><table align="center" border="0" cellpadding="3" cellspacing="1" class="tabela">
						<tr>
							<td class="SubTituloDireita" width="10%">Comercial:</td>
							<td>'.($arDados['entnumcomercial'] ? '('.trim($arDados['entnumdddcomercial']).') '.$arDados['entnumcomercial'] : '').'</td>
						</tr>
						<tr>
							<td class="SubTituloDireita" width="10%">Celular:</td>
							<td>'.($arDados['entnumcelular'] ? '('.trim($arDados['entnumdddcelular']).') '.$arDados['entnumcelular'] : '').'</td>
						</tr>
						<tr>
							<td class="SubTituloDireita" width="10%">Fax:</td>
							<td>'.($arDados['entnumfax'] ? '('.trim($arDados['entnumdddfax']).') '.$arDados['entnumfax'] : $arDados['entnumfax']).'</td>
						</tr>
						</table></td>
				</tr>
				<tr>
					<td class="SubTituloDireita">e-mail:</td>
					<td>'.$arDados['entemail'].'</td>
				</tr>
				</table><br>';
	echo $html;
}
?>
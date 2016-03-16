<?php
$usucpf = $_SESSION['usucpf'];

if( $_REQUEST['gdiid'] )
{
	$sql = "SELECT DISTINCT
				prc.prcid,
				prc.prcnumsidoc,
				case
				  when ent.entnome is null then '-'
				  else ent.entnome
                end as entnome,
				g.gdidsc,
				to_char(hst.htddata, 'DD/MM/YYYY') as data,
				esd.esdid,
				CASE 
					WHEN esd.esdid = 81 THEN coo.coosigla
					WHEN esd.esdid = 82 THEN ent.entnome
					ELSE esd.esddsc
				END as esddsc,
				aed.aeddscrealizada,
				esd2.esddsc as situacao_origem,
				hst.hstid,
				coo.coosigla,
				g.usucpf,
				to_char(g.gdidt,'DD/MM/YYYY') as dt,
				prc.prcnomeinteressado
			FROM
				conjur.processoconjur prc
			INNER JOIN conjur.itemguiadistribuicao   i ON i.prcid 	 = prc.prcid
			INNER JOIN conjur.guiadistribuicao       g ON g.gdiid	 = i.gdiid
			LEFT JOIN conjur.historicoadvogados    had ON had.prcid  = prc.prcid and had.hadid = (select max(hadid) from conjur.historicoadvogados where prcid = prc.prcid)
			LEFT  JOIN conjur.advogados 		   adv ON adv.advid  = had.advid
			LEFT  JOIN entidade.entidade		   ent ON ent.entid  = adv.entid
			LEFT  JOIN conjur.coordenacao          coo ON coo.coonid = prc.cooid
			INNER JOIN conjur.estruturaprocesso    esp ON esp.prcid  = prc.prcid
			INNER JOIN workflow.documento 	       doc ON doc.docid  = esp.docid
			INNER JOIN workflow.estadodocumento    esd ON esd.esdid  = doc.esdid
			INNER JOIN workflow.historicodocumento hst ON hst.docid  = doc.docid AND 
														  hst.hstid  = i.hstid
			INNER JOIN workflow.acaoestadodoc 	   aed ON aed.aedid  = hst.aedid
			INNER JOIN workflow.estadodocumento   esd2 ON esd2.esdid = aed.esdidorigem
			WHERE
				prc.prcstatus = 'A'
				AND g.gdiid = ".$_REQUEST['gdiid']."
			ORDER BY
				esd2.esddsc, coo.coosigla ASC";
	$dados = $db->carregar($sql);
	$usucpf = $dados[0]['usucpf'];
}

if( $_POST['guia'] )
{
	foreach($_POST['guia'] as $hstid => $prcid){
		$arhstid[] = $hstid;
	}
	$sql = "SELECT DISTINCT
				prc.prcid,
				prc.prcnumsidoc,
				case
				  when ent.entnome is null then '-'
				  else ent.entnome
                end as entnome,
				to_char(hst.htddata, 'DD/MM/YYYY') as data,
				esd.esdid,
				CASE 
					WHEN esd.esdid = 81 THEN coo.coosigla
					WHEN esd.esdid = 82 THEN ent.entnome
					ELSE esd.esddsc
				END as esddsc,
				aed.aeddscrealizada,
				esd2.esddsc as situacao_origem,
				hst.hstid,
				coo.coosigla,
				prc.prcnomeinteressado
			FROM
				conjur.processoconjur prc
			LEFT JOIN conjur.historicoadvogados    had ON had.prcid  = prc.prcid and had.hadid = (select max(hadid) from conjur.historicoadvogados where prcid = prc.prcid)
			LEFT  JOIN conjur.advogados 		   adv ON adv.advid  = had.advid
			LEFT  JOIN entidade.entidade		   ent ON ent.entid = adv.entid
			LEFT  JOIN conjur.coordenacao          coo ON coo.coonid = prc.cooid
			INNER JOIN conjur.estruturaprocesso    esp ON esp.prcid = prc.prcid
			INNER JOIN workflow.documento 	       doc ON doc.docid = esp.docid
			INNER JOIN workflow.estadodocumento    esd ON esd.esdid = doc.esdid
			INNER JOIN workflow.historicodocumento hst ON hst.docid = doc.docid AND 
                        hst.htddata = (
                            SELECT max(h.htddata) --, h.docid 
                            FROM workflow.historicodocumento h 
                            INNER JOIN workflow.documento d on d.docid = h.docid and d.tpdid = 49 --COLOCADO  A PEDIDO DO DANIEL(FECHE) NA DATA DE 01/07/2013 PROBLEMA: SQL ESTA CONSUMINDO MUITO O BANCO.
                            WHERE h.docid = doc.docid AND h.hstid IN (".implode(',', $arhstid).")
                            --GROUP BY h.docid
                        )
			INNER JOIN workflow.acaoestadodoc 	   aed ON aed.aedid = hst.aedid
			INNER JOIN workflow.estadodocumento   esd2 ON esd2.esdid = aed.esdidorigem
			WHERE
				prc.prcstatus = 'A'
				AND prc.prcid IN (".implode(',', $_POST['guia']).")
			ORDER BY
				esd2.esddsc, coo.coosigla ASC";
	$dados = $db->carregar($sql);
}

$arGuia = array();
if( $dados && $_POST['guia'] ){
	$gdiid = $db->pegaUm("INSERT INTO conjur.guiadistribuicao(gdidsc,usucpf) VALUES('Guia de Distribuição - ".date('YmdHisu')."','".$_SESSION['usucpf']."') RETURNING gdiid");
	array_push( $arGuia, $gdiid );
	foreach($dados as $dado):
		if( $_POST['guia'] ){ 
			$sql = "INSERT INTO conjur.itemguiadistribuicao(prcid,hstid,gdiid) VALUES(".$dado['prcid'].",".$dado['hstid'].",$gdiid)";  
			$db->executar($sql); 
		}
	endforeach;
	$db->commit();
}

if( $_POST['guia'] || $_GET['gdiid'] ){ 

$filtro = $_POST['guia'] ? " gd.gdiid IN (".implode(',', $arGuia).") " : " gd.gdiid = ".$_GET['gdiid'];

$sql = "SELECT
			gd.gdidsc,
		    esd.esddsc as situacao_origem,
		    aed.aeddscrealizada as situacao_destino,
		    to_char(gd.gdidt,'DD/MM/YYYY') as data
		FROM
			conjur.guiadistribuicao gd
		    inner join conjur.itemguiadistribuicao i on i.gdiid = gd.gdiid
		    inner join workflow.historicodocumento hst on  hst.hstid = i.hstid
		    inner join workflow.acaoestadodoc aed ON aed.aedid = hst.aedid
		    inner join workflow.estadodocumento esd on esd.esdid = aed.esdidorigem
		WHERE
			$filtro
		    and gd.gdistatus = 'A'
		ORDER BY gd.gdiid DESC";
	
	$arrOrigDest = $db->pegaLinha( $sql );

	$origem = $arrOrigDest['origem'];
	$destino = $arrOrigDest['destino'];
	$situacao_origem = $arrOrigDest['situacao_origem'];
	$situacao_destino = $arrOrigDest['situacao_destino'];
	$dtcriacao = $arrOrigDest['data'];	
	$titulo_guia = $arrOrigDest['gdidsc'];
}

$usunome = $db->pegaUm("SELECT usunome FROM seguranca.usuario WHERE usucpf = '".$usucpf."'" );
$setor = recuperaCoordenacaoUsuario( $usucpf );
$dtcriacao = $dtcriacao ? $dtcriacao : date('d/m/Y');
$titulo_guia = $titulo_guia ? $titulo_guia : 'CONJUR - Guia de Distribuição';
?>
<html>
	<head>
		<style type="text/css" media="print">
			body{
			 	@PAGE landscape {size: landscape;} 
				TABLE {PAGE: landscape;}
			}
		</style>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script type="text/javascript">
//		$(document).ready(function()
//		{
//			$(window).unload(function()
//			{
//				window.opener.location.href = window.opener.location.href;
//			});
//		});
		</script>
		<script type="text/javascript">
		this._closeWindows = false;
		</script>
	</head>
	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
		<table width="100%" cellspacing="0" cellpadding="0" border="0" style="border-bottom: 1px solid; size: landscape;" class="notscreen1 debug landscape">
			<tbody>
				<?php echo monta_cabecalho_relatorio('100'); ?>
			</tbody>
		</table>
    		<br />
		<center><span style="font-weight:bold;font-size:16px;">CONJUR - <?=$titulo_guia; ?></span></center>
		<br />
		<center>
			<table width="90%" cellspacing="0" cellpadding="2" border="0" style="" class="tabela">
				<thead>
					<tr>
					<td><table width="90%" cellspacing="0" cellpadding="2" border="0" style="" class="">
						<tr>
							<td style="background-color:white;text-align:right; width: 01%" ><b>Origem:</b></td>
							<td><?=$situacao_origem; ?></td>
						</tr>
						<tr>
							<td style="background-color:white;text-align:right; width: 05%" ><b>Destino:</b></td>
							<td><?=$situacao_destino; ?></td>
						</tr>
						<tr>
							<td>&nbsp;</td>
						</tr>
					</table></td>
					<td><table width="90%" cellspacing="0" cellpadding="2" border="0" style="" >
						<tr>
							<td style="background-color:white;text-align:right;"><b>Responsável:</b></td>
							<td style="background-color:white;text-align:left;"><?=$usunome;?></td>
						</tr>
						<tr>
							<td style="background-color:white;text-align:right;"><b>Setor:</b></td>
							<td style="background-color:white;text-align:left;"><?=$setor;?></td>
						</tr>
						<tr>
							<td style="background-color:white;text-align:right;"><b>Data:</b></td>
							<td style="background-color:white;text-align:left;"><?=$dtcriacao; ?></td>
						</tr>
					</table></td>
					</tr>
				</thead>
				<tr>
					<td colspan="7">
						<table width="100%" cellspacing="0" cellpadding="2" border="0" align="center" class="tabela">
						<thead>
							<tr>
								<th style="background-color:#e9e9e9;border:1px solid #ccc;" colspan="4">Processo</th>
								<th style="background-color:#e9e9e9;border:1px solid #ccc;" colspan="2">Recebimento</th>
							</tr>
							<tr>
								<th style="background-color:#e9e9e9;border:1px solid #ccc;" width="15%">Número do Processo</th>
                                <th style="background-color:#e9e9e9;border:1px solid #ccc;" width="15%">Advogado</th>
								<th style="background-color:#e9e9e9;border:1px solid #ccc;" width="30%">Interessado</th>
								<th style="background-color:#e9e9e9;border:1px solid #ccc;" width="10%">Data de Tramitação</th>
								<th style="background-color:#e9e9e9;border:1px solid #ccc;" width="10%">Data de Recebimento</th>
								<th style="background-color:#e9e9e9;border:1px solid #ccc;" width="20%">Nome Legível</th>
							</tr>
						</thead>
						<tbody>
						<?php if( $dados ): ?>
							<?php $cont = 0; ?>
							<?php $situacao = '';?>
							<?php foreach($dados as $dado): ?>
							<?php $cor = ($cont%2) ? '#f5f5f5' : '#ffffff'; ?>
							<tr style="height:20px;">
								<td  style="background-color:<?=$cor?>; text-align: center; border:1px solid #ccc;"><?=$dado['prcnumsidoc']?></td>
                                <td  style="background-color:<?=$cor?>; text-align: center; border:1px solid #ccc;"><?=$dado['entnome']?></td>
								<td  style="background-color:<?=$cor?>; text-align: center; border:1px solid #ccc;"><?=$dado['prcnomeinteressado']; ?></td>
								<td  style="background-color:<?=$cor?>; text-align: center; border:1px solid #ccc;"><?=$dado['data'];?></td>
								<td  style="background-color:<?=$cor?>; text-align: center; border:1px solid #ccc;">___/___/_____</td>
								<td  style="background-color:<?=$cor?>; border:1px solid #ccc;">___________________________________________</td>
							</tr>
							<?php $cont++; ?>
							<?php endforeach; ?>
						<?php endif; 
							unset($_POST); ?>
						</tbody>
					</td>
				</tr>
			</table>
			<br>
		</center>
	</body>
        <br><a href="javascript: window.print();" ><img style="text-align: center;" border="0" height="20px;" src="../imagens/print2.gif"></a>
</html>
<?
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$unidade = $db->usuarioUnidadesPermitidas('reuni');

if( ! in_array($_REQUEST['unicod'], $unidade))
{
	echo "<script>location.href = 'reuni.php?modulo=principal/listamonitoramento&acao=C' </script>";
}
$menu[0] = array("descricao" => "Lista de Unidades", "link"=> "/reuni/reuni.php?modulo=principal/listamonitoramento&acao=C");
$menu[1] = array("descricao" => "Monitoramento Acadêmico", "link"=> "/reuni/reuni.php?modulo=principal/cadastromonitoramento&acao=C&unicod=".$_REQUEST['unicod']."");
$menu[2] = array("descricao" => "Formulário de Impressão", "link"=> "/reuni/reuni.php?modulo=principal/relatorio_monitoramento&acao=C&unicod=".$_REQUEST['unicod']."");

echo montarAbasArray($menu, "/reuni/reuni.php?modulo=principal/relatorio_monitoramento&acao=C&unicod=".$_REQUEST['unicod']."");

//$db->cria_aba( $abacod_tela, $url, '' );
$db->cria_aba( $abacod_tela, $url, "&unicod=" . $_REQUEST['unicod'] );
monta_titulo( 'Formulário de Impressão', "" );
?>

<html>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>Relatório - Monitoramento Acadêmico</title>
<script language="JavaScript" src="../../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>

</head>
<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">

<table class="tabela" bgcolor="#f5f5f5" width="100%" cellpadding="0" align="center"	cellspacing="0">
<?

	$sql="	
			select
			 gp.grpdsc  as filho-- dsc Filho
			, gp2.grpcod as codpai
			, gp2.grpdsc as pai -- dsc  Pai
			, gp2.grpord as ordempai -- ordem pai
			, gp.grpord as ordemfilho-- ordem filho
			, rp.rspdsc -- Metas cadastradas
			, si.sitdsc -- Qualificação
			, pa.pardsc -- Parecer ADHOC
			, coalesce (st.stadsc, 'Não Informado') as stadsc 
			, coalesce(to_char(mo.monpercentual, '999 %'), 'Não Informado') as monpercentual 
			, coalesce (mo.monparecer, 'Não Informado') as monparecer
			, coalesce (sim.sitdsc, 'Não Informado') as sitdsc 
			, coalesce (mo.monprovidencias, 'Não Informado') as monprovidencias 
			from reuni.pergunta pr
			left join reuni.resposta rp on pr.prgcod = rp.prgcod
			left join reuni.parecer pa on rp.rspcod = pa.rspcod
			left join reuni.situacao si on pa.sitcod = si.sitcod
			left join reuni.monitoramento mo on rp.unicod = mo.unicod and rp.unitpocod = mo.unitpocod and pr.grpcod = mo.grpcod
			left join reuni.status st on mo.staid = st.staid
			left join reuni.situacao sim on mo.sitcod = sim.sitcod
			inner join reuni.grupopergunta gp ON gp.grpcod = pr.grpcod
			inner join reuni.grupopergunta gp2 ON gp.grpcodfil = gp2.grpcod
			where rp.unicod = '".$_REQUEST['unicod']."' and rp.unitpocod = 'U' -- Fundação Universidade de Brasília - variável		
			and pr.prgdsc = 'Metas a serem alcançadas com o cronograma de execução' -- fixo - Metas a serem alcançadas
			and pa.tpacod = 2 -- Parecer ADHOC -- fixo
			order by
			ordempai,
			ordemfilho
	
	";	
	$sql_unidade 	= "select unidsc from public.unidade where unicod = '".$_REQUEST['unicod']."'";
	$unidade 		= $db->pegaUm($sql_unidade);
	
	echo("<tr  bgcolor='#c0c0c0' align='center'>			
			<td height='25px' style='font-size: 12px;'' ><b>".$unidade."
			</b>				
			</td>
		 </tr>");
	
	$dados = $db->carregar($sql);	
	if($dados){
		foreach ($dados as $chave =>  $dado) {
		$codpai = $dado['codpai'];
		$chave_anterior = $chave - 1;
		$codpai_anterior = $dados[$chave_anterior]['codpai'];
			
		?><tr> 
			<td>
			<table class="listagem" style="width: 100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				
				<?
					if($codpai != $codpai_anterior){
						?>
						<tr>
							<td bgcolor='#DCDCDC' align="right" valign="top" width="15%"><b>Tópico:</b></td>
							<td width="85%"><b><?=$dado['pai']; ?></b>
							</td>
						</tr>
						<?
					}
				?>

				
				<tr>
				<td align="right" bgcolor='#DCDCDC' valign="top" width="15%"><b>Item:</b></td>
				<td width="85%"><b><?=$dado['filho']; ?></b>
				</td>
				</tr>
				
				<tr>
				<td class="SubTituloDireita" align="right" valign="top"><b>Metas:</b></td>
				<td><?=$dado['rspdsc']; ?>
				</td>
				</tr>
				
				<tr>
				<td class="SubTituloDireita" align="right" valign="top"><b>Qualificação:</b></td>
				<td><?=$dado['sitdsc']; ?>
				</td>
				</tr>
				
				<tr>
				<td class="SubTituloDireita" align="right" valign="top"><b>Parecer ADHOC:</b></td>
				<td><?=$dado['pardsc']; ?>
				</td>
				</tr>
				
				<tr>
				<td class="SubTituloDireita"></td>
				<td class="SubTituloCentro">Monitoramento Acadêmico</td>	
				</tr>
				
				<tr>
				<td class="SubTituloDireita" valign="top"><b>Situação:</b></td>
				<td><?=$dado['stadsc']; ?>
				</td>
				</tr>
				
				<tr>
				<td class="SubTituloDireita" align="right" valign="top"><b>Andamento:</b></td>
				<td><?=$dado['monpercentual']; ?>
				</td>
				</tr>		
				
				<tr>
				<td class="SubTituloDireita" align="right" valign="top"><b>Parecer ADHOC:</b></td>
				<td><?=$dado['monparecer']; ?>
				</td>
				</tr>		
				
				<tr>
				<td class="SubTituloDireita" align="right" valign="top"><b>Qualificação:</b></td>
				<td><?=$dado['sitdsc']; ?>
				</td>			
				</tr>
				
				<tr>
				<td class="SubTituloDireita" align="right" valign="top"><b>Providência:</b></td>
				<td><?=$dado['monprovidencias']; ?> 
				</td>
				</tr>
				
				 				
			</table>
			</td>
		</tr>					
		<?
		}
		?>
		
		
	<?	
	}else{		
		echo("<tr><td align='center'><b>");
		echo("<br/>Nenhum registro encontrado<br/><br/>");
		echo("</b></td></tr>");
	}
	
	
?>

	<tr>
			<td>
				<table class="tabela" style="width: 100%" bgcolor="#f5f5f5" width="100%" cellpadding="0" align="center"	cellspacing="0">
					<tr>							
						<td>&nbsp;</td>
						<td>&nbsp;</td>
					</tr>
					<tr>							
						<td>&nbsp;</td>
						<td>&nbsp;</td>
					</tr>
					<tr align="center">							
						<td>----------------------------------------------------</td>
						<td>----------------------------------------------------</td>
					</tr>
					<tr align="center">
						<td><b>Responsável IFES</b></td>
						<td><b>Parecerista ADHOC</b></td>
					</tr>	
					<tr>							
						<td>&nbsp;</td>
						<td>&nbsp;</td>
					</tr>
					<tr>							
						<td>&nbsp;</td>
						<td>&nbsp;</td>
					</tr>
					<tr align="center">							
						<td>----------------------------------------------------</td>
						<td>----------------------------------------------------</td>
					</tr>
					<tr align="center">
						<td><b>Parecerista ADHOC</b></td>
						<td><b>Representante MEC</b></td>
					</tr>	
					<tr>							
						<td>&nbsp;</td>
						<td>&nbsp;</td>
					</tr>
					<tr>							
						<td>&nbsp;</td>
						<td>&nbsp;</td>
					</tr>				
				</table>
			</td>	
		</tr>
		<tr>
		<td align="center" height="25">
			<input type="button" onclick="relatorioMonitoramento()" value="Imprimir">
		</td>
		</tr>

</table>
</body>
</html>

<script type="text/javascript">
   function relatorioMonitoramento()
	{
		window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/reuni/reuni.php?modulo=principal/popup_monitoramento&acao=C&unicod=<?=$_REQUEST['unicod'] ?>','blank','height=800,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no');
	}
</script>



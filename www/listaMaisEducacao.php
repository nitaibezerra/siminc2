<?php 

//echo md5('correpula2013');
//394bd0c5670359dc0cf1683e85c25263


$_REQUEST['baselogin'] = "simec_espelho_producao";

// CPF do administrador de sistemas
$_SESSION['usucpf'] = '00000000191';

if(!$_SESSION['usucpf'])

	$_SESSION['usucpforigem'] = '00000000191';

	
if(md5('proemi2014') == $_GET['chave'])
{
	include 'config.inc';
	include_once APPRAIZ . "includes/funcoes.inc";
	include_once APPRAIZ . "includes/classes_simec.inc";
	
	$db = new cls_banco();
	
	?>
	<html>
	<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='/includes/listagem.css'/>
	<script type="text/javascript" src="/includes/prototype.js"></script>
	
	<body>
	

	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
		<tr>
			<td	colspan="2">
				<?
				echo '<center><b>Programa Mais Educação – Expansão de escolas e alunos</b></center><br>';
						
				$sql = "SELECT /*
							   '<center>'||periodo||'</center>' as periodo,
							   '<center>'||trim( to_char( coalesce(SUM(alunado),0 ), 'FM999G999G999G999' ) )||'</center>' AS alunado, 
							   '<center>'||trim( to_char( coalesce(SUM(escolas),0 ), 'FM999G999G999G999' ) )||'</center>' AS escolas
							   */
							   periodo,
							   SUM(alunado) AS alunado, 
							   SUM(escolas) AS escolas
						FROM(
						                SELECT
						                               dpe.dpeanoref AS periodo,
						                               CASE WHEN sh.indid IN (102) THEN dsh.dshqtde::integer END AS alunado,
						                               CASE WHEN sh.indid IN (690) THEN dsh.dshqtde::integer END AS escolas
						                FROM painel.seriehistorica sh
						                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						                WHERE sh.indid in (102,690)
						                AND sh.sehstatus <> 'I'
						) AS FOO
						GROUP BY periodo
						ORDER BY periodo";
				
				$cabecalho 		= array( "Ano", "Estudante", "Escola");
				$tamanho		= array( '34%', '33%', '33%');
				$alinhamento	= array( 'center', 'center', 'center');
				//$db->monta_lista_simples( $sql, $cabecalho, 100, 10, 'N', '', 'N', 'N');
				//$db->monta_lista( $sql, $cabecalho, 100, 10, 'N', 'center', '', '',$tamanho,$alinhamento);
				//monta_lista_array($dados,$cabecalho="",$perpage,$pages,$soma,$alinha,$html=array(),$arrayDeTiposParaOrdenacao=array(),$formName = "formlista") {
				//$dados = $db->carregar($sql);
				//$db->monta_lista_array($dados,$cabecalho,100,10,'N',$alinhamento);
				?>
				
				<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
					<thead>
						<tr>
							<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Ano</td>
							<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Estudante</td>
							<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Escola</td>
						</tr>
					</thead>
					<tbody>
						<?
						$dados = $db->carregar($sql);
						
						foreach ($dados as $v){
						?>
							<tr bgcolor="" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
								<td valign="top" title="Ano"><center><?=$v['periodo']?></center></td>
								<td valign="top" title="Estudante"><center><?=number_format($v['alunado'],0,',','.')?></center></td>
								<td valign="top" title="Escola">
									<center>
									<?if($v['periodo'] == 2014) echo '&nbsp;&nbsp;';?>
									<?=number_format($v['escolas'],0,',','.')?>
									<?if($v['periodo'] == 2014) echo '*';?>
									</center>
								</td>
							</tr>
						<?}?>
					</tbody>
				</table>
				<table width="98%" align="center" border="0">
						<tr>
							<td>
								* Dado de agosto/2014, antes do término da adesão.
							</td>
						</tr>
				</table>
			</td>
		</tr>
	</table>
		
	</body>
	</html>
	
	<?
	
}//fecha if(md5('correpula2013') == $_GET['chave'])

?>


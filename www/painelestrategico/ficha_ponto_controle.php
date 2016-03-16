<?php
/*
 * Desenvolvido por: FGV Projetos
 * Data: 21/04/09
 * Programa: ficha_objetivo.php
 * Descrição sumária: contém o detalhamento do objetivo selecionado
 */
?>

<?php
  session_start();
  include 'cabecalho2.php';
  include 'config_banco.php';
?>


<?php
	#Testar se deve atualizar dados no banco
	if ( isset($_REQUEST['nm_indicador']) )
	{

		if ( isset($_REQUEST['status']) ) {
		#realizar a atualizacao para um novo status
			$sql = "UPDATE mec_painel.TB_INDICADOR_FUNCAO SET CD_STATUS = cast( (cast(CD_STATUS as integer) % 3) + 1  as char) WHERE cd_ponto_controle = " . $_REQUEST['cd_ponto_controle'] . " AND NM_INDICADOR = '" . $_REQUEST['nm_indicador'] . "'";
			pg_query($sql);
			echo "<script language='javascript'>window.opener.location.reload(true);</script>";
		}

		if ( isset($_REQUEST['tendencia']) ) {
		#realizar a atualizacao para uma nova tendencia
			$sql = "UPDATE mec_painel.TB_INDICADOR_FUNCAO SET CD_TENDENCIA = cast( (cast(CD_TENDENCIA as integer) % 5) + 1  as char) WHERE cd_ponto_controle = " . $_REQUEST['cd_ponto_controle'] . " AND NM_INDICADOR = '" . $_REQUEST['nm_indicador'] . "'";
			pg_query($sql);
			echo "<script language='javascript'>window.opener.location.reload(true);</script>";
		}

	}

  #fazer a carga dos dados de acordo como objetivo escolhido
  $sql = "SELECT nm_ponto_controle FROM mec_painel.TB_PONTO_CONTROLE WHERE cd_ponto_controle = " . $_REQUEST['cd_ponto_controle'] ;
  $query = pg_query($sql);

  while ($row = pg_fetch_array($query))
  {
	$ponto_controle = trim($row["nm_ponto_controle"]);
  }
?>

<script type="text/javascript">
<!-- /* Created by: Lee Underwood :: http://javascript.internet.com/ */
function newWindow(link) {
  var bookWindow;
  bookWindow = window.open(link, "mapas_estrategicos", "scrollbars=yes,toolbar=no,status=no,location=no,menubar=no,resizable=no,height=600,width=1025");
  if (bookWindow.open) {
    bookWindow.close;
  }
  bookWindow.focus();
}
-->
</script>
<script type="text/javascript">
<!-- 
image1 = new Image();
image1.src = "img/uf-mouseover.gif";

image2 = new Image();
image2.src = "img/hist-mouseover.gif";
//  -->
</script>

<div id="pagina">

	<div id="cabecalho-objetivo">
		<h1>Fluxos de Trabalho de A&ccedil;&otilde;es PDE</h1>
	</div> <!-- fecha cabecalho -->
	
	<div id="conteudo2">
		<h2 class="ficha-objetivo">Ponto de Controle</h2>
		<div id="descr-objetivo">
			<p>
				<strong>Nome:</strong> <? echo $ponto_controle; ?><br />
			</p>
		</div>
		
		<div id="tabela-objetivo">
		<table id="tabela-3" summary="Ficha do Objetivo">
		
			<colgroup>  
			   <col style="width: 50%" />  
			   <col style="width: 10%" />  
			   <col style="width: 10%" />  
			   <col style="width: 10%" />  
			   <col style="width: 10%" />  
			   <col style="width: 10%" />  
			</colgroup>

			<thead>
				<tr>
					<th scope="col">Indicador</th>
					<th scope="col">Apurado</th>
					<th scope="col">Meta</th>
					<th scope="col">Status</th>
					<th scope="col">Tend&ecirc;ncia</th>
					<th scope="col">S&eacute;rie Hist&oacute;rica</th>
				</tr>
			</thead>
			<tbody>


<?php

	$sql = "SELECT * FROM mec_painel.tb_indicador_funcao WHERE CD_PONTO_CONTROLE = " . $_REQUEST['cd_ponto_controle'] . " ORDER BY nr_ordem_apresentacao " ;
	$query = pg_query($sql);

	while ($row = pg_fetch_array($query))
	{
		$src_status = $row["cd_status"];	
		$src_tendencia = $row["cd_tendencia"];
		$desatualizado = (trim($row["cd_desatualizado"]) == "1");

		// define o led do status
		switch ($row["cd_status"]) {
		case 1:
			$src_status = "img/led-tabela-ok.gif";
			break;
		case 2:
			$src_status = "img/led-tabela-alerta.gif";
			break;
		case 3:
			$src_status = "img/led-tabela-critico.gif";
			break;
		}
		
		// define a seta da tendência
		switch ($row["cd_tendencia"]) {
		case 1:
			$src_tendencia = "img/seta-up-verde.gif";
			break;
		case 2:
			$src_tendencia = "img/tendencia-equilibrio.gif";
			break;
		case 3:
			$src_tendencia = "img/seta-down-verm.gif";
			break;
		case 4:
			$src_tendencia = "img/seta-down-verde.gif";
			break;
		case 5:
			$src_tendencia = "img/seta-up-verm.gif";
			break;
		}
		
		// apresenta o indicador
?>

		<tr>
			<td class="bold">
				<a href="ficha_indicador.php"
					onclick="newWindow(this.href); return false;">
					<? echo trim($row["nm_indicador"]) ?>
					<? if ($desatualizado) {?>
						&nbsp;<img src="img/ic_desatualizado.png"> 
					<? } ?>
				</a>
			</td>
			<td><? echo trim($row["dc_apurado"]) ?></td>
			<td><? echo trim($row["dc_meta"]) ?></td>
			<td align="center">
				<a href='?cd_ponto_controle=<?echo trim($_REQUEST["cd_ponto_controle"])?>&nm_indicador=<? echo $row["nm_indicador"]?>&status=1'>
				<img border='0' height='20' width='20' src='<? echo $src_status ?>'>
				</a>
			</td>
			<td align="center">
				<a href='?cd_ponto_controle=<?echo trim($_REQUEST["cd_ponto_controle"])?>&nm_indicador=<? echo $row["nm_indicador"]?>&tendencia=1'>
				<img border='0' height='20' width='20' src='<? echo $src_tendencia ?>'>
				</a>
			</td>
			<td align="center">
				<a href="serie_indicador.php" 
					onclick="newWindow(this.href); return false;"
					onmouseover="hist.src='img/hist-mouseover.gif';"
					onmouseout="hist.src='img/hist-mouseout.gif';">
					<img src='img/hist-mouseout.gif' name="hist" border='0' height='20' width='20' >
				</a>
			</td>
		</tr>
        
<?
	}
?>
	</tbody>
	</table>
</div> <!-- fecha tabela-objetivo -->

<div id="legenda-cores"> <img src="img/cores-status.gif" alt="legenda das cores" /></div>

</table>


		<div id="tabela-acoes-objetivo">
		<table id="tabela-4" summary="Ações do PDE que impactam no objetivo">

			<thead>
				<tr>
					<th scope="col">Objetivos Estrat&eacute;gicos Relacionados:</th>
				</tr>
			</thead>
			<tbody>

<?php
  $sql = "SELECT M.*, O.* FROM mec_painel.TB_PONTO_CONTROLE P INNER JOIN mec_painel.TB_ACAO_OBJETIVO AO on p.CD_ACAO = AO.CD_ACAO INNER JOIN mec_painel.TB_OBJETIVO O on O.CD_OBJETIVO = AO.CD_OBJETIVO INNER JOIN mec_painel.TB_MAPA M on M.CD_MAPA = O.CD_MAPA WHERE CD_ponto_controle = " . $_REQUEST['cd_ponto_controle'] . " ORDER BY nm_mapa, nm_objetivo " ;
  $query = pg_query($sql);

	while ($row = pg_fetch_array($query))
	{
#	echo "<tr  class='cor_mapa'><td align='left' class='perspectiva'><a class='link' href='javascript:window.opener.location=\"fluxo_trabalho.php?cmbAcao=" . trim($row["cd_acao"]) . "\";window.close();'>".trim($row["nm_acao"]) . "</a></td></tr>";
?>
		<tr>
			<td class="bold">
				<? echo trim($row["nm_mapa"]) ?>
				<br />
				<a href='javascript:window.opener.location=&quot;index.php?cmbMapa=
				<? echo trim($row["cd_mapa"]) ?>&quot;;window.close();'>
					<? echo trim($row["nm_objetivo"]) ?>
				</a>
			</td>
		</tr>
<?
	}

?>

			</tbody>
		</table>
		</div> <!-- fecha tabela-acoes-objetivo -->

<?php

  pg_free_result($query);
  pg_close($conexao);
?>

<p align="center"><input type="image" src="img/fechar.gif" value="Fechar" onClick="window.close();"></p>


<?php include 'rodape.php' ?>

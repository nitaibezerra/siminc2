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
  include 'cabecalho.php';
  include 'config_banco.php';
?>


<?php
	#Testar se deve atualizar dados no banco
	if ( isset($_REQUEST['nm_indicador']) )
	{

		if ( isset($_REQUEST['status']) ) {
		#realizar a atualizacao para um novo status
			$sql = "UPDATE mec_painel.TB_INDICADOR SET CD_STATUS = cast( (cast(CD_STATUS as integer) % 3) + 1  as char) WHERE CD_OBJETIVO = " . $_REQUEST['cd_objetivo'] . " AND NM_INDICADOR = '" . $_REQUEST['nm_indicador'] . "'";
			pg_query($sql);
			echo "<script language='javascript'>window.opener.location.reload(true);</script>";
		}

		if ( isset($_REQUEST['tendencia']) ) {
		#realizar a atualizacao para uma nova tendencia
			$sql = "UPDATE mec_painel.TB_INDICADOR SET CD_TENDENCIA = cast( (cast(CD_TENDENCIA as integer) % 5) + 1  as char) WHERE CD_OBJETIVO = " . $_REQUEST['cd_objetivo'] . " AND NM_INDICADOR = '" . $_REQUEST['nm_indicador'] . "'";
			pg_query($sql);
			echo "<script language='javascript'>window.opener.location.reload(true);</script>";
		}

	}

  #fazer a carga dos dados de acordo como objetivo escolhido
  $sql = "SELECT m.*, o.*, p.*, t.*, o.cd_tema  as cd_tema_aux FROM mec_painel.TB_OBJETIVO O INNER JOIN mec_painel.TB_MAPA M on M.CD_MAPA = O.CD_MAPA INNER JOIN mec_painel.TB_PERSPECTIVA P on P.cd_perspectiva = O.cd_perspectiva left outer join mec_painel.tb_tema t on t.cd_tema = o.cd_tema WHERE o.CD_OBJETIVO = '" . $_REQUEST['cd_objetivo'] . "'";
  $query = pg_query($sql);

  while ($row = pg_fetch_array($query))
  {

	$mapa = trim($row["nm_mapa"]);
        $perspectiva = trim($row["nm_perspectiva"]);
	$tema = trim($row["nm_tema"]);
	$objetivo = trim($row["nm_objetivo"]);
	$in_corporativo = trim($row["in_corporativo"]);

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
		<h1>Mapas Estrat&eacute;gicos</h1>
	</div> <!-- fecha cabecalho -->
	
	<div id="conteudo">
		<h2 class="ficha-objetivo">Ficha do objetivo</h2>
		<div id="descr-objetivo">
			<p>
				<strong>Mapa:</strong> <? echo $mapa; ?><br />
				<strong>Perspectiva:</strong> <? echo $perspectiva; ?><br />
				<? if ($tema != null) { ?> <strong>Tema:</strong> <? echo $tema; ?><br /> <? } ?>
				<strong>Objetivo:</strong> <? echo $objetivo; ?>
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

	$sql = "SELECT * FROM mec_painel.TB_INDICADOR WHERE CD_OBJETIVO = " . $_REQUEST['cd_objetivo'] . " ORDER BY nr_ordem_apresentacao " ;
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
				<a href='?cd_objetivo=<?echo trim($_REQUEST["cd_objetivo"])?>&nm_indicador=<? echo $row["nm_indicador"]?>&status=1'>
				<img border='0' height='20' width='20' src='<? echo $src_status ?>'>
				</a>
			</td>
			<td align="center">
				<a href='?cd_objetivo=<?echo trim($_REQUEST["cd_objetivo"])?>&nm_indicador=<? echo $row["nm_indicador"]?>&tendencia=1'>
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


<?php

	if ($in_corporativo == "N")
	{

?>

		<div id="tabela-acoes-objetivo">
		<table id="tabela-4" summary="Ações do PDE que impactam no objetivo">

			<thead>
				<tr>
					<th scope="col">A&ccedil;&otilde;es do PDE que impactam no objetivo</th>
				</tr>
			</thead>
			<tbody>

<?php
  $sql = "SELECT A.* FROM mec_painel.TB_ACAO A INNER JOIN mec_painel.TB_ACAO_OBJETIVO O on O.CD_ACAO = A.CD_ACAO WHERE CD_OBJETIVO = " . $_REQUEST['cd_objetivo'] . " ORDER BY nr_ordem_apresentacao " ;
  $query = pg_query($sql);

	while ($row = pg_fetch_array($query))
	{
#	echo "<tr  class='cor_mapa'><td align='left' class='perspectiva'><a class='link' href='javascript:window.opener.location=\"fluxo_trabalho.php?cmbAcao=" . trim($row["cd_acao"]) . "\";window.close();'>".trim($row["nm_acao"]) . "</a></td></tr>";
?>
		<tr>
			<td class="bold">
				<a href='javascript:window.opener.location=&quot;fluxo_trabalho.php?cmbAcao=
				<? echo trim($row["cd_acao"]) ?>&quot;;window.close();'>
					<? echo trim($row["nm_acao"]) ?>
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
  }

  pg_free_result($query);
  pg_close($conexao);
?>

<p align="center"><input type="image" src="img/fechar.gif" value="Fechar" onClick="window.close();"></p>


<?php include 'rodape.php' ?>

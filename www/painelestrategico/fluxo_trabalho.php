<?php
/*
 * Desenvolvido por: FGV Projetos
 * Data: 22/06/09
 * Programa: fluxo_trabalho.php
 * Descrição sumária: contém a interface para seleção do fluxo de trabalho desejado
 */
?>


<?php
  session_start();
  include 'cabecalho2.php';
  include 'config_banco.php';
?>

<script src="validarForm.js" type="text/javascript"></script>

<script>

function Validar()
{

	if(document.frm.cmbAcao.value == "")
	{
		alert('Selecione uma ação.');
		document.frm.cmbAcao.focus();
		return false;
	}

	window.location = "fluxo_trabalho.php?cmbAcao=" + document.frm.cmbAcao.value;

	return false;
}

</script>

<script type="text/javascript">
<!-- /* Created by: Lee Underwood :: http://javascript.internet.com/ */ -->
function newWindow(link) {
  var bookWindow;
  bookWindow = window.open(link, "ficha_objetivo", "scrollbars=yes,toolbar=no,status=no,location=no,menubar=no,resizable=no,height=580,width=840");
  if (bookWindow.open) {
    bookWindow.close;
  }
  bookWindow.focus();
}
</script>

<div id="pagina">

	<div id="cabecalho">
		<div id="mec"><img src="img/mec.png"/></div>
		<div id="fgv-pde"><img src="img/fgv.png"/><img src="img/pde.jpg"/></div>
	</div> <!-- fecha cabecalho -->
	
	<div id="conteudo2">
	
		<div id="tit-mapas">
		<h1>Fluxos de Trabalho de A&ccedil;&otilde;es PDE</h1>
		<?
		$cmbAcao = $_REQUEST['cmbAcao'];
		if ($cmbAcao) {
			$sql = "SELECT * FROM mec_painel.TB_ACAO WHERE CD_ACAO = '" . $cmbAcao . "'";
			$query = pg_query($sql);
			$row = pg_fetch_array($query);
			echo "\n";
			echo "<h2>A&ccedil;&atilde;o: " . trim($row["nm_acao"]) . "</h2>";
		}
		?>
		</div>
		
		<div id="selecao">
		<p>
		<form name="frm" action="fluxo_trabalho.php" method="post">
		A&ccedil;&atilde;o Desejada:<br />
		<select name="cmbAcao">


		<option value="">---Selecione---</option>

		<?php
	 
			#fazer a carga dos dados de acordo com valores do banco de dados
			$sql = "SELECT * FROM mec_painel.TB_ACAO ORDER BY CD_ACAO";
			$query = pg_query($sql);

			while ($row = pg_fetch_array($query))
			{
				echo "<option value='"
					. trim($row["cd_acao"]) 
					. "'>" 
					.  simec_htmlspecialchars(trim($row["nm_acao"]))
					. "</option>";
				echo "\n";
			}

		?>
		</select>

		<input type="button" value="Visualizar" onClick="return Validar()">

		</form>
		</p>
		</div> <!-- fecha selecao -->

<?php
	#se clicou em "visualizar", montar a matriz
	if (isset($_REQUEST['cmbAcao']))
	{
?>

<div id="tabela">

<?php

#obter a URL do fluxo

	$sql = "select nm_arquivo_fluxo from mec_painel.tb_acao where cd_acao = '" . $_REQUEST['cmbAcao'] . "'";
	$query = pg_query($sql);

	$url = "";
	While ($row = pg_fetch_array($query))
	{
		$url = $row["nm_arquivo_fluxo"];	
	}
?>

<p>
<font color="white">
Clique <a href="javascript: void(0);"  onclick="javascript:window.open('<?php echo $url; ?>','','scrollbars=yes,toolbar=no,status=no,location=no,menubar=no,resizable=no,height=580,width=840'); return false;">aqui</a> para visualizar o fluxo detalhado ou <a href="javascript: void(0);"  onclick="javascript:window.open('ficha_indicadores_acao.php?cd_acao=<?php echo $_REQUEST['cmbAcao']; ?>','','scrollbars=yes,toolbar=no,status=no,location=no,menubar=no,resizable=no,height=580,width=840'); return false;">aqui</a> para visualizar os Indicadores da A&ccedil;&atilde;o.
</font>
</p>

<? #################################################################################### ?>

<br><br>

<table id="tabela-1">
<!--
<table border=1>
-->
<tbody>



<?php
	    #obtendo coluna dos atores
	    $sql = "select * from mec_painel.tb_acao_ator aa inner join mec_painel.tb_ator a on aa.cd_ator = a.cd_ator where cd_acao = '" . $_REQUEST['cmbAcao'] . "' order by  nr_ordem_apresentacao_ator" ;
	    $query = pg_query($sql);

	    #para cada um dos atores
	    While ($row = pg_fetch_array($query))
	    {
		echo "\n";
		echo "<tr><td><b>" . $row["nm_ator"] . "</b></td>";

		#para cada um dos tempos preencher os pontos de controle
		#descobrindo quantidade de colunas (tempos) que existem
		
		$sql = "Select distinct cd_tempo from mec_painel.tb_ponto_controle aa where cd_acao = '" . $_REQUEST['cmbAcao'] . "' order by  cd_tempo" ;
		$query_tempos = pg_query($sql);

		while( $row_tempo = pg_fetch_array($query_tempos) )
		{	
#			echo "<td align='center' valign='center'><table cellpadding='10' cellspacing='10'><tr>";
			echo "\n";
			echo "<td>";
			
			
			$sql = 
				"Select pc.*,
				(SELECT COUNT(*)
					FROM mec_painel.tb_indicador_funcao I
					WHERE I.cd_ponto_controle = PC.cd_ponto_controle AND I.CD_DESATUALIZADO = '1') AS QT_DESATUALIZADO,
				(SELECT COUNT(*)
					FROM mec_painel.tb_indicador_funcao I 
					WHERE I.cd_ponto_controle = PC.cd_ponto_controle AND I.CD_STATUS = '1') AS QT_OK,
				(SELECT COUNT(*)
					FROM mec_painel.tb_indicador_funcao I 
					WHERE I.cd_ponto_controle = PC.cd_ponto_controle AND I.CD_STATUS = '2') AS QT_ALERTA, 
				(SELECT COUNT(*)
					FROM mec_painel.tb_indicador_funcao I 
					WHERE I.cd_ponto_controle = PC.cd_ponto_controle AND I.CD_STATUS = '3') AS QT_CRITICO
				from mec_painel.tb_ponto_controle pc ";
			$sql = $sql . " where cd_acao = '". $row["cd_acao"] . "' and cd_ator = ". $row["cd_ator"] . " and cd_tempo = ". $row_tempo["cd_tempo"] . " order by  nr_ordem_apresentacao ";

			$query_ponto = pg_query($sql);

			#para cada ponto encontrado

			$qt = 0;

			while ($row_ponto = pg_fetch_array($query_ponto))
			{	
				if ($qt > 0)
				{
					echo "<br />";
				}


				if ($row_ponto["qt_critico"] > 0)
				{
					$cor = "cor_critico";
				}
				else
				{
					if ($row_ponto["qt_alerta"] > 0)
					{
						$cor = "cor_alerta";
					}
					else
					{
						$cor = "cor_ok";
					}
				}

		// verifica se fluxo  objetivo deve ser marcado com tendo algum indicador desatualizado
		$fluxo_desatualizado = 0;
		if ($row_ponto["qt_desatualizado"] > 0) {
			$fluxo_desatualizado = 1;
		}
		
		//montar lista de indicadores, com variação de cor
		$sql_ind =
			"SELECT *
			FROM mec_painel.TB_INDICADOR_FUNCAO
			where cd_ponto_controle = " . $row_ponto["cd_ponto_controle"] . "
			order by nr_ordem_apresentacao ";

		$query_ind = pg_query($sql_ind);

		if ( pg_num_rows($query_ind) > 0)
		{
			$indicadores = "<div><ul class=&quot;flutuantes&quot;>";
		    While ($row_ind = pg_fetch_array($query_ind))
		    {
				switch ($row_ind["cd_status"]) {
					case 1:
						$cor_led = "led-ok";
						break;
					case 2:
						$cor_led = "led-alerta";
						break;
					case 3:
						$cor_led = "led-critico";
						break;
				}
				
				$desatualizado = $row_ind["cd_desatualizado"];
					
				$indicadores = $indicadores . '<li class=&quot;' . $cor_led . '&quot;>';
				$indicadores = $indicadores . '<a href=&quot;serie_indicador.php?indicador=';
				$indicadores = $indicadores . $row_ind["nm_indicador"] . '&quot;>';
				$indicadores = $indicadores . trim($row_ind["nm_indicador"]);
				$indicadores = $indicadores . '</a>';
				if ($desatualizado) {
					$indicadores = $indicadores . '&nbsp;<img src=&quot;img/ic_desatualizado.png&quot;>';
				}
				$indicadores = $indicadores . '</li>';
			}
			$indicadores = $indicadores . "</ul></div>";
		}
		else
		{
			$indicadores = "Nenhum indicador encontrado";
		}
				
?>				
			<ul id="opcoes">
				<li class="<?echo $cor?>">
					<? if ($fluxo_desatualizado) {?><ul><li class="cor_desatualizado"> <? }?>
						<a href="ficha_ponto_controle.php?cd_ponto_controle=<? echo $row_ponto["cd_ponto_controle"] ?>" 
						onclick="newWindow(this.href); return false;"
						onMouseOver="Tip('<?echo $indicadores?>',FIX, [this, 0, -2]);" 
						onMouseOut="UnTip();">

						<? echo trim($row_ponto["nm_ponto_controle"]) ?></a>
					<? if ($fluxo_desatualizado) {?></li></ul> <? }?>
				</li>
			</ul>
<?				
				$qt++;

			}			

#			echo "</tr></table></td>";
			echo "</td>";
			
		}

		
		echo "</tr>";		

	    }

?>

</td></tr>

</tbody>
</table>
</div> <!-- fecha tabela -->

<div id="legenda-cores"> <img src="img/cores-status.gif" alt="legenda das cores" /></div>
<?php
}
?>
</div> <!-- fecha conteudo -->

</div> <!-- fecha pagina -->
<?php include 'rodape.php' ?>
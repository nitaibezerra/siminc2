<?php
  session_start();
  include 'cabecalho.php';
  include 'config_banco.php';
?>

<?php
/*
 * Desenvolvido por: FGV Projetos
 * Data: 21/04/09
 * Programa: mapa.php
 * Descrição sumária: contém a interface para seleção do mapa desejado
 */
?>

<script src="validarForm.js" type="text/javascript"></script>

<script>
function Validar()
{

	if(document.frm.cmbMapa.value == "")
	{
		alert('Selecione um mapa.');
		document.frm.cmbMapa.focus();
		return false;
	}
	
	window.location = "index.php?cmbMapa=" + document.frm.cmbMapa.value;

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
		<div id="mec"><img src="img/MEC.png"/></div>
		<div id="fgv-pde"><img src="img/FGV.png"/><img src="img/PDE.jpg"/></div>
	</div> <!-- fecha cabecalho -->
	
	<div id="conteudo">
	
		<div id="tit-mapas">
		<h1>Mapas Estrat&eacute;gicos</h1>
		<?
		$cmbMapa = $_REQUEST['cmbMapa'];
		if ($cmbMapa) {
			$sql = "SELECT * FROM mec_painel.TB_MAPA WHERE CD_MAPA = '" . $cmbMapa . "'";
			$query = pg_query($sql);
			$row = pg_fetch_array($query);
			echo "\n";
			echo "<h2>Mapa: " . trim($row["nm_mapa"]) . "</h2>";
		}
		?>
		</div>
		
		<div id="selecao">
		<p>
		<form name="frm" action="index.php" method="post">
		Mapa Desejado:<br />
		<select name="cmbMapa">


		<option value="">---Selecione---</option>

		<?php
	 
			#fazer a carga dos dados de acordo com valores do banco de dados
			$sql = "SELECT * FROM mec_painel.TB_MAPA ORDER BY CD_MAPA";
			$query = pg_query($sql);

			while ($row = pg_fetch_array($query))
			{
				echo "<option value='"
					. trim($row["cd_mapa"]) 
					. "'>" 
					.  simec_htmlspecialchars(trim($row["nm_mapa"]))
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
	#se clicou em "visualizar", montar o mapa
	if (isset($_REQUEST['cmbMapa']))
	{
?>

<div id="tabela">

<?php
 
	    #fazer a montagem do cabecalho do mapa

	    $sql = "SELECT * FROM mec_painel.TB_MAPA WHERE CD_MAPA = '" . $_REQUEST['cmbMapa'] . "'";
	    $query = pg_query($sql);

	    $qt_tema = 0;
	    $ind_corporativo = "N";

	    While ($row = pg_fetch_array($query))
	    {
			if ( $row["in_corporativo"] == "S")
			{
				$ind_corporativo = "S";

				#montar colunas de temas
				$sql = "SELECT * FROM mec_painel.TB_TEMA T  WHERE Cd_Tema in (SELECT distinct CD_TEMA FROM mec_painel.TB_OBJETIVO WHERE CD_MAPA = '" . $_REQUEST['cmbMapa'] . "') ORDER BY CD_TEMA";

				$query_tema = pg_query($sql);
				while ($row_tema = pg_fetch_array($query_tema))
				{	
				#	echo "<td align='center' class='cabecalho_mapa' width='30%'>" . trim($row_tema["nm_tema"]) . "</td>";
				#	echo "\n";
					$qt_tema++;
				}
	?>
			<table id="tabela-1">
				<colgroup>  
				   <col style="width: 10%" />  
				   <col style="width: 30%" />  
				   <col style="width: 30%" />  
				   <col style="width: 30%" />  
				</colgroup>
				<thead>
					<tr>
						<th scope="col">Perspectiva</th>
						<th scope="col">Acesso e Perman&ecirc;ncia</th>
						<th scope="col">Qualidade</th>
						<th scope="col">Equidade</th>
					</tr>
				</thead>
				<tbody>
			
	<?php
			}
			else
			{ ?>
			<table id="tabela-2">
				<colgroup>  
					<col style="width: 10%" />  
					<col style="width: 90%" />  
				</colgroup>
				<thead>
					<tr>
						<th scope="col">Perspectiva</th>
						<th scope="col">Objetivos</th>
					</tr>
				</thead>
				<tbody>						
		 <?	}		
    		
	    }
 	    
	    #fazer a montagem dos objetivos
	    
            $sql = 
				"SELECT o.*, p.*, t.*, o.cd_tema as cd_tema_aux,
				(SELECT COUNT(*)
					FROM mec_painel.TB_INDICADOR I
					WHERE I.CD_OBJETIVO = O.CD_OBJETIVO AND I.CD_DESATUALIZADO = '1') AS QT_DESATUALIZADO,
				(SELECT COUNT(*) 
					FROM mec_painel.TB_INDICADOR I
					WHERE I.CD_OBJETIVO = O.CD_OBJETIVO AND I.CD_STATUS = '1') AS QT_OK,
				" . "
				(SELECT COUNT(*)
					FROM mec_painel.TB_INDICADOR I
					WHERE I.CD_OBJETIVO = O.CD_OBJETIVO AND I.CD_STATUS = '2') AS QT_ALERTA,
				(SELECT COUNT(*)
					FROM mec_painel.TB_INDICADOR I
					WHERE I.CD_OBJETIVO = O.CD_OBJETIVO AND I.CD_STATUS = '3') AS QT_CRITICO
				" .  "
				FROM mec_painel.TB_OBJETIVO O 
				INNER JOIN mec_painel.TB_PERSPECTIVA P on P.cd_perspectiva = O.cd_perspectiva
				left outer join mec_painel.tb_tema t on t.cd_tema = o.cd_tema
				WHERE o.CD_MAPA = '" . $_REQUEST['cmbMapa'] . 
				"' ORDER BY P.NR_ORDEM_APRESENTACAO, t.cd_tema, O.NR_ORDEM_APRESENTACAO ";
	    $query = pg_query($sql);

	    $cd_perspectiva = "";
	    $cd_tema = "";

		
	    While ($row = pg_fetch_array($query))
	    {
		
		if ( $cd_perspectiva != $row["cd_perspectiva"])
		{			
			$qtd = 0;
			
			if ($cd_perspectiva != "")
			{
				echo "</td></tr>";
				echo "\n";
			}

			echo '<tr>';
			echo "\n";
			echo '<td class="bold">'. $row["nm_perspectiva"] . "</td>";
			echo "\n";
			
			if ($ind_corporativo != "S") {
				echo '<td>';
			}
			
			$cd_perspectiva = $row["cd_perspectiva"];
			$cd_tema = "";
		}

		if ($ind_corporativo == "S")
		{
			if ( ($cd_tema != $row["cd_tema_aux"]) || ($row["cd_tema_aux"] == null)  )
			{
				if ($cd_tema != "")
				{
					echo "</td>";
					echo "\n";
				}

				if ( ($row["cd_tema_aux"] == "0")  )
				{
					echo "<td align='center' colspan='". $qt_tema . "' valign='center'><table cellpadding='10' cellspacing='10'><tr><td>";
					echo "\n";
				}
				else
				{
					$qtd = 0;
					echo "<td>";
					echo "\n";
				}

				$cd_tema = $row["cd_tema_aux"];		

			}
		}

		// define a cor do objetivo estratégico
		if ($row["qt_critico"] > 0) {
			$cor = "cor_critico";
		} else {
			if ($row["qt_alerta"] > 0) {
				$cor = "cor_alerta";
			}else {
				$cor = "cor_ok";
			}
		}
		
		// verifica se o objetivo deve ser mardado com tendo algum indicador desatualizado
		$objetivo_desatualizado = 0;
		if ($row["qt_desatualizado"] > 0) {
			$objetivo_desatualizado = 1;
		}
		
		$qtd += 1;
		$tam = min (strlen (trim($row["nm_objetivo"])), 50);
		if ($tam <= 25) {$tam = 25;}
		if ($ind_corporativo == "N") {$tam = min($tam, 16);}


		//montar lista de indicadores, com variação de cor

		$sql_ind =
			"SELECT *
			FROM mec_painel.TB_INDICADOR
			where cd_objetivo = " . $row["cd_objetivo"] . "
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
				} # switch
				
				$desatualizado = $row_ind["cd_desatualizado"];
					
				$indicadores = $indicadores . '<li class=&quot;' . $cor_led . '&quot;>';
				$indicadores = $indicadores . '<a href=&quot;serie_indicador.php?indicador=';
				$indicadores = $indicadores . $row_ind["nm_indicador"] . '&quot;>';
				$indicadores = $indicadores . trim($row_ind["nm_indicador"]);
				$indicadores = $indicadores . '</a>';
				if ($desatualizado) {
					$indicadores = $indicadores . '&nbsp;<img src=&quot;img/ic_desatualizado.png&quot;>';
				} # if
				$indicadores = $indicadores . '</li>';
			} # while
			$indicadores = $indicadores . "</ul></div>";
		}
		else
		{
			$indicadores = "Nenhum indicador encontrado";
		} # if

		

	if ($ind_corporativo == "S")
	{
		echo '<ul id="opcoes">';
	} else {
		echo '<ul id="opcoes-hor">';
	} # if
?>
		<li class="<?echo $cor?>">
			<? if ($objetivo_desatualizado) {?><ul><li class="cor_desatualizado"> <? }?>
					<a href="ficha_objetivo.php?cd_objetivo= <? echo $row["cd_objetivo"] ?>" 
						onclick="newWindow(this.href); return false;"
						onMouseOver="Tip('<?echo $indicadores?>',FIX, [this, 0, -2]);" 
						onMouseOut="UnTip();">
						<? echo trim($row["nm_objetivo"]) ?>
					</a>
			<? if ($objetivo_desatualizado) {?></li></ul> <? }?>
		</li>
	</ul>

<?php

		echo "\n";

	    } #  while

	if ($ind_corporativo == "S")
	{
		echo "</td>";
		echo "\n";
	} # if
?>
	</tbody>
	</table>
	</div> <!-- fecha tabela -->
	<div id="legenda-cores"> <img src="img/cores-status.gif" alt="legenda das cores" /></div>
<?
} # if
?>
	</div> <!-- fecha conteudo -->

</div> <!-- fecha pagina -->
<?php include 'rodape.php' ?>

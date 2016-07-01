<style>
table,tr,td {
	border-spacing: 0.2px;
}

td {
	padding: 4px;
	border-radius: 3px;
}

.tabela>tbody>tr:nth-child(2n)>td,.tabela>tbody>tr:nth-child(2n)>th {
	background-color: #f5f5f5;
}

.tabela>tbody>tr:nth-child(2n+1)>td,.tabela>tbody>tr:nth-child(2n+1)>th
	{
	background-color: #EEEEEE;
}

.tabela>tbody>tr:nth-child(2n+1)>th {
	background-color: #f00;
}

.tabela>tbody>tr:hover>td,.tabela>tbody>tr:hover>th {
	background-color: #e5e5e5;
}
</style>

<?
extract ( $_POST );
$preid = $_REQUEST ['preid'];
$sql = "SELECT distinct ipp.hupid 
          FROM rehuf.itenspregaopreenchido ipp 
     LEFT JOIN rehuf.huparticipante hup ON hup.hupid=ipp.hupid 
         WHERE preid = {$preid}
           AND ippstatus = 'A'
           AND ippquantidadeanual is not null";
$arrParticipante = $db->carregar($sql);         
$qtdParticipante = ($arrParticipante)? count($arrParticipante) : 0;
      
$sql = "SELECT *
			FROM 
			  rehuf.item i 
			  inner join rehuf.itemgrupo ig 
			  	on (i.iteid = ig.iteid) 
			  inner join rehuf.grupoitens g
			  	on (ig.gruid = g.gruid)
			  inner join rehuf.itemgrupopregao igp
    			on (ig.itgid = igp.itgid) 
    		  left join rehuf.itemlote il 
    		  	on (il.itlid = i.itlid)
			WHERE i.itestatus = 'A'
			AND g.grustatus = 'A'
			AND ig.itgstatus = 'A'
			AND igp.igpstatus = 'A'
			AND igp.preid in ({$preid}) 
			ORDER BY i.itlid, i.itedescricao, g.gruid, i.itecatmat, i.iteapresentacao";

$itens = $db->carregar ( $sql );

$sql = "SELECT ent.entnome, ena.entsig, ent.entid FROM entidade.entidade ent 
		LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
		LEFT JOIN entidade.funentassoc fue ON fue.fueid = fen.fueid
		LEFT JOIN entidade.entidade ena ON ena.entid = fue.entid 
		LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid 
		WHERE fen.funid IN ('" . HOSPITALUNIV . "','" . HOSPITALFEDE . "') AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) 
		ORDER BY ena.entsig";

$entidades = $db->carregar ( $sql );

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<table width="100%" border="0" cellpadding="0" cellspacing="0"
	class="notscreen1 debug" style="border-bottom: 1px solid;">
	<tbody>
		<tr bgcolor="#ffffff">
			<td valign="top" width="50" rowspan="2">
                           <?php if($_POST ['buscar'] != '2'){ ?>
                             <img src="../imagens/brasao.gif" width="45" height="45" border="0">
                           <?php }else{ echo "&nbsp;"; }?></td>
			<td nowrap="" align="left" valign="middle" height="1"
				style="padding: 5px 0 0 0;">SIMEC- Sistema Integrado do Ministério
				da Educação<br> MEC / SE - Secretaria Executiva <br>
			</td>
			<td align="right" valign="middle" height="1"
				style="padding: 5px 0 0 0;">Impresso por: <b><?php echo $_SESSION['usunome']?></b><br> Hora da Impressão:<?php echo date("d/m/y"). " - ".date("H:i:s");?><br>
			</td>
		</tr>
		<tr bgcolor="#ffffff">
			<td colspan="2" align="center" valign="top"
				style="padding: 5 0 5px 0;"><b><font style="font-size: 14px;">RELATÓRIO DE ANÁLISE ESTATÍSTICA POR PREGÃO - REHUF</font></b>
			</td>
		</tr>
	</tbody>
</table>
<?php
if ($_POST ['buscar'] != "2") :
	?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css' />
<?php endif;?>
<table class="tabela" align="center" bgcolor="#f5f5f5" border="1px solid #000000;">
	<tr>
		<td class="SubTituloEsquerda"
			colspan="<?php echo (count($entidades)*2)+6?>"><? echo strtoupper($db->pegaUm("SELECT preobjeto FROM rehuf.pregao WHERE preid='".$_REQUEST['preid']."'")); ?></td>
	</tr>
	<tr>
		<td rowspan="2" class="SubTituloCentro">DESCRIÇÃO</td>
		<td rowspan="2" class="SubTituloCentro">APRESENTAÇÃO</td>
		<td rowspan="2" class="SubTituloCentro">CATMAT</td>
<?
if ($entidades [0]) {
	foreach ( $entidades as $entidade ) {
		$resumoent = explode ( " ", $entidade ['entnome'] );
		unset ( $hospabrv );
		for($i = 0; $i < count ( $resumoent ); $i ++) {
			$hospabrv .= strtoupper ( substr ( trim ( $resumoent [$i] ), 0, 1 ) );
		}
		echo "<td colspan='2' class='SubTituloCentro' title='" . $entidade ['entnome'] . "'>" . (($entidade ['entsig']) ? $entidade ['entsig'] . "-" : "") . $hospabrv . "</td>";
	}
}
?>
                <td colspan="1" rowspan="2" class="SubTituloCentro" title="Representa o somatório das Quantidades Anual dos HUs.">TOTAL<br/>QTD ANUAL</td>
                <td colspan="1" rowspan="2" class="SubTituloCentro" title="Representa Quantidade total de HUs x Quantidade total de HUs que participaram do pregão.">TOTAL HUs<br/> PARTICIPANTES</td>
				<td colspan="1" rowspan="2" class="SubTituloCentro" title="Apresenta o percentual referente a Qtd Anual (TOTAL) por Qtd de HUs que participaram do pregão (TOTAL (n)).">PARTICIPAÇÃO<br/>DOS HUs NO ITEM (%)</td>
	</tr>
	<tr>
<?
if ($entidades [0]) {
	foreach ( $entidades as $entidade ) {
		echo "<td class='SubTituloCentro' style='font-size:9px;' nowrap>QTD Anual</td>";
		echo "<td class='SubTituloCentro' style='font-size:9px;' nowrap>%</td>";
	}
}
  ?>
	
</tr>
<?
if ($itens [0]) {
	foreach ( $itens as $i => $itm ) {
		if (fmod ( $i, 2 ) == 0)
			$marcado = '';
		else
			$marcado = '#F7F7F7';
		?>
		<tr bgcolor="<? echo $marcado; ?>"
		onmouseover="this.bgColor='#ffffcc';"
		onmouseout="this.bgColor='<? echo $marcado; ?>';">
		<td><? echo (($itm['itedescricao'])?$itm['itedescricao']:"-"); ?></td>
		<td><? echo (($itm['iteapresentacao'])?$itm['iteapresentacao']:"-"); ?></td>
		<td><? echo (($itm['itecatmat'])?$itm['itecatmat']:"-"); ?></td>
<?
		$totalizador = 0;
		$n = 0;
		foreach ( $entidades as $entidade ) {
			$sql = "SELECT ippquantidadeanual FROM rehuf.itenspregaopreenchido ipp 
						LEFT JOIN rehuf.huparticipante hup ON hup.hupid=ipp.hupid 
						WHERE igpid='" . $itm ['igpid'] . "' AND entid='" . $entidade ['entid'] . "' AND preid='" . $_REQUEST ['preid'] . "' AND ippquantidadeanual is not null";
			
			$sqlSoma = "SELECT SUM(ippquantidadeanual) FROM rehuf.itenspregaopreenchido ipp 
						LEFT JOIN rehuf.huparticipante hup ON hup.hupid=ipp.hupid 
						WHERE igpid='" . $itm ['igpid'] . "' AND preid='" . $_REQUEST ['preid'] . "' AND ippquantidadeanual is not null";
			$totalItem = $db->pegaUm ( $sqlSoma );
			$ippquantidadeanual = $db->pegaUm ( $sql );
			$totalizador += $ippquantidadeanual;
			
			if ($ippquantidadeanual > 0) {
				$porcentagemTotalTexto = round ( ($ippquantidadeanual / $totalItem) * 100, 1 ) . "%";
				$porcentagemTotal = ($ippquantidadeanual / $totalItem) * 100;
				$n ++;
			} else {
				$porcentagemTotalTexto = "-";
				$porcentagemTotal = null;
			}
			$verificar = false;
			if ($ippquantidadeanual) {
				echo "<td style='text-align:center'>" . $ippquantidadeanual . "</td>";
			} else {
				echo "<td style='text-align:center'>-</td>";
			}
			if ($inferioritem != "" || $superioritem != "") {
				if ($inferioritem != "") {
					if ($porcentagemTotal != null && $inferioritem >= $porcentagemTotal) {
						echo "<td style='text-align:center; background-color:#F7FE2E'>" . $porcentagemTotalTexto . "</td>";
						$verificar = true;
					}
				}
				if ($superioritem != "") {
					if ($porcentagemTotal != null && $porcentagemTotal >= $superioritem) {
						echo "<td style='text-align:center; background-color:#FE2E2E; color:white'>" . $porcentagemTotalTexto . "</td>";
						$verificar = true;
					}
				}
				if (! $verificar) {
					echo "<td style='text-align:center'>" . $porcentagemTotalTexto . "</td>";
				}
			} else {
				echo "<td style='text-align:center'>" . $porcentagemTotalTexto . "</td>";
			}
		}
		
		?>
		<td style="text-align: center"><strong><? echo $totalizador; ?></strong></td>
		<td style="text-align: center"><strong><? echo $n?></strong></td>
		<?php
		$porcentagemHU = ($n /$qtdParticipante * 100);
		$porcentagemHUtexto = round ( ($n / $qtdParticipante * 100), 1 ) . "%";
                $verificar2 = false;
		?>
		<?php
		
		if ($inferiorHU != "" || $superiorHU != "") {
			if ($inferiorHU != "") {
				if ($porcentagemHU != null && $inferiorHU >= $porcentagemHU) {
					echo "<td style='text-align:center; background-color:#FE642E; color:white;'>" . $porcentagemHUtexto . "</td>";
					$verificar2 = true;
				}
			}
			if ($superiorHU != "") {
				if ($porcentagemHU != null && $porcentagemHU >= $superiorHU) {
					echo "<td style='text-align:center; background-color:#40FF00;'>" . $porcentagemHUtexto . "</td>";
					$verificar2 = true;
				}
			}
			if (! $verificar2) {
				echo "<td style='text-align:center'>" . $porcentagemHUtexto . "</td>";
			}
		} else {
			echo "<td style='text-align:center'>" . $porcentagemHUtexto . "</td>";
		}
		?>
		<?php $n=0;?>
	</tr>
<?
	}
	
} else {
	?>
<tr>
		<td class="SubTituloEsquerda"
			colspan="<? echo ((count($entidades)*2)+7); ?>">Não existem itens
			cadastrados</td>
	</tr>
<?
}
?>
  <?php
		if ($_POST ['buscar'] == "2") {
			header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT" );
			header ( "Last-Modified: " . gmdate ( "D,d M YH:i:s" ) . " GMT" );
			header ( "Pragma: no-cache" );
			header ( "Content-type: application/xls; name=REHUF_RelatorioAnalise_" . date ( "Ymdhis" ) . ".xls" );
			header ( "Content-Disposition: attachment; filename=REHUF_RelatorioAnalise_" . date ( "Ymdhis" ) . ".xls" );
		header ( "Content-Description: MID Gera excel" );
  }
  ?>
<?php if($_POST ['buscar'] != '2'){ ?>
	<table>
		<tr>
			<td>
				<?php if($inferioritem != ""){?>
					<img alt="" src="../imagens/quad_amarelo.png">&nbsp;<b>Preenchimento item inferior <?php echo $inferioritem?>%</b>
				<?php }?>
			</td>
			<td>
				<?php if($superioritem != ""){?>
					<img alt="" src="../imagens/quad_vermelho.png">&nbsp;<b>Preenchimento item superior <?php echo $superioritem?>%</b>
				<?php }?>
			</td>
			<td>
				<?php if($inferiorHU != ""){?>
					<img alt="" src="../imagens/quad_laranja.png">&nbsp;<b>Participação do HU inferior <?php echo $inferiorHU?>%</b>
				<?php }?>
			</td>
			<td>
				<?php if($superiorHU != ""){?>
					<img alt="" src="../imagens/quad_verde.png">&nbsp;<b>Participação do HU superior <?php echo $superiorHU?>%</b>
				<?php }?>
			</td>
		</tr>
	</table>
<?php }?>
  <br/><div class="notprint" align="center">
                     <input type="button" value="Imprimir" 
                         class="notprint" style="cursor: pointer, info{ display: none; }" onclick="self.print();">
                 </div>        
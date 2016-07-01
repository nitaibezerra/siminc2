<?php
$sql = "SELECT DISTINCT
			ini.iniid,
   			g.guinome,
   			g.guijustificativa,
   			g.guiid ,
   			ini.ininome,
   			gi.ginidetalhamento,
                gi.ginid
		FROM
            emenda.guia g
                inner join emenda.guia_guiainiciativa ggi on ggi.guiid = g.guiid
                inner join emenda.guiainiciativa gi on gi.ginid = ggi.ginid
                inner join emenda.iniciativa ini on ini.iniid = gi.iniid
		WHERE
			g.guiid = {$_REQUEST['guiid']}
    		and g.guistatus = 'A'
            and gi.ginistatus = 'A'";
$arrDados = $db->pegaLinha( $sql );
$tipo = $_REQUEST['tipo'];
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<table align="center" class="Tabela" cellpadding="2" cellspacing="1">
	 <tbody>
	 	<tr>
	 		<td colspan="4" class="subtitulocentro">Dados da Guia</td>
	 	</tr>
	 	<tr>
			<td style="text-align: right; width:20.0%;" class="SubTituloEsquerda">Guia:</td>
			<td><?=$arrDados['guinome']?></td>
		</tr>
		<tr>
			<td style="text-align: right;" class="SubTituloEsquerda">Iniciativa:</td>
			<td><?=$arrDados['ininome']?></td>
		</tr>
		<?if( $tipo == 'just' ){ ?>
		<tr>
			<td style="text-align: right; width:20.0%;" class="SubTituloEsquerda">Justificativa:</td>
			<td><?=$arrDados['guijustificativa']; ?></td>
		</tr>
		<?}else{ ?>
		<tr>
			<td style="text-align: right;" class="SubTituloEsquerda">Detalhamento da Iniciativa:</td>
			<td><?=$arrDados['ginidetalhamento']; ?></td>
		</tr>
		<?} ?>
		<tr>
			<th align="center" colspan="2">
				<input type="button" value="Fechar" name="btnFechar" id="btnFechar" onclick="window.close();"/></th>
		</tr>
	 </tbody>
</table>
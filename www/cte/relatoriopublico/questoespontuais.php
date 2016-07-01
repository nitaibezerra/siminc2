<?php

$inuid =  $_REQUEST['inuid'];
$itrid =  $_REQUEST['itrid'];


$sql = sprintf(
				"select d.dimid as codigo, d.dimdsc as descricao
				from cte.dimensao d
				where d.dimstatus = 'A' and d.itrid = %d
				and exists ( select p.prgid from cte.pergunta p where p.dimid = d.dimid )
				order by d.dimcod",
				$itrid
			);
$dimensao = $db->carregar( $sql );

?>
<script type="text/javascript">
	function selecionarDimensao( dimid ){
		window.location = '?modulo=relatorio/questoes_pontuais&acao=A&dimid=' + dimid;
	}
</script>

<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'/>
<table border="0" style="border:1px solid #CCCCCC; margin-right:2cm; margin-left:0.8cm; width: 700pt;">
	<tr> 
		<td class="tituloPrincipalAbas"><b>SINTESE DAS QUESTÕES PONTUAIS</b></td>
	</tr>
</table>

<div style="border:1px solid #CCCCCC; margin-right:2cm; margin-left:0.8cm; width: 700pt;">
<div style=" width:200px; height:5px;" ></div>
<?php if( is_array( $dimensao ) && count( $dimensao ) > 0 ): ?>
<?php foreach( $dimensao as $item ): ?>
	<table class="tabela" bgcolor="#f5f5f5" style=" margin-bottom:5px;margin: auto; width: 98%;" cellSpacing="1" cellPadding="3" align="center">
		<tbody class="SubTituloEsquerda" style=" background-color:#999999; ">
		<tr>
		<td bgcolor="#7e8e47" style=" font-weight:bold;"><?=$item['descricao']; ?></td>
		</tr>
		</tbody>
		<?php 
		$dimid = (integer) $item['codigo'];
		$restricao = $dimid ? " and d.dimid = {$dimid} " : "";
				$sql = sprintf(
			"select p.*, r.*
			from cte.resposta r
			inner join cte.pergunta p on p.prgid = r.prgid
			inner join cte.dimensao d on d.dimid = p.dimid and d.dimstatus = 'A' %s
			where r.inuid = %d
			order by d.dimcod, p.prgcod
			",
			$restricao,
			$inuid
		);
		$respostas = $db->carregar( $sql );
		
		if( is_array( $respostas ) && count( $respostas ) > 0 ): ?>
	<?php foreach( $respostas as $resposta ): ?>
		<tbody>
			<tr>
				<td bgcolor="#acbc73"  style="padding: 10px;"><?= $resposta['prgdsc'] ?></td>
			</tr>
			<tr>
				<td style="padding: 20px;"><?= $resposta['rspdsc'] ?></td>
			</tr>
			</tbody>
		<?php endforeach; ?>
<?php endif; ?>
	</table>
			<?php endforeach; ?>
<?php endif; ?>
<div style=" width:200px; height:5px;" ></div>
</div>
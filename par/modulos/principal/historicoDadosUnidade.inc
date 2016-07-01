<?php
$obDadosUniade = new DadosUnidade();

$entid  = $_REQUEST['entid'];
$muncod = $_REQUEST['muncod'];
$estuf  = $_REQUEST['estuf'];
$funid  = $_REQUEST['funid'];

$stFuncaoEntidade = $_GET['stFuncaoEntidade'];

if($entid){
	$arDados = $obDadosUniade->recuperarHistoricoEntidade($entid, $stFuncaoEntidade);
}
//ver($arDados, d);
$arDados = $arDados ? $arDados : array();
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<table border="0" align="center" width="100%" cellspacing="1" cellpadding="3" class="tabela">
	<tbody>
		<tr>
			<th colspan="3" class="tdDegradde02">Lista de Histórico (<?php echo $obDadosUniade->recuperarNomeFuncaoEntidade($funid)?>)</th>
		</tr>
		<tr>
			
		</tr>
	</tbody>
</table>
<table border="0" align="center" width="95%" cellspacing="0" cellpadding="2" class="listagem">
	<thead>
		<tr>
			<td height="25" bgcolor="" align="" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title">
				<strong>Ação</strong>
			</td>
			<td align="" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title">
				<strong>Usuário</strong>
			</td>
			<td bgcolor="" align="" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title">
				<strong>Data de alteração</strong>
			</td>
			<td align="" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title">
				<strong>Entidade</strong>
			</td>
			<td align="" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title">
				<strong>Alteração feita</strong>
			</td>
		</tr> 
	</thead>
	<tbody>
		<?php if(is_array($arDados) && count($arDados) > 0):?>
			<?php foreach($arDados as $dados): ?>
			  <tr bgcolor="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#ffffcc';">
			  	<td height="25" align="left" title="Ação">
					<!-- a onclick="mostraParecerFinaceiro(3620);" style="margin: 0pt -20px 0pt 20px; cursor: pointer;">
						<img border="0" title="Selecionar" src="/imagens/alterar.gif"></a -->
					<?php echo $dados['hstacao']?>
				</td>
			    <td><?php echo $dados['usunome']?></td>
			    <td><?php echo $dados['hstdata']?></td>
			    <td><?php echo $dados['entnome']?></td>
			    <td><?php echo $dados['alteracao_feita']?></td>
			  </tr>
			<?php endforeach; ?>
		<?php else: ?>
			<tr>
				<td height="25" colspan="4">
					<center>Sem registros.</center>
				</td>
			</tr>	
		<?php endif; ?>
		<tr>
			<td colspan="5" style="border-top:2px solid black;">
				<b>Total:</b> <?php echo count($arDados); ?>
			</td>
		</tr>
	</tbody>
</table>
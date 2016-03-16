<?php 
if( $_REQUEST['req'] == 'excluir' ){
	if( cte_possuiPerfil(array(CTE_PERFIL_SUPER_USUARIO,CTE_PERFIL_ADMINISTRADOR)) ){
		$sql = "UPDATE
					cte.monitoramentohistorico
				SET
					mhistatus = 'I'
				WHERE
					mhiid = {$_REQUEST['mhiid']}";
		$db->executar($sql);
		$db->commit();
	}else{
		echo "<script>alert('Operação não permitida');</script>";
	}		
}

if($_REQUEST['mosid']){
	
	$sql = "SELECT 
				mhiid, 
				mhidsc, 
				mhidatainicio, 
				mhidatafinal, 
				mhidscanexo,
				to_char(mhidataatualizacao, 'DD/MM/YYY')  as data,	
				to_char(mhidataatualizacao, 'HH:MM:SS')  as horario	
			FROM 
				cte.monitoramentohistorico
			WHERE
				mosid = {$_REQUEST['mosid']}
				AND mhistatus = 'A'  
			ORDER BY
				mhiid,
				mhidatainicio,
				mhidatafinal,
				mhidscanexo";
	
	$historico = $db->carregar($sql);
	
	$ordem = 1;
?>
<script>

var superuser = <?=cte_possuiPerfil(array(CTE_PERFIL_SUPER_USUARIO,CTE_PERFIL_ADMINISTRADOR)) ?>;

function excluirHistorico( mhiid ){
	if( confirm('Realmente deseja exluir este registro?') && superuser ){
		document.getElementById('req').value  = 'excluir';
		document.getElementById('mhiid').value = mhiid;
	
		document.getElementById('frmHistorico').submit();
	}else{
		alert('Operação não permitida!');
	}
}
</script>
<head>
	<meta http-equiv="Cache-Control" content="no-cache">
	<meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="-1">
    <title>PAR - Brasil Profissionalizado - Monitoramento Subação - Histórico de Monitoramento </title>

    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
</head>
<form method="post" id="frmHistorico" enctype="multipart/form-data" action="">
<table class="listagem" width="95%">
	<input type="hidden" id="req"   name="req"   value="">
	<input type="hidden" id="mhiid" name="mhiid" value="">
	<tr style="border-bottom: 2px solid rgb(0,0,0);">
		<td colspan="2" width="5%" align="center">
			Data/Ação
		</td>
		<td width="45%" align="center">
			Fase
		</td>
		<td width="15%" align="center">
			Data de Inicio
		</td>
		<td width="15%" align="center">
			Data de Fim
		</td>
		<td width="15%" align="center">
			Anexo
		</td>
	</tr>
<?php 
		if(is_array( $historico )){
			foreach( $historico as $dado ){
				
				if( cte_possuiPerfil(array(CTE_PERFIL_SUPER_USUARIO,CTE_PERFIL_ADMINISTRADOR)) ){
					$acao = "excluirHistorico(".$dado['mhiid'].");";
					$icone = "excluir.gif";
				}else{
					$acao = "";
					$icone = "excluir_01.gif";
				}
				
				if( ($ordem % 2) == 1 ){
					$cor = "rgb(240,240,240)";
				}else{
					$cor = "rgb(250,250,250)";
				}
			
?>	
	<tr style="border-bottom: 2px solid <?=$cor ?>;" height="50px">
		<td align="center" valign="center" style="background-color: <?=$cor ?>">
			<b><?=$dado['data'] ?></b><br />
			<b><?=$dado['horario'] ?></b>
		</td>
		<td align="center" valign="center" style="background-color: <?=$cor ?>">
			<img border="0" title="Excluir Registro" src="/imagens/<?=$icone ?>" onclick="<?=$acao ?>">
		</td>
		<td align="center"><?=$dado['mhidsc'] ?></td>
		<td align="center"><?=$dado['mhidatainicio'] ?></td>
		<td align="center"><?=$dado['mhidatafinal'] ?></td>
		<td align="center"><?=$dado['mhidscanexo'] ?></td>
	</tr>		
<?php 		
			$ordem++;	
			}
		}else{
?>
	<tr>
		<td colspan="3" style="padding: 5px;">
			<label style="color:red; "> Não existem registros de histórico.</label>
		</td>
	</tr>
<?php 			
		}
?>		
</table>
</form>
<?php 
	}else{
?>
<script type="text/javascript">
    alert('Favor escolher lista de respostas no monitoramento desejado.');
    window.close();
</script>

<?php
		
	}

?>
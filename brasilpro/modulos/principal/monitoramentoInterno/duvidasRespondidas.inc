<?php 
	if($_REQUEST['mosid']){
		
		if( cte_possuiPerfil(CTE_PERFIL_MONITORA_SUBACAO) ){
			$sql = "UPDATE 
						cte.monitoramentoduvidas
					SET 
						mdusituacao = 'S'
					WHERE
						mosid 	    = {$_REQUEST['mosid']} AND
						mdusituacao = 'A'";
			$db->executar($sql);
			$db->commit();
		}
		
		$sql = "SELECT
					mdupergunta,
					CASE WHEN mduresposta is null
						THEN '<label style=\"color: red;\"> Não respondido. </label>'
						ELSE mduresposta
					END as resposta,
					mdudatapergunta,
					mdudataresposta
				FROM
					cte.monitoramentoduvidas
				WHERE
					mosid 	    = {$_REQUEST['mosid']}
				ORDER BY
					mdudatapergunta,
					resposta,
					mdupergunta,
					mduresposta";
		
		$duvidas = $db->carregar($sql);
		
		$ordem = 1;
?>
<head>
	<meta http-equiv="Cache-Control" content="no-cache">
	<meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="-1">
    <title>PAR - Brasil Profissionalizado - Monitoramento Subação - Duvidas Respondidas </title>

    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
</head>
<table class="listagem" width="95%">
	<tr style="border-bottom: 2px solid rgb(0,0,0);">
		<td width="5%" align="center">
			Ordem
		</td>
		<td width="15%" align="center">
			Data da Pergunta/Resposta
		</td>
		<td>
			Pergunta/Resposta
		</td>
	</tr>
<?php 
		if(is_array($duvidas)){
			foreach( $duvidas as $duvida ){
				
				if( ($ordem % 2) == 1 ){
					$cor = "rgb(240,240,240)";
				}else{
					$cor = "rgb(250,250,250)";
				}
			
?>	
	<tr style="border-bottom: 2px solid <?=$cor ?>;">
		<td rowspan="2" align="center" valign="center" style="background-color: <?=$cor ?>"><b><?=$ordem ?></b></td>
		<td align="center" style="padding: 5px; background-color: <?=$cor ?>"><?=formata_data($duvida['mdudatapergunta']) ?></td>
		<td style="background-color: <?=$cor ?>"><b> Pergunta: </b><?=$duvida['mdupergunta'] ?></td>
	</tr>		
	<tr style="border-bottom: 2px solid <?=$cor ?>;">
		<td align="center" style="padding: 5px;"><?=formata_data($duvida['mdudataresposta']) ?></td>
		<td><b> Resposta: </b><?=$duvida['resposta'] ?></td>
	</tr>		
<?php 		
			$ordem++;	
			}
		}else{
?>
	<tr>
		<td colspan="3" style="padding: 5px;">
			<label style="color:red; "> Não existem Perguntas respondidas para este item do monitoramento.</label>
		</td>
	</tr>
<?php 			
		}
?>		
</table>
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
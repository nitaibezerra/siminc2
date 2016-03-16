<?php

$sql = "select m.mundescricao || ' - ' || m.estuf as municipio 
		from territorios.municipio m 
			inner join cte.instrumentounidade iu on iu.muncod = m.muncod
		where iu.inuid = {$_REQUEST['inuid']}";
		
$municipio = $db->pegaUm( $sql );		

if( $_REQUEST["boGravar"] ){
	
	$obEscolaAtiva = new EscolaAtiva( '', $_REQUEST["inuid"] );
	$obEscolaAtiva->excluirAdesao();
	
	echo "
			<script>
    			alert('Operação Efetuada com sucesso!');
    			window.opener.location.reload();
    			window.close();
    		</script>";
    exit();
	
}

?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<form action="" method="post" name="formulario">
					
					<input type="hidden" name="inuid" value="<?= $_REQUEST['inuid'] ?>"/>
					<input type="hidden" name="boGravar" value="1" />
					
					<div style="float: left;">
						<h4 align="center"><?php echo $municipio; ?></h4>
						<table border="0" cellpadding="3" cellspacing="0">
							<tr>
								<td valign="bottom">
									Alterar Situação da Adesão para:
									<br/>
									<?php 
									
									$arSituacao[0]["codigo"] = "2";
									$arSituacao[0]["descricao"] = "Não Confirmo a Adesão";
									
									$arSituacao[1]["codigo"] = "3";
									$arSituacao[1]["descricao"] = "Não há Classes Multisseriadas";
									
									$db->monta_combo( "inusituacaoadesao", $arSituacao , 'S', 'Selecione', '', '', '', '', '', 'inusituacaoadesao' ); ?>
								</td>
							</tr>

						</table>
						<br />
						<div style="text-align: center;">
							<input type="button" name="Gravar" value="Gravar" onclick="gravar();"/>
						</div>					
					</div>					
				</form>
			</td>
		</tr>
	</tbody>
</table>

<script type="text/javascript">
	function gravar(){
	
		if( !document.getElementById( 'inusituacaoadesao' ).value ){
			alert( 'O campo ["Alterar Situação da Adesão para"] é obrigatório!' );
			return false;
		}
	
		formulario.submit();
	}
	
</script>
<?php


include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório de Adesão ao PSE', '&nbsp;' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>

</head>
<body>
<form name="formulario" id="formulario" action="" method="post">

<table width="50%" class="listagem" cellspacing="3" cellpadding="3" bgcolor="#f5f5f5" align="center" >
		<tr>
			<td width="20%" class="SubTituloCentro">
				ESTADO
			</td>
			<td width="20%" class="SubTituloCentro">
				Secretário(a)
			</td>
			<td class="SubTituloCentro">
				Nome
			</td>
			<td width="20%" class="SubTituloCentro">
				Aderiu?
			</td>
		</tr>
		<?
		$sql = "select estuf from territorios.estado order by 1";
		$dados = $db->carregar($sql);
		
		foreach($dados as $d){
			
			$sql = "select usucpfseceducacao, usucpfsecsaude from par.pfpse 
					where estuf = '".$d['estuf']."'";
			$dadospse = $db->pegaLinha($sql);
			if($dadospse){
				$usucpfseceducacao = $dadospse['usucpfseceducacao'];
				$usucpfsecsaude = $dadospse['usucpfsecsaude'];
			}
			else{
				$usucpfseceducacao = "";
				$usucpfsecsaude = "";
			}
			
			?>
				<tr>
					<td align="center" rowspan="2">
						<?=$d['estuf'];?>
					</td>
					<td align="center">
						EDUCAÇÃO
					</td>
					<td nowrap="nowrap">
						<?
						$sql = "select usunome from seguranca.usuario u
								inner join par.usuarioresponsabilidade ur 
									on ur.usucpf = u.usucpf 
									and ur.rpustatus = 'A'
									and ur.pflcod = ".PAR_PERFIL_SECRETARIO_ESTADUAL_EDUCACAO."
									and ur.estuf = '".$d['estuf']."'
								inner join seguranca.usuario_sistema us 
									on us.usucpf = ur.usucpf 
									and us.suscod='A' 
									and us.sisid = ".SISID_PAR;
						$nomeSecEduc = $db->pegaUm($sql);
						if($nomeSecEduc){
							echo $nomeSecEduc;
						}
						else{
							echo '<font color=red>Perfil não vinculado ao Secretário Estadual de Educação para este Estado.</font>';
						}
						?>
					</td>
					<td align="center">
						<?
							if($usucpfseceducacao){
								echo '<font color=blue>SIM</font>';
							}
							else{
								echo '<font color=red>NÃO</font>';
							}
						?>
					</td>
				</tr>
				<tr>
					
					<td align="center">
						SAÚDE
					</td>
					<td nowrap="nowrap">
						<?
						$sql = "select usunome from seguranca.usuario u
								inner join par.usuarioresponsabilidade ur 
									on ur.usucpf = u.usucpf 
									and ur.rpustatus = 'A'
									and ur.pflcod = ".PAR_PERFIL_SECRETARIO_ESTADUAL_SAUDE."
									and ur.estuf = '".$d['estuf']."'
								inner join seguranca.usuario_sistema us 
									on us.usucpf = ur.usucpf 
									and us.suscod='A' 
									and us.sisid = ".SISID_PAR;
						$nomeSecSaude = $db->pegaUm($sql);
						if($nomeSecSaude){
							echo $nomeSecSaude;
						}
						else{
							echo '<font color=red>Perfil não vinculado ao Secretário Estadual de Saúde para este Estado.</font>';
						}
						?>
					</td>
					<td align="center">
						<?
							if($usucpfsecsaude){
								echo '<font color=blue>SIM</font>';
							}
							else{
								echo '<font color=red>NÃO</font>';
							}
						?>
					</td>
				</tr>
				<tr>
					<td colspan="4" bgcolor="#DCDCDC">&nbsp;</td>
				</tr>
		<?}?>
		</table>
								
</form>
</body>
</html>
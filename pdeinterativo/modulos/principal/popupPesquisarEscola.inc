<?php
if($_POST['requisicao']){
	$arrDados = $_POST['requisicao']();
}
if($_POST['requisicaoAjax']){
	$_POST['requisicaoAjax']();
	exit;
}

$arrPerfil = pegaPerfilGeral();

if(!$db->testa_superuser()) {
	
	if(in_array(PDEINT_PERFIL_COMITE_MUNICIPAL,$arrPerfil)){
		$sql = "select mun.muncod
				from pdeinterativo.usuarioresponsabilidade ur
				inner join territorios.municipio mun ON mun.muncod::integer = ur.muncod::integer
				where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = ".PDEINT_PERFIL_COMITE_MUNICIPAL."
				order by rpuid desc";
		
		$muncod = $db->pegaUm($sql);
		$paramperfil[] = "(pde.pdiesfera = 'Municipal' AND mun.muncod='".$muncod."' AND pde.pdigeridapde = TRUE)";
	}
	
	if(in_array(PDEINT_PERFIL_COMITE_PAR_MUNICIPAL,$arrPerfil)){
		$sql = "select mun.muncod
				from pdeinterativo.usuarioresponsabilidade ur
				inner join territorios.municipio mun ON mun.muncod::integer = ur.muncod::integer
				where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = ".PDEINT_PERFIL_COMITE_PAR_MUNICIPAL."
				order by rpuid desc";
		
		$muncod = $db->pegaUm($sql);
		$paramperfil[] = "(pde.pdiesfera = 'Municipal' AND mun.muncod='".$muncod."' AND pde.pdigeridapde = FALSE)";
	}
	
	if(in_array(PDEINT_PERFIL_COMITE_ESTADUAL,$arrPerfil)){
		$sql = "select estuf
				from pdeinterativo.usuarioresponsabilidade ur
				where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = ".PDEINT_PERFIL_COMITE_ESTADUAL."
				order by rpuid desc";

		$estuf = $db->pegaUm($sql);
		$paramperfil[] = "(pde.pdiesfera = 'Estadual' AND mun.estuf='".$estuf."' AND pde.pdigeridapde = TRUE)";
		
	}
	
	if(in_array(PDEINT_PERFIL_COMITE_PAR_ESTADUAL,$arrPerfil)){
		$sql = "select estuf
				from pdeinterativo.usuarioresponsabilidade ur
				where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = ".PDEINT_PERFIL_COMITE_PAR_ESTADUAL."
				order by rpuid desc";

		$estuf = $db->pegaUm($sql);
		$paramperfil[] = "(pde.pdiesfera = 'Estadual' AND mun.estuf='".$estuf."' AND pde.pdigeridapde = FALSE)";
		
	}
	
	if($paramperfil) {
		
		$arrWhere[] = "(".implode(" OR ",$paramperfil).")";
		
	} else {
		
		if($estuf){
			$arrWhere[] = "mun.estuf = '$estuf'";
		}
		if($muncod){
			$arrWhere[] = "mun.muncod = '$muncod'";
		}
		
	}
} else {
		
	if($estuf){
		$arrWhere[] = "mun.estuf = '$estuf'";
	}
	if($muncod){
		$arrWhere[] = "mun.muncod = '$muncod'";
	}
	
}


?>
<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<link rel="stylesheet" type="text/css" href="../pdeinterativo/css/pdeinterativo.css"/>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
		<script type="text/javascript" src="../pdeinterativo/js/pdeinterativo.js"></script>
	</head>
	<script>
		function pesquisar()
		{
			jQuery("[name=form_pesquisa]").submit();
		}
		
		function selecionaINEP(inep)
		{
			jQuery("[name=pdicodinep]",window.opener.document).val(inep);
			window.opener.carregaEscola();
			window.close();
		}
	</script>
	<body>
		<?php
		monta_titulo( "Pesquisar Escola", '&nbsp' );
		?>
		<form name="form_pesquisa" method="post" id="form_pesquisa" action="" />
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td class="SubTituloDireita" >Nome da Escola:</td>
					<td><?php echo campo_texto("pdenome","N","S","Nome","40","40","","","","","","","",$_POST['pdenome'])?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita" >Tipo de Órgão:</td>
					<td id="td_tpocod">
						<?php 
						$esfera = array(0 =>array('codigo'=>'1','descricao'=>'FEDERAL'),
							  			1 =>array('codigo'=>'2','descricao'=>'ESTADUAL'),
							  			2 =>array('codigo'=>'3','descricao'=>'MUNICIPAL'),
							  			3 =>array('codigo'=>'4','descricao'=>'PRIVADA')); 
						
						?>
						<?php $db->monta_combo("id_dependencia_adm",$esfera,($arrTravaConsulta['pdiesfera'] ? "N" : "S"),"Selecione...","filtraOrgao","","","","N","","",$_POST['id_dependencia_adm']) ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" >Estado:</td>
					<td>
						<?php $sql = "	select
											estuf as codigo,
											estdescricao as descricao
										from
											territorios.estado
										order by
											estdescricao" ?>
						<?php $db->monta_combo("estuf",$sql,($arrTravaConsulta['estuf'] ? "N" : "S"),"Selecione...","filtraMunicipio","","","","N","","",$_POST['estuf']) ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" >Município:</td>
					<td id="td_muncod">
						<?php if($_POST['estuf']): ?>
							<?php $sql = "	select
												muncod as codigo,
												mundescricao as descricao
											from
												territorios.municipio
											where
												estuf = '{$_POST['estuf']}'
											order by
												mundescricao" ?>
							<?php $db->monta_combo("muncod",$sql,($arrTravaConsulta['muncod'] ? "N" : "S"),"Selecione...","","","","","N","","",$_POST['muncod']) ?>
						<?php else: ?>
							<?php $db->monta_combo("muncod",array(),"N","Selecione o Estado","","","","","S") ?>
						<?php endif; ?>
					</td>
				</tr>
				<tr>
					<td colspan="2" align="center">
						<input type="button" value="Pesquisar" onclick="pesquisar();" />
						<input type="button" value="Fechar" onclick="window.close();" />
					</td>
				</tr>
			</table>
		</form>
		<?php listaEscolas() ?>
	</body>
</html>
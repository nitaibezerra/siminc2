<?php
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

if( !$_SESSION['par']['inuid'] ){
	echo '<script>
				alert("Faltam dados da sessão!");
				window.location="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore";
			</script>';
	die;
}

$mnuid = verificaAbasAtualizacao( $_SESSION['par']['inuid'] );

$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];

$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);
$mostra = 'N';

if(	in_array(PAR_PERFIL_PREFEITO,$arrayPerfil) ||
	in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,$arrayPerfil) ||
	in_array(PAR_PERFIL_EQUIPE_ESTADUAL,$arrayPerfil) ||
	in_array(PAR_PERFIL_EQUIPE_MUNICIPAL,$arrayPerfil) ||
	in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$arrayPerfil) ||
	in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) || 
	in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil)
){
	$mostra = "S";
}

//$db->cria_aba( $abacod_tela, $url, '' );
echo cabecalho();
echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td style="color: blue; font-size: 22px">
			<a href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore">';
				echo EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar, $_SESSION['par']['itrid']);
			echo '</a>';

$db->cria_aba( $abacod_tela, $url, $parametros, $mnuid );
monta_titulo( 'Atualização do PAR', 'Índices do IDEB' );

if( $_POST['requisicao'] == 'salvar' ){
	global $b;
	
	$sql = "UPDATE par.atualizacaopar SET atpconhecimentoideb = 't', atpjustificativaideb = '".$_POST['justificativa']."' WHERE inuid = ".$_SESSION['par']['inuid'];
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Operação realizada com sucesso!');
			window.location='par.php?modulo=principal/situacaoIndicadoresAtualizacaoPAR&acao=A';
		  </script>";
		die;
}

$dados = $db->pegaLinha("SELECT * FROM par.atualizacaopar WHERE inuid = ".$_SESSION['par']['inuid']);

$muncod = $db->pegaUm("SELECT muncod FROM par.instrumentounidade WHERE inuid = ".$_SESSION['par']['inuid']);

$sqlIDEB = "SELECT 
	            idbensino as intensino,
	            idbano as ano,
	            idbvlrobservado as valor,
	            idbvlrmeta as meta
			FROM public.ideb
			WHERE idbrede = 'Municipal'
			AND muncod = '".$muncod."'
			AND idbano IN ('2005', '2007', '2009', '2011', '2013')
			ORDER BY idbensino DESC, idbano";

$dadosIDEB = $db->carregar($sqlIDEB);

if( is_array($dadosIDEB) ){
	foreach( $dadosIDEB as $ideb ){
		//inicial
		if( $ideb['intensino'] == 'I' && $ideb['ano'] == 2005 ){
			$metaI2005 = $ideb['meta'];
			$valorI2005 = $ideb['valor'];
		}
		if( $ideb['intensino'] == 'I' && $ideb['ano'] == 2007 ){
			$metaI2007 = $ideb['meta'];
			$valorI2007 = $ideb['valor'];
		}
		if( $ideb['intensino'] == 'I' && $ideb['ano'] == 2009 ){
			$metaI2009 = $ideb['meta'];
			$valorI2009 = $ideb['valor'];
		}
		if( $ideb['intensino'] == 'I' && $ideb['ano'] == 2011 ){
			$metaI2011 = $ideb['meta'];
			$valorI2011 = $ideb['valor'];
		}
		if( $ideb['intensino'] == 'I' && $ideb['ano'] == 2013 ){
			$metaI2013 = $ideb['meta'];
			$valorI2013 = $ideb['valor'];
		}
		
		//Final
		if( $ideb['intensino'] == 'F' && $ideb['ano'] == 2005 ){
			$metaF2005 = $ideb['meta'];
			$valorF2005 = $ideb['valor'];
		}
		if( $ideb['intensino'] == 'F' && $ideb['ano'] == 2007 ){
			$metaF2007 = $ideb['meta'];
			$valorF2007 = $ideb['valor'];
		}
		if( $ideb['intensino'] == 'F' && $ideb['ano'] == 2009 ){
			$metaF2009 = $ideb['meta'];
			$valorF2009 = $ideb['valor'];
		}
		if( $ideb['intensino'] == 'F' && $ideb['ano'] == 2011 ){
			$metaF2011 = $ideb['meta'];
			$valorF2011 = $ideb['valor'];
		}
		if( $ideb['intensino'] == 'F' && $ideb['ano'] == 2013 ){
			$metaF2013 = $ideb['meta'];
			$valorF2013 = $ideb['valor'];
		}
	}
}

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	function salvarIDEB(){
		if($('#justificativa').val() == ''){
			alert('Justifique seu índice, apontando ações para melhoria.');
			return false;
		}
		$('#requisicao').val('salvar');
		$('#formulario').submit();
	}
</script>
<form name="formulario" id="formulario" action="" method="post">
	<input type="hidden" id="requisicao" name="requisicao">	
	<table style="background-color: #F5F5F5;" align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
	<tr>
		<td><br />
			<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="50%" cellspacing="0" cellpadding="5">
				<tr>
					<th colspan="5">Anos Iniciais do Ensino Fundamental</th>
					<th colspan="5">Anos Finais do Ensino Fundamental</th>
				</tr>
				<tr>
					<th>Ano</th>
					<th>IDEB TOTAL</th>
					<th>IDEB ESTADUAL</th>
					<th>IDEB MUNICIPAL</th>
					<th>Metas</th>
					<th>IDEB TOTAL</th>
					<th>IDEB ESTADUAL</th>
					<th>IDEB MUNICIPAL</th>
					<th>Metas</th>
				</tr>
				<tr>
					<td style="text-align: center;">2005</td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"><?=$metaI2005 > 0 ? number_format($metaI2005, 1, ",", "") : '-' ?></td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"><?=$metaF2005 > 0 ? number_format($metaF2005, 1, ",", "") : '-' ?></td>
				</tr>
				<tr>
					<td style="text-align: center;">2007</td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"><?=$metaI2007 > 0 ? number_format($metaI2007, 1, ",", "") : '-' ?></td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"><?=$metaF2007 > 0 ? number_format($metaF2007, 1, ",", "") : '-' ?></td>
				</tr>
				<tr>
					<td style="text-align: center;">2009</td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"><?=$metaI2009 > 0 ? number_format($metaI2009, 1, ",", "") : '-' ?></td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"><?=$metaF2009 > 0 ? number_format($metaF2009, 1, ",", "") : '-' ?></td>
				</tr>
				<tr>
					<td style="text-align: center;">2011</td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"><?=$metaI2011 > 0 ? number_format($metaI2011, 1, ",", "") : '-' ?></td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"></td>
					<td style="text-align: center;"><?=$metaF2011 > 0 ? number_format($metaF2011, 1, ",", "") : '-' ?></td>
				</tr>
				<!--<tr>
					<td style="text-align: left;">Ideb verificado</td>
					<td style="text-align: center;"><?=$valorI2005 > 0 ? number_format($valorI2005, 1, ",", "") : '-' ?></td>
					<td style="text-align: center;"><?=$valorI2007 > 0 ? number_format($valorI2007, 1, ",", "") : '-' ?></td>
					<td style="text-align: center;"><?=$valorI2009 > 0 ? number_format($valorI2009, 1, ",", "") : '-' ?></td>
					<td style="text-align: center;"><?=$valorI2011 > 0 ? number_format($valorI2011, 1, ",", "") : '-' ?></td>
				</tr>
			--></table>
		</td>
	</tr>
	<tr>
		<td align="center">
			<div style="width: 70%; text-align: justify;">
				<BR><h1>A qualidade da educação básica vem sendo aferida, objetivamente, com base no IDEB.
				A meta nacional para o IDEB, em 2021, nos anos iniciais do ensino fundamental, deverá chegar ao índice 6,0.
				Indique as principais ações do seu município para a melhoria do IDEB.
				</h1>
			</div>
		</td>
	</tr>
	<tr>
		<td>
			<center>
				<?
				echo campo_textarea('justificativa','S',$mostra,'Justificativa',100,10,1000,'',0,'',false,null,$dados['atpjustificativaideb']);
				?>
			</center>
		</td>
	</tr>
	<?php if( $mostra == 'S' ){ ?>
	<tr>
		<td align="center" colspan="2">
			<input type="button" name="salvar" value="Salvar" onclick="salvarIDEB()"/>
		</td>
	</tr>
	<?php } ?>
	</table>
</form>
		</td>
	</tr>
</table>
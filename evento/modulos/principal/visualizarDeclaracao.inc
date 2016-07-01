<?php
if($_POST['gerarDeclaracao']=='sim') {
	verificaSessao(true);
	$_POST['ptres'] = ($_POST['ptres']) ? $_POST['ptres'] : array();
	$ptres    = implode(',',$_POST['ptres']);
	//$elemento = implode(',',$_POST['elemento']);
	$sql = "UPDATE evento.declaracao SET decstatus = 'I' WHERE usgid='".$_SESSION['unidade']."' and copid ='".$_SESSION['copid']."'";
	$db->executar($sql);

	$sql = "INSERT INTO evento.declaracao (
   		        usucpf, usgid, copid, decconteudo, decptres)
   			VALUES ('".$_SESSION['usucpf']."','".$_SESSION['unidade']."','".$_SESSION['copid']."', '','".$ptres."') RETURNING decid;";
	$decid = $db->pegaUm($sql);
	//salvaPDF($decid);
	
	$sql = "SELECT usgid, usgdsc, usgcod FROM evento.uasg WHERE usgid = {$_SESSION['unidade']}";
	$unidade = $db->pegaLinha($sql);

	$espaco = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";

} elseif($_REQUEST['decid']) {
	$sql = "SELECT decconteudo FROM evento.declaracao WHERE decid='".$_REQUEST['decid']."'";
	echo $db->pegaUm($sql);
	exit;
}else {
	?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<form method="post" id="formulario" name="formulario">
	<input type="hidden" name="gerarDeclaracao" value="sim">
	<table class="tabela" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2">DECLARAÇÃO</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" valign="top">
				Tipo de Declaração:
			</td>
			<td>
				<select name="tipoDeclaracao" class="classe_simec">
					<option value="v">Veículos</option>
					<option value="m">Mobiliário</option>
					<option value="d">Diversos</option>
				</select>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" valign="top">
				Programa de Trabalho PTRES:
				<input type="hidden" id="ptres_campo_flag" name="ptres_campo_flag" value="0"/>
			</td>
			<td>
				<div id="ptres_campo_on">
					<? 
//	
// Alteração emergencial PTRANO = 2009. AInda não há programas de trabalho cadastrados para 2010.
// Trecho alterado (  and ptrano = '".date(Y)."'" )
//	
					
					$sql_combo = "select  pt.ptres as codigo,
								pt.ptres || ' - ' || a.acadsc || ' - ' || a.sacdsc as descricao
						from monitora.ptres pt
							inner join monitora.acao a ON a.acaid = pt.acaid
							inner join evento.uasg us on pt.unicod = us.usggestaouasg
						where  us.usgid = {$_SESSION['unidade']} and ptrstatus = 'A' and ptrano = '{$_SESSION['exercicio']}'";
					?>
					<? combo_popup( 'ptres', $sql_combo, 'Selecione o(s) PTRES(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
				</div>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td colspan="2" align="center"><input type="button" onclick="enviaForm();" value="Gerar declaração"></td>
		</tr>
	</table>
	</form>
<script type="text/javascript">
<!--
    function enviaForm(){
		var msg 	 = "";
		var ptres 	 = document.getElementById('ptres');
		var elemento = document.getElementById('elemento');
		
		if(ptres.length > 0){
			if(ptres.length == 1){
				var valor = ptres.options[0].value;
				if(valor == ""){
							msg += "O campo Programa de Trabalho PTRES é obrigatório. \n";
				}
			}
		} else {
			msg += "O campo Programa de Trabalho PTRES é obrigatório. \n";
		}
		
		if(msg != ""){
			alert(msg);
			return false;		
		}
		
		selectAllOptions( ptres );
		
		document.formulario.submit();
	}
-->
</script>
<?php
	exit;
}
?>
<html>
<head>
<meta http-equiv="Cache-Control" content="no-cache">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Expires" content="-1">

<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<style>
.titulounidade {
	background-color:#CDCDCD;
	border-top:2px solid #000000;
	border-bottom:2px solid #000000;
	text-align:center;
	font-weight: bold;
}
</style>
</head>
 <!-- onunload="window.opener.location.reload();"-->
<body onunload="window.opener.location.reload();">
	<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0">
		<tr bgcolor="#ffffff"> 
			<td align="right"  class="notprint"><a style="cursor:pointer;" onclick="window.print();"><img src="../imagens/ico_print.jpg" border="0"></a></td>
		</tr>
	</table>
	
<p align=center><img src="../imagens/brasao.gif" width="80" height="80" border="0"></p>	
<p align=center><font face="arial, sans-serif"><font size=4 style="font-size: 16pt">MINISTÉRIO DA EDUCAÇÃO</font></font></p>
<p align=center><font face="arial, sans-serif"><font size=4 style="font-size: 16pt"><u><b>DECLARAÇÃO</b></u></font></font></p>
<p class="western" align=justify><br></p>
<?php
$sql = "select copnumprocesso from evento.coprocesso
	where copid = {$_SESSION['copid']}";
$numprocesso = $db->pegaUm($sql);
if($_POST['tipoDeclaracao'] == "m"){
	
?>
<p class="western" style="text-indent: 2.54cm">
	<font face="arial, sans-serif"><u><?php echo $unidade['usgdsc']; ?></u>, código UASG nº
		<u><?php echo $unidade['usgcod']; ?></u> , vem manifestar interesse em participar do processo
		licitatório referente à aquisição do processo <?php echo  $numprocesso; ?>, através de Pregão no Sistema
		de Registro de Preços, pelo período de 12 meses, cujo Termo de Referência
		tomamos conhecimento e estamos plenamente de acordo.
	</font>
</p>
<?php
} elseif($_POST['tipoDeclaracao'] == "v") {
?>
<p class="western" style="text-indent: 2.54cm">
	<font face="arial, sans-serif"><u><?php echo $unidade['usgdsc']; ?></u>, código UASG nº
		<u><?php echo $unidade['usgcod']; ?></u> , vem manifestar interesse em participar do processo 
		licitatório referente à aquisição do processo  <?php echo  $numprocesso; ?>, através de Pregão no Sistema
		de Registro de Preços pelo período de 12 meses, cujo Termo de Referência 
		tomamos conhecimento e estamos plenamente de acordo.
	</font>
</p>
<?php
} else {
?>
<p class="western" style="text-indent: 2.54cm">
	<font face="arial, sans-serif"><u><?php echo $unidade['usgdsc']; ?></u>, código UASG nº
		<u><?php echo $unidade['usgcod']; ?></u> , vem manifestar interesse em participar do processo 
		licitatório referente à aquisição do processo  <?php echo  $numprocesso; ?>, através de Pregão no Sistema 
		de Registro de Preços, pelo período de 12 meses, cujo Termo de Referência 
		tomamos conhecimento e estamos plenamente de acordo.
	</font>
</p>
<?php
}
?>
<p class="western" align=justify style="text-indent: 2.54cm">
	<font face="Arial, sans-serif">Declaramos que os recursos orçamentários disponíveis para cobrir as despesas decorrentes da presente contratação estão </font>
	<font color="#000000"><font face="Arial, sans-serif">consignados</font></font>
	<font face="arial, sans-serif">no(s) Programa(s) de Trabalho(s): <br />
	<?php 
	$arPtres = explode(",",$ptres);
	$ptresTemp = "";
	foreach($arPtres as $ptres){
		$ptresTemp .= "'".$ptres."',";
	}
	$tamanho = strlen($ptresTemp);
	$stPtres = substr($ptresTemp,0,($tamanho-1));

//	
// Alteração emergencial PTRANO = 2009. AInda não há programas de trabalho cadastrados para 2010.
// Trecho alterado ( and ptrano = '".date(Y)."' )
//	
	$sql = "select  pt.funcod || '.' || pt.sfucod || '.' || pt.prgcod || '.' || pt.acacod || '.' || pt.loccod as progtrabalho,
					--a.acadsc || ' - ' || a.sacdsc as descricao
					a.acadsc as descricao
						from monitora.ptres pt
							inner join monitora.acao a ON a.acaid = pt.acaid
							inner join evento.uasg us on pt.unicod = us.usggestaouasg
						where  us.usgid = {$_SESSION['unidade']} and ptrstatus = 'A' and ptrano = '".date(Y)."'
						and pt.ptres in ($stPtres)";
	$arPtres = $db->carregar($sql);
	if(is_array($arPtres)){
		foreach($arPtres as $ptres){
			echo "- <u>".$ptres['progtrabalho']."</u><br />";
		}
	}
	?>
	<br /> 
	Elemento de Despesa:<br />
	<?php 
	$sql = "select ed.edpcod as codigo, ed.edpdsc as descricao 
				from evento.coprocessoelementodespesa ped
			inner join public.elementodespesa ed on ped.edpcod = ed.edpcod 
			where ped.copid = ".$_SESSION['copid'];
	$arElemento = $db->carregar($sql);

	$arElemento = ($arElemento[0] != "") ? $arElemento : array();
	
	foreach($arElemento as $elemento){
		echo "<u>".$elemento['descricao']."</u><br />";
	}
	
$coaid = pegaCoaid($_SESSION['copid'], $_SESSION['unidade']);
$sql = "select u.usunome, a.usucpf, tss.nu_matricula_siape from evento.coadesao a 
			inner join seguranca.usuario u on  a.usucpf = u.usucpf 
			inner join siape.tb_siape_cadastro_servidor tss on a.usucpf = tss.nu_cpf
			inner join seguranca.perfilusuario pu on  a.usucpf = pu.usucpf 						
		where coaid = $coaid";
$demandante = $db->pegaLinha($sql);

$sql = "select u.usunome, u.usucpf, tss.nu_matricula_siape, mun.mundescricao from evento.coadesao a 
			inner join evento.usuarioresponsabilidade ur on  a.usgid = ur.usgid and ur.pflcod = 263 and ur.rpustatus = 'A' 
			inner join seguranca.usuario u on  u.usucpf = ur.usucpf
			inner join siape.tb_siape_cadastro_servidor tss on ur.usucpf = tss.nu_cpf
			inner join territorios.municipio mun on u.muncod = mun.muncod			
		where coaid = $coaid";
$ordenador_despesa = $db->pegaLinha($sql);


?></font>
</p>
<p align=justify style="text-indent: 2.54cm; margin-top: 0.49cm; margin-bottom: 0.21cm">
<font face="Arial, sans-serif">Os itens e quantidades objeto da Ata de Registro de Preços para atendimento das necessidades desta
entidade constam na planilha anexa.</FONT></P>
<p align=justify style="text-indent: 2.54cm; margin-top: 0.49cm; margin-bottom: 0.21cm">
<font face="Arial, sans-serif">Informamos que o gestor da Ata de
Registro de Preços, nesta entidade, será o servidor <u><?php echo $demandante['usunome']; ?></u>, Matrícula SIAPE: <u><?php echo $demandante['nu_matricula_siape']; ?></u>.</font></p>
<p align=justify style="text-indent: 2.54cm; margin-top: 0.49cm; margin-bottom: 0.21cm">
<font face="arial, sans-serif">Informamos, por fim, que o endereço
para entrega dos matériais a serem adquiridos é o seguinte: </font></p>
<p align=justify style="text-indent: 2.54cm; margin-top: 0.49cm; margin-bottom: 0.21cm">
<font face="arial, sans-serif">
ENDEREÇO(S): <br />
<?php 
$sql = "select 
			ee.coeid,
			ee.coenddsc  as nome,
			ee.coendlog  || ', ' || ee.coendnum || ' ' || ee.coendcom || ', ' || ee.coendbai || ', ' || m.mundescricao || ' - ' || m.estuf as endereco,
			c.coidsc,
			ci.cotid
		FROM evento.coenderecoentrega  ee 
			inner join evento.coadesao a on a.coaid = ee.coaid
			inner join evento.codemandaitem di on ee.coeid = di.coeid
			inner join evento.coitemprocesso as ci on ci.cotid = di.cotid
			inner join evento.coitem as c on c.coiid = ci.coiid
			inner join territorios.municipio m on ee.muncod = m.muncod
		where a.copid = ". $_SESSION['copid'] ." and a.usgid = ". $_SESSION['unidade'];
$arDados = $db->carregar( $sql );

if(is_array($arDados)){
	$arTemp = array();
	foreach($arDados as $dados){
		$nome = $dados['nome'];
		$endereco = $dados['endereco'];
		$coidsc = $dados['coidsc'];
		$arTemp[$nome.' - '.$endereco][$coidsc] = $dados;
	}
	
	$count = 1;
	$count2 = 1;
	foreach($arTemp as $end=>$arItem){
		echo $espaco.$count.'. '.$end."<br />";
		foreach($arItem as $item=>$dados){
			$sql = "select cdiqtde as quantidade from evento.codemandaitem where coaid = {$coaid} and cotid = {$dados['cotid']} and coeid = {$dados['coeid']}";
			$quantidade = $db->pegaUm($sql);
			echo $espaco."&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".$count2.' - '.$item.' - '. $quantidade ."<br />";
			$count2++;
		}
		$count++;
		$count2 = 1;
	}
}
?>
</font></p>
<p align=left style="text-indent: 6.35cm; margin-top: 0.49cm; margin-bottom: 0.21cm">
<font face="arial, sans-serif"><?php echo $ordenador_despesa['mundescricao']; ?>, <?php echo date ("d/m/Y"); ?> </font></p><br><br>
<p align=center><font face="arial, sans-serif"><b><?php echo $ordenador_despesa['usunome']; ?></b></font></p>
<p align=center ><font face="arial, sans-serif"><b>CPF: <?php echo $ordenador_despesa['usucpf']; ?></b></font></p>
<p align=center ><font face="arial, sans-serif"><b>Matrícula SIAPE: <?php echo $ordenador_despesa['nu_matricula_siape']; ?></b></font></p>
<p align=left style="margin-top: 0.49cm; margin-bottom: 0.21cm"><br></p>
<p align=center ><font face="arial, sans-serif"><b>Autenticação do Documento</b></font></p>
<p align=center ><font face="arial, sans-serif"><b>Número: <?php echo $decid.date (".Ymd.His"); ?></b></font></p>
<p align=left style="margin-top: 0.49cm; margin-bottom: 0.21cm"><font face="arial, sans-serif"><b>IMPORTANTE</b></font></p>
<p align=justify><font face="arial, sans-serif">A) ESTA DECLARAÇÃO
DEVERÁ SER IMPRESSA EM PAPEL TIMBRADO DA INSTITUIÇÃO E ASSINADA
PELO SEU TITULAR OU PELO ORDENADOR DE DESPESAS, SENDO ENDEREÇADA
PARA:</font></p>
<p class="western" align=justify style="text-indent: 0.64cm"><font face="arial, sans-serif"><b>Valeria Grilanda Rodrigues Paiva</b></font></p>
<p class="western" align=justify style="text-indent: 0.64cm"><font face="arial, sans-serif"><b>Subsecretária
de Assuntos Administrativos</b></font></p>
<p class="western" align=justify style="text-indent: 0.64cm"><font face="arial, sans-serif"><b>Esplanada 
dos Ministérios - Bloco L - Anexo I - Sala 300
CEP: 70.047-900 - Brasília/DF
</b></font></p>
<p class="western"><font face="arial, sans-serif">B) FAVOR
TRANSMITÍ-LA POR FAX PARA O NÚMERO (61) 2022-7000 E, EM SEGUIDA,
ENCAMINHAR A VIA ORIGINAL, POR SEDEX PARA O MEC.</font></p>
</body>
</html>
<?php
	$sql = "UPDATE evento.declaracao SET decconteudo='".addslashes(ob_get_contents())."' WHERE decid = $decid";
	$db->executar($sql);
	$db->commit();
	ob_end_clean();
	echo "<script>
			window.location= '?modulo=principal/visualizarDeclaracao&acao=A&decid=".$decid."';
		  </script>";
?>
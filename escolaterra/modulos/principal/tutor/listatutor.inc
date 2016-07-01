<?php

include "_funcoes_tutor.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/escolaterra.js"></script>';

echo "<br>";

?>
<script>

function acessarTutor(iusid) {
	window.location='escolaterra.php?modulo=principal/tutor/listatutor&acao=A&iusid='+iusid+'&requisicao=carregarTutor&direcionar=true';
}

</script>
<form method="post" name="formulario" id="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita">CPF</td>
	<td><? echo campo_texto('iuscpf', "S", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, '', '', $_REQUEST['iuscpf'] ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Nome</td>
	<td><? echo campo_texto('iusnome', "S", "S", "Nome", 67, 150, "", "", '', '', 0, '', '', $_REQUEST['iusnome'] ); ?></td>
</tr>
<tr>
     <td class="SubTituloDireita" width="25%">Situação cadastramento</td>
     <td>
     <?
     $sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='".TPD_CADASTRAMENTO."' ORDER BY esdordem";
	$db->monta_combo('esdid', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'esdid', false, $_REQUEST['esdid']);
        	
    ?>        	
    </td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Todos" onclick="divCarregando();window.location='escolaterra.php?modulo=principal/tutor/listatutor&acao=A';"></td>
</tr>
</table>
</form>

<?

$perfis = pegaPerfilGeral();

if(in_array(PFL_COORDENADORESTADUAL,$perfis)) {
	if($_SESSION['escolaterra']['coordenadorestadual']['ufpid']) {
		$wh[] = "i.ufpid = '".$_SESSION['escolaterra']['coordenadorestadual']['ufpid']."'";
	} else {
		$wh[] = "1=2";
	}
}


if($_REQUEST['iusnome']) {
	$wh[] = "i.iusnome ilike '%".$_REQUEST['iusnome']."%'";
}

if($_REQUEST['iuscpf']) {
	$wh[] = "i.iuscpf='".str_replace(array(".","-"),array("",""),$_REQUEST['iuscpf'])."'";
}

if($_REQUEST['esdid']) {
	$wh[] = "e.esdid='".$_REQUEST['esdid']."'";
}

$sql = "SELECT '<center><img src=\"../imagens/alterar.gif\" border=0 style=\"cursor:pointer;\" onclick=\"acessarTutor(\''||i.iusid||'\');\"></center>' as acao1,
			   u.estuf as estado,
			   replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') as cpf,
			   i.iusnome as nome,
			   i.iusemailprincipal as email,
			   COALESCE(e.esddsc,'Não iniciado') as situacaocadastramento 
		FROM escolaterra.identificacaousuario i 
		INNER JOIN escolaterra.tipoperfil t ON i.iusid=t.iusid 
		INNER JOIN escolaterra.ufparticipantes u ON u.ufpid = i.ufpid  
		LEFT JOIN escolaterra.turmas tu          ON tu.iusid = i.iusid 
		LEFT JOIN workflow.documento d           ON d.docid = tu.docid 
		LEFT JOIN workflow.estadodocumento e     ON e.esdid = d.esdid 
		WHERE iusstatus='A' AND t.pflcod='".PFL_TUTOR."' ".(($wh)?"AND ".implode(" AND ",$wh):"");

$cabecalho = array("&nbsp;","UF","CPF","Nome","Email","Situação do cadastramento");
$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);

?>
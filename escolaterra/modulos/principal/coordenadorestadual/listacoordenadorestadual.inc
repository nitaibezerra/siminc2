<?php

include "_funcoes_coordenadorestadual.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sispacto.js"></script>';

echo "<br>";

?>
<script>

function gerenciarCoordenadorEstadual(ufpid) {
	window.open('escolaterra.php?modulo=principal/coordenadorestadual/gerenciarcoordenadorestadual&acao=A&ufpid='+ufpid,'Gerenciar','scrollbars=no,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');	
}

function acessarCoordenadorEstadual(ufpid) {
	window.location='escolaterra.php?modulo=principal/coordenadorestadual/listacoordenadorestadual&acao=A&ufpid='+ufpid+'&requisicao=carregarCoordenadorEstadual&direcionar=true';
}

</script>
<?

$sql = "SELECT CASE WHEN foo.iusidcoordenadorestadual IS NOT NULL THEN '<center><img src=\"../imagens/alterar.gif\" border=0 style=\"cursor:pointer;\" onclick=\"acessarCoordenadorEstadual(\''||foo.ufpid||'\',\'\',this);\"></center>' 
			        ELSE '&nbsp;' END as acao1,
			   '<center><img src=\"../imagens/usuario.gif\" border=\"0\" onclick=\"gerenciarCoordenadorEstadual(\''||foo.ufpid||'\');\" style=\"cursor:pointer;\"></center>' as acao2, 
			   foo.estuf as estado,
			   COALESCE(replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') || ' / ' || i.iusnome,'<span style=color:red>Coordenador Estadual não cadastrado</span>') as coordenadorestadual,
			   COALESCE(e.esddsc,'<span style=color:red>Cadastramento não iniciado</span>') as sitcadastramento,
			   (SELECT COUNT(DISTINCT i.iusid) FROM escolaterra.identificacaousuario i INNER JOIN escolaterra.tipoperfil t ON i.iusid=t.iusid WHERE i.ufpid=foo.ufpid AND t.pflcod=".PFL_TUTOR.") as qtdtutores
		FROM (
		SELECT u.ufpid,
			   u.estuf,
			   (SELECT i.iusid FROM escolaterra.identificacaousuario i INNER JOIN escolaterra.tipoperfil t ON i.iusid=t.iusid WHERE i.ufpid=u.ufpid AND t.pflcod=".PFL_COORDENADORESTADUAL.") as iusidcoordenadorestadual
		FROM escolaterra.ufparticipantes u 
		WHERE ufpstatus='A'
		) foo 
		LEFT JOIN escolaterra.identificacaousuario i ON i.iusid = foo.iusidcoordenadorestadual
		LEFT JOIN escolaterra.turmas t               ON t.iusid = i.iusid 
		LEFT JOIN workflow.documento d               ON d.docid = t.docid 
		LEFT JOIN workflow.estadodocumento e         ON e.esdid = d.esdid  
		ORDER BY foo.estuf";

$cabecalho = array("&nbsp;","&nbsp;","UF","Coordenador Estadual","Situação Cadastramento Tutores","Qtd Tutores");
$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);

?>
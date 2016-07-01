<?
include "_funcoes_universidade.php";


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sisindigena.js"></script>';

echo "<br>";

monta_titulo( "Lista - Universidades", "Lista das universidades participantes");

?>

 <script>

function gerenciarCoordenadorIES(uncid) {
	window.open('sisindigena.php?modulo=principal/universidade/gerenciarcoordenadories&acao=A&uncid='+uncid,'CoordenadorIES','scrollbars=no,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');	
}

</script>
<?

$sql = "SELECT CASE WHEN foo.coordenadories='Coordenador IES não cadastrado' THEN '&nbsp;' ELSE foo.acao1 END as acao1,
			   foo.acao2,
			   foo.uninome,
			   foo.estuf,
			   foo.mundescricao,
			   foo.coordenadories,
			   foo.situacao,
			   foo.reinome
		FROM(
		SELECT '<center><img src=\"../imagens/alterar.gif\" border=0 style=\"cursor:pointer;\" onclick=\"window.location=\'sisindigena.php?modulo=principal/universidade/universidade&acao=A&uncid='||u.uncid||'&requisicao=carregarCoordenadorIES&direcionar=true\';\"></center>' as acao1,
			   '<center><img src=\"../imagens/usuario.gif\" border=\"0\" onclick=\"gerenciarCoordenadorIES(\''||u.uncid||'\');\" style=\"cursor:pointer;\"></center>' as acao2,
				re.reinome,
				su.uninome,
				COALESCE((SELECT iusnome FROM sisindigena.identificacaousuario i INNER JOIN sisindigena.tipoperfil t ON i.iusd=t.iusd WHERE i.uncid=u.uncid AND t.pflcod=".PFL_COORDENADORIES."),'Coordenador IES não cadastrado') as coordenadories,
				COALESCE(esd.esddsc,'Não iniciou Elaboração') as situacao,
				su.uniuf as estuf,
				m.muncod,
				m.mundescricao,
				esd.esdid 
		FROM sisindigena.universidadecadastro u 
		INNER JOIN sisindigena.universidade su ON su.uniid = u.uniid
		LEFT JOIN sisindigena.reitor re on re.uniid = su.uniid 
		LEFT JOIN workflow.documento doc on doc.docid = u.docid 
		LEFT JOIN workflow.estadodocumento esd on esd.esdid = doc.esdid
		LEFT JOIN territorios.municipio m ON m.muncod = su.muncod 
		WHERE u.uncstatus='A'
		) foo
		ORDER BY foo.uninome";

$cabecalho = array("&nbsp;","&nbsp;","Universidade","UF","Município","Coordenador IES","Situação","Dirigente");
$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);
?>